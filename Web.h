/**
*      FileName : Web.h
*      author   : ldj
*      date     : 2017/05/15
*
*		Web function
* */


#ifndef _BOMEANS_WEB_H_
#define _BOMEANS_WEB_H_

#include <string>
#include <map>
#include "BomeansType.h"
#include "WebItem.h"
#include "IRReader.h"

SP_BOMEANS

namespace Web
{

class IAPIProgress
{
public:
	virtual ~IAPIProgress() {}

	/**
	* 是否終止執行
	*
	*  web api 會在執行過程當中呼叫此function 判斷是否要終止執行
	* @return
	*   true  終止執行
	*   false 繼續執行
	*/
	virtual bool isCancelled() =0;

	/**
	* 通知過程的上限
	* @param limit
	* limit <0  情況下, 表示上限未知
	*   目前大部分的web api limit 都會是 <0
	*/
	virtual void postLimit(int limit) =0;


	/**
	* 通知還在進行中
	* @param current : 目前進度
	*   current 為 <0 之下只是通知還在進行當中
	*
	*/
	virtual void postProgress(int current) =0;
};   // end of IWebAPIProgress



/**
* 取得某個類remote 所有的key list
* @param typeId         remote type
* @param language       key name 的語言別可以是en tw cn
* @param getNew         true     抓取新的資料
* 				         false    抓取catch 在local 端的資料, 如無cache 資料..抓取新的
* @param userIf         回call 介面. 用來通知取得進度, 此回call 介面的中的所有function 都跟你叫用getKeyName 時的thread 是同一個
*                       可以為null
* @return               null 表示取得失敗
*                       other 取得成功
*/
bool getKeyName(const std::string &typeId, const std::string &language, bool getNew, BomArray<Web::KeyName> &items, IAPIProgress *userIf);



bool getTypeList(const std::string &language, bool getNew, BomArray<TypeItem>& items, IAPIProgress *userIf);


bool getTopBrandList(const std::string &typeId, int start, int number, const std::string &language, bool getNew, BomArray<BrandItem>& items, IAPIProgress *userIf);

/**
* 取得廠牌list
* @param typeId : 由 getTypeList 取得(Remote type)
* @param start  : 開始位置
* @param number : 共取得最多幾筆
* @param language : 語言代號 只有 cn tw en 可選用
* @param brandName: search 廠牌名稱. 給null 表示不搜尋.. 列出所有的廠牌
* @param items    : 取得的資料
* @param userIf
* @return
*   true  : 取得成功
*   false : 取得失敗
*/
bool  getBrandList(const std::string &typeId, int start, int number, const std::string &language, const std::string &brandName, bool getNew, BomArray<BrandItem>& items, IAPIProgress *userIf);

/**
*  取得某類型, 某廠牌下的所有Remote model
* @param typeId   : 種類id 由取得type list 得到
* @param brandId  : 廠牌id 由取得廠牌list 得到
* @param items    : 取得的資料
* @param userIf   : 可以是null 這表示你不需要在抓取過程當中取得抓取進度
* @return
* 		true  : 取得成功
*       false : 取得失敗
*/
bool getRemoteModelList(const std::string &typeId, const std::string &brandId, bool getNew, BomArray<ModelItem>& items, IAPIProgress *userIf);


bool getAllFormatIDs(bool getNew, bool isAcType, BomArray<std::string> &items, IAPIProgress *userIf);



bool getAllFormatContents(bool getNew, bool isAcType, std::map<std::string, std::string> &items, IAPIProgress *userIf);


bool getIRFormat(const std::string& irFormat, bool getNew, std::string &result);
bool getRemote(const std::string& typeId, const std::string& brandId, const std::string& model, bool getNew, std::string &path);

bool getACRemoteByFormat(const BomArray<std::string>& formatList, BomArray<RemoteUID> &remoteUIDs);


bool getRemoteByKeys(const BomArray<ReaderMatchResult>& keyInfoList, BomArray<RemoteUID> &remoteUIDs);

}   // end of Web


SP_END


#endif // _BOMEANS_WEB_H_