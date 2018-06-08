//
//  DatabaseAccess.swift
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/16.
//

import Foundation

class DatabaseAccess {
    
    let model: BoardPhoto
    var db: SQLiteDatabase?
    
    init(boardID: Int, photoFilePath: String, date: Date) {
        self.model = BoardPhoto(boardID: boardID, photo: photoFilePath, date: date)
    }
    
    //アクセス〜保存
    func dbAccessToAdd() {
        do {
            //self.db = try SQLiteDatabase.open(element: DatabaseAccess.self)
            self.db = try SQLiteDatabase.open(element: ConstructionDao.self)
            print("Successfully opened connection to db.")
        } catch {
            self.db = nil
            print("unable to open database.")
        }
        //tableCreate()
        addModel()
        
        //確認用クエリ
        getBoardPhotoRecord()
    }
    
    //テーブル作成
    private func tableCreate() {
        if let database = db {
            do {
              try database.createTable(table: BoardPhoto.self)
                print("Successfully table created.")
            } catch {
                print("unable create table.")
            }
        }
    }
    
    //追加
    private func addModel() {
        if let database = db {
            do {
                try database.insert(boardPhoto: model, table: BoardPhoto.self)
                print("successfully write data in table")
            } catch {
                print("unable write data in table.")
            }
        }
    }
}

extension DatabaseAccess: SQLAccess {
    
    static var path: String {
        let applicationSupportDir = FileManager.default.urls(for: .applicationSupportDirectory, in: .userDomainMask)[0]
        //ApplicationSupportDirectoryがない場合、作成
        if !(FileManager.default.fileExists(atPath: applicationSupportDir.path)) {
            do {
                try FileManager.default.createDirectory(at: applicationSupportDir, withIntermediateDirectories: false, attributes: nil)
            } catch let error {
                print(error.localizedDescription)
            }
        }
        //return applicationSupportDir.path + "/test.sqlite3"
        return applicationSupportDir.path + "/photoapp.db"
    }
}

extension DatabaseAccess {
    
    //確認用のselect
    func getBoardPhotoRecord() {
        let sqlQuery = "SELECT * FROM BoardPhoto;"
        if let database = db {
            database.getRecord(query: sqlQuery)
            print("successfully get record")
        }
    }
}

