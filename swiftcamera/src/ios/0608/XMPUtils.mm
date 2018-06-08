//
//  XMPUtils.m
//  どこでも写真管理Plus
//
//  Created by atd on 2018/05/29.
//
#import <Foundation/Foundation.h>
#import "XMPUtils.h"

#include <string>
#include <cstring>

//#define ENABLE_XMP_CPP_INTERFACE 1

// Must be defined to instantiate template classes
#define TXMP_STRING_TYPE std::string

// Must be defined to give access to XMPFiles
#define XMP_INCLUDE_XMPFILES 1
//#define XMP_INCLUDE_XMPFILES 0

#define ENABLE_NEW_DOM_MODEL 1

#define XMPUtilsDomain @"org.ryu1.xmptoolkit.ios.sample.XMPToolKitSDK-iOS-Sample"

// Ensure XMP templates are instantiated
#include "XMP.incl_cpp"

// Provide access to the API
#include "XMP.hpp"

#include <iostream>
#include <fstream>

@implementation XMPUtils

+ (const char *)getStringValue:(XMPInfo *)xmpInfo
{
    if (xmpInfo == nil) {
        NSLog(@"getStringValue  >>  xmpInfo is nil");
        NSString *str = @"";
        const char *chr = [str cStringUsingEncoding:NSUTF8StringEncoding];
        return chr;
    }
    NSLog(@"getStringValue  >>  xmpInfo is not nil");
    NSString *value = [xmpInfo stringValue];
    const char *returnValue = [value cStringUsingEncoding:NSUTF8StringEncoding];
    return returnValue;
}

+ (const char *)getNumValue:(XMPInfo *)xmpInfo
{
    if (xmpInfo == nil) {
        NSLog(@"getNumValue  >>  xmpInfo is nil");
        NSString *str = @"";
        const char *chr = [str cStringUsingEncoding:NSUTF8StringEncoding];
        return chr;
    }
    NSLog(@"getNumValue  >>  xmpInfo is not nil");
    int value = [xmpInfo numValue];
    NSString *intstr = [NSString stringWithFormat:@"%d", value];
    const char *returnValue = [intstr cStringUsingEncoding:NSUTF8StringEncoding];
    return returnValue;
}

+ (const char *)getItemString:(NSString *)value
{
    if (value == nil) {
        NSLog(@"getItemString  >>  xmpInfo is nil");
        NSString *str = @"";
        const char *chr = [str cStringUsingEncoding:NSUTF8StringEncoding];
        return chr;
    }
    NSLog(@"getItemString  >>  xmpInfo is not nil");
    const char *returnValue = [value cStringUsingEncoding:NSUTF8StringEncoding];
    return returnValue;
}


