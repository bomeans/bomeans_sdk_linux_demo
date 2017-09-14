#include <stdio.h>
#include <string.h>
#include "CommandReadBuffer.h"
#include "IRUartCommand.h"

namespace BOMEANS
{
	void CommandReadBuffer::reset()
	{
		memset(mBuff, 0, MAX_READ_BUFFER_SIZE);
		mReadPointer = 0;
		mWritePointer = 0;
	}

	// get free size in buffer
	int CommandReadBuffer::getAvailableSize()
	{
		return getFullSize() - getFullness();
	}

	// get the fullness of the buffer
	int CommandReadBuffer::getFullness()
	{
		int dataSize = 0;
		if (mWritePointer > mReadPointer)
		{
			dataSize = mWritePointer - mReadPointer;
				
		}
		else if (mWritePointer < mReadPointer)
		{
			dataSize = getFullSize() - mReadPointer + mWritePointer;
		}
		else	// ==
		{
			dataSize = 0;//getFullSize();
		}

		return dataSize;
	}

	// get full buffer size
	int CommandReadBuffer::getFullSize()
	{
		return MAX_READ_BUFFER_SIZE;
	}
	
	// check (and sync) the buffer if a valid packet has arrived.
	// A valid packet here means the packet is in the format of Bomeans UART command format.
	// (FF, 61, 00, ....., F0)
	bool CommandReadBuffer::hasValidPacket()
	{
		int dataSize = getFullness();
		uint8_t* pData = new uint8_t[dataSize];
		trypop(pData, dataSize);

		IRUartCommand uartCmd;
		bool result = uartCmd.Load(pData, dataSize);

		delete[] pData;

		return result;

		/*
		if (getFullness() > 0)
		{
			bool isSync = false;
			int currentIdx;
			while (!isSync)
			{
				// check prefix 0xFF
				while (mBuff[mReadPointer] != 0xFF)
				{
					mReadPointer++;
					if (mReadPointer > getFullSize())
					{
						mReadPointer = 0;	// wrapped
					}
					if (mReadPointer == mWritePointer)
					{
						return false;	// end of data
					}
				}

				// check prefix 0x61
				currentIdx = mReadPointer + 1;
				if (currentIdx > getFullSize())
				{
					currentIdx = 0;	// wrapped
				}
				if (currentIdx == mWritePointer)
				{
					return false;
				}

				if (mBuff[currentIdx] == 0x61)
				{
					isSync = true;
				}
			}

			// check postfix 0xF0
			// we didn't check the length and the checksum fields, take the risk and simpler for coding
			currentIdx = mWritePointer - 1;
			if (currentIdx < 0)
			{
				currentIdx = getFullSize() - 1;
			}

			if (mBuff[currentIdx] == 0xF0)
			{
				return true;
			}

		}

		return false;
		*/
	}

	CommandReadBuffer::CommandReadBuffer()
	{
		reset();
	}

	// when bytes received from serial port, call this to put them into buffer
	void CommandReadBuffer::push(const uint8_t* pBuff, int size)
	{
		if (size > getAvailableSize())
		{
			// buffer overflow
			printf("read buffer overflow!\n");
			reset();
		}

		if (size == 0)
		{
			return;
		}

		/* debug
		printf("%d bytes received: ", size);
		for (int i = 0; i < size; i++)
		{
			printf("%02X,", pBuff[i]);
		}
		printf("\n");
		*/

		if (size <= getFullSize() - mWritePointer)
		{
			memcpy(mBuff + mWritePointer, pBuff, size);
			mWritePointer += size;
			if (mWritePointer > getFullSize() - 1)	// wrapped?
			{
				mWritePointer = 0;
			}
		}
		else
		{
			int bytesToBeWritten1 = getFullSize() - mWritePointer;
			int bytesToBeWritten2 = size - bytesToBeWritten1;

			memcpy(mBuff + mWritePointer, pBuff, bytesToBeWritten1);
			memcpy(mBuff, pBuff + bytesToBeWritten1, bytesToBeWritten2);

			mWritePointer = bytesToBeWritten2;
		}
	}

	// for poping up a specified amount of bytes from the buffer
	int CommandReadBuffer::pop(uint8_t* pBuff, int size)
	{
		int dataSize = getFullness();
		int readSize = size > dataSize ? dataSize : size;

		if (readSize <= getFullSize() - mReadPointer)
		{
			memcpy(pBuff, mBuff + mReadPointer, readSize);
			mReadPointer += readSize;
			if (mReadPointer > getFullSize() - 1)	// wrapped?
			{
				mReadPointer = 0;
			}
		}
		else
		{
			int bytesToBeRead1 = getFullSize() - mReadPointer;
			int bytesToBeRead2 = readSize - bytesToBeRead1;

			memcpy(pBuff, mBuff + mReadPointer, bytesToBeRead1);
			memcpy(pBuff + bytesToBeRead1, mBuff, bytesToBeRead2);

			mReadPointer = bytesToBeRead2;
		}

		//printf("read idx: %d, write idx: %d\n", mReadPointer, mWritePointer);

		return readSize;
	}

	// for poping up a specified amount of bytes from the buffer
	int CommandReadBuffer::trypop(uint8_t* pBuff, int size)
	{
		int dataSize = getFullness();
		int readSize = size > dataSize ? dataSize : size;

		int tmpReadPointer = mReadPointer;

		if (readSize <= getFullSize() - tmpReadPointer)
		{
			memcpy(pBuff, mBuff + tmpReadPointer, readSize);
			tmpReadPointer += readSize;
			if (tmpReadPointer > getFullSize() - 1)	// wrapped?
			{
				tmpReadPointer = 0;
			}
		}
		else
		{
			int bytesToBeRead1 = getFullSize() - tmpReadPointer;
			int bytesToBeRead2 = readSize - bytesToBeRead1;

			memcpy(pBuff, mBuff + tmpReadPointer, bytesToBeRead1);
			memcpy(pBuff + bytesToBeRead1, mBuff, bytesToBeRead2);

			tmpReadPointer = bytesToBeRead2;
		}

		//printf("read idx: %d, write idx: %d\n", mReadPointer, mWritePointer);

		return readSize;
	}
	
	// get a complete packet from the buffer, if any
	bool CommandReadBuffer::getValidPacket(uint8_t* pBuff, int* size)
	{
		if (!hasValidPacket())
		{
			return false;
		}

		if (*size < getFullness())
		{
			return false;	// make sure the buff is big enough
		}

		*size = pop(pBuff, getFullness());

		return true;
	}
	
}

