//
//  xmp.c
//  どこでも写真管理Plus
//
//  Created by atd on 2018/06/07.
//

#include "xmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

static const char *rdf = "xmlns:exifEX=\"http://cipa.jp/exif/1.0/\" xmlns:aux=\"http://ns.adobe.com/exif/1.0/aux/\" xmlns:photo=\"http://dcpadv.org/schema/3.0/photoinfo\" xmlns:measurement=\"http://dcpadv.org/schema/3.0/measurement\" xmlns:item=\"http://dcpadv.org/schema/3.0/measurementitem\" ";

static const char *rdf2 = "<photo:ConstructionName>sample construction</photo:ConstructionName> <photo:Contructor>sample Contructor</photo:Contructor> <photo:LargeClassification>sample LargeClassification</photo:LargeClassification> <photo:PhotoClassification>sample PhotoClassification</photo:PhotoClassification> <photo:ConstructionType>sample ConstructionType</photo:ConstructionType> <photo:MiddleClassification>sample MiddleClassification</photo:MiddleClassification> <photo:SmallClassification>sample SmallClassification</photo:SmallClassification> <photo:Title>sample Title</photo:Title> <photo:ClassificationRemarks> <rdf:Seq> <rdf:li>Item 1</rdf:li> <rdf:li>Item 2</rdf:li> </rdf:Seq> </photo:ClassificationRemarks> <photo:ShootingSpot>sample ShootingSpot</photo:ShootingSpot> <photo:IsRepresentative>sample IsRepresentative</photo:IsRepresentative> <photo:IsFrequencyOfSubmission>sample IsFrequencyOfSubmission</photo:IsFrequencyOfSubmission> <photo:Measurements> <rdf:Description rdf:parseType='Resource'> <measurement:Classification>sample classification</measurement:Classification> <measurement:MeasurementItems> <rdf:Seq> <rdf:li> <rdf:Description rdf:parseType='Resource'> <item:Name>sample name</item:Name> <item:Mark>sample Mark</item:Mark> <item:DesignedValue>sample DesignedValue</item:DesignedValue> <item:MeasuredValue>sample MeasuredValue</item:MeasuredValue> <item:UnitName>sample UnitName</item:UnitName> <item:Remarks> <rdf:Seq> <rdf:li>spare value1</rdf:li> <rdf:li>spare value2</rdf:li> <rdf:li>spare value3</rdf:li> </rdf:Seq> </item:Remarks> </rdf:Description> </rdf:li> </rdf:Seq> </measurement:MeasurementItems> </rdf:Description> </photo:Measurements> <photo:ContractorRemarks>sample contractor remarks</photo:ContractorRemarks> <photo:Remarks>sample remarks</photo:Remarks> ";

static const unsigned char SOI = 0xFF;
static const unsigned char APP1 = 0xE1;

//static const unsigned char XMP[] = {
//    0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x6E, 0x73,
//    0x2E, 0x61, 0x64, 0x6F, 0x62, 0x65, 0x2E, 0x63, 0x6F,
//    0x6D, 0x2F, 0x78, 0x61, 0x70, 0x2F, 0x31, 0x2E, 0x30,
//    0x2F
//};

/*http://ns.adobe.com/photoshop/1.0/*/
static const unsigned char PHOTOSHOP[] = {
    0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x6E, 0x73,
    0x2E, 0x61, 0x64, 0x6F, 0x62, 0x65, 0x2E, 0x63, 0x6F,
    0x6D, 0x2F, 0x70, 0x68, 0x6F, 0x74, 0x6F, 0x73, 0x68,
    0x6F, 0x70, 0x2F, 0x31, 0x2E, 0x30, 0x2F
};

//DCP_PHOTO</rdf:li> </rdf:Alt> </dc:description>
static const unsigned char DCPPHOTO[] = {
    0x44, 0x43, 0x50, 0x20, 0x50, 0x48, 0x4F, 0x54, 0x4F,
    0x3C, 0x2F, 0x72, 0x64, 0x66, 0x3A, 0x6C, 0x69, 0x3E,
    0x20, 0x3C, 0x2F, 0x72, 0x64, 0x66, 0x3A, 0x41, 0x6C,
    0x74, 0x3E, 0x20, 0x3C, 0x2F, 0x64, 0x63, 0x3A, 0x64,
    0x65, 0x73, 0x63, 0x72, 0x69, 0x70, 0x74, 0x69, 0x6F,
    0x6E, 0x3E
};

int endianGet(void) {
    static int i = 1;
    int ret = 0;
    
    if (((int)(*(char *)&i)) == 1) {
        ret = 1;
    }
    return ret;
}

unsigned short convertEndian(unsigned short src) {
    return (src << 8) | ((src >> 8) & 0x00FF);
}

long getFileSize(const char *file)
{
    struct stat statBuf;
    
    if (stat(file, &statBuf) == 0)
        return statBuf.st_size;
    
    return -1L;
}

