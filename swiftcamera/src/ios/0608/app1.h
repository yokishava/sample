/**
 * @file app1.h
 *
 * @date Created on: 2016/01/18
 * @author Author: DATT JAPAN Inc.
 * @version 1.1
 * @brief Exif APP1領域解析用ヘッダ
 */

#ifndef APP1_H_
#define APP1_H_

#include "common.h"

/*!
@brief Exif ファイルに格納されている APP1 領域の存在確認とその開始位置を取得する。
@param [in] *src 対象となる JPEG 画像のバイナリ読み込み結果データ構造体
@param [in, out] *seek 取得する走査開始位置
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER 不正な引数が指定された場合
@retval INCORRECT_EXIF_FORMAT Exif ファイルが不正な形式の場合
@retval APP1_NOT_EXISTS APP1 領域が走査位置より下に存在しないとされる場合
*/
int checkApp1Exists(JpegBuffer *src, unsigned long *seek);

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
int getDateTimeOriginal(JpegBuffer *src, HashBuffer **retDateBin);

#endif /* APP1_H_ */
