/*!
@file sha256.c
@date Created on: 2015/08/10
@author DATT JAPAN Inc.
@version 1.1
@brief ハッシュ値 (SHA-256) 計算モジュールのソースコード
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "sha256.h"

/*!
@struct UIntHashBuffer
@brief 整数型の配列と長さのペア
*/
typedef struct
{
    size_t _len;          /*!< @brief 値の長さ */
    unsigned int _buff[]; /*!< @brief 値 */
} UIntHashBuffer;

/* @name ビットマスク */
/* @{ */
#define MASK_64BIT (0x3F)             /*!< @brief 下 6 ビットのみを抽出するマスク */
#define MASK_1BYTE (0xFF)             /*!< @brief 1 バイトのみを抽出するマスク */
#define MASK_TOP (0x80000000U)        /*!< @brief 最上位ビットのみを抽出するマスク */
#define MASK_IGNORE_TOP (0x7FFFFFFFU) /*!< @brief 最上位ビットのみを削除するマスク */
/* @} */

/* @name ビット操作 */
/* @{ */
#define SHR(target, shift)  (target >> shift)                               /*!< @brief 右ビットシフト */
#define ROTR(target, shift) (SHR(target, shift) | (target << (32 - shift))) /*!< @brief 右ビット回転 */
/* @} */

/* @name 各種計算処理 */
/* @{ */
#define CH(x, y, z) ((x & y) ^ ((~x) & z))                      /*!< @brief ハッシュ計算処理 */
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))              /*!< @brief ハッシュ計算処理 */
#define SIGMA_A0(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))   /*!< @brief ハッシュ計算処理 */
#define SIGMA_A1(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))   /*!< @brief ハッシュ計算処理 */
#define SIGMA_B0(x) (ROTR(x,  7) ^ ROTR(x, 18) ^  SHR(x,  3))   /*!< @brief ハッシュ計算処理 */
#define SIGMA_B1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^  SHR(x, 10))   /*!< @brief ハッシュ計算処理 */
/* @} */

/* @name 定数マクロ定義 */
/* @{ */
#define BYTE_SIZE_ORIDINAL_DATA_LENGTH ((size_t)8)              /*!< @brief ハッシュ作成元データの本来のサイズをビットで格納する際の長さ */
#define BYTE_SIZE_MESSAGE_BLOCK ((size_t)64)                    /*!< @brief メッセージブロックのビット数 */

#define BYTE_SIZE_UNSIGNED_INT_HASH (sizeof(DEFAULT_UINT_HASH)) /*!< @brief 整数値で計算を行う際のハッシュのバイトサイズ */

#define PADDING_DELIMITER_BYTE (0x80)                           /*!< @brief パディングの区切りバイト */
/* @} */

/*! 定数 K */
static const unsigned int UNSIGNED_INT_K[] =
{
    0x428A2F98U, 0x71374491U, 0xB5C0FBCFU, 0xE9B5DBA5U, 0x3956C25BU, 0x59F111F1U, 0x923F82A4U, 0xAB1C5ED5U,
    0xD807AA98U, 0x12835B01U, 0x243185BEU, 0x550C7DC3U, 0x72BE5D74U, 0x80DEB1FEU, 0x9BDC06A7U, 0xC19BF174U,
    0xE49B69C1U, 0xEFBE4786U, 0x0FC19DC6U, 0x240CA1CCU, 0x2DE92C6FU, 0x4A7484AAU, 0x5CB0A9DCU, 0x76F988DAU,
    0x983E5152U, 0xA831C66DU, 0xB00327C8U, 0xBF597FC7U, 0xC6E00BF3U, 0xD5A79147U, 0x06CA6351U, 0x14292967U,
    0x27B70A85U, 0x2E1B2138U, 0x4D2C6DFCU, 0x53380D13U, 0x650A7354U, 0x766A0ABBU, 0x81C2C92EU, 0x92722C85U,
    0xA2BFE8A1U, 0xA81A664BU, 0xC24B8B70U, 0xC76C51A3U, 0xD192E819U, 0xD6990624U, 0xF40E3585U, 0x106AA070U,
    0x19A4C116U, 0x1E376C08U, 0x2748774CU, 0x34B0BCB5U, 0x391C0CB3U, 0x4ED8AA4AU, 0x5B9CCA4FU, 0x682E6FF3U,
    0x748F82EEU, 0x78A5636FU, 0x84C87814U, 0x8CC70208U, 0x90BEFFFAU, 0xA4506CEBU, 0xBEF9A3F7U, 0xC67178F2U
};

