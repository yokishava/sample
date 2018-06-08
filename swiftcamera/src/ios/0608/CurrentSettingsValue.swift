//
//  CurrentSettingsValue.swift
//  AutolayoutPractice
//
//  Created by 吉川昂広 on 2018/05/28.
//  Copyright © 2018年 takahiro yoshikawa. All rights reserved.
//

import Foundation

enum FlashMode {
    //ON
    case on
    //OFF
    case off
    //Auto
    case auto
}

enum PresetMode: String {
    //120万画素
    case million = "120万画素"
    //200万画素
    case twoMillion = "200万画素"
    //300万画素
    case threeMillion = "300万画素"
    //Full
    case max = "FULL"
}

final class CurrentSettingsValue {
    
    //黒板があるかないか
    //trueの時存在する
    let boardNoThere: Bool
    //黒板表示・非表示
    var boardDisplay: Bool 
    //黒板透過表示・非表示
    //trueの時
    //ボタン = 白い
    //グリッド線表示
    var alphaSliderDisplay: Bool
    //グリッド表示・非表示
    var gridLineDisplay: Bool
    //表示設定表示・非表示
    //黒板表示、透過表示、グリッド表示も管理する
    var buttonsDisplay: Bool 
    //画素数
    var presetMode: PresetMode
    //フラッシュ
    var flashMode: FlashMode
    //デバイスロック
    var deviceLockStatus: Bool
    //黒板透過度のMaxとMinの値
    let alphaMin: Float = 0.0
    let alphaMax: Float = 1.0
    //ズームのMaxとMinの値
    let zoomMin: Float = 1.0
    let zoomMax: Float = 2.0
    //ズームのその時の値を保持
    var currentZoomValue: Float = 1.0
    //透過度のその時の値を保持
    var currentAlphaValue: Float = 0.0
    
    //黒板の位置サイズ情報
    var boardrect: CGRect!
    
    init(board: Bool) {
        //黒板があるかないか
        self.boardNoThere = board
        
        //表示設定ボタンは初期表示時はグレーアウト
        self.buttonsDisplay = false
        //画素数は初期は120万画素
        self.presetMode = .million
        //フラッシュは初期はオート
        self.flashMode = .auto
        //初期はデバイスがロックされている
        self.deviceLockStatus = false
        
        guard board else {
            //boardDisplay = false
            //黒板がない時
            self.boardDisplay = false
            self.alphaSliderDisplay = false
            self.gridLineDisplay = false
            return
        }
        
        //boardDisplay = true
        //黒板がある時
        self.boardDisplay = board
        self.alphaSliderDisplay = false
        self.gridLineDisplay = false
    }
}




