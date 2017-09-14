//
//  FileName : IRReader.h
//  Date     : 2017/07/06
//  author   : ldj
//


#ifndef _IR_READER_H_B
#define _IR_READER_H_B

#include "BomeansType.h"
#include "WebItem.h"

SP_BOMEANS


enum PREFER_REMOTE_TYPE
{
	/**
	* The best matched format is decided by the parsing core.
	*/
	Auto,
	/**
	* The best matched format is the AC format if any.<p>
	* If no AC format is matched, TV format will be selected
	*/
	AC,
	/**
	* The best matched format is the TV format if any.<p>
	* If no TV format is matched, AC format will be selected
	*/
	TV
};

namespace LearningErrorCode {
enum V
{
	LearningModeFailed,
	TimeOut,
	IncorrectLearnedData
};
}

namespace FormatParsingErrorCode
{
enum V
{
	LearningModeFailed,
	NoValidLearningData,
	UnrecognizedFormat
};
}
namespace CloudMatchErrorCode
{
enum V
{
	LearningModeFailed,
	NoValidLearningData,
	UnrecognizedFormat,
	NoValidMatch,
	ServerError
};
}

struct  ReaderMatchResult
{
	enum { UNDEFINED_CODE = -1 };

	std::string formatId;
	long customCode{0};
	long keyCode{0};

	ReaderMatchResult() {}

	ReaderMatchResult(const std::string &a_formatId, long a_customCode, long a_keyCode) :
		formatId(a_formatId),
		customCode(a_customCode),
		keyCode(a_keyCode)
	{
	}

	/**
	* Indicate if the matched format is AC format.
	* For non-AC format match (TV-like format), customCode/keyCode field will be filled if any.
	* @return true for AC format, false otherwise.
	*/
	bool isAc()
	{
		return (
			(formatId.size() > 0) &&
			(customCode == UNDEFINED_CODE) && (keyCode == UNDEFINED_CODE));
	}

	// 是否為可用的資料
	bool isVerify()
	{
		return (formatId.size() > 0);
	}
};

Interface ReaderRemoteMatchCallback
{
	virtual ~ReaderRemoteMatchCallback() {}

	/**
	* The matched remote controller(s) of the loaded learning data.<br>
	* Note: the matched remote controller list is the result of accumulated learning data since the last call of reset().
	* @param mCloudMatchResultList list of remote controller info
	*/
	virtual void onRemoteMatchSucceeded(const ArrayList<Web::RemoteUID> &remoteMatchResultList) PURE;

	/**
	* If failed to find the matched remote in the cloud database.
	* @param errorCode error code
	*/
	virtual void onRemoteMatchFailed(CloudMatchErrorCode::V errorCode) PURE;

	/**
	* Invoked if matched format of the learned result(s) is found.<br>
	* Note: The matched results are accumulated if not calling reset()
	* @param formatMatchResultList the matched result list
	*/
	virtual void onFormatMatchSucceeded(const ArrayList<ReaderMatchResult>& formatMatchResultList) PURE;

	/**
	* Invoked if no matched format of the learned result(s) is found.
	* @param errorCode error code
	*/
	virtual void onFormatMatchFailed(FormatParsingErrorCode::V errorCode) PURE;

};   // end of ReaderRemoteMatchCallback


Interface ReaderFormatMatchCallback
{
	virtual ~ReaderFormatMatchCallback() {}
	
	/**
	* Invoked if matched format of learned result is found.<br>
	* @param formatMatchResult matched result
	*/
	virtual void onFormatMatchSucceeded(const ReaderMatchResult &formatMatchResult) PURE;

	/**
	* Invoked if there is no match for the learned result.
	* @param errorCode error code
	*/
	virtual void onFormatMatchFailed(FormatParsingErrorCode::V errorCode) PURE;

	/**
	* Invoked when the learning data is received.
	* @param learningData the learned IR signal data. This data can be stored and re-transmit by calling sendLearningData(byte[] data)
	*/
	virtual void onLearningDataReceived( const BomArray<uint8_t> &learningData) PURE;

	/**
	* Invoked when the learning data is not received or is incorrect.
	* @param errorCode
	*/
	virtual void onLearningDataFailed(LearningErrorCode::V errorCode) PURE;

};  // end of ReaderFormatMatchCallback



