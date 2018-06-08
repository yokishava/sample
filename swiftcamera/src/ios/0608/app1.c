/**
 * @file app1.c
 *
 * @date Created on: 2016/01/18
 * @author Author: DATT JAPAN Inc.
 * @version 1.1
 * @brief Exif APP1領域解析用ソースコード
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "app1.h"
#include "common.h"
#include "exif.h"

/*!
@name Exif エントリ関連定義
@{
*/
typedef unsigned short EntryTag_t;         /*!< @brief エントリ『タグ識別番号』の型 */
typedef unsigned short EntryType_t;        /*!< @brief エントリ『値の型情報』の型 */
typedef unsigned int   EntryCount_t;       /*!< @brief エントリ『値の個数』の型 */
typedef unsigned int   EntryValueOffset_t; /*!< @brief エントリ『値または値へのオフセット』の型 */

/*!
@struct Entry
@brief Exif エントリ情報を格納する構造体
*/
typedef struct
{
    EntryTag_t         tag;         /*!< @brief 0 -  1 バイト：タグ識別番号 */
    EntryType_t        type;        /*!< @brief 2 -  3 バイト：値の型情報 */
    EntryCount_t       count;       /*!< @brief 4 -  7 バイト：値の個数 */
    EntryValueOffset_t valueOffset; /*!< @brief 8 - 11 バイト：値または値へのオフセット */
} Entry;

/*!
@}

@brief IFD タイプ
*/
enum
{
    IFD_TYPE_BYTE      =  1, /*< 8 ビット符号無し整数 */
    IFD_TYPE_ASCII     =  2, /*< NULL 終端の 8 ビットバイト */
    IFD_TYPE_SHORT     =  3, /*< 2 バイト符号なし整数 */
    IFD_TYPE_LONG      =  4, /*< 4 バイト符号なし整数 */
    IFD_TYPE_RATIONAL  =  5, /*< LONG 値 2 個による分数 */
    IFD_TYPE_UNDEFINED =  7, /*< どんな値をとってもよい 8 ビットバイト */
    IFD_TYPE_SLONG     =  9, /*< 4 バイト符号つき整数（ 2 の補数） */
    IFD_TYPE_SRATIONAL = 10  /*< SLONG 値 2 個による分数 */
};

