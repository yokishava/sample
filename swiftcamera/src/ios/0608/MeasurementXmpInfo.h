//
//  MeasurementXmpInfo.h
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/31.
//

#import <Foundation/Foundation.h>

@interface MeasurementXmpInfo : NSObject

@property (nonatomic) int itemID;
@property (nonatomic) int boardID;
@property (nonatomic) NSString *itemName;
@property (nonatomic) NSString *exifName;
@property (nonatomic) NSString *stringValue;
@property (nonatomic) int numValue;
@property (nonatomic) NSString *expectedValue;
@property (nonatomic) NSString *resultValue;
@property (nonatomic) NSString *symbol;
@property (nonatomic) NSString *unit;
@property (nonatomic) NSString *originalUnit;
@property (nonatomic) NSString *spareValue1;
@property (nonatomic) NSString *spareValue2;
@property (nonatomic) NSString *spareValue3;

@end
