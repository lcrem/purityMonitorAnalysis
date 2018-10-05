#include "functions.h"

// string basename="2018Apr11liquefaction/Day3_allLiquid/K_GK_signals/";
// string fields[] = {"noField", "K-45GK45_50Vcm", "K-135GK135_150Vcm", "K-405GK405_450Vcm", "K-1600GK1100_1500Vcm"};
// string divisions[]={"200mVdiv", "200mVdiv", "200mVdiv", "200mVdiv", "500mVdiv"};
// string fieldNice[] = {"No field", "50 V/cm", "150 V/cm", "450 V/cm", "1500 V/cm"};
//int colors[]       = {kBlack,  kGreen+1, kBlue+1, kRed, kViolet};

// string basename="2018Apr11liquefaction/Day3_allLiquid/cathodeANDanode/";
// string fields[]    = {"noField", "K-22GK22_25Vcm", "K-45GK45_50Vcm", "K-135GK135_150Vcm", "K-405GK405_450Vcm", "K-900GK900_1000Vcm", "K-1600GK1100_1500Vcm"};
// string divisions[] = {"200mVdiv", "100mVdiv", "100mVdiv", "100mVdiv", "100mVdiv", "100mVdiv", "100mVdiv"};
// string fieldNice[] = {"No field", "25 V/cm", "50 V/cm", "150 V/cm", "450 V/cm", "1000 V/cm", "1500 V/cm"};
// int colors[]       = {kGray+2,  kBlue+1, kCyan+1, kGreen+1,  kRed, kOrange+1, kViolet+1};


double function1(double *x, double *par);

double function2(double *x, double *par);
  
 string basename="2018May29liquefaction/liquid/";
 string fields[]    = {
   //"noField",

   // "K-356GK-320GA320A400_20.40.80Vcm",

   "K-445GK-400GA400A500_25.50.100Vcm",
   "K-445GK-400GA400A500_25.50.100Vcm",
   "K-445GK-400GA400A500_25.50.100Vcm",  
   "K-534GK-480GA480A600_30.60.120Vcm",  
   "K-712GK-640GA640A800_40.80.160Vcm",  
   "K-890GK-800GA800A1000_50.100.200Vcm",  
   "K-1068GK-960GA960A1200_60.120.240Vcm",  
   "K-1246GK-1120GA1120A1400_70.140.280Vcm",  
   "K-1424GK-1280GA1280A1600_80.160.320Vcm",  
   "K-1602GK-1440GA1440A1800_90.180.360Vcm",  
   "K-1780GK-1600GA1600A2000_100.200.400Vcm"


 };
 string divisions[] = {
   //"50mVdiv_13.14"

   // "100mVdiv_20.11",

   "100mVdiv_18.42",
   "100mVdiv_18.57",
   "100mVdiv_20.16",  
   "100mVdiv_20.21",
   "100mVdiv_20.26",  
   "100mVdiv_20.31",  
   "100mVdiv_20.37",  
   "200mVdiv_20.44",  
   "200mVdiv_20.49",  
   "200mVdiv_20.56",  
   "200mVdiv_21.01"
 };

 string fieldNice[] = {
   //"nofield"
   // "20-40-80 V/cm",

   "25-50-100 V/cm",
   "25-50-100 V/cm",
   "25-50-100 V/cm",

   "30-60-120 V/cm",

   "40-80-160 V/cm",

   "50-100-200 V/cm",

   "60-120-240 V/cm",

   "70-140-280 V/cm",

   "80-160-320 V/cm",

   "90-180-360 V/cm",

   "100-200-400 V/cm"
 };
double fieldInTheMiddle[] = { 50, 50, 50, 60, 80, 100, 120, 140, 160, 180, 200};


// string basename="2018May02liquefaction/liquid/";
// string fields[]={
//   "K-136GK-100GA100A180_20.40.80Vcm",
//   "K-170GK-125GA125A225_25.50.100Vcm",
//   "K-204GK-150G150GA270_30.60.120Vcm",
//   "K-204GK-150G150GA270_30.60.120Vcm",
//   "K-272GK-200GA200A360_40.80.160Vcm",
//   "K-340GK-250GA250A450_50.100.200Vcm",
//   "K-475GK-350GA350A630_70.140.280Vcm",
//   "K-475GK-350GA350A630_70.140.280Vcm",
//   "K-475GK-350GA350A630_70.140.280Vcm",
//   "K-68GK-50GA50A90_10.20.40Vcm",
// };

