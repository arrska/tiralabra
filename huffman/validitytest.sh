#!/bin/bash

orighash=$(md5sum $1 | cut -d' ' -f1)
origlen=$($(which ls) -l $1 | cut -d' ' -f5)

newfile=hufftest.$RANDOM
./main c $1 $newfile.hff

./main d $newfile.hff $newfile
newhash=$(md5sum $newfile | cut -d' ' -f1)
newlen=$($(which ls) -l $newfile | cut -d' ' -f5)

rm $newfile
rm $newfile.hff

echo
echo
echo original file:
echo hash: $orighash
echo size: $origlen
echo
echo new file:
echo hash: $newhash
echo size: $newlen
echo

if [ "$orighash"="$newhash" ]; then
	echo OK! Files match!!
	exit 0
else
	echo Files dont match!
	exit 1
fi


