//
//  LandscapeButtonsView.swift
//  AutolayoutPractice
//
//  Created by 吉川昂広 on 2018/05/27.
//  Copyright © 2018年 takahiro yoshikawa. All rights reserved.
//

import Foundation
import UIKit
import AVFoundation

final class LandscapeCameraView: ButtonsConfigration {
    
    weak var viewController: UIViewController!
    
    weak var currentSettingsValue: CurrentSettingsValue!
    //黒のバックのview
    var buttonsView: UIView
    //シャッターボタン
    var captureButton: UIButton
    //黒板表示ボタン
    var boardButton: UIButton
    //黒板透過ボタン
    var alphaButton: UIButton
    //グリッドボタン
    var gridButton: UIButton
    //表示設定ボタン
    var displaySettingsButton: UIButton
    //画素数変更ボタン
    var presetButton: UIButton
    //フラッシュボタン
    var flashButton: UIButton
    //デバイスロックボタン
    var deviceLockButton: UIButton
    //ズームスライダー
    var zoomSlider: UISlider
    //透過スライダー
    var alphaSlider: UISlider
    //戻るボタン
    var returnButton: UIButton
    //ズームアイコン
    var zoomIcon: UIImageView
    //透過アイコン
    var alphaIcon: UIImageView
    //黒板ラベル
    var boardLabel: UILabel
    //グリッドラベル
    var gridLabel: UILabel
    
    var delegate: CameraButtonDelegate?
    