/*! ハッシュの初期値 */
static const unsigned int DEFAULT_UINT_HASH[] =
{
    0x6A09E667U, 0xBB67AE85U, 0x3C6EF372U, 0xA54FF53AU,
    0x510E527FU, 0x9B05688CU, 0x1F83D9ABU, 0x5BE0CD19U
};

/*!
@brief バイト配列構造体の指定した範囲を反転させる。
@param [out] *byteArray 対象となるバイト配列構造体
@param startIndex 反転させる範囲の先頭のインデックス
@param length 反転させる範囲の長さ
*/
void reverseByteArray(HashBuffer *byteArray, unsigned long startIndex, size_t length)
{
    unsigned long i;
    unsigned long j;

    for(i = startIndex, j = startIndex + length - 1; i < j; ++i, --j)
    {
        unsigned char tmp = byteArray->_buff[i];
        byteArray->_buff[i] = byteArray->_buff[j];
        byteArray->_buff[j] = tmp;
    }
}

/*!
@brief バイト配列構造体の指定したインデックスから 4 バイトまでを符号なし整数値に変換する。
@details startIndexがバイト配列構造体の長さを超過した場合、その時点で計算を止めて結果を返す。
@param [in] *byteArray 対象となるバイト配列構造体
@param startIndex 処理の開始位置となる先頭のインデックス
@return 計算結果
*/
unsigned int byteArrayToUInt(HashBuffer *byteArray, int startIndex)
{
    unsigned int i;
    unsigned int result = 0U;

    for(i = 0; i < 4 && i + startIndex < byteArray->_len; ++i)
    {
        result += (byteArray->_buff[startIndex + i] << (i << 3));
    }

    return result;
}

/*!
@brief 符号なし倍精度整数をバイトごとの配列に分割する。
@param [out] *result 変換結果を格納するバイト配列構造体
@param value 変換対象の数値
@param length 変換するバイトの長さ ( 最大 8 バイトまで )
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER 対象となる数のバイト数が多すぎる場合
@retval OTHER_ERROR 配列の長さが十分でない場合
*/
int getBytes(HashBuffer *result, unsigned long long int value, size_t length)
{
    unsigned int i;

    if(sizeof(value) < length)
    {
        /* 対象となる数のバイト数が多すぎる */
        return INCORRECT_PARAMETER;
    }
    else if(result->_len < length)
    {
        /* 配列の長さが十分でない */
        return OTHER_ERROR;
    }

    for(i = 0; i < length; ++i)
    {
        result->_buff[i] = (value >> (i << 3)) & MASK_1BYTE;
    }

    return FUNCTION_SUCCESS;
}

/*!
@brief オーバーフローを起こさないよう、計算した結果溢れた桁を無視する加算処理を行う。
@param paramLength この値を除いた引数の個数
@return 計算結果
*/
unsigned int safetyAdd(int paramLength, ...)
{
    va_list args;
    unsigned int result = 0U;
    int i;

    va_start(args, paramLength);

    /* 引数の数だけ加算する */
    for(i = 0; i < paramLength; ++i)
    {
        unsigned int value = va_arg(args, unsigned int);
        result = ((result & MASK_IGNORE_TOP) + (value & MASK_IGNORE_TOP)) ^ (result & MASK_TOP) ^ (value & MASK_TOP);
    }

    va_end(args);

    return result;
}

