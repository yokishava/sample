//
//  BoardPhoto.swift
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/16.
//

import Foundation

struct BoardPhoto {
    
    let boardID: Int32
    let photo: NSString
    let isUSBSynced: Bool = false
    let isSySynced: Bool = false
    let createDateTime: Date
    let updateDateTime: Date
    
    init(boardID: Int, photo: String, date: Date) {
        self.boardID = Int32(boardID)
        self.photo = NSString(string: photo)
        self.createDateTime = date
        self.updateDateTime = date
    }
    
}

extension BoardPhoto: SQLTable {
    
    static var createStatement: String {
        return """
        CREATE TABLE IF NOT EXISTS BoardPhoto(
        boardId INTEGER,
        photoFileName TEXT,
        isUSBSynced BOOLEAN,
        isSCSynced BOOLEAN,
        createdDateTime DATE,
        updateDateTime DATE
        );
        """
    }
    
    static var insertStatement: String {
        return "INSERT INTO BoardPhoto (boardId, photoFileName, isUSBSynced, isSCSynced, createdDateTime, updateDateTime) VALUES (?, ?, ?, ?, ?, ?);"
    }
}
