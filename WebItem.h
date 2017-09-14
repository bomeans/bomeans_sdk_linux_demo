/**
*      FileName : WebItem.h
*      author   : ldj
*      date     : 2017/05/15
*
*      for store data form Web function
*
* */


#ifndef _BOMEANS_WEB_ITEM_H__
#define _BOMEANS_WEB_ITEM_H__

#include "BomeansType.h"
#include <string>

SP_BOMEANS

namespace Web
{

/// <summary>
/// TypeItem  
/// </summary>
struct TypeItem
{
	/// <summary>
	/// id
	/// </summary>
	std::string Id;

	/// <summary>
	/// localized name
	/// </summary>
	std::string LocalizedName;

	/// <summary>
	/// English name
	/// </summary>
	std::string EnglishName;

	TypeItem() {}

	TypeItem(const std::string &id, const std::string &localizedName, const std::string &englishName) :
		Id(id),LocalizedName(localizedName),EnglishName(englishName)
	{	}

};


struct BrandItem
{
	/// <summary>
	/// ID
	/// </summary>
	std::string Id;   // id for pass to WebApi

	/// <summary>
	/// localized brand name
	/// </summary>
	std::string LocalizedName;    // 地區名稱 example 新力 in tw

	/// <summary>
	/// English brand name
	/// </summary>
	std::string EnglishName;  // 英文名稱 example SONY

	BrandItem()
	{	}

	BrandItem(const std::string &id, const std::string &localizedName, const std::string &englishName) :
		Id(id), LocalizedName(localizedName), EnglishName(englishName)
	{	}

};  // end of BrandItem

struct BrandItemEx : public BrandItem
{
	/// <summary>
	/// Type ID which this brand belongs to
	/// </summary>
	std::string TypeId;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="id"></param>
	/// <param name="localizedName"></param>
	/// <param name="englishName"></param>
	/// <param name="typeId"></param>
	BrandItemEx(const std::string &id, const std::string &localizedName, const std::string &englishName, const std::string &typeId)
		: BrandItem(id, localizedName, englishName) ,
		TypeId(TypeId)
	{

	}

	BrandItemEx() : BrandItem() {}
};

/// <summary>
/// 
/// </summary>
struct ModelItem
{
	/// <summary>
	/// ID
	/// </summary>
	std::string Id;   // 遙控器ID

						/// <summary>
						/// models which can be controlled by this remote
						/// </summary>
	std::string MachineModel; //此遙控器可以控制的電器型號, 多筆資料石以逗號分開

								/// <summary>
								/// the contry code which is remote belongs to
								/// </summary>
	std::string Country; // 區域（語言）代碼

						   /// <summary>
						   /// the date this remote is registered to database
						   /// </summary>
	std::string ReleaseTime; // 遙控器登錄時間

	ModelItem()
	{
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="id"></param>
	/// <param name="machineModel"></param>
	/// <param name="countryCode"></param>
	/// <param name="releaseDate"></param>
	ModelItem(const std::string &id, const std::string &machineModel, const std::string &countryCode, const std::string &releaseDate) :
		Id(id),MachineModel(machineModel),Country(countryCode),ReleaseTime(releaseDate)
	{
	}
};

/// <summary>
/// 
/// </summary>
struct KeyName
{
	/// <summary>
	/// the type this key belongs to
	/// </summary>
	std::string TypeId;    // 是哪一類的remote 使用的

    /// <summary>
	/// id
	/// </summary>
	std::string Id;   // the key id     ex : IR_KEY_PLAY 

	/// <summary>
	/// localized name of this key
	/// </summary>
	std::string LocalizedName;    // key name       ex : 播放

	KeyName()
	{
	
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="typeId"></param>
	/// <param name="id"></param>
	/// <param name="localizedName"></param>
	KeyName(const std::string &typeId, const std::string &id, const std::string &localizedName) :
		TypeId(typeId),Id(id),LocalizedName(localizedName)
	{
	}

};

/// <summary>
/// 
/// </summary>
struct KeyItem
{
	std::string Id;   // key id for 發碼使用
	std::string LocalizedName;    // 地區名稱
	std::string EnglishName;  // 英文名稱

	KeyItem()
	{	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="id"></param>
	/// <param name="localizedName"></param>
	/// <param name="englishName"></param>
	KeyItem(const std::string &id, const std::string &localizedName, const std::string &englishName)
		: Id(id), LocalizedName(localizedName),EnglishName(englishName) 
	{	}
};

/// <summary>
/// 
/// </summary>
struct VoiceSearchResultItem
{
	TypeItem Type;
	BrandItemEx Brand;
	KeyItem Key;

	VoiceSearchResultItem()
	{
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="type"></param>
	/// <param name="brand"></param>
	/// <param name="key"></param>
	VoiceSearchResultItem(const TypeItem &type,const BrandItemEx &brand,const KeyItem &key)
	{
		Type = type;
		Brand = brand;
		Key = key;
	}
};

/// <summary>
/// 
/// </summary>
struct RemoteUID
{
	std::string TypeID;     // type id
	std::string BrandID;    // brand id
	std::string ModelID;    // model id   這三個id 用來建立一個remote

	RemoteUID()
	{
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="typeId"></param>
	/// <param name="brandId"></param>
	/// <param name="modelId"></param>
	RemoteUID(const std::string &typeId, const std::string &brandId, const std::string &modelId)
		: TypeID(typeId),BrandID(brandId),ModelID(modelId)
	{	}

};



} // end of namespace Web



SP_END


#endif // _BOMEANS_WEB_ITEM_H__
