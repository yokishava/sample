/*!
@file writeHashLib.c
@date Created on: 2015/08/10
@author DATT JAPAN Inc.
@version 1.1
@brief 改ざんチェック用ハッシュ埋め込みモジュールのソースコード
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app1.h"
#include "app5.h"
#include "common.h"
#include "exif.h"
#include "sha256.h"
#include "writeHashLib.h"

/*!
@brief 指定ファイルの存在チェックを行う。
@param [in] *src 検索ファイルパス
@retval FUNCTION_SUCCESS ファイルが存在する
@retval FILE_NOT_EXISTS ファイルが存在しない
*/
int _fileExist(const char *src)
{
    int ret = FUNCTION_SUCCESS;
    FILE *fp = NULL;

    fp = fopen(src, "r");

    if(fp == NULL)
    {
        /* ファイルが存在しない */
        ret = FILE_NOT_EXISTS;

    }
    else
    {
        /* ファイルクローズ */
        int closeResult = fclose(fp);

        if(closeResult == EOF)
        {
            /* ファイルクローズに失敗 */
            ret = FILE_CLOSE_FAILED;
        }

        fp = NULL;

    }

    return ret;
}

/*!
@brief ファイルのサイズを取得する。
@param [out] *result ファイルサイズを格納する符号なし長整数
@param [in] *fp 検査対象となるファイルのポインタ
@retval FUNCTION_SUCCESS 正常終了
@retval FILE_OPEN_FAILED ファイルオープンに失敗した場合
@retval OTHER_ERROR ファイル読み込み途中でエラーが発生した場合
*/
int _fileSize(unsigned long *result, FILE *fp)
{
    int ret;
    fpos_t fsize;

    if(fp == NULL)
    {
        return FILE_OPEN_FAILED;
    }

    /* ファイルサイズ取得 */
    ret = fseek(fp, 0L, SEEK_END);

    if(ret != 0)
    {
        return OTHER_ERROR;
    }

    ret = fgetpos(fp, &fsize);

    if(ret != 0)
    {
        return OTHER_ERROR;
    }

    *result = (unsigned long)GET_POS(fsize);

    /* ファイルポインタを戻す */
    ret = fseek(fp, 0L, SEEK_SET);

    if(ret != 0)
    {
        return OTHER_ERROR;
    }

    return FUNCTION_SUCCESS;
}

/*!
@brief APP5 領域から取得したハッシュ値と画像データから生成したハッシュ値を比較した結果を返す。
@param app5ImageHash APP5 領域から取得したハッシュ値（画像）
@param [in] *dataImageHash 画像データから生成したハッシュ値（画像）
@param app5DateHash APP5 領域から取得したハッシュ値（撮影日時）
@param [in] *dataDateHash 画像データから生成したハッシュ値（撮影日時）
@retval SAME_HASH 画像、撮影日時のどちらもハッシュ値が等しい
@retval INCORRECT_HASH_BOTH 画像、撮影日時のどちらもハッシュ値が等しくない
@retval INCORRECT_HASH_IMAGE 画像のハッシュ値が等しくなく、撮影日時のハッシュ値が等しい
@retval INCORRECT_HASH_DATE 画像のハッシュ値が等しく、撮影日時のハッシュ値が等しくない
*/
int _compareHashValues(RMetaItem app5ImageHash, HashBuffer *dataImageHash, RMetaItem app5DateHash, HashBuffer *dataDateHash)
{
    int imageResult = -1;
    int dateResult  = -1;

    /* APP5領域にいずれのハッシュ値も存在しない */
    if(app5ImageHash.titleExistsFlag == JACIC_FLAG_OFF &&
            app5DateHash.titleExistsFlag == JACIC_FLAG_OFF)
    {
        /* ハッシュ値の存在しない画像 */
        return HASH_NOT_EXISTS;
    }

    if(app5ImageHash.value != NULL &&
            dataImageHash != NULL)
    {
        imageResult = memcmp(app5ImageHash.value->_buff, dataImageHash->_buff, BYTE_SIZE_HASH_LENGTH);
    }

    if(app5DateHash.value != NULL &&
            dataDateHash != NULL)
    {
        dateResult  = memcmp(app5DateHash.value->_buff,  dataDateHash->_buff,  BYTE_SIZE_HASH_LENGTH);
    }

    if(imageResult == 0 && dateResult == 0)
    {
        /* 画像 : OK , 日時 : OK */
        return SAME_HASH;
    }
    else if(imageResult != 0 && dateResult == 0)
    {
        /* 画像 : NG , 日時 : OK */
        return INCORRECT_HASH_IMAGE;
    }
    else if(imageResult == 0 && dateResult != 0)
    {
        /* 画像 : OK , 日時 : NG */
        return INCORRECT_HASH_DATE;
    }
    else
    {
        /* 画像 : NG , 日時 : NG */
        return INCORRECT_HASH_BOTH;
    }
}

