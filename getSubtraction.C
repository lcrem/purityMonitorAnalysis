#include "FFTtools.h"
#include "FFTWComplex.h"
#include "RFSignal.h"

#include "functions.h"

double xmin = 0.05E-3;
double xmax = 0.015E-3;
double ymin = -0.8;
double ymax = +0.8;

string basename = "2018Apr11liquefaction/Day3_allLiquid/cathodeANDanode/";
string whichAvg[4] = {"justAvg", "filteredAvg", "zeroedAvg", "fancyFilteredAvg"};

void getSubtraction(string fieldname, string divname){

  string outfile = basename + fieldname + "_" + divname + "_signals.root";

  cout << "Output file is " << outfile << endl;
  
  string chname[2] = {"ch3", "ch4"};
  string chnamenice[2] = {"gridCathode", "Cathode"};

  TFile *out = new TFile(outfile.c_str(), "recreate");
  
  for (int ich=1; ich<2; ich++){
    for (int iavg=0; iavg<4; iavg++){
      string f1 = basename + fieldname + "_FibreIn_" + divname + "." + chname[ich] +".traces_averages.root";
      string f2 = basename + fieldname + "_FibreOut_" + divname + "."+ chname[ich] + ".traces_averages.root";
      
      TFile *file1 = new TFile(f1.c_str(), "read");
      TFile *file2 = new TFile(f2.c_str(), "read");

      TGraph *g1 = (TGraph*)file1->Get(whichAvg[iavg].c_str());
      g1->SetName("g1");
      TGraph *g2 = (TGraph*)file2->Get(whichAvg[iavg].c_str());
      g2->SetName("g2");

      file1->Close();
      file2->Close();
      
      int N = g1->GetN();
      double *x = g1->GetX();
      double *y1 = g1->GetY();
      double *y2 = g2->GetY();
      double newy[100000];
      double newx[100000];
      for (int i=0; i<N; i++){
	newy[i] = y1[i] -  y2[i];
	newx[i] = x[i];
      }
      TGraph *gdiff = new TGraph(N, newx, newy);

      TCanvas *c = new TCanvas("c");
      c->Divide(1,2);
      c->cd(1);
      g1->SetLineColor(kBlue);
      g2->SetLineColor(kRed);
      g1->GetXaxis()->SetRangeUser(xmin, xmax);
      g1->GetYaxis()->SetRangeUser(ymin, ymax);
      g1->Draw("Al");
      g2->Draw("l");
      c->cd(2);
      gdiff->GetXaxis()->SetRangeUser(xmin, xmax);
      gdiff->GetYaxis()->SetRangeUser(ymin, ymax);  
      gdiff->Draw("Al");

      
      out->cd();
      c->Write((chnamenice[ich]+"_"+whichAvg[iavg]+"_canvas").c_str());
      gdiff->Write((chnamenice[ich]+"_"+whichAvg[iavg]).c_str());

      delete c;
    }
  }

}

