#!/bin/sh
## Script to copy files from compact flash card
## to a specific directory


## Check if memory card is mounted at the correct location,
## check again if destination directory exists, and if
## not, create it, then find files on memory card and
## copy them to the specified directory.
if [ -d /mnt/sdb1 ];then 
	cp -v /mnt/sda1/datalog.csv /mnt/sdb1/datalog$(date +%j%m%y%H%s).csv
	>/mnt/sda1/datalog.csv
	echo "Date;Température;Action">/mnt/sda1/datalog.csv
	echo "transfer ok"
	exit 0
 else
    echo "Key not found"
    exit 0
fi


