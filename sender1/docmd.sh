#!/bin/bash
cd ..
rm -rf sender*
cp -r requester sender1
cp -r requester sender2
cd sender1
mv -f split1.txt split.txt
make clean
make
cd ../sender2
mv -f split2.txt split.txt
make clean
make
cd ..
cd requester
