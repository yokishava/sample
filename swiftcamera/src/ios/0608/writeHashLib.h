/*!
@file writeHashLib.h
@date Created on: 2015/08/10
@author DATT JAPAN Inc.
@version 1.1
@brief 改ざんチェック用ハッシュ埋め込みモジュールのヘッダ
*/

#ifndef WRITEHASHLIB_H_
#define WRITEHASHLIB_H_

/* Linux 環境の場合は構造体で戻る */
/*!
@def GET_POS(pos)
@brief ファイルの位置を取得する
@param pos ファイル位置を格納している fpos_t 型の変数
*/
#if defined(linux) && !defined(__ANDROID__) && !defined(__APPLE__)
#define GET_POS(pos) (pos.__pos)
#else
#define GET_POS(pos) (pos)
#endif

/*!
@def DECLSPEC_PORT
@brief コンパイル条件により DLL インポート / エクスポートを自動振り分けする定義
*/
#ifdef _MSC_VER
#include <windows.h>
#ifdef JACIC_HASH_EXPORTS
#define DECLSPEC_PORT __declspec( dllexport )
#else
#define DECLSPEC_PORT __declspec( dllimport )
#endif
#else
/*!
@def WINAPI
@brief Visual C++ 以外で利用している場合に Windows アプリケーションを無効にするための定義
*/
#define WINAPI
#define DECLSPEC_PORT
#endif

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
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#ifdef WRITE_EXPORTS
DECLSPEC_PORT
#endif /* WRITE_EXPORTS */
int WINAPI JACIC_WriteHashValue(const char *sourceFile, const char *destFile);

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
#ifdef CHECK_EXPORTS
DECLSPEC_PORT
#endif /* CHECK_EXPORTS */
int WINAPI JACIC_CheckHashValue(const char *checkFile);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* WRITEHASHLIB_H_ */
