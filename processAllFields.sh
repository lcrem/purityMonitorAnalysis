##!/bin/bash

#BASELINE="2018Apr11liquefaction/Day3_allLiquid/cathodeANDanode/"
#FIELDS=("K-135GK135_150Vcm" "K-1600GK1100_1500Vcm" "K-22GK22_25Vcm" "K-405GK405_450Vcm" "K-45GK45_50Vcm" "K-900GK900_1000Vcm")
#DIVISIONS=("100mVdiv" "100mVdiv" "100mVdiv" "100mVdiv" "100mVdiv" "100mVdiv" )

#FIELDS=("K-135GK135_150Vcm" "K-1600GK1100_1500Vcm" "K-1600GK1100_1500Vcm" "K-405GK405_450Vcm" "K-45GK45_50Vcm" "K-900GK900_1000Vcm" "K-900GK900_1000Vcm" "noField")
#DIVISIONS=("200mVdiv" "100mVdiv" "500mVdiv" "200mVdiv" "200mVdiv" "100mVdiV" "500mVdiv" "200mVdiv")

BASELINE="2018May02liquefaction/liquid/"
FIELDS=(
    "K-68GK-50GA50A90_10.20.40Vcm"      
#    "K-43GK-25GA25A45_10.10.20Vcm"      
    "K-136GK-100GA100A180_20.40.80Vcm"  
    "K-340GK-250GA250A450_50.100.200Vcm"
    "K-272GK-200GA200A360_40.80.160Vcm" 
    "K-204GK-150G150GA270_30.60.120Vcm" 
    "K-204GK-150G150GA270_30.60.120Vcm" 
    "K-170GK-125GA125A225_25.50.100Vcm" 
    "K-475GK-350GA350A630_70.140.280Vcm"
    "K-475GK-350GA350A630_70.140.280Vcm"
    "K-475GK-350GA350A630_70.140.280Vcm"
)

DIVISIONS=(
    "50mVdiv_19.11"
#    "50mVdiv_19.35"
    "50mVdiv_18.53"
    "100mVdiv_17.30_ampSwitch"
    "100mVdiv_17.49_ampSwitch"
    "50mVdiv_18.15"
    "100mVdiv_18.09"
    "100mVdiv_18.38"
    "200mVdiv_16.59"
    "200mVdiv_17.10_ampSwitch"
    "100mVdiv_16.54"
)

#########################################################
#BASELINE="2018May02liquefaction/liquidDay2/"
#FIELDS=(
#    "K-680GK-500GA500A900_100.200.400cm"     
#    "K-680GK-500GA500A900_100.200.400cm"     
#    "K-680GK-500GA500A900_100.200.400cm"     
#    "K-680GK-500GA500A900_100.200.400cm"     
#    "K-612GK-450GA450A810_90.180.360cm"      
#    "K-544GK-400GA400A720_80.160.320cm"      
#    "K-476GK-350GA350A630_70.140.280cm"      
#    "K-476GK-350GA350A630_70.140.280cm"      
#    "K-476GK-350GA350A630_70.140.280cm"      
#    "K-476GK-350GA350A630_70.140.280cm"      
#    "K-476GK-350GA350A630_70.140.280cm"      
#    "K-340GK-250GA250A450_50.100.200cm"      
#    "K-272GK-200GA200A360_40.80.160cm"       
#    "K-680GK-500GA500A900_100.200.400cm"     
#    "K-1530GK-1125GA1125A2025_225.450.900cm" 
#    "K-1530GK-1125GA1125A2025_225.450.900cm" 
#    "K-1360GK-1000GA1000A1800_200.400.800cm" 
#    "K-1020GK-750GA750A1350_150.300.600cm"    
#)

#DIVISIONS=(
#    "100mVdiv_16.43"  
#    "100mVdiv_16.25"  
#    "100mVdiv_15.51"  
#    "100mVdiv_15.26"  
#    "100mVdiv_13.23"   
#    "100mVdiv_13.38"   
#    "100mVdiv_16.35"   
#    "100mVdiv_16.13"   
#    "100mVdiv_16.02"   
#    "100mVdiv_15.13"   
#    "100mVdiv_13.50"   
#    "100mVdiv_14.07"   
#    "100mVdiv_14.20"   
#    "100mVdiv_13.10"  
#    "200Vdiv_12.34"  
#    "1Vdiv_12.13"   
#    "200Vdiv_12.46"     
#    "200Vdiv_12.59"
#)


FIBRES="FibreIn FibreOut"

CHANNELS="ch1 ch3 ch4"

for ((ifield=0;ifield<${#FIELDS[@]};++ifield));
do

    field=${FIELDS[ifield]}
    div=${DIVISIONS[ifield]}
    printf "%s is in %s\n" "$field" "$div"

#    for fibre in `echo $FIBRES`;
#    do
#	for channel in `echo $CHANNELS`;
#	do
#	    filename=$BASELINE$field"_"$fibre"_"$div"."$channel".traces"
#	    echo $filename
#    	    root -b -q findAllAverages.C\(\"$filename\"\)
#	done
#    done
    
    echo Process all files for field $field   

    root -b -q getSubtraction.C\(\"$BASELINE\"\,\"$field\"\,\"$div\"\)  
done

