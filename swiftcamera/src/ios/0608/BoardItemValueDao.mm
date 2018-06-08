//
//  BoardItemValueDao.m
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/31.
//

#import "BoardItemValueDao.h"

@implementation BoardItemValueDao

+ (NSMutableDictionary<NSString *,XMPInfo *> *)getXmpInfoDictionary:(int)boardId
{
    NSString *dbName = @"/LocalDatabase/photoapp.db";
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSString *path = [paths objectAtIndex:0];
    NSString *dbPath = [path stringByAppendingPathComponent:dbName];
    
    NSFileManager *manager = [NSFileManager defaultManager];
    //NSError *error = nil;
    
    if (![manager fileExistsAtPath:dbPath]) {
        NSLog(@"BoardItemValueDao  >  getXmpInfoDictionary  >  there is not detabase file in dbPath");
    } else {
        NSLog(@"BoardItemValueDao  >  getXmpInfoDictionary  >  there is detabase file in dbPath");
    }
    
    sqlite3 *database;
    sqlite3_stmt *statement;
    NSString *str = @"SELECT itm.itemId, itm.itemName, itm.exifName, biv.boardId, biv.numValue, biv.stringValue From BoardItemValue biv JOIN Item itm on biv.itemId = itm.itemId  Where boardId = ";
    NSString *num = [NSString stringWithFormat:@"%d", boardId];
    NSString *query = [str stringByAppendingString:num];
    
    NSLog(@"BoardItemValueDao  >  getXmpInfoDictionary  >  %@", query);
    NSMutableDictionary<NSString *, XMPInfo *>* xmpDic = [[NSMutableDictionary<NSString *, XMPInfo *> alloc] init];
    
    if (sqlite3_open([dbPath UTF8String], &database) == SQLITE_OK) {
        NSLog(@"BoardItemValueDao  >  getXmpInfoDictionary  >  can opened database!!");
        
        if (sqlite3_prepare_v2(database, [query UTF8String], -1, &statement, NULL) == SQLITE_OK) {
            while (sqlite3_step(statement) == SQLITE_ROW) {
                int itemId = sqlite3_column_int(statement, 0);
                char *itemName = (char *) sqlite3_column_text(statement, 1);
                char *exifName = (char *) sqlite3_column_text(statement, 2);
                int boardId = sqlite3_column_int(statement, 3);
                int numValue = sqlite3_column_int(statement, 4);
                char *stringValue = (char *) sqlite3_column_text(statement, 5);
                
                NSString *itm = [NSString stringWithCString:itemName encoding:NSUTF8StringEncoding];
                NSString *exi = [NSString stringWithCString:exifName encoding:NSUTF8StringEncoding];
                NSString *str = [NSString stringWithCString:stringValue encoding:NSUTF8StringEncoding];
                
                NSLog(@"BoardItemValueDao  >  getXmpInfoDictionary  >  query results");
                NSLog(@"itemID : %d, itemName : %@, exifName : %@, boardID : %d, numValue : %d, stringValue : %@", itemId, itm, exi, boardId, numValue, str);
                
                XMPInfo *xmpInfo = [[XMPInfo alloc] init];
                [xmpInfo setItemID:itemId];
                [xmpInfo setItemName:itm];
                [xmpInfo setExifName:exi];
                [xmpInfo setBoardID:boardId];
                [xmpInfo setNumValue:numValue];
                [xmpInfo setStringValue:str];
                
                [xmpDic setValue:xmpInfo forKey:exi];
                
            }
            sqlite3_finalize(statement);
            sqlite3_close(database);
        } else {
            NSLog(@"BoardPhotoDao  >  getXmpInfoDictionary  >  query cannnot compile");
        }
    } else {
        NSLog(@"BoardItemValueDao  >  getXmpInfoDictionary  >  can opened database");
    }
    
    NSUInteger count = [xmpDic count];
    NSLog(@"xmp Dictionary count : %lu", (unsigned long)count);
    
    return xmpDic;
}

