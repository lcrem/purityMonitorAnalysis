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


//string basename="2018May02liquefaction/liquid/";
//string fields[]    = {
//  //  "K-43GK-25GA25A45_10.10.20Vcm"      ,
//  "K-68GK-50GA50A90_10.20.40Vcm"     , 
//  "K-136GK-100GA100A180_20.40.80Vcm",
//  "K-170GK-125GA125A225_25.50.100Vcm", 
//  "K-204GK-150G150GA270_30.60.120Vcm", 
//  // "K-204GK-150G150GA270_30.60.120Vcm", 
//  "K-272GK-200GA200A360_40.80.160Vcm", 
//  "K-340GK-250GA250A450_50.100.200Vcm",
//  // "K-475GK-350GA350A630_70.140.280Vcm",
//  // "K-475GK-350GA350A630_70.140.280Vcm",
//  "K-475GK-350GA350A630_70.140.280Vcm"
//};
//string divisions[] = {
//  //  "50mVdiv_19.35",
//  "50mVdiv_19.11",
//  "50mVdiv_18.53",
//  "100mVdiv_18.38",
//  "50mVdiv_18.15",
//  // "100mVdiv_18.09",
//  "100mVdiv_17.49_ampSwitch",
//  "100mVdiv_17.30_ampSwitch",
//  // "100mVdiv_16.54",
//  // "200mVdiv_16.59",
//  "200mVdiv_17.10_ampSwitch"
//};
//
//string fieldNice[] = {
//  //  "10-10-20 V/m",
//  "10-20-40 V/m",
//  "20-40-80 V/m",
//  "25-50-100 V/m",
//  "30-60-120 V/m",
//  "30-60-120 V/m",
//  "40-80-160 V/m",
//  "50-100-200 V/m",
//  // "70-140-280 V/m",
//  // "70-140-280 V/m",
//  "70-140-280 V/m"
//};

////////////////////////////////////////////////////////////

string basename="2018May02liquefaction/liquidDay2/";
string fields[]    = {
  "K-680GK-500GA500A900_100.200.400cm",
  "K-680GK-500GA500A900_100.200.400cm",
  "K-680GK-500GA500A900_100.200.400cm",
  "K-680GK-500GA500A900_100.200.400cm",
  "K-612GK-450GA450A810_90.180.360cm",
  "K-544GK-400GA400A720_80.160.320cm",
  "K-476GK-350GA350A630_70.140.280cm",
  "K-476GK-350GA350A630_70.140.280cm",
  "K-476GK-350GA350A630_70.140.280cm",
  "K-476GK-350GA350A630_70.140.280cm",
  "K-476GK-350GA350A630_70.140.280cm",
  "K-340GK-250GA250A450_50.100.200cm",
  "K-272GK-200GA200A360_40.80.160cm",
  "K-680GK-500GA500A900_100.200.400cm",
  "K-1530GK-1125GA1125A2025_225.450.900cm",
  "K-1360GK-1000GA1000A1800_200.400.800cm",
  "K-1020GK-750GA750A1350_150.300.600cm"
  
};
string divisions[] = {
  "100mVdiv_16.43",
  "100mVdiv_16.25",
  "100mVdiv_15.51",
  "100mVdiv_15.26",
  "100mVdiv_13.23",
  "100mVdiv_13.38",
  "100mVdiv_16.35",
  "100mVdiv_16.13",
  "100mVdiv_16.02",
  "100mVdiv_15.13",
  "100mVdiv_13.50",
  "100mVdiv_14.07",
  "100mVdiv_14.20",
  "100mVdiv_13.10",
  "200mVdiv_12.34",
  "1Vdiv_12.13",
  "200mVdiv_12.46"
};

string fieldNice[] = {
  "100-200-400 V/cm",
  "100-200-400 V/cm",
  "100-200-400 V/cm",
  "100-200-400 V/cm",
  
  "90-180-360 V/cm",
  "80-160-320 V/cm",
  "70-140-280 V/cm",
  "70-140-280 V/cm",
  "70-140-280 V/cm",
  "70-140-280 V/cm",
  "70-140-280 V/cm",
  
  "50-100-200 V/cm",
  "40-80-160 V/cm",
  "100-200-400 V/cm",
  
  "225-450-900 V/cm",
  "200-400-800 V/cm",
  "150-300-600 V/cm"
};

