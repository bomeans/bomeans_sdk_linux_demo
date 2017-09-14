

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <pthread.h>
#include <curl/curl.h>
#include <BomeansIRSDK.h>
#include <deque>
#include "CommandReadBuffer.h"
#include "IRUartCommand.h"


// fill up your API Key applied from Bomeans Design.
std::string APIKEY="your-api-key";

// make sure you the serial device is properly connected and set as accessible
#define UART_DEVICE "/dev/ttyUSB0"

BOMEANS::CommandReadBuffer gMyReadBuffer;
BOMEANS_LIB_NAME::BIRReceiveDataCallback* mpMyCallback;

/*
This class handles all the communication between the SDK and the underlying hardware.
 (1) for out data from SDK: SDK invokes sendData() with the raw data which should be passed to MCU transparently.
 (2) for in(returned) data to SDK: User receives the data sent out from the MCU in his own way, and pass it back
     to the SDK by calling the BIRReceiveDataCallback::onDataReceived() with the received raw data as parameter.

Note this callback is passed to the user from SDK by invoking setReceiveDataCallback(). Whenever setReceiveDataCallback()
is invoked by the SDK, user should keep this callback in order to pass data back to SDK. 
This callback could be null. In such cases, the returned data is just dropped since the SDK is either not handling the returned
data from MCU or not expecting there is returned data.

User should write his own class implements BOMEANS_LIB_NAME::BIRIRBlaster interface to fit his own hardware setup, such as
passing the data via WiFi or Zigbee to the MCU.
*/
class MYIRBlaster : public BOMEANS_LIB_NAME::BIRIRBlaster
{ 
private:
	int mfd = -1;
	const char* SERIAL_DEV = UART_DEVICE;
	bool isDeviceConnected = false;

	bool connect()
	{
		mfd = open(SERIAL_DEV, O_RDWR | O_NOCTTY | O_NONBLOCK);
		if (mfd < 0)
		{
			printf("error opening %s\n", SERIAL_DEV);
			isDeviceConnected = false;
			return false;
		}
		else
		{
			printf("%s connected\n", SERIAL_DEV);
		}

		// connect com port
		struct termios oldtio;
	    struct termios newtio;
	    tcgetattr(mfd, &oldtio);
	    bzero(&newtio, sizeof(newtio));

	    newtio.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
	    newtio.c_iflag = IGNPAR;
	    newtio.c_oflag = 0;
	    newtio.c_lflag = 0;

	    tcflush(mfd, TCIFLUSH);
	    tcsetattr(mfd, TCSANOW, &newtio);


		pthread_t serialReadThread_t;
 
 		pthread_create(&serialReadThread_t, nullptr, serialReadThread, (void *)&mfd);

 		isDeviceConnected = true;
	    return true;
	}

	// This thread keep running to receive the bytes from the serial port.
	// Once a complete packet is received, pass it back to the SDK by calling the callback function provided by the SDK.
	static void* serialReadThread(void* parameters)
	{
		uint8_t readBuff[2048];
		int fd = *((int*)parameters);

		struct timeval timeout;
		ssize_t len;
		fd_set v_Rfds;
		int retval;
		int bytesToBeRead;
		int bytesRead;
		int i;

		while (true)
		{
			memset(readBuff, 0, MAX_READ_BUFFER_SIZE);

			FD_ZERO(&v_Rfds);
		    FD_SET(fd, &v_Rfds);
		    timeout.tv_sec = 1;
		    timeout.tv_usec = 0;

		    retval = select((fd + 1), &v_Rfds, nullptr, nullptr, &timeout);
			bytesToBeRead = 0;
	    	bytesRead = 0;

		    if (retval > 0)
		    {
		    	// try to wait for at least 8 bytes are ready
			    for (i = 0; i < 100; i++)
			    {
			    	ioctl(fd, FIONREAD, &bytesToBeRead);
			    	if (bytesToBeRead >= 8)
			    	{
			    		break;
			    	}
			    	usleep(100);	// 0.1ms
			    } 
		    }

		    bytesRead = read(fd, readBuff, bytesToBeRead);
		    gMyReadBuffer.push(readBuff, bytesRead);


		    bytesRead = MAX_READ_BUFFER_SIZE;
		    if (gMyReadBuffer.getValidPacket(readBuff, &bytesRead))
		    {
		    	// debug output
		    	printf("\n\n<<<Received %d bytes: ", bytesRead);

		    	for (int i = 0; i < bytesRead; i++)
		    	{
		    		printf("%02X,", readBuff[i]);
		    	}
		    	printf("\n");

		    	// passing back the packet to the SDK for handling
		    	if (mpMyCallback != nullptr)
		    	{
		    		mpMyCallback->onDataReceived(std::vector<uint8_t>(readBuff, readBuff + bytesRead));
		    	}
		    }
		    
		}

		return NULL;
	}


public:

