#/bin/bash

BASELINE="2018Apr11liquefaction/Day3_allLiquid/cathodeANDanode/"

FIELDS=("K-135GK135_150Vcm" "K-1600GK1100_1500Vcm" "K-22GK22_25Vcm" "K-405GK405_450Vcm" "K-45GK45_50Vcm" "K-900GK900_1000Vcm")
DIVISIONS=("100mVdiv" "100mVdiv" "100mVdiv" "100mVdiv" "100mVdiv" "100mVdiv" )

#FIELDS=("K-135GK135_150Vcm" "K-1600GK1100_1500Vcm" "K-1600GK1100_1500Vcm" "K-405GK405_450Vcm" "K-45GK45_50Vcm" "K-900GK900_1000Vcm" "K-900GK900_1000Vcm" "noField")

#DIVISIONS=("200mVdiv" "100mVdiv" "500mVdiv" "200mVdiv" "200mVdiv" "100mVdiV" "500mVdiv" "200mVdiv")

FIBRES="FibreIn FibreOut"

CHANNELS="ch4"

for ((ifield=0;ifield<${#FIELDS[@]};++ifield));
do

    field=${FIELDS[ifield]}
    div=${DIVISIONS[ifield]}
    printf "%s is in %s\n" "$field" "$div"
    
    for fibre in `echo $FIBRES`;
    do
	for channel in `echo $CHANNELS`;
	do
	    filename=$BASELINE$field"_"$fibre"_"$div"."$channel".traces"
	    echo $filename
	    root -b -q findAllAverages.C\(\"$filename\"\)
	done
    done
    
    echo Process all files for field $field   

    root -b -q getSubtraction.C\(\"$field\"\,\"$div\"\)  
done

