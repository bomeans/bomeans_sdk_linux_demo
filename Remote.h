/**
*  file name : Remote.h
*  author    : ldj
*  date      : 2017/06/12
*
*
*
*/


#ifndef _REMOTE_IF_H_
#define _REMOTE_IF_H_

#include <string>
#include "BomeansType.h"
#include "ACDataType.h"

SP_BOMEANS

struct Remote
{ 
	virtual ~Remote() {}

	// 取得所有的key 
	virtual BomArray<std::string> getAllKeys() PURE;

	// 送出一個key
	// for TV option 不使用
	// for AC option can be empty string 會自動切換到下一個選項
	//  不是nil 直接設定成選項
	// 小萬能的 AC
	//  option 對 溫度來說 IR_ACKEY_TEMP 可設定成"UP" 溫度上升 or "DOWN"  溫度下降
	virtual int transmitIR(const std::string &keyID, const std::string &option) PURE;


	// Set the remote internal state (without transmit the IR)
	// For AC only.
	virtual int setKeyOption(const std::string &keyID,const std::string &option) PURE;


	// 連續送出key 直到endTransmitIR
	virtual int beginTransmitIR(const std::string keyId) PURE;
	virtual void endTransmitIR() PURE;

	// 取得module name
	virtual std::string getModuleName() PURE;
	// 取得 brand name
	virtual std::string getBrandName() PURE;
	// 取得 type 代號
	virtual std::string getType() PURE;

	// 設定要repreat 次數. 只對TV 有用. 小萬能無用
	virtual void setRepeatCount(int count) PURE;
	virtual int getRepeatCount() PURE;


	// for AC use 取得所有的key
	virtual BomArray<std::string> getActiveKeys() PURE;

	// 取得key 的options
	virtual std::pair<KeyOption, bool>  getKeyOption(const std::string keyID) PURE;

	// 取得AC 的GUI Feature
	virtual std::pair<GUIFeature, bool> getGuiFeature() PURE;

	// 取得AC 的timer key
	virtual BomArray<std::string>  getTimerKeys() PURE;

	// 設定冷氣的 off time
	virtual void setOffTime(int hour, int minute, int sec) PURE;
	// 設定冷氣的 on time
	virtual void setOnTime(int hour, int minute, int sec) PURE;

	/**
	*  取得AC 的狀態資料
	* @return
	*   ACStoreDataItem array
	*   此funciotn 會把AC remote 目前的狀態資訊通通儲存到Array 中
	*   你必須把array 中的每一個元素的資料存放到磁碟中. 在你下次建立起同一個AC remote 時, 利用restoreACStoreDatas 來把AC 的狀態回復
	*   note :
	*    適用於AC remote 跟AC 小萬能, 其他類都不適用

	*/
	virtual BomArray<StoreData> getACStoreDatas() PURE;

	/**
	* 回復AC 的狀態
	* @param storeDatas
	*   狀態資料array , 狀態資料是先前你透過getACStroeDatas() 取得的資料.
	* @return
	*   true : 回復成功
	*   false : 回復失敗, 原因是給的restore 跟此AC 並不吻合
	* note :
	*   適用於AC remote 跟AC 小萬能, 其他類都不適用
	*/
	virtual bool restoreACStoreDatas(const BomArray<StoreData> &storeDatas) PURE;

};


SP_END


#endif // of _REMOTE_IF_H_