	MYIRBlaster()
	{
		connect();
	}

	~MYIRBlaster()
	{
		close(mfd);
	}

	// When there are data to be sent to the underlying hardware (Bomeans MCU), SDK will
	// call this function with the data as the parameter.
	virtual int sendData(const BomArray<uint8_t>& irUartData)
	{
		const uint8_t* pIrUartData = &irUartData[0];
		int irUartDataLen = irUartData.size();

		printf(">>>sendData received %d bytes: ", irUartDataLen);
		for (int i = 0; i < irUartDataLen; i++)
		{
			printf("%02X,", pIrUartData[i]);
		}
		printf("\n");

		if (isConnection())
		{
			// pass the data to the serial port.
			if (mfd > 0)
			{
				
				int ret = write(mfd, pIrUartData, irUartDataLen);
				if (ret < 0)
				{
					printf("*** Send UART data failed!\n");
				}
				else
				{
					printf("*** Send UART data succeeded\n");
				}
			}
		}
		else
		{
			printf("*** No serial port connection, data not sent!\n");
		}


		return  BIRError::BIRNoError;
	}

	// return true if the underlying hw is connected, or false otherwise.
	virtual int isConnection()
	{
		return isDeviceConnected;
		//return true;	// assume the hardware is always connected.
	}

	// This is where the SDK pass the callback function for the underyling hw to send back data to the SDK.
	// Keep the pasisng callback function, and whenere there are data to be sent back to SDK, call the 
	// onDataReceived() of the passing callback.
	virtual void setReceiveDataCallback(BOMEANS_LIB_NAME::BIRReceiveDataCallback* pCallback)
	{
		//printf("setReceiveDataCallback is invoked\n");
		mpMyCallback = pCallback;
	}

	void sendDummy()
	{
		if (NULL != mpMyCallback)
		{
			BomArray<uint8_t> v = {
				0xff, 0x61, 0x00, 0x54, 0x00, 0x92, 0x09, 0x86, 
				0x2c, 0x06, 0x08, 0x20, 0xc0, 0x00, 0x90, 0x03, 
				0xd4, 0x08, 0x0c, 0x03, 0x00, 0x0b, 0x1c, 0x05, 
				0xa0, 0x05, 0x48, 0x07, 0xcc, 0x35, 0x18, 0x04, 
				0x5d, 0xa7, 0x6b, 0x99, 0xcc, 0x07, 0x00, 0x10, 
				0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 
				0xa0, 0xb0, 0x48, 0x00, 0x10, 0x32, 0x44, 0x65, 
				0x07, 0x26, 0x82, 0x25, 0x92, 0x10, 0x32, 0x44, 
				0x65, 0x07, 0x62, 0x82, 0x25, 0x92, 0x10, 0x32, 
				0x44, 0x65, 0x07, 0x62, 0x82, 0x25, 0xa2, 0x10, 
				0x32, 0x44, 0x65, 0x07, 0xb3, 0x82, 0x25, 0x92, 
				0xe6, 0xf0
			};
			mpMyCallback->onDataReceived(v);
		}
	}
};

bool gLearningCompletedFlag = false;