/*!
@name A. バージョンに関するタグ
@{
*/
const unsigned short EXIF_TAG_EXIF_VERSION                = 0x9000; /*!< @brief Exifタグ：Exifバージョン */
const unsigned short EXIF_TAG_FLASHPIX_VERSION            = 0xA000; /*!< @brief Exifタグ：対応Flashpixバージョン */
/*!
@}

@name B. 画像データの特性に関するタグ
@{
*/
const unsigned short EXIF_TAG_COLOR_SPACE                 = 0xA001; /*!< @brief Exifタグ：色空間情報 */
const unsigned short EXIF_TAG_GAMMA                       = 0xA500; /*!< @brief Exifタグ：再生ガンマ */
/*!
@}

@name C. 構造に関するタグ
@{
*/
const unsigned short EXIF_TAG_COMPONENTS_CONFIGURATION    = 0x9101; /*!< @brief Exifタグ：各コンポーネントの意味 */
const unsigned short EXIF_TAG_COMPRESSED_BITS_PER_PIXEL   = 0x9102; /*!< @brief Exifタグ：画像圧縮モード */
const unsigned short EXIF_TAG_PIXEL_X_DIMENSION           = 0xA002; /*!< @brief Exifタグ：実効画像幅 */
const unsigned short EXIF_TAG_PIXEL_Y_DIMENSION           = 0xA003; /*!< @brief Exifタグ：実効画像高さ */
/*!
@}

@name D. ユーザ情報に関するタグ
@{
*/
const unsigned short EXIF_TAG_MAKER_NOTE                  = 0x927C; /*!< @brief Exifタグ：メーカノート */
const unsigned short EXIF_TAG_USER_COMMENT                = 0x9286; /*!< @brief Exifタグ：ユーザコメント */
/*!
@}

@name E. 関連ファイル情報に関するタグ
@{
*/
const unsigned short EXIF_TAG_RELATED_SOUND_FILE          = 0xA004; /*!< @brief Exifタグ：関連音声ファイル */
/*!
@}

@name F. 日時に関するタグ
@{
*/
const unsigned short EXIF_TAG_DATE_TIME_ORIGINAL          = 0x9003; /*!< @brief Exifタグ：原画像データの生成日時 */
const unsigned short EXIF_TAG_DATE_TIME_DIGITIZED         = 0x9004; /*!< @brief Exifタグ：ディジタルデジタルデータの作成日時 */
const unsigned short EXIF_TAG_SUB_SEC_TIME                = 0x9290; /*!< @brief Exifタグ：DateTimeのサブセック */
const unsigned short EXIF_TAG_SUB_SEC_TIME_ORIGINAL       = 0x9291; /*!< @brief Exifタグ：DateTimeOriginalのサブセック */
const unsigned short EXIF_TAG_SUB_SEC_TIME_DIGITIZED      = 0x9292; /*!< @brief Exifタグ：DateTimeDigitizedのサブセック */
/*!
@}

@name G. 撮影条件に関するタグ
@{
*/
const unsigned short EXIF_TAG_EXPOSURE_TIME               = 0x829A; /*!< @brief Exifタグ：露出時間 */
const unsigned short EXIF_TAG_F_NUMBER                    = 0x829D; /*!< @brief Exifタグ：Fナンバー */
const unsigned short EXIF_TAG_EXPOSURE_PROGRAM            = 0x8822; /*!< @brief Exifタグ：露出プログラム */
const unsigned short EXIF_TAG_SPECTRAL_SENSITIVITY        = 0x8824; /*!< @brief Exifタグ：スペクトル感度 */
const unsigned short EXIF_TAG_PHOTOGRAPHIC_SENSITIVITY    = 0x8827; /*!< @brief Exifタグ：撮影感度 */
const unsigned short EXIF_TAG_OECF                        = 0x8828; /*!< @brief Exifタグ：光電変換関数 */
const unsigned short EXIF_TAG_SENSITIVITY_TYPE            = 0x8830; /*!< @brief Exifタグ：感度種別 */
const unsigned short EXIF_TAG_STANDARD_OUTPUT_SENSITIVITY = 0x8831; /*!< @brief Exifタグ：標準出力感度 */
const unsigned short EXIF_TAG_RECOMMENDED_EXPOSURE_INDEX  = 0x8832; /*!< @brief Exifタグ：推奨露光指数 */
const unsigned short EXIF_TAG_ISO_SPEED                   = 0x8833; /*!< @brief Exifタグ：ISOスピード */
const unsigned short EXIF_TAG_ISO_SPEED_LATITUDE_YYY      = 0x8834; /*!< @brief Exifタグ：ISOスピードラチチュードyyy */
const unsigned short EXIF_TAG_ISO_SPEED_LATITUDE_ZZZ      = 0x8835; /*!< @brief Exifタグ：ISOスピードラチチュードzzz */
const unsigned short EXIF_TAG_SHUTTER_SPEED_VALUE         = 0x9201; /*!< @brief Exifタグ：シャッタースピード */
const unsigned short EXIF_TAG_APERTURE_VALUE              = 0x9202; /*!< @brief Exifタグ：絞り値 */
const unsigned short EXIF_TAG_BRIGHTNESS_VALUE            = 0x9203; /*!< @brief Exifタグ：輝度値 */
const unsigned short EXIF_TAG_EXPOSURE_BIAS_BALUE         = 0x9204; /*!< @brief Exifタグ：露光補正値 */
const unsigned short EXIF_TAG_MAX_APERTURE_VALUE          = 0x9205; /*!< @brief Exifタグ：レンズ最小F値 */
const unsigned short EXIF_TAG_SUBJECT_DISTANCE            = 0x9206; /*!< @brief Exifタグ：被写体距離 */
const unsigned short EXIF_TAG_METERING_MODE               = 0x9207; /*!< @brief Exifタグ：測光方式 */
const unsigned short EXIF_TAG_LIGHT_SOURCE                = 0x9208; /*!< @brief Exifタグ：光源 */
const unsigned short EXIF_TAG_FLASH                       = 0x9209; /*!< @brief Exifタグ：フラッシュ */
const unsigned short EXIF_TAG_FOCAL_LENGTH                = 0x920A; /*!< @brief Exifタグ：レンズ焦点距離 */
const unsigned short EXIF_TAG_SUBJECT_AREA                = 0x9214; /*!< @brief Exifタグ：被写体領域 */
const unsigned short EXIF_TAG_FLASH_ENERGY                = 0xA20B; /*!< @brief Exifタグ：フラッシュ強度 */
const unsigned short EXIF_TAG_SPATIAL_FREQUENCY_RESPONSE  = 0xA20C; /*!< @brief Exifタグ：空間周波数応答 */
const unsigned short EXIF_TAG_FOCAL_PLANE_X_RESOLUTION    = 0xA20E; /*!< @brief Exifタグ：焦点面の幅の解像度 */
const unsigned short EXIF_TAG_FOCAL_PLANE_Y_RESOLUTION    = 0xA20F; /*!< @brief Exifタグ：焦点面の高さの解像度 */
const unsigned short EXIF_TAG_FOCAL_PLANE_RESOLUTION_UNIT = 0xA210; /*!< @brief Exifタグ：焦点面解像度単位 */
const unsigned short EXIF_TAG_SUBJECT_LOCATION            = 0xA214; /*!< @brief Exifタグ：被写体位置 */
const unsigned short EXIF_TAG_EXPOSURE_INDEX              = 0xA215; /*!< @brief Exifタグ：露出インデックス */
const unsigned short EXIF_TAG_SENSING_METHOD              = 0xA217; /*!< @brief Exifタグ：センサ方式 */
const unsigned short EXIF_TAG_FILE_SOURCE                 = 0xA300; /*!< @brief Exifタグ：ファイルソース */
const unsigned short EXIF_TAG_SCENE_TYPE                  = 0xA301; /*!< @brief Exifタグ：シーンタイプ */
const unsigned short EXIF_TAG_CFA_PATTERN                 = 0xA302; /*!< @brief Exifタグ：CFAパターン */
const unsigned short EXIF_TAG_CUSTOM_RENDERED             = 0xA401; /*!< @brief Exifタグ：個別画像処理 */
const unsigned short EXIF_TAG_EXPOSURE_MODE               = 0xA402; /*!< @brief Exifタグ：露出モード */
const unsigned short EXIF_TAG_WHITE_BALANCE               = 0xA403; /*!< @brief Exifタグ：ホワイトバランス */
const unsigned short EXIF_TAG_DIGITAL_ZOOM_RATIO          = 0xA404; /*!< @brief Exifタグ：デジタルズーム倍率 */
const unsigned short EXIF_TAG_FOCAL_COUNT_IN_35mm_FILM    = 0xA405; /*!< @brief Exifタグ：35mm換算レンズ焦点距離 */
const unsigned short EXIF_TAG_SCENE_CAPTURE_TYPE          = 0xA406; /*!< @brief Exifタグ：撮影シーンタイプ */
const unsigned short EXIF_TAG_GAIN_CONTROL                = 0xA407; /*!< @brief Exifタグ：ゲイン制御 */
const unsigned short EXIF_TAG_CONTRAST                    = 0xA408; /*!< @brief Exifタグ：撮影コントラスト */
const unsigned short EXIF_TAG_SATURATION                  = 0xA409; /*!< @brief Exifタグ：撮影彩度 */
const unsigned short EXIF_TAG_SHARPNESS                   = 0xA40A; /*!< @brief Exifタグ：撮影シャープネス */
const unsigned short EXIF_TAG_DEVICE_SETTING_DESCRIPTION  = 0xA40B; /*!< @brief Exifタグ：撮影条件記述情報 */
const unsigned short EXIF_TAG_SUBJECT_DISTANCE_RANGE      = 0xA40C; /*!< @brief Exifタグ：被写体距離レンジ */
/*!
@}

@name H. その他のタグ
@{
*/
const unsigned short EXIF_TAG_IMAGE_UNIQUE_ID             = 0xA420; /*!< @brief Exifタグ：画像ユニークID */
const unsigned short EXIF_TAG_CAMERA_OWNER_NAME           = 0xA430; /*!< @brief Exifタグ：カメラ所有者番号 */
const unsigned short EXIF_TAG_BODY_SERIAL_NUMBER          = 0xA431; /*!< @brief Exifタグ：カメラシリアル番号 */
const unsigned short EXIF_TAG_LENS_SPECIFICATION          = 0xA432; /*!< @brief Exifタグ：レンズの仕様情報 */
const unsigned short EXIF_TAG_LENS_MAKE                   = 0xA433; /*!< @brief Exifタグ：レンズのメーカ名 */
const unsigned short EXIF_TAG_LENS_MODEL                  = 0xA434; /*!< @brief Exifタグ：レンズのモデル名 */
const unsigned short EXIF_TAG_LENS_SERIAL_NUMBER          = 0xA435; /*!< @brief Exifタグ：レンズシリアル番号 */
/*! @} */

