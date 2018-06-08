/*!
@file exif.c
@date Created on: 2015/08/10
@author DATT JAPAN Inc.
@version 1.1
@brief JPEG 画像の Exif データ操作用モジュールのソースコード
*/

#include <stdlib.h>
#include <string.h>

#include "exif.h"

/*!
@name エンディアン定義
@{
*/
const unsigned short EXIF_LITTLE_ENDIAN  = 0x4949;  /*!< @brief エンディアン定義：リトルエンディアン (II) */
const unsigned short EXIF_BIG_ENDIAN     = 0x4D4D;  /*!< @brief エンディアン定義：ビッグエンディアン (MM) */
/*!
@}

@name メタデータ全般のバイトサイズ定義
@{
*/
const size_t BYTE_SIZE_SEGMENT_MARKER    = 2;       /*!< @brief APP セグメント定義のバイト数 */
const size_t BYTE_SIZE_ENDIAN            = 2;       /*!< @brief エンディアン定義のバイト数 */
const size_t BYTE_SIZE_SEGMENT_SIZE      = 2;       /*!< @brief サイズ定義のバイト数 */
/*!
@}

@name セグメントマーカー定義
@{
*/
const unsigned short EXIF_MARKER_SOF00 = 0xFFC0;    /*!< @brief SOF0 フレームヘッダ */
const unsigned short EXIF_MARKER_SOF01 = 0xFFC1;    /*!< @brief SOF1 フレームヘッダ */
const unsigned short EXIF_MARKER_SOF02 = 0xFFC2;    /*!< @brief SOF2 フレームヘッダ */
const unsigned short EXIF_MARKER_SOF03 = 0xFFC3;    /*!< @brief SOF3 フレームヘッダ */
const unsigned short EXIF_MARKER_DHT   = 0xFFC4;    /*!< @brief DHT ハフマンテーブル定義 */
const unsigned short EXIF_MARKER_SOF05 = 0xFFC5;    /*!< @brief SOF5 フレームヘッダ */
const unsigned short EXIF_MARKER_SOF06 = 0xFFC6;    /*!< @brief SOF6 フレームヘッダ */
const unsigned short EXIF_MARKER_SOF07 = 0xFFC7;    /*!< @brief SOF7 フレームヘッダ */
const unsigned short EXIF_MARKER_JPG00 = 0xFFC8;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_SOF09 = 0xFFC9;    /*!< @brief SOF9 フレームヘッダ */
const unsigned short EXIF_MARKER_SOF10 = 0xFFCA;    /*!< @brief SOF10 フレームヘッダ */
const unsigned short EXIF_MARKER_SOF11 = 0xFFCB;    /*!< @brief SOF11 フレームヘッダ */
const unsigned short EXIF_MARKER_DAC   = 0xFFCC;    /*!< @brief DAC 算術符号テーブル定義 */
const unsigned short EXIF_MARKER_SOF13 = 0xFFCD;    /*!< @brief SOF13 フレームヘッダ */
const unsigned short EXIF_MARKER_SOF14 = 0xFFCE;    /*!< @brief SOF14 フレームヘッダ */
const unsigned short EXIF_MARKER_SOF15 = 0xFFCF;    /*!< @brief SOF15 フレームヘッダ */
const unsigned short EXIF_MARKER_RST00 = 0xFFD0;    /*!< @brief RST0 リスタートマーカ */
const unsigned short EXIF_MARKER_RST01 = 0xFFD1;    /*!< @brief RST1 リスタートマーカ */
const unsigned short EXIF_MARKER_RST02 = 0xFFD2;    /*!< @brief RST2 リスタートマーカ */
const unsigned short EXIF_MARKER_RST03 = 0xFFD3;    /*!< @brief RST3 リスタートマーカ */
const unsigned short EXIF_MARKER_RST04 = 0xFFD4;    /*!< @brief RST4 リスタートマーカ */
const unsigned short EXIF_MARKER_RST05 = 0xFFD5;    /*!< @brief RST5 リスタートマーカ */
const unsigned short EXIF_MARKER_RST06 = 0xFFD6;    /*!< @brief RST6 リスタートマーカ */
const unsigned short EXIF_MARKER_RST07 = 0xFFD7;    /*!< @brief RST7 リスタートマーカ */
const unsigned short EXIF_MARKER_SOI   = 0xFFD8;    /*!< @brief SOI 圧縮データスタート */
const unsigned short EXIF_MARKER_EOI   = 0xFFD9;    /*!< @brief EOI 圧縮データ終了 */
const unsigned short EXIF_MARKER_SOS   = 0xFFDA;    /*!< @brief SOS スキャンヘッダ */
const unsigned short EXIF_MARKER_DQT   = 0xFFDB;    /*!< @brief DQT 量子化テーブル定義 */
const unsigned short EXIF_MARKER_DNL   = 0xFFDC;    /*!< @brief DNL ライン数定義 */
const unsigned short EXIF_MARKER_DRI   = 0xFFDD;    /*!< @brief DRI リスタートインターバル */
const unsigned short EXIF_MARKER_DHP   = 0xFFDE;    /*!< @brief DHP ハイアラーキカルプログレッション（階層数列）定義 */
const unsigned short EXIF_MARKER_EXP   = 0xFFDF;    /*!< @brief EXP 成分拡大 */
const unsigned short EXIF_MARKER_APP00 = 0xFFE0;    /*!< @brief APP0 セグメント */
const unsigned short EXIF_MARKER_APP01 = 0xFFE1;    /*!< @brief APP1 セグメント */
const unsigned short EXIF_MARKER_APP02 = 0xFFE2;    /*!< @brief APP2 セグメント */
const unsigned short EXIF_MARKER_APP03 = 0xFFE3;    /*!< @brief APP3 セグメント */
const unsigned short EXIF_MARKER_APP04 = 0xFFE4;    /*!< @brief APP4 セグメント */
const unsigned short EXIF_MARKER_APP05 = 0xFFE5;    /*!< @brief APP5 セグメント */
const unsigned short EXIF_MARKER_APP06 = 0xFFE6;    /*!< @brief APP6 セグメント */
const unsigned short EXIF_MARKER_APP07 = 0xFFE7;    /*!< @brief APP7 セグメント */
const unsigned short EXIF_MARKER_APP08 = 0xFFE8;    /*!< @brief APP8 セグメント */
const unsigned short EXIF_MARKER_APP09 = 0xFFE9;    /*!< @brief APP9 セグメント */
const unsigned short EXIF_MARKER_APP10 = 0xFFEA;    /*!< @brief APP10 セグメント */
const unsigned short EXIF_MARKER_APP11 = 0xFFEB;    /*!< @brief APP11 セグメント */
const unsigned short EXIF_MARKER_APP12 = 0xFFEC;    /*!< @brief APP12 セグメント */
const unsigned short EXIF_MARKER_APP13 = 0xFFED;    /*!< @brief APP13 セグメント */
const unsigned short EXIF_MARKER_APP14 = 0xFFEE;    /*!< @brief APP14 セグメント */
const unsigned short EXIF_MARKER_APP15 = 0xFFEF;    /*!< @brief APP15 セグメント */
const unsigned short EXIF_MARKER_JPG01 = 0xFFF0;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG02 = 0xFFF1;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG03 = 0xFFF2;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG04 = 0xFFF3;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG05 = 0xFFF4;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG06 = 0xFFF5;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG07 = 0xFFF6;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG08 = 0xFFF7;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG09 = 0xFFF8;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG10 = 0xFFF9;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG11 = 0xFFFA;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG12 = 0xFFFB;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG13 = 0xFFFC;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_JPG14 = 0xFFFD;    /*!< @brief JPEG 拡張用リザーブ */
const unsigned short EXIF_MARKER_COM   = 0xFFFE;    /*!< @brief Exif マーカーセグメント コメント */
/*! @} */


