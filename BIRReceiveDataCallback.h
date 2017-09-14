
#ifndef __BOMEANS_BIRRECEIVEDATACALLBACK_H__
#define __BOMEANS_BIRRECEIVEDATACALLBACK_H__

#include "BomeansType.h"

SP_BOMEANS

Interface BIRReceiveDataCallback
{

public:
	virtual void onDataReceived(const BomArray<uint8_t> &receivedDataBytes) PURE;

	virtual ~BIRReceiveDataCallback() {}
};

SP_END

#endif //__BOMEANS_BIRRECEIVEDATACALLBACK_H__