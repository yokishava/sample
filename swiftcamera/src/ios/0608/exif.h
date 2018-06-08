/*!
@file exif.h
@date Created on: 2015/08/10
@author DATT JAPAN Inc.
@version 1.1
@brief JPEG 画像の Exif データ操作用モジュールのヘッダ
*/

#ifndef EXIF_H_
#define EXIF_H_

#include "common.h"

/*!
@name エンディアン定義
@{
*/
extern const unsigned short EXIF_LITTLE_ENDIAN;   /*!< @brief エンディアン定義：リトルエンディアン (II) */
extern const unsigned short EXIF_BIG_ENDIAN;      /*!< @brief エンディアン定義：ビッグエンディアン (MM) */
/*!
@}

@name メタデータ全般のバイトサイズ定義
@{
*/
extern const size_t BYTE_SIZE_SEGMENT_MARKER;     /*!< @brief APP セグメント定義のバイト数 */
extern const size_t BYTE_SIZE_ENDIAN;             /*!< @brief エンディアン定義のバイト数 */
extern const size_t BYTE_SIZE_SEGMENT_SIZE;       /*!< @brief サイズ定義のバイト数 */
/*!
@}

@name セグメントマーカー定義
@{
*/
extern const unsigned short EXIF_MARKER_SOF00;    /*!< @brief SOF0 フレームヘッダ */
extern const unsigned short EXIF_MARKER_SOF01;    /*!< @brief SOF1 フレームヘッダ */
extern const unsigned short EXIF_MARKER_SOF02;    /*!< @brief SOF2 フレームヘッダ */
extern const unsigned short EXIF_MARKER_SOF03;    /*!< @brief SOF3 フレームヘッダ */
extern const unsigned short EXIF_MARKER_DHT;      /*!< @brief DHT ハフマンテーブル定義 */
extern const unsigned short EXIF_MARKER_SOF05;    /*!< @brief SOF5 フレームヘッダ */
extern const unsigned short EXIF_MARKER_SOF06;    /*!< @brief SOF6 フレームヘッダ */
extern const unsigned short EXIF_MARKER_SOF07;    /*!< @brief SOF7 フレームヘッダ */
extern const unsigned short EXIF_MARKER_JPG00;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_SOF09;    /*!< @brief SOF9 フレームヘッダ */
extern const unsigned short EXIF_MARKER_SOF10;    /*!< @brief SOF10 フレームヘッダ */
extern const unsigned short EXIF_MARKER_SOF11;    /*!< @brief SOF11 フレームヘッダ */
extern const unsigned short EXIF_MARKER_DAC;      /*!< @brief DAC 算術符号テーブル定義 */
extern const unsigned short EXIF_MARKER_SOF13;    /*!< @brief SOF13 フレームヘッダ */
extern const unsigned short EXIF_MARKER_SOF14;    /*!< @brief SOF14 フレームヘッダ */
extern const unsigned short EXIF_MARKER_SOF15;    /*!< @brief SOF15 フレームヘッダ */
extern const unsigned short EXIF_MARKER_RST00;    /*!< @brief RST0 リスタートマーカ */
extern const unsigned short EXIF_MARKER_RST01;    /*!< @brief RST1 リスタートマーカ */
extern const unsigned short EXIF_MARKER_RST02;    /*!< @brief RST2 リスタートマーカ */
extern const unsigned short EXIF_MARKER_RST03;    /*!< @brief RST3 リスタートマーカ */
extern const unsigned short EXIF_MARKER_RST04;    /*!< @brief RST4 リスタートマーカ */
extern const unsigned short EXIF_MARKER_RST05;    /*!< @brief RST5 リスタートマーカ */
extern const unsigned short EXIF_MARKER_RST06;    /*!< @brief RST6 リスタートマーカ */
extern const unsigned short EXIF_MARKER_RST07;    /*!< @brief RST7 リスタートマーカ */
extern const unsigned short EXIF_MARKER_SOI;      /*!< @brief SOI 圧縮データスタート */
extern const unsigned short EXIF_MARKER_EOI;      /*!< @brief EOI 圧縮データ終了 */
extern const unsigned short EXIF_MARKER_SOS;      /*!< @brief SOS スキャンヘッダ */
extern const unsigned short EXIF_MARKER_DQT;      /*!< @brief DQT 量子化テーブル定義 */
extern const unsigned short EXIF_MARKER_DNL;      /*!< @brief DNL ライン数定義 */
extern const unsigned short EXIF_MARKER_DRI;      /*!< @brief DRI リスタートインターバル */
extern const unsigned short EXIF_MARKER_DHP;      /*!< @brief DHP ハイアラーキカルプログレッション（階層数列）定義 */
extern const unsigned short EXIF_MARKER_EXP;      /*!< @brief EXP 成分拡大 */
extern const unsigned short EXIF_MARKER_APP00;    /*!< @brief APP0 セグメント */
extern const unsigned short EXIF_MARKER_APP01;    /*!< @brief APP1 セグメント */
extern const unsigned short EXIF_MARKER_APP02;    /*!< @brief APP2 セグメント */
extern const unsigned short EXIF_MARKER_APP03;    /*!< @brief APP3 セグメント */
extern const unsigned short EXIF_MARKER_APP04;    /*!< @brief APP4 セグメント */
extern const unsigned short EXIF_MARKER_APP05;    /*!< @brief APP5 セグメント */
extern const unsigned short EXIF_MARKER_APP06;    /*!< @brief APP6 セグメント */
extern const unsigned short EXIF_MARKER_APP07;    /*!< @brief APP7 セグメント */
extern const unsigned short EXIF_MARKER_APP08;    /*!< @brief APP8 セグメント */
extern const unsigned short EXIF_MARKER_APP09;    /*!< @brief APP9 セグメント */
extern const unsigned short EXIF_MARKER_APP10;    /*!< @brief APP10 セグメント */
extern const unsigned short EXIF_MARKER_APP11;    /*!< @brief APP11 セグメント */
extern const unsigned short EXIF_MARKER_APP12;    /*!< @brief APP12 セグメント */
extern const unsigned short EXIF_MARKER_APP13;    /*!< @brief APP13 セグメント */
extern const unsigned short EXIF_MARKER_APP14;    /*!< @brief APP14 セグメント */
extern const unsigned short EXIF_MARKER_APP15;    /*!< @brief APP15 セグメント */
extern const unsigned short EXIF_MARKER_JPG01;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG02;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG03;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG04;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG05;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG06;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG07;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG08;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG09;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG10;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG11;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG12;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG13;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_JPG14;    /*!< @brief JPEG 拡張用リザーブ */
extern const unsigned short EXIF_MARKER_COM;      /*!< @brief Exif マーカーセグメント コメント */
/*! @} */