/*!
@brief Exif ファイルに格納されている画像開始位置マーカーの存在確認とその開始位置を取得する。
@param [in] *src 対象となる JPEG 画像のバイナリ読み込み結果データ構造体
@param [in, out] *seek 取得する走査開始位置
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER いずれかの引数の参照先が NULL の場合
@retval INCORRECT_EXIF_FORMAT 処理の最後まで SOI マーカーが見つからなかった場合
*/
int checkFirstFindSOI(JpegBuffer *src, unsigned long *seek)
{
    unsigned short seg = 0xFFFFU;

    /* パラメータチェック */
    if(src == NULL)
    {
        return INCORRECT_PARAMETER;
    }

    if(seek == NULL)
    {
        return INCORRECT_PARAMETER;
    }

    /* Exif スタートマーカー(SOI)チェック */
    seg = getMarkerSegment(src, seek);

    if(seg != EXIF_MARKER_SOI)
    {
        /*
         * スタートマーカー(SOI)でない場合、
         * 「Exif ファイルフォーマットが不正」としてエラーを返す
         */
        return INCORRECT_EXIF_FORMAT;
    }

    /* セグメントの長さ分、走査位置をずらす */
    *seek += BYTE_SIZE_SEGMENT_MARKER;

    /* 走査位置がデータ長を超える場合 */
    if(src->_len < *seek)
    {
        return INCORRECT_EXIF_FORMAT;
    }

    return FUNCTION_SUCCESS;
}

