#!/usr/bin/env bash

SECTOR_SIZE=`fdisk -lu /dev/loop1 | sed -n 3p | cut -d ' ' -f 4` 
END=`fdisk -lu /dev/loop1 | sed -n 9p | tr -s " " |cut -d ' ' -f 4`
SIZE_LIMIT=`echo "$SECTOR_SIZE * $END" | bc`
echo "$SIZE_LIMIT"