///////////////////////////////////////////////////

int colors[]       = {kGray+2,  kBlue+1, kCyan+1, kGreen+1,  kRed, kOrange+1, kViolet+1, kBlue+2, kBlue+3, kBlue+5, kBlue+9};

string whichAvg[4] = {"justAvg", "filteredAvg", "zeroedAvg", "fancyFilteredAvg"};
string chnamenice[2] = {"anode", "Cathode"};
double scaleY[2]     = {0.1,      0.1};
string smoothnice[2] = {"", "smooth"};

void plotOne(TGraph *gK, int fieldIndex, bool isCathode);

void plotTwo(TGraph *gA, TGraph *gK, int fieldIndex);

void plotAllCathode(){

  bool isSmooth = true;
  //gErrorIgnoreLevel = kWarning;
  
  int nAvgToUse=2;
  
  int numFields = (sizeof(fields)/sizeof(*fields));

  TGraph *gA[10];
  TGraph *gK[10];  
  
  for(int ifield=0; ifield<numFields; ifield++){
    
    string filename = basename + fields[ifield] + "_" + divisions[ifield] + "_signals.root";

    TFile *fin = new TFile(filename.c_str(), "read");

    gA[ifield] = (TGraph*)fin->Get((chnamenice[0]+"_"+whichAvg[nAvgToUse]+"_smoothed").c_str());
    gK[ifield] = (TGraph*)fin->Get((chnamenice[1]+"_"+whichAvg[nAvgToUse]+"_smoothed").c_str());
    
    
    fin->Close();
  }
  
  TCanvas *c1 = new TCanvas("c1");
  
  TCanvas *c2 = new TCanvas("c2");
  
  int icolor = 52;
  
  TLegend *leg1 = new TLegend(0.6, 0.55, 0.89, 0.89);

  char randomtxt[20];
  
  double tK, tGK, tGA, tA, t1, t2, t3;
  double QA, QK, R, purity, purity2;
  
  
  for(int ifield=0; ifield<numFields; ifield++){
    
    
    string intxtfile = basename +  fields[ifield] + "_" + divisions[ifield] + "_purity.txt";

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
    f >> randomtxt >> randomtxt >> R;
    f >> randomtxt >> randomtxt >> purity;
    f >> randomtxt >> randomtxt >> purity2;

    printf("tK     : %12.4e \n",  tK  );
    printf("tGK    : %12.4e \n",  tGK );
    printf("tGA    : %12.4e \n",  tGA );
    printf("tA     : %12.4e \n",  tA  );
    printf("t1     : %12.4e \n",  t1  );
    printf("t2     : %12.4e \n",  t2  );
    printf("t3     : %12.4e \n",  t3  );
    printf("QA     : %12.4e \n",  QA  );
    printf("QK     : %12.4e \n",  QK  );
    printf("R      : %12.4e \n",  R   );
    printf("purity : %12.4e \n",  purity  );
    printf("purity2: %12.4e \n",  purity2 );

    f.close();

    
    gA[ifield]->SetLineColor(colors[ifield]) ;
    gK[ifield]->SetLineColor(colors[ifield]) ;

    gA[ifield]->SetLineWidth(2);
    gK[ifield]->SetLineWidth(2);
    
    gA[ifield]->GetYaxis()->SetRangeUser(-0.07  , 0.07);
    gA[ifield]->GetXaxis()->SetRangeUser(0.00005, 0.0015);
    gK[ifield]->GetYaxis()->SetRangeUser(-0.07  , 0.07);
    gK[ifield]->GetXaxis()->SetRangeUser(-0.00005, 0.001);
      
      
    if (isSmooth){
      plotOne(gA[ifield], ifield, false);
      plotOne(gK[ifield], ifield, true);
      plotTwo(gA[ifield], gK[ifield], ifield);
      
    }
    
    leg1->AddEntry(gA[ifield], fieldNice[ifield].c_str(), "l");

    if (ifield==0){
      c1->cd();
      gA[ifield]->SetTitle(Form("%s;Time [s];Amplitude [V]", chnamenice[0].c_str()));
      gA[ifield]->Draw("Al");
      c2->cd();
      gK[ifield]->SetTitle(Form("%s;Time [s];Amplitude [V]", chnamenice[1].c_str()));
      gK[ifield]->Draw("Al");
    }else{
      c1->cd();
      gA[ifield]->Draw("l");
      c2->cd();
      gK[ifield]->Draw("l");
    }
    icolor+=10;


  }
  c1->cd();
  leg1->Draw();
  c2->cd();
  leg1->Draw();


  c1->Print((basename+"/plots/Allfields_"+chnamenice[0]+smoothnice[isSmooth]+".png").c_str());
  c1->Print((basename+"/plots/Allfields_"+chnamenice[0]+smoothnice[isSmooth]+".pdf").c_str());
  c1->Print((basename+"/plots/Allfields_"+chnamenice[0]+smoothnice[isSmooth]+".C"  ).c_str());

  c2->Print((basename+"/plots/Allfields_"+chnamenice[1]+smoothnice[isSmooth]+".png").c_str());
  c2->Print((basename+"/plots/Allfields_"+chnamenice[1]+smoothnice[isSmooth]+".pdf").c_str());
  c2->Print((basename+"/plots/Allfields_"+chnamenice[1]+smoothnice[isSmooth]+".C"  ).c_str());
 
}


