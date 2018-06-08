//
//  BoardImageView.swift
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/14.
//

import UIKit

enum BoardOrientation {
    case portrait
    case landscape
}

class BoardImageView: UIImageView {
    
    var boardStr: String
    var orientation = BoardOrientation.portrait
    
    private let buttonsViewSize: CGRect
    private var locationInitialTouch: CGPoint!
    private let headerCount: Int = 23
    private var currentScale: CGFloat = 1.0
    private var maxScale: CGFloat!
    private var scaleArray: [CGFloat] = []
    private var specifiedMaxScale = false
    
    weak var currentSettingsValue: CurrentSettingsValue!
    
    init(board: String, buttons: CGRect, currentSettingsValue: CurrentSettingsValue) {
        self.boardStr = board
        self.buttonsViewSize = buttons
        self.currentSettingsValue = currentSettingsValue
        
        //ヘッダー削除
        let substr = String(boardStr.suffix(boardStr.count - headerCount))
        //decode
        let decode = Data(base64Encoded: substr, options: .ignoreUnknownCharacters)
        
        let boardImage = UIImage(data: decode!)
        
        super.init(image: boardImage)
        
        let pinchGesture = UIPinchGestureRecognizer(target: self, action: #selector(pinchAction(_:)))
        self.addGestureRecognizer(pinchGesture)
        self.isUserInteractionEnabled = true
        
        //表示、非表示について
        guard currentSettingsValue.boardDisplay else {
            self.isHidden = true
            return
        }
        
        //表示するときの位置、サイズ情報
        guard currentSettingsValue.boardrect == nil else {
            self.frame = currentSettingsValue.boardrect
            boardCheckInScreen()
            return
        }
        
        //表示するときの透過度
        //self.alpha = 1.0 - CGFloat(currentSettingsValue.currentAlphaValue)
    }
    
    func orientaionChange(_ current: UIDeviceOrientation, _ shouldAutoRotate: Bool) {
        //true = ロックされていない
        if shouldAutoRotate {
            switch current {
            case .landscapeRight, .landscapeLeft:
                self.orientation = .landscape
            default:
                self.orientation = .portrait
            }
        }
        print(orientation)
    }
    
    func updateBoardAlphaValue(_ value: Float) {
        self.alpha = CGFloat(1.0 - value)
    }
    
    func boardDisplayChanged(_ value: Bool) {
        //value = boardDisplay
        guard value else {
            self.isHidden = true
            return
        }
        self.isHidden = false
        
        guard currentSettingsValue.boardrect == nil else {
            self.frame = currentSettingsValue.boardrect
            boardCheckInScreen()
            return
        }
    }
    
    @objc func pinchAction(_ sender: UIPinchGestureRecognizer) {
        // imageViewを拡大縮小する
        // ピンチ中の拡大率は0.3〜2.5倍、指を離した時の拡大率は0.5〜2.0倍とする
        switch sender.state {
        case .began:
            //startScale = self.transform
            break
        case .changed:
            // senderのscaleは、指を動かしていない状態が1.0となる
            // 現在の拡大率に、(scaleから1を引いたもの) / 10(補正率)を加算する
            currentScale = currentScale + (sender.scale - 1) / 10
            // 拡大率が基準から外れる場合は、補正する
            if currentScale < 0.3 {
                currentScale = 0.3
            } else if currentScale > 2.5 {
                currentScale = 2.5
            }
            judgeZoom(currentScale)
        default:
            if currentScale < 0.5 {
                UIView.animate(withDuration: 0.2, animations: {
                    self.transform = CGAffineTransform(scaleX: 0.5, y: 0.5)
                }, completion: nil)
            }
            //リセット
            scaleArray.removeAll()
            specifiedMaxScale = false
            //currentScaleをリセットしていなかったから次のpinchGestureのときに2.5からスタートしてた
            currentScale = 1.0
        }
    }
    
    //拡大時の境界判定
    func judgeZoom(_ currentScale: CGFloat) {
        switch UIDevice.current.orientation {
        case .portrait, .faceDown, .faceUp, .portraitUpsideDown:
            //すでに画面の境界に位置してpinchActionをしても拡大しない
            if self.frame.origin.x == 0 || self.frame.origin.y == 0 || self.frame.maxX == UIScreen.main.bounds.width || self.frame.maxY == UIScreen.main.bounds.height - buttonsViewSize.height {
            } else {
                // 計算後の拡大率で、imageViewを拡大縮小する
                self.transform = CGAffineTransform(scaleX: currentScale, y: currentScale)
                scaleArray.append(currentScale)
                //拡大していって画面の境界を超えた時、超える直前のcurrentScaleを使用する
                if self.frame.origin.x < 0 || self.frame.origin.y < 0 || self.frame.maxX > UIScreen.main.bounds.width || self.frame.maxY > UIScreen.main.bounds.height - buttonsViewSize.height {
                    if !specifiedMaxScale {
                        maxScale = scaleArray[self.scaleArray.count - 1]
                        specifiedMaxScale = true
                    }
                    self.transform = CGAffineTransform(scaleX: maxScale, y: maxScale)
                }
            }
            //位置、サイズ情報の更新
            currentSettingsValue.boardrect = self.frame
            
        case .landscapeRight, .landscapeLeft:
            if self.frame.origin.x == 0 || self.frame.origin.y == 0 || self.frame.maxX == UIScreen.main.bounds.width - buttonsViewSize.height || self.frame.maxY == UIScreen.main.bounds.height {} else {
                self.transform = CGAffineTransform(scaleX: currentScale, y: currentScale)
                scaleArray.append(currentScale)
                
                if self.frame.origin.x < 0 || self.frame.origin.y < 0 || self.frame.maxX > UIScreen.main.bounds.width - buttonsViewSize.height || self.frame.maxY > UIScreen.main.bounds.height {
                    if !specifiedMaxScale {
                        maxScale = scaleArray[self.scaleArray.count - 1]
                        specifiedMaxScale = true
                    }
                    self.transform = CGAffineTransform(scaleX: maxScale, y: maxScale)
                }
            }
            
            //位置、サイズ情報の更新
            currentSettingsValue.boardrect = self.frame
            
        default:
            break
        }
        
    }
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        if let touch = touches.first {
            let location = touch.location(in: self)
            locationInitialTouch = location
        }
    }
    