+ (void)writeXmp:(NSString *)path boardId:(int)bid error:(NSError *__autoreleasing *)error
{
    
    NSMutableDictionary<NSString *, XMPInfo *>* xmpDic = [BoardItemValueDao getXmpInfoDictionary:bid];
    NSMutableArray<XMPInfo *>* xmpArray = [BoardItemValueDao getClassificationRemarks:bid];
    NSMutableArray<MeasurementXmpInfo *>* items = [BoardItemValueDao getMeasurementItems:bid];
    
    //ファイルパス
    //dest.jpgが保存されている
    std::string filename = [path UTF8String];
    
    //新しく定義する名前空間
    const XMP_StringPtr  kXMP_NS_SDK_PHOTO = "http://dcpadv.org/schema/3.0/photoinfo";
    const XMP_StringPtr  kXMP_NS_SDK_MEASUREMENT = "http://dcpadv.org/schema/3.0/measurement";
    const XMP_StringPtr  kXMP_NS_SDK_ITEM = "http://dcpadv.org/schema/3.0/measurementitem";
    
    if (!SXMPMeta::Initialize()) {
        std::cout << "Could not initialize toolkit!";
        if (error) {
            *error = [NSError errorWithDomain:XMPUtilsDomain code:-1 userInfo:@{@"message": @"Could not initialize toolkit!"}];
            return;
        }
    }
    XMP_OptionBits options = 0;
    
    // Must initialize SXMPFiles before we use it
    if(SXMPFiles::Initialize(options))
    {
        try
        {
            // Options to open the file with - open for editing and use a smart handler
            XMP_OptionBits opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUseSmartHandler;
            
            bool ok;
            SXMPFiles myFile;
            std::string status = "";
            
            // First we try and open the file
            ok = myFile.OpenFile(filename, kXMP_UnknownFile, opts);
            if( ! ok )
            {
                status += "No smart handler available for " + filename + "\n";
                status += "Trying packet scanning.\n";
                
                // Now try using packet scanning
                opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUsePacketScanning;
                ok = myFile.OpenFile(filename, kXMP_UnknownFile, opts);
            }
            
            // If the file is open then read get the XMP data
            if(ok)
            {
                std::string actualPrefix;
                SXMPMeta::RegisterNamespace(kXMP_NS_SDK_PHOTO, "photo", &actualPrefix);
                SXMPMeta::RegisterNamespace(kXMP_NS_SDK_MEASUREMENT, "measurement", &actualPrefix);
                SXMPMeta::RegisterNamespace(kXMP_NS_SDK_ITEM, "item", &actualPrefix);
                
                SXMPMeta meta;
                myFile.GetXMP(&meta);
                
                // Add 工事件名
                NSLog(@"ConstructionName");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "ConstructionName", [self getStringValue:xmpDic[@"ConstructionName"]], 0);
        
                // Add 受注者名
                NSLog(@"Contructor");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "Contructor", [self getStringValue:xmpDic[@"Contructor"]], 0);
                
                // Add 写真・大分類
                NSLog(@"LargeClassification");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "LargeClassification", [self getStringValue:xmpDic[@"LargeClassification"]], 0);
                
                // Add 写真区分
                NSLog(@"PhotoClassification");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "PhotoClassification", [self getStringValue:xmpDic[@"PhotoClassification"]], 0);
                
                // Add 工種
                NSLog(@"ConstructionType");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "ConstructionType", [self getStringValue:xmpDic[@"ConstructionType"]], 0);
                
                // Add 種別
                NSLog(@"MiddleClassification");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "MiddleClassification", [self getStringValue:xmpDic[@"MiddleClassification"]], 0);
                
                // Add 細別
                NSLog(@"SmallClassification");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "SmallClassification", [self getStringValue:xmpDic[@"SmallClassification"]], 0);
                
                // Add 写真タイトル
                NSLog(@"Title");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "Title", [self getStringValue:xmpDic[@"Title"]], 0);
                
                
                //工種区分予備
//                if (xmpArray != nil && [xmpArray count] > 0) {
//
//                    std::string remarksPath;
//                    meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, "ClassificationRemarks", kXMP_PropArrayIsOrdered, 0, kXMP_PropValueIsArray);
//                    SXMPUtils::ComposeArrayItemPath(kXMP_NS_SDK_PHOTO, "ClassificationRemarks", kXMP_ArrayLastItem, &remarksPath);
//
//                    for (XMPInfo *element in xmpArray) {
//                        meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, remarksPath.c_str(), 0, [self getStringValue:element], 0);
//                    }
//                }
                std::string remarksPath;
                std::string listPath;
                meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, "ClassificationRemarks", kXMP_PropArrayIsOrdered, 0, kXMP_PropArrayIsUnordered);
                SXMPUtils::ComposeArrayItemPath(kXMP_NS_SDK_PHOTO, "ClassificationRemarks", kXMP_ArrayLastItem, &remarksPath);
                meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, remarksPath.c_str(), 0, "sample", 0);
                meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, remarksPath.c_str(), 0, "sample", 0);
                meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, remarksPath.c_str(), 0, "sample", 0);
                
                //
                NSLog(@"ShootingSpot");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "ShootingSpot", [self getStringValue:xmpDic[@"ShootingSpot"]], 0);
                NSLog(@"sRepresentative");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "sRepresentative", [self getStringValue:xmpDic[@"sRepresentative"]], 0);
                NSLog(@"IsFrequencyOfSubmission");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "IsFrequencyOfSubmission", [self getStringValue:xmpDic[@"IsFrequencyOfSubmission"]], 0);
                
                
                
                std::string lastItemPath;
                //施工管理値
                //photo:Measurements
                meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, "Measurements", kXMP_PropArrayIsOrdered, 0, kXMP_PropValueIsStruct);
                SXMPUtils::ComposeArrayItemPath(kXMP_NS_SDK_PHOTO, "Measurements", kXMP_ArrayLastItem, &lastItemPath);
                
                
                std::string clssificationPath;
                //分類
                //measurement:Classification
                SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, lastItemPath.c_str(), kXMP_NS_SDK_MEASUREMENT, "Classification", &clssificationPath);
                NSLog(@"measurement:Classification");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, clssificationPath.c_str(), [self getNumValue:xmpDic[@"Classification"]], 0);
                
                
