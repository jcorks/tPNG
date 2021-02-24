/**************************************************************************
 *
 * tPNG:
 * tPNG is part of the topaz project. 
 * 2021, Johnathan Corkery
 *
 *
 * TINFL:
 * Copyright 2013-2014 RAD Game Tools and Valve Software
 * Copyright 2010-2014 Rich Geldreich and Tenacious Software LLC
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 **************************************************************************/


#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHUNK_SIZE 1024
uint8_t * dump_file_data(const char * filename, uint32_t * size) {
    FILE * f = fopen(filename, "rb");
    if (!f) {
        printf("Cannot open file!");
        exit(2);
    }

    char chunk[CHUNK_SIZE];
    *size = 0;
    uint32_t readAmt = 0;
    while((readAmt = fread(chunk, 1, CHUNK_SIZE, f))) 
        *size += readAmt;

    fseek(f, SEEK_SET, 0);
    uint8_t * out = malloc(*size);
    if (fread(out, 1, *size, f) != *size) {
        printf("Cannot read file!");
        exit(1);
    }
    fclose(f);
    return out;
}