/*!
@brief ハッシュ (SHA-256) の計算処理を行う。
@param [in] *block 計算のもととなるバイト配列構造体
@param [out] *hash 最終的な結果を受け取る整数配列構造体
*/
void computation(HashBuffer *block, UIntHashBuffer *hash)
{
    unsigned int uIntWord[64];

    /* ローテーション用 */
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
    unsigned int e;
    unsigned int f;
    unsigned int g;
    unsigned int h;

    /* ループカウンタ */
    int i;

    /* 現在のハッシュを複製 */
    a = hash->_buff[0];
    b = hash->_buff[1];
    c = hash->_buff[2];
    d = hash->_buff[3];
    e = hash->_buff[4];
    f = hash->_buff[5];
    g = hash->_buff[6];
    h = hash->_buff[7];

    /* ローテーション処理 */
    for(i = 0; i < 64; ++i)
    {
        /* ローテーション処理中に利用する一時変数 */
        unsigned int uIntTmp1;
        unsigned int uIntTmp2;

        /* 0 ～ 15は、Block の配列を 4 バイトずつ uIntWord に代入していく */
        if(i < 16)
        {

            /* リトルエンディアンの場合は反転させてビッグエンディアンにする */
            if(getEndian() == JACIC_LITTLE_ENDIAN)
            {
                reverseByteArray(block, i * BYTE_SIZE_UNSIGNED_INT, BYTE_SIZE_UNSIGNED_INT);
            }

            uIntWord[i] = byteArrayToUInt(block, i * BYTE_SIZE_UNSIGNED_INT);

            /* 16 ～ 63は、すでに代入された値から生成する */
        }
        else
        {

            uIntWord[i] = safetyAdd(4,
                                    SIGMA_B1(uIntWord[i -  2]), uIntWord[i -  7],
                                    SIGMA_B0(uIntWord[i - 15]), uIntWord[i - 16]
                                   );

        }

        /* ローテーション時に変化を与える値を算出 */
        uIntTmp1 = safetyAdd(5, h, SIGMA_A1(e), CH(e, f, g), UNSIGNED_INT_K[i], uIntWord[i]);
        uIntTmp2 = safetyAdd(2, SIGMA_A0(a), MAJ(a, b, c));

        h = g;
        g = f;
        f = e;
        e = safetyAdd(2, d, uIntTmp1);
        d = c;
        c = b;
        b = a;
        a = safetyAdd(2, uIntTmp1, uIntTmp2);

    }

    /* 算出されたハッシュを現在のハッシュに加算 */
    hash->_buff[0] = safetyAdd(2, hash->_buff[0], a);
    hash->_buff[1] = safetyAdd(2, hash->_buff[1], b);
    hash->_buff[2] = safetyAdd(2, hash->_buff[2], c);
    hash->_buff[3] = safetyAdd(2, hash->_buff[3], d);
    hash->_buff[4] = safetyAdd(2, hash->_buff[4], e);
    hash->_buff[5] = safetyAdd(2, hash->_buff[5], f);
    hash->_buff[6] = safetyAdd(2, hash->_buff[6], g);
    hash->_buff[7] = safetyAdd(2, hash->_buff[7], h);

}

