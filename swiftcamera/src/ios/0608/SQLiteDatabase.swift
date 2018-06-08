//
//  SQLiteDatabase.swift
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/16.
//

import Foundation
import SQLite3

protocol SQLTable {
    static var createStatement: String { get }
    static var insertStatement: String { get }
}


protocol SQLAccess {
    static var path: String { get }
}

class SQLiteDatabase {
    fileprivate let dbPointer: OpaquePointer?
    
    fileprivate init(dbPointer: OpaquePointer?) {
        self.dbPointer = dbPointer
    }
    
    static func open2(path: String) throws -> SQLiteDatabase {
        var db: OpaquePointer?
        
        if sqlite3_open(path, &db) == SQLITE_OK {
            print("successfully open database" + path)
            return SQLiteDatabase(dbPointer: db)
        }
        else {
            defer {
                if db != nil {
                    sqlite3_close(db)
                }
            }
        }
        if let errorPointer = sqlite3_errmsg(db) {
            let message = String.init(cString: errorPointer)
            print(message)
            throw SQliteError.OpenDatabase(message: message)
        } else {
            throw SQliteError.OpenDatabase(message: "No error message provided from sqlite")
        }
    }
    
    static func open(element: SQLAccess.Type) throws -> SQLiteDatabase {
        var db: OpaquePointer?
        
        if sqlite3_open(element.path, &db) == SQLITE_OK {
            print("successfully open database" + element.path)
            return SQLiteDatabase(dbPointer: db)
        }
        else {
            defer {
                if db != nil {
                    sqlite3_close(db)
                }
            }
        }
        if let errorPointer = sqlite3_errmsg(db) {
            let message = String.init(cString: errorPointer)
            print(message)
            throw SQliteError.OpenDatabase(message: message)
        } else {
            throw SQliteError.OpenDatabase(message: "No error message provided from sqlite")
        }
    }
    
    fileprivate var errorMessage: String {
        if let errorPointer = sqlite3_errmsg(dbPointer) {
            let errorMessage = String(cString: errorPointer)
            return errorMessage
        } else {
            return "no error message provided from sqlite"
        }
    }
    
    deinit {
        sqlite3_close(dbPointer)
    }
}

extension SQLiteDatabase {
    
    func prepareStatement(sql: String) throws -> OpaquePointer? {
        var statement: OpaquePointer?
        guard sqlite3_prepare_v2(dbPointer, sql, -1, &statement, nil) == SQLITE_OK else {
            throw SQliteError.Prepare(message: errorMessage)
        }
        return statement
    }
}

extension SQLiteDatabase {
    //テーブル作成
    func createTable(table: SQLTable.Type ) throws {
        let createTableStatement = try prepareStatement(sql: table.createStatement)
        
        defer {
            sqlite3_finalize(createTableStatement)
        }
        
        guard sqlite3_step(createTableStatement) == SQLITE_DONE else {
            throw SQliteError.Step(message: errorMessage)
        }
        print("table created")
    }
}

extension SQLiteDatabase {
    
    //追加
    //Generics使えそう。。。
    func insert(boardPhoto: BoardPhoto, table: SQLTable.Type) throws {
        
        let insertStatement = try prepareStatement(sql: table.insertStatement)
    
        defer {
            sqlite3_finalize(insertStatement)
        }
        
        let dateformatter = DateFormat.format
        dateformatter.dateFormat = "yyyy/MM/dd HH:mm:ss"
        
        guard sqlite3_bind_int(insertStatement, 1, boardPhoto.boardID) == SQLITE_OK else {
            throw SQliteError.Step(message: errorMessage)
        }
        guard sqlite3_bind_text(insertStatement, 2, boardPhoto.photo.utf8String, -1, nil) == SQLITE_OK else {
            throw SQliteError.Step(message: errorMessage)
        }
        let isUsb = String(boardPhoto.isUSBSynced) as NSString
        guard sqlite3_bind_text(insertStatement, 3, isUsb.utf8String, -1, nil) == SQLITE_OK else {
            throw SQliteError.Step(message: errorMessage)
        }
        let isSys = String(boardPhoto.isSySynced) as NSString
        guard sqlite3_bind_text(insertStatement, 4, isSys.utf8String, -1, nil) == SQLITE_OK else {
            throw SQliteError.Step(message: errorMessage)
        }
        let create = dateformatter.string(from: boardPhoto.createDateTime) as NSString
        guard sqlite3_bind_text(insertStatement, 5, create.utf8String, -1, nil) == SQLITE_OK else {
            throw SQliteError.Step(message: errorMessage)
        }
        let update = dateformatter.string(from: boardPhoto.updateDateTime) as NSString
        guard sqlite3_bind_text(insertStatement, 6, update.utf8String, -1, nil) == SQLITE_OK else {
            throw SQliteError.Step(message: errorMessage)
        }
        guard sqlite3_step(insertStatement) == SQLITE_DONE else {
            throw SQliteError.Step(message: errorMessage)
        }
        print("Successfully inserted row.")
    }
}

extension SQLiteDatabase {
    
    //select
    func getConstructionID(query: String) -> Int? {
        guard let queryStatement = try? prepareStatement(sql: query) else {
            return nil
        }
            
        defer {
            sqlite3_finalize(queryStatement)
        }
        
        guard sqlite3_step(queryStatement) == SQLITE_ROW else {
            return nil
        }
        let id = sqlite3_column_int(queryStatement, 0)
        
        return Int(id)
    }
    
    func getRecord(query: String) {
        guard let queryStatement = try? prepareStatement(sql: query) else {
            return
        }
        defer {
            sqlite3_finalize(queryStatement)
        }

        while (sqlite3_step(queryStatement) == SQLITE_ROW) {
            let id = sqlite3_column_int(queryStatement, 0)
            let photofilename = sqlite3_column_text(queryStatement, 1)
            let usb = sqlite3_column_text(queryStatement, 2)
            let snapchamber = sqlite3_column_text(queryStatement, 3)
            let createdate = sqlite3_column_text(queryStatement, 4)
            let updatedate = sqlite3_column_text(queryStatement, 5)
            
            let filename = String(cString: photofilename!)
            let isUSB = String(cString: usb!)
            let isSC = String(cString: snapchamber!)
            let create = String(cString: createdate!)
            let update = String(cString: updatedate!)
            
            print("****************BoardPhoto Record********************")
            print("\(id), \(filename), \(isUSB), \(isSC), \(create), \(update), ")
        }
    }

}









