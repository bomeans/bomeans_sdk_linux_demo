//
//  BomeansConst.h
//  IRLib
//
//  Created by ldj on 2015/6/4.
//  Copyright (c) 2015年 lin. All rights reserved.
//

#ifndef IRLib_BomeansConst_h
#define IRLib_BomeansConst_h

enum BIRError {
    BIRNoError=0,                    // 沒有錯誤
    BIROK     =0,
    
    BIRTransmitFail=1,               // 送出IR wave 失敗
    BIRTransmitFailWifiToIR,       // 送出IR wave 失敗, 因wifi To IR 不正常
    BIRNoImplement,                // 此功能沒有設計
    
    BIRNotConnectToNetWork,       // iDevice 沒有連上網路
    BIRNotConnectToAP,            // iDevice 沒有連上 wifi AP
    BIRCantGetFileFromServer,     // 無法從server 取得資料
    BIRSaveFileError,             // 儲存檔案錯誤
    BIRReadFileError,             // 讀檔錯誤
    BIRXMLFormatError,            // xml format error
    BIRJsonFormatError,           // json 檔案格式錯誤
    BIRWebAPIFail,                // web api fail , 如無法連接到web server ..

    
    BIRBackgroudProcessFail,       // 建立backgroud 程序失敗
    
    BIRNotFindWifiToIR,            // 找不到小火山
    
    BIRKeyIDNotExist,              // 預送出的的key id 不存在
    
    BIRSmartPickerCreateFail,      // smart picker create fail
    BIRPramNotRight,                // 輸入的參數不正確
    
    BIRTimeOut,                     // 某些程序, 發生time out 問題. 通常是在learning 方面. 取得F/W version 時
    BIRPackageForamtError,          // Bomeans 發碼ic 回傳的資料有錯誤
    BIRPackageTypeError,            // Bomeans 發碼ic 回傳的資料類型有錯誤
  
	BIRACRestoreDataFail,

	BIRUserCancel = 2000,
    
    BIRLibImplementError=100000,
    
    BIR_CustomerErrorBegin=0x40000000    // 第三方開發者. .開發  @protocol BIRIrHW  時.. 回傳的的錯誤值請大於 0x40000000
};
// note : 第三方開發者. .開發  @protocol BIRIrHW  時.. 回傳的的錯誤值請大於 0x40000000

enum BIRGuiDisplayType {
    BIRGuiDisplayType_NO=0,      // 實體Remote 沒有顯示任何資訊
    BIRGuiDisplayType_YES=1,     // 實體Remote 只有在AC power on 時會顯示資訊
    BIRGuiDisplayType_ALWAYS=2,  // 實體Remote 總是顯示資訊
    
};


enum BIRWifiToIRResult {
    BIRResultFind = 0,      //
    BIRResultSetOk = 0,     // wifiToIR 設定成功(note 目前版本.不會有此結果)
    BIRResultTimeOut,       // 時間用光結束
    BIRResultUserCancal     // 使用者中斷後結束
};


enum BIRWifiAuthMode{    // wifi 的 Auth mode
    AuthModeOpen = 0x00,
    AuthModeShared = 0x01,
    AuthModeAutoSwitch = 0x02,
    AuthModeWPA = 0x03,
    AuthModeWPAPSK = 0x04,
    AuthModeWPANone = 0x05,
    AuthModeWPA2 = 0x06,
    AuthModeWPA2PSK = 0x07,
    AuthModeWPA1WPA2 = 0x08,
    AuthModeWPA1PSKWPA2PSK = 0x09

};

// smart picker  key result
enum BIRPickerKeyResult {
    BIR_PNext,    // 必須再測試下一個key
    BIR_PFind,    // 找到remote
    BIR_PFail,     // 找不到remote
    BIR_PUnknow,  // 不確定的狀態 可能是根本沒有呼叫begin
};

typedef enum BIRServer {    // Data Server
    BIRServerTW = 0,
    BIRServerCN = 1,
    BIRServerTEST = 99
} BIRServer;


#endif