/*!
@brief ハッシュ書き込み関数で内部利用している戻り値定数を外部公開用の定数に置き換える。
@param retval 対象となる戻り値
*/
int _hashWriteReturnValueConvert(int retval)
{
    int ret;

    switch(retval)
    {
    case FUNCTION_SUCCESS:
        ret = JW_SUCCESS;
        break;

    case INCORRECT_PARAMETER:
        ret = JW_ERROR_INCORRECT_PARAMETER;
        break;

    case SAME_FILE_PATH:
        ret = JW_ERROR_SAME_FILE_PATH;
        break;

    case FILE_NOT_EXISTS:
        ret = JW_ERROR_READ_FILE_NOT_EXISTS;
        break;

    case FILE_ALREADY_EXISTS:
        ret = JW_ERROR_WRITE_FILE_ALREADY_EXISTS;
        break;

    case FILE_OPEN_FAILED:
        ret = JW_ERROR_READ_FILE_OPEN_FAILED;
        break;

    case FILE_SIZE_ZERO:
        ret = JW_ERROR_READ_FILE_SIZE_ZERO;
        break;

    case FILE_WRITE_FAILED:
        ret = JW_ERROR_WRITE_FILE_FAILED;
        break;

    case FILE_CLOSE_FAILED:
        ret = JW_ERROR_FILE_CLOSE_FAILED;
        break;

    case INCORRECT_EXIF_FORMAT:
        ret = JW_ERROR_INCORRECT_EXIF_FORMAT;
        break;

    case APP5_ALREADY_EXISTS:
        ret = JW_ERROR_APP5_ALREADY_EXISTS;
        break;

    case OTHER_ERROR:
    default:
        ret = JW_ERROR_OTHER;
        break;
    }

    return ret;
}

/*!
@brief ハッシュチェック関数で内部利用している戻り値定数を外部公開用の定数に置き換える。
@param retval 対象となる戻り値
*/
int _hashCheckReturnValueConvert(int retval)
{
    int ret;

    switch(retval)
    {
    case SAME_HASH:
        ret = JC_OK;
        break;

    case INCORRECT_HASH_BOTH:
        ret = JC_NG;
        break;

    case INCORRECT_HASH_IMAGE:
        ret = JC_NG_IMAGE;
        break;

    case INCORRECT_HASH_DATE:
        ret = JC_NG_DATE;
        break;

    case INCORRECT_PARAMETER:
        ret = JC_ERROR_INCORRECT_PARAMETER;
        break;

    case FILE_NOT_EXISTS:
        ret = JC_ERROR_READ_FILE_NOT_EXISTS;
        break;

    case FILE_OPEN_FAILED:
        ret = JC_ERROR_READ_FILE_OPEN_FAILED;
        break;

    case FILE_SIZE_ZERO:
        ret = JC_ERROR_READ_FILE_SIZE_ZERO;
        break;

    case FILE_CLOSE_FAILED:
        ret = JC_ERROR_FILE_CLOSE_FAILED;
        break;

    case INCORRECT_EXIF_FORMAT:
        ret = JC_ERROR_INCORRECT_EXIF_FORMAT;
        break;

    case APP5_NOT_EXISTS:
        ret = JC_ERROR_APP5_NOT_EXISTS;
        break;

    case INCORRECT_APP5_FORMAT:
        ret = JC_ERROR_INCORRECT_APP5_FORMAT;
        break;

    case HASH_NOT_EXISTS:
        ret = JC_ERROR_HASH_NOT_EXISTS;
        break;

    case OTHER_ERROR:
    default:
        ret = JC_ERROR_OTHER;
        break;
    }

    return ret;
}

