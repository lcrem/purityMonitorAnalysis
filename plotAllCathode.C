

// string basename="2018Apr11liquefaction/Day3_allLiquid/K_GK_signals/";
// string fields[] = {"noField", "K-45GK45_50Vcm", "K-135GK135_150Vcm", "K-405GK405_450Vcm", "K-1600GK1100_1500Vcm"};
// string divisions[]={"200mVdiv", "200mVdiv", "200mVdiv", "200mVdiv", "500mVdiv"};
// string fieldNice[] = {"No field", "50 V/cm", "150 V/cm", "450 V/cm", "1500 V/cm"};
//int colors[]       = {kBlack,  kGreen+1, kBlue+1, kRed, kViolet};   

string basename="2018Apr11liquefaction/Day3_allLiquid/cathodeANDanode/";
string fields[]    = {"K-22GK22_25Vcm", "K-45GK45_50Vcm", "K-135GK135_150Vcm", "K-405GK405_450Vcm", "K-900GK900_1000Vcm", "K-1600GK1100_1500Vcm"};
string divisions[] = {"100mVdiv", "100mVdiv", "100mVdiv", "100mVdiv", "100mVdiv", "100mVdiv"};
string fieldNice[] = {"25 V/cm", "50 V/cm", "150 V/cm", "450 V/cm", "1000 V/cm", "1500 V/cm"};
int colors[]       = {kBlack,  kGreen+1, kBlue+1, kRed, kViolet, kCyan};   


TGraph *smoothGraph(TGraph *g, int nnn);

void zeroBaseline(TGraph *g);

string whichAvg[4] = {"justAvg", "filteredAvg", "zeroedAvg", "fancyFilteredAvg"};
string chnamenice[2] = {"gridCathode", "Cathode"};
double scaleY[2]     = {0.1,      0.2};
string smoothnice[2] = {"", "smooth"};

void plotAll(bool isCathode=true,   bool isSmooth=true);

void getPurity(TGraph *gGK, TGraph *gK, int fieldIndex);

void plotAllCathode(){

  plotAll(true, true);
  // plotAll(false, true);
  // plotAll(true, false);
  // plotAll(false, false);

}

void plotAll(bool isCathode=true,
	     bool isSmooth=true){
  int nAvgToUse=1;
  
  int numFields = (sizeof(fields)/sizeof(*fields));

  TGraph *gK[10];
  TGraph *gGK[10];

  
  for(int ifield=0; ifield<numFields; ifield++){
    
    string filename = basename + fields[ifield] + "_" + divisions[ifield] + "_signals.root";

    TFile *fin = new TFile(filename.c_str(), "read");

    if (!isCathode) gGK[ifield] = (TGraph*)fin->Get((chnamenice[0]+"_"+whichAvg[nAvgToUse]).c_str());
    else  gGK[ifield] = (TGraph*)fin->Get((chnamenice[1]+"_"+whichAvg[nAvgToUse]).c_str());
    gK[ifield] = (TGraph*)fin->Get((chnamenice[1]+"_"+whichAvg[2]).c_str());
    
    if (isSmooth){
      zeroBaseline(gGK[ifield]);
      zeroBaseline(gK[ifield]);
      gGK[ifield] = smoothGraph(gGK[ifield], 100);
      gK[ifield]  = smoothGraph(gK[ifield],  50);
    }
    
    fin->Close();
  }
  cout << "Got all graphs" << endl;
  
  TCanvas *c = new TCanvas("c");
  
  int icolor = 52;
  
  
  TLegend *leg1 = new TLegend(0.6, 0.6, 0.89, 0.89);
  
  for(int ifield=0; ifield<numFields; ifield++){

    gGK[ifield]->SetLineColor(colors[ifield]) ;
    gK[ifield]->SetLineColor(colors[ifield]) ;

    gGK[ifield]->SetLineWidth(2);
    gK[ifield]->SetLineWidth(2);
    
    gGK[ifield]->GetYaxis()->SetRangeUser(-scaleY[0], scaleY[0]);
    gK[ifield]->GetYaxis()->SetRangeUser(-scaleY[1], scaleY[1]);
    
    leg1->AddEntry(gK[ifield], fieldNice[ifield].c_str(), "l");


    if (isSmooth){
      cout << "Getting purity for field : " << fieldNice[ifield] << endl;
      getPurity(gGK[ifield], gK[ifield], ifield);
    }
    
    if (ifield==0){
      c->cd();
      gGK[ifield]->SetTitle(Form("%s;Time [s];Amplitude [V]", chnamenice[0].c_str()));
      gK[ifield]->SetTitle(Form("%s;Time [s];Amplitude [V]", chnamenice[1].c_str()));
      if (isCathode) gK[ifield]->Draw("Al");
      else gGK[ifield]->Draw("Al");
    }else{
      c->cd();
      if (isCathode) gK[ifield]->Draw("l");
      else gGK[ifield]->Draw("l");
    }
    icolor+=10;
  }
  
  leg1->Draw();


  c->Print(("plots/Allfields_"+chnamenice[isCathode]+smoothnice[isSmooth]+".png").c_str());
  c->Print(("plots/Allfields_"+chnamenice[isCathode]+smoothnice[isSmooth]+".pdf").c_str());
  c->Print(("plots/Allfields_"+chnamenice[isCathode]+smoothnice[isSmooth]+".C"  ).c_str());
  
}


