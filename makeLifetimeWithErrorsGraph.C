#include "functions.h"

// string basefolder="2018Apr11liquefaction/Day3_allLiquid/K_GK_signals/";
// string fields[] = {"noField", "K-45GK45_50Vcm", "K-135GK135_150Vcm", "K-405GK405_450Vcm", "K-1600GK1100_1500Vcm"};
// string divisions[]={"200mVdiv", "200mVdiv", "200mVdiv", "200mVdiv", "500mVdiv"};
// string fieldNice[] = {"No field", "50 V/cm", "150 V/cm", "450 V/cm", "1500 V/cm"};
//int colors[]       = {kBlack,  kGreen+1, kBlue+1, kRed, kViolet};

// string basefolder="2018Apr11liquefaction/Day3_allLiquid/cathodeANDanode/";
// string fields[]    = {"noField", "K-22GK22_25Vcm", "K-45GK45_50Vcm", "K-135GK135_150Vcm", "K-405GK405_450Vcm", "K-900GK900_1000Vcm", "K-1600GK1100_1500Vcm"};
// string divisions[] = {"200mVdiv", "100mVdiv", "100mVdiv", "100mVdiv", "100mVdiv", "100mVdiv", "100mVdiv"};
// string fieldNice[] = {"No field", "25 V/cm", "50 V/cm", "150 V/cm", "450 V/cm", "1000 V/cm", "1500 V/cm"};
// int colors[]       = {kGray+2,  kBlue+1, kCyan+1, kGreen+1,  kRed, kOrange+1, kViolet+1};


// string basefolder="2018May29liquefaction/liquid/";
// string fields[]    = {
//   //"noField",
  
//   "K-356GK-320GA320A400_20.40.80Vcm",
  
//   "K-445GK-400GA400A500_25.50.100Vcm",
  
//   "K-534GK-480GA480A600_30.60.120Vcm",
  
//   "K-712GK-640GA640A800_40.80.160Vcm",
  
//   "K-890GK-800GA800A1000_50.100.200Vcm",
  
//   "K-1068GK-960GA960A1200_60.120.240Vcm",
  
//   "K-1246GK-1120GA1120A1400_70.140.280Vcm",
  
//   "K-1424GK-1280GA1280A1600_80.160.320Vcm",
  
//   "K-1602GK-1440GA1440A1800_90.180.360Vcm",
  
//   "K-1780GK-1600GA1600A2000_100.200.400Vcm"
  
  
// };
// string divisions[] = {
//   //"50mVdiv_13.14"
  
//   "100mVdiv_20.11",

//   "100mVdiv_20.16",
  
//   "100mVdiv_20.21",
  
//   "100mVdiv_20.26",
  
//   "100mVdiv_20.31",
  
//   "100mVdiv_20.37",
  
//   "200mVdiv_20.44",
  
//   "200mVdiv_20.49",
  
//   "200mVdiv_20.56",
  
//   "200mVdiv_21.01"
// };

// string fieldNice[] = {
//   //"nofield"
//   "20-40-80 V/cm",
  
//   "25-50-100 V/cm",
  
//   "30-60-120 V/cm",
  
//   "40-80-160 V/cm",
  
//   "50-100-200 V/cm",
  
//   "60-120-240 V/cm",
  
//   "70-140-280 V/cm",
  
//   "80-140-280 V/cm",
  
//   "90-180-360 V/cm",
  
//   "100-200-400 V/cm"
// };





