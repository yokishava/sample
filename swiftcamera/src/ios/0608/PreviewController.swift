//
//  PreviewController.swift
//


import UIKit

class PreviewController: UIViewController {

    private var activityIndicator: UIActivityIndicatorView!
    
    var photoInfo: PhotoInfo!
    var boardInfo: BoardInfo!
    
    private var resizeImg: UIImage!
    private var exifHelper: ExifHelper!
    private var metaData: MetaData!
    
    override func loadView() {
        super.loadView()
        
        //exif取得
        exifHelper = ExifHelper(fileNameDate: photoInfo.date)
        metaData = exifHelper.getExifProperties(image: CIImage(data: photoInfo.photoData)!, gpsInfo: (photoInfo.latitude, photoInfo.longitude))
        
        self.view.backgroundColor = UIColor.black
        
        activityIndicator = UIActivityIndicatorView(activityIndicatorStyle: UIActivityIndicatorViewStyle.whiteLarge)
        activityIndicator.center = self.view.center
        self.view.addSubview(activityIndicator)
        activityIndicator.startAnimating()
        
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        //撮影写真
        let image = UIImage(data: photoInfo.photoData)
        let imgView = UIImageView(image: image)
        imgView.frame = photoFrame(orientaion: photoInfo.orientation, view: self.view)
        
        
        guard boardInfo != nil else {
            //黒板がない場合
            print("no board")
            if photoInfo.orientation == .landscapeLeft || photoInfo.orientation == .landscapeRight {
                imageTranslate(imgView: imgView, orientaion: photoInfo.orientation)
                print("imageTranslate because orientaion is landscape")
            }
            self.view.addSubview(imgView)
            //リサイズ
            resizeImg = self.photoCombineBoard(view: self.view, orientaition: photoInfo.orientation)?.resized(currentPreset: photoInfo.preset, orientation: photoInfo.orientation)
            //リサイズで使用していたViewを削除
            for subView in self.view.subviews {
                subView.removeFromSuperview()
            }
            DispatchQueue.global().async {
                //保存
                self.photoSave(image: self.resizeImg, id: self.photoInfo.boardID, date: self.photoInfo.date)
                print("save photo")
                DispatchQueue.main.async {
                    let newImgView = UIImageView(image: self.resizeImg)
                    newImgView.frame = self.newImgFrame(orientation: self.photoInfo.orientation, view: self.view)
                    //viewへの描画
                    self.view.addSubview(newImgView)
                    self.selfDismiss()
                }
            }
            return
        }
        
        guard !boardInfo.boardHidden else {
            //黒板が非表示の場合
            print("board is hidden")
             if photoInfo.orientation == .landscapeLeft || photoInfo.orientation == .landscapeRight {
                imageTranslate(imgView: imgView, orientaion: photoInfo.orientation)
                print("imageTranslate because orientaion is landscape")
            }
            self.view.addSubview(imgView)
            //リサイズ
            resizeImg = self.photoCombineBoard(view: self.view, orientaition: photoInfo.orientation)?.resized(currentPreset: photoInfo.preset, orientation: photoInfo.orientation)
            //リサイズで使用していたViewを削除
            for subView in self.view.subviews {
                subView.removeFromSuperview()
            }
            DispatchQueue.global().async {
                //保存
                self.photoSave(image: self.resizeImg, id: self.photoInfo.boardID, date: self.photoInfo.date)
                print("save photo")
                DispatchQueue.main.async {
                    let newImgView = UIImageView(image: self.resizeImg)
                    newImgView.frame = self.newImgFrame(orientation: self.photoInfo.orientation, view: self.view)
                    //viewへの描画
                    self.view.addSubview(newImgView)
                    self.selfDismiss()
                }
            }
            return
        }
        

        //以下は黒板がある場合の処理
        //landscapeの場合はさらに回転を加える
        if photoInfo.orientation == .landscapeLeft || photoInfo.orientation == .landscapeRight {
            imageTranslate(imgView: imgView, orientaion: photoInfo.orientation)
            print("imageTranslate because orientaion is landscape")
        }
        
        print("board exist")
        //黒板のImageViewの設定
        let boardView = UIImageView(image: boardInfo.board)
        boardView.frame = boardInfo.boardRect
        //変更していないと0が入ってしまう
        boardView.alpha = CGFloat(boardInfo.boardAlpha)
        
        //合成のために一時的に描画
        self.view.addSubview(imgView)
        self.view.addSubview(boardView)
        
        //合成とリサイズ
        self.resizeImg = self.photoCombineBoard(view: self.view, orientaition: photoInfo.orientation)?.resized(currentPreset: photoInfo.preset, orientation: photoInfo.orientation)
        //合成のためにaddSubviewしていたviewを削除
        for subView in self.view.subviews {
            subView.removeFromSuperview()
        }
        DispatchQueue.global().async {
            //保存
            self.photoSave(image: self.resizeImg, id: self.photoInfo.boardID, date: self.photoInfo.date)
            print("save photo")
            
            DispatchQueue.main.async {

                let newImgView = UIImageView(image: self.resizeImg)
                newImgView.frame = self.newImgFrame(orientation: self.photoInfo.orientation, view: self.view)
                //Viewへの描画
                self.view.addSubview(newImgView)
            }
        }
        
        selfDismiss()
    }
    