/*!
@brief 指定されたJPEGファイルに改ざんチェック値を埋め込んだファイルを出力する。
@brief sourceFileとdestFileには同じファイルを設定できない。

@param [in] *sourceFile 改ざんチェック値を埋め込みたいファイル名
@param [out] *destFile 改ざんチェック値を埋め込んだファイル名

@retval JW_SUCCESS                            0 : 正常終了

@retval JW_ERROR_INCORRECT_PARAMETER       -101 : 不正な引数が指定された場合
@retval JW_ERROR_SAME_FILE_PATH            -102 : 読込元と出力先の画像ファイルパスが同じ

@retval JW_ERROR_READ_FILE_NOT_EXISTS      -201 : 読込元画像ファイルが存在しない
@retval JW_ERROR_WRITE_FILE_ALREADY_EXISTS -202 : 出力先画像ファイルが既に存在する
@retval JW_ERROR_READ_FILE_OPEN_FAILED     -203 : ファイルオープン失敗
@retval JW_ERROR_READ_FILE_SIZE_ZERO       -204 : 読み込んだファイルサイズがゼロ
@retval JW_ERROR_WRITE_FILE_FAILED         -205 : ファイル書き込み失敗
@retval JW_ERROR_FILE_CLOSE_FAILED         -206 : ファイルのクローズに失敗

@retval JW_ERROR_INCORRECT_EXIF_FORMAT     -301 : Exif フォーマットが不正
@retval JW_ERROR_APP5_ALREADY_EXISTS       -302 : APP5 セグメントが既に存在する

@retval JW_ERROR_OTHER                     -900 : その他のエラー
*/
int WINAPI JACIC_WriteHashValue(const char *sourceFile, const char *destFile)
{
    int ret = JW_SUCCESS;
    int closeResult;
    unsigned long fsize = 0UL;
    unsigned long readSize = 0UL;
    FILE *fp = NULL;
    JpegBuffer *readJpegBuffer = NULL;
    HashBuffer *dateBuff = NULL;
    HashBuffer *dateHash = NULL;
    HashBuffer *imgBuff = NULL;
    HashBuffer *imageHash = NULL;
    size_t readJpegBufferMemorySize;
    size_t hashMemorySize = sizeof(HashBuffer) + (BYTE_SIZE_UNSIGNED_CHAR * BYTE_SIZE_HASH_LENGTH);

    /* パラメータが不正 */
    if(sourceFile == NULL || destFile == NULL)
    {
        return JW_ERROR_INCORRECT_PARAMETER;
    }

    /* 読込元と出力先の画像ファイルパスが同じ場合 */
    if(strcmp(sourceFile, destFile) == 0)
    {
        return JW_ERROR_SAME_FILE_PATH;
    }

    /* 読込元画像ファイルが存在しない場合 */
    if(_fileExist(sourceFile) != FUNCTION_SUCCESS)
    {
        return JW_ERROR_READ_FILE_NOT_EXISTS;
    }

    /* 出力先画像ファイルが既に存在する場合 */
    if(_fileExist(destFile) == FUNCTION_SUCCESS)
    {
        return JW_ERROR_WRITE_FILE_ALREADY_EXISTS;
    }


    /* ファイルオープン */
    fp = fopen(sourceFile, "rb");

    if(fp == NULL)
    {
        /* ファイルオープンエラー */
        ret = JW_ERROR_READ_FILE_OPEN_FAILED;
        goto FINALIZE;
    }

    /* ファイルサイズ取得 */
    ret = _fileSize(&fsize, fp);

    if(ret != FUNCTION_SUCCESS)
    {
        goto FINALIZE;
    }

    if(fsize == 0UL)
    {
        /* ファイルサイズゼロ */
        ret = JW_ERROR_READ_FILE_SIZE_ZERO;
        goto FINALIZE;
    }

    /* 読込元画像 バイナリデータ領域確保 */
    readJpegBufferMemorySize = sizeof(JpegBuffer) + fsize;
    readJpegBuffer = (JpegBuffer *) malloc(readJpegBufferMemorySize);

    if(readJpegBuffer == NULL)
    {
        /* メモリ確保失敗 */
        ret = JW_ERROR_OTHER;
        goto FINALIZE;
    }

    /* 領域のゼロクリア */
    memset(readJpegBuffer, 0x00, readJpegBufferMemorySize);

    readJpegBuffer->_len = fsize;

    /* 読込元画像 バイナリデータ読込 */
    readSize = fread(readJpegBuffer->_buff, BYTE_SIZE_UNSIGNED_CHAR, fsize, fp);

    if(readSize < fsize)
    {
        /* ファイル読み込み失敗 */
        ret = JW_ERROR_READ_FILE_OPEN_FAILED;
    }

    /*
     * ハッシュ値計算に必要となる
     * 『画像の圧縮データ開始位置以降のバイナリ』を取得する
     */
    ret = clipCompressedImage(readJpegBuffer, &imgBuff, JACIC_FLAG_ON);

    if(ret != FUNCTION_SUCCESS)
    {
        goto FINALIZE;
    }

    imageHash = (HashBuffer *) malloc(hashMemorySize);

    if(imageHash == NULL)
    {
        /* メモリ確保失敗 */
        ret = JW_ERROR_OTHER;
        goto FINALIZE;
    }

    /* 領域のゼロクリア */
    memset(imageHash, 0x00, hashMemorySize);

    imageHash->_len = BYTE_SIZE_HASH_LENGTH;

    /* ハッシュ値計算 */
    ret = hash(imgBuff, imageHash);

    if(ret != FUNCTION_SUCCESS)
    {
        goto FINALIZE;
    }


    /*
     * APP1 領域からハッシュ値計算に必要となる『撮影日時情報』を取得する
     * APP1 領域が取得できなかった場合、または、
     * APP1 領域から撮影日時情報が取得できなかった場合は、
     * 空白データを生成する
     */
    ret = getDateTimeOriginal(readJpegBuffer, &dateBuff);

    if(ret != FUNCTION_SUCCESS)
    {
        /* 不正な Exif フォーマット */
        goto FINALIZE;
    }

    dateHash = (HashBuffer *) malloc(hashMemorySize);

    if(dateHash == NULL)
    {
        /* メモリ確保失敗 */
        ret = JW_ERROR_OTHER;
        goto FINALIZE;
    }

    /* 領域のゼロクリア */
    memset(dateHash, 0x00, hashMemorySize);

    dateHash->_len = BYTE_SIZE_HASH_LENGTH;

    /* ハッシュ値計算 */
    ret = hash(dateBuff, dateHash);

    if(ret != FUNCTION_SUCCESS)
    {
        goto FINALIZE;
    }


    /* 出力 */
    ret = writeApp5(readJpegBuffer, imageHash, dateHash, destFile);

FINALIZE:

    /* メモリ解放 */
    if(readJpegBuffer != NULL)
    {
        free(readJpegBuffer);
        readJpegBuffer = NULL;
    }

    if(dateBuff != NULL)
    {
        free(dateBuff);
        dateBuff = NULL;
    }

    if(dateHash != NULL)
    {
        free(dateHash);
        dateHash = NULL;
    }

    if(imgBuff != NULL)
    {
        free(imgBuff);
        imgBuff = NULL;
    }

    if(imageHash != NULL)
    {
        free(imageHash);
        imageHash = NULL;
    }

    /* ファイルクローズ */
    if(fp != NULL)
    {
        closeResult = fclose(fp);

        if(closeResult == EOF)
        {
            /* ファイルクローズに失敗 */
            ret = FILE_CLOSE_FAILED;
        }

        fp = NULL;
    }

    /* 戻り値を外部公開用の定数に置き換えて返す */
    return _hashWriteReturnValueConvert(ret);
}

