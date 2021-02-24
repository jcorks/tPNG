/**************************************************************************
 *
 * tPNG:
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



#include "../tpng.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#define CHUNK_SIZE 256

enum {
    // The file could not be opened with fopen.
    TPNG_ERROR__CANNOT_OPEN_FILE = 1,    
    TPNG_ERROR__CANNOT_READ_FILE,    
    TPNG_ERROR__PARSE_FAILED,
    TPNG_ERROR__SIZE_MISMATCH,
    TPNG_ERROR__PIXEL_MISMATCH,
};

char * TPNG_ERROR__STRINGS[] = {
    "No error.",
    "Cannot open file.",
    "Cannot read file.",
    "the PNG file data was not successfully parsed.",
    "The width/height of the pixel data does not match the correct size.",
    "Incorrect pixel data."
};



static void throw_error(int error) {
    printf("TEST FAILED. Reason: %s\n", TPNG_ERROR__STRINGS[error]);
    fflush(stdout);
    exit(error);
}


static uint8_t * dump_file_data(const char * filename, uint32_t * size) {
    FILE * f = fopen(filename, "rb");
    if (!f) {
        throw_error(TPNG_ERROR__CANNOT_OPEN_FILE);
    }

    char chunk[CHUNK_SIZE];
    *size = 0;
    uint32_t readAmt = 0;
    while((readAmt = fread(chunk, 1, CHUNK_SIZE, f))) 
        *size += readAmt;

    fseek(f, SEEK_SET, 0);
    uint8_t * out = malloc(*size);
    if (fread(out, 1, *size, f) != *size) {
        throw_error(TPNG_ERROR__CANNOT_READ_FILE);
    }
    fclose(f);
    return out;
}


static void integrity_check(const char * filenamePNG) {
    printf("checking integrity of %s...\n", filenamePNG);

    uint32_t  pngsize;
    uint8_t * pngdata = dump_file_data(filenamePNG, &pngsize);

    uint32_t w;
    uint32_t h;

    uint8_t * pixels = tpng_get_rgba(
        pngdata,
        pngsize,
        &w, 
        &h 
    );

    free(pixels);
    free(pngdata);
}


static int verify_test(const char * filenamePNG) {
    char * filenameKey = malloc(strlen(filenamePNG) + 256);;
    sprintf(filenameKey, "rawdata/%s.c.data", filenamePNG);


    printf("checking %s against %s...\n", filenamePNG, filenameKey);

    uint32_t  pngsize;
    uint8_t * pngdata = dump_file_data(filenamePNG, &pngsize);

    uint32_t  keysize;
    uint8_t * keydata = dump_file_data(filenameKey, &keysize);



    uint32_t w;
    uint32_t h;

    uint8_t * pixels = tpng_get_rgba(
        pngdata,
        pngsize,
        &w, 
        &h 
    );
    if (!pixels || w==0 || h==0) {
        throw_error(TPNG_ERROR__PARSE_FAILED);
    }

    if (keysize != w*h*4) {
        throw_error(TPNG_ERROR__SIZE_MISMATCH);
    }

    uint8_t * pixelIter = pixels;
    uint8_t * keyIter = keydata;
    uint32_t x, y;
    for(y = 0; y < h; ++y) {
        for(x = 0; x < w; ++x) {
            if (pixelIter[0] != keyIter[0] ||
                pixelIter[1] != keyIter[1] ||
                pixelIter[2] != keyIter[2] ||
                pixelIter[3] != keyIter[3]) {
                printf("At pixel (%d, %d): Pixel differs from key.\n", x+1, y+1);  
                printf("                 :  R   G   B   A \n");
                printf("            tPNG : %3d %3d %3d %3d\n", pixelIter[0], pixelIter[1], pixelIter[2], pixelIter[3]);
                printf("         correct : %3d %3d %3d %3d\n", keyIter  [0], keyIter  [1], keyIter  [2], keyIter  [3]);

                throw_error(TPNG_ERROR__PIXEL_MISMATCH);
            }

            pixelIter += 4;
            keyIter += 4;
        }
    }


    free(pixels);
    free(keydata);
    free(pngdata);
    free(filenameKey);
    return 0;
}


int main() {


    verify_test("gray-1.png");
    verify_test("gray-1-1.8.png");
    verify_test("gray-1-1.8-tRNS.png");
    verify_test("gray-1-linear.png");
    verify_test("gray-1-linear-tRNS.png");
    verify_test("gray-1-sRGB.png");
    verify_test("gray-1-sRGB-tRNS.png");
    verify_test("gray-1-tRNS.png");
    verify_test("gray-2.png");
    verify_test("gray-2-1.8.png");
    verify_test("gray-2-linear.png");
    verify_test("gray-2-linear-tRNS.png");
    verify_test("gray-2-sRGB.png");
    verify_test("gray-2-sRGB-tRNS.png");
    verify_test("gray-2-tRNS.png");
    verify_test("gray-4.png");
    verify_test("gray-4-1.8.png");
    verify_test("gray-4-linear.png");
    verify_test("gray-4-linear-tRNS.png");
    verify_test("gray-8.png");
    verify_test("gray-8-1.8.png");
    verify_test("gray-8-1.8-tRNS.png");
    verify_test("gray-8-linear.png");
    verify_test("gray-8-linear-tRNS.png");
    verify_test("gray-8-sRGB.png");
    verify_test("gray-8-sRGB-tRNS.png");
    verify_test("gray-16.png");
    verify_test("gray-16-1.8.png");
    verify_test("gray-16-1.8-tRNS.png");
    verify_test("gray-16-linear.png");
    verify_test("gray-16-linear-tRNS.png");
    verify_test("gray-16-sRGB.png");
    verify_test("gray-16-sRGB-tRNS.png");
    verify_test("gray-16-tRNS.png");
    verify_test("gray-filter0.png");
    verify_test("gray-filter1.png");
    verify_test("gray-filter2.png");
    verify_test("gray-filter3.png");
    verify_test("gray-filter4.png");
    verify_test("gray-filtern.png");

    verify_test("palette-1-1.8.png");
    verify_test("palette-1-1.8-tRNS.png");
    verify_test("palette-1-linear.png");
    verify_test("palette-1-linear-tRNS.png");
    verify_test("palette-1.png");
    verify_test("palette-1-sRGB.png");
    verify_test("palette-1-sRGB-tRNS.png");
    verify_test("palette-2-1.8.png");
    verify_test("palette-2-1.8-tRNS.png");
    verify_test("palette-2-linear.png");
    verify_test("palette-2-linear.png");
    verify_test("palette-2.png");
    verify_test("palette-2-sRGB.png");
    verify_test("palette-2-sRGB-tRNS.png");
    verify_test("palette-2-tRNS.png");
    verify_test("palette-4-1.8.png");
    verify_test("palette-4-1.8-tRNS.png");
    verify_test("palette-4-linear.png");
    verify_test("palette-4-linear-tRNS.png");
    verify_test("palette-4.png");
    verify_test("palette-4-sRGB.png");
    verify_test("palette-4-sRGB-tRNS.png");
    verify_test("palette-4-tRNS.png");
    verify_test("palette-8-1.8.png");
    verify_test("palette-8-1.8-tRNS.png");
    verify_test("palette-8-linear.png");
    verify_test("palette-8-linear-tRNS.png");
    verify_test("palette-8.png");
    verify_test("palette-8-sRGB.png");
    verify_test("palette-8-sRGB-tRNS.png");
    verify_test("palette-8-tRNS.png");
    verify_test("rgb-16-1.8.png");
    verify_test("rgb-16-1.8-tRNS.png");
    verify_test("rgb-16-linear.png");
    verify_test("rgb-16-linear-tRNS.png");
    verify_test("rgb-16.png");
    verify_test("rgb-16-sRGB.png");
    verify_test("rgb-16-sRGB-tRNS.png");
    verify_test("rgb-16-tRNS.png");
    verify_test("rgb-8-1.8.png");
    verify_test("rgb-8-1.8-tRNS.png");
    verify_test("rgb-8-linear.png");
    verify_test("rgb-8-linear-tRNS.png");
    verify_test("rgb-8.png");
    verify_test("rgb-8-sRGB.png");
    verify_test("rgb-8-sRGB-tRNS.png");
    verify_test("rgb-8-tRNS.png");
    verify_test("rgb-alpha-16-1.8.png");
    verify_test("rgb-alpha-16-linear.png");
    verify_test("rgb-alpha-16.png");
    verify_test("rgb-alpha-16-sRGB.png");
    verify_test("rgb-alpha-8-1.8.png");
    verify_test("rgb-alpha-8-linear.png");
    verify_test("rgb-alpha-8.png");
    verify_test("rgb-alpha-8-sRGB.png");
    verify_test("rgb-filter0.png");
    verify_test("rgb-filter1.png");
    verify_test("rgb-filter2.png");
    verify_test("rgb-filter3.png");
    verify_test("rgb-filter4.png");

    verify_test("interlace-8-grayscale-alpha.png");
    verify_test("interlace-1-palette.png");
    verify_test("interlace-2-grayscale.png");
    verify_test("interlace-2-palette.png");
    verify_test("interlace-4-grayscale.png");
    verify_test("interlace-4-palette.png");
    verify_test("interlace-8-grayscale.png");
    verify_test("interlace-8-palette.png");
    verify_test("interlace-8-rgb.png");
    verify_test("interlace-8-rgba.png");
    verify_test("interlace-16-grayscale.png");
    verify_test("interlace-16-grayscale-alpha.png");
    verify_test("interlace-16-rgb.png");
    verify_test("interlace-16-rgba.png");
    verify_test("interlace-bw.png");



    integrity_check("crashers/badadler.png");
    integrity_check("crashers/badcrc.png");
    integrity_check("crashers/bad_iCCP.png");
    integrity_check("crashers/empty_ancillary_chunks.png");
    integrity_check("crashers/huge_bKGD_chunk.png");
    integrity_check("crashers/huge_cHRM_chunk.png");
    integrity_check("crashers/huge_eXIf_chunk.png");
    integrity_check("crashers/huge_gAMA_chunk.png");
    integrity_check("crashers/huge_hIST_chunk.png");
    integrity_check("crashers/huge_iCCP_chunk.png");
    integrity_check("crashers/huge_IDAT.png");
    integrity_check("crashers/huge_iTXt_chunk.png");
    integrity_check("crashers/huge_juNk_safe_to_copy.png");
    integrity_check("crashers/huge_juNK_unsafe_to_copy.png");
    integrity_check("crashers/huge_pCAL_chunk.png");
    integrity_check("crashers/huge_pHYs_chunk.png");
    integrity_check("crashers/huge_sCAL_chunk.png");
    integrity_check("crashers/huge_sPLT_chunk.png");
    integrity_check("crashers/huge_sRGB_chunk.png");
    integrity_check("crashers/huge_sTER_chunk.png");
    integrity_check("crashers/huge_tEXt_chunk.png");
    integrity_check("crashers/huge_tIME_chunk.png");
    integrity_check("crashers/huge_zTXt_chunk.png");

    verify_test("average-a.png");
    verify_test("average-b.png");
    verify_test("important.png");
    
    verify_test("interlace-small.png");
    verify_test("interlace.png");
    verify_test("interlace-medium.png");

    
    printf("The test is complete.\n");
    return 0;
}
