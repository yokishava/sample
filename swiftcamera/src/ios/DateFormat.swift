//
//  DateFormat.swift
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/17.
//

import Foundation

struct DateFormat {
    
    static var format: DateFormatter = {
        let _formatter = DateFormatter()
        _formatter.locale = Locale(identifier: "ja_JP")
        _formatter.timeZone = TimeZone(identifier: "Asia/Tokyo")
        return _formatter
    }()
    
    //DB
    //_formatter.dateFormat = "yyyy/MM/dd HH:mm:ss"
    
    //ファイル
    //_formatter.dateFormat = "yyyyMMddHHmmss"
}