    init(vc: UIViewController, currentSettingsValue: CurrentSettingsValue) {
        self.viewController = vc
        self.currentSettingsValue = currentSettingsValue
        
        buttonsView = UIView()
        captureButton = UIButton()
        boardButton = UIButton()
        alphaButton = UIButton()
        gridButton = UIButton()
        displaySettingsButton = UIButton()
        presetButton = UIButton()
        flashButton = UIButton()
        deviceLockButton = UIButton()
        zoomSlider = UISlider()
        alphaSlider = UISlider()
        returnButton = UIButton()
        zoomIcon = UIImageView(image: UIImage(named: "zoom_in.png"))
        alphaIcon = UIImageView(image: UIImage(named: "humidity.png"))
        boardLabel = UILabel()
        gridLabel = UILabel()
        
        zoomSlider.minimumValue = currentSettingsValue.zoomMin
        zoomSlider.maximumValue = currentSettingsValue.zoomMax
        alphaSlider.minimumValue = currentSettingsValue.alphaMin
        alphaSlider.maximumValue = currentSettingsValue.alphaMax
        
        captureButton.addTarget(self, action: #selector(captureButtonTapped(_:)), for: .touchUpInside)
        boardButton.addTarget(self, action: #selector(boardButtonTapped(_:)), for: .touchUpInside)
        alphaButton.addTarget(self, action: #selector(alphaButtonTapped(_:)), for: .touchUpInside)
        gridButton.addTarget(self, action: #selector(gridButtonTapped(_:)), for: .touchUpInside)
        displaySettingsButton.addTarget(self, action: #selector(displaySettingsButtonTapped(_:)), for: .touchUpInside)
        presetButton.addTarget(self, action: #selector(presetButtonTapped(_:)), for: .touchUpInside)
        flashButton.addTarget(self, action: #selector(flashButtonTapped(_:)), for: .touchUpInside)
        deviceLockButton.addTarget(self, action: #selector(deviceLockButtonTapped(_:)), for: .touchUpInside)
        zoomSlider.addTarget(self, action: #selector(zoomSliderSwiped(_:)), for: .valueChanged)
        alphaSlider.addTarget(self, action: #selector(alphaSliderSwiped(_:)), for: .valueChanged)
        returnButton.addTarget(self, action: #selector(returnButtonTapped(_:)), for: .touchUpInside)
        
        guard currentSettingsValue.boardNoThere else {
            //false = 黒板がないとき
            boardButton.isEnabled = false
            alphaButton.isEnabled = false
            return
        }
        
    }
    
    //レイアウトの決定
    func layoutConstraint(view: UIView) {
        
        let buttonsViewWidth = view.frame.width * (1/4)
        let captureButtonHeightAndWidth = buttonsViewWidth * (1/2.5)
        let boardButtonWidth = buttonsViewWidth / 2
        let boardButtonHeight = boardButtonWidth * (1.5/3)
        
        //ボタンのバックviewをaddSubView
        otherComponentConfig(component: buttonsView)
        viewController.view.addSubview(buttonsView)
        //シャッターボタンをaddSubView
        otherComponentConfig(component: captureButton)
        buttonsView.addSubview(captureButton)
        
        presetButton.setTitle(UpdatePresetText(status: currentSettingsValue.presetMode), for: .normal)
        presetButton.titleLabel?.adjustsFontSizeToFitWidth = true
        buttonStatusSettings(button: presetButton, status: true)
        buttonsView.addSubview(presetButton)
        
        flashButton.setImage(UIImage(named: UpdateFlashImage(status: currentSettingsValue.flashMode)), for: .normal)
        flashButton.imageView?.contentMode = .scaleAspectFit
        flashButton.contentHorizontalAlignment = .fill
        flashButton.contentVerticalAlignment = .fill
        buttonStatusSettings(button: flashButton, status: true)
        buttonsView.addSubview(flashButton)
        
        deviceLockButton.setImage(UIImage(named: UpdateDevicelockImage(status: currentSettingsValue.deviceLockStatus)), for: .normal)
        deviceLockButton.imageView?.contentMode = .scaleAspectFit
        deviceLockButton.contentHorizontalAlignment = .fill
        deviceLockButton.contentVerticalAlignment = .fill
        buttonStatusSettings(button: deviceLockButton, status: true)
        buttonsView.addSubview(deviceLockButton)
        
        returnButton.setTitle("戻る", for: .normal)
        buttonStatusSettings(button: returnButton, status: true)
        buttonsView.addSubview(returnButton)

        zoomSlider.translatesAutoresizingMaskIntoConstraints = false
        viewController.view.addSubview(zoomSlider)
        alphaSlider.translatesAutoresizingMaskIntoConstraints = false
        viewController.view.addSubview(alphaSlider)
        
        zoomIcon.translatesAutoresizingMaskIntoConstraints = false
        viewController.view.addSubview(zoomIcon)
        alphaIcon.translatesAutoresizingMaskIntoConstraints = false
        viewController.view.addSubview(alphaIcon)
        
        zoomSlider.value = currentSettingsValue.currentZoomValue
        alphaSlider.value = currentSettingsValue.currentAlphaValue
        
        boardButton.setTitle("黒板", for: .normal)
        buttonStatusSettings(button: boardButton, status: currentSettingsValue.boardDisplay)
        viewController.view.addSubview(boardButton)
        
        boardLabel.text = "黒板"
        boardLabel.translatesAutoresizingMaskIntoConstraints = false
        labelStatusSettings(label: boardLabel, status: currentSettingsValue.boardDisplay)
        buttonsView.addSubview(boardLabel)
        
        alphaButton.setTitle("黒板透過度", for: .normal)
        alphaButton.titleLabel?.adjustsFontSizeToFitWidth = true
        buttonStatusSettings(button: alphaButton, status: currentSettingsValue.alphaSliderDisplay)
        viewController.view.addSubview(alphaButton)
        
        gridButton.setTitle("グリッド", for: .normal)
        gridButton.titleLabel?.adjustsFontSizeToFitWidth = true
        buttonStatusSettings(button: gridButton, status: currentSettingsValue.gridLineDisplay)
        viewController.view.addSubview(gridButton)
        
        gridLabel.text = "グリッド"
        gridLabel.translatesAutoresizingMaskIntoConstraints = false
        labelStatusSettings(label: gridLabel, status: currentSettingsValue.gridLineDisplay)
        buttonsView.addSubview(gridLabel)
        
        buttonsHiddenSettings(button: boardButton, hidden: currentSettingsValue.buttonsDisplay)
        buttonsHiddenSettings(button: alphaButton, hidden: currentSettingsValue.buttonsDisplay)
        buttonsHiddenSettings(button: gridButton, hidden: currentSettingsValue.buttonsDisplay)
        
        //透過スライダーの表示状態
        alphaSlider.isHidden = currentSettingsValueUpdate(currentStatus: currentSettingsValue.alphaSliderDisplay)
        alphaIcon.isHidden = currentSettingsValueUpdate(currentStatus: currentSettingsValue.alphaSliderDisplay)
        
        displaySettingsButton.setTitle("表示設定", for: .normal)
        displaySettingsButton.titleLabel?.adjustsFontSizeToFitWidth = true
        buttonStatusSettings(button: displaySettingsButton, status: currentSettingsValue.buttonsDisplay)
        buttonsView.addSubview(displaySettingsButton)
        
        //ボタンのバックviewの制約追加
        buttonsView.trailingAnchor.constraint(equalTo: viewController.view.trailingAnchor).isActive = true
        buttonsView.topAnchor.constraint(equalTo: viewController.view.topAnchor).isActive = true
        buttonsView.heightAnchor.constraint(equalTo: viewController.view.heightAnchor).isActive = true
        buttonsView.widthAnchor.constraint(equalToConstant: buttonsViewWidth).isActive = true
        
        //ズームアイコンの制約追加
        zoomIcon.leadingAnchor.constraint(equalTo: viewController.view.leadingAnchor, constant: 20).isActive = true
        zoomIcon.bottomAnchor.constraint(equalTo: viewController.view.bottomAnchor, constant: -10).isActive = true
        zoomIcon.widthAnchor.constraint(equalToConstant: boardButtonHeight).isActive = true
        zoomIcon.heightAnchor.constraint(equalToConstant: boardButtonHeight).isActive = true
        
        //ズームスライダーの制約追加
        zoomSlider.leadingAnchor.constraint(equalTo: zoomIcon.trailingAnchor, constant: 10).isActive = true
        //zoomSlider.widthAnchor.constraint(equalToConstant: viewController.view.frame.width - buttonsViewWidth - 50).isActive = true
        zoomSlider.trailingAnchor.constraint(equalTo: buttonsView.leadingAnchor, constant: -40).isActive = true
        zoomSlider.centerYAnchor.constraint(equalTo: zoomIcon.centerYAnchor).isActive = true
        zoomSlider.heightAnchor.constraint(equalToConstant: 20).isActive = true
        
        //透過アイコンの制約追加
        alphaIcon.leadingAnchor.constraint(equalTo: zoomIcon.leadingAnchor).isActive = true
        alphaIcon.bottomAnchor.constraint(equalTo: zoomIcon.topAnchor, constant: -10).isActive = true
        alphaIcon.widthAnchor.constraint(equalTo: zoomIcon.widthAnchor).isActive = true
        alphaIcon.heightAnchor.constraint(equalTo: zoomIcon.heightAnchor).isActive = true
        
        //透過スライダーの制約追加
        alphaSlider.leadingAnchor.constraint(equalTo: zoomSlider.leadingAnchor).isActive = true
        alphaSlider.widthAnchor.constraint(equalTo: zoomSlider.widthAnchor).isActive = true
        alphaSlider.centerYAnchor.constraint(equalTo: alphaIcon.centerYAnchor).isActive = true
        alphaSlider.heightAnchor.constraint(equalTo: zoomSlider.heightAnchor).isActive = true
        
        //シャッターボタンの制約追加
        captureButton.trailingAnchor.constraint(equalTo: buttonsView.trailingAnchor, constant: -5).isActive = true
        captureButton.widthAnchor.constraint(equalToConstant: captureButtonHeightAndWidth).isActive = true
        captureButton.centerYAnchor.constraint(equalTo: buttonsView.centerYAnchor).isActive = true
        captureButton.heightAnchor.constraint(equalToConstant: captureButtonHeightAndWidth).isActive = true
        //シャッターボタンを丸くする
        captureButton.layer.cornerRadius = captureButtonHeightAndWidth / 2
        
        //黒板表示ボタンの制約追加
        boardButton.leadingAnchor.constraint(equalTo: buttonsView.leadingAnchor, constant: 3).isActive = true
        boardButton.topAnchor.constraint(equalTo: buttonsView.topAnchor, constant: 5).isActive = true
        boardButton.widthAnchor.constraint(equalToConstant: boardButtonWidth - 3).isActive = true
        boardButton.heightAnchor.constraint(equalToConstant: boardButtonHeight).isActive = true
        
        //黒板透過ボタンの制約追加
        alphaButton.leadingAnchor.constraint(equalTo: boardButton.leadingAnchor).isActive = true
        alphaButton.topAnchor.constraint(equalTo: boardButton.bottomAnchor, constant: 5).isActive = true
        alphaButton.widthAnchor.constraint(equalTo: boardButton.widthAnchor).isActive = true
        alphaButton.heightAnchor.constraint(equalTo: boardButton.heightAnchor).isActive = true
        
        //グリッドボタンの制約追加
        gridButton.leadingAnchor.constraint(equalTo: boardButton.leadingAnchor).isActive = true
        gridButton.topAnchor.constraint(equalTo: alphaButton.bottomAnchor, constant: 5).isActive = true
        gridButton.widthAnchor.constraint(equalTo: alphaButton.widthAnchor).isActive = true
        gridButton.heightAnchor.constraint(equalTo: alphaButton.heightAnchor).isActive = true
        
        //表示設定ボタンの制約追加
        displaySettingsButton.topAnchor.constraint(equalTo: boardButton.topAnchor).isActive = true
        displaySettingsButton.leadingAnchor.constraint(equalTo: boardButton.trailingAnchor, constant: 3).isActive = true
        displaySettingsButton.widthAnchor.constraint(equalToConstant: boardButtonWidth - 3).isActive = true
        displaySettingsButton.heightAnchor.constraint(equalTo: boardButton.heightAnchor).isActive = true
        
        //黒板ラベルの制約追加
        boardLabel.topAnchor.constraint(equalTo: displaySettingsButton.bottomAnchor, constant: 5).isActive = true
        boardLabel.leadingAnchor.constraint(equalTo: displaySettingsButton.leadingAnchor).isActive = true
        boardLabel.trailingAnchor.constraint(equalTo: displaySettingsButton.trailingAnchor).isActive = true
        boardLabel.heightAnchor.constraint(equalToConstant: boardButtonHeight / 2).isActive = true
        
        //グリッドラベルの制約追加
        gridLabel.topAnchor.constraint(equalTo: boardLabel.bottomAnchor, constant: 5).isActive = true
        gridLabel.leadingAnchor.constraint(equalTo: boardLabel.leadingAnchor).isActive = true
        gridLabel.trailingAnchor.constraint(equalTo: boardLabel.trailingAnchor).isActive = true
        gridLabel.heightAnchor.constraint(equalTo: boardLabel.heightAnchor).isActive = true
        
        //画素数変更ボタンの制約追加
        presetButton.topAnchor.constraint(equalTo: captureButton.bottomAnchor, constant: 15).isActive = true
        presetButton.leadingAnchor.constraint(equalTo: displaySettingsButton.leadingAnchor).isActive = true
        presetButton.widthAnchor.constraint(equalTo: displaySettingsButton.widthAnchor).isActive = true
        presetButton.heightAnchor.constraint(equalTo: displaySettingsButton.heightAnchor).isActive = true
        
        //フラッシュボタンの制約追加
        flashButton.topAnchor.constraint(equalTo: presetButton.bottomAnchor, constant: 5).isActive = true
        flashButton.leadingAnchor.constraint(equalTo: presetButton.leadingAnchor).isActive = true
        flashButton.widthAnchor.constraint(equalTo: presetButton.widthAnchor).isActive = true
        flashButton.heightAnchor.constraint(equalTo: presetButton.heightAnchor).isActive = true
        
        //デバイスロックボタンの制約追加
        deviceLockButton.topAnchor.constraint(equalTo: flashButton.bottomAnchor, constant: 5).isActive = true
        deviceLockButton.leadingAnchor.constraint(equalTo: flashButton.leadingAnchor).isActive = true
        deviceLockButton.widthAnchor.constraint(equalTo: flashButton.widthAnchor).isActive = true
        deviceLockButton.heightAnchor.constraint(equalTo: flashButton.heightAnchor).isActive = true
        
        //戻るボタンの制約追加
        returnButton.leadingAnchor.constraint(equalTo: boardButton.leadingAnchor).isActive = true
        returnButton.widthAnchor.constraint(equalTo: boardButton.widthAnchor).isActive = true
        returnButton.topAnchor.constraint(equalTo: deviceLockButton.topAnchor).isActive = true
        returnButton.heightAnchor.constraint(equalTo: deviceLockButton.heightAnchor).isActive = true
    }
}

extension LandscapeCameraView {
    
    @objc func captureButtonTapped(_ sender: UIButton) {
        self.delegate?.takePicture(sender)
    }
    
    @objc func boardButtonTapped(_ sender: UIButton) {
        //
        currentSettingsValue.boardDisplay = currentSettingsValueUpdate(currentStatus: currentSettingsValue.boardDisplay)
        //
        buttonStatusSettings(button: boardButton, status: currentSettingsValue.boardDisplay)
        //
        labelStatusSettings(label: boardLabel, status: currentSettingsValue.boardDisplay)
        //
        self.delegate?.boardDisplay(sender)
    }
    
    @objc func alphaButtonTapped(_ sender: UIButton) {
        //
        currentSettingsValue.alphaSliderDisplay = currentSettingsValueUpdate(currentStatus: currentSettingsValue.alphaSliderDisplay)
        //
        buttonStatusSettings(button: alphaButton, status: currentSettingsValue.alphaSliderDisplay)
        //
        alphaSlider.isHidden = currentSettingsValueUpdate(currentStatus: currentSettingsValue.alphaSliderDisplay)
        alphaIcon.isHidden = currentSettingsValueUpdate(currentStatus: currentSettingsValue.alphaSliderDisplay)
    }
    
    @objc func gridButtonTapped(_ sender: UIButton) {
        //
        currentSettingsValue.gridLineDisplay = currentSettingsValueUpdate(currentStatus: currentSettingsValue.gridLineDisplay)
        //
        buttonStatusSettings(button: gridButton, status: currentSettingsValue.gridLineDisplay)
        //
        labelStatusSettings(label: gridLabel, status: currentSettingsValue.gridLineDisplay)
        //
        self.delegate?.displayGridLine(sender)
    }
    
    @objc func displaySettingsButtonTapped(_ sender: UIButton) {
        //
        currentSettingsValue.buttonsDisplay = currentSettingsValueUpdate(currentStatus: currentSettingsValue.buttonsDisplay)
        //
        buttonsHiddenSettings(button: boardButton, hidden: currentSettingsValue.buttonsDisplay)
        //
        buttonsHiddenSettings(button: alphaButton, hidden: currentSettingsValue.buttonsDisplay)
        //
        buttonsHiddenSettings(button: gridButton, hidden: currentSettingsValue.buttonsDisplay)
        //
        buttonStatusSettings(button: displaySettingsButton, status: currentSettingsValue.buttonsDisplay)
    }
    
    @objc func presetButtonTapped(_ sender: UIButton) {
        //
        currentSettingsValue.presetMode = UpdatecurrentPreset(status: currentSettingsValue.presetMode)
        //
        presetButton.setTitle(UpdatePresetText(status: currentSettingsValue.presetMode), for: .normal)
    }
    
    @objc func flashButtonTapped(_ sender: UIButton) {
        //
        currentSettingsValue.flashMode = UpdateCurrentFlash(status: currentSettingsValue.flashMode)
        //
        flashButton.setImage(UIImage(named: UpdateFlashImage(status: currentSettingsValue.flashMode)), for: .normal)
    }
    
    @objc func deviceLockButtonTapped(_ sender: UIButton) {
        //
        currentSettingsValue.deviceLockStatus = currentSettingsValueUpdate(currentStatus: currentSettingsValue.deviceLockStatus)
        //
        deviceLockButton.setImage(UIImage(named: UpdateDevicelockImage(status: currentSettingsValue.deviceLockStatus)), for: .normal)
    }
    
    @objc func zoomSliderSwiped(_ sender: UISlider) {
        currentSettingsValue.currentZoomValue = sender.value
        self.delegate?.updateZoomValue(currentSettingsValue.currentZoomValue)
    }
    
    @objc func alphaSliderSwiped(_ sender: UISlider) {
        currentSettingsValue.currentAlphaValue = sender.value
        self.delegate?.updateAlphaValue(currentSettingsValue.currentAlphaValue)
    }
    
    @objc func returnButtonTapped(_ sender: UIButton) {
        self.delegate?.endCamera(sender)
    }
}

