TGraph *smoothGraph(TGraph *g, int nnn){

  int n = g->GetN();
  double *x = g->GetX();
  double *y = g->GetY();
  double newy[1000000];
  
  for (int i=0; i<nnn; i++){
    newy[i]=y[i];
    newy[n-i]=y[n-i];
  }

  for (int i=nnn; i<(n-nnn); i++){
    newy[i]=0;
    
    for (int j=i-nnn; j<i+nnn; j++){
      newy[i]+=(y[j]/(nnn+nnn));
    }
  }

  
  cout << "Here fine" << endl;
  TGraph *gnew = new TGraph(n, x, newy);

  return gnew;

}


void zeroBaseline(TGraph *g){


  double *y = g->GetY();
  Double_t meanVal=TMath::Mean(2000,g->GetY());
  for(int ip=0;ip<g->GetN();ip++) {
    y[ip] -=  meanVal;
  }

}

void getPurity(TGraph *gGK, TGraph *gK, int ifield){

  int nGK     = gGK->GetN();
  double *xGK = gGK->GetX();
  double *yGK = gGK->GetY();

  int preTrigger = nGK*45./200.;

  int nK      = gK->GetN();
  double *xK  = gK->GetX();
  double *yK  = gK->GetY();

  double maxGK     = 0;
  double maxGKtime = 0;
  double minK  = 999999;
  double minKtime = 0;
  for (int ip=preTrigger; ip<nGK*0.75; ip++){
    if (yGK[ip]>maxGK){
      maxGK     = yGK[ip];
      maxGKtime = xGK[ip];
    }
  }
  for (int ip=preTrigger; ip<nK*0.75; ip++){
    if (yK[ip]<minK){
      minK     = yK[ip];
      minKtime = xK[ip];
    }
  }

  maxGKtime*=1e6;
  minKtime*=1e6;
  
  cout << "Max GK and time are " << maxGK << " " << maxGKtime << endl;
  cout << "Min K and time are  " << minK  << " " << minKtime << endl;

  double t0 = 85;
  double lifetime = - (minKtime-t0)/TMath::Log(maxGK/TMath::Abs(minK));
  cout << "The electrons lifetime is " << lifetime << endl;

  TCanvas *c1 = new TCanvas("c1");
  
  gK->SetTitle(Form("%s;Time [s];Amplitude [V]", fieldNice[ifield].c_str()));
  gK->Draw("Al");
  gGK->Draw("l");

  double xmin = gK->GetXaxis()->GetBinLowEdge(0);
  double xmax = gK->GetXaxis()->GetBinUpEdge(nK);

  TLine *l0 = new TLine(xmin, 0, xmax, 0);
  l0->SetLineStyle(2);
  l0->Draw();

  TLine *lmin = new TLine(xmin, minK, xmax, minK);
  lmin->SetLineStyle(2);
  lmin->Draw();

  TLine *lmax = new TLine(xmin, maxGK, xmax, maxGK);
  lmax->SetLineStyle(2);
  lmax->Draw();
  
  c1->Print(Form("plots/Field_%s.png", fields[ifield].c_str()));
  c1->Print(Form("plots/Field_%s.pdf", fields[ifield].c_str()));
  c1->Print(Form("plots/Field_%s.C", fields[ifield].c_str()));
  
  
}
