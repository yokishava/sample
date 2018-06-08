//
//  ExtensionClass.swift
//  どこでも写真管理Plus
//
//  Created by atd on 2018/06/04.
//

import Foundation

extension UIImage {
    
    //リサイズ
    //func resized(currentPreset preset: ImagePreset, orientation: UIDeviceOrientation) -> UIImage? {
    func resized(currentPreset preset: PresetMode, orientation: UIDeviceOrientation) -> UIImage? {
        var size: CGFloat = 0
        switch preset {
        //case .millionPixels:
        case .million:
            size = 1280 / 2
        //case .TwoMillionPixels:
        case .twoMillion:
            size = 1600 / 2
        //case .ThreeMillionPixels:
        case .threeMillion:
            size = 2048 / 2
        default:
            size = 3200 / 2
        }
        
        var canvasSize: CGSize = CGSize(width: 0, height: 0)
        switch orientation {
        case .portrait:
            canvasSize = CGSize(width: size * (3/4), height: size)
        case .landscapeRight, .landscapeLeft:
            canvasSize = CGSize(width: size, height: size * (3/4))
        default:
            //landscape
            if UIScreen.main.bounds.width > UIScreen.main.bounds.height {
                canvasSize = CGSize(width: size, height: size * (3/4))
                //portrait
            } else {
                canvasSize = CGSize(width: size * (3/4), height: size)
            }
        }
        
        UIGraphicsBeginImageContextWithOptions(canvasSize, false, scale)
        defer { UIGraphicsEndImageContext() }
        draw(in: CGRect(origin: .zero, size: canvasSize))
        print("can resize image")
        return UIGraphicsGetImageFromCurrentImageContext()
    }
    
    func fixedOrientation() -> UIImage {
        
        if imageOrientation == UIImageOrientation.up {
            return self
        }
        
        var transform: CGAffineTransform = CGAffineTransform.identity
        
        switch imageOrientation {
        case .down, .downMirrored:
            transform = transform.translatedBy(x: size.width, y: size.height)
            transform = transform.rotated(by: CGFloat.pi)
            break
        case .left, .leftMirrored:
            transform = transform.translatedBy(x: size.width, y: 0)
            transform = transform.rotated(by: CGFloat.pi / 2.0)
            break
        case .right, .rightMirrored:
            transform = transform.translatedBy(x: 0, y: size.height)
            transform = transform.rotated(by: CGFloat.pi / -2.0)
            break
        case .up, .upMirrored:
            break
        }
        switch imageOrientation {
        case .upMirrored, .downMirrored:
            transform.translatedBy(x: size.width, y: 0)
            transform.scaledBy(x: -1, y: 1)
            break
        case .leftMirrored, .rightMirrored:
            transform.translatedBy(x: size.height, y: 0)
            transform.scaledBy(x: -1, y: 1)
        case .up, .down, .left, .right:
            break
        }
        
        let ctx: CGContext = CGContext(data: nil, width: Int(size.width), height: Int(size.height), bitsPerComponent: self.cgImage!.bitsPerComponent, bytesPerRow: 0, space: self.cgImage!.colorSpace!, bitmapInfo: CGImageAlphaInfo.premultipliedLast.rawValue)!
        
        ctx.concatenate(transform)
        
        switch imageOrientation {
        case .left, .leftMirrored, .right, .rightMirrored:
            ctx.draw(self.cgImage!, in: CGRect(x: 0, y: 0, width: size.height, height: size.width))
        default:
            ctx.draw(self.cgImage!, in: CGRect(x: 0, y: 0, width: size.width, height: size.height))
            break
        }
        
        return UIImage(cgImage: ctx.makeImage()!)
    }
}