    override var shouldAutorotate: Bool {
        return false
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    //合成
    private func photoCombineBoard(view: UIView, orientaition: UIDeviceOrientation) -> UIImage? {
        switch orientaition {
        case .portrait:
            UIGraphicsBeginImageContextWithOptions(CGSize(width: self.view.frame.width, height: self.view.frame.width * (4/3)), false, 2.0)
        case .landscapeLeft, .landscapeRight:
            UIGraphicsBeginImageContextWithOptions(CGSize(width: self.view.frame.height * (4/3), height: self.view.frame.height), false, 2.0)
        default:
            //landscape
            if UIScreen.main.bounds.width > UIScreen.main.bounds.height {
                UIGraphicsBeginImageContextWithOptions(CGSize(width: self.view.frame.height * (4/3), height: self.view.frame.height), false, 2.0)
            } else {
                //portrait
                UIGraphicsBeginImageContextWithOptions(CGSize(width: self.view.frame.width, height: self.view.frame.width * (4/3)), false, 2.0)
            }
        }
        if let context = UIGraphicsGetCurrentContext() {
            //self.view.layer.render(in: context)
            view.layer.render(in: context)
            let newImg = UIGraphicsGetImageFromCurrentImageContext()
            UIGraphicsEndImageContext()
            print("can synthesize images.")
            return newImg
        } else {
            print("can't unwrapped UIGraphicsGetCurrentContext")
            return nil
        }
    }
    
    //撮影写真の合成前のフレーム決定
    private func photoFrame(orientaion: UIDeviceOrientation, view: UIView) -> CGRect {
        print("photoFrame orientation\(orientaion.rawValue)")
        switch orientaion {
        case .portrait:
            print("\(orientaion) is portrait")
            return CGRect(x: 0, y: 0, width: view.frame.width, height: view.frame.height)
        case .landscapeLeft, .landscapeRight:
            print("\(orientaion) is landscape")
            return CGRect(x: 0, y: 0, width: view.frame.height, height: view.frame.width)
        default:
            //landscapre
            if UIScreen.main.bounds.width > UIScreen.main.bounds.height {
                return CGRect(x: 0, y: 0, width: view.frame.height, height: view.frame.width)
            //portrait
            } else {
                return CGRect(x: 0, y: 0, width: view.frame.width, height: view.frame.height)
            }
        }
    }
    
    //合成後のUIImageのフレーム決定
    private func newImgFrame(orientation: UIDeviceOrientation, view: UIView) -> CGRect {
        switch orientation {
        case .portrait:
            let margin = view.frame.height * (1/4) / 2
            return CGRect(x: 0, y: margin, width: view.frame.width, height: view.frame.width * (4/3))
        case .landscapeLeft, .landscapeRight:
            let margin = view.frame.width * (1/4) / 2
            return CGRect(x: margin, y: 0, width: view.frame.height * (4/3), height: view.frame.height)
        default:
            //landscape
            if UIScreen.main.bounds.width > UIScreen.main.bounds.height {
                let margin = view.frame.width * (1/4) / 2
                return CGRect(x: margin, y: 0, width: view.frame.height * (4/3), height: view.frame.height)
            //portrait
            } else {
                let margin = view.frame.height * (1/4) / 2
                return CGRect(x: 0, y: margin, width: view.frame.width, height: view.frame.width * (4/3))
            }
        }
    }
    
    //撮影写真の合成前のフレーム決定（回転対応）
    private func imageTranslate(imgView: UIImageView, orientaion: UIDeviceOrientation) {
        var transRotate: CGAffineTransform = CGAffineTransform(rotationAngle: 0)
        print("imageTranslate orientation \(orientaion.rawValue)")
        if orientaion.rawValue == 3 {
            print("\(orientaion) .landscapeRight")
            let angle = -(90 * CGFloat.pi / 180)
            transRotate = CGAffineTransform(rotationAngle: CGFloat(angle))
        } else if orientaion.rawValue == 4 {
            print("\(orientaion) .landscapeLeft")
            let angle = (90 * CGFloat.pi / 180)
            transRotate = CGAffineTransform(rotationAngle: CGFloat(angle))
        }
        imgView.transform = transRotate
        imgView.frame.origin.x = 0
        imgView.frame.origin.y = 0
    }
    
    //保存
    private func photoSave(image: UIImage, id: Int, date: Date) {
        var filePath: String = ""
        
        let constructionDao = ConstructionDao(boardID: id)
        let constructionID = constructionDao.getConstructionID()
        //ファイル保存
//        let fileSystem = FileSystemAccess(constructionID: constructionID!, photo: image, date: date)
        let fileSystem = FileSystemAccess(constructionID: constructionID!, photo: image)
        //filePathは信憑性チェック前のdest.jpgのパス
        filePath = fileSystem.saveFile()
        
        //exif書き込み
        exifHelper.writeExifProperties(path: filePath, data: metaData)
        
        //xmp書き込み
        var error: NSError?
//        //XMPUtils.writePhotoXmp(filePath, error: &error)
        //XMPUtils.writeXmp(filePath, boardId: Int32(boardID), error: &error)
        XMPUtils.writeNewDom(filePath, error: &error)
        //XMPUtils.modifyingXmp(filePath, error: &error)
        guard (error == nil) else {
            print(error.debugDescription)
            return
        }
        
        //dest.jpgを削除 → Documents/Construction/工事ID/photos/までになる
        let destPath = String(filePath.prefix(filePath.count - 8))
        print("---------- dest.jpg path ---------------")
        print(destPath)
        
        let workPath = destPath + "work.jpg"
        let workPath2 = destPath + "work2.jpg"
        
        //1
        //CWrapper.wrapp(filePath, destFile: workPath)
        //2
        //CWrapper.wrapp2(workPath, destFile: workPath2)
        
        //日付のフォーマット
        let dateFormatter = DateFormat.format
        dateFormatter.dateFormat = "yyyyMMddHHmmss"
        //改ざんチェック値を埋め込んだファイル名
        //let destFilePath = destPath + dateFormatter.string(from: date) + ".jpg"
        let destFilePath = destPath + String(constructionID!) + "_" + dateFormatter.string(from: date) + ".jpg"
        //信憑性チェック
        //let wrapper = CWrapper.call_JACIC_WriteHashValue(filePath, destFile: destFilePath)
        let wrapper = CWrapper.call_JACIC_WriteHashValue(filePath, destFile: destFilePath)
        print("--------------------------信憑性チェックの結果-------------------------------")
        print(wrapper)
        print("-------------------------------------------------------------------------")
        
        //DB保存
        let db = DatabaseAccess(boardID: id, photoFilePath: destFilePath, date: date)
        db.dbAccessToAdd()
    }
    
    //3秒後閉じる
    private func selfDismiss() {
        DispatchQueue.main.asyncAfter(deadline: .now() + 3.0, execute: {
            self.dismiss(animated: true, completion: nil)
        })
    }
}