int writeBinary(const char *src, const char *dst)
{
    //読込みファイル
    FILE *fpr;

    unsigned long fsize;
    unsigned long readSize;
    JpegBuffer *readJpegBuffer;
    size_t readJpegBufferMemorySize;
    //size_t rdfLength = sizeof(rdf);
    size_t rdfLength = strlen(rdf);
    size_t outBufLength;
    
    //ファイルサイズ取得
    fsize = getFileSize(src);
    
    //ファイルオープン
    fpr = fopen(src, "rb");
    if( fpr == NULL ){
        printf( "読込用ファイルが開けません");
        return -1;
    }
    
    readJpegBufferMemorySize = sizeof(JpegBuffer) + fsize;
    readJpegBuffer = (JpegBuffer *) malloc(readJpegBufferMemorySize);
    
    readJpegBuffer->len = fsize;
    
    readSize = fread(readJpegBuffer->buf, 1, fsize, fpr);
    
    unsigned long iti;
    int i;
    for (i = 0; i < (int)readSize; i++) {
        if (readJpegBuffer->buf[i] == SOI) {
            if (readJpegBuffer->buf[i+1] == APP1) {
                
                if (readJpegBuffer->buf[i + 347] == PHOTOSHOP[0]) {
                    char readXmp[34] = {};
                    int j;
                    for (j = 0; j < 34; j++) {
                        
                        //printf("%02X : %02X", readJpegBuffer->buf[i + 347 + j], XMP[j]);
                        printf("%02X : %02X", readJpegBuffer->buf[i + 347 + j], PHOTOSHOP[j]);
                        printf("\n");
                        
                        readXmp[j] = readJpegBuffer->buf[i + 347 + j];
                    }
                    //挿入位置の特定
                    //+2は "とスペース分
                    iti = (long)(i + 347 + j + 2);
                    printf("%s", readXmp);
                    printf("\n");
                    printf("%d", i);
                    printf("\n");
                }
            }
        }
    }
    
    

    outBufLength = readJpegBuffer->len + rdfLength;

    printf("outBufLength %zX", outBufLength);
    printf("src->len %zX", readJpegBuffer->len);
    printf("rdfLength %zX", rdfLength);

    //挿入データ分を追加したサイズのバッファ
    JpegBuffer *outBuf = (JpegBuffer *) malloc(sizeof(JpegBuffer *) + outBufLength);

    //元データ前半部をコピー
    memcpy(outBuf->buf, readJpegBuffer->buf, iti);

    //挿入データをコピー
    memcpy(outBuf->buf + iti, rdf, rdfLength);

    //元データ後半部をコピー
    memcpy((outBuf->buf + iti + rdfLength), &(readJpegBuffer->buf[iti]), readJpegBuffer->len - iti);

    FILE *fpw;
    fpw = fopen(dst, "wb");
    if (fpw == NULL) {
        printf( "書込用ファイルが開けません");
        return -1;
    }

    size_t writeSize = 1;

    fwrite(outBuf->buf, writeSize, readJpegBuffer->len + rdfLength, fpw);

    fclose(fpw);
    fclose(fpr);
    
    //メモリ解放
    //rdfLength = NULL;
    outBuf = NULL;
    
    fpw = NULL;
    fpr = NULL;
    
    return 0;
    
}

int writeBinary2(const char *wrk, const char *dst)
{
    //読込みファイル
    FILE *fpr;
    
    unsigned long fsize;
    unsigned long readSize;
    JpegBuffer *readJpegBuffer;
    size_t readJpegBufferMemorySize;
    size_t rdf2Length = strlen(rdf2);
    size_t outBufLength;
    
    //ファイルサイズ取得
    fsize = getFileSize(wrk);
    
    //ファイルオープン
    fpr = fopen(wrk, "rb");
    if( fpr == NULL ){
        printf( "読込用ファイルが開けません");
        return -1;
    }
    
    readJpegBufferMemorySize = sizeof(JpegBuffer) + fsize;
    readJpegBuffer = (JpegBuffer *) malloc(readJpegBufferMemorySize);
    
    readJpegBuffer->len = fsize;
    
    readSize = fread(readJpegBuffer->buf, 1, fsize, fpr);
    
    unsigned long iti;
    int i;
    for (i = 0; i < (int)readSize; i++) {
        if (readJpegBuffer->buf[i] == SOI) {
            if (readJpegBuffer->buf[i+1] == APP1) {
                
                if (readJpegBuffer->buf[i + 850] == DCPPHOTO[0]) {
                    char readXmp[47] = {};
                    int j;
                    for (j = 0; j < 47; j++) {
                        
                        //printf("%02X : %02X", readJpegBuffer->buf[i + 347 + j], XMP[j]);
                        printf("%02X : %02X", readJpegBuffer->buf[i + 850 + j], DCPPHOTO[j]);
                        printf("\n");
                        
                        readXmp[j] = readJpegBuffer->buf[i + 850 + j];
                    }
                    //挿入位置の特定
                    //+1は 半角スペース分
                    iti = (long)(i + 850 + j + 1);
                    printf("%s", readXmp);
                    printf("\n");
                    printf("%d", i);
                    printf("\n");
                }
            }
        }
    }
    
    
    
    outBufLength = readJpegBuffer->len + rdf2Length;
    
    printf("outBufLength %zX", outBufLength);
    printf("\n");
    printf("src->len %zX", readJpegBuffer->len);
    printf("\n");
    printf("rdfLength %zX", rdf2Length);
    printf("\n");
    
    //挿入データ分を追加したサイズのバッファ
    JpegBuffer *outBuf = (JpegBuffer *) malloc(sizeof(JpegBuffer *) + outBufLength);
    
    //元データ前半部をコピー
    memcpy(outBuf->buf, readJpegBuffer->buf, iti);
    
    //挿入データをコピー
    memcpy(outBuf->buf + iti, rdf2, rdf2Length);
    
    //元データ後半部をコピー
    memcpy((outBuf->buf + iti + rdf2Length), &(readJpegBuffer->buf[iti]), readJpegBuffer->len - iti);
    
    FILE *fpw;
    fpw = fopen(dst, "wb");
    if (fpw == NULL) {
        printf( "書込用ファイルが開けません");
        return -1;
    }
    
    size_t writeSize = 1;
    
    fwrite(outBuf->buf, writeSize, readJpegBuffer->len + rdf2Length, fpw);
    
    fclose(fpw);
    fclose(fpr);
    
    //メモリ解放
    //rdfLength = NULL;
    outBuf = NULL;
    
    fpw = NULL;
    fpr = NULL;
    
    return 0;
    
}





