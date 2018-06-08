//
//  LocationManager.swift
//


import Foundation
import CoreLocation

class LocationManager: NSObject, CLLocationManagerDelegate {
    
    let locationManager: CLLocationManager
    private let dateformatter = DateFormat.format
    
    override init() {
        
        locationManager = CLLocationManager()
        locationManager.desiredAccuracy = kCLLocationAccuracyBest
        locationManager.distanceFilter = 1
        super.init()
        
    }
    
    func locationManager(_ manager: CLLocationManager, didChangeAuthorization status: CLAuthorizationStatus) {
        if CLLocationManager.locationServicesEnabled() {
            if status == CLAuthorizationStatus.notDetermined {
                locationManager.requestWhenInUseAuthorization()
            } else {}
        }
    }
    
    func locationManager(_ manager: CLLocationManager, didFailWithError error: Error) {
        print("位置情報の取得に失敗しました")
    }
    
}
