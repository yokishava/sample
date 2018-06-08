//
//  ExifHelper.swift
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/23.
//

import Foundation
import ImageIO
import MobileCoreServices

fileprivate protocol ExifMethod {
    func getExifProperties(image: CIImage, gpsInfo: (latitude: Double, longitude: Double)) -> MetaData
    func writeExifProperties(path: String, data: MetaData)
}

struct ExifHelper {
    
    let fileNameDate: Date
    
    init(fileNameDate: Date) {
        self.fileNameDate = fileNameDate
    }
    
}

extension ExifHelper: ExifMethod {
    
    //書き込み
    func writeExifProperties(path: String, data: MetaData) {
        let url = URL(fileURLWithPath: path)
        let cgImage = CGImageSourceCreateWithURL(url as CFURL, nil)
        
        //exif情報セット
        let exif = NSMutableDictionary()
        //絞り値
        exif.setObject(data.exif.apertureValue, forKey: kCGImagePropertyExifApertureValue as! NSCopying)
        //輝度値
        exif.setObject(data.exif.brightnessValue, forKey: kCGImagePropertyExifBrightnessValue as! NSCopying)
        //色空間情報
        exif.setObject(data.exif.colorSpace, forKey: kCGImagePropertyExifColorSpace as! NSCopying)
        //コンポーネント構成
        exif.setObject(data.exif.componentsConfiguration, forKey: kCGImagePropertyExifComponentsConfiguration as! NSCopying)
        //デジタル化日時
        exif.setObject(data.exif.dateTimeDigitized, forKey: kCGImagePropertyExifDateTimeDigitized as! NSCopying)
        //オリジナルの日時
        exif.setObject(data.exif.dateTimeOriginal, forKey: kCGImagePropertyExifDateTimeOriginal as! NSCopying)
        //Exifバージョン
        exif.setObject(data.exif.exifVersion, forKey: kCGImagePropertyExifVersion as! NSCopying)
        //露出補正値
        exif.setObject(data.exif.exposureBiasValue, forKey: kCGImagePropertyExifExposureBiasValue as! NSCopying)
        //露出モード
        exif.setObject(data.exif.exposureMode, forKey: kCGImagePropertyExifExposureMode as! NSCopying)
        //露出プログラム
        exif.setObject(data.exif.exposureProgram, forKey: kCGImagePropertyExifExposureProgram as! NSCopying)
        //露出時間
        exif.setObject(data.exif.exposureTime, forKey: kCGImagePropertyExifExposureTime as! NSCopying)
        //Fナンバー
        exif.setObject(data.exif.fNumber, forKey: kCGImagePropertyExifFNumber as! NSCopying)
        //フラッシュ
        exif.setObject(data.exif.flash, forKey: kCGImagePropertyExifFlash as! NSCopying)
        //対応FlashPixバージョン
        exif.setObject(data.exif.flashPixelVersion, forKey: kCGImagePropertyExifFlashPixVersion as! NSCopying)
        //35mm換算レンズ焦点距離
        exif.setObject(data.exif.focalLenIn35mmFilm, forKey: kCGImagePropertyExifFocalLenIn35mmFilm as! NSCopying)
        //レンズ焦点距離
        exif.setObject(data.exif.focalLength, forKey: kCGImagePropertyExifFocalLength as! NSCopying)
        //ISOスピードレート
        exif.setObject(data.exif.isoSpeedRatings, forKey: kCGImagePropertyExifISOSpeedRatings as! NSCopying)
        //レンズ製造元
        exif.setObject(data.exif.lensMake, forKey: kCGImagePropertyExifLensMake as! NSCopying)
        //レンズ名
        exif.setObject(data.exif.lensModel, forKey: kCGImagePropertyExifLensModel as! NSCopying)
        //レンズ仕様
        exif.setObject(data.exif.lensSpecification, forKey: kCGImagePropertyExifLensSpecification as! NSCopying)
        //測光方式
        exif.setObject(data.exif.meteringMode, forKey: kCGImagePropertyExifMeteringMode as! NSCopying)
        //ピクセル幅
        //exif.setObject(data.exif.pixelXDimension, forKey: kCGImagePropertyExifPixelXDimension as! NSCopying)
        //ピクセル高さ
        //exif.setObject(data.exif.pixelYDimension, forKey: kCGImagePropertyExifPixelYDimension as! NSCopying)
        //撮影シーンタイプ
        exif.setObject(data.exif.sceneCaptureType, forKey: kCGImagePropertyExifSceneCaptureType as! NSCopying)
        //シーンタイプ
        exif.setObject(data.exif.sceneType, forKey: kCGImagePropertyExifSceneType as! NSCopying)
        //センサー方式
        exif.setObject(data.exif.sensingMethod, forKey: kCGImagePropertyExifSensingMethod as! NSCopying)
        //シャッタースピード
        exif.setObject(data.exif.shutterSpeedValue, forKey: kCGImagePropertyExifShutterSpeedValue as! NSCopying)
        //被写体領域
        exif.setObject(data.exif.subjectArea, forKey: kCGImagePropertyExifSubjectArea as! NSCopying)
        //デジタル化時刻
        exif.setObject(data.exif.subsecTimeDigitized, forKey: kCGImagePropertyExifSubsecTimeDigitized as! NSCopying)
        //オリジナル作成時刻
        exif.setObject(data.exif.subsecTimeOriginal, forKey: kCGImagePropertyExifSubsecTimeOriginal as! NSCopying)
        //ホワイトバランス
        exif.setObject(data.exif.whiteBalance, forKey: kCGImagePropertyExifWhiteBalance as! NSCopying)
        
        //tiff情報セット
        let tiff = NSMutableDictionary()
        //日時
        tiff.setObject(data.tiff.dateTime, forKey: kCGImagePropertyTIFFDateTime as! NSCopying)
        //製造元
        tiff.setObject(data.tiff.make, forKey: kCGImagePropertyTIFFMake as! NSCopying)
        //機種名
        tiff.setObject(data.tiff.model, forKey: kCGImagePropertyTIFFModel as! NSCopying)
        //方向
        tiff.setObject(data.tiff.orientaion, forKey: kCGImagePropertyTIFFOrientation as! NSCopying)
        //解像度の単位
        tiff.setObject(data.tiff.resolutionUnit, forKey: kCGImagePropertyTIFFResolutionUnit as! NSCopying)
        //使用ソフトウェア名
        tiff.setObject(data.tiff.software, forKey: kCGImagePropertyTIFFSoftware as! NSCopying)
        //幅の解像度
        tiff.setObject(data.tiff.xResolution, forKey: kCGImagePropertyTIFFXResolution as! NSCopying)
        //高さの解像度
        tiff.setObject(data.tiff.yResolution, forKey: kCGImagePropertyTIFFYResolution as! NSCopying)
        //イメージの説明
        tiff.setObject(data.tiff.imageDescription, forKey: kCGImagePropertyTIFFImageDescription as! NSCopying)
        
        //GPS
        let gps = NSMutableDictionary()
        //緯度：latitude
        gps.setObject("N", forKey: kCGImagePropertyGPSLatitudeRef as! NSCopying)
        gps.setObject(data.latitude, forKey: kCGImagePropertyGPSLatitude as! NSCopying)
        //経度：longitude
        gps.setObject("E", forKey: kCGImagePropertyGPSLongitudeRef as! NSCopying)
        gps.setObject(data.longitude, forKey: kCGImagePropertyGPSLongitude as! NSCopying)
        
        //トップのkeyValueとexif,tiff情報すべてをセット
        let metadata = NSMutableDictionary()
        //exif
        metadata.setObject(exif, forKey: kCGImagePropertyExifDictionary as! NSCopying)
        //tiff
        metadata.setObject(tiff, forKey: kCGImagePropertyTIFFDictionary as! NSCopying)
        //gps
        metadata.setObject(gps, forKey: kCGImagePropertyGPSDictionary as! NSCopying)
        //幅（ピクセル）
        //metadata.setObject(data.pixelWidth, forKey: kCGImagePropertyPixelWidth as! NSCopying)
        //高さ（DPI）
        //metadata.setObject(data.pixelHeight, forKey: kCGImagePropertyPixelHeight as! NSCopying)
        //幅（DPI）
        metadata.setObject(data.dpiWidth, forKey: kCGImagePropertyDPIWidth as! NSCopying)
        //高さ（ピクセル））
        metadata.setObject(data.dpiHeight, forKey: kCGImagePropertyDPIHeight as! NSCopying)
        //プロファイル名
        metadata.setObject(data.profileName, forKey: kCGImagePropertyProfileName as! NSCopying)
        //方向
        metadata.setObject(data.orientation, forKey: kCGImagePropertyOrientation as! NSCopying)
        //カラーモデル
        metadata.setObject(data.colorModel, forKey: kCGImagePropertyColorModel as! NSCopying)
        
        //書き込み
        let dest: CGImageDestination? = CGImageDestinationCreateWithURL(url as CFURL,
                                                                       kUTTypeJPEG,
                                                                       1,
                                                                       nil)
        CGImageDestinationAddImageFromSource(dest!, cgImage!, 0, metadata)
        CGImageDestinationFinalize(dest!)
        
    }
    
