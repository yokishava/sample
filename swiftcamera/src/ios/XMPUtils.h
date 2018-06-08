//
//  XMPUtils.h
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/29.
//

#import <Foundation/Foundation.h>
#import "XMPInfo.h"
#import "BoardItemValueDao.h"
#import "MeasurementXmpInfo.h"


@interface XMPUtils : NSObject

+ (const char *)getStringValue:(XMPInfo *)xmpInfo;

+ (const char *)getNumValue:(XMPInfo *)xmpInfo;

+ (const char *)getItemString:(NSString *)value;

+ (void)writeXmp:(NSString *)path boardId:(int) bid error:(NSError **) error;

+ (void)writeNewDom:(NSString *)path error:(NSError **) error;

@end
