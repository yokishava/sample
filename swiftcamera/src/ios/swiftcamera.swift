//
//  swiftcamera.swift
//


import Foundation

@objc(swiftcamera) class swiftcamera: CDVPlugin {
    
    var boardPicture: String?
    var bID: Int?
    var nobo: Bool?
    
    func startcamera(_ command: CDVInvokedUrlCommand) {
        
        let pluginResult = CDVPluginResult(status: CDVCommandStatus_OK, messageAs: "message")
        
        //boardID
        let bid = command.argument(at: 0)
        if bid != nil {
            if let boardID = bid as? Int {
                bID = boardID
            }
        }
        
        //boardData
        //base64のstring
        let bdata = command.argument(at: 1)
        if bdata != nil {
            if let board = bdata as? String {
                boardPicture = board
            }
        }
        
        //noboard
        let no = command.argument(at: 2)
        if no != nil {
            if let noboard = no as? Bool {
                nobo = noboard
            }
        }
        
        //let cameraVC = CameraViewController()
        //let cameraVC = CameraViewController(bid: bID, bdata: boardPicture, noboard: nobo)
        
        self.commandDelegate!.send(pluginResult, callbackId: command.callbackId)
        
        let vc = ViewController(boardID: bID, board: boardPicture, noBoard: nobo)
        self.viewController.present(vc, animated: true, completion: nil)
        
        //self.viewController.dismiss(animated: true, completion: nil)
        //self.viewController.present(cameraVC, animated: true, completion: nil)
        
    }
    
}
