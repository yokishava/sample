//
//  BoardInfo.swift
//  AutolayoutPractice
//
//  Created by 吉川昂広 on 2018/06/03.
//  Copyright © 2018年 takahiro yoshikawa. All rights reserved.
//

import Foundation
import UIKit

struct BoardInfo {
    
    //黒板
    let board: UIImage
    //撮影時の黒板の位置、サイズ
    let boardRect: CGRect
    //撮影時の黒板透過度
    let boardAlpha: Float
    //撮影時の黒板表示、非表示
    let boardHidden: Bool
    
}
