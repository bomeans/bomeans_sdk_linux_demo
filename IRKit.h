/**
*      FileName : IRKit.h
*      author   : ldj
*      date     : 2017/05/15
*
*
*
* */


#ifndef _BOMEANS_IR_KIT_H_
#define _BOMEANS_IR_KIT_H_

#include "BomeansType.h"
#include "BIRIRBlaster.h"
#include "Remote.h"
#include "Web.h"

SP_BOMEANS

namespace IRKit {


void setup(const std::string &apiKey, bool useChinaServer, BIRIRBlaster *myIrBlaster);

std::string getAPIKey();

/**
* 設定是否使用中國大陸的server
* @param cn : true 使用中國大陸的server , false 不使用中國大陸的server
*/
void setUseChineseServer(bool cn);

/**
*	建立remote
*   @param : type 
*   @param : brand
*   @param : model
*   @retrun
*     a point of Remote   YOU muse USE delete to destory object when you never use it.
*/
Remote* createRemote(const std::string &type, 
					 const std::string &brand, 
					 const std::string &model,
					 bool getNew, Web::IAPIProgress *userIf);

/**
*	建立IRReader 
*   @param : newData
*   @return :
*    a point of IRReader. YOU muse USE delete to destory object when you never use it.
*/
IRReader* createIRReader(bool newData);

void setIRHW(BIRIRBlaster *irBlaster);

};   // end of namespace IRKit



SP_END

#endif    // of _BOMEANS_IR_KIT_H_