Interface IRReader          // this is Interface 
{ 
	virtual ~IRReader() {}

	/**
	* <b>Note</b> This method is for debug purpose.<br>
	* Not to call this method unless you understand its usage.<p>
	* Loading learning data into the reader for parsing.
	*
	* @param data data bytes to be parsed
	* @param isPayloadOnly true if the passing data bytes are payload data only (exclude the prefix, postfix, checksum,..etc),
	* or false if the passing data contains the full packet returned from the MCU (the full packet looks like this: 0xFF, 0x61, ..., 0xF0)
	* @param isCompressedFormat true if the containing payload data are in compressed format, or false if in plain(uncompressed) format.
	* (note: most likely it's in compressed format, so if unsure, set it to true)
	* @return true if the passing data is valid
	*/
	virtual bool load(const BomArray<uint8_t> &data, bool isPayloadOnly, bool isCompressedFormat) PURE;

	/**
	* Get the best matched format(s) of the passing learning data.
	* The best match(es) might not be only. But most likely you can pick the first entry as the best one.
	* Note the best match(es) might contain TV and AC results. You can filter the result by checking the
	* ReaderMatchResult::isAc().
	* @return best matched format(s) of the loaded learning data
	*/
	virtual ArrayList<ReaderMatchResult> getBestMatches() PURE;

	/**
	* All the other matched formats not treated as the "best" ones.
	* @return the possible matched format(s) of the loaded learning data
	*/
	virtual ArrayList<ReaderMatchResult> getPossibleMatches() PURE;

	/**
	* getBestMatches() + getPossibleMatches()
	* @return
	*/
	virtual ArrayList<ReaderMatchResult> getAllMatches() PURE;

	
	/**
	* <b>Note</b> This method is for debug purpose.<br>
	* Get the wave count (number of on/off IR signals) of the loaded learning data.
	*
	* @return the wave count (number of on/off IR signals) of the loaded data
	*/
	virtual int getWaveCount() PURE;

	/**
	* <b>Note</b> This method is for debug purpose.<br>
	* Get the carrier frequency of the loaded learning data.
	* @return carrier frequency, in Hz
	*/
	virtual int getFrequency() PURE;

	/**
	* Start the learning process, and get the learned raw data as well as the matched format in the passing callback functions.<br>
	* <b>Note</b>: Only format matching is processed in this call. If you need to search the cloud server
	* for matching remote controllers, call <i>startLearningAndSearchCloud()</i>
	* @param preferRemoteType AC, TV, or Auto. This affects the decision of the "best match" returned.
	* @param callback To receive the learning/parsing results
	*/
	virtual void startLearningAndGetData(PREFER_REMOTE_TYPE preferRemoteType, ReaderFormatMatchCallback *callback) PURE;

	/**
	* Start the learning process, get the matched format and matched remote controller(s) in the passing callback functions.<br>
	* @param isNewSearch Set to true if you are starting a new learning. Set to false if you are adding the learning to filter the previous matched result.
	* @param preferRemoteType AC, TV, or Auto. This affects the decision of the "best match" returned.
	* @param callback To receive the parsing/searching results
	*/
	virtual void startLearningAndSearchCloud(bool isNewSearch, PREFER_REMOTE_TYPE preferRemoteType, ReaderRemoteMatchCallback *callback) PURE;

	/**
	* Reset the internal learning states if you have previous called startLearningAndSearchCloud().
	*/
	virtual void reset() PURE;

	/**
	* Send the stop learning command to IR Blaster. (Return to normal mode)
	* @return ConstValue.BIROK if succeeded, other error code if failed
	*/
	virtual int stopLearning() PURE;

	/**
	* Send the learned IR signal.<br>
	*
	* @param learningData The learning data get from startLearningAndGetData();
	*/
	/**
	* Send the learned IR signal.<br>
	* @param learningData The learning data get from startLearningAndGetData()
	* @return ConstValue.BIROK if succeeded, other error code if failed
	*/
	virtual int sendLearningData(const BomArray<uint8_t> &learningData) PURE;


};



SP_END

#endif // _IR_READER_H_B
