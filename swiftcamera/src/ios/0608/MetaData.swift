//
//  MetaData.swift
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/23.
//

import Foundation

struct MetaData  {
    
    struct ExifData {
        //絞り値
        var apertureValue: CGFloat
        //輝度値
        var brightnessValue: CGFloat
        //色空間情報
        var colorSpace: Int
        //コンポーネント構成
        var componentsConfiguration: [Int]
        //デジタル化日時
        var dateTimeDigitized: String
        //オリジナルの日時
        var dateTimeOriginal: String
        //Exifバージョン
        var exifVersion: [Int]
        //露出補正値
        var exposureBiasValue: Int
        //露出モード
        var exposureMode: Int
        //露出プログラム
        var exposureProgram: Int
        //露出時間
        var exposureTime: CGFloat
        //Fナンバー
        var fNumber: CGFloat
        //フラッシュ
        var flash: Int
        //対応FlashPixバージョン
        var flashPixelVersion: [Int]
        //35mm換算レンズ焦点距離
        var focalLenIn35mmFilm: Int
        //レンズ焦点距離
        var focalLength: CGFloat
        //ISOスピードレート
        var isoSpeedRatings: [Int]
        //レンズ製造元
        var lensMake: String
        //レンズ名
        var lensModel: String
        //レンズ仕様
        var lensSpecification: [CGFloat]
        //測光方式
        var meteringMode: Int
        //ピクセル幅
        //var pixelXDimension: Int
        //ピクセル高さ
        //var pixelYDimension: Int
        //撮影シーンタイプ
        var sceneCaptureType: Int
        //シーンタイプ
        var sceneType: Int
        //センサー方式
        var sensingMethod: Int
        //シャッタースピード
        var shutterSpeedValue: CGFloat
        //被写体領域
        var subjectArea: [Int]
        //デジタル化時刻
        var subsecTimeDigitized: String
        //オリジナル作成時刻
        var subsecTimeOriginal: String
        //ホワイトバランス
        var whiteBalance: Int
    }
    
    struct TiffData {
        //日時
        var dateTime: String
        //製造元
        var make: String
        //機種名
        var model: String
        //方向
        var orientaion: Int
        //解像度の単位
        var resolutionUnit: Int
        //使用ソフトウェア名
        var software: String
        //幅の解像度
        var xResolution: Int
        //高さの解像度
        var yResolution: Int
        //イメージの説明
        var imageDescription: String
    }
    
    //幅（ピクセル）
    //var pixelWidth: Int
    //高さ（ピクセル）
    //var pixelHeight: Int
    //幅（DPI）
    var dpiWidth: Int
    //高さ（DPI）
    var dpiHeight: Int
    //プロファイル名
    var profileName: String
    //方向
    var orientation: Int
    //カラーモデル
    var colorModel: String
    //exif辞書
    var exif: ExifData
    //tiff辞書
    var tiff: TiffData
    //緯度
    var latitude: Double
    //経度
    var longitude: Double
}


