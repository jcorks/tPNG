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



// First, include tpng.h.
// This only brings in one header: the stdint.h header 
// for sized integers.
//
#include "../tpng.h"


// this will be used to print the data we read in decode_png()
#include <stdio.h>
#include <stdlib.h>
void print_png(const uint8_t * data, uint32_t width, uint32_t height) {
    uint32_t x, y;
    for(y = 0; y < height; ++y) {
        for(x = 0; x < width; ++x) {
            printf("Pixel @ X and Y (%d, %d): %3d %3d %3d %3d\n",
                x+1, y+1,
                data[0],
                data[1],
                data[2],
                data[3]
            );
            data+=4;
        }
    }
}


// tPNG is for decoding raw PNG file data, so before starting
// it is up to you to read the raw bytes from the PNG source, whether its a simple
// .png file, a network location, etc. You'll also need the size in bytes!
//
int decode_png(void * pngdata, unsigned int pngSize) {

    // Then, declare a place to hold the width / height.
    //
    uint32_t width;
    uint32_t height;


    // Next, we use tPNG to extract the raw color values as 8bit R G B A
    // data. The width and height are also extracted.
    //
    uint8_t * rgbaData = tpng_get_rgba(
        pngdata,
        pngSize,
        &width,
        &height
    );

    // There could have been an error! In such a case, the pixel data will 
    // be NULL and the width/height 0. Note that in most cases tPNG will 
    // try to read as much of the file as possible. Any parts that cannot be read 
    // default to "fully transparent black" (#00000000)
    //
    if (rgbaData == NULL) {
        return 0;
    }

    // Now we can use it! Nothing fancy. It's just in RGBA, 32bit format.
    // The whole image is a single buffer, laid out in rows from left to right,
    // top to bottom.
    //
    print_png(rgbaData, width, height);
    
    // Don't forget to free the heap-allocated buffer after you're done!
    //
    free(rgbaData);
    return 1;
}


// gives a simple function to dump data from a file.
#include "helper.h"

int main() {
    uint32_t  pngsize;
    uint8_t * pngdata = dump_file_data("example.png", &pngsize);
    if (!decode_png(pngdata, pngsize)) {
        printf("An error was encountered!");
        return 1;
    }

    return 0;
}