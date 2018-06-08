/*!
@file sha256.h
@date Created on: 2015/08/10
@author DATT JAPAN Inc.
@version 1.1
@brief ハッシュ値 (SHA-256) 計算モジュールのヘッダ
*/

#ifndef SHA256_H_
#define SHA256_H_

#include "common.h"

/*!
@brief 可変長のバイト配列を受けてハッシュ値を返す。
@param [in] *byteArray ハッシュ値を計算するもととなるバイト配列構造体
@param [out] *result 最終的に算出されたハッシュ値を受けとる変数
@retval FUNCTION_SUCCESS 正常終了
@retval OTHER_ERROR メモリの確保に失敗した場合、ハッシュ値の格納に失敗した場合など
*/
int hash(HashBuffer *byteArray, HashBuffer *result);

#endif /* SHA256_H_ */