/*!
@brief 指定位置より、 EOI セグメントまで走査位置を移動する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param [in, out] *seek 走査位置
@return 取得した EOI セグメント（見つからなかった場合は 0xFFFF ）
*/
unsigned short seekToEOI(JpegBuffer *src, unsigned long *seek)
{
    unsigned short ret = 0xFFFF;
    unsigned short seg = 0xFFFF;

    /* パラメータチェック */
    if(src == NULL)
    {
        return ret;
    }

    if(seek == NULL)
    {
        return ret;
    }

    while(*seek < src->_len)
    {
        memcpy(&seg, &(src->_buff[*seek]), BYTE_SIZE_SEGMENT_SIZE);

        /* リトルエンディアンの場合は反転させてビッグエンディアンにする */
        if(getEndian() == JACIC_LITTLE_ENDIAN)
        {
            seg = swapEndian16(seg);
        }

        if(seg == EXIF_MARKER_EOI)
        {
            ret = seg;
            *seek += BYTE_SIZE_SEGMENT_SIZE;
            break;
        }

        *seek += 1;
    }

    return ret;
}

/*!
@brief 指定した走査位置より、「セグメントのサイズ定義」の値として 2 バイト取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param seek 指定位置
@return 取得した領域サイズ （見つからなかった、引数不正などで失敗した場合は 0x0000 ）
*/
unsigned short getAppSize(JpegBuffer *src, unsigned long seek)
{
    unsigned short appSize = 0x0000U;

    /* パラメータチェック */
    if(src == NULL)
    {
        return appSize;
    }

    if(seek + BYTE_SIZE_SEGMENT_SIZE <= src->_len)
    {
        memcpy(&appSize, &(src->_buff[seek]), BYTE_SIZE_SEGMENT_SIZE);

        /* リトルエンディアンの場合は反転させてビッグエンディアンにする */
        if(getEndian() == JACIC_LITTLE_ENDIAN)
        {
            appSize = swapEndian16(appSize);
        }
    }

    return appSize;
}

