#!/bin/bash
for dir in ./man*; do
    cd $dir;
    for name in *; do
        cat $name | groff -mandoc -Tutf8 | col -b > ../txt/${name%.?}.txt
    done
    cd ../;
done