//                if (items != nil && [items count] > 0) {
//
//                    std::string measurementPath;
//                    //測定値
//                    //measurement:MeasurementItems
//                    meta.SetStructField(kXMP_NS_SDK_PHOTO, lastItemPath.c_str(), kXMP_NS_SDK_MEASUREMENT, "MeasurementItems", 0, kXMP_PropValueIsStruct);
//                    SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, lastItemPath.c_str(), kXMP_NS_SDK_MEASUREMENT, "MeasurementItems", &measurementPath);
//
//                    for (MeasurementXmpInfo *element in items) {
//                        std::string itemPath;
//                        //item:Name
//                        NSLog(@"item:Name");
//                        meta.SetStructField(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "Name", "", 0);
//                        SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "Name", &itemPath);
//                        meta.SetProperty(kXMP_NS_SDK_PHOTO, itemPath.c_str(), [self getItemString:[element stringValue]], 0);
//
//                        //item:Mark
//                        NSLog(@"item:Mark");
//                        meta.SetStructField(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "Mark", "", 0);
//                        SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "Mark", &itemPath);
//                        meta.SetProperty(kXMP_NS_SDK_PHOTO, itemPath.c_str(), [self getItemString:[element symbol]], 0);
//
//                        //item:DesignedValue
//                        NSLog(@"item:DesignedValue(expectedValue)");
//                        meta.SetStructField(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "DesignedValue", "", 0);
//                        SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "DesignedValue", &itemPath);
//                        meta.SetProperty(kXMP_NS_SDK_PHOTO, itemPath.c_str(), [self getItemString:[element expectedValue]], 0);
//
//                        //item:MeasuredValue
//                        NSLog(@"item:MeasuredValue(resultValue)");
//                        meta.SetStructField(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "MeasuredValue", "", 0);
//                        SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "MeasuredValue", &itemPath);
//                        meta.SetProperty(kXMP_NS_SDK_PHOTO, itemPath.c_str(), [self getItemString:[element resultValue]], 0);
//
//                        //item:UnitName
//                        NSLog(@"item:UnitName(unit)");
//                        meta.SetStructField(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "UnitName", "", 0);
//                        SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "UnitName", &itemPath);
//                        meta.SetProperty(kXMP_NS_SDK_PHOTO, itemPath.c_str(), [self getItemString:[element unit]], 0);
//
//                        //item:Remarks
//                        NSLog(@"item:Remarks(spareValue)");
//                        meta.SetStructField(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "Remarks", 0, kXMP_PropArrayIsAlternate);
//                        SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "Remarks", &itemPath);
//                        meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, itemPath.c_str(), 0, [self getItemString:[element spareValue1]], 0);
//                        meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, itemPath.c_str(), 0, [self getItemString:[element spareValue2]], 0);
//                        meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, itemPath.c_str(), 0, [self getItemString:[element spareValue3]], 0);
//                    }
//                }
                
                std::string measurementPath;
                //測定値
                //measurement:MeasurementItems
                meta.SetStructField(kXMP_NS_SDK_PHOTO, lastItemPath.c_str(), kXMP_NS_SDK_MEASUREMENT, "MeasurementItems", 0, kXMP_PropValueIsStruct);
                SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, lastItemPath.c_str(), kXMP_NS_SDK_MEASUREMENT, "MeasurementItems", &measurementPath);
                
                std::string itemPath;
                //item:Name
                NSLog(@"item:Name");
                meta.SetStructField(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "Name", "", 0);
                SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "Name", &itemPath);
                meta.SetProperty(kXMP_NS_SDK_PHOTO, itemPath.c_str(), "item name", 0);
                
                //item:Mark
                NSLog(@"item:Mark");
                meta.SetStructField(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "Mark", "", 0);
                SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "Mark", &itemPath);
                meta.SetProperty(kXMP_NS_SDK_PHOTO, itemPath.c_str(), "item mark", 0);
                
                //item:DesignedValue
                NSLog(@"item:DesignedValue(expectedValue)");
                meta.SetStructField(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "DesignedValue", "", 0);
                SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "DesignedValue", &itemPath);
                meta.SetProperty(kXMP_NS_SDK_PHOTO, itemPath.c_str(), "item expected value", 0);
                
                //item:MeasuredValue
                NSLog(@"item:MeasuredValue(resultValue)");
                meta.SetStructField(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "MeasuredValue", "", 0);
                SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "MeasuredValue", &itemPath);
                meta.SetProperty(kXMP_NS_SDK_PHOTO, itemPath.c_str(), "item result value", 0);
                
                //item:UnitName
                NSLog(@"item:UnitName(unit)");
                meta.SetStructField(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "UnitName", "", 0);
                SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "UnitName", &itemPath);
                meta.SetProperty(kXMP_NS_SDK_PHOTO, itemPath.c_str(), " item unit name", 0);
                
                //item:Remarks
                NSLog(@"item:Remarks(spareValue)");
                meta.SetStructField(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "Remarks", 0, kXMP_PropArrayIsAlternate);
                SXMPUtils::ComposeStructFieldPath(kXMP_NS_SDK_PHOTO, measurementPath.c_str(), kXMP_NS_SDK_ITEM, "Remarks", &itemPath);
                meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, itemPath.c_str(), 0, "item remarks", 0);
                meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, itemPath.c_str(), 0, "item remarks", 0);
                meta.AppendArrayItem(kXMP_NS_SDK_PHOTO, itemPath.c_str(), 0, "item remarks", 0);
                
                
                // Add 受注者説明文
                NSLog(@"ContractorRemarks");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "ContractorRemarks", [self getStringValue:xmpDic[@"ContractorRemarks"]], 0);
                // Add 備考
                NSLog(@"Remarks");
                meta.SetProperty(kXMP_NS_SDK_PHOTO, "Remarks", [self getStringValue:xmpDic[@"Remarks"]], 0);
                
                
                // Write the RDF to a file
                std::cout << "writing RDF to file CS_RDF.txt" << std::endl;
                std::string metaBuffer;
                meta.SerializeToBuffer(&metaBuffer);
                //writeRDFToFile(&metaBuffer, "CS_RDF.txt");
                std::ofstream outfile;
                std::string rdfwritefile = filename+"CS_RDF.txt";
                outfile.open(rdfwritefile.c_str(), std::ios::out);
                outfile << *&metaBuffer;
                outfile.close();
                
                //書き込み
                if (myFile.CanPutXMP(meta)) {
                    myFile.PutXMP(meta);
                } else {
                    *error = [NSError errorWithDomain:XMPUtilsDomain code:-2 userInfo:@{@"message": @"write error!"}];
                }
                myFile.CloseFile();
            }
            else
            {
                std::cout << "Unable to open " << filename << std::endl;
                *error = [NSError errorWithDomain:XMPUtilsDomain code:-3 userInfo:@{@"message": @"Unable to open"}];
            }
        }
        catch(XMP_Error & e)
        {
            std::cout << "ERROR: " << e.GetErrMsg() << std::endl;
            *error = [NSError errorWithDomain:XMPUtilsDomain code:-3 userInfo:@{@"message": @"write other error"}];
        }
        
        // Terminate the toolkit
        SXMPFiles::Terminate();
        SXMPMeta::Terminate();
        
    }
    else
    {
        std::cout << "Could not initialize SXMPFiles.";
        *error = [NSError errorWithDomain:XMPUtilsDomain code:-4 userInfo:@{@"message": @"Could not initialize SXMPFiles."}];
    }
}
    