/*!
@brief 指定した走査位置より、「エンディアン定義」として 2 バイト取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param seek 指定位置
@return 取得した領域サイズ （見つからなかった、引数不正などで失敗した場合は 0x0000 ）
*/
unsigned short getByteOrder(JpegBuffer *src, unsigned long seek)
{
    unsigned short byteOrder = 0x0000U;

    /* パラメータチェック */
    if(src == NULL)
    {
        return byteOrder;
    }

    if(seek + BYTE_SIZE_ENDIAN <= src->_len)
    {
        memcpy(&byteOrder, &(src->_buff[seek]), BYTE_SIZE_ENDIAN);

        /* リトルエンディアンの場合は反転させてビッグエンディアンにする */
        if(getEndian() == JACIC_LITTLE_ENDIAN)
        {
            byteOrder = swapEndian16(byteOrder);
        }
    }

    return byteOrder;
}

/*!
@brief Exif ファイルフォーマット仕様に従い、指定位置のマーカーセグメントを取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param [in, out] *seek 指定位置
@return 取得したマーカーセグメント（見つからなかった場合は 0xFFFF ）
*/
unsigned short getMarkerSegment(JpegBuffer *src, unsigned long *seek)
{
    /*
     * マーカーセグメント取得の際、FFの連続は最後のFFを除いて無視する
     */
    unsigned short ret = 0xFFFF;
    unsigned short seg = 0xFFFF;

    /* パラメータチェック */
    if(src == NULL)
    {
        return ret;
    }

    if(seek == NULL)
    {
        return ret;
    }

    while(*seek + BYTE_SIZE_SEGMENT_MARKER <= src->_len)
    {
        memcpy(&seg, &(src->_buff[*seek]), BYTE_SIZE_SEGMENT_MARKER);

        /* リトルエンディアンの場合は反転させてビッグエンディアンにする */
        if(getEndian() == JACIC_LITTLE_ENDIAN)
        {
            seg = swapEndian16(seg);
        }

        if(seg == 0xFFFFU)
        {
            *seek += 1;
        }
        else
        {
            ret = seg;
            break;
        }

    }

    return ret;
}

/*!
@brief 走査開始位置から指定した識別子と同じバイト数を読み込み、等しいか調べる。
@param [in] *identifier 走査対象の文字列
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param [out] *startIndex 走査開始インデックス
@param size 読み込むバイト数
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER 不正な引数が指定された場合
@retval INCORRECT_IDENTIFIER 取得した識別子と引数の識別子が別だった場合
@retval OTHER_ERROR メモリ確保に失敗した場合
*/
int checkIdentifier(const char *identifier, JpegBuffer *src, unsigned long *startIndex, size_t size)
{
    int ret;
    unsigned char *tmpReadBuffer = NULL;
    size_t readBufferSize = BYTE_SIZE_UNSIGNED_CHAR * size;

    if(src == NULL)
    {
        return INCORRECT_PARAMETER;
    }

    if(src->_len < size)
    {
        return INCORRECT_PARAMETER;
    }

    if(src->_len < *startIndex)
    {
        return INCORRECT_PARAMETER;
    }

    tmpReadBuffer = (unsigned char *) malloc(readBufferSize);

    if(tmpReadBuffer == NULL)
    {
        /* メモリ確保失敗 */
        ret = OTHER_ERROR;
    }
    else
    {
        /* 領域のゼロクリア */
        memset(tmpReadBuffer, 0x00, readBufferSize);

        /* 指定された長さで識別子の取得 */
        memcpy(tmpReadBuffer, &(src->_buff[*startIndex]), readBufferSize);

        if(memcmp(identifier, (char *)tmpReadBuffer, readBufferSize) == 0)
        {
            ret = FUNCTION_SUCCESS;
        }
        else
        {
            ret = INCORRECT_IDENTIFIER;
        }
    }

    /* メモリ解放 */
    free(tmpReadBuffer);
    tmpReadBuffer = NULL;

    return ret;
}