+ (NSMutableArray<XMPInfo *> *)getClassificationRemarks:(int)boardId
{
    NSString *dbName = @"/LocalDatabase/photoapp.db";
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSString *path = [paths objectAtIndex:0];
    NSString *dbPath = [path stringByAppendingPathComponent:dbName];
    
    NSFileManager *manager = [NSFileManager defaultManager];
    
    if (![manager fileExistsAtPath:dbPath]) {
        NSLog(@"BoardItemValueDao  >  getXmpInfoDictionary  >  there is not detabase file in dbPath");
    } else {
        NSLog(@"BoardItemValueDao  >  getXmpInfoDictionary  >  there is detabase file in dbPath");
    }
    
    sqlite3 *database;
    sqlite3_stmt *statement;
    
    NSString *str = @"SELECT itm.itemId, itm.itemName, itm.exifName, biv.boardId, biv.numValue, biv.stringValue From BoardItemValue biv JOIN Item itm on biv.itemId = itm.itemId Where itm.exifName = 'ClassificationRemarks' and  biv.boardId = ";
    NSString *num = [NSString stringWithFormat:@"%d", boardId];
    NSString *strPlusNum = [str stringByAppendingString:num];
    NSString *query = [strPlusNum stringByAppendingString:@" Order by itm.sortNumber "];
    
    NSLog(@"BoardItemValueDao  >  getClassificationRemarks  >  %@", query);
    NSMutableArray<XMPInfo *>* xmpArray = [[NSMutableArray<XMPInfo *> alloc] init];
    
    if (sqlite3_open([dbPath UTF8String], &database) == SQLITE_OK) {
        NSLog(@"BoardItemValueDao  >  getClassificationRemarks  >  can opened database!!");
        
        if (sqlite3_prepare_v2(database, [query UTF8String], -1, &statement, NULL) == SQLITE_OK) {
            while (sqlite3_step(statement) == SQLITE_ROW) {
                int itemId = sqlite3_column_int(statement, 0);
                char *itemName = (char *) sqlite3_column_text(statement, 1);
                char *exifName = (char *) sqlite3_column_text(statement, 2);
                int boardId = sqlite3_column_int(statement, 3);
                int numValue = sqlite3_column_int(statement, 4);
                char *stringValue = (char *) sqlite3_column_text(statement, 5);
                
                NSString *itm = [NSString stringWithCString:itemName encoding:NSUTF8StringEncoding];
                NSString *exi = [NSString stringWithCString:exifName encoding:NSUTF8StringEncoding];
                NSString *str = [NSString stringWithCString:stringValue encoding:NSUTF8StringEncoding];
                
                NSLog(@"BoardItemValueDao  >  getClassificationRemarks  >  query results");
                NSLog(@"itemID : %d, itemName : %@, exifName : %@, boardID : %d, numValue : %d, stringValue : %@", itemId, itm, exi, boardId, numValue, str);
                
                XMPInfo *xmpInfo = [[XMPInfo alloc] init];
                [xmpInfo setItemID:itemId];
                [xmpInfo setItemName:itm];
                [xmpInfo setExifName:exi];
                [xmpInfo setBoardID:boardId];
                [xmpInfo setNumValue:numValue];
                [xmpInfo setStringValue:str];
                
                
                [xmpArray addObject:xmpInfo];
            }
            sqlite3_finalize(statement);
            sqlite3_close(database);
        } else {
            NSLog(@"BoardPhotoDao  >  getClassificationRemarks  >  query cannnot compile");
        }
    } else {
        NSLog(@"BoardItemValueDao  >  getClassificationRemarks  >  can opened database");
    }
    
    NSUInteger count = [xmpArray count];
    NSLog(@"xmp Array count : %lu", (unsigned long)count);
    
    return xmpArray;
}

