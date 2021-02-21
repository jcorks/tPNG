# tPNG
PNG reader in a single, portable C file and header that dumps to raw RGBA pixels.


Current status
--------------
Still under construction! Major tasks left: 
* Interlacing support is missing.

Current tests seem to indicate that its fit for usage outside of this missing feature.


What is it?
-----------
tPNG has one intent: to retrieve raw pixels for immediate use without hassle.
The header contains one definition for one function, which returns a heap copy of 
32bit RGBA-encoded pixels, a common format for graphics-oriented software.
tPNG is a good choice if you want to get started quickly with using 
PNG files without dealing with the configuration, compilation, and 
management of libpng or other image loaders.

What is it not?
---------------
tPNG is not a replacement for more robust tools such as libpng,
which is much more performant and general purpose. tPNG is 
intended to excel in convenience and easy portability.


Why use tPNG?
------------
* No external dependencies.
* Easy to use, easy to drop in a project
* No need for a separate shared library
* Very little executable overhead (<2000 lines of C code)
* Written in portable C




