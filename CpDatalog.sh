#!/bin/sh
## Script to copy files from compact flash card
## to a specific directory


echo "Preparing to transfer files..."

## Check if memory card is mounted at the correct location,
## check again if destination directory exists, and if
## not, create it, then find files on memory card and
## copy them to the specified directory.
if [ -d /mnt/sdb1/ ];then
	echo "USB key find"
    if [ 
        echo "card appears to be empty,"
		echo "please check card "
        exit 0
    else
        if [ `cp -v /mnt/sda1/datalog.csv /mnt/sdb1/datalog$(date +%j%m%y%H%s).csv | wc -l` -eq 0 ];then
			echo "error"
			else echo "file tansferred" 
				 >/mnt/sda1/datalog.csv
				 echo "file cleared"
		fi
		exit 0
     fi
else
    echo "USB Key not found"
    exit 0
fi

##mv datalog.csv /mnt/sdb1/datalog$(date +%j%m%y%H%s).csv
##touch datalog.csv
##echo "transfert ok"

