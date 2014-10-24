#!/bin/sh
## Script to copy files from compact flash card
## to a specific directory

echo
echo "Preparing to transfer files..."
echo
FILEPATH=`date +%j%m%y%H%s`  ## Create folder path
COPIES=0
ERRORS=0

## Check if memory card is mounted at the correct location,
## check again if destination directory exists, and if
## not, create it, then find files on memory card and
## copy them to the specified directory.
if [ -d /mnt/sdb1/ ];then
    if [ `find /mnt/sda1/ datalog.csv | wc -l` -eq 0 ];then
        echo "card appears to be empty,"
        echo "please check card "; echo
        exit 0
    else
        if [ -d $FILEPATH ]; then
            echo "The directory already exists"
        else
            mkdir -pv $FILEPATH
        fi
        echo
        find /mnt/sda1/ datalog.csv -exec cp -v '{}' $FILEPATH \;
     fi
else
    echo "Memory card not found"
    exit 0
fi

## Verify that each file copies without any errors
echo
echo "Verifying integrity of destination files..."
while read FILE; do
    COPIES=$[$COPIES+1]
    NAME=`echo $FILE | cut -f5 -d"/"`
    if ! cmp -s "$FILE" "$FILEPATH/$NAME";then
        echo "There was a problem copying $NAME"
        ERRORS=$[$ERRORS+1]
    fi
done < <(find /mnt/sda1/ datalog.csv)

## Report status
if [ $ERRORS -gt 0 ];then
    if [ $ERRORS -eq 1 ];then
        echo "$COPIES with $ERRORS error"
    else
        echo "$COPIES with $ERRORS errors"
    fi
else
    if [ $COPIES -eq 1 ];then
        echo "$COPIES successfully"
    else
        echo "$COPIES successfully"
    fi
fi
echo
exit 0




##mv datalog.csv /mnt/sdb1/datalog$(date +%j%m%y%H%s).csv
##touch datalog.csv
##echo "transfert ok"

