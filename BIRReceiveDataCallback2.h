
#ifndef __BOMEANS_BIRRECEIVEDATACALLBACK2_H__
#define __BOMEANS_BIRRECEIVEDATACALLBACK2_H__

#include "BomeansType.h"

SP_BOMEANS

Interface BIRReceiveDataCallback2
{

public:
	virtual void onDataReceived(const BomArray<uint8_t>  &receivedDataBytes) PURE;

	virtual void onLearningDataReceived(int carrierFrequency, const BomArray<int> &data) PURE;

	virtual void onLearningFailed() PURE;

	virtual ~BIRReceiveDataCallback2() {}
};

SP_END

#endif //__BOMEANS_BIRRECEIVEDATACALLBACK2_H__