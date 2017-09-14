#ifndef __IR_UART_COMMAND__
#define __IR_UART_COMMAND__

#include <stdint.h>

namespace BOMEANS {

	class IRUartCommand {

	private:
		const uint8_t PREFIX0_CODE = 0xFF;
		const uint8_t PREFIX1_CODE = 0x61;
		const uint8_t VERSION_CODE = 0x00;
		const uint8_t POSTFIX_CODE = 0xF0;
		const uint8_t UNKNOWN_COMMAND_ID = 0xFF;

		uint8_t mCommandID = UNKNOWN_COMMAND_ID;
		uint8_t* mpPayload;
		uint16_t mPayloadLength;
		uint8_t* mpCmdBytes;
		uint16_t mCmdLength;
		uint8_t mVersionCode = VERSION_CODE;

		void Clear(void);

	public:
		IRUartCommand(void);
		~IRUartCommand(void);

		bool Load(uint8_t commandId, uint8_t* pPayloadData, uint32_t payloadLength);
		bool Load(uint8_t* pCommandData, uint32_t commandLength);
		bool IsValid();
		uint8_t GetCommandID();
		uint16_t GetPayloadLength();
		bool GetPayload(uint8_t* pBuffer, uint16_t bufferLength);
        const uint8_t* GetPayLoad() const;
		
        uint16_t GetCommandLength();
		bool GetCommandBytes(uint8_t* pBuffer, uint16_t bufferLength);
        const uint8_t* GetCommandBytes() const;
		uint8_t GetVersionCode();
	};
}

#endif //__IR_UART_COMMAND__
