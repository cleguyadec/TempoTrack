#!/bin/sh
mv datalog.csv /mnt/sdb1/datalog$(date +%j%m%y%H%s).csv
touch datalog.csv
echo "transfert ok"

