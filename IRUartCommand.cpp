
#include "IRUartCommand.h"

namespace BOMEANS {

	IRUartCommand::IRUartCommand(void) {
		mCommandID = UNKNOWN_COMMAND_ID;
		mVersionCode = VERSION_CODE;
		mpCmdBytes = nullptr;
		mCmdLength = 0;
		mpPayload = nullptr;
		mPayloadLength = 0;
	}

	IRUartCommand::~IRUartCommand(void) {
		Clear();
	}

	void IRUartCommand::Clear(void) {
		if (mpCmdBytes != nullptr) {
			delete[] mpCmdBytes;
			mpCmdBytes = nullptr;
		}
		mCmdLength = 0;

		if (mpPayload != nullptr) {
			delete[] mpPayload;
			mpPayload = nullptr;
		}
		mPayloadLength = 0;

		mVersionCode = VERSION_CODE;
		mCommandID = UNKNOWN_COMMAND_ID;
	}

	bool IRUartCommand::Load(uint8_t commandId, uint8_t* pPayloadData, uint32_t payloadLength) {
		
		Clear();

		/*if ((pPayloadData == NULL) || (payloadLength == 0)) {
			return false;
		}*/

		mCommandID = commandId;

		// copy payload bytes
		if (nullptr != mpPayload) {
			delete[] mpPayload;
			mpPayload = nullptr;
		}
		mPayloadLength = payloadLength;
		mpPayload = new uint8_t[mPayloadLength];
		for (int i = 0; i < mPayloadLength; i++) {
			mpPayload[i] = pPayloadData[i];
		}

		// filling command data bytes

		mCmdLength = mPayloadLength + 8;
		mpCmdBytes = new uint8_t[mCmdLength];
		
		// prefix
		mpCmdBytes[0] = PREFIX0_CODE;
		mpCmdBytes[1] = PREFIX1_CODE;

		// version
		mpCmdBytes[2] = VERSION_CODE;

		// length
		uint16_t length = mPayloadLength + 2; // command (1-byte) + payload length + checksum (1-byte)
		mpCmdBytes[3] = (uint8_t)(length & 0xFF);
		mpCmdBytes[4] = (uint8_t)((length >> 8) & 0xFF);

		// command
		mpCmdBytes[5] = mCommandID;

		// data/payload
		for (int i = 0; i < mPayloadLength; i++) {
			mpCmdBytes[6 + i] = mpPayload[i];
		}

		// checksum
		uint32_t checksum = 0;
		for (int i = 2; i < 6 + mPayloadLength; i++) {
			checksum += mpCmdBytes[i];
		}
		checksum &= 0xFF;
		mpCmdBytes[6 + mPayloadLength] = (uint8_t)checksum;

		// postfix
		mpCmdBytes[7 + mPayloadLength] = POSTFIX_CODE;

		return true;
	}

	bool IRUartCommand::Load(uint8_t* pCommandData, uint32_t commandLength) {
		
		Clear();

		if (commandLength < 8) {
			return false;
		}

		// prefix & postfix
		if ((pCommandData[0] != PREFIX0_CODE) || (pCommandData[1] != PREFIX1_CODE)) {
			return false;
		}

		// version
		mVersionCode = pCommandData[2];

		// length
		uint16_t length = (pCommandData[3] & 0xff) + (((uint16_t)(pCommandData[4] & 0xff)) << 8);   // command (1-byte) + data length + checksum (1-byte)
		if (length + 6 > commandLength) { //if (length + 6 != cmdBytes.length)   // allow passed in data has more bytes than a full package
			return false;
		}
		uint16_t payloadLength = length - 2;

		// command ID
		mCommandID = pCommandData[5];

		// copy command bytes
		int fullCmdLen = length + 6;	// include prefix etc
		mpCmdBytes = new uint8_t[fullCmdLen];
		for (int i = 0; i < fullCmdLen; i++) {
			mpCmdBytes[i] = pCommandData[i];
		}

		// payload data
		if (payloadLength > 0) {
			if (nullptr != mpPayload) {
				delete[] mpPayload;
				mpPayload = nullptr;
			}
			mPayloadLength = payloadLength;
			mpPayload = new uint8_t[mPayloadLength];
			for (int i = 0; i < mPayloadLength; i++) {
				mpPayload[i] = mpCmdBytes[6 + i];
			}
		}

		// checksum & postfix
		uint32_t checksum = 0;
		for (int i = 2; i < 6 + mPayloadLength; i++) {
			checksum += pCommandData[i];
		}
		checksum &= 0xFF;
		if ((pCommandData[6 + mPayloadLength] != (uint8_t)checksum) ||
			(pCommandData[7 + mPayloadLength] != POSTFIX_CODE))
		{
			Clear();
			return false;
		}

		// copy the data bytes
		if (mpCmdBytes != nullptr) {
			delete[] mpCmdBytes;
			mpCmdBytes = nullptr;
		}
		mCmdLength = 8 + mPayloadLength;
		mpCmdBytes = new uint8_t[mCmdLength];
		for (int i = 0; i < mCmdLength; i++) {
			mpCmdBytes[i] = pCommandData[i];
		}

		return true;
	}

	bool IRUartCommand::IsValid() {
		return mCommandID != UNKNOWN_COMMAND_ID;
	}

	uint8_t IRUartCommand::GetCommandID() {
		return mCommandID;
	}

	uint16_t IRUartCommand::GetPayloadLength() {
		return mPayloadLength;
	}

	bool IRUartCommand::GetPayload(uint8_t* pBuffer, uint16_t bufferLength) {

		if ((nullptr == pBuffer) || (bufferLength != mPayloadLength)) {
			return false;
		}

		for (int i = 0; i < mPayloadLength; i++) {
			pBuffer[i] = mpPayload[i];
		}
		return true;
	}

    const uint8_t* IRUartCommand::GetPayLoad() const
    {
        return mpPayload;
    }

    
	uint16_t IRUartCommand::GetCommandLength() {
		return mCmdLength;
	}

	bool IRUartCommand::GetCommandBytes(uint8_t* pBuffer, uint16_t bufferLength) {

		if ((pBuffer == nullptr) || (bufferLength != mCmdLength)) {
			return false;
		}

		for (int i = 0; i < mCmdLength; i++) {
			pBuffer[i] = mpCmdBytes[i];
		}

		return true;
	}

    const uint8_t* IRUartCommand::GetCommandBytes() const
    {
        return mpCmdBytes;
    }
    
	uint8_t IRUartCommand::GetVersionCode() {
		return mVersionCode;
	}
}