/*!
@brief メタデータに対し、指定位置から終端文字までのテキストを走査し、取得する。
@param [in] *src 対象となるバイト配列構造体
@param [out] *text 取得するテキスト（終端文字を含む）
@param [in, out] *seek 走査開始位置。走査したバイト数だけ呼び出し元の数値も加算される
@param maxByteSize 最大走査数
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER 最大走査数が 0 の場合
@retval INCORRECT_TEXT 最大文字数まで終端文字が見つからなかった場合
*/
int getByteText(HashBuffer *src, unsigned char *text, unsigned long *seek, size_t maxByteSize)
{
    int ret = FUNCTION_SUCCESS;
    unsigned char c = 0x00;
    unsigned int i;

    if(maxByteSize == 0U)
    {
        /* 走査できない（不正な引数） */
        return INCORRECT_PARAMETER;
    }

    for(i = 0; i < maxByteSize; ++i)
    {
        c = src->_buff[(*seek)++];
        text[i] = c;

        if(c == 0x00)
        {
            /* 終端文字に到達 */
            break;
        }

    }

    if(c != 0x00)
    {
        /* 最後まで終端文字が見つからなかった */
        ret = INCORRECT_TEXT;
    }

    return ret;
}

/*!
@brief ハッシュ値計算に用いるデータ開始位置を取得する。
@param [in] *src JPEG画像のバイナリ読み込み結果データ構造体
@param [out] *retStartIndex 取得する圧縮データ開始位置
@param [out] *retEndIndex 取得する圧縮データ終了位置
@param ignoreApp5Flag FLAG_ON のとき、 APP5 領域を発見した際にエラーを返す
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER 不正な引数が指定された場合
@retval INCORRECT_EXIF_FORMAT Exif ファイルが不正な形式の場合
@retval APP5_ALREADY_EXISTS ignoreApp5Flag が FLAG_ON であり、APP5 領域が既に存在する場合
*/
static int getHashSourceData(JpegBuffer *src, unsigned long *retStartIndex, unsigned long *retEndIndex, int ignoreApp5Flag)
{
    unsigned short seg = 0xFFFFU;
    unsigned short len = 0x0000U;
    unsigned long seek = 0UL;

    /* パラメータチェック */
    if(src == NULL)
    {
        return INCORRECT_PARAMETER;
    }

    if(retStartIndex == NULL)
    {
        return INCORRECT_PARAMETER;
    }

    /* Exif スタートマーカー (SOI) チェック */
    seg = getMarkerSegment(src, &seek);

    if(seg != EXIF_MARKER_SOI)
    {
        /*
         * スタートマーカー (SOI) でない場合、
         * 「Exif ファイルフォーマットが不正」としてエラーを返す
         */
        return INCORRECT_EXIF_FORMAT;
    }

    /* セグメントの長さ分、走査位置をずらす */
    seek += BYTE_SIZE_UNSIGNED_SHORT;

    /* 走査位置がデータ長を超える場合 */
    if(src->_len < seek)
    {
        return INCORRECT_EXIF_FORMAT;
    }

    /* SOS の走査を行う */
    while(1)
    {
        seg = getMarkerSegment(src, &seek);

        /*
         * スキャンデータ開始セグメント (SOS) の場合
         */
        if(seg == EXIF_MARKER_SOS)
        {
            /* 以降をハッシュ値の計算に用いる為に走査位置を返す */
            *retStartIndex = seek;

            /* 走査位置をセグメント ID の後ろへ移動 */
            seek += BYTE_SIZE_UNSIGNED_SHORT;

            break;
        }

        /*
         * セグメントが EOI の場合
         */
        else if(seg == EXIF_MARKER_EOI)
        {
            /*
             * SOS よりも前に EOI が見つかったので、
             * 「 Exif フォーマットが不正」としてエラーを返す
             */
            return INCORRECT_EXIF_FORMAT;
        }

        /*
         * セグメントが APP5 の場合
         */
        else if(ignoreApp5Flag == JACIC_FLAG_ON && seg == EXIF_MARKER_APP05)
        {
            /* APP5 セグメントが既に存在するのでエラーを返す */
            return APP5_ALREADY_EXISTS;
        }

        /*
         * セグメントが SOF0 ～ RST7 、または DQT ～ COM の場合
         */
        else if(((seg >= EXIF_MARKER_SOF00) && (seg <= EXIF_MARKER_RST07)) ||
                ((seg >= EXIF_MARKER_DQT)   && (seg <= EXIF_MARKER_COM)))
        {
            /* 走査位置をセグメント ID の後ろへ移動 */
            seek += BYTE_SIZE_UNSIGNED_SHORT;

            /* セグメントのサイズを取得 */
            memcpy(&len, &(src->_buff[seek]), BYTE_SIZE_UNSIGNED_SHORT);

            /* リトルエンディアンの場合は反転させてビッグエンディアンにする */
            if(getEndian() == JACIC_LITTLE_ENDIAN)
            {
                len = swapEndian16(len);
            }

            /* 走査位置を移動 */
            seek += len;
        }

        /*
         * その他、判別不可能なセグメントの場合
         */
        else
        {
            /* 「 Exif フォーマットが不正」としてエラーを返す */
            return INCORRECT_EXIF_FORMAT;
        }

        /* 走査位置がデータ長を超える場合 */
        if(src->_len < seek)
        {
            return INCORRECT_EXIF_FORMAT;
        }

    }

    /* EOI の走査を行う */
    seg = seekToEOI(src, &seek);

    if(seg != EXIF_MARKER_EOI)
    {
        /* SOI の次が EOI ではない場合「フォーマットが不正」としてエラーを返す */
        return INCORRECT_EXIF_FORMAT;
    }

    /* ここまでをハッシュ値の計算に用いる為に走査位置を返す */
    *retEndIndex = seek;

    return FUNCTION_SUCCESS;
}

