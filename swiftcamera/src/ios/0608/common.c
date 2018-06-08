/*!
@file common.c
@date Created on: 2015/08/18
@author DATT JAPAN Inc.
@version 1.1
@brief 共通処理用ソースコード
*/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#include "common.h"

/*!
@brief 稼働環境のエンディアンを判定し、エンディアンを示す値を返す。
@retval BIG_ENDIAN ビッグエンディアン
@retval LITTLE_ENDIAN リトルエンディアン
*/
int getEndian(void)
{
    static int i = 1;
    int ret = JACIC_BIG_ENDIAN;

    if(((int)(*(char *)&i)) == 1)
    {
        ret = JACIC_LITTLE_ENDIAN;
    }

    return ret;
}

/*!
@brief 符号なし短整数値をエンディアン変換する。
@param src 符号なし短整数値
@return エンディアン変換後の符号なし短整数値
*/
unsigned short swapEndian16(unsigned short src)
{
    return (src << BIT_SIZE_1BYTE) | ((src >> BIT_SIZE_1BYTE) & 0x00FF);
}

/*!
@brief 符号なし整数値をエンディアン変換する。
@param src 符号なし整数値
@return エンディアン変換後の符号なし整数値
*/
unsigned int swapEndian32(unsigned int src)
{
    return (
               (src) >> (BIT_SIZE_1BYTE * 3) |
               (src & 0x00FF0000) >> (BIT_SIZE_1BYTE * 1) |
               (src & 0x0000FF00) << (BIT_SIZE_1BYTE * 1) |
               (src & 0x000000FF) << (BIT_SIZE_1BYTE * 3)
           );
}

/*!
@brief 数値を 16 進数の文字列表現で取得する。
@param [out] *dst 取得対象の文字変数のアドレス
@param hexChar 文字列表現に変換する 16 進数 1 文字で表現可能な数値
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER hexChar が不正である
*/
int getCharCode(unsigned char *dst, unsigned char hexChar)
{
    char tmp[2];

    if(0x0FU < hexChar)
    {
        /* 1 文字表現の範囲外 */
        return INCORRECT_PARAMETER;
    }

    sprintf(tmp, "%x", hexChar);
    *dst = (unsigned char)tmp[0];

    return FUNCTION_SUCCESS;
}

/*!
@brief ファイルにバッファの内容を length バイト書き込む。
@param [in] *dst 書き込み対象となるファイルパス
@param [out] *outBuff 書き込むバイト配列
@param length 書き込むバイト長
@retval FUNCTION_SUCCESS 正常終了
@retval FILE_OPEN_FAILED ファイルのオープンに失敗
@retval FILE_WRITE_FAILED ファイルへの書き込みに失敗
@retval FILE_CLOSE_FAILED ファイルのクローズに失敗
*/
int writeFile(const char *dst, unsigned char *outBuff, size_t length)
{
    int ret = FUNCTION_SUCCESS;
    int funcRet;
    unsigned long writeResult;
    FILE *fp = NULL;

    fp = fopen(dst, "wb");

    if(fp == NULL)
    {
        /* ファイルのオープンに失敗 */
        return FILE_OPEN_FAILED;
    }

    writeResult = fwrite(outBuff, BYTE_SIZE_UNSIGNED_CHAR, length, fp);

    if(writeResult < length)
    {
        /* ファイルへの書き込みに失敗 */
        ret = FILE_WRITE_FAILED;
    }

    /* ファイルクローズ */
    funcRet = fclose(fp);

    if(funcRet == EOF)
    {
        /* ファイルクローズに失敗 */
        ret = FILE_CLOSE_FAILED;
    }

    fp = NULL;

    return ret;
}