static const unsigned short TIFF_HEADER_SEPARATOR    = 0x002A; /*!< @brief TIFF ヘッダ固有の区切り文字 */
static const unsigned short IFD_TAG_EXIF_IFD_POINTER = 0x8769; /*!< @brief Exif タグ集合 IFD へのタグ番号 */

static const size_t BYTE_SIZE_EXIF_IDENTIFIER_SIZE   =      6; /*!< @brief APP1 領域 EXIF 識別子のバイト数 */
static const size_t BYTE_SIZE_TIFF_HEADER_SEPARATOR  =      2; /*!< @brief TIFF ヘッダ固有の区切り文字のバイト数 */
static const size_t BYTE_SIZE_TIFF_HEADER_OFFSET     =      4; /*!< @brief TIFF ヘッダオフセット定義バイト数 */
static const size_t BYTE_SIZE_IFD_COUNT_SIZE         =      2; /*!< @brief IFD エントリ数定義のバイト数 */

static const size_t BYTE_SIZE_DATE_TIME_ORIGINAL     =     20; /*!< @brief 『原画像データの生成日時』のバイト数（規定） */

/*!
@brief APP1 識別名 「Exif」（終端文字を含む 6 バイトの配列）
*/
static const char IDENTIFIER_EXIF[] =
{
    0x45, 0x78, 0x69, 0x66, 0x00, 0x00
};

/*!
@brief エントリの型情報を受け取り、型情報に応じた 1 カウントごとのデータ長を取得する。
@param type エントリの型情報
@return 型情報から取得したデータ長。型情報が未定義のものの場合は 0 を返す。
*/
static size_t getEntryTypeSize(unsigned short type)
{
    switch(type)
    {
    case IFD_TYPE_BYTE:
        return BYTE_SIZE_UNSIGNED_CHAR;

    case IFD_TYPE_ASCII:
        return BYTE_SIZE_UNSIGNED_CHAR;

    case IFD_TYPE_SHORT:
        return BYTE_SIZE_UNSIGNED_SHORT;

    case IFD_TYPE_LONG:
        return BYTE_SIZE_UNSIGNED_INT;

    case IFD_TYPE_UNDEFINED:
        return BYTE_SIZE_UNSIGNED_CHAR;

    case IFD_TYPE_SLONG:
        return BYTE_SIZE_UNSIGNED_INT;

    case IFD_TYPE_SRATIONAL:
        return BYTE_SIZE_UNSIGNED_INT * 2;

    default:
        return 0;
    }
}

/*!
@brief Exif タグのバイトサイズ総数を取得する。
@param サイズ取得用の Exif タグ構造体
*/
static size_t getByteSizeOfEntry(Entry entry)
{
    size_t ret = 0;

    ret += sizeof(entry.tag);
    ret += sizeof(entry.type);
    ret += sizeof(entry.count);
    ret += sizeof(entry.valueOffset);

    return ret;
}

/*!
@brief 指定した走査位置より、Tiff ヘッダ固有の区切り文字として 2 バイト取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param seek 指定位置
@param jacicEndian エンディアン情報（ common.h の JACIC 定数を使用）
@return 取得した区切り文字（見つからなかった場合は 0x0000 ）
*/
static unsigned short getTiffHeaderSeparator(JpegBuffer *src, unsigned long seek, int jacicEndian)
{
    unsigned short tag = 0x0000U;

    /* パラメータチェック */
    if(src == NULL)
    {
        return tag;
    }

    if(seek + BYTE_SIZE_TIFF_HEADER_SEPARATOR <= src->_len)
    {
        memcpy(&tag, &(src->_buff[seek]), BYTE_SIZE_TIFF_HEADER_SEPARATOR);

        /* エンディアンが等しくない場合は反転させる */
        if(getEndian() != jacicEndian)
        {
            tag = swapEndian16(tag);
        }
    }

    return tag;
}