/*!
@brief ハッシュ値計算に必要となる『画像の圧縮データのバイナリ』を取得する。
@details 対象となる範囲は、SOS ～ EOI の間（ SOS と EOI 自身を含む）。 EOI 移行の冗長なデータは無効なものとして無視する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param [out] **retImgBin 画像から抽出した『圧縮データ開始位置以降』のバイト配列のポインタ
@param ignoreApp5Flag FLAG_ON のとき、 APP5 領域を発見した際にエラーを返す
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER 不正な引数が指定された場合
@retval INCORRECT_EXIF_FORMAT Exif ファイルが不正な形式の場合
@retval APP5_ALREADY_EXISTS ignoreApp5Flag が真であり、APP5 領域が既に存在する場合
@retval OTHER_ERROR メモリの確保に失敗した場合
*/
int clipCompressedImage(JpegBuffer *src, HashBuffer **retImgBin, int ignoreApp5Flag)
{
    int ret;
    unsigned long startIndex = 0UL;
    unsigned long endIndex = 0UL;
    size_t dataSize = 0;
    size_t memorySize = 0;

    if(*retImgBin != NULL)
    {
        /* 初期化されていない場合初期化する */
        free(*retImgBin);
        *retImgBin = NULL;
    }

    /*
     * ハッシュ値算出に必要なデータ開始位置を取得する
     * SOSセグメント以降を利用する。
     */
    ret = getHashSourceData(src, &startIndex, &endIndex, ignoreApp5Flag);

    if(ret != FUNCTION_SUCCESS)
    {
        return ret;
    }

    /* ハッシュ計算に利用する圧縮データ部分の取得 */
    dataSize = endIndex - startIndex;
    memorySize = sizeof(HashBuffer) + dataSize;
    *retImgBin = (HashBuffer *) malloc(memorySize);

    if(*retImgBin == NULL)
    {
        /* メモリ確保失敗 */
        return OTHER_ERROR;
    }

    /* 領域のゼロクリア */
    memset(*retImgBin, 0x00, memorySize);

    (*retImgBin)->_len = dataSize;

    memcpy((*retImgBin)->_buff, &(src->_buff[startIndex]), dataSize);

    return ret;
}