    override func touchesMoved(_ touches: Set<UITouch>, with event: UIEvent?) {
        if let touch = touches.first {
            let location = touch.location(in: self)
            
            frame = frame.offsetBy(dx: location.x - locationInitialTouch.x, dy: location.y - locationInitialTouch.y)
            boardMovement()
        }
    }
    
    override func touchesEnded(_ touches: Set<UITouch>, with event: UIEvent?) {
        if let touch = touches.first {
            let location = touch.location(in: self)
            
            frame = frame.offsetBy(dx: location.x - locationInitialTouch.x, dy: location.y - locationInitialTouch.y)
            boardMovement()
            //位置、サイズ情報の更新
            currentSettingsValue.boardrect = self.frame
        }
    }
    
    //移動時の境界判定
    func boardMovement() {
        //true = ロックされていない
        if currentSettingsValue.deviceLockStatus {
            switch UIDevice.current.orientation {
            case .portrait:
                if frame.origin.x < 0 {
                    frame.origin.x = 0
                }
                if frame.origin.y < 0 {
                    frame.origin.y = 0
                }
                if frame.maxX > UIScreen.main.bounds.width {
                    frame.origin.x = UIScreen.main.bounds.width - frame.width
                }
                if frame.maxY > UIScreen.main.bounds.height - buttonsViewSize.height {
                    frame.origin.y = UIScreen.main.bounds.height - (buttonsViewSize.height + frame.height)
                }
            case .landscapeRight, .landscapeLeft:
                
                if frame.origin.x < 0 {
                    frame.origin.x = 0
                }
                if frame.origin.y < 0 {
                    frame.origin.y = 0
                }
                if frame.maxX > UIScreen.main.bounds.width - buttonsViewSize.height {
                    frame.origin.x = UIScreen.main.bounds.width - (buttonsViewSize.height + frame.width)
                }
                if frame.maxY > UIScreen.main.bounds.height {
                    frame.origin.y = UIScreen.main.bounds.height - frame.height
                }
            default:
                //landscapeからfaceupなど
                if UIScreen.main.bounds.width > UIScreen.main.bounds.height {
                    if frame.origin.x < 0 {
                        frame.origin.x = 0
                    }
                    if frame.origin.y < 0 {
                        frame.origin.y = 0
                    }
                    if frame.maxX > UIScreen.main.bounds.width - buttonsViewSize.height {
                        frame.origin.x = UIScreen.main.bounds.width - (buttonsViewSize.height + frame.width)
                    }
                    if frame.maxY > UIScreen.main.bounds.height {
                        frame.origin.y = UIScreen.main.bounds.height - frame.height
                    }
                } else {
                    //portraitからfaceupなど
                    if frame.origin.x < 0 {
                        frame.origin.x = 0
                    }
                    if frame.origin.y < 0 {
                        frame.origin.y = 0
                    }
                    if frame.maxX > UIScreen.main.bounds.width {
                        frame.origin.x = UIScreen.main.bounds.width - frame.width
                    }
                    if frame.maxY > UIScreen.main.bounds.height - buttonsViewSize.height {
                        frame.origin.y = UIScreen.main.bounds.height - (buttonsViewSize.height + frame.height)
                    }
                }
            }
            //ロックされている
        } else {
            //親ビューのorientation
            switch UIDevice.current.orientation {
            case .portrait, .faceDown, .faceUp, .portraitUpsideDown:
                //黒板のオリエンテーション
                switch orientation {
                case .portrait:
                    if frame.origin.x < 0 {
                        frame.origin.x = 0
                    }
                    if frame.origin.y < 0 {
                        frame.origin.y = 0
                    }
                    if frame.maxX > UIScreen.main.bounds.width {
                        frame.origin.x = UIScreen.main.bounds.width - frame.width
                    }
                    if frame.maxY > UIScreen.main.bounds.height - buttonsViewSize.height {
                        frame.origin.y = UIScreen.main.bounds.height - (buttonsViewSize.height + frame.height)
                    }
                case .landscape:
                    if frame.origin.x < 0 {
                        frame.origin.x = 0
                    }
                    if frame.origin.y < 0 {
                        frame.origin.y = 0
                    }
                    if frame.maxX > UIScreen.main.bounds.width - buttonsViewSize.height {
                        frame.origin.x = UIScreen.main.bounds.width - (buttonsViewSize.height + frame.width )
                    }
                    if frame.maxY > UIScreen.main.bounds.height {
                        frame.origin.y = UIScreen.main.bounds.height - frame.height
                    }
                }
            case .landscapeRight, .landscapeLeft:
                //黒板のオリエンテーション
                switch orientation {
                case .portrait:
                    if frame.origin.x < 0 {
                        frame.origin.x = 0
                    }
                    if frame.origin.y < 0 {
                        frame.origin.y = 0
                    }
                    if frame.maxX > UIScreen.main.bounds.width {
                        frame.origin.x = UIScreen.main.bounds.width - frame.width
                    }
                    if frame.maxY > UIScreen.main.bounds.height - buttonsViewSize.height {
                        frame.origin.y = UIScreen.main.bounds.height - (buttonsViewSize.height + frame.height)
                    }
                case .landscape:
                    if frame.origin.x < 0 {
                        frame.origin.x = 0
                    }
                    if frame.origin.y < 0 {
                        frame.origin.y = 0
                    }
                    if frame.maxX > UIScreen.main.bounds.width - buttonsViewSize.height {
                        frame.origin.x = UIScreen.main.bounds.width - (buttonsViewSize.height + frame.width)
                    }
                    if frame.maxY > UIScreen.main.bounds.height {
                        frame.origin.y = UIScreen.main.bounds.height - frame.height
                    }
                }
            default:
                break
            }
        }
    }
    
