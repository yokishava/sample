//
//  ConstructionDao.swift
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/17.
//

import Foundation

struct ConstructionDao {
    
    private let boardID: Int
    private let sqlQuery: String
    
    init(boardID: Int) {
        self.boardID = boardID
        
        sqlQuery = "SELECT c.constructionId From Construction c join BoardSet bs on c.constructionId = bs.constructionId join board b on bs.boardSetId = b.BoardSetId Where b.boardId = " + String(self.boardID)
    }
    
    //db接続~工事ID取得
    func getConstructionID() -> Int? {
        var db: SQLiteDatabase? = nil
        do {
            db = try SQLiteDatabase.open(element: ConstructionDao.self)
        } catch {
            print("unable to open photoapp.db")
        }
        let id = ExecutionQuery(db: db)
        return id
    }
    //selectの呼び出し
    private func ExecutionQuery(db: SQLiteDatabase?) -> Int? {
        var constructionID: Int? = nil
        if let database = db {
            do {
                constructionID = database.getConstructionID(query: self.sqlQuery)
            }
        }
        if constructionID == nil {
            print("can't get constructionID")
        } else {
            print("get constructionID!!")
        }
        return constructionID
    }

}

extension ConstructionDao: SQLAccess {
    
    //path
    static var path: String {
        let fileurl = FileManager.default.urls(for: .libraryDirectory, in: .userDomainMask)[0]
        return fileurl.path + "/LocalDatabase/photoapp.db"
    }
    
}
