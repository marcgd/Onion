#!/bin/sh	

cd "$1"
for FILE in ./"$2"/*
	do
    cp -rf "$FILE" "/mnt/SDCARD/"
done

sync

#cd "$1"
#rm -rf "$2"