/*!
@brief 指定した走査位置より、Tiff ヘッダから実データへのオフセットとして 4 バイト取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param seek 指定位置
@param jacicEndian エンディアン情報（ common.h の JACIC 定数を使用）
@return 取得した区切り文字（見つからなかった場合は 0x00000000 ）
*/
static unsigned int getTiffHeaderOffset(JpegBuffer *src, unsigned long seek, int jacicEndian)
{
    unsigned int offset = 0U;

    /* パラメータチェック */
    if(src == NULL)
    {
        return offset;
    }

    if(seek + BYTE_SIZE_TIFF_HEADER_OFFSET <= src->_len)
    {
        memcpy(&offset, &(src->_buff[seek]), BYTE_SIZE_TIFF_HEADER_OFFSET);

        /* エンディアンが等しくない場合は反転させる */
        if(getEndian() != jacicEndian)
        {
            offset = swapEndian32(offset);
        }
    }

    return offset;
}

/*!
@brief 指定した走査位置より、IFD のエントリ数カウントとして 2 バイト取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param seek 指定位置
@param jacicEndian エンディアン情報（ common.h の JACIC 定数を使用）
@return 取得した区切り文字（見つからなかった場合は 0x0000 ）
*/
static unsigned short getIFDCount(JpegBuffer *src, unsigned long seek, int jacicEndian)
{
    unsigned short count = 0x0000U;

    /* パラメータチェック */
    if(src == NULL)
    {
        return count;
    }

    if(seek + BYTE_SIZE_IFD_COUNT_SIZE <= src->_len)
    {
        memcpy(&count, &(src->_buff[seek]), BYTE_SIZE_IFD_COUNT_SIZE);

        /* エンディアンが等しくない場合は反転させる */
        if(getEndian() != jacicEndian)
        {
            count = swapEndian16(count);
        }
    }

    return count;
}

/*!
@brief 指定した走査位置より、エントリタグとして 2 バイト取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param seek 指定位置
@param jacicEndian エンディアン情報（ common.h の JACIC 定数を使用）
@return 取得した Exif タグ（見つからなかった場合は 0x0000 ）
*/
static EntryTag_t getEntryTag(JpegBuffer *src, unsigned long seek, int jacicEndian)
{
    EntryTag_t tag = 0x0000U;
    size_t readSize = sizeof(tag);

    /* パラメータチェック */
    if(src == NULL)
    {
        return tag;
    }

    if(seek + readSize <= src->_len)
    {
        memcpy(&tag, &(src->_buff[seek]), readSize);

        /* エンディアンが等しくない場合は反転させる */
        if(getEndian() != jacicEndian)
        {
            tag = swapEndian16(tag);
        }
    }

    return tag;
}

/*!
@brief 指定した走査位置より、エントリの型情報として 2 バイト取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param seek 指定位置
@param jacicEndian エンディアン情報（ common.h の JACIC 定数を使用）
@return 取得した型情報（見つからなかった場合は 0x0000 ）
*/
static EntryType_t getEntryType(JpegBuffer *src, unsigned long seek, int jacicEndian)
{
    EntryType_t type = 0x0000U;
    size_t readSize = sizeof(type);

    /* パラメータチェック */
    if(src == NULL)
    {
        return type;
    }

    if(seek + readSize <= src->_len)
    {
        memcpy(&type, &(src->_buff[seek]), readSize);

        /* エンディアンが等しくない場合は反転させる */
        if(getEndian() != jacicEndian)
        {
            type = swapEndian16(type);
        }
    }

    return type;
}

/*!
@brief 指定した走査位置より、エントリの個数として 4 バイト取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param seek 指定位置
@param jacicEndian エンディアン情報（ common.h の JACIC 定数を使用）
@return 取得した値の個数（見つからなかった場合は 0x00000000 ）
*/
static EntryCount_t getEntryCount(JpegBuffer *src, unsigned long seek, int jacicEndian)
{
    EntryCount_t count = 0U;
    size_t readSize = sizeof(count);

    /* パラメータチェック */
    if(src == NULL)
    {
        return count;
    }

    if(seek + readSize <= src->_len)
    {
        memcpy(&count, &(src->_buff[seek]), readSize);

        /* エンディアンが等しくない場合は反転させる */
        if(getEndian() != jacicEndian)
        {
            count = swapEndian32(count);
        }
    }

    return count;
}

/*!
@brief 指定した走査位置より、値へのオフセットとして 4 バイト取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param seek 指定位置
@param jacicEndian エンディアン情報（ common.h の JACIC 定数を使用）
@return 取得した値へのオフセット（見つからなかった場合は 0x00000000 ）
*/
static EntryValueOffset_t getEntryValueOffset(JpegBuffer *src, unsigned long seek, int jacicEndian)
{
    EntryValueOffset_t valueOffset = 0U;
    size_t readSize = sizeof(valueOffset);

    /* パラメータチェック */
    if(src == NULL)
    {
        return valueOffset;
    }

    if(seek + readSize <= src->_len)
    {
        memcpy(&valueOffset, &(src->_buff[seek]), readSize);

        /* エンディアンが等しくない場合は反転させる */
        if(getEndian() != jacicEndian)
        {
            valueOffset = swapEndian32(valueOffset);
        }
    }

    return valueOffset;
}

