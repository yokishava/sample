//
//  PhotoInfo.swift
//  AutolayoutPractice
//
//  Created by 吉川昂広 on 2018/06/03.
//  Copyright © 2018年 takahiro yoshikawa. All rights reserved.
//

import Foundation
import UIKit

struct PhotoInfo {
    
    //撮影写真データ
    let photoData: Data
    //撮影時の画素数
    let preset: PresetMode
    //黒板ID
    let boardID: Int
    //撮影時のオリエンテーション
    let orientation: UIDeviceOrientation
    //撮影日時
    let date: Date
    //緯度
    let latitude: Double
    //経度
    let longitude: Double
    
}