/*!
@brief Exif ファイルに格納されている画像開始位置マーカーの存在確認とその開始位置を取得する。
@param [in] *src 対象となる JPEG 画像のバイナリ読み込み結果データ構造体
@param [in, out] *seek 取得する走査開始位置
@retval FUNCTION_SUCCESS 正常終了
@retval INCORRECT_PARAMETER いずれかの引数の参照先が NULL の場合
@retval INCORRECT_EXIF_FORMAT 処理の最後まで SOI マーカーが見つからなかった場合
*/
int checkFirstFindSOI(JpegBuffer *src, unsigned long *seek);

/*!
@brief 指定位置より、 EOI セグメントまで走査位置を移動する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param [in, out] *seek 走査位置
@return 取得した EOI セグメント（見つからなかった場合は 0xFFFF ）
*/
unsigned short seekToEOI(JpegBuffer *src, unsigned long *seek);

/*!
@brief 指定した走査位置より、「セグメントのサイズ定義」の値として 2 バイト取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param seek 指定位置
@return 取得した領域サイズ （見つからなかった、引数不正などで失敗した場合は 0x0000 ）
*/
unsigned short getAppSize(JpegBuffer *src, unsigned long seek);

/*!
@brief 指定した走査位置より、「エンディアン定義」として 2 バイト取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param seek 指定位置
@return 取得した領域サイズ （見つからなかった、引数不正などで失敗した場合は 0x0000 ）
*/
unsigned short getByteOrder(JpegBuffer *src, unsigned long seek);

/*!
@brief Exif ファイルフォーマット仕様に従い、指定位置のマーカーセグメントを取得する。
@param [in] *src JPEG 画像のバイナリ読み込み結果データ構造体
@param [in, out] *seek 指定位置
@return 取得したマーカーセグメント（見つからなかった場合は 0xFFFF ）
*/
unsigned short getMarkerSegment(JpegBuffer *src, unsigned long *seek);

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
int checkIdentifier(const char *identifier, JpegBuffer *src, unsigned long *startIndex, size_t size);

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
int getByteText(HashBuffer *src, unsigned char *text, unsigned long *seek, size_t maxByteSize);

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
int clipCompressedImage(JpegBuffer *src, HashBuffer **retImgBin, int ignoreApp5Flag);

#endif /* EXIF_H_ */