/*!
@brief 指定されたJPEGファイルに正しい改ざんチェック値が設定されているかを確認する。
@param [in] *checkFile チェック対象のファイル名

@retval JC_OK                                 1 : ハッシュ値が一致した
@retval JC_NG                                 0 : ハッシュ値（画像、撮影日時の両方）が一致しない
@retval JC_NG_IMAGE                          -1 : ハッシュ値（画像）が一致しない
@retval JC_NG_DATE                           -2 : ハッシュ値（撮影日時）が一致しない

@retval JC_ERROR_INCORRECT_PARAMETER       -101 : 不正な引数が指定された場合

@retval JC_ERROR_READ_FILE_NOT_EXISTS      -201 : 読込元画像ファイルが存在しない
@retval JC_ERROR_READ_FILE_OPEN_FAILED     -203 : ファイルオープン失敗
@retval JC_ERROR_READ_FILE_SIZE_ZERO       -204 : 読み込んだファイルサイズがゼロ
@retval JC_ERROR_FILE_CLOSE_FAILED         -206 : ファイルのクローズに失敗

@retval JC_ERROR_INCORRECT_EXIF_FORMAT     -301 : Exif フォーマットが不正
@retval JC_ERROR_APP5_NOT_EXISTS           -303 : APP5 領域が見つからない
@retval JC_ERROR_INCORRECT_APP5_FORMAT     -304 : APP5 領域の記述形式が異なる
@retval JC_ERROR_HASH_NOT_EXISTS           -305 : ハッシュ値が設定されていない

@retval JC_ERROR_OTHER                     -900 : その他のエラー
*/
int WINAPI JACIC_CheckHashValue(const char *checkFile)
{
    int ret;
    int closeResult;
    unsigned long seek = 0UL;
    unsigned long fsize = 0UL;
    unsigned long readSize = 0UL;
    FILE *fp = NULL;
    JpegBuffer *readJpegBuffer = NULL;
    HashBuffer *generateImageHash = NULL;
    HashBuffer *generateDateHash = NULL;
    HashBuffer *imageBuff = NULL;
    HashBuffer *dateBuff = NULL;
    RMetaItem app5ImageHash = {JACIC_FLAG_OFF, NULL};
    RMetaItem app5DateHash = {JACIC_FLAG_OFF, NULL};
    size_t readJpegBufferMemorySize;
    size_t hashMemorySize = sizeof(HashBuffer) + (BYTE_SIZE_HASH_LENGTH * BYTE_SIZE_UNSIGNED_CHAR);

    /*
     * ファイル読み込みチェック
     */
    if(checkFile == NULL)
    {
        /* パラメータが不正 */
        return JC_ERROR_INCORRECT_PARAMETER;
    }

    /* チェック対象画像ファイルが存在しない場合 */
    if(_fileExist(checkFile) != FUNCTION_SUCCESS)
    {
        return JC_ERROR_READ_FILE_NOT_EXISTS;
    }

    /* ファイルオープン */
    fp = fopen(checkFile, "rb");

    if(fp == NULL)
    {
        /* ファイルオープンエラー */
        ret = JC_ERROR_READ_FILE_OPEN_FAILED;
        goto FINALIZE;
    }

    /* ファイルサイズ取得 */
    ret = _fileSize(&fsize, fp);

    if(ret != FUNCTION_SUCCESS)
    {
        goto FINALIZE;
    }

    if(fsize == 0UL)
    {
        /* ファイルサイズゼロ */
        ret = JC_ERROR_READ_FILE_SIZE_ZERO;
        goto FINALIZE;
    }

    /* チェック対象画像 バイナリデータ領域確保 */
    readJpegBufferMemorySize = sizeof(JpegBuffer) + fsize;
    readJpegBuffer = (JpegBuffer *) malloc(readJpegBufferMemorySize);

    if(readJpegBuffer == NULL)
    {
        /* メモリ確保失敗 */
        ret = JC_ERROR_OTHER;
        goto FINALIZE;
    }

    /* 領域のゼロクリア */
    memset(readJpegBuffer, 0x00, readJpegBufferMemorySize);

    readJpegBuffer->_len = fsize;

    /* チェック対象画像 バイナリデータ読込 */
    readSize = fread(readJpegBuffer->_buff, BYTE_SIZE_UNSIGNED_CHAR, fsize, fp);

    if(readSize < fsize)
    {
        /* ファイル読み込み失敗 */
        ret = JW_ERROR_READ_FILE_OPEN_FAILED;
        goto FINALIZE;
    }

    /*
     * SOI を検出する（検出できなければ Exif 形式のエラー）
     */
    ret = checkFirstFindSOI(readJpegBuffer, &seek);

    if(ret != FUNCTION_SUCCESS)
    {
        /* 不正な Exif フォーマット */
        goto FINALIZE;
    }

    /*
     * APP5 セグメント内のハッシュ値を確認、取得
     */
    ret = getApp5HashValue(readJpegBuffer, &app5ImageHash, &app5DateHash, seek);

    if(ret != FUNCTION_SUCCESS &&
            ret != HASH_NOT_EXISTS)
    {
        goto FINALIZE;
    }


    /*
     * チェック対象画像からハッシュ値取得
     */
    if(app5ImageHash.value != NULL &&
            app5ImageHash.value->_len == BYTE_SIZE_HASH_LENGTH)
    {
        /*
         * APP5 領域からハッシュ値計算に必要となる
         * 『画像の圧縮データ開始位置以降のバイナリ』を取得する
         */
        ret = clipCompressedImage(readJpegBuffer, &imageBuff, JACIC_FLAG_OFF);

        if(ret != FUNCTION_SUCCESS)
        {
            goto FINALIZE;
        }

        generateImageHash = (HashBuffer *) malloc(hashMemorySize);

        if(generateImageHash == NULL)
        {
            /* メモリ確保失敗 */
            ret = JC_ERROR_OTHER;
            goto FINALIZE;
        }

        /* 領域のゼロクリア */
        memset(generateImageHash, 0x00, hashMemorySize);

        generateImageHash->_len = BYTE_SIZE_HASH_LENGTH;

        /* ハッシュ値計算 */
        ret = hash(imageBuff, generateImageHash);

        if(ret != FUNCTION_SUCCESS)
        {
            goto FINALIZE;
        }
    }

    if(app5DateHash.value != NULL &&
            app5DateHash.value->_len == BYTE_SIZE_HASH_LENGTH)
    {
        /*
         * APP1 領域からハッシュ値計算に必要となる『撮影日時情報』を取得する
         * APP1 領域が取得できなかった場合、または、
         * APP1 領域から撮影日時情報が取得できなかった場合は、
         * 空白データを生成する
         */
        ret = getDateTimeOriginal(readJpegBuffer, &dateBuff);

        if(ret != FUNCTION_SUCCESS)
        {
            /* 不正な Exif フォーマット */
            goto FINALIZE;
        }

        generateDateHash = (HashBuffer *) malloc(hashMemorySize);

        if(generateDateHash == NULL)
        {
            /* メモリ確保失敗 */
            ret = JC_ERROR_OTHER;
            goto FINALIZE;
        }

        /* 領域のゼロクリア */
        memset(generateDateHash, 0x00, hashMemorySize);

        generateDateHash->_len = BYTE_SIZE_HASH_LENGTH;

        /* ハッシュ値計算 */
        ret = hash(dateBuff, generateDateHash);

        if(ret != FUNCTION_SUCCESS)
        {
            goto FINALIZE;
        }
    }

    /*
     * ハッシュ値一致判定
     */
    ret = _compareHashValues(app5ImageHash, generateImageHash, app5DateHash, generateDateHash);


FINALIZE:

    /* メモリ解放 */
    if(readJpegBuffer != NULL)
    {
        free(readJpegBuffer);
        readJpegBuffer = NULL;
    }

    if(app5ImageHash.value != NULL)
    {
        free(app5ImageHash.value);
        app5ImageHash.value = NULL;
    }

    if(generateImageHash != NULL)
    {
        free(generateImageHash);
        generateImageHash = NULL;
    }

    if(imageBuff != NULL)
    {
        free(imageBuff);
        imageBuff = NULL;
    }

    if(app5DateHash.value != NULL)
    {
        free(app5DateHash.value);
        app5DateHash.value = NULL;
    }

    if(generateDateHash != NULL)
    {
        free(generateDateHash);
        generateDateHash = NULL;
    }

    if(dateBuff != NULL)
    {
        free(dateBuff);
        dateBuff = NULL;
    }

    /* ファイルクローズ */
    if(fp != NULL)
    {
        closeResult = fclose(fp);

        if(closeResult == EOF)
        {
            /* ファイルクローズに失敗 */
            ret = FILE_CLOSE_FAILED;
        }

        fp = NULL;
    }

    /* 戻り値を外部公開用の定数に置き換えて返す */
    return _hashCheckReturnValueConvert(ret);
}
