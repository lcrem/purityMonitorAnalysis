

string basename="2018Apr11liquefaction/Day3_allLiquid/K_GK_signals/";

string fields[] = {"noField", "K-135GK135_150Vcm", "K-405GK405_450Vcm", "K-1600GK1100_1500Vcm"};

string divisions[]={"200mVdiv",  "200mVdiv", "200mVdiv", "500mVdiv"};

string fieldNice[] = {"No field", "150 V/cm", "450 V/cm", "1500 V/cm"};

int colors[]       = {kBlack,  kGreen+1, kBlue+1, kRed};   

TGraph *smoothGraph(TGraph *g, int nnn);

void zeroBaseline(TGraph *g);

string whichAvg[4] = {"justAvg", "filteredAvg", "zeroedAvg", "fancyFilteredAvg"};
string chnamenice[2] = {"gridCathode", "Cathode"};
double scaleY[2]     = {0.1,      0.2};
string smoothnice[2] = {"", "smooth"};

void plotAll(bool isCathode=true,   bool isSmooth=true);

void plotAllCathode(){

  plotAll(true, true);
  plotAll(false, true);
  plotAll(true, false);
  plotAll(false, false);

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

    gGK[ifield] = (TGraph*)fin->Get((chnamenice[0]+"_"+whichAvg[nAvgToUse]).c_str());
    gK[ifield] = (TGraph*)fin->Get((chnamenice[1]+"_"+whichAvg[2]).c_str());
    
    if (isSmooth){
      zeroBaseline(gGK[ifield]);
      zeroBaseline(gK[ifield]);
      gGK[ifield] = smoothGraph(gGK[ifield], 100);
      gK[ifield]  = smoothGraph(gK[ifield],  20);
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
    
    leg1->AddEntry(gK[ifield], fieldNice[ifield].c_str(), "l");

    
    if (ifield==0){
      cout << "Drawing" << endl;
      gGK[ifield]->SetTitle(Form("%s;Time [s];Amplitude [V]", chnamenice[0].c_str()));
      gK[ifield]->SetTitle(Form("%s;Time [s];Amplitude [V]", chnamenice[1].c_str()));
      gGK[ifield]->GetYaxis()->SetRangeUser(-scaleY[isCathode], scaleY[isCathode]);
      gK[ifield]->GetYaxis()->SetRangeUser(-scaleY[isCathode], scaleY[isCathode]);
      if (isCathode) gK[ifield]->Draw("Al");
      else gGK[ifield]->Draw("Al");
    }else{
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
