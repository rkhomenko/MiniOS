#!/usr/bin/env bash

SECTOR_SIZE=`fdisk -lu /dev/loop1 | sed -n 3p | cut -d ' ' -f 4` 
START=`fdisk -lu /dev/loop1 | sed -n 9p | tr -s " " |cut -d ' ' -f 3`
OFFSET=`echo "$SECTOR_SIZE * $START" | bc`
echo "$OFFSET"
