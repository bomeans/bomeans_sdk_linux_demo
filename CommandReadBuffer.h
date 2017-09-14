#ifndef __COMMAND_READ_BUFFER__
#define __COMMAND_READ_BUFFER__

#define MAX_READ_BUFFER_SIZE	(2048)

#include <stdint.h>

namespace BOMEANS {
	/*
	 Read buffer for sotring the bytes come from the serial port.
	 Since one batch of bytes sent from the MCU may not be read at once at host(Linux) side,
	 we had better cached the bytes in a buffer and read them later in order to get the full 
	 packet.
	 */
	class CommandReadBuffer
	{
	private:
		uint8_t mBuff[MAX_READ_BUFFER_SIZE];
		int mReadPointer;	// next read index
		int mWritePointer;	// nexy write index

		// get free size in buffer
		int getAvailableSize();

		// get the fullness of the buffer
		int getFullness();

		// get full buffer size
		int getFullSize();
		
		// check (and sync) the buffer if a valid packet has arrived.
		// A valid packet here means the packet is in the format of Bomeans UART command format.
		// (FF, 61, 00, ....., F0)
		bool hasValidPacket();

	public:
		CommandReadBuffer();

		// rest the buffer read/write pointers
		void reset();

		// when bytes received from serial port, call this to put them into buffer
		void push(const uint8_t* pBuff, int size);

		// for poping up a specified amount of bytes from the buffer
		int pop(uint8_t* pBuff, int size);

		int trypop(uint8_t* pBuff, int size);
		
		// get a complete packet from the buffer, if any
		bool getValidPacket(uint8_t* pBuff, int* size);
		
	};
}

#endif //__COMMAND_READ_BUFFER__