/*!
@brief 指定した走査位置より、Entry として 12 バイト取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param seek 指定位置
@param tiffHeaderStartPosition TIFF ヘッダ開始位置
@param jacicEndian エンディアン情報（ common.h の JACIC 定数を使用）
@return 取得したエントリ。処理が正常に終了しなかった場合はすべての値が 0 。
*/
static Entry getEntry(JpegBuffer *src, unsigned long seek, size_t tiffHeaderStartPosition, int jacicEndian)
{
    Entry entry = {0, 0, 0, 0};
    size_t offsetSize = sizeof(entry.valueOffset);

    /* パラメータチェック */
    if(src == NULL)
    {
        return entry;
    }

    if(src->_len < seek + getByteSizeOfEntry(entry))
    {
        return entry;
    }

    /* 各要素の取得 */
    entry.tag = getEntryTag(src, seek, jacicEndian);
    seek += sizeof(entry.tag);

    entry.type = getEntryType(src, seek, jacicEndian);
    seek += sizeof(entry.type);

    entry.count = getEntryCount(src, seek, jacicEndian);
    seek += sizeof(entry.count);

    if((entry.tag == EXIF_TAG_SUB_SEC_TIME ||
            entry.tag == EXIF_TAG_SUB_SEC_TIME_DIGITIZED ||
            entry.tag == EXIF_TAG_SUB_SEC_TIME_ORIGINAL) &&
            entry.count <= offsetSize)
    {
        /*
         * サブセック値は終端の NULL を含めて
         * オフセットポインタを保存するフィールドに収まる場合、
         * オフセットポインタの代わりに実際の値を記述する。
         * Entry 構造体を利用するコードからはどちらの場合も同じコードで利用できるように、
         * ここではオフセットポインタの先頭を指すようにする。
         */
        entry.valueOffset = (unsigned int)(seek - tiffHeaderStartPosition);
    }
    else
    {
        /*
         * 終端の NULL を含めて 5 バイト以上の場合、他の値と同様、
         * オフセットフィールドに値を記述する。
         */
        entry.valueOffset = getEntryValueOffset(src, seek, jacicEndian);
    }

    return entry;
}

/*!
@brief 原画像データの生成日時が取得できなかった場合に利用するダミーを作成する。
@param [out] **dest ダミーデータのセット対象
@retval FUNCTION_SUCCESS 正常終了
@retval OTHER_ERROR メモリ確保に失敗した場合
*/
static int createDummyDateTimeOrigin(HashBuffer **dest)
{
    size_t memorySize = sizeof(HashBuffer *) + (BYTE_SIZE_UNSIGNED_CHAR * BYTE_SIZE_DATE_TIME_ORIGINAL);

    /* メモリの確保 */
    *dest = (HashBuffer *) malloc(memorySize);

    if(*dest == NULL)
    {
        /* メモリ確保に失敗 */
        return OTHER_ERROR;
    }

    /* 領域のゼロクリア */
    memset(*dest, 0x00, memorySize);

    /* バイト長の設定 */
    (*dest)->_len = BYTE_SIZE_DATE_TIME_ORIGINAL;

    /* _buff に対し、最後の 1 バイトを除き、全て空白 (0x20) で埋める */
    memset((*dest)->_buff, 0x20, (*dest)->_len - 1);

    return FUNCTION_SUCCESS;
}

/*!
@brief APP1 セグメント内のデータ形式が Exif であるかを確認する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param [out] *startIndex 走査開始インデックス
@param size 読み込み最大サイズ
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER 不正な引数が指定された場合
@retval INCORRECT_APP1_FORMAT APP1 領域が異なる形式の場合
@retval OTHER_ERROR メモリ確保に失敗した場合
*/
static int checkIdentifierIsExif(JpegBuffer *src, unsigned long *startIndex)
{
    /* 識別子が正しいか調べる */
    int ret = checkIdentifier(IDENTIFIER_EXIF, src, startIndex, BYTE_SIZE_EXIF_IDENTIFIER_SIZE);

    if(ret == INCORRECT_IDENTIFIER)
    {
        /* 識別子が違う場合、フォーマットが違う */
        return INCORRECT_APP1_FORMAT;
    }

    return ret;
}

/*!
@brief Exif ファイルに格納されている APP1 領域の存在確認とその開始位置を取得する。
@param [in] *src 対象となる JPEG 画像のバイナリ読み込み結果データ構造体
@param [in, out] *seek 取得する走査開始位置
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER 不正な引数が指定された場合
@retval INCORRECT_EXIF_FORMAT Exif ファイルが不正な形式の場合
@retval APP1_NOT_EXISTS APP1 領域が走査位置より下に存在しないとされる場合
*/
int checkApp1Exists(JpegBuffer *src, unsigned long *seek)
{
    int ret = FUNCTION_SUCCESS;
    unsigned short len = 0x0000U;

    /* パラメータチェック */
    if(src == NULL)
    {
        return INCORRECT_PARAMETER;
    }

    if(seek == NULL)
    {
        return INCORRECT_PARAMETER;
    }

    /* 走査位置がデータ長を超える場合 */
    if(src->_len < *seek)
    {
        return INCORRECT_PARAMETER;
    }

    while(1)
    {
        /* マーカセグメントの取得 */
        int seg = getMarkerSegment(src, seek);

        /*
         * セグメントが SOI の場合
         */
        if(seg == EXIF_MARKER_SOI)
        {
            /* 走査位置をセグメントIDの後ろへ移動 */
            *seek += BYTE_SIZE_SEGMENT_MARKER;
        }

        /*
         * SOF、DHT、SOS、DQT、DRIのいずれかの場合
         */
        else if(seg == EXIF_MARKER_SOF00 ||
                seg == EXIF_MARKER_DHT   ||
                seg == EXIF_MARKER_SOS   ||
                seg == EXIF_MARKER_DQT   ||
                seg == EXIF_MARKER_DRI)
        {
            /* APP5領域よりも下に来ているのでエラーを返す */
            ret = APP1_NOT_EXISTS;
            break;
        }

        /*
         * セグメントがEOIの場合
         */
        else if(seg == EXIF_MARKER_EOI)
        {
            /*
             * SOSよりも前にEOIが見つかったので、
             * 「Exif ファイルフォーマットが不正」としてエラーを返す
             */
            ret = INCORRECT_EXIF_FORMAT;
            break;
        }

        /*
         * セグメントが APP1 の場合
         */
        else if(seg == EXIF_MARKER_APP01)
        {
            /* ループを抜ける */
            break;
        }

        /*
         * 上記以外のセグメントの場合
         */
        else if(((seg >= EXIF_MARKER_SOF01) && (seg <= EXIF_MARKER_SOF03)) ||
                ((seg >= EXIF_MARKER_SOF05) && (seg <= EXIF_MARKER_RST07)) ||
                ((seg >= EXIF_MARKER_DHP)   && (seg <= EXIF_MARKER_COM))   || (seg == EXIF_MARKER_DNL))
        {
            /* 走査位置をセグメントIDの後ろへ移動 */
            *seek += BYTE_SIZE_UNSIGNED_SHORT;

            /* セグメントのサイズを取得 */
            len = getAppSize(src, *seek);

            /* 走査位置を移動 */
            *seek += len;
        }

        /*
         * その他、判別不可能なセグメントの場合
         */
        else
        {
            /* 「Exif ファイルフォーマットが不正」としてエラーを返す */
            ret = INCORRECT_EXIF_FORMAT;
            break;
        }

        /* 走査位置がデータ長を超える場合 */
        if(src->_len < *seek)
        {
            /* 「Exif ファイルフォーマットが不正」としてエラーを返す */
            ret = INCORRECT_EXIF_FORMAT;
            break;
        }
    }

    return ret;
}