/*string basefolder="2018May02liquefaction/liquid/";
 string fields[]    = {
 "K-68GK-50GA50A90_10.20.40Vcm",
 "K-136GK-100GA100A180_20.40.80Vcm",
 "K-170GK-125GA125A225_25.50.100Vcm",
 "K-204GK-150G150GA270_30.60.120Vcm",
 "K-204GK-150G150GA270_30.60.120Vcm",
 "K-272GK-200GA200A360_40.80.160Vcm",
 "K-340GK-250GA250A450_50.100.200Vcm",
 "K-475GK-350GA350A630_70.140.280Vcm",
 "K-475GK-350GA350A630_70.140.280Vcm",
 "K-475GK-350GA350A630_70.140.280Vcm"
 
 };
 string divisions[] = {
 "50mVdiv_19.11",
 "50mVdiv_18.53",
 "100mVdiv_18.38",
 "100mVdiv_18.09",
 "50mVdiv_18.15",
 "100mVdiv_17.49_ampSwitch",
 "100mVdiv_17.30_ampSwitch",
 "100mVdiv_16.54",
 "200mVdiv_16.59",
 "200mVdiv_17.10_ampSwitch"
 
 };
 
 string fieldNice[] = {
 "10-20-40 V/cm",
 "20-40-80 V/cm",
 "25-50-100 V/cm",
 "30-60-120 V/cm",
 "30-60-120 V/cm",
 "40-80-160 V/cm",
 "50-100-200 V/cm",
 "70-140-280 V/cm",
 "70-140-280 V/cm",
 "70-140-280 V/cm"
 };*/

// string basefolder="2018May02liquefaction/liquidDay2/";
// string fields[]    = {
//   "K-680GK-500GA500A900_100.200.400cm",
//   "K-680GK-500GA500A900_100.200.400cm",
//   "K-680GK-500GA500A900_100.200.400cm",
//   "K-680GK-500GA500A900_100.200.400cm",
//   "K-612GK-450GA450A810_90.180.360cm",
//   "K-544GK-400GA400A720_80.160.320cm",
//   "K-476GK-350GA350A630_70.140.280cm",
//   "K-476GK-350GA350A630_70.140.280cm",
//   "K-476GK-350GA350A630_70.140.280cm",
//   "K-476GK-350GA350A630_70.140.280cm",
//   "K-476GK-350GA350A630_70.140.280cm",
//   "K-340GK-250GA250A450_50.100.200cm",
//   "K-272GK-200GA200A360_40.80.160cm",
//   "K-680GK-500GA500A900_100.200.400cm",
//   "K-1530GK-1125GA1125A2025_225.450.900cm",
//   "K-1360GK-1000GA1000A1800_200.400.800cm",
//   "K-1020GK-750GA750A1350_150.300.600cm"

// };
// string divisions[] = {
//   "100mVdiv_16.43",
//   "100mVdiv_16.25",
//   "100mVdiv_15.51",
//   "100mVdiv_15.26",
//   "100mVdiv_13.23",
//   "100mVdiv_13.38",
//   "100mVdiv_16.35",
//   "100mVdiv_16.13",
//   "100mVdiv_16.02",
//   "100mVdiv_15.13",
//   "100mVdiv_13.50",
//   "100mVdiv_14.07",
//   "100mVdiv_14.20",
//   "100mVdiv_13.10",
//   "200mVdiv_12.34",
//   "1Vdiv_12.13",
//   "200mVdiv_12.46"
// };

// string fieldNice[] = {
//   "100-200-400 V/cm",
//   "100-200-400 V/cm",
//   "100-200-400 V/cm",
//   "100-200-400 V/cm",

//   "90-180-360 V/cm",
//   "80-160-320 V/cm",
//   "70-140-280 V/cm",
//   "70-140-280 V/cm",
//   "70-140-280 V/cm",
//   "70-140-280 V/cm",
//   "70-140-280 V/cm",

//   "50-100-200 V/cm",
//   "40-80-160 V/cm",
//   "100-200-400 V/cm",

//   "225-450-900 V/cm",
//   "200-400-800 V/cm",
//   "150-300-600 V/cm"
// };

