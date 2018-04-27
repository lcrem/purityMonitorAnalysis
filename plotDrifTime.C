#include "functions.h"

string basename="2018Apr11liquefaction/Day3_allLiquid/cathodeANDanode/";
string fields[]    = {"noField", "K-22GK22_25Vcm", "K-45GK45_50Vcm", "K-135GK135_150Vcm", "K-405GK405_450Vcm", "K-900GK900_1000Vcm", "K-1600GK1100_1500Vcm"};
string divisions[] = {"200mVdiv", "100mVdiv", "100mVdiv", "100mVdiv", "100mVdiv", "100mVdiv", "100mVdiv"};
string fieldNice[] = {"No field", "25 V/cm", "50 V/cm", "150 V/cm", "450 V/cm", "1000 V/cm", "1500 V/cm"};
int  numToAverage[] = { 20, 20, 20, 20, 20, 10, 10, 10};
double scaleY[2]     = {0.1,      0.2};



int avgSomeGraphs(string filename, TGraph **g, int nmax, TGraph *fibreOut);

void getPurity(TGraph *gK, double output[4], int igraph, int ifield);

TGraph *noiseTemplatePS;

void getTheNumbers(int ifield, double dip[2], double drift[2], int numToAverage);

void plotDrifTime(){
  
  gErrorIgnoreLevel = kWarning;

  double dip[2];
  double drift[2];

  for (int ifield=0; ifield<7; ifield++){
    getTheNumbers(ifield, dip, drift, numToAverage[ifield]);
    printf("%15s:  -(%5.2f +/- %5.2f),  %5.2f +/- %5.2f \n", fields[ifield].c_str(), dip[0], dip[1], drift[0], drift[1]);
  }
  
}

void getTheNumbers(int ifield, double dip[2], double drift[2], int numToAverage){
  
  TGraph *someCathode[1000];

  string fnameOut = basename + fields[ifield] + "_FibreOut_" + divisions[ifield] + ".ch4.traces_averages.root";
  TFile *finOut = new TFile(fnameOut.c_str(), "read");
  TGraph *fibreOut = (TGraph*)finOut->Get("zeroedAvg");

  string fname = basename + fields[ifield] + "_FibreIn_" + divisions[ifield] + ".ch4.traces_averages.root";
  TFile *finIn = new TFile(fname.c_str(), "read");
  noiseTemplatePS  = (TGraph*)finIn->Get("avgPowerSpectrum");

  
  string fnameIn  = basename + fields[ifield] + "_FibreIn_" + divisions[ifield] + ".ch4.traces.root";
  int ntot        = avgSomeGraphs(fnameIn, someCathode, numToAverage, fibreOut);

  double thisoutput[4];

  TH1D *hDip   = new TH1D("hDip",   "", 100,  0, 300 );
  TH1D *hDrift = new TH1D("hDrift", "", 100,  0,  30 );

  for (int i=0; i<ntot; i++){
    getPurity(someCathode[i], thisoutput, i, ifield);
    hDip->Fill(TMath::Abs(thisoutput[0]*1e3));
    hDrift->Fill(thisoutput[3]*1e6);
    //    cout << thisoutput[0] << " " << thisoutput[3] << endl;
  }

  gStyle->SetOptStat("MR");
  TCanvas *c = new TCanvas("c");
  hDip->SetTitle(";Cathode dip [mV];Entries");
  hDip->Draw();
  c->Print(Form("plots/HistoDip_%s.png", fields[ifield].c_str()));
  dip[0] = hDip->GetMean();
  dip[1] = hDip->GetRMS();
  TCanvas *c2 = new TCanvas("c2");
  hDrift->SetTitle(";Rising time [#musec];Entries");
  hDrift->Draw();
  c2->Print(Form("plots/HistoRisingTime_%s.png", fields[ifield].c_str()));
  drift[0] = hDrift->GetMean();
  drift[1] = hDrift->GetRMS();
  
  finOut->Close();
  
  finIn->Close();

  delete hDip;
  delete hDrift;
  delete c;
  delete c2;
}