    //読み込み
    func getExifProperties(image: CIImage, gpsInfo: (latitude: Double, longitude: Double)) -> MetaData {
        
        //
        let metaData = image.properties
        
        //exif辞書以下のkeyValue
        //絞り値
        var exif_apertureValue: CGFloat = 0
        //輝度値
        var exif_brightnessValue: CGFloat = 0
        //色空間情報
        var exif_colorSpace: Int = 0
        //コンポーネント構成
        var exif_componentsConfiguration: [Int] = []
        //デジタル化日時
        var exif_datetimeDigitized: String = ""
        //オリジナルの日時
        var exif_datetimeOriginal: String = ""
        //Exifバージョン
        var exif_exifVersion: [Int] = []
        //露出補正値
        var exif_exposureBiasValue: Int = 0
        //露出モード
        var exif_exposureMode: Int = 0
        //露出プログラム
        var exif_exposureProgram: Int = 0
        //露出時間
        var exif_exposureTime: CGFloat = 0
        //Fナンバー
        var exif_fNumber: CGFloat = 0
        //フラッシュ
        var exif_flash: Int = 0
        //対応FlashPixバージョン
        var exif_flashPixelVersion: [Int] = []
        //35mm換算レンズ焦点距離
        var exif_focalLenIn35mmFilm: Int = 0
        //レンズ焦点距離
        var exif_focalLength: CGFloat = 0
        //ISOスピードレート
        var exif_isoSpeedRatings: [Int] = []
        //レンズ製造元
        var exif_lensMake: String = ""
        //レンズ名
        var exif_lensModel: String = ""
        //レンズ仕様
        var exif_lensSpecification: [CGFloat] = []
        //測光方式
        var exif_meteringMode: Int = 0
        //ピクセル幅
        //var exif_pixelXDimension: Int = 0
        //ピクセル高さ
        //var exif_pixelYDimension: Int = 0
        //撮影シーンタイプ
        var exif_sceneCaptureType: Int = 0
        //シーンタイプ
        var exif_sceneType: Int = 0
        //センサー方式
        var exif_sensingMethod: Int = 0
        //シャッタースピード
        var exif_shutterSpeedValue: CGFloat = 0
        //被写体領域
        var exif_subjectArea: [Int] = []
        //デジタル化時刻
        var exif_subsecTimeDigitized: String = ""
        //オリジナル作成時刻
        var exif_subsecTimeOriginal: String = ""
        //ホワイトバランス
        var exif_whiteBalance: Int = 0
        
        //exif取得
        let exifData = metaData[kCGImagePropertyExifDictionary as String] as? [String: Any]
        if let exif = exifData {
            //絞り値
            print("***************************exif data********************************")
            if let apertureValue = exif[kCGImagePropertyExifApertureValue as String] as? CGFloat {
                print("ApertureValue \(apertureValue)")
                exif_apertureValue = apertureValue
            }
            //輝度値
            if let brightnessValue = exif[kCGImagePropertyExifBrightnessValue as String] as? CGFloat {
                print("BrightnessValue \(brightnessValue)")
                exif_brightnessValue = brightnessValue
            }
            //色空間情報
            if let colorSpace = exif[kCGImagePropertyExifColorSpace as String] as? Int {
                print("ColorSpace \(colorSpace)")
                exif_colorSpace = colorSpace
            }
            //コンポーネント構成
            if let componentsConfiguration = exif[kCGImagePropertyExifComponentsConfiguration as String] as? [Int] {
                for cc in componentsConfiguration {
                    print("ComponentsConfiguration \(cc)")
                }
                exif_componentsConfiguration = componentsConfiguration
            }
            //デジタル化日時
            if let dateTimeDigitized = exif[kCGImagePropertyExifDateTimeDigitized as String] as? String {
                print("DateTimeDigitized \(dateTimeDigitized)")
                exif_datetimeDigitized = dateTimeDigitized
            }
            //オリジナルの日時
            if let dateTimeOriginal = exif[kCGImagePropertyExifDateTimeOriginal as String] as? String {
                print("DateTimeOriginal \(dateTimeOriginal)")
                exif_datetimeOriginal = dateTimeOriginal
            }
            //Exifバージョン
            if let ExifVersion = exif[kCGImagePropertyExifVersion as String] as? [Int] {
                for ver in ExifVersion {
                    print("ExifVersion \(ver)")
                }
                exif_exifVersion = ExifVersion
            }
            //露出補正値
            if let exposureBiasValue = exif[kCGImagePropertyExifExposureBiasValue as String] as? Int {
                print("ExposureBiasValue \(exposureBiasValue)")
                exif_exposureBiasValue = exposureBiasValue
            }
            //露出モード
            if let exposureMode = exif[kCGImagePropertyExifExposureMode as String] as? Int {
                print("ExposureMode \(exposureMode)")
                exif_exposureMode = exposureMode
            }
            //露出プログラム
            if let exposureProgram = exif[kCGImagePropertyExifExposureProgram as String] as? Int {
                print("ExposureProgram \(exposureProgram)")
                exif_exposureProgram = exposureProgram
            }
            //露出時間
            if let exposureTime = exif[kCGImagePropertyExifExposureTime as String] as? CGFloat {
                print("ExposureTime \(exposureTime)")
                exif_exposureTime = exposureTime
            }
            //Fナンバー
            if let fNumber = exif[kCGImagePropertyExifFNumber as String] as? CGFloat {
                print("FNumber \(fNumber)")
                exif_fNumber = fNumber
            }
            //フラッシュ
            if let flash = exif[kCGImagePropertyExifFlash as String] as? Int {
                print("Flash \(flash)")
                exif_flash = flash
            }
            //対応FlashPixバージョン
            if let flashPixelVersion = exif[kCGImagePropertyExifFlashPixVersion as String] as? [Int] {
                for ver in flashPixelVersion {
                    print("FlashPixelVersion \(ver)")
                }
                exif_flashPixelVersion = flashPixelVersion
            }
            //35mm換算レンズ焦点距離
            if let focalLenIn35mmFilm = exif[kCGImagePropertyExifFocalLenIn35mmFilm as String] as? Int {
                print("FocalLenIn35mmFilm \(focalLenIn35mmFilm)")
                exif_focalLenIn35mmFilm = focalLenIn35mmFilm
            }
            //レンズ焦点距離
            if let focalLength = exif[kCGImagePropertyExifFocalLength as String] as? CGFloat {
                print("FocalLength \(focalLength)")
                exif_focalLength = focalLength
            }
            //ISOスピードレート
            if let isoSpeedRatings = exif[kCGImagePropertyExifISOSpeedRatings as String] as? [Int] {
                for rating in isoSpeedRatings {
                    print("ISOSpeedRatings \(rating)")
                }
                exif_isoSpeedRatings = isoSpeedRatings
            }
            //レンズ製造元
            if let lensMake = exif[kCGImagePropertyExifLensMake as String] as? String {
                print("LensMake \(lensMake)")
                exif_lensMake = lensMake
            }
            //レンズ名
            if let lensModel = exif[kCGImagePropertyExifLensModel as String] as? String {
                print("LensModel \(lensModel)")
                exif_lensModel = lensModel
            }
            //レンズ仕様
            if let lensSpecification = exif[kCGImagePropertyExifLensSpecification as String] as? [CGFloat] {
                for lens in lensSpecification {
                    print("LensSpecification \(lens)")
                }
                exif_lensSpecification = lensSpecification
            }
            //測光方式
            if let meteringMode = exif[kCGImagePropertyExifMeteringMode as String] as? Int {
                print("MeteringMode \(meteringMode)")
                exif_meteringMode = meteringMode
            }
            //ピクセル幅
//            if let pixelX = exif[kCGImagePropertyExifPixelXDimension as String] as? Int {
//                print("PixelXDimension \(pixelX)")
//                exif_pixelXDimension = pixelX
//            }
            //ピクセル高さ
//            if let pixelY = exif[kCGImagePropertyExifPixelYDimension as String] as? Int {
//                print("PixelYDimension \(pixelY)")
//                exif_pixelYDimension = pixelY
//            }
            //撮影シーンタイプ
            if let sceneCaptureType = exif[kCGImagePropertyExifSceneCaptureType as String] as? Int {
                print("SceneCaptureType \(sceneCaptureType)")
                exif_sceneCaptureType = sceneCaptureType
            }
            //シーンタイプ
            if let sceneType = exif[kCGImagePropertyExifSceneType as String] as? Int {
                print("SceneType \(sceneType)")
                exif_sceneType = sceneType
            }
            //センサー方式
            if let sensingMethod = exif[kCGImagePropertyExifSensingMethod as String] as? Int {
                print("SensingMethod \(sensingMethod)")
                exif_sensingMethod = sensingMethod
            }
            //シャッタースピード
            if let shutterSpeedValue = exif[kCGImagePropertyExifShutterSpeedValue as String] as? CGFloat {
                print("ShutterSpeedValue \(shutterSpeedValue)")
                exif_shutterSpeedValue = shutterSpeedValue
            }
            //被写体領域
            if let subjectArea = exif[kCGImagePropertyExifSubjectArea as String] as? [Int] {
                for area in subjectArea {
                    print("SubjectArea \(area)")
                }
                exif_subjectArea = subjectArea
            }
            //デジタル化時刻
            if let subsecTimeDigitized = exif[kCGImagePropertyExifSubsecTimeDigitized as String] as? String {
                print("SubsecTimeDigitized \(subsecTimeDigitized)")
                exif_subsecTimeDigitized = subsecTimeDigitized
            }
            //オリジナル作成時刻
            if let subsecTimeOriginal = exif[kCGImagePropertyExifSubsecTimeOriginal as String] as? String {
                print("SubsecTimeOriginal \(subsecTimeOriginal)")
                exif_subsecTimeOriginal = subsecTimeOriginal
            }
            //ホワイトバランス
            if let whiteBalance = exif[kCGImagePropertyExifWhiteBalance as String] as? Int {
                print("WhiteBalance \(whiteBalance)")
                exif_whiteBalance = whiteBalance
            }
        }
        
        //tiff
        //日時
        var tiff_datetime: String = ""
        //製造元
        var tiff_make: String = ""
        //機種名
        var tiff_model: String = ""
        //方向
        let tiff_orientaion: Int = 1
        //解像度の単位
        var tiff_resolutionUnit: Int = 0
        //使用ソフトウェア名
        let tiff_software: String = "DOKOSHAPlus [1]"
        //幅の解像度
        var tiff_xResolution: Int = 0
        //高さの解像度
        var tiff_yResolution: Int = 0
        //イメージの説明
        let tiff_imageDescription: String = "DCP PHOTO"
        
        //tiff取得
        let tiffData = metaData[kCGImagePropertyTIFFDictionary as String] as? [String: Any]
        if let tiff = tiffData {
            print("++++++++++++++++++++++ tiff properties ++++++++++++++++++++++++++++++")
            //日時
            if let dateTime = tiff[kCGImagePropertyTIFFDateTime as String] as? String {
                print("DateTime \(dateTime)")
                tiff_datetime = dateTime
            }
            //製造元
            if let make = tiff[kCGImagePropertyTIFFMake as String] as? String {
                print("Make \(make)")
                tiff_make = make
            }
            //機種名
            if let model = tiff[kCGImagePropertyTIFFModel as String] as? String {
                print("Model \(model)")
                tiff_model = model
            }
            //方向
//            if let orientation = tiff[kCGImagePropertyTIFFOrientation as String] as? Int {
//                print("Orientation \(orientation)")
//                tiff_orientaion = orientation
//            }
            //解像度の単位
            if let resolutionUnit = tiff[kCGImagePropertyTIFFResolutionUnit as String] as? Int {
                print("ResolutionUnit \(resolutionUnit)")
                tiff_resolutionUnit = resolutionUnit
            }
            //使用ソフトウェア名
//            if let software = tiff[kCGImagePropertyTIFFSoftware as String] as? String {
//                print("Software \(software)")
//                tiff_software = software
//            }
            //幅の解像度
            if let xResolution = tiff[kCGImagePropertyTIFFXResolution as String] as? Int {
                print("XResolution \(xResolution)")
                tiff_xResolution = xResolution
            }
            //高さの解像度
            if let yResolution = tiff[kCGImagePropertyTIFFYResolution as String] as? Int {
                print("YResolution \(yResolution)")
                tiff_yResolution = yResolution
            }
        }
        
        //top key
        //幅（ピクセル）
        //var top_pixelWidth: Int = 0
        //高さ（ピクセル）
        //var top_pixelHeight: Int = 0
        //幅（DPI）
        var top_dpiWidth: Int = 0
        //高さ（DPI）
        var top_dpiHeight: Int = 0
        //プロファイル名
        var top_profileName: String = ""
        //方向
        let top_orientation: Int = 1
        //カラーモデル
        var top_colorModel: String = ""
        
        //top key取得
        print("<<<<<<<<<<<<<<<<<<<< top key properties >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>")
        //幅（ピクセル）
//        if let pixelWidth = metaData[kCGImagePropertyPixelWidth as String] as? Int {
//            print("PixelWidth \(pixelWidth)")
//            top_pixelWidth = pixelWidth
//        }
        //高さ（ピクセル）
//        if let pixelHeight = metaData[kCGImagePropertyPixelHeight as String] as? Int {
//            print("PixelHeight \(pixelHeight)")
//            top_pixelHeight = pixelHeight
//        }
        //幅（DPI）
        if let dpiWidth = metaData[kCGImagePropertyDPIWidth as String] as? Int {
            print("DPIWidth \(dpiWidth)")
            top_dpiWidth = dpiWidth
        }
        //高さ（DPI）
        if let dpiHeight = metaData[kCGImagePropertyDPIHeight as String] as? Int {
            print("DPIHeight \(dpiHeight)")
            top_dpiHeight = dpiHeight
        }
        //プロファイル名
        if let profileName = metaData[kCGImagePropertyProfileName as String] as? String {
            print("ProfileName \(profileName)")
            top_profileName = profileName
        }
        //方向
//        if let orientation = metaData[kCGImagePropertyOrientation as String] as? Int {
//            print("Orientation \(orientation)")
//            top_orientation = orientation
//        }
        //カラーモデル
        if let colorModel = metaData[kCGImagePropertyColorModel as String] as? String {
            print("ColorModel \(colorModel)")
            top_colorModel = colorModel
        }
        
        //取得したexif情報セット
        let exifs = MetaData.ExifData(apertureValue: exif_apertureValue,
                                         brightnessValue: exif_brightnessValue,
                                         colorSpace: exif_colorSpace,
                                         componentsConfiguration: exif_componentsConfiguration,
                                         dateTimeDigitized: exif_datetimeDigitized,
                                         dateTimeOriginal: exif_datetimeOriginal,
                                         exifVersion: exif_exifVersion,
                                         exposureBiasValue: exif_exposureBiasValue,
                                         exposureMode: exif_exposureMode,
                                         exposureProgram: exif_exposureProgram,
                                         exposureTime: exif_exposureTime,
                                         fNumber: exif_fNumber,
                                         flash: exif_flash,
                                         flashPixelVersion: exif_flashPixelVersion,
                                         focalLenIn35mmFilm: exif_focalLenIn35mmFilm,
                                         focalLength: exif_focalLength,
                                         isoSpeedRatings: exif_isoSpeedRatings,
                                         lensMake: exif_lensMake,
                                         lensModel: exif_lensModel,
                                         lensSpecification: exif_lensSpecification,
                                         meteringMode: exif_meteringMode,
                                         //pixelXDimension: exif_pixelXDimension,
                                         //pixelYDimension: exif_pixelYDimension,
                                         sceneCaptureType: exif_sceneCaptureType,
                                         sceneType: exif_sceneType,
                                         sensingMethod: exif_sensingMethod,
                                         shutterSpeedValue: exif_shutterSpeedValue,
                                         subjectArea: exif_subjectArea,
                                         subsecTimeDigitized: exif_subsecTimeDigitized,
                                         subsecTimeOriginal: exif_subsecTimeOriginal,
                                         whiteBalance: exif_whiteBalance)
        
        //取得したtiff情報セット
        let tiffs = MetaData.TiffData(dateTime: tiff_datetime,
                                         make: tiff_make,
                                         model: tiff_model,
                                         orientaion: tiff_orientaion,
                                         resolutionUnit: tiff_resolutionUnit,
                                         software: tiff_software,
                                         xResolution: tiff_xResolution,
                                         yResolution: tiff_yResolution,
                                         imageDescription: tiff_imageDescription)
        
        //すべての情報をセット
        let metaModel = MetaData(//pixelWidth: top_pixelWidth,
                                //pixelHeight: top_pixelHeight,
                                dpiWidth: top_dpiWidth,
                                dpiHeight: top_dpiHeight,
                                profileName: top_profileName,
                                orientation: top_orientation,
                                colorModel: top_colorModel,
                                exif: exifs,
                                tiff: tiffs,
                                latitude: gpsInfo.latitude,
                                longitude: gpsInfo.longitude)
        
        return metaModel
    }
}

