// string divisions[]={
//   "50mVdiv_18.53",
//   "100mVdiv_18.38",
//   "100mVdiv_18.09",
//   "50mVdiv_18.15",
//   "100mVdiv_17.49_ampSwitch",
//   "100mVdiv_17.30_ampSwitch",
//   "100mVdiv_16.54",
//   "200mVdiv_16.59",
//   "200mVdiv_17.10_ampSwitch",
//   "50mVdiv_19.11",
// };

// double fieldInTheMiddle[] = { 40, 50, 60, 60, 80, 100, 140, 140, 140,20};




string fieldsLong[]    = {

  "../liquidLongFibre/K-890GK-800GA800A1000_50.100.200Vcm"   ,
  "../liquidLongFibre/K-1068GK-960GA960A1200_60.120.240Vcm"  ,  
  "../liquidLongFibre/K-1246GK-1120GA1120A1400_70.140.280Vcm", 
  "../liquidLongFibre/K-1424GK-1280GA1280A1600_80.160.320Vcm",
  "../liquidLongFibre/K-1424GK-1280GA1280A1600_80.160.320Vcm",
  
};
string divisionsLong[] = {
  
  "100mVdiv_23.06",
  "100mVdiv_23.00",
  "200mVdiv_22.08",
  "200mVdiv_22.31",
  "200mVdiv_22.12",
};

double fieldLongInTheMiddle[] = { 100, 120, 140, 160, 160};


