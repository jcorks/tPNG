#!/bin/bash

# Helper tool to convert GIMP .c (with alpha) sources 
# into RGBA dumps that match tPNGs output.

items=`ls -1 | grep '\\.png\\.c'`

for i in $items; do 
    gcc -g -DIMAGENAME="\"$i\"" dumper.c -o dumper
    ./dumper
    rm -f ./dumper
done