string myname="LongFibreGold";
string basefolder="/unix/dune/purity/2018September21Liquefaction/";
string fields[]={
  // // "liquid/Silver/K-890GK-800GA800A1000_50.100.200V.cm"    ,
  // "liquid/Silver/K-890GK-800GA800A1000_50.100.200V.cm"    ,
  // "liquid/Silver/K-445GK-400GA400A500_25.50.100V.cm"      ,
  // "liquid/Silver/K-445GK-400GA400A500_25.50.100V.cm"      ,
  // "liquid/Silver/K-1780GK-1600GA1600A2000_100.200.400V.cm",
  // "liquid/Silver/K-1780GK-1600GA1600A2000_100.200.400V.cm",
  // "liquid/Silver/K-1602GK-1440GA1440A1800_90.180.360V.cm" ,
  // "liquid/Silver/K-1246GK-1120GA1120A1400_70.140.280V.cm" ,
  // "liquid/Silver/K-1246GK-1120GA1120A1400_70.140.280V.cm" ,
  // "liquid/Silver/K-1246GK-1120GA1120A1400_70.140.280V.cm" ,
  
   // "liquid/Gold/K-890GK-800GA800A1000_50.100.200V.cm"       ,
   // "liquid/Gold/K-890GK-800GA800A1000_50.100.200V.cm"       ,
   // "liquid/Gold/K-445GK-400GA400A500_25.50.100V.cm"         ,
   // "liquid/Gold/K-1780GK-1600GA1600A2000_100.200.400V.cm"   ,
   // "liquid/Gold/K-1780GK-1600GA1600A2000_100.200.400V.cm"   ,
   // "liquid/Gold/K-1602GK-1440GA1440A1800_90.180.360V.cm"    ,
   // "liquid/Gold/K-1246GK-1120GA1120A1400_70.140.280V.cm"    ,
   // "liquid/Gold/K-1246GK-1120GA1120A1400_70.140.280V.cm"    ,
  
  // "longFibreLiquid/Silver/K-890GK-800GA800A1000_50.100.200V.cm"    ,  
  // "longFibreLiquid/Silver/K-1246GK-1120GA1120A1400_70.140.280V.cm",   
  // "longFibreLiquid/Silver/K-1780GK-1600GA1600A2000_100.200.400V.cm",  

  "longFibreLiquid/Gold/K-890GK-800GA800A1000_50.100.200V.cm",        
  "longFibreLiquid/Gold/K-1246GK-1120GA1120A1400_70.140.280V.cm",     
  "longFibreLiquid/Gold/K-1780GK-1600GA1600A2000_100.200.400V.cm",    




	       };

string fieldNice[]={
  // // "Silver 50.100.200V.cm"    ,
  // "Silver 50.100.200V.cm"    ,
  // "Silver 25.50.100V.cm"      ,
  // "Silver 25.50.100V.cm"      ,
  // "Silver 100.200.400V.cm",
  // "Silver 100.200.400V.cm",
  // "Silver 90.180.360V.cm" ,
  // "Silver 70.140.280V.cm" ,
  // "Silver 70.140.280V.cm" ,
  // "Silver 70.140.280V.cm" ,
  
   // "Gold 50.100.200V.cm"       ,
   // "Gold 50.100.200V.cm"       ,
   // "Gold 25.50.100V.cm"         ,
   // "Gold 100.200.400V.cm"   ,
   // "Gold 100.200.400V.cm"   ,
   // "Gold 90.180.360V.cm"    ,
   // "Gold 70.140.280V.cm"    ,
   // "Gold 70.140.280V.cm"    ,

  "50.100.200V.cm",  
  "70.140.280V.cm",   
  "100.200.400V.cm",  

};

string divisions[]={
  // // "23.08",
  // "02.01",
  // "22.25",
  // "22.17",
  // "23.18",
  // "01.56",
  // "23.02",
  // "22.56",
  // "22.52",
  // "01.50",
  
   // "01.33",
   // "00.18",
   // "23.49",
   // "23.42",
   // "01.27",
   // "23.59",
   // "01.43",
   // "00.12",

  // "01.03",
  // "00.58",
  // "01.09",
       
  "00.34",
  "00.50",
  "00.43",

};


int colors[]       = {kGray+2,  kBlue+1, kCyan+1, kGreen+1,  kRed, kOrange+1, kViolet+1, kBlue+2, kBlue+3, kBlue+5, kBlue+9};

string whichAvg[4] = {"justAvg", "filteredAvg", "zeroedAvg", "fancyFilteredAvg"};
string chnamenice[2] = {"anode", "Cathode"};
double scaleY[2]     = {0.1,      0.1};
string smoothnice[2] = {"", "smooth"};

string plots[] = {"t1", "t2", "t3", "QA", "QK", "QAcorr", "QKcorr" , "R", "lifetime", "lifetime2"};