// callback for startLearningAndGetData()
class MyLearningCallback : public BOMEANS_LIB_NAME::ReaderFormatMatchCallback
{
public:
	void onFormatMatchSucceeded(const BOMEANS_LIB_NAME::ReaderMatchResult &formatMatchResult)
	{
		printf("onFormatMatchSucceeded\n");
		printf("format id: %s, c-code: 0x%X, k-code: 0x%X\n",
			formatMatchResult.formatId.c_str(), formatMatchResult.customCode, formatMatchResult.keyCode);

		gLearningCompletedFlag = true;
	}

	void onFormatMatchFailed(BOMEANS_LIB_NAME::FormatParsingErrorCode::V errorCode)
	{
		printf("onFormatMatchFailed: %d\n", errorCode);

		gLearningCompletedFlag = true;
	}

	void onLearningDataReceived( const BomArray<uint8_t> &learningData) 
	{
		printf("onLearningDataReceived\n");
	}

	void onLearningDataFailed(BOMEANS_LIB_NAME::LearningErrorCode::V errorCode) 
	{
		printf("onLearningDataFailed: %d\n", errorCode);
	}
};

// callback for startLearningAndGetData()
class MyLearningCallback2 : public BOMEANS_LIB_NAME::ReaderRemoteMatchCallback
{
public:
	void onRemoteMatchSucceeded(const ArrayList<BOMEANS_LIB_NAME::Web::RemoteUID> &remoteMatchResultList)
	{
		printf("onRemoteMatchSucceeded\n");
		for (int i = 0; i < remoteMatchResultList.size(); i++)
		{
			printf("Type: %s, Brand: %s, Model: %s\n", 
				remoteMatchResultList[i].TypeID.c_str(), remoteMatchResultList[i].BrandID.c_str(), remoteMatchResultList[i].ModelID.c_str());
		}
		printf("\n");

		gLearningCompletedFlag = true;
	}

	void onRemoteMatchFailed(BOMEANS_LIB_NAME::CloudMatchErrorCode::V errorCode)
	{
		printf("onRemoteMatchFailed: %d\n", errorCode);

		gLearningCompletedFlag = true;
	}

	void onFormatMatchSucceeded(const ArrayList<BOMEANS_LIB_NAME::ReaderMatchResult>& formatMatchResultList)
	{
		printf("onFormatMatchSucceeded\n");
		if (formatMatchResultList.size() > 0)
		{
			for (int i = 0; i < formatMatchResultList.size(); i++)
			{
				printf("format id: %s, c-code: 0x%X, k-code: 0x%X\n",
					formatMatchResultList[i].formatId.c_str(), formatMatchResultList[i].customCode, formatMatchResultList[i].keyCode);
			}
		}
	}

	void onFormatMatchFailed(BOMEANS_LIB_NAME::FormatParsingErrorCode::V errorCode)
	{
		printf("onFormatMatchFailed: %d\n", errorCode);
	}
};


MYIRBlaster& getMyIRBlaster()
{
	static MYIRBlaster irBlaster;
	return irBlaster;
}


