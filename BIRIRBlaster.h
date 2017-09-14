
#ifndef __BOMEANS_BIRIRBLASTER_H__
#define __BOMEANS_BIRIRBLASTER_H__

#include "BomeansType.h"
#include "BIRReceiveDataCallback.h"

SP_BOMEANS

class BIRIRBlaster
	{
	public:
		virtual int sendData(const BomArray<uint8_t>& irUartData) =0;

		virtual int isConnection() =0;

		virtual void setReceiveDataCallback(BIRReceiveDataCallback* pCallback)=0;
		virtual ~BIRIRBlaster() {}
	};

SP_END

#endif //__BOMEANS_BIRIRBLASTER_H__