    //回転時、画面からはみ出ないかチェック
    func boardCheckInScreen() {
        //true = デバイスがロックされていない
        if currentSettingsValue.deviceLockStatus {
            switch UIDevice.current.orientation {
            //landscape → portrait
            case .portrait:
                if frame.origin.x < 0 {
                    frame.origin.x = 0
                }
                if frame.origin.y < 0 {
                    frame.origin.y = 0
                }
                if frame.maxX > UIScreen.main.bounds.width {
                    frame.origin.x = UIScreen.main.bounds.width - frame.width
                }
                if frame.maxY > UIScreen.main.bounds.height - buttonsViewSize.height {
                    frame.origin.y = UIScreen.main.bounds.height - (buttonsViewSize.height + frame.height)
                }
            //portrait → landscape
            case .landscapeLeft, .landscapeRight:
                if frame.origin.x < 0 {
                    frame.origin.x = 0
                }
                if frame.origin.y < 0 {
                    frame.origin.y = 0
                }
                if frame.maxX > UIScreen.main.bounds.width - buttonsViewSize.height {
                    frame.origin.x = UIScreen.main.bounds.width - (buttonsViewSize.height + frame.width)
                }
                if frame.maxY > UIScreen.main.bounds.height {
                    frame.origin.y = UIScreen.main.bounds.height - frame.height
                }
            default:
                break
            }
        }
    }
    
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }

}
