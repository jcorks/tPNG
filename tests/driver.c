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

    int chunkSize = CHUNK_SIZE;
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


static int verify_test(const char * filenamePNG, const char * filenameKey) {
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
        for(x = 0; x < h; ++x) {
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
}


int main() {
    verify_test("gray-1.png", "gray-1.data");
    
    printf("The test is complete.\n");
    return 0;
}