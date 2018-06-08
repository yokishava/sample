//
//  CWrapper.m
//  CWrapper
//
//  Created by atd on 2018/05/24.
//  Copyright © 2018年 atd. All rights reserved.
//

#import "CWrapper.h"
#import <Foundation/Foundation.h>

@implementation CWrapper

+ (int)call_JACIC_WriteHashValue:(NSString *)sourceFile destFile:(NSString *)destFile
{
    int returnCode;
    returnCode = 0;
    
    const char *srcFile = [sourceFile UTF8String];
    const char *dstFile = [destFile UTF8String];
    
    returnCode = JACIC_WriteHashValue(srcFile, dstFile);
    
    return returnCode;
}

@end