/*!
@brief 原画像データの生成日時を Exif データから取得する。
@param [in] *src 対象となる JPEG 画像のバイナリ読み込み結果データ構造体
@param [out] **dest 取得した原画像データの生成日時
@param entry 対象データのエントリ構造体
@param startIndex 取得開始位置のインデックス
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_EXIF_FORMAT 原画像データの生成日時フィールドの大きさが 20 より大きい場合
@retval OTHER_ERROR メモリ確保に失敗した場合
*/
static int readDateTimeOriginal(JpegBuffer *src, HashBuffer **dest, Entry entry, unsigned long startIndex)
{
    /* 確保するべきメモリサイズ */
    size_t memorySize = sizeof(HashBuffer *) + (entry.count * getEntryTypeSize(entry.type));

    /*
     * 原画像データの生成日時はサイズ固定なので、
     * 基準より小さい場合は残りをゼロで埋め、
     * 基準より大きい場合はExif ファイルフォーマット不正としてエラーを返す。
     */
    if(BYTE_SIZE_DATE_TIME_ORIGINAL < entry.count)
    {
        return INCORRECT_EXIF_FORMAT;
    }

    /* 領域の確保 */
    *dest = (HashBuffer *) malloc(memorySize);

    if(*dest == NULL)
    {
        /* メモリ確保失敗 */
        return OTHER_ERROR;
    }

    /* 領域のゼロクリア */
    memset(*dest, 0x00, memorySize);

    /* データ取得 */
    memcpy((*dest)->_buff, &(src->_buff[startIndex]), entry.count);

    /* データ長の設定 */
    (*dest)->_len = entry.count;

    return FUNCTION_SUCCESS;
}

/*!
@brief 原画像データの生成日時の秒以下の値を Exif データから取得する。
@param [in] *src 対象となる JPEG 画像のバイナリ読み込み結果データ構造体
@param [out] **dest 取得した原画像データの生成日時
@param entry 対象データのエントリ構造体
@param startIndex 取得開始位置のインデックス
@retval FUNCTION_SUCCESS 正常終了
@retval OTHER_ERROR メモリ確保に失敗した場合
*/
static int readSubSecTimeOriginal(JpegBuffer *src, HashBuffer **dest, Entry entry, unsigned long startIndex)
{
    /* 確保するべきメモリサイズ */
    size_t memorySize = sizeof(HashBuffer *) + (entry.count * getEntryTypeSize(entry.type));

    /* 領域の確保 */
    *dest = (HashBuffer *) malloc(memorySize);

    if(*dest == NULL)
    {
        /* メモリ確保失敗 */
        return OTHER_ERROR;
    }

    /* 領域のゼロクリア */
    memset(*dest, 0x00, memorySize);

    /* データ取得 */
    memcpy((*dest)->_buff, &(src->_buff[startIndex]), entry.count);

    /* データ長の設定 */
    (*dest)->_len = entry.count;

    return FUNCTION_SUCCESS;
}

