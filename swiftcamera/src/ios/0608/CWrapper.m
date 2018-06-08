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

+ (void)wrapp:(NSString*) sourceFile destFile:(NSString*) destFile
{
    const char *src = [sourceFile UTF8String];
    const char *dst = [destFile UTF8String];
    int result = writeBinary(src, dst);
    NSLog(@"\n");
    NSLog(@"xmp 埋め込み結果1　%d", result);
    NSLog(@"\n");
}

+ (void)wrapp2:(NSString *)workFile destFile:(NSString *)destFile
{
    const char *wrk = [workFile UTF8String];
    const char *dst = [destFile UTF8String];
    int result = writeBinary2(wrk, dst);
    NSLog(@"\n");
    NSLog(@"xmp 埋め込み結果2　%d", result);
    NSLog(@"\n");
}

@end