+ (NSMutableArray<MeasurementXmpInfo *> *)getMeasurementItems:(int)boardId
{
    NSString *dbName = @"/LocalDatabase/photoapp.db";
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSString *path = [paths objectAtIndex:0];
    NSString *dbPath = [path stringByAppendingPathComponent:dbName];
    
    NSFileManager *manager = [NSFileManager defaultManager];
    
    if (![manager fileExistsAtPath:dbPath]) {
        NSLog(@"BoardItemValueDao  >  getMeasurementItems  >  there is not detabase file in dbPath");
    } else {
        NSLog(@"BoardItemValueDao  >  getMeasurementItems  >  there is detabase file in dbPath");
    }
    
    sqlite3 *database;
    sqlite3_stmt *statement;
    
    NSString *str = @"SELECT itm.itemId, itm.itemName, itm.exifName, biv.boardId, biv.numValue, biv.stringValue, biv.expectedValue, biv.resultValue, biv.symbol, biv.unit, biv.originalUnit, biv.spareValue1, biv.spareValue2, biv.spareValue3  From BoardItemValue biv JOIN Item itm on biv.itemId = itm.itemId  Where itm.exifName = 'Measurement' and  biv.boardId = ";
    NSString *num = [NSString stringWithFormat:@"%d", boardId];
    NSString *strPlusNum = [str stringByAppendingString:num];
    NSString *query = [strPlusNum stringByAppendingString:@" and itm.boarddataId in('EXIF_11','EXIF_13','EXIF_15','EXIF_17','EXIF_19')  Order by itm.sortNumber "];
    
    NSLog(@"BoardItemValueDao  >  getMeasurementItems  >  %@", query);
    NSMutableArray<MeasurementXmpInfo *>* itemsArray = [[NSMutableArray<MeasurementXmpInfo *> alloc] init];
    
    if (sqlite3_open([dbPath UTF8String], &database) == SQLITE_OK) {
        NSLog(@"BoardItemValueDao  >  getMeasurementItems  >  can opened database!!");
        
        if (sqlite3_prepare_v2(database, [query UTF8String], -1, &statement, NULL) == SQLITE_OK) {
            while (sqlite3_step(statement) == SQLITE_ROW) {
                int itemId = sqlite3_column_int(statement, 0);
                int boardId = sqlite3_column_int(statement, 1);
                char *itemName = (char *) sqlite3_column_text(statement, 2);
                char *exifName = (char *) sqlite3_column_text(statement, 3);
                char *stringValue = (char *) sqlite3_column_text(statement, 4);
                int numValue = sqlite3_column_int(statement, 5);
                char *expectedValue = (char *) sqlite3_column_text(statement, 6);
                char *resultValue = (char *) sqlite3_column_text(statement, 7);
                char *symbol = (char *) sqlite3_column_text(statement, 8);
                char *unit = (char *) sqlite3_column_text(statement, 9);
                char *originalUnit = (char *) sqlite3_column_text(statement, 10);
                char *spareValue1 = (char *) sqlite3_column_text(statement, 11);
                char *spareValue2 = (char *) sqlite3_column_text(statement, 12);
                char *spareValue3 = (char *) sqlite3_column_text(statement, 13);
                
                NSString *itm = [NSString stringWithCString:itemName encoding:NSUTF8StringEncoding];
                NSString *exi = [NSString stringWithCString:exifName encoding:NSUTF8StringEncoding];
                NSString *str = [NSString stringWithCString:stringValue encoding:NSUTF8StringEncoding];
                NSString *exp = [NSString stringWithCString:expectedValue encoding:NSUTF8StringEncoding];
                NSString *rus = [NSString stringWithCString:resultValue encoding:NSUTF8StringEncoding];
                NSString *sym = [NSString stringWithCString:symbol encoding:NSUTF8StringEncoding];
                NSString *uni = [NSString stringWithCString:unit encoding:NSUTF8StringEncoding];
                NSString *oriuni = [NSString stringWithCString:originalUnit encoding:NSUTF8StringEncoding];
                NSString *spv1 = [NSString stringWithCString:spareValue1 encoding:NSUTF8StringEncoding];
                NSString *spv2 = [NSString stringWithCString:spareValue2 encoding:NSUTF8StringEncoding];
                NSString *spv3 = [NSString stringWithCString:spareValue3 encoding:NSUTF8StringEncoding];
                
                NSLog(@"BoardItemValueDao  >  getMeasurementItems  >  query results");
                NSLog(@"itemID : %d, boardID : %d, itemName : %@, exifName : %@, stringValue : %@, numValue : %d, expectedValue : %@, resultValue : %@, symbol : %@, unit : %@, originalUnit : %@, spareValue1 : %@, spareValue2 : %@, spareValue3 : %@,", itemId, boardId, itm, exi, str, numValue, exp, rus, sym, uni, oriuni, spv1, spv2, spv3);
                
                MeasurementXmpInfo *item = [[MeasurementXmpInfo alloc] init];
                [item setItemID:itemId];
                [item setBoardID:boardId];
                [item setItemName:itm];
                [item setExifName:exi];
                [item setStringValue:str];
                [item setNumValue:numValue];
                [item setExpectedValue:exp];
                [item setResultValue:rus];
                [item setSymbol:sym];
                [item setUnit:uni];
                [item setOriginalUnit:oriuni];
                [item setSpareValue1:spv1];
                [item setSpareValue2:spv2];
                [item setSpareValue3:spv3];
                
                [itemsArray addObject:item];
                
            }
            sqlite3_finalize(statement);
            sqlite3_close(database);
        } else {
            NSLog(@"BoardPhotoDao  >  getMeasurementItems  >  query cannnot compile");
        }
    } else {
        NSLog(@"BoardItemValueDao  >  getMeasurementItems  >  can opened database");
    }
    
    NSUInteger count = [itemsArray count];
    NSLog(@"items Array count : %lu", (unsigned long)count);
    
    return itemsArray;
}

@end
