+ (void)writeNewDom:(NSString *)path error:(NSError **) error
{
    static const char * rdf =
    "<rdf:RDF xmlns:rdf='http://www.w3.org/1999/02/22-rdf-syntax-ns#'>"
    "  <rdf:Description rdf:about='' xmlns:photo='http://dcpadv.org/schema/3.0/photoinfo' xmlns:measurement='http://dcpadv.org/schema/3.0/measurement' xmlns:item='http://dcpadv.org/schema/3.0/measurementitem'>"
    ""
    "     <photo:ConstructionName>sample construction</photo:ConstructionName>"
    "     <photo:Contructor>sample Contructor</photo:Contructor>"
    "     <photo:LargeClassification>sample LargeClassification</photo:LargeClassification>"
    "     <photo:PhotoClassification>sample PhotoClassification</photo:PhotoClassification>"
    "     <photo:ConstructionType>sample ConstructionType</photo:ConstructionType>"
    "     <photo:MiddleClassification>sample MiddleClassification</photo:MiddleClassification>"
    "     <photo:SmallClassification>sample SmallClassification</photo:SmallClassification>"
    "     <photo:Title>sample Title</photo:Title>"
    ""
    "     <photo:ClassificationRemarks>"
    "           <rdf:Seq>"
    "               <rdf:li>Item 1</rdf:li>"
    "               <rdf:li>Item 2</rdf:li>"
    "           </rdf:Seq>"
    "     </photo:ClassificationRemarks>"
    ""
    "     <photo:ShootingSpot>sample ShootingSpot</photo:ShootingSpot>"
    "     <photo:IsRepresentative>sample IsRepresentative</photo:IsRepresentative>"
    "     <photo:IsFrequencyOfSubmission>sample IsFrequencyOfSubmission</photo:IsFrequencyOfSubmission>"
    ""
    "     <photo:Measurements>"
    "           <rdf:Description  rdf:parseType='Resource'>"
    "               <measurement:Classification>sample classification</measurement:Classification>"
    "               <measurement:MeasurementItems>"
    "                       <rdf:Seq>"
    "                           <rdf:li>"
    "                               <rdf:Description  rdf:parseType='Resource'>"
    "                                   <item:Name>sample name</item:Name>"
    "                                   <item:Mark>sample Mark</item:Mark>"
    "                                   <item:DesignedValue>sample DesignedValue</item:DesignedValue>"
    "                                   <item:MeasuredValue>sample MeasuredValue</item:MeasuredValue>"
    "                                   <item:UnitName>sample UnitName</item:UnitName>"
    "                                   <item:Remarks>"
    "                                           <rdf:Seq>"
    "                                               <rdf:li>spare value1</rdf:li>"
    "                                               <rdf:li>spare value2</rdf:li>"
    "                                               <rdf:li>spare value3</rdf:li>"
    "                                           </rdf:Seq>"
    "                                   </item:Remarks>"
    "                               </rdf:Description>"
    "                           </rdf:li>"
    "                       </rdf:Seq>"
    "               </measurement:MeasurementItems>"
    "           </rdf:Description>"
    "     </photo:Measurements>"
    ""
    "     <photo:ContractorRemarks>sample contractor remarks</photo:ContractorRemarks>"
    "     <photo:Remarks>sample remarks</photo:Remarks>"
    ""
    " </rdf:Description>"
    "</rdf:RDF>";
    
    //const XMP_StringPtr kXMP_NS_SDK = "http://ns.adobe.com/xmpTest/";
    
    //ファイルパス
    //dest.jpgが保存されている
    std::string filename = [path UTF8String];
    
    
    if (!SXMPMeta::Initialize()) {
        std::cout << "Could not initialize toolkit!";
        if (error) {
            *error = [NSError errorWithDomain:XMPUtilsDomain code:-1 userInfo:@{@"message": @"Could not initialize toolkit!"}];
            return;
        }
    }
    XMP_OptionBits options = 0;
    
    // Must initialize SXMPFiles before we use it
    if(SXMPFiles::Initialize(options))
    {
        try
        {
            // Options to open the file with - open for editing and use a smart handler
            XMP_OptionBits opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUseSmartHandler;
            
            bool ok;
            SXMPFiles myFile;
            std::string status = "";
            
            // First we try and open the file
            ok = myFile.OpenFile(filename, kXMP_UnknownFile, opts);
            if( ! ok )
            {
                status += "No smart handler available for " + filename + "\n";
                status += "Trying packet scanning.\n";
                
                // Now try using packet scanning
                opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUsePacketScanning;
                ok = myFile.OpenFile(filename, kXMP_UnknownFile, opts);
            }
            
            // If the file is open then read get the XMP data
            if(ok)
            {
                std::string actualPrefix;
                
                SXMPMeta meta;
                myFile.GetXMP(&meta);
                
                SXMPMeta custXMP(rdf, (XMP_StringLen) strlen(rdf));
                SXMPUtils::ApplyTemplate(&meta, custXMP, kXMPTemplate_AddNewProperties);
                
                //XMP_OptionBits outOpts =
                std::string metaBuffer;
                meta.SerializeToBuffer(&metaBuffer, 0, 0, "", "", 0);
                
                //書き込み
                if (myFile.CanPutXMP(meta)) {
                    myFile.PutXMP(meta);
                } else {
                    *error = [NSError errorWithDomain:XMPUtilsDomain code:-2 userInfo:@{@"message": @"write error!"}];
                }
                myFile.CloseFile();
            }
            else
            {
                std::cout << "Unable to open " << filename << std::endl;
                *error = [NSError errorWithDomain:XMPUtilsDomain code:-3 userInfo:@{@"message": @"Unable to open"}];
            }
        }
        catch(XMP_Error & e)
        {
            std::cout << "ERROR: " << e.GetErrMsg() << std::endl;
            *error = [NSError errorWithDomain:XMPUtilsDomain code:-3 userInfo:@{@"message": @"write other error"}];
        }
        
        // Terminate the toolkit
        SXMPFiles::Terminate();
        SXMPMeta::Terminate();
        
    }
    else
    {
        std::cout << "Could not initialize SXMPFiles.";
        *error = [NSError errorWithDomain:XMPUtilsDomain code:-4 userInfo:@{@"message": @"Could not initialize SXMPFiles."}];
    }
    
}

