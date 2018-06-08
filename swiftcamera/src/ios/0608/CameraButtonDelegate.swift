//
//  CameraButtonDelegate.swift
//  AutolayoutPractice
//
//  Created by 吉川昂広 on 2018/05/27.
//  Copyright © 2018年 takahiro yoshikawa. All rights reserved.
//

import Foundation
import UIKit

protocol CameraButtonDelegate: class {
    //写真撮影
    func takePicture(_ sender: UIButton)
    //黒板表示・非表示
    func boardDisplay(_ sender: UIButton)
    //黒板透過度スライダー表示・非表示
    //func changeBoardAlpha(_ sender: UIButton)
    //グリッド線表示・非表示
    func displayGridLine(_ sender: UIButton)
    //ズームスライダーを移動させた時
    func updateZoomValue(_ value: Float)
    //透過スライダーを移動させた時
    func updateAlphaValue(_ value: Float)
    //戻るボタンを押下したとき
    func endCamera(_ sender: UIButton)

}