/*!
@brief APP1 領域を探査し、原画像データの生成日時、およびその秒以下の値を取得する。
@details 日時とサブセックは別々に取得する。
@details この関数を呼び出した時点でポインタが何らかの値を指している場合、その値は更新しない。
@details 取得できなかった場合、NULL ポインタを指す。
@param [in] *src 対象となる JPEG 画像のバイナリ読み込み結果データ構造体
@param [out] **dateTimeOriginalBin 取得した原画像データの生成日時
@param [out] **subSecTimeOriginalBin 取得した原画像データの生成日時
@param [in, out] *seek 取得する走査開始位置。終了した時点で、探査したセグメントの次のアドレスを指す。
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER *src が NULL の場合。
                            *seek が *src->_len より大きい場合。
                            **dateTimeOriginalBin, **subSecTimeOriginalBin の両方が何らかの値を指している場合。
@retval INCORRECT_EXIF_FORMAT Exif ファイルの形式が正しくない場合
@retval INCORRECT_APP1_FORMAT Exif 情報を含む APP1 とは記述形式が異なる場合
@retval DATE_NOT_EXISTS 日時データを取得できなかった場合
@retval OTHER_ERROR メモリ確保に失敗した場合など
*/
static int takeDateTimeFromAPP1(JpegBuffer *src, HashBuffer **dateTimeOriginalBin, HashBuffer **subSecTimeOriginalBin, unsigned long *seek)
{
    int i;
    int ret = FUNCTION_SUCCESS;
    int jacicEndian;
    unsigned int offset;
    unsigned short segment;
    unsigned short app1Length;
    unsigned short byteOrder;
    unsigned short separator;
    unsigned short IFD0Count;
    unsigned short exifIFDCount;
    unsigned long tmpSeek;
    unsigned long tiffHeaderStartPosition;
    Entry entry = {0, 0, 0, 0};

    /* パラメータチェック */
    if(src == NULL)
    {
        return INCORRECT_PARAMETER;
    }

    if(src->_len <= *seek)
    {
        return INCORRECT_PARAMETER;
    }

    if(*dateTimeOriginalBin != NULL &&
            *subSecTimeOriginalBin != NULL)
    {
        /* 両方が値を持っている場合は設定対象がないのでエラーとする */
        return INCORRECT_PARAMETER;
    }

    /*
     * 仮想走査位置に現在の走査位置をコピー
     * 以下、この関数内では仮想走査位置を使用して走査を行う
     */
    tmpSeek = *seek;


    /*
     * APP1 セグメントヘッダ
     */

    /* セグメントマーカを取得 */
    segment = getMarkerSegment(src, &tmpSeek);

    if(segment != EXIF_MARKER_APP01)
    {
        /* APP1 セグメントではない */
        return INCORRECT_APP1_FORMAT;
    }

    /* 仮想走査位置を進める */
    tmpSeek += BYTE_SIZE_SEGMENT_MARKER;

    /* マーカのサイズを取得 */
    app1Length = getAppSize(src, tmpSeek);

    /* 実際の走査位置を次のセグメント開始位置へ進める */
    *seek = tmpSeek + app1Length;

    /* 仮想走査位置を進める */
    tmpSeek += BYTE_SIZE_SEGMENT_SIZE;

    /* Exif 識別コード判定 */
    ret = checkIdentifierIsExif(src, &tmpSeek);

    /* 既定の識別コードでない場合は失敗 */
    if(ret != FUNCTION_SUCCESS)
    {
        return ret;
    }

    /* 仮想走査位置を進める */
    tmpSeek += BYTE_SIZE_EXIF_IDENTIFIER_SIZE;


    /*
     * TIFF ヘッダ
     */

    /* ヘッダの開始位置を記録 */
    tiffHeaderStartPosition = tmpSeek;

    /* バイトオーダーの取得 */
    byteOrder = getByteOrder(src, tmpSeek);

    if(byteOrder == EXIF_BIG_ENDIAN)
    {
        jacicEndian = JACIC_BIG_ENDIAN;
    }
    else
    {
        jacicEndian = JACIC_LITTLE_ENDIAN;
    }

    /* 仮想走査位置を進める */
    tmpSeek += BYTE_SIZE_ENDIAN;

    /* 区切り文字の取得 */
    separator = getTiffHeaderSeparator(src, tmpSeek, jacicEndian);

    /* 区切り文字が標準で定義されているものではない場合エラー */
    if(separator != TIFF_HEADER_SEPARATOR)
    {
        return INCORRECT_APP1_FORMAT;
    }

    /* 仮想走査位置を進める */
    tmpSeek += BYTE_SIZE_TIFF_HEADER_SEPARATOR;

    /* TIFF ヘッダのデータまでのオフセットの取得 */
    offset = getTiffHeaderOffset(src, tmpSeek, jacicEndian);

    /* 仮想走査位置を TIFF ヘッダのオフセット分進める */
    tmpSeek = tiffHeaderStartPosition + offset;


    /*
     * 0th IFD ( 主画像データ部 )
     */

    /* IFD 0 内部のエントリ数を取得 */
    IFD0Count = getIFDCount(src, tmpSeek, jacicEndian);

    /* 仮想走査位置を進める */
    tmpSeek += BYTE_SIZE_IFD_COUNT_SIZE;

    /* IFD 0 走査開始（ ExifIFDPointer を探す ） */
    for(i = 0; i < IFD0Count; ++i)
    {
        /* エントリを取得 */
        entry = getEntry(src, tmpSeek, tiffHeaderStartPosition, jacicEndian);

        if(entry.tag == IFD_TAG_EXIF_IFD_POINTER)
        {
            /*
             * Exif 情報の IFD ポインタの場合、
             * 仮想走査位置を Exif 情報 IFD の開始位置まで移動
             */
            tmpSeek = tiffHeaderStartPosition + entry.valueOffset;
            break;
        }
        else
        {
            /*
             * それ以外の場合、
             * 仮想走査位置をエントリの大きさだけ進める
             */
            tmpSeek += getByteSizeOfEntry(entry);
        }
    }

    /* ExifIFDPointer が見つからなかった */
    if(entry.tag != IFD_TAG_EXIF_IFD_POINTER)
    {
        /* ExifIFDPointer は必須項目なので、存在しない場合はエラーとする */
        return INCORRECT_EXIF_FORMAT;
    }


    /*
     * Exif データの取得
     */

    /* Exif IFD 内部のエントリ数を取得 */
    exifIFDCount = getIFDCount(src, tmpSeek, jacicEndian);

    /* 仮想走査位置を進める */
    tmpSeek += BYTE_SIZE_IFD_COUNT_SIZE;

    /* Exif IFD 走査開始（ 日時データ を探す ） */
    for(i = 0; i < exifIFDCount; ++i)
    {
        /* エントリを取得 */
        entry = getEntry(src, tmpSeek, tiffHeaderStartPosition, jacicEndian);

        if(entry.tag == EXIF_TAG_DATE_TIME_ORIGINAL &&
                *dateTimeOriginalBin == NULL)
        {
            /* 原画像データの撮影日時の取得 */
            ret = readDateTimeOriginal(
                      src,
                      dateTimeOriginalBin,
                      entry,
                      tiffHeaderStartPosition + entry.valueOffset
                  );

            /* 失敗した場合は処理を中止する */
            if(ret != FUNCTION_SUCCESS)
            {
                return ret;
            }

            /* 両方取得できた場合は中止する */
            if(*dateTimeOriginalBin != NULL &&
                    *subSecTimeOriginalBin != NULL)
            {
                break;
            }

        }
        else if(entry.tag == EXIF_TAG_SUB_SEC_TIME_ORIGINAL)
        {
            /*
             * 原画像データの撮影日時（秒以下の値）の取得
             */
            ret = readSubSecTimeOriginal(
                      src,
                      subSecTimeOriginalBin,
                      entry,
                      tiffHeaderStartPosition + entry.valueOffset
                  );

            /* 失敗した場合は処理を中止する */
            if(ret != FUNCTION_SUCCESS)
            {
                return ret;
            }

            /* 両方取得できた場合は中止する */
            if(*dateTimeOriginalBin != NULL &&
                    *subSecTimeOriginalBin != NULL)
            {
                break;
            }
        }
        else
        {
            /*
             * それ以外の場合、
             * 仮想走査位置をエントリのサイズ分進める
             */
            tmpSeek += getByteSizeOfEntry(entry);
        }
    }

    /* DateTimeOriginal が見つからなかった */
    if(dateTimeOriginalBin == NULL)
    {
        return DATE_NOT_EXISTS;
    }

    return ret;
}

