# tPNG
A portable C99 file and header that dumps PNG files to raw RGBA pixels with no dependencies.
The interface consists of one function.


What is it?
-----------
_tPNG has one goal: to retrieve raw pixels for immediate use without hassle._
The header contains one definition for one function, which returns a heap copy of 
32bit RGBA-encoded pixels, a common format for graphics-oriented software.
tPNG is a good choice if you want to get started quickly with using 
PNG files without dealing with the configuration, compilation, and 
management of libpng or other image loaders.

What is it not?
---------------
tPNG is not a replacement for more robust tools such as libpng,
which is much more performant and general purpose. tPNG is an alternative
intended to excel in convenience and easy portability.


Why use tPNG?
------------
* No external dependencies.
* Easy to use, easy to drop in a project.
* No need for a separate shared library.
* Very little executable overhead (~2000 lines of C code).
* Thread-safe.
* Written in portable, plain C99 (mostly for the sized ints!).


How is tPNG used?
----------------
If you've worked with pixel byte data before, tPNG will be straightfoward.
It is a single call that takes in a raw data buffer for a PNG file and 
returns raw pixels which you free. That's it! 

```C
// First, include tpng.h.
// This only brings in one header: the stdint.h header 
// for sized integers.
//
#include "tpng.h"

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
    // data. The width and height of the image are also extracted.
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
        handle_error();
        return 0;
    }

    // Now we can use it! Nothing fancy. It's just in RGBA, 32bit format.
    // The whole image is a single buffer, laid out in rows from left to right,
    // top to bottom.
    //
    function_using_rgba_data(rgbaData, width, height);
    
    // Don't forget to free the heap-allocated buffer after you're done!
    //
    free(rgbaData);
    return 1;
}

```



