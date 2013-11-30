#!/bin/bash

orighash=$(md5sum $1);
./main c $1
./main x $1.hff $1.test
newhash=$(md5sum $1.test)

echo original file:
echo $orighash
$(which ls) -l $1 | cut -d' ' -f5

echo new file:
echo $newhash
$(which ls) -l $1.test | cut -d' ' -f5


rm $1.hff
rm $1.test

