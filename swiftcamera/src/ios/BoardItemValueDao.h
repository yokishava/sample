//
//  BoardItemValueDao.h
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/31.
//

#import <Foundation/Foundation.h>
#import "sqlite3.h"
#import "XMPInfo.h"
#import "MeasurementXmpInfo.h"

@interface BoardItemValueDao : NSObject

+ (NSMutableDictionary<NSString*, XMPInfo*>*)getXmpInfoDictionary:(int)boardId;

+ (NSMutableArray<XMPInfo*>*)getClassificationRemarks:(int)boardId;

+ (NSMutableArray<MeasurementXmpInfo*>*)getMeasurementItems:(int)boardId;

@end