int avgSomeGraphs(string filename, TGraph **g, int nmax, TGraph *fibreOut){

  TFile *f = new TFile(filename.c_str(), "read");
  TGraph *graphs[1000];
  int count = 0;
  int ntot = 0;

  double newy[10005];
  double newx[10005];

  for (int i=0; i<1000; i++){
    // cout << i << endl;
    graphs[count] = (TGraph*) f->Get(Form("graph%i", i+1));
    if(!graphs[count]) break;
    count++;
    if (count==nmax){
      
     g[ntot] = getZeroedAverage( count, graphs );
      // //TGraph *gtemp = getFilteredAverage( count, graphs, noiseTemplatePS, 0.1 );
      g[ntot] = subtractGraphs(g[ntot], fibreOut);
      zeroBaseline(g[ntot]);
      g[ntot]  = smoothGraph(g[ntot],  50);
      
      count = 0;
      ntot++;
      // delete gtemp;
    }
  }

  f->Close();

  // cout << ntot << endl;
  return ntot;
  
}




void getPurity(TGraph *gK, double output[4], int igraph, int ifield){

  int nK      = gK->GetN();
  double *xK  = gK->GetX();
  double *yK  = gK->GetY();

  double minK  = 999999;
  double minKtime = 0;

  for (int ip=2000; ip<nK*0.75; ip++){
    if (yK[ip]<minK){
      minK     = yK[ip];
      minKtime = xK[ip];
    }
  }

  minKtime*=1e6;
  
  //  cout << "Min K and time are  " << minK  << " " << minKtime << endl;

  // double t0 = 85;
  // double lifetime = - (minKtime-t0)/TMath::Log(maxGK/TMath::Abs(minK));
  // cout << "The electrons lifetime is " << lifetime << endl;

  
  gK->SetTitle(Form(";Time [s];Amplitude [V]"));
  gK->GetYaxis()->SetRangeUser(-scaleY[1], scaleY[1]);
  

  double xmin = gK->GetXaxis()->GetBinLowEdge(0);
  double xmax = gK->GetXaxis()->GetBinUpEdge(nK);

  TLine *l0 = new TLine(xmin, 0, xmax, 0);
  l0->SetLineStyle(2);

  TF1 *func = new TF1("func",fittingFunction,0.04E-3,0.24E-3,4);

  func->SetParameters(2, 40, -0.5, 90.);
  func->SetParLimits(0, 1, 20);
  func->SetParLimits(1, 10, 100);
  func->SetParLimits(2, -100, 0);
  func->SetParLimits(3, 10, 100);
  func->SetParName(0, "#sigma");
  func->SetParName(1, "#tau_{D}");
  func->SetParName(2, "a");
  func->SetParName(3, "t_0");
  

  gStyle->SetOptFit(1);
// Fit histogram in range defined by function
  gK->Fit("func","RQ");

  // cout << func->GetParameter(0) << " " << func->GetParameter(1) << " " << func->GetParameter(2) << " " << func->GetParameter(3) <<  endl;
  double fittedMinK = func->GetMinimum(0.04E-3,0.24E-3);
  double fittedMinKtime = func->GetMinimumX(0.04E-3,0.24E-3);
  double fittedKstartTime = 0;

  double tempx, tempy;
  for (int ip=0; ip<nK; ip++){
    tempx = xK[ip];
    tempy = func->Eval(xK[ip]);
    if (tempy<0.01*fittedMinK){
      // cout << ip << " " << tempx << " " << tempy << " " << endl;
      fittedKstartTime = tempx;
      break;
    }
  }

  double fittedDriftTime = fittedMinKtime - fittedKstartTime;

  // cout << "Fitted Cathode min " << fittedMinK << endl;
  // cout << "Fitted Cathode min time " << fittedMinKtime << endl;
  // cout << "Fitted Cathode start time " << fittedKstartTime << endl;
  // cout << "Fitted drift time " << fittedDriftTime << endl;

  output[0] = fittedMinK;
  output[1] = fittedMinKtime;
  output[2] = fittedKstartTime;
  output[3] = fittedDriftTime;

  if (igraph==0){
   TCanvas *c1 = new TCanvas("c1");
   gK->Draw("Al");
   l0->Draw();
   c1->Print(Form("plots/ExampleAverage_%s.png", fields[ifield].c_str()));
   delete c1;
  }

}


