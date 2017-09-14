/**
*  file name : ACDataType.h
*  author    : ldj
*  date      : 2017/06/12
*
*
*
*/



#ifndef _AC_DATA_TYPE_H_
#define _AC_DATA_TYPE_H_

#include "BomeansType.h"


SP_BOMEANS


struct KeyOption
{ 
	int currentOption;
	BomArray<std::string> options;
	bool enable{true};
};


struct GUIFeature
{ 
	enum DisplayType
	{ 
		TYPE_NO,				// 沒有LCD顯示
		TYPE_YES,				// 有LCD 顯示屏, 但只有在開機時. 才會有畫面
		TYPE_ALWAYS             // 有LCD 顯示屏, 且永遠會顯示資料
	};
	
	DisplayType displayType{DisplayType::TYPE_NO}; //=Display.NO;
	bool RTC{false}; // = false;
	int timerMode{0}; // = 0;
	bool timerCountDown{false}; // = false;
	bool timerClock{false}; // = false;

};

struct StoreData
{ 
	int mode;     // << 0x1000 為mode
	std::string state;  // (state or 變數)
	int value;
};


SP_END

#endif // of _AC_DATA_TYPE_H_