/*!
@brief 可変長のバイト配列を受けてハッシュ値を返す。
@param [in] *byteArray ハッシュ値を計算するもととなるバイト配列構造体
@param [out] *result 最終的に算出されたハッシュ値を受けとる変数
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER 不正な引数により、内部で使用する関数でエラーが発生した場合
@retval OTHER_ERROR メモリの確保に失敗した場合、ハッシュ値の格納に失敗した場合など
*/
int hash(HashBuffer *byteArray, HashBuffer *result)
{
    int ret = FUNCTION_SUCCESS;
    HashBuffer *orgLength = NULL;
    HashBuffer *resizedByteArray = NULL;
    HashBuffer *block = NULL;
    HashBuffer *hashBytes = NULL;
    UIntHashBuffer *uIntHash = NULL;
    size_t tmpByteArrayLength = 0;
    size_t totalBytes = 0;
    size_t totalBlocks = 0;
    size_t orgLengthMemorySize = sizeof(HashBuffer) + BYTE_SIZE_ORIDINAL_DATA_LENGTH;
    size_t resizedByteArrayMemorySize = 0;
    size_t blockMemorySize = sizeof(HashBuffer) + BYTE_SIZE_MESSAGE_BLOCK;
    size_t uIntHashMemorySize = sizeof(HashBuffer) + sizeof(DEFAULT_UINT_HASH);
    size_t hashBytesMemorySize = sizeof(HashBuffer) + BYTE_SIZE_UNSIGNED_INT;
    unsigned int i;
    unsigned int j;
    unsigned int k;

    /* 元データの長さをビット数で記憶する変数 */
    orgLength = (HashBuffer *) malloc(orgLengthMemorySize);

    if(orgLength == NULL)
    {
        /* メモリ確保失敗 */
        ret = OTHER_ERROR;
        goto FINALIZE;
    }

    /* 領域のゼロクリア */
    memset(orgLength, 0x00, orgLengthMemorySize);

    orgLength->_len = BYTE_SIZE_ORIDINAL_DATA_LENGTH;

    /* 元データの長さをビット数 ( 8 倍 ) で記憶 */
    ret = getBytes(orgLength, byteArray->_len * BIT_SIZE_1BYTE, BYTE_SIZE_ORIDINAL_DATA_LENGTH);

    if(ret != FUNCTION_SUCCESS)
    {
        goto FINALIZE;    /* 処理失敗 */
    }

    /* リトルエンディアンの場合は反転させてビッグエンディアンにする */
    if(getEndian() == JACIC_LITTLE_ENDIAN)
    {
        reverseByteArray(orgLength, 0, BYTE_SIZE_ORIDINAL_DATA_LENGTH);
    }

    /* 配列を 1 増加させた長さを取得 */
    tmpByteArrayLength = byteArray->_len + 1;

    /*
     * ブロックの総数を算出
     *  1 ブロックあたりのバイト数で割り + 1 する
     */
    totalBlocks = tmpByteArrayLength / BYTE_SIZE_MESSAGE_BLOCK + 1;

    /*
     * 最後のメッセージブロックが 56 ( 全体 64 バイト - 元データ長の 8 バイト表現 ) バイトを超えている場合、
     * メッセージブロックを一つ増やす
     */
    if((BYTE_SIZE_MESSAGE_BLOCK - BYTE_SIZE_ORIDINAL_DATA_LENGTH) < (tmpByteArrayLength & MASK_64BIT))
    {
        totalBlocks += 1;
    }

    /* 最終的なバイト数は [ ブロック数 ] * [ 1 ブロックあたりのバイト数 ] */
    totalBytes = totalBlocks * BYTE_SIZE_MESSAGE_BLOCK;

    /* 総ブロック数の領域を確保 */
    resizedByteArrayMemorySize = sizeof(HashBuffer) + (BYTE_SIZE_UNSIGNED_CHAR * totalBytes);
    resizedByteArray = (HashBuffer *) malloc(resizedByteArrayMemorySize);

    if(resizedByteArray == NULL)
    {
        /* メモリ確保失敗 */
        ret = OTHER_ERROR;
        goto FINALIZE;
    }

    resizedByteArray->_len = totalBytes;

    /* 領域のゼロクリア */
    memset(resizedByteArray, 0x00, resizedByteArrayMemorySize);

    /* 元データをコピー */
    memcpy(resizedByteArray->_buff, byteArray->_buff, byteArray->_len);

    /* パディング用の区切り文字 */
    resizedByteArray->_buff[byteArray->_len] = PADDING_DELIMITER_BYTE;

    /* malloc で増加した領域（末尾の元データ長を埋める 8 バイトを除く）のゼロクリア */
    memset(
        resizedByteArray->_buff + tmpByteArrayLength,
        0x00,
        totalBytes - tmpByteArrayLength - BYTE_SIZE_ORIDINAL_DATA_LENGTH
    );

    /* データ長を末尾にコピー */
    memcpy(
        resizedByteArray->_buff + totalBytes - BYTE_SIZE_ORIDINAL_DATA_LENGTH,
        orgLength->_buff,
        BYTE_SIZE_ORIDINAL_DATA_LENGTH
    );


    /* メッセージブロック用の配列 */
    block = (HashBuffer *) malloc(blockMemorySize);

    if(block == NULL)
    {
        /* メモリ確保失敗 */
        ret = OTHER_ERROR;
        goto FINALIZE;
    }

    /* 領域のゼロクリア */
    memset(block, 0x00, blockMemorySize);

    block->_len = BYTE_SIZE_MESSAGE_BLOCK;

    /* ハッシュの初期値 */
    uIntHash = (UIntHashBuffer *) malloc(uIntHashMemorySize);

    if(uIntHash == NULL)
    {
        /* メモリ確保失敗 */
        ret = OTHER_ERROR;
        goto FINALIZE;
    }

    /* 領域のゼロクリア */
    memset(uIntHash, 0x00, uIntHashMemorySize);

    uIntHash->_len = sizeof(DEFAULT_UINT_HASH) / BYTE_SIZE_UNSIGNED_INT;

    memcpy(uIntHash->_buff, DEFAULT_UINT_HASH, sizeof(DEFAULT_UINT_HASH));

    /* ハッシュ計算処理 */
    for(i = 0; i < totalBytes; i += BYTE_SIZE_MESSAGE_BLOCK)
    {

        /* メッセージブロックにデータをコピー */
        memcpy(block->_buff, resizedByteArray->_buff + i, BYTE_SIZE_MESSAGE_BLOCK);

        /* メッセージブロックのハッシュを計算 */
        computation(block, uIntHash);
    }


    /* データの長さのビット数表示をバイトに変換するための変数 */
    hashBytes = (HashBuffer *) malloc(hashBytesMemorySize);

    if(hashBytes == NULL)
    {
        /* メモリ確保失敗 */
        ret = OTHER_ERROR;
        goto FINALIZE;
    }

    /* 領域のゼロクリア */
    memset(hashBytes, 0x00, hashBytesMemorySize);

    hashBytes->_len = BYTE_SIZE_UNSIGNED_INT;

    /* 符号なし整数型の配列から 1 バイトごとの配列に変換 */
    for(i = 0; i < (sizeof(DEFAULT_UINT_HASH) / BYTE_SIZE_UNSIGNED_INT); ++i)
    {

        ret = getBytes(hashBytes, uIntHash->_buff[i], BYTE_SIZE_UNSIGNED_INT);

        if(ret != FUNCTION_SUCCESS)
        {
            goto FINALIZE;    /* 処理失敗 */
        }

        /* リトルエンディアンの場合は反転させてビッグエンディアンにする */
        if(getEndian() == JACIC_LITTLE_ENDIAN)
        {
            reverseByteArray(hashBytes, 0, BYTE_SIZE_UNSIGNED_INT);

        }

        /* 数値を文字に変換し、結果の配列に格納する */
        for(j = 0; j < BYTE_SIZE_UNSIGNED_INT; ++j)
        {

            /* 1 バイトを 2 文字に変換 */
            for(k = 0; k < 2; ++k)
            {

                ret = getCharCode(
                          &(result->_buff[i * BYTE_SIZE_UNSIGNED_INT * 2 + j * 2 + k]),
                          (hashBytes->_buff[j] >> (4 * (1 - k))) & 0x0F
                      );

                if(ret != FUNCTION_SUCCESS)
                {
                    goto FINALIZE;
                }

            }

        }

    }

    result->_len = BYTE_SIZE_HASH_LENGTH;

FINALIZE:

    /* メモリ解放 */
    if(orgLength != NULL)
    {
        free(orgLength);
        orgLength = NULL;
    }

    if(resizedByteArray != NULL)
    {
        free(resizedByteArray);
        resizedByteArray = NULL;
    }

    if(block != NULL)
    {
        free(block);
        block = NULL;
    }

    if(uIntHash != NULL)
    {
        free(uIntHash);
        uIntHash = NULL;
    }

    if(hashBytes != NULL)
    {
        free(hashBytes);
        hashBytes = NULL;
    }

    return ret;
}