/*  modifyingXMPをコピー*/


SXMPMeta createXMPFromRDF()
{
    const char * rdf =
    "<rdf:RDF xmlns:rdf='http://www.w3.org/1999/02/22-rdf-syntax-ns#'>"
    "<rdf:Description rdf:about='' xmlns:dc='http://purl.org/dc/elements/1.1/'>"
    "<dc:subject>"
    "<rdf:Bag>"
    "<rdf:li>XMP</rdf:li>"
    "<rdf:li>SDK</rdf:li>"
    "<rdf:li>Sample</rdf:li>"
    "</rdf:Bag>"
    "</dc:subject>"
    "<dc:format>image/tiff</dc:format>"
    "</rdf:Description>"
    "</rdf:RDF>";
    
    SXMPMeta meta;
    // Loop over the rdf string and create the XMP object
    // 10 characters at a time
    int i;
    for (i = 0; i < (long)strlen(rdf) - 10; i += 10 )
    {
        meta.ParseFromBuffer ( &rdf[i], 10, kXMP_ParseMoreBuffers );
    }
    
    // The last call has no kXMP_ParseMoreBuffers options, signifying
    // this is the last input buffer
    meta.ParseFromBuffer ( &rdf[i], (XMP_StringLen) strlen(rdf) - i );
    return meta;
    
}


