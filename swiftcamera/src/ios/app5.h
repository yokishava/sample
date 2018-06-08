/**
 * @file app5.h
 *
 * @date Created on: 2016/01/18
 * @author Author: DATT JAPAN Inc.
 * @version 1.1
 * @brief Exif APP5領域作成・解析用ソースコード
 */

#ifndef APP5_H_
#define APP5_H_

#include "common.h"

/*!
@struct RMetaItem
@brief APP5 セグメントの RMETA 領域から取得した項目
*/
typedef struct
{
    int titleExistsFlag; /*!< @brief 項目テキスト存在フラグ */
    HashBuffer *value;   /*!< @brief 内容テキストバッファ */
} RMetaItem;

/*!
@brief Exif ファイルに格納されている APP5 領域の存在確認とその開始位置を取得する。
@param [in] *src 対象となる JPEG 画像のバイナリ読み込み結果データ構造体
@param [in, out] *seek 取得する走査開始位置
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER いずれかの引数が NULL, または走査不可能な位置が指定された場合
@retval INCORRECT_EXIF_FORMAT Exif ファイルが不正な形式の場合
@retval APP5_NOT_EXISTS APP1 領域が見つからなかった場合
*/
int checkApp5Exists(JpegBuffer *src, unsigned long *seek);

/*!
@brief 改ざんチェック用の APP5 領域を JPEG 画像に埋め込んで保存する。
@details JPEG 画像データを先頭から走査していき、最初に見つかった
@details 次のセグメント ( APP6 ～ APP15、DQT、DHT、DRI、SOF、SOS ) の前に、
@details APP5 セグメント領域を追加する。
@remarks ※スマートフォン (HUAWEI P8Lite) で APP1 -> APP0 の順番でセグメントが並んでいる
@remarks   ケースがあったので、上記ルールにて APP5 セグメント領域を追加する。
@param [in] *src JPEG画像のバイナリ読み込み結果データ構造体
@param [in] *imgHash 埋め込むハッシュ値（画像）
@param [in] *dateHash 埋め込むハッシュ値（原画像生成日時）
@param [in] *dst 出力先ファイルパス
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER 不正な引数が指定された場合
@retval FILE_OPEN_FAILED ファイルのオープンに失敗
@retval FILE_WRITE_FAILED ファイルへの書き込みに失敗
@retval FILE_CLOSE_FAILED ファイルのクローズに失敗
@retval INCORRECT_EXIF_FORMAT Exif ファイルが不正な形式の場合
@retval APP5_ALREADY_EXISTS APP5 領域が既に存在する場合
@retval OTHER_ERROR メモリの確保に失敗した場合
*/
int writeApp5(JpegBuffer *src, HashBuffer *imgHash, HashBuffer *dateHash, const char *dst);

/*!
@brief チェック対象画像の APP5 領域からハッシュ値を取得する。
@param [in] *src 対象となる JPEG 画像のバイナリ読み込み結果データ構造体
@param [out] *retImageHash 取得したハッシュ値（画像）を格納する変数
@param [out] *retDateHash 取得したハッシュ値（撮影日時）を格納する変数
@param seek 走査開始位置
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER 不正な引数が指定された場合
@retval INCORRECT_APP5_FORMAT APP5 領域が不正な形式の場合
@retval HASH_NOT_EXISTS ハッシュ値が取得できない場合
@retval OTHER_ERROR メモリ確保に失敗した場合
*/
int getApp5HashValue(JpegBuffer *src, RMetaItem *retImageHash, RMetaItem *retDateHash, unsigned long seek);

#endif /* APP5_H_ */
