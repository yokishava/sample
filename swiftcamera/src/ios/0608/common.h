/*!
@file common.h
@date Created on: 2015/08/18
@author DATT JAPAN Inc.
@version 1.1
@brief 共通処理用ヘッダ
*/

#ifndef COMMON_H_
#define COMMON_H_

#include <stddef.h>

/*!
@name フラグ定義
@{
*/
#ifndef JACIC_FLAGS_

#define JACIC_FLAGS_

#define JACIC_FLAG_ON  ((int)1) /*!< @brief フラグ ON  */
#define JACIC_FLAG_OFF ((int)0) /*!< @brief フラグ OFF */

#endif /* JACIC_FLAGS_ */
/*! @} */

/*!
@name エンディアンフラグ定義
@{
*/
#ifndef JACIC_ENDIANS_

#define JACIC_ENDIANS_

#define JACIC_LITTLE_ENDIAN ((int)1) /*!< @brief リトルエンディアン */
#define JACIC_BIG_ENDIAN    ((int)0) /*!< @brief ビッグエンディアン */

#endif /* JACIC_ENDIANS_ */
/*! @} */

/*!
@name 関数のリターンコード定義

@details  0          : 正常終了
@details  1          : ハッシュ値が正常（チェック時のみ）
@details  0, -1, -2  : ハッシュ値の不正（チェック時のみ）
@details -101 ～     : 引数の不正などコード上の問題
@details -201 ～     : ファイル操作関連全般の問題
@details -301 ～     : Exif ファイル操作関連の問題
@details -900        : その他の問題

@{
*/
#define FUNCTION_SUCCESS                      ((int)   0)                /*!< @brief 関数が正常に終了した */

#define SAME_HASH                             ((int)   1)                /*!< @brief ハッシュ値が正しい */
#define INCORRECT_HASH_BOTH                   ((int)   0)                /*!< @brief ハッシュ値（画像、撮影日時）が正しくない */
#define INCORRECT_HASH_IMAGE                  ((int)  -1)                /*!< @brief ハッシュ値（画像）が正しくない */
#define INCORRECT_HASH_DATE                   ((int)  -2)                /*!< @brief ハッシュ値（撮影日時）が正しくない */

#define INCORRECT_PARAMETER                   ((int)-101)                /*!< @brief 不正な引数が指定された場合 */
#define SAME_FILE_PATH                        ((int)-102)                /*!< @brief 読込元と出力先の画像ファイルパスが同じ */

#define FILE_NOT_EXISTS                       ((int)-201)                /*!< @brief 読込元画像ファイルが存在しない */
#define FILE_ALREADY_EXISTS                   ((int)-202)                /*!< @brief 出力先画像ファイルが既に存在する */
#define FILE_OPEN_FAILED                      ((int)-203)                /*!< @brief ファイルオープン失敗 */
#define FILE_SIZE_ZERO                        ((int)-204)                /*!< @brief 読み込んだファイルサイズがゼロ */
#define FILE_WRITE_FAILED                     ((int)-205)                /*!< @brief ファイル書き込み失敗 */
#define FILE_CLOSE_FAILED                     ((int)-206)                /*!< @brief ファイルのクローズに失敗 */

#define INCORRECT_EXIF_FORMAT                 ((int)-301)                /*!< @brief Exif フォーマットが不正 */
#define APP5_ALREADY_EXISTS                   ((int)-302)                /*!< @brief APP5 セグメントが既に存在する */
#define APP5_NOT_EXISTS                       ((int)-303)                /*!< @brief APP5 領域が見つからない */
#define INCORRECT_APP5_FORMAT                 ((int)-304)                /*!< @brief APP5 領域の記述形式が異なる */
#define HASH_NOT_EXISTS                       ((int)-305)                /*!< @brief ハッシュ値が見つからない */
#define INCORRECT_APP1_FORMAT                 ((int)-306)                /*!< @brief APP1 領域の記述形式が異なる */
#define DATE_NOT_EXISTS                       ((int)-307)                /*!< @brief 日時情報が見つからない */
#define INCORRECT_TEXT                        ((int)-308)                /*!< @brief テキスト情報が不正 */
#define INCORRECT_IDENTIFIER                  ((int)-309)                /*!< @brief 識別子が不正 */
#define APP1_NOT_EXISTS                       ((int)-310)                /*!< @brief APP1 領域が見つからない */

#define OTHER_ERROR                           ((int)-900)                /*!< @brief その他のエラー */
/*! @} */

/*!
@name ハッシュ値埋め込み処理リターンコード

@details    0    : 正常終了
@details -101 ～ : 引数の不正などコード上の問題
@details -201 ～ : ファイル操作関連全般の問題
@details -301 ～ : Exif ファイル操作関連の問題
@details -900    : その他の問題

@{
*/
#define JW_SUCCESS                            FUNCTION_SUCCESS         /*!< @brief 正常終了 */

#define JW_ERROR_INCORRECT_PARAMETER          INCORRECT_PARAMETER      /*!< @brief 不正な引数が指定された場合 */
#define JW_ERROR_SAME_FILE_PATH               SAME_FILE_PATH           /*!< @brief 読込元と出力先の画像ファイルパスが同じ */

