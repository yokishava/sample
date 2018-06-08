//
//  FileSystemAccess.swift
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/16.
//

import Foundation

protocol FileAccess {
    func makeSaveFileFolder() -> String
    func saveFile() -> String
}

class FileSystemAccess: FileAccess {

    //private var filepath: String = ""
    private var constructionID: Int
    private var photo: UIImage
    //private var date: Date
    private let dateformatter = DateFormat.format
    
//    init(constructionID: Int, photo: UIImage, date: Date) {
//        self.constructionID = constructionID
//        self.photo = photo
//        self.date = date
//
//        dateformatter.dateFormat = "yyyyMMddHHmmss"
//    }
    init(constructionID: Int, photo: UIImage) {
        self.constructionID = constructionID
        self.photo = photo
        //self.date = date
        
        //dateformatter.dateFormat = "yyyyMMddHHmmss"
    }
    
    //写真ファイル保存
    func saveFile() -> String {
        let dirPath = makeSaveFileFolder()
        
        //写真ファイル作成
        let photoData = UIImageJPEGRepresentation(photo, 1.0)
        //let fileName = dateformatter.string(from: self.date) + ".jpg"
        let fileName = "dest.jpg"
        let dirUrl = URL(fileURLWithPath: dirPath)
        let fileUrl = dirUrl.appendingPathComponent(fileName)
        do {
            try photoData?.write(to: fileUrl)
            print("Successfully save file")
            //print(fileUrl.path)
        } catch let error {
            print(error.localizedDescription)
        }
        
        //確認
        fileExistence()
        
        return fileUrl.path
    }
    
    //写真ファイル保存フォルダ作成
    func makeSaveFileFolder() -> String {
        //Documentsディレクトリの取得
        let documentPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
        let constructions = "/constructions"
        let constructionID = "/\(self.constructionID)"
        let photos = "/photos"
        
        if FileManager.default.fileExists(atPath: documentPath + constructions + constructionID + photos) {
            //すでにある場合
            //filepath = documentPath + constructions + constructionID + photos
            let filepath = documentPath + constructions + constructionID + photos
            return filepath
            
        } else {
            //まだない場合
            makeNewFolder(documentPath, constructions, constructionID, photos)
            let filepath = documentPath + constructions + constructionID + photos
            return filepath
        }
    }
    
    //新規フォルダ作成
    func makeNewFolder(_ documentPath: String, _ constructions: String, _ constructionID: String, _ photos: String) {
        //constructions作成
        do {
            try FileManager.default.createDirectory(atPath: documentPath + constructions, withIntermediateDirectories: false, attributes: nil)
        } catch let errorMsg {
            print(errorMsg.localizedDescription)
        }
        
        //工事IDフォルダ作成
        let constructionsPath = documentPath + constructions
        do {
            try FileManager.default.createDirectory(atPath: constructionsPath + constructionID, withIntermediateDirectories: false, attributes: nil)
        } catch let errorMsg {
            print(errorMsg.localizedDescription)
        }
        
        //photosフォルダ作成
        let photosPath = constructionsPath + constructionID
        do {
            try FileManager.default.createDirectory(atPath: photosPath + photos, withIntermediateDirectories: false, attributes: nil)
        } catch let errorMsg {
            print(errorMsg.localizedDescription)
        }
    }
    
    //保存されているか確認
    func fileExistence() {
        let documentPath = NSSearchPathForDirectoriesInDomains(.documentDirectory, .userDomainMask, true)[0]
        
        //ファイルが存在するパスを取得
        if let dir = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first {
            let filePath = dir.appendingPathComponent("constructions/\(self.constructionID)/photos")
            let path: String = filePath.path
            print("*********************************************")
            print(path)
            
            //Documents以下のフォルダ及びファイルを取得
            print("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@")
            do {
                let items = try FileManager.default.contentsOfDirectory(atPath: documentPath)
                for item in items {
                    print(item)
                }
            } catch let error {
                print(error.localizedDescription)
            }
            
            //photos以下のファイルを全取得
            print("++++++++++++++++++++++++++++++++++++++++++++")
            do {
                let items = try FileManager.default.contentsOfDirectory(atPath: path)
                for item in items {
                    print(item)
                }
            } catch let error {
                print(error.localizedDescription)
            }
        }
    }
    
}






