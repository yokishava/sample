//
//  SQLiteError.swift
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/16.
//

import Foundation

enum SQliteError: Error {
    case OpenDatabase(message: String)
    case Prepare(message: String)
    case Step(message: String)
    case Bind(message: String)
}