#define JW_ERROR_READ_FILE_NOT_EXISTS         FILE_NOT_EXISTS          /*!< @brief 読込元画像ファイルが存在しない */
#define JW_ERROR_WRITE_FILE_ALREADY_EXISTS    FILE_ALREADY_EXISTS      /*!< @brief 出力先画像ファイルが既に存在する */
#define JW_ERROR_READ_FILE_OPEN_FAILED        FILE_OPEN_FAILED         /*!< @brief ファイルオープン失敗 */
#define JW_ERROR_READ_FILE_SIZE_ZERO          FILE_SIZE_ZERO           /*!< @brief 読み込んだファイルサイズがゼロ */
#define JW_ERROR_WRITE_FILE_FAILED            FILE_WRITE_FAILED        /*!< @brief ファイル書き込み失敗 */
#define JW_ERROR_FILE_CLOSE_FAILED            FILE_CLOSE_FAILED        /*!< @brief ファイルのクローズに失敗 */

#define JW_ERROR_INCORRECT_EXIF_FORMAT        INCORRECT_EXIF_FORMAT    /*!< @brief Exif フォーマットが不正 */
#define JW_ERROR_APP5_ALREADY_EXISTS          APP5_ALREADY_EXISTS      /*!< @brief APP5 セグメントが既に存在する */

#define JW_ERROR_OTHER                        OTHER_ERROR              /*!< @brief その他のエラー */
/*! @} */

/*!
@name ハッシュ値チェック処理リターンコード

@details 1         : 正常終了、かつ正しい
@details 0, -1, -2 : 正常終了、かつ正しくない
@details -101 ～   : 引数の不正などコード上の問題
@details -201 ～   : ファイル操作関連全般の問題
@details -301 ～   : Exif ファイル操作関連の問題
@details -900      : その他の問題

@{
*/
#define JC_OK                               ((int) 1)                   /*!< @brief ハッシュ値が正しい */
#define JC_NG                               ((int) 0)                   /*!< @brief ハッシュ値（画像、撮影日時）が正しくない */
#define JC_NG_IMAGE                         ((int)-1)                   /*!< @brief ハッシュ値（画像）が正しくない */
#define JC_NG_DATE                          ((int)-2)                   /*!< @brief ハッシュ値（撮影日時）が正しくない */

#define JC_ERROR_INCORRECT_PARAMETER        INCORRECT_PARAMETER         /*!< @brief 不正な引数が指定された場合 */

#define JC_ERROR_READ_FILE_NOT_EXISTS       FILE_NOT_EXISTS             /*!< @brief 読込元画像ファイルが存在しない */
#define JC_ERROR_READ_FILE_OPEN_FAILED      FILE_OPEN_FAILED            /*!< @brief ファイルオープン失敗 */
#define JC_ERROR_READ_FILE_SIZE_ZERO        FILE_SIZE_ZERO              /*!< @brief 読み込んだファイルサイズがゼロ */
#define JC_ERROR_FILE_CLOSE_FAILED          FILE_CLOSE_FAILED           /*!< @brief ファイルのクローズに失敗 */

#define JC_ERROR_INCORRECT_EXIF_FORMAT      INCORRECT_EXIF_FORMAT       /*!< @brief Exif ファイルフォーマットが不正 */
#define JC_ERROR_APP5_NOT_EXISTS            APP5_NOT_EXISTS             /*!< @brief APP5 領域が見つからない */
#define JC_ERROR_INCORRECT_APP5_FORMAT      INCORRECT_APP5_FORMAT       /*!< @brief APP5 領域の記述形式が異なる */
#define JC_ERROR_HASH_NOT_EXISTS            HASH_NOT_EXISTS             /*!< @brief ハッシュ値が設定されていない */

#define JC_ERROR_OTHER                      OTHER_ERROR                 /*!< @brief その他のエラー */
/*! @} */

/*! @name 定数マクロ定義 */
/*! @{ */
#define BYTE_SIZE_UNSIGNED_CHAR  ((size_t) 1) /*!< @brief unsigned char のバイト数 */
#define BYTE_SIZE_UNSIGNED_SHORT ((size_t) 2) /*!< @brief unsigned short のバイト数 */
#define BYTE_SIZE_UNSIGNED_INT   ((size_t) 4) /*!< @brief unsigned int のバイト数 */
#define BIT_SIZE_1BYTE           ((size_t) 8) /*!< @brief 1 バイトのビット数 */
#define BYTE_SIZE_HASH_LENGTH    ((size_t)64) /*!< @brief ハッシュのバイト数 */
/*! @} */


typedef struct _binaryData HashBuffer; /*!< @brief バイト配列とその長さを持つ構造体 */
typedef struct _binaryData JpegBuffer; /*!< @brief 画像読み込み時に画像のバイト情報とサイズを持つ構造体 */

/*!
@struct _binaryData
@brief 自身の長さと実体を持つバイナリ構造体
*/
struct _binaryData
{
    size_t _len;           /*!< @brief 値の長さ */
    unsigned char _buff[]; /*!< @brief 値 */
};

/*!
@brief 稼働環境のエンディアンを判定し、エンディアンを示す値を返す。
@retval BIG_ENDIAN ビッグエンディアン
@retval LITTLE_ENDIAN リトルエンディアン
*/
int getEndian(void);

/*!
@brief 符号なし短整数値をエンディアン変換する。
@param src 符号なし短整数値
@return エンディアン変換後の符号なし短整数値
*/
unsigned short swapEndian16(unsigned short src);

/*!
@brief 符号なし整数値をエンディアン変換する。
@param src 符号なし整数値
@return エンディアン変換後の符号なし整数値
*/
unsigned int swapEndian32(unsigned int src);

/*!
@brief 数値を 16 進数の文字列表現で取得する。
@param [out] *dst 取得対象の文字変数のアドレス
@param hexChar 文字列表現に変換する 16 進数 1 文字で表現可能な数値
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER hexChar が不正である
*/
int getCharCode(unsigned char *dst, unsigned char hexChar);

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
int writeFile(const char *dst, unsigned char *outBuff, size_t length);

#endif /* COMMON_H_ */