/*!
@brief APP1 領域を探査し、原画像データの生成日時、およびその秒以下の値を取得する。
@details 原画像データの生成日時 (DateTimeOriginal) が取得できなかった場合、
@details NULL 終端を含めた長さ 20 の空白 (0x20) 埋めされたダミーデータを生成して返す。
@details 原画像データの生成日時のサブセック (SubSecTimeOriginal) が取得できなかった場合は無視する。
@param [in] *src 対象となる JPEG 画像のバイナリ読み込み結果データ構造体
@param [out] **retDateBin 取得した日時データ
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER いずれかの引数が NULL の場合
@retval INCORRECT_EXIF_FORMAT Exif ファイルが不正の場合
@retval OTHER_ERROR メモリ確保に失敗した場合
 */
int getDateTimeOriginal(JpegBuffer *src, HashBuffer **retDateBin)
{
    int ret;
    unsigned long seek = 0UL;
    HashBuffer *dateTimeOriginalBuffer = NULL;
    HashBuffer *subSecTimeOriginalBuffer = NULL;
    size_t dataSize;
    size_t memorySize;

    /* パラメータチェック */
    if(src == NULL)
    {
        return INCORRECT_PARAMETER;
    }

    if(src->_len <= seek)
    {
        return INCORRECT_PARAMETER;
    }

    /*
     * SOI を検出する（検出できなければ Exif 形式のエラー）
     */
    ret = checkFirstFindSOI(src, &seek);

    if(ret != FUNCTION_SUCCESS)
    {
        /* 不正な Exif フォーマット */
        return INCORRECT_EXIF_FORMAT;
    }

    /* APP1 領域を繰り返し検索する */
    while(seek < src->_len)
    {
        /* APP1 領域を検出する */
        ret = checkApp1Exists(src, &seek);

        if(ret == FUNCTION_SUCCESS)
        {
            /* 正常に取得できたので、取得内容から原画像データの生成日時を探す */
            ret = takeDateTimeFromAPP1(
                      src,
                      &dateTimeOriginalBuffer,
                      &subSecTimeOriginalBuffer,
                      &seek
                  );

            /*
             * エラーが発生した場合は終了する
             * ただし、日時が見つからない場合は次の APP1 領域を探すため、終了しない
             */
            if(ret != FUNCTION_SUCCESS &&
                    ret != INCORRECT_EXIF_FORMAT &&
                    ret != DATE_NOT_EXISTS &&
                    ret != INCORRECT_APP1_FORMAT)
            {
                goto FINALIZE;
            }

            /* 両方のデータが取得できていれば終了する */
            if(dateTimeOriginalBuffer != NULL &&
                    subSecTimeOriginalBuffer != NULL)
            {
                break;
            }
        }
        else
        {
            /**
             * APP1領域を全て検索し終えた場合
             */
            if(ret == APP1_NOT_EXISTS)
            {
                if(dateTimeOriginalBuffer == NULL)
                {
                    /*
                     * 最後まで原画像データの生成日時を取得できなかった場合は、
                     * 空のダミーデータを作成する。
                     */
                    ret = createDummyDateTimeOrigin(&dateTimeOriginalBuffer);

                    if(ret != FUNCTION_SUCCESS)
                    {
                        goto FINALIZE;
                    }
                }
                else
                {
                    /* 少なくとも原画像データの生成日時が取得できているので、正常終了扱いとする */
                    ret = FUNCTION_SUCCESS;
                }
            }

            break;
        }
    }

    /* 日時データと秒以下の値を結合する */

    dataSize = dateTimeOriginalBuffer->_len;

    if(subSecTimeOriginalBuffer != NULL &&
            0 < subSecTimeOriginalBuffer->_len)
    {
        dataSize += subSecTimeOriginalBuffer->_len;
    }

    memorySize = sizeof(HashBuffer *) + dataSize;

    *retDateBin = (HashBuffer *) malloc(memorySize);

    if(*retDateBin == NULL)
    {
        /* メモリ確保失敗 */
        ret = OTHER_ERROR;
        goto FINALIZE;
    }

    /* 領域のゼロクリア */
    memset(*retDateBin, 0x00, memorySize);

    /* データ長の設定 */
    (*retDateBin)->_len = dataSize;

    memcpy((*retDateBin)->_buff, dateTimeOriginalBuffer->_buff, dateTimeOriginalBuffer->_len);

    /* 秒以下の値が存在する場合は末尾に追加 */
    if(subSecTimeOriginalBuffer != NULL &&
            0 < subSecTimeOriginalBuffer->_len)
    {
        memcpy(&(*retDateBin)->_buff[dateTimeOriginalBuffer->_len], &subSecTimeOriginalBuffer->_buff, subSecTimeOriginalBuffer->_len);
    }

FINALIZE:

    /* メモリ解放 */
    if(dateTimeOriginalBuffer != NULL)
    {
        free(dateTimeOriginalBuffer);
        dateTimeOriginalBuffer = NULL;
    }

    if(subSecTimeOriginalBuffer != NULL)
    {
        free(subSecTimeOriginalBuffer);
        subSecTimeOriginalBuffer = NULL;
    }

    return ret;
}
