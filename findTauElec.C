#include "FFTtools.h"
#include "FFTWComplex.h"
#include "RFSignal.h"

#include "functions.h"

double xmin = 0.05E-3;
double xmax = 0.015E-3;
double ymin = -0.8;
double ymax = +0.8;

//string basename = "2018Apr11liquefaction/Day3_allLiquid/cathodeANDanode/";
string whichAvg[4] = {"justAvg", "filteredAvg", "zeroedAvg", "fancyFilteredAvg"};

void findTauElec(string basename, string fieldname, string divname, string preamp){

  string outfile    = basename + fieldname + "_" + divname + "_signals.root";  
  string outcanvas    = basename + fieldname + "_" + divname + "_tauelec";
  
  cout << "Output file is " << outfile << endl;
  
  string chname[2] = {"ch3", "ch4"};
  string chnamenice[2] = {"anode", "Cathode"};

  double timedelay=0;

  string stimedelay = basename + fieldname + "_FibreIn_" + divname + ".ch1.traces_averages.root";
  TFile *ftimedelay = new TFile (stimedelay.c_str(), "read");
  TGraph *gtimedelay = (TGraph*)ftimedelay->Get("justAvg");
  double *xTimeDelay = gtimedelay->GetX();
  double *yTimeDelay = gtimedelay->GetY();
  double baseY = yTimeDelay[0];
  double step = 2.5;
  for (int ip=0; ip<gtimedelay->GetN(); ip++){
    if (yTimeDelay[ip]>(baseY+step)){
      timedelay = xTimeDelay[ip];
      break;
    }
  }
  ftimedelay->Close();
  cout << "The time delay is " << timedelay << endl;
  
  double newy[20000];
  double newx[20000];
  
  TFile *out = new TFile(outfile.c_str(), "recreate");


  double finalNumbers[2][3]; // [0 anode, 1 cathode] [0 amplitude, 1 start time, 2 peak time]
  
  for (int iavg=0; iavg<3; iavg++){
    for (int ich=1; ich<2; ich++){
      string f1 = basename + fieldname + "_FibreIn_" + divname + "." + chname[ich] +".traces_averages.root";
      string f2 = basename + fieldname + "_FibreOut_" + divname + "."+ chname[ich] + ".traces_averages.root";
      
      TFile *file1 = new TFile(f1.c_str(), "read");
      TFile *file2 = new TFile(f2.c_str(), "read");

      TGraph *g1 = (TGraph*)file1->Get(whichAvg[iavg].c_str());
      g1->SetName("g1");

      TGraph *g2;
      bool isFibreOut;

      g2 = (TGraph*)file2->Get(whichAvg[iavg].c_str());
      g2->SetName("g2");
      isFibreOut = true;
      
      file1->Close();
      file2->Close();

      g1 = FFTtools::translateGraph(g1, -timedelay);
      g2 = FFTtools::translateGraph(g2, -timedelay);

      
      int N = g1->GetN();
      double *x = g1->GetX();
      double *y1 = g1->GetY();
      double *y2 = g2->GetY();
      for (int i=0; i<N; i++){
	if (isFibreOut) newy[i] = y1[i] -  y2[i];	
	else newy[i] = y1[i];
	newx[i] = x[i];
      }
      
      TGraph *gdiff = new TGraph(N, newx, newy);

      // TGraph *gdiff = subtractNoise(g1, g2);
      
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
      
      // gdiff = FFTtools::translateGraph(gdiff, -gdiff->GetX()[0]);
      zeroBaseline(gdiff);
      // removeGarbage(gdiff);
      TGraph *gdiff2 = smoothGraph(gdiff, 10);

      gdiff2->Write((chnamenice[ich]+"_"+whichAvg[iavg]+"_smoothed").c_str());

      if (iavg==2){

	gStyle->SetOptFit(1);
	gStyle->SetStatY(0.4);                
	gStyle->SetStatX(0.9);                
	gStyle->SetStatW(0.2);                
	gStyle->SetStatH(0.2);
 
	TCanvas *ctemp = new TCanvas ("ctemp");
	gdiff2->SetTitle(Form("%s;Time (s);Amplitude (V)", preamp.c_str()));
	gdiff2->Draw("Al");
	// TF1 *func = new TF1("func",greenFunction2,-0.1E-3,0.7E-3,4);

	// func->SetParameters(0.01, 0.1, 500, 6);
	// func->SetParName(0, "Q (pC)");
	// //	func->SetParLimits(0, 1e-6, 2e-6);
	// func->SetParName(1, "C (pF)");
	// // func->FixParameter(1, 0.1);
	// func->SetParLimits(1, 0.1, 0.2);
	// func->SetParName(2, "R (M#Omega)");
	// //func->SetParLimits(2, 490, 510);
	// func->SetParName(3, "t_0 (#mus)");

	TF1 *func = new TF1("func",greenFunction,-0.1E-3,0.7E-3,4);

	func->SetParameters(1, 0.1, 50, 0);
	func->SetParName(0, "Q (pC)");
	func->SetParName(1, "Gain ");
	// func->SetParLimits(1, 0.1, 0.2);
	func->SetParName(2, "Tau (#mus)");
	//	func->SetParLimits(2, 490, 510);
	func->SetParName(3, "t_0 (#mus)");
	func->FixParameter(0, 1);
	// func->FixParameter(2, 500);
	


	gdiff2->Fit("func","R");
	func->Draw("same");

	double tauelec = func->GetParameter(1)*func->GetParameter(2);

	ctemp->Update();
	TLatex *text = new TLatex();
	text->DrawTextNDC(0.5, 0.5, Form("#tau_{elec} = %3.2f #mus", tauelec));
	
	ctemp->Print((outcanvas+".png").c_str());
	ctemp->Print((outcanvas+".pdf").c_str());
	ctemp->Print((outcanvas+".C").c_str());
	
      }
      
	
      
      
      delete gdiff;
      // delete gdiff2;
      delete c;


      
      file1->Close();
      file2->Close();
    }
  }

}