void makeLifetimeWithErrorsGraph(){
  
  
  int numFields = (sizeof(fields)/sizeof(*fields));
  int nplots    = (sizeof(plots)/sizeof(*plots));
  
  char randomtxt[20];
  
  double tK, tGK, tGA, tA, t1, t2, t3;
  double QA, QK, QAcorr, QKcorr, R, lifetime, lifetime2;
  double lifetime50, error50, rms50, rmserr50;
  double lifetime100, error100, rms100, rmserr100;
  double lifetime200, error200, rms200, rmserr200;

  TGraph *gpurity1000 = new TGraph(numFields);
  TGraphErrors *gpurity50  = new TGraphErrors(numFields);
  TGraphErrors *gpurity100  = new TGraphErrors(numFields);
  TGraphErrors *gpurity200  = new TGraphErrors(numFields);
  
  for(int ifield=0; ifield<numFields; ifield++){
    
    //avg 1000
    string intxtfile = basefolder +  fields[ifield] + "_" + divisions[ifield] + "_lifetime.txt";
    cout << intxtfile << endl;
    ifstream f (intxtfile.c_str());
    //    f.open();
    f >> randomtxt >> randomtxt >> tK;
    f >> randomtxt >> randomtxt >> tGK;
    f >> randomtxt >> randomtxt >> tGA;
    f >> randomtxt >> randomtxt >> tA;
    f >> randomtxt >> randomtxt >> t1;
    f >> randomtxt >> randomtxt >> t2;
    f >> randomtxt >> randomtxt >> t3;
    f >> randomtxt >> randomtxt >> QA;
    f >> randomtxt >> randomtxt >> QK;
    f >> randomtxt >> randomtxt >> QAcorr;
    f >> randomtxt >> randomtxt >> QKcorr;
    f >> randomtxt >> randomtxt >> R;
    f >> randomtxt >> randomtxt >> lifetime;
    f >> randomtxt >> randomtxt >> lifetime2;
    
    printf("lifetime us: %12.4e \n",  lifetime*1e6);
    
    f.close();
    
    //avg 200
    string intxtfileavg = basefolder +  fields[ifield] + "_" + divisions[ifield] + "_lifetime_withErrors.txt";
    cout << intxtfileavg << endl;
    ifstream favg (intxtfileavg.c_str());
    //    f.open();
    favg >> randomtxt;
    favg >> lifetime50  >> error50 >> rms50 >> rmserr50;
    favg >> randomtxt;
    favg >> lifetime100  >> error100 >> rms100 >> rmserr100;
    favg >> randomtxt;
    favg >> lifetime200  >> error200 >> rms200 >> rmserr200;
    
    printf("avg50  lifetime us: %12.4e +- %12.4e \n",  lifetime50*1e6, rms50*1e6);
    printf("avg100 lifetime us: %12.4e +- %12.4e \n",  lifetime100*1e6, rms100*1e6);
    printf("avg200 lifetime us: %12.4e +- %12.4e \n",  lifetime200*1e6, rms200*1e6);
    
    favg.close();
    
    std::string::size_type i = fieldNice[ifield].find(".");
    if (i != std::string::npos)
      fieldNice[ifield].erase(0, i+1);
    std::string::size_type j = fieldNice[ifield].find(".");
    if (j != std::string::npos)
      fieldNice[ifield].erase(j, fieldNice[ifield].length());

    double fieldValue = stoi(fieldNice[ifield]);
    cout << fieldValue << endl;

    gpurity1000->SetPoint(ifield, fieldValue, lifetime*1e6);
    gpurity200->SetPoint(ifield,  fieldValue, lifetime100*1e6);
    gpurity100->SetPoint(ifield,  fieldValue, lifetime200*1e6);
    gpurity50->SetPoint(ifield,   fieldValue, lifetime50*1e6);
    gpurity200->SetPointError(ifield, 0, rms200*1e6);
    gpurity100->SetPointError(ifield, 0, rms100*1e6);
    gpurity50->SetPointError(ifield, 0, rms50*1e6);
    
  }
  
  TFile *fout = new TFile(((basefolder+"/liquid/plots/Allfields_lifetimeWithErrors"+myname+".root").c_str()), "recreate");
  gpurity1000->Write("glifetime1000");
  gpurity200->Write("glifetime200");
  gpurity100->Write("glifetime100");
  gpurity50->Write("glifetime50");
  
  
}