void writeRDFToFile(std::string * rdf, std::string filename)
{
    std::ofstream outFile;
    outFile.open(filename.c_str(), std::ios::out);
    outFile << *rdf;
    outFile.close();
}

+ (void)modifyingXmp:(NSString *)path error:(NSError **) error
{
    
    std::string filename = [path UTF8String];
    
    if(!SXMPMeta::Initialize())
    {
        std::cout << "Could not initialize toolkit!";
    }
    
    XMP_OptionBits options = 0;
#if UNIX_ENV
    options |= kXMPFiles_ServerMode;
#endif
    
    // Must initialize SXMPFiles before we use it
    if(SXMPFiles::Initialize(options))
    {
        try
        {
            // Options to open the file with - open for editing and use a smart handler
            XMP_OptionBits opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUseSmartHandler;
            
            bool ok;
            SXMPFiles myFile;
            std::string status = "";
            
            // First we try and open the file
            ok = myFile.OpenFile(filename, kXMP_UnknownFile, opts);
            if( ! ok )
            {
                status += "No smart handler available for " + filename + "\n";
                status += "Trying packet scanning.\n";
                
                // Now try using packet scanning
                opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUsePacketScanning;
                ok = myFile.OpenFile(filename, kXMP_UnknownFile, opts);
            }
            
            // If the file is open then read get the XMP data
            if(ok)
            {
                std::cout << status << std::endl;
                std::cout << filename << " is opened successfully" << std::endl;
                // Create the XMP object and get the XMP data
                SXMPMeta meta;
                myFile.GetXMP(&meta);
                
                
                ///////////////////////////////////////////////////
                // Now modify the XMP
                if(meta.DoesPropertyExist(kXMP_NS_XMP, "CreatorTool"))
                {
                    // Update xap:CreatorTool - we don't need to set any option bits
                    meta.SetProperty(kXMP_NS_XMP, "CreatorTool", "Updated By XMP SDK", 0);
                }
                
                // Update the Metadata Date
                XMP_DateTime updatedTime;
                // Get the current time.  This is a UTC time automatically
                // adjusted for the local time
                SXMPUtils::CurrentDateTime(&updatedTime);
                if(meta.DoesPropertyExist(kXMP_NS_XMP, "MetadataDate"))
                {
                    meta.SetProperty_Date(kXMP_NS_XMP, "MetadataDate", updatedTime, 0);
                }
                
                // Add an item onto the dc:creator array
                // Note the options used, kXMP_PropArrayIsOrdered, if the array does not exist it will be created
                meta.AppendArrayItem(kXMP_NS_DC, "creator", kXMP_PropArrayIsOrdered, "Author Name", 0);
                meta.AppendArrayItem(kXMP_NS_DC, "creator", kXMP_PropArrayIsOrdered, "Another Author Name", 0);
                
                // Now update alt-text properties
                meta.SetLocalizedText(kXMP_NS_DC, "title", "en", "en-US", "An English title");
                meta.SetLocalizedText(kXMP_NS_DC, "title", "fr", "fr-FR", "Un titre Francais");
                
                // Display the properties again to show changes
                std::cout << "After update:" << std::endl;
                
                // Create a new XMP object from an RDF string
                SXMPMeta rdfMeta = createXMPFromRDF();
                
                // Append the newly created properties onto the original XMP object
                // This will:
                // a) Add ANY new TOP LEVEL properties in the source (rdfMeta) to the destination (meta)
                // b) Replace any top level properties in the source with the matching properties from the destination
                SXMPUtils::ApplyTemplate(&meta, rdfMeta, kXMPTemplate_AddNewProperties | kXMPTemplate_ReplaceExistingProperties | kXMPTemplate_IncludeInternalProperties);
                
                // Display the properties again to show changes
                std::cout << "After Appending Properties:" << std::endl;
                
                // Serialize the packet and write the buffer to a file
                // Let the padding be computed and use the default linefeed and indents without limits
                std::string metaBuffer;
                meta.SerializeToBuffer(&metaBuffer, 0, 0, "", "", 0);
                
                
                // Write the packet to a file as RDF
                writeRDFToFile(&metaBuffer, filename+"_XMP_RDF.txt");
                
                // Write the packet to a file but this time as compact RDF
                XMP_OptionBits outOpts = kXMP_OmitPacketWrapper | kXMP_UseCompactFormat;
                meta.SerializeToBuffer(&metaBuffer, outOpts);
                writeRDFToFile(&metaBuffer, filename+"_XMP_RDF_Compact.txt");
                
                // Check we can put the XMP packet back into the file
                if(myFile.CanPutXMP(meta))
                {
                    // If so then update the file with the modified XMP
                    myFile.PutXMP(meta);
                }
                
                // Close the SXMPFile.  This *must* be called.  The XMP is not
                // actually written and the disk file is not closed until this call is made.
                myFile.CloseFile();
            }
            else
            {
                std::cout << "Unable to open " << filename << std::endl;
            }
        }
        catch(XMP_Error & e)
        {
            std::cout << "ERROR: " << e.GetErrMsg() << std::endl;
        }
        
        // Terminate the toolkit
        SXMPFiles::Terminate();
        SXMPMeta::Terminate();
        
    }
    else
    {
        std::cout << "Could not initialize SXMPFiles.";
        
    }
}
    

@end
