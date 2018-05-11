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


double getCorrection(double fittedDriftTime, double tauelec, double taulife);

void getSubtraction(string basename, string fieldname, string divname){

  string outfile    = basename + fieldname + "_" + divname + "_signals.root";
  string outtxtfile = basename + fieldname + "_" + divname + "_purity.txt";
  
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
  
  for (int iavg=0; iavg<4; iavg++){
    for (int ich=0; ich<2; ich++){
      string f1 = basename + fieldname + "_FibreIn_" + divname + "." + chname[ich] +".traces_averages.root";
      string f2 = basename + fieldname + "_FibreOut_" + divname + "."+ chname[ich] + ".traces_averages.root";
      
      TFile *file1 = new TFile(f1.c_str(), "read");
      TFile *file2 = new TFile(f2.c_str(), "read");

      TGraph *g1 = (TGraph*)file1->Get(whichAvg[iavg].c_str());
      g1->SetName("g1");

      TGraph *g2;
      bool isFibreOut;
      if (file2->IsZombie()){
	string tempfilename = "2018May02liquefaction/liquid/K-475GK-350GA350A630_70.140.280Vcm_FibreOut_200mVdiv_16.59."+ chname[ich] +".traces_averages.root";
	TFile *ftemp = new TFile(tempfilename.c_str(), "read");
	g2 = (TGraph*)ftemp->Get(whichAvg[iavg].c_str());
	g2->SetName("g2");
	ftemp->Close();
	isFibreOut = false;
      } else {
	g2 = (TGraph*)file2->Get(whichAvg[iavg].c_str());
	g2->SetName("g2");
	isFibreOut = true;
      }
      
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
      
	int nK      = gdiff2->GetN();
	double *xK  = gdiff2->GetX();
	double *yK  = gdiff2->GetY();

  
	double fittedK;
	double fittedKtime;
	double fittedKstartTime = 0;

	int loc;
	double peak;
	if (ich==1){
	  peak = 99999;
	  // loc = TMath::LocMin(nK,yK);
	  for (int ip=nK-2; ip>0; ip--){
	    if (xK[ip]<0) break;
	    if (yK[ip]<peak){
	      peak = yK[ip];
	      loc = ip;
	    }
	  }
	  fittedKtime = xK[loc];
	  fittedK = yK[loc];

	  fittedKstartTime = 0.;
	} else{
	  //loc = TMath::LocMax(nK,yK);
	  peak = -99999.;
	  for (int ip=nK-2; ip>0; ip--){
	    if (xK[ip]<0.05E-3) break;
	    if (yK[ip]>peak){
	      peak = yK[ip];
	      loc = ip;
	    }
	  }
	  
	  fittedKtime = xK[loc];
	  fittedK = yK[loc];
	  
	  TF1 *straightLine = new TF1("straightLine", "[0]+[1]*x");
	  gdiff2->Fit("straightLine", "RQ0", "", fittedKtime-0.05e-3, fittedKtime);
	  fittedKstartTime = -straightLine->GetParameter(0)/straightLine->GetParameter(1);
	  
	  delete straightLine;
	}
	// cout << chnamenice[ich] << " " << loc << " " << fittedKtime << " " << fittedK << endl;
	
    
	double fittedDriftTime = fittedKtime - fittedKstartTime;

	// cout << "Fitted Cathode  " << fittedK << endl;
	// cout << "Fitted Cathode  time " << fittedKtime << endl;
	// cout << "Fitted Cathode start time " << fittedKstartTime << endl;
	// cout << "Fitted drift time " << fittedDriftTime << endl;
	


	// TF1 *exponential;
	// if (ich==0)  exponential = new TF1("exponential", "exp([0]+[1]*x[0])");
	// else exponential = new TF1("exponential", "-exp([0]+[1]*x[0])");
	// gdiff2->Fit("exponential", "RQ", "", fittedKtime, gdiff2->GetX()[gdiff2->GetN()-1]);

	// double tauelec = -1./exponential->GetParameter(1);
	// cout << "Tau electronics : " << tauelec << endl;

	// double taulife = 0.003;

	// double correction = (1.-exp(-(fittedDriftTime)*(1./tauelec + 1./taulife)))/(fittedDriftTime*(1./tauelec + 1./taulife));

	// cout << "Correction : " << correction << endl;

	// if (ich==0) fittedK*=correction;
	// else fittedK/=correction;
	
	finalNumbers[ich][0] = fittedK;
	finalNumbers[ich][1] = fittedKstartTime;
	finalNumbers[ich][2] = fittedKtime;
	
      }
      
      delete gdiff;
      delete gdiff2;
      delete c;


      
      file1->Close();
      file2->Close();
    }
  }

  double tK, tGK, tGA, tA, t1, t2, t3;
  double QA, QK, R, purity, purity2;
  tK  = finalNumbers[1][1];
  tGK = finalNumbers[1][2];
  tA  = finalNumbers[0][2];
  tGA = finalNumbers[0][1];
  t1 = tGK - tK;
  t2 = tGA - tGK;
  t3 = tA - tGA;


  if (tK<tGK && tGK<tGA && tGA < tA){

  } else {
    cout << "THESE NUMBERS DON'T MAKE SENSE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    
  }
  
  QA = finalNumbers[0][0];
  QK = finalNumbers[1][0];

  double tauelec = 0.00012;
  double taulife = 0.003;
  double Kcorrection;
  double Acorrection;
  double newQK = QK;
  double newQA = QA;
  
  if (R<1){

    int count = 0;
    while(1){
      Kcorrection = getCorrection(t1, tauelec, taulife);
      Acorrection = getCorrection(t3, tauelec, taulife);

      newQK = QK*Kcorrection;
      newQA = QA*Acorrection;
    
      R =  TMath::Abs(newQA/newQK);

      purity = (1/TMath::Abs(TMath::Log(R)))*(t2 + 0.5*(t1+t3));

      if (TMath::Abs(purity-taulife)<0.0000001) break;
      taulife=purity;
      count++;
    }
    cout << "Number of iterations : " << count << endl;
    
    TF1 f ("Purity function", "([1]/[3])*(TMath::SinH([3]/(2*x))/TMath::SinH([1]/(2*x)))*TMath::Exp(-([2]+0.5*([1]+[3]))) - [0]", -0.1, 0.1);
    f.SetParameters(R, t1, t2, t3);

    ROOT::Math::WrappedTF1 wf1(f);
 
    // Create the Integrator
    ROOT::Math::BrentRootFinder brf;
 
    // Set parameters of the method
    brf.SetFunction( wf1, -0.1, 0.1 );
    brf.Solve();

    purity2 =  brf.Root() ;
  } else {
    purity = purity2 = 0;
  }
  
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

  FILE * pFile;

  cout << "Writing these info to " << outtxtfile << endl;
  
  pFile = fopen (outtxtfile.c_str(),"w");

  fprintf(pFile, "tK     : %12.4e \n",  tK  );
  fprintf(pFile, "tGK    : %12.4e \n",  tGK );
  fprintf(pFile, "tGA    : %12.4e \n",  tGA );
  fprintf(pFile, "tA     : %12.4e \n",  tA  );
  fprintf(pFile, "t1     : %12.4e \n",  t1  );
  fprintf(pFile, "t2     : %12.4e \n",  t2  );
  fprintf(pFile, "t3     : %12.4e \n",  t3  );
  fprintf(pFile, "QA     : %12.4e \n",  QA  );
  fprintf(pFile, "QK     : %12.4e \n",  QK  );
  fprintf(pFile, "R      : %12.4e \n",  R   );
  fprintf(pFile, "purity : %12.4e \n",  purity  );
  fprintf(pFile, "purity2: %12.4e \n",  purity2 );

  
  fclose (pFile);
}


double getCorrection(double fittedDriftTime, double tauelec, double taulife){

  return (1.-exp(-(fittedDriftTime)*(1./tauelec + 1./taulife)))/(fittedDriftTime*(1./tauelec + 1./taulife));

}