void plotOne(TGraph *gK, int ifield, bool isCathode){

  int nK      = gK->GetN();
  double *xK  = gK->GetX();
  double *yK  = gK->GetY();


  TCanvas *c3 = new TCanvas("c3");
  
  gK->SetTitle(Form("%s;Time [s];Amplitude [V]", fieldNice[ifield].c_str()));
  gK->Draw("Al");
  //  gA->Draw("l");

  double xmin = gK->GetXaxis()->GetBinLowEdge(0);
  double xmax = gK->GetXaxis()->GetBinUpEdge(nK);

  TLine *l0 = new TLine(xmin, 0, xmax, 0);
  l0->SetLineStyle(2);
  l0->Draw();
  
  TF1 *func = new TF1("func",fittingFunction,xmin, 0.001,4);

  if (isCathode){
    func->SetParameters(2, 40, -0.1, 400);
    func->SetParLimits(0, -100, 100);
    func->SetParLimits(1, 1, 100);
    func->SetParLimits(2, -1000, 0);
    func->SetParLimits(3, 0.1, 1000);
  } else{
    func->SetParameters(2, 40, +0.1, 400);
    func->SetParLimits(0, -100, 100);
    func->SetParLimits(1, 1, 1000);
    func->SetParLimits(2, 0, 1000);
    func->SetParLimits(3, 0.1, 1000);
  }
  func->SetParName(0, "#sigma");
  func->SetParName(1, "#tau_{D}");
  func->SetParName(2, "a");
  func->SetParName(3, "t_0");
  

  bool fit = false;
  double fittedK;
  double fittedKtime;
  double fittedKstartTime = 0;

  // if (fit){
    

  //   gStyle->SetOptFit(1);
  //   // Fit histogram in range defined by function
  //   gK->Fit("func","RQ");

  //   cout << "I used a fitting function and these are the parameters I got " << endl;
  //   cout << func->GetParameter(0) << " " << func->GetParameter(1) << " " << func->GetParameter(2) << " " << func->GetParameter(3) <<  endl;
    
  //   if (isCathode){
  //     fittedKtime = func->GetMinimumX(xmin, xmax);
  //     fittedK = func->GetMinimum(xmin, xmax);
  //   } else{
  //     fittedKtime = func->GetMaximumX(xmin, xmax);
  //     fittedK = func->GetMaximum(xmin, xmax);
  //   }
  //   double tempx, tempy;
  //   for (int ip=0; ip<nK; ip++){
  //     tempx = xK[ip];
  //     tempy = func->Eval(xK[ip]);
  //     if (tempy>0.01*TMath::Abs(fittedK)){
  // 	// cout << ip << " " << tempx << " " << tempy << " " << endl;
  // 	fittedKstartTime = tempx;
  //     break;
  //     }
  //   }

    
  // } else {

  //   // int loc;
  //   // if (isCathode){
  //   //   loc = TMath::LocMin(nK,yK);
  //   // } else{
  //   //   loc = TMath::LocMax(nK,yK);
  //   // }
  //   // fittedKtime = xK[loc];
  //   // fittedK = yK[loc];
  //   // cout << chnamenice[isCathode] << " " << loc << " " << fittedKtime << " " << fittedK << endl;

  //   // TF1 *straightLine = new TF1("straightLine", "[0]+[1]*x");
  //   // gK->Fit("straightLine", "RQ0", "", fittedKtime-0.01e-3, fittedKtime);
  //   // fittedKstartTime = -straightLine->GetParameter(0)/straightLine->GetParameter(1);
    
  // }
  

  
  // double fittedDriftTime = fittedKtime - fittedKstartTime;

  // cout << "Fitted Cathode  " << fittedK << endl;
  // cout << "Fitted Cathode  time " << fittedKtime << endl;
  // cout << "Fitted Cathode start time " << fittedKstartTime << endl;
  // cout << "Fitted drift time " << fittedDriftTime << endl;


  // numbers[0] = fittedK;
  // numbers[1] = fittedDriftTime;
  
  // TLine *lmin = new TLine(fittedKstartTime, -scaleY[1], fittedKstartTime, +scaleY[1]);
  // lmin->SetLineStyle(2);
  // lmin->Draw();

  // TLine *lmax = new TLine(fittedKtime, -scaleY[1], fittedKtime, +scaleY[1]);
  // lmax->SetLineStyle(2);
  // lmax->Draw();

  
  
  c3->Print(Form("%s/plots/Field%s_%s.png", basename.c_str(), chnamenice[isCathode].c_str(), fields[ifield].c_str()));
  c3->Print(Form("%s/plots/Field%s_%s.pdf", basename.c_str(), chnamenice[isCathode].c_str(), fields[ifield].c_str()));
  c3->Print(Form("%s/plots/Field%s_%s.C",   basename.c_str(), chnamenice[isCathode].c_str(), fields[ifield].c_str()));

  delete c3;
  delete func;
  delete l0;
  
}






