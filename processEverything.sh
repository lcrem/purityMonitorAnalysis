#!/bin/bash

source ~/ANITA/anita3/env.sh

for f in `ls $1/*ch[134].traces`; do
    echo "File is $f"
    root -b -q findAllAverages.C\(\"$f\"\)
done


for f in `ls $1/*/*ch[134].traces`; do
    echo "File is $f"
    root -b -q findAllAverages.C\(\"$f\"\)
done

for f in `ls $1/*/*/*ch[134].traces`; do
    echo "File is $f"
    root -b -q findAllAverages.C\(\"$f\"\)

done