/*string basename="2018May02liquefaction/liquid/";
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

// string basename="2018May02liquefaction/liquidDay2/";
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

int colors[]       = {kGray+2,  kBlue+1, kCyan+1, kGreen+1,  kRed, kOrange+1, kViolet+1, kBlue+2, kBlue+3, kBlue+5, kBlue+9};

string whichAvg[4] = {"justAvg", "filteredAvg", "zeroedAvg", "fancyFilteredAvg"};
string chnamenice[2] = {"anode", "Cathode"};
double scaleY[2]     = {0.1,      0.1};
string smoothnice[2] = {"", "smooth"};

string plots[] = {"t1", "t2", "t3", "QA", "QK", "QAcorr", "QKcorr" , "R", "lifetime", "lifetime2"};

void fitAttachmentCoefficient(){
  
  
  int numFields = (sizeof(fields)/sizeof(*fields));
  
  int numFieldsLong = (sizeof(fieldsLong)/sizeof(*fieldsLong));
  int nplots    = (sizeof(plots)/sizeof(*plots));
  
  char randomtxt[20];
  
  double tK, tGK, tGA, tA, t1, t2, t3;
  double QA, QK, QAcorr, QKcorr, R, lifetime, lifetime2;

  TGraphErrors *graph   = new TGraphErrors(numFields);
  TGraphErrors *gpurity = new TGraphErrors(numFields);

  TGraphErrors *graphLong   = new TGraphErrors(numFieldsLong);
  
  double error = 0.05;
  double purity;
  
  for(int ifield=0; ifield<numFields; ifield++){
    
    
    string intxtfile = basename +  fields[ifield] + "_" + divisions[ifield] + "_lifetime.txt";
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
    
    
    graph->SetPoint(ifield, fieldInTheMiddle[ifield], lifetime*1e6);
    graph->SetPointError(ifield, 0, lifetime*1e6*error);

    purity = 1.E9*1000/(attachmentCoefficientPoly(fieldInTheMiddle[ifield])*lifetime*1E6);
    gpurity->SetPoint(ifield, fieldInTheMiddle[ifield], purity);
    gpurity->SetPointError(ifield, 0, purity*error);

  }


  for(int ifield=0; ifield<numFieldsLong; ifield++){
    
    
    string intxtfile = basename +  fieldsLong[ifield] + "_" + divisionsLong[ifield] + "_lifetime.txt";
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
    
    
    graphLong->SetPoint(ifield, fieldLongInTheMiddle[ifield], lifetime*1e6);
    graphLong->SetPointError(ifield, 0, lifetime*1e6*error);


  }
  
  
  TCanvas *c1 = new TCanvas("c1");
  
  graph->SetTitle("Lifetime");
  graph->GetYaxis()->SetTitle("Lifetime [us]");
  graph->GetXaxis()->SetTitle("Drift Field [V/cm]");
  graph->SetMarkerStyle(kCircle);
  graph->SetMarkerSize(0.8);
  graph->SetLineColor(kBlack);
  graph->SetMarkerColor(kBlack);
  graph->SetMinimum(0);
  // for (int i=1;i<=numFields;i++) graph->GetHistogram()->GetXaxis()->SetBinLabel(i,fieldNice[i-1].c_str());
  graph->Draw("Ap");
  graph->Draw("pe");

  graphLong->SetMarkerStyle(kCircle);
  graphLong->SetMarkerSize(0.3);
  graphLong->SetLineColor(kBlue);
  graphLong->SetMarkerColor(kBlue);

  graphLong->Draw("pe");
  
  double p0 =  9.06962616E10;
  double p1 = -4.07856249E7;
  double p2 = 5809.220018;


  
  TF1 *func = new TF1("func",purityAttachmentCoefficient, 50, fieldInTheMiddle[numFields-1],4);
  func->SetParameters(p0, -p1, p2, 500);
  // func->FixParameter(0, p0);
  // func->FixParameter(1, p1);
  // func->FixParameter(2, p2);
  // func->FixParameter(3, 500);
  
  func->SetParName(0, "P0");
  func->SetParName(1, "P1");
  func->SetParName(2, "P2");
  func->SetParName(3, "Purity");
  graph->Fit("func", "R");

  // TCanvas *c2 =  new TCanvas("c2");
  // func->Draw();


  //  TCanvas *c3 = new TCanvas("c3");
  
  // gpurity->SetTitle("Lifetime");
  // gpurity->GetYaxis()->SetTitle("Purity [ppt]");
  // gpurity->GetXaxis()->SetTitle("Drift Field [V/cm]");
  // gpurity->SetMarkerStyle(kCircle);
  // gpurity->SetMarkerSize(0.8);
  // gpurity->SetLineColor(kBlack);
  // gpurity->SetMarkerColor(kBlack);
  // // for (int i=1;i<=numFields;i++) gpurity->GetHistogram()->GetXaxis()->SetBinLabel(i,fieldNice[i-1].c_str());
  // gpurity->Draw("Ap");
  // gpurity->Draw("pe");

  // TCanvas *c4 = new TCanvas("c4");


  // TF1 *f1 = new TF1("f1",function1, 0, 2E-3,1);
  // f1->FixParameter(0,30);

  // TF1 *f2 = new TF1("f2",function2, 0, 2E-3,1);
  // f2->FixParameter(0,30);

  // f1->SetLineColor(kRed);
  // f2->SetLineColor(kBlue);
  
  // f1->Draw();

  // f2->Draw("same");

}


double function1(double *x, double *par){

  double v1 = ICARUSpolynomial(par[0]);
  double v2 = ICARUSpolynomial(par[0]*2);
  double v3 = ICARUSpolynomial(par[0]*4);

  double distance1 = 0.01823; // 1.8cm
  double distance2 = 0.16424; // 1.0cm
  double distance3 = 0.0985; // 1.0cm
    
  double t1 = distance1/v1;
  double t2 = distance2/v2;
  double t3 = distance3/v3;

  double tau = x[0];

  
  double R = t1/t3;

  R *= (TMath::SinH(t3/(2*tau))/TMath::SinH(t1/(2*tau)));

  R *= (TMath::Exp(-( t2 + 0.5*(t1+t3) )/tau ));

  return R;
}


double function2(double *x, double *par){

  double v1 = ICARUSpolynomial(par[0]);
  double v2 = ICARUSpolynomial(par[0]*2);
  double v3 = ICARUSpolynomial(par[0]*4);

  double distance1 = 0.01823; // 1.8cm
  double distance2 = 0.16424; // 1.0cm
  double distance3 = 0.0985; // 1.0cm
    
  double t1 = distance1/v1;
  double t2 = distance2/v2;
  double t3 = distance3/v3;

  double tau = x[0];

  
  double R = 1;
  
  R *= (TMath::Exp(-( t2 + 0.5*(t1+t3) )/tau ));
  return R;
  
}