void plotTwo(TGraph *gA, TGraph *gK, int ifield){


  TCanvas *c3 = new TCanvas("c3");
  
  gK->SetTitle(Form("%s;Time [s];Amplitude [V]", fieldNice[ifield].c_str()));
  gK->Draw("Al");
  gA->Draw("l");

  double xmin = gK->GetXaxis()->GetBinLowEdge(0);
  double xmax = gK->GetXaxis()->GetBinUpEdge(gK->GetN());

  TLine *l0 = new TLine(xmin, 0, xmax, 0);
  l0->SetLineStyle(2);
  l0->Draw();
  
  
  
  // double fittedDriftTime = fittedKtime - fittedKstartTime;

  // cout << "Fitted Cathode  " << fittedK << endl;
  // cout << "Fitted Cathode  time " << fittedKtime << endl;
  // cout << "Fitted Cathode start time " << fittedKstartTime << endl;
  // cout << "Fitted drift time " << fittedDriftTime << endl;


  // numbers[0] = fittedK;
  // numbers[1] = fittedDriftTime;
  
  // TLine *lmin = new TLine(fittedKstartTime, -scaleY[1], fittedKstartTime, +scaleY[1]);
  // lmin->SetLineStyle(2);
  // lmin->Draw();

  // TLine *lmax = new TLine(fittedKtime, -scaleY[1], fittedKtime, +scaleY[1]);
  // lmax->SetLineStyle(2);
  // lmax->Draw();

  
  
  c3->Print(Form("%s/plots/Field_%s.png", basename.c_str(), fields[ifield].c_str()));
  c3->Print(Form("%s/plots/Field_%s.pdf", basename.c_str(), fields[ifield].c_str()));
  c3->Print(Form("%s/plots/Field_%s.C",   basename.c_str(), fields[ifield].c_str()));

  delete c3;
  delete l0;
  
}





