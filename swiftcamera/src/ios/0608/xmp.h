//
//  xmp.h
//  どこでも写真管理Plus
//
//  Created by atd on 2018/06/07.
//

#ifndef xmp_h
#define xmp_h

#include <stdio.h>

typedef struct binaryData JpegBuffer;

struct binaryData {
    size_t len;
    unsigned char buf[];
};

int writeBinary(const char *src, const char *dst);

int writeBinary2(const char *wrk, const char *dst);

#endif /* xmp_h */
