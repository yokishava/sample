//
//  ViewController.swift
//  AutolayoutPractice
//
//  Created by 吉川昂広 on 2018/05/27.
//  Copyright © 2018年 takahiro yoshikawa. All rights reserved.
//

import UIKit
import AVFoundation

class ViewController: UIViewController {
    
    private var portraitView: PortraitCameraView!
    private var landscapeView: LandscapeCameraView!
    private var currentValue: CurrentSettingsValue!
    
    private var session: AVCaptureSession!
    private var output: AVCapturePhotoOutput!
    private var layer: AVCaptureVideoPreviewLayer!
    
    private var boardID: Int?
    private var board: String?
    private var noBoard: Bool?
    
    private var boardView: BoardImageView!
    private var rect: CGRect!
    
    private var locationManager: LocationManager!
    
    private let sharpLayer = CAShapeLayer()
    private let line = UIBezierPath()
    
    init(boardID: Int?, board: String?, noBoard: Bool?) {
        self.boardID = boardID
        self.board = board
        self.noBoard = noBoard
        super.init(nibName: nil, bundle: nil)
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        print("view did road")
        //黒板があるかないかチェック
        if !(board == "none") {
            currentValue = CurrentSettingsValue(board: true)
            rect = CGRect(x: 0, y: self.view.frame.height * (3/4), width: self.view.frame.width, height: self.view.frame.height * (1/4))
        } else {
            currentValue = CurrentSettingsValue(board: false)
        }
        
        portraitView = PortraitCameraView(vc: self, currentSettingsValue: currentValue)
        landscapeView = LandscapeCameraView(vc: self, currentSettingsValue: currentValue)
        portraitView.delegate = self
        landscapeView.delegate = self
        
        locationManager = LocationManager()
        
        statusAuthorize()
        
        sessionConfigure()
        
        NotificationCenter.default.addObserver(self, selector: #selector(orientationDidChange(_:)), name: NSNotification.Name.UIDeviceOrientationDidChange, object: nil)
        let tapGesuture = UITapGestureRecognizer(target: self, action: #selector(focusPoint(gesture:)))
        self.view.addGestureRecognizer(tapGesuture)
    }
    
    private func sessionConfigure() {
        
        session = AVCaptureSession()
        output = AVCapturePhotoOutput()
        
        session.sessionPreset = AVCaptureSession.Preset.hd1920x1080
        
        if let device = AVCaptureDevice.default(for: .video) {
        
            do {
                let input = try AVCaptureDeviceInput(device: device)
                
                if session.canAddInput(input) {
                    session.addInput(input)
                    
                    if session.canAddOutput(output) {
                        session.addOutput(output)
                        
                        session.startRunning()
                        
                        layer = AVCaptureVideoPreviewLayer(session: session)
                        layer.videoGravity = .resizeAspect
                        layer.connection?.videoOrientation = .portrait
                        self.view.layer.addSublayer(layer)
                    }
                }
            } catch {
                print("error")
            }
        }
    }
    
    //viewのサイズ調整
    override func viewDidAppear(_ animated: Bool) {
        print("view did appear")
        //previewLayerのサイズ調整
        layer.frame = self.view.bounds
        //黒板の存在チェック
        guard boardView == nil else {
            //プレビュー画面から戻ってきたときは、buttonsのみでいい
            //黒板を表示させると2つになってしまう
            changeDisplayView()
            return
        }
        //黒板がまだない場合は黒板を生成する
        boardImageShow()
        //buttons
        changeDisplayView()
    }
    
    //回転対応
    @objc func orientationDidChange(_ notification: NSNotification) {
        guard shouldAutorotate else {
            return
        }
        for subView in self.view.subviews {
            subView.removeFromSuperview()
        }
        //previewLayer
        updatePreviewLayerConnection()
        layer.frame = self.view.bounds
        //黒板
        if boardView != nil {
            boardImageShow()
            boardView.orientaionChange(UIDevice.current.orientation, shouldAutorotate)
        }
        //buttons
        changeDisplayView()
    }
    
    private func boardImageShow() {
        guard !currentValue.boardNoThere else {
            boardView = BoardImageView(board: board!, buttons: rect, currentSettingsValue: currentValue)
            self.view.addSubview(boardView)
            //addSubviewで追加してからでなければ、alphaが回転時に引き継がれない
            boardView.updateBoardAlphaValue(currentValue.currentAlphaValue)
            return
        }
    }
    
    private func updatePreviewLayerConnection() {
        if let connection = layer.connection {
            switch UIDevice.current.orientation {
            case .portrait:
                connection.videoOrientation = .portrait
            case .landscapeLeft:
                connection.videoOrientation = .landscapeRight
            case .landscapeRight:
                connection.videoOrientation = .landscapeLeft
            case .portraitUpsideDown:
                connection.videoOrientation = .portraitUpsideDown
            default:
                break
            }
        }
    }

    //画面切替
    private func changeDisplayView() {
        if self.view.frame.width < self.view.frame.height {
            portraitView.layoutConstraint(view: self.view)
        } else {
            landscapeView.layoutConstraint(view: self.view)
        }
    }
    
    override var shouldAutorotate: Bool {
        return currentValue.deviceLockStatus
    }
    
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
}

extension ViewController: CameraButtonDelegate {
    
    //写真の撮影
    func takePicture(_ sender: UIButton) {
        let settings = AVCapturePhotoSettings()
        //フラッシュモードを設定
        switch currentValue.flashMode {
        case .auto:
            settings.flashMode = .auto
        case .on:
            settings.flashMode = .on
        default:
            settings.flashMode = .off
        }
        
        settings.isAutoStillImageStabilizationEnabled = true
        settings.isHighResolutionPhotoEnabled = false
        
        output.capturePhoto(with: settings, delegate: self)
    }
    
    //黒板の表示、非表示
    func boardDisplay(_ sender: UIButton) {
        boardView.boardDisplayChanged(currentValue.boardDisplay)
    }
    
    //グリッド線の表示、非表示
    func displayGridLine(_ sender: UIButton) {
        
        guard currentValue.gridLineDisplay else {
            //falseの時 = 非表示
            sharpLayer.strokeColor = UIColor.clear.cgColor
            return
        }
        //trueの時 = 表示
        for i in 1...10 {
            line.move(to: CGPoint(x: (self.view.frame.origin.x + 100) * CGFloat(i), y: self.view.frame.origin.y))
            line.addLine(to: CGPoint(x: (self.view.frame.origin.x + 100) * CGFloat(i), y: self.view.frame.height * 2))
        }
        for i in 1...10 {
            line.move(to: CGPoint(x: self.view.frame.origin.x, y: (self.view.frame.origin.y + 100) * CGFloat(i)))
            line.addLine(to: CGPoint(x: self.view.frame.width * 2, y: (self.view.frame.origin.y + 100) * CGFloat(i)))
        }
        sharpLayer.strokeColor = UIColor.white.cgColor
        sharpLayer.path = line.cgPath
        self.view.layer.insertSublayer(sharpLayer, at: 1)
    }
    
    //カメラのズームを更新
    func updateZoomValue(_ value: Float) {
        if let device = AVCaptureDevice.default(for: .video) {
            do {
                try device.lockForConfiguration()
                device.ramp(toVideoZoomFactor: CGFloat(value), withRate: 5.0)
                device.unlockForConfiguration()
            } catch {
                print("error zoom")
            }
        }
    }
    
    //黒板透過度の更新
    func updateAlphaValue(_ value: Float) {
        boardView.updateBoardAlphaValue(value)
    }
    
    //戻るボタンを押下してカメラを終了する
    func endCamera(_ sender: UIButton) {
        self.dismiss(animated: true, completion: nil)
    }
}

extension ViewController: AVCapturePhotoCaptureDelegate {
    
    //
    func photoOutput(_ output: AVCapturePhotoOutput, didFinishProcessingPhoto photoSampleBuffer: CMSampleBuffer?, previewPhoto previewPhotoSampleBuffer: CMSampleBuffer?, resolvedSettings: AVCaptureResolvedPhotoSettings, bracketSettings: AVCaptureBracketedStillImageSettings?, error: Error?) {
        if let photoSampleBuffer = photoSampleBuffer {
            let photoData = AVCapturePhotoOutput.jpegPhotoDataRepresentation(forJPEGSampleBuffer: photoSampleBuffer, previewPhotoSampleBuffer: previewPhotoSampleBuffer)
            
            //位置情報取得
            let location: (latitude: Double, longitude: Double) = getlocation()
            //撮影データ類セット
            let photoInfo = photoInfoInit(data: photoData!, location: location)
            
            guard boardView == nil else {
                //黒板があるとき
                let boardInfo = boardInfoInit(image: boardView.image!)
                showPreview(photo: photoInfo, board: boardInfo)
                return
            }
            //黒板がないとき
            showPreview(photo: photoInfo)
        }
    }
    
    //位置情報取得
    private func getlocation() -> (Double, Double) {
        let latitude = locationManager.locationManager.location?.coordinate.latitude
        let longitude = locationManager.locationManager.location?.coordinate.longitude
        return (latitude!, longitude!)
    }
    
    //PhotoInfo取得
    private func photoInfoInit(data: Data, location: (latitude: Double, longitude: Double)) -> PhotoInfo {
        let photoInfo = PhotoInfo(photoData: data,
                                  preset: currentValue.presetMode,
                                  boardID: self.boardID!,
                                  orientation: UIDevice.current.orientation,
                                  date: Date(),
                                  latitude: location.latitude, longitude: location.longitude)
        return photoInfo
    }
    
    //BoardInfo取得
    private func boardInfoInit(image: UIImage) -> BoardInfo {
        let boardInfo = BoardInfo(board: image,
                                  boardRect: boardView.frame,
                                  boardAlpha: Float(boardView.alpha),
                                  boardHidden: boardView.isHidden)
        return boardInfo
    }
    
    //プレビュー画面表示(黒板があるとき)
    private func showPreview(photo: PhotoInfo, board: BoardInfo) {
        let previewVC = PreviewController()
        previewVC.photoInfo = photo
        previewVC.boardInfo = board
        self.present(previewVC, animated: true, completion: nil)
    }
    
    //プレビュー画面表示(黒板がないとき)
    private func showPreview(photo: PhotoInfo) {
        let previewVC = PreviewController()
        previewVC.photoInfo = photo
        self.present(previewVC, animated: true, completion: nil)
    }
}

extension ViewController {
    
    //フォーカス
    @objc func focusPoint(gesture: UITapGestureRecognizer) {
        
        let tapPoint = gesture.location(ofTouch: 0, in: gesture.view)
        //ボタンの部分をタップしていた場合は処理終了
        if self.view.frame.width > self.view.frame.height {
            guard tapPoint.x < self.view.frame.width * (3/4) else {
                return
            }
        } else {
            guard tapPoint.y < self.view.frame.height * (3/4) else {
                return
            }
        }
        
        //フォーカス
        let focusPoint = CGPoint(x: tapPoint.y / layer.frame.height,
                                 y: 1.0 - tapPoint.x / layer.frame.width)
        if let device = AVCaptureDevice.default(for: .video) {
            do {
                try device.lockForConfiguration()
                if device.isFocusPointOfInterestSupported == true {
                    device.focusPointOfInterest = focusPoint
                    device.focusMode = .autoFocus
                }
                device.exposurePointOfInterest = focusPoint
                device.exposureMode = AVCaptureDevice.ExposureMode.continuousAutoExposure
                device.unlockForConfiguration()
            } catch {
                print("don't cahnge focus mode")
            }
        }
        
        //タップしたポイントに四角の枠を表示
        let focusView = UIView()
        let focusViewSize: CGFloat = 80
        focusView.frame.size = CGSize(width: focusViewSize, height: focusViewSize)
        focusView.center = tapPoint
        focusView.backgroundColor = UIColor.white.withAlphaComponent(0)
        focusView.layer.borderColor = UIColor.white.cgColor
        focusView.layer.borderWidth = 1.5
        focusView.alpha = 1
        self.view.addSubview(focusView)
        UIView.animate(withDuration: 0.5,
                       animations: {
                        focusView.frame.size = CGSize(width: focusViewSize * (2/3), height: focusViewSize * (2/3))
                        focusView.center = tapPoint
        }, completion: { Void in
            UIView.animate(withDuration: 0.5, animations: {
                focusView.alpha = 0
            })
        })
    }
}

//camera permission
extension ViewController {
    
    private func statusAuthorize() {
        switch AVCaptureDevice.authorizationStatus(for: .video) {
        case .authorized:
            print("authorize")
            break
        case .notDetermined:
            AVCaptureDevice.requestAccess(for: .video, completionHandler: { (granted) in
                if !(granted) {
                    self.settingDevice()
                }
            })
            break
        default:
            break
        }
    }
    
    private func settingDevice() {
        DispatchQueue.main.async(execute: { [unowned self] in
            let message = NSLocalizedString("カメラを使用するためには「設定」よりカメラの使用を有効にしてください", comment: "Alert message when the user has denied access to the camera")
            let alertController = UIAlertController(title: "カメラの使用が許可されていません", message: message, preferredStyle: .alert)
            alertController.addAction(UIAlertAction(title: NSLocalizedString("OK", comment: "Alert OK button"), style: .cancel, handler: { action in
                //self.changeEnabledButton()
            }))
            alertController.addAction(UIAlertAction(title: NSLocalizedString("Settings", comment: "Alert button to open Settings"), style: .default, handler: { action in
                if let appSettings = URL(string: UIApplicationOpenSettingsURLString) {
                    UIApplication.shared.open(appSettings, options: [:], completionHandler: nil)
                }
            }))
            self.present(alertController, animated: true, completion: nil)
        })
    }
    
}