int main()
{
	USING_BOMEANS

	// The SDK required curl library, for accessing the web APIs
	curl_global_init(CURL_GLOBAL_ALL);
	
	// setup API Key
	IRKit::setup(APIKEY, false, nullptr);

	// setup the underlying hardware
	IRKit::setIRHW(&getMyIRBlaster());

	// test SDK: get types
	BomArray<Web::TypeItem> typeItems;
	bool r = Web::getTypeList("tw", false, typeItems, nullptr);
	printf("\nTypes:\n");
	for (Web::TypeItem typeItem : typeItems)
	{
		printf("%s (%s, %s)\n", typeItem.Id.c_str(), typeItem.LocalizedName.c_str(), typeItem.EnglishName.c_str());
	}	

	// test SDK: get brands	
	BomArray<Web::BrandItem> brandItems;
	Web::getTopBrandList(typeItems[0].Id, 0, 10, "tw", false, brandItems, nullptr);
	printf("\nBrands:\n");
	for (Web::BrandItem brandItem : brandItems)
	{
		printf("%s (%s, %s)\n", brandItem.Id.c_str(), brandItem.LocalizedName.c_str(), brandItem.EnglishName.c_str());
	}

	// test SDK: get models
	BomArray<Web::ModelItem> modelItems;
	Web::getRemoteModelList(typeItems[0].Id, brandItems[1].Id, false, modelItems, nullptr);
	printf("\nModels (Remote IDs):\n");
	for (Web::ModelItem modelItem : modelItems)
	{
		printf("%s (%s)\n", modelItem.Id.c_str(), modelItem.MachineModel.c_str());
	}

	// test SDK: create remote - TV
	brandItems.clear();
	Web::getTopBrandList("1", 0, 10, "tw", false, brandItems, nullptr);
	Web::getRemoteModelList("1", brandItems[1].Id, false, modelItems, nullptr);
	auto tvRemote = IRKit::createRemote("1",
								 brandItems[1].Id,
								 modelItems[0].Id, false, nullptr);
	printf("Create Remote: type: %s, brand: %s, model: %s\n", "1", brandItems[1].Id.c_str(), modelItems[0].Id.c_str());

	printf("Supported Keys:\n");
	auto keys = tvRemote->getAllKeys();
	for (auto key : keys)
	{
		printf("%s\n", key.c_str());
	}

	printf("Send POWER Key\n");
	tvRemote->transmitIR("IR_KEY_POWER_TOGGLE", "");
	delete tvRemote;

	sleep(5);

	// test SDK: create remote - AC
	brandItems.clear();
	Web::getTopBrandList("2", 0, 10, "tw", false, brandItems, nullptr);
	Web::getRemoteModelList("2", brandItems[1].Id, false, modelItems, nullptr);
	auto remote = IRKit::createRemote("2",
								 brandItems[1].Id,
								 modelItems[0].Id, false, nullptr);
	printf("Create Remote: type: %s, brand: %s, model: %s\n", "2", brandItems[1].Id.c_str(), modelItems[0].Id.c_str());

	printf("Supported Keys:\n");
	keys = remote->getAllKeys();
	for (auto key : keys)
	{
		printf("%s\n", key.c_str());
	}

	// test SDK: send IR
	for (auto key : keys)
	{
		if (strcmp(key.c_str(), "IR_ACKEY_POWER") == 0)
		{
			printf("Send POWER Key\n");
			remote->transmitIR("IR_ACKEY_POWER", "IR_ACOPT_POWER_ON");
		}
	}

	for (auto key : keys)
	{
		if (strcmp(key.c_str(), "IR_ACKEY_MODE") == 0)
		{
			printf("Send MODE Key\n");
			remote->transmitIR("IR_ACKEY_MODE", "");
		}
	}

	
	// need manually delete the created remote
	delete remote;

	sleep(1);

	//------------------------------------
	// test SDK: IR Learning
	//------------------------------------
	MyLearningCallback myCallback;
	MyLearningCallback2 myCallback2;

	auto reader = IRKit::createIRReader(false);

	gLearningCompletedFlag = false;

	printf("*** Entering IR Learning Mode and waiting for IR Signal...\n");

	// return the parsed format data and the matched remote controller IDs if any
	reader->startLearningAndSearchCloud(true, PREFER_REMOTE_TYPE::Auto, &myCallback2);

	// return the raw IR packet data and the parsed format data in the callback
	//reader->startLearningAndGetData(PREFER_REMOTE_TYPE::Auto, &myCallback);

	// debug. simulate the returned IR raw data from the hardware, if no physical IR hardware is connected for IR learning
	//getMyIRBlaster().sendDummy();

	int counter = 0;
	while (!gLearningCompletedFlag && counter++ < 20000)
	{
		//Sleep(1000);
		usleep(1000);
	}

	// need to manually delete the created IRReader instance
	delete reader;

	curl_global_cleanup();
}


/// g++ -c main.cpp -lcurl -I../ -std=c++11
/// g++  main.o *.a  -lcurl -lstdc++ -std=c++11
