//
//  ButtonsConfigration.swift
//  AutolayoutPractice
//
//  Created by 吉川昂広 on 2018/05/27.
//  Copyright © 2018年 takahiro yoshikawa. All rights reserved.
//

import Foundation
import UIKit
import AVFoundation

protocol ButtonsViewProperty: class {
    //ボタンを支える黒いview
    var buttonsView: UIView { get set }
    //シャッターボタン
    var captureButton: UIButton { get set }
    //表示、非表示ボタン
    var boardButton: UIButton { get set }
    //透過ボタン
    var alphaButton: UIButton { get set }
    //グリッドボタン
    var gridButton: UIButton { get set }
    //表示設定ボタン
    var displaySettingsButton: UIButton { get set }
    //画素数ボタン
    var presetButton: UIButton { get set }
    //フラッシュボタン
    var flashButton: UIButton { get set }
    //デバイスロックボタン
    var deviceLockButton: UIButton { get set }
    //ズームスライダー
    var zoomSlider: UISlider { get set }
    //透過スライダー
    var alphaSlider: UISlider { get set }
    //戻るボタン
    var returnButton: UIButton { get set }
    //ズームアイコン
    var zoomIcon: UIImageView { get set }
    //透過アイコン
    var alphaIcon: UIImageView { get set }
    //レイアウトの制約、各ボタンの状態決定
    func layoutConstraint(view: UIView)
    //黒板ラベル
    var boardLabel: UILabel { get set }
    //グリッドラベル
    var gridLabel: UILabel { get set }
}

protocol ButtonsConfigration: ButtonsViewProperty {
    
    func otherComponentConfig<T>(component: T) where T: UIView
    
}

extension ButtonsConfigration {
    
    //押下時のボタンの状態を更新
    func buttonStatusSettings(button: UIButton, status: Bool) {
        button.backgroundColor = UIColor.clear
        button.layer.borderWidth = 2.0
        button.layer.cornerRadius = 5
        button.translatesAutoresizingMaskIntoConstraints = false
        if !status {
            button.layer.borderColor = UIColor.gray.cgColor
            button.setTitleColor(UIColor.gray, for: .normal)
        } else {
            button.layer.borderColor = UIColor.white.cgColor
            button.setTitleColor(UIColor.white, for: .normal)
        }
    }
    
    //
    func currentSettingsValueUpdate(currentStatus: Bool) -> Bool {
        guard currentStatus else {
            //falseの時
            return true
        }
        return false
    }
    
    //黒板表示ボタン
    //透過ボタン
    //グリッドボタン
    //3つの表示、非表示の状態を更新
    func buttonsHiddenSettings(button: UIButton, hidden: Bool) {
        if hidden {
            button.isHidden = true
        } else {
            button.isHidden = false
        }
    }
    
    //caseを返す
    func UpdatecurrentPreset(status: PresetMode) -> PresetMode {
        switch status {
        case .million:
            return PresetMode.twoMillion
        case .twoMillion:
            return PresetMode.threeMillion
        case .threeMillion:
            return PresetMode.max
        default:
            return PresetMode.million
        }
    }
    
    //caseからボタンのステータスを更新
    func UpdatePresetText(status: PresetMode) -> String {
        switch status {
        case .million:
            return PresetMode.million.rawValue
        case .twoMillion:
            return PresetMode.twoMillion.rawValue
        case .threeMillion:
            return PresetMode.threeMillion.rawValue
        default:
            return PresetMode.max.rawValue
        }
    }
    
    //caseを返す
    func UpdateCurrentFlash(status: FlashMode) -> FlashMode {
        switch status {
        case .auto:
            return FlashMode.off
        case .off:
            return FlashMode.on
        default:
            return FlashMode.auto
        }
    }
    
    func UpdateFlashImage(status: FlashMode) -> String {
        switch status {
        case .auto:
            return "ic_flash_auto_image.png"
        case .off:
            return "ic_flash_off_image.png"
        default:
            return "ic_flash_on_image.png"
        }
    }
    
    func UpdateDevicelockImage(status: Bool) -> String {
        guard status else {
            //falseの時
            return "ic_rotate_lock.png"
        }
        return "ic_rotate_on.png"
    }
    
    func otherComponentConfig<T>(component: T) where T: UIView {
        if T.self == UIView.self {
            component.backgroundColor = UIColor.black
            component.translatesAutoresizingMaskIntoConstraints = false
        } else if T.self == UIButton.self {
            component.backgroundColor = UIColor.white
            component.translatesAutoresizingMaskIntoConstraints = false
        }
    }
    
    func labelStatusSettings(label: UILabel, status: Bool) {
        label.textAlignment = .center
        label.adjustsFontSizeToFitWidth = true
        if status {
            label.textColor = UIColor.white
        } else {
            label.textColor = UIColor.gray
        }
    }
}
