#include "FFTtools.h"
#include "FFTWComplex.h"
#include "RFSignal.h"

#include "functions.h"

double xmin = 0.05E-3;
double xmax = 0.015E-3;
double ymin = -0.8;
double ymax = +0.8;

double tauelec_preampA = 91.5031*1e-6;
double tauelec_preampB = 43.4835*1e-6;
double tauelecK, tauelecA;
double gain_preampA    = 10.;//9.69;//1./0.104421;
double gain_preampB    = 9.;//7.97;//1./0.102084;
double gain_AoverB     = 0.89512;


//string basename = "2018Apr11liquefaction/Day3_allLiquid/cathodeANDanode/";
string whichAvg[4] = {"justAvg", "filteredAvg", "zeroedAvg", "fancyFilteredAvg"};

string howManyAvg[4] = {"avg20", "avg50", "avg100", "avg200"};
int howManyGraphs[4] = {50, 20, 10, 5};
int smoothing[2] = {5, 15};

void getFields (string fieldname, double fields[3]);

double getCorrection(double fittedDriftTime, double tauelec, double taulife);

int calculateLifetime(TGraph *gdiff[2], double tTheory[3], double lifetime[2]);

void calculatePurityWithErrors(string basename, string fieldname, string divname){


  string outfile    = basename + fieldname + "_" + divname + "_signals_withErrors.root";
  string outtxtfile = basename + fieldname + "_" + divname + "_lifetime_withErrors.txt";
  
  double fields[3], distance[3], tTheory[3];
  getFields (fieldname, fields);
  //distances from K to GK, GK to GA, GA to A as measured on 12.06.2018 by Laura
  distance[0] = 0.01823; // 1.8cm
  distance[1] = 0.16424; // 1.0cm
  distance[2] = 0.00985; // 1.0cm
  
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
  double step = TMath::MaxElement(gtimedelay->GetN(), yTimeDelay)*0.9;
  double timeStep = xTimeDelay[1]-xTimeDelay[0];
  for (int ip=0; ip<gtimedelay->GetN(); ip++){
    if (yTimeDelay[ip]>(step)){
      timedelay = xTimeDelay[ip];
      break;
    }
  }
  ftimedelay->Close();
  cout << "The time delay is " << timedelay << endl;

  for (int i=0; i<3; i++){
    tTheory[i] = distance[i]/ICARUSpolynomial(fields[i]);
    cout << distance[i] << " " << fields[i] << " " << tTheory[i] << " " << timeStep << " " << getSmoothingNumber(timeStep, tTheory[i]) <<endl;
  }
  smoothing[0] =  getSmoothingNumber(timeStep, tTheory[2]);
  smoothing[1] =  getSmoothingNumber(timeStep, tTheory[0]);
  double newy[20000];
  double newx[20000];
 
  
  if (divname.find("ampSwitch") != std::string::npos){
    cout << "AMP SWITCHED !!!!" << endl;
    tauelecK = tauelec_preampA;
    tauelecA = tauelec_preampB;
        
  }else{
    tauelecK = tauelec_preampB;
    tauelecA = tauelec_preampA;
    
  }

  
  TFile *out = new TFile(outfile.c_str(), "recreate");
  
  TH1D *hpurity[5];

  

  

  int iavg=2;
  // int inum=3;

  for (int inum=1; inum<4; inum++){
    hpurity[inum]= new TH1D (Form("hpurity_%d", inum), "", 1000, 0, 0.005);
    double finalNumbers[2][3]; // [0 anode, 1 cathode] [0 amplitude, 1 start time, 2 peak time]

    for (int igraph=0; igraph<howManyGraphs[inum]; igraph++){

      TGraph *gdiff[2];
      
      for (int ich=0; ich<2; ich++){
	string f1 = basename + fieldname + "_FibreIn_" + divname + "." + chname[ich] +".traces_averages.root";
        
	TFile *file1 = new TFile(f1.c_str(), "read");

	
	TGraph *g1 = (TGraph*)file1->Get(Form("%s/g%s_%d", howManyAvg[inum].c_str(), howManyAvg[inum].c_str(), igraph));
	cout << f1 << " " <<  Form("%s/g%s_%d", howManyAvg[inum].c_str(), howManyAvg[inum].c_str(), igraph) << endl;
	g1->SetName("g1");
        
	file1->Close();
        
	g1 = FFTtools::translateGraph(g1, -timedelay);
        
	zeroBaseline(g1);
	
	//	gdiff[ich] = smoothGraph(g1, smoothing[ich]);
	gdiff[ich] = (TGraph*)g1->Clone();

      }

      double lifetime[2];
      
      int ok = calculateLifetime(gdiff,  tTheory, lifetime);
      
      if (ok==1) hpurity[inum]->Fill(lifetime[0]);

  
    }
    
  }
  
  FILE * pFile;
  
  cout << "Writing these info to " << outtxtfile << endl;
  
  
  pFile = fopen (outtxtfile.c_str(),"w");
  
  
  for (int inum=1; inum<4; inum++){

    double avgLifetime = hpurity[inum]->GetMean();
    double errLifetime = hpurity[inum]->GetMeanError();
    double rmsLifetime = hpurity[inum]->GetRMS();
    double rmserrLifetime = hpurity[inum]->GetRMSError();
  

    cout << "THIS IS OUR PURITY AND ERROR: " << avgLifetime << " " << errLifetime << endl;


    // TF1 *fgaus = new TF1("fgaus", "gaus");
    // fgaus->SetParameter(1, avgLifetime);
    // hpurity[inum]->Fit("fgaus");
    // cout << "SOMENUMBERS " << fgaus->GetParameter(0) << " " << fgaus->GetParameter(1) << " " << fgaus->GetParameter(2) << endl;

    
    fprintf(pFile, "%s \n", howManyAvg[inum].c_str());
    fprintf(pFile, "%8.3e %8.3e %8.3e %8.3e \n", avgLifetime , errLifetime, rmsLifetime, rmserrLifetime);
    // fprintf(pFile, "tK     : %12.4e \n",  tK  );
    // fprintf(pFile, "tGK    : %12.4e \n",  tGK );
    // fprintf(pFile, "tGA    : %12.4e \n",  tGA );
    // fprintf(pFile, "tA     : %12.4e \n",  tA  );
    // fprintf(pFile, "t1     : %12.4e \n",  t1  );
    // fprintf(pFile, "t2     : %12.4e \n",  t2  );
    // fprintf(pFile, "t3     : %12.4e \n",  t3  );
    // fprintf(pFile, "QA     : %12.4e \n",  QA  );
    // fprintf(pFile, "QK     : %12.4e \n",  QK  );
    // fprintf(pFile, "QA corr: %12.4e \n",  newQA  );
    // fprintf(pFile, "QK corr: %12.4e \n",  newQK  );
    // fprintf(pFile, "R      : %12.4e \n",  R   );
    // fprintf(pFile, "lifetime : %12.4e \n",  lifetime  );
    // fprintf(pFile, "lifetime2: %12.4e \n",  lifetime2 );
  
    out->cd();
    hpurity[inum]->Write(Form("hpurity_%d", inum));
  }

  
  fclose (pFile);

  

}
  


  
double getCorrection(double fittedDriftTime, double tauelec, double taulife){
  
  double laura = (1.-exp(-(fittedDriftTime)*(1./tauelec + 1./taulife)))/(fittedDriftTime*(1./tauelec + 1./taulife));
  double alan  = (exp(-fittedDriftTime/tauelec)-exp(-fittedDriftTime/taulife))/(fittedDriftTime*(1/taulife - 1/tauelec));
  
  cout << "Correction laura " << laura << "; alan " << alan << endl;
  return laura;
  
}


void getFields (string fieldname, double fields[3]){
  
  string s = fieldname.substr(fieldname.find("_") + 1) ;
  
  std::string ext("Vcm");
  if ( s != ext &&
       s.size() > ext.size() &&
       s.substr(s.size() - ext.size()) == "Vcm" )
    {
      // if so then strip them off
      s = s.substr(0, s.size() - ext.size());
    }
  std::replace( s.begin(), s.end(), '.', ' ');
  
  std::stringstream ss(s);
  ss >> fields[0] >> fields[1] >> fields[2];
  
  cout << " The three fields are " << fields[0] << " " << fields[1] << " " << fields[2] << " V/cm" << endl;
}




int calculateLifetime(TGraph *gdiff[2], double tTheory[3], double lifetime[2]){
      
  int nK      = gdiff[1]->GetN();
  double *xK  = gdiff[1]->GetX();
  double *yK  = gdiff[1]->GetY();
      
  int nA      = gdiff[0]->GetN();
  double *xA  = gdiff[0]->GetX();
  double *yA  = gdiff[0]->GetY();
      
  double fittedK;
  double fittedKtime;
  double fittedKstartTime = 0;
      
  double fittedA;
  double fittedAtime;
  double fittedAstartTime = 0;
      
  int loc;
  double peak;
        
  // cout << " Cathode field " << fields[0] << endl;
      
  for (int ip=nK-2; ip>0; ip--){
    if (xK[ip]<0) break;
    if (yK[ip]<peak && (xK[ip]>(tTheory[0]-0.1E-3)) && (xK[ip]<(tTheory[0]+0.1E-3)) ){
      peak = yK[ip];
      loc = ip;
    }
  }
      
  fittedKtime = xK[loc];
  fittedK = yK[loc];
      
  fittedKstartTime = 0.;



      
  TF1 *funcK = new TF1("funcK",fittingFunction,0.,0.9E-3,4);
  funcK->SetParameters(5,  tauelecK*1E6, -0.002, (tTheory[0])*1e6);
  // funcK->FixParameter(0, 5);
  // funcK->FixParameter(1, 43);
  // funcK->FixParameter(2, 0.002);
  // funcK->FixParameter(3, (tTheory[0]+tTheory[1])*1e6);
  // funcK->SetParLimits(0, 1, 50);
  funcK->SetParLimits(1, tauelecK*1E6*0.95, tauelecK*1E6*1.05);
  // funcK->SetParLimits(2, 0, 0.1);
  // funcK->SetParLimits(3, 10, 500);
  funcK->SetParName(0, "#sigma");
  funcK->SetParName(1, "#tau_{D}");
  funcK->SetParName(2, "a");
  funcK->SetParName(3, "t_0");
  gdiff[1]->Fit("funcK", "R", "", 0, tTheory[0]+tTheory[1]-0.1E-3);

  
  
      
  double tempx, tempy;
  for (int ip=0; ip<nK; ip++){
    tempx = xK[ip];
    tempy = funcK->Eval(xK[ip]);
    if (tempy<0.01*fittedK){
      // cout << ip << " " << tempx << " " << tempy << " " << endl;
      fittedKstartTime = tempx;
      break;
    }
  }
  if (fittedKstartTime<0) fittedKstartTime=0;
  fittedKtime = funcK->GetMinimumX();
  fittedK = funcK->GetMinimum();
      

  
  // cout << " Anode field " << fields[2] << endl;
  loc = TMath::LocMax(nA,yA);
  peak = -99999.;
  for (int ip=nA-2; ip>0; ip--){
    if (xA[ip]<0.05E-3) break;
    if (yA[ip]>peak){
      peak = yA[ip];
      loc = ip;
    }
  }
	
      
  fittedAtime = xA[loc];
  fittedA = yA[loc];
      
  // double R, C;
  // if (divname.find("ampSwitch") != std::string::npos){
  //   cout << "AMP SWITCHED !!!!" << endl;
  //   R = 800.;
  //   C = 0.102;
  // }else{
  //   R = 500.	;
  //   C = 0.104;
  // }
      
  TF1 *funcA = new TF1("funcA",fittingFunction,0.,0.9E-3,4);
  funcA->SetParameters(5, 90, 0.002, (tTheory[0]+tTheory[1])*1e6);
  // funcA->FixParameter(0, 5);
  // funcA->FixParameter(1, 43);
  // funcA->FixParameter(2, 0.002);
  // funcA->FixParameter(3, (tTheory[0]+tTheory[1])*1e6);
  // funcA->SetParLimits(0, 1, 50);
  funcA->SetParLimits(1, tauelecA*1E6*0.95, tauelecA*1E6*1.05);
  // funcA->SetParLimits(2, 0, 0.1);
  // funcA->SetParLimits(3, 10, 500);
  funcA->SetParName(0, "#sigma");
  funcA->SetParName(1, "#tau_{D}");
  funcA->SetParName(2, "a");
  funcA->SetParName(3, "t_0");
  gdiff[0]->Fit("funcA", "R", "", tTheory[0]+tTheory[1]-0.1E-3, xA[nA-1]);
      
      
      
  // TF1 *funcA = new TF1("funcA",signalFunction,tTheory[0]+tTheory[1]-0.1E-3, xK[nK-1],6);
  // funcA->SetParName(0, "Q [pC]");
  // funcA->SetParName(1, "R [MOhm]");
  // funcA->SetParName(2, "C [pF]");
  // funcA->SetParName(3, "taulife [us]");
  // funcA->SetParName(4, "tdrift [us]");
  // funcA->SetParName(5, "t0 [us]");
  // funcA->SetParameters(5, R, C, 1000, icarusTime*1E6, (tTheory[0]+tTheory[1])*1E6);
  // // funcA->FixParameter(0, 5);
  // // funcA->FixParameter(1, R);
  // // funcA->FixParameter(2, C);
  // //	  funcA->FixParameter(3, 1000);
  // // funcA->FixParameter(4, icarusTime*1E6);
  // // funcA->FixParameter(5,  (tTheory[0]+tTheory[1])*1E6);
  // gdiff2->Fit("funcA", "R", "", tTheory[0]+tTheory[1]-0.1E-3, xK[nK-1]);
      
      
  for (int ip=0; ip<nA; ip++){
    tempx = xA[ip];
    tempy = funcA->Eval(xA[ip]);
    if (tempy>0.01*fittedA){
      // cout << ip << " " << tempx << " " << tempy << " " << endl;
      fittedAstartTime = tempx;
      break;
    }
  }
  fittedA     = funcA->GetMaximum();
  fittedAtime = funcA->GetMaximumX();
      
      
          
      
  cout << " Expected vs measured time at cathode " << tTheory[0] << " " << fittedKtime - fittedKstartTime << endl;
  cout << " Expected vs measured time at anode " << tTheory[2] << " " << fittedAtime - fittedAstartTime << endl;
      
    
  // //  TF1 *funcAll = new TF1("funcAll", [&](double*x, double *p){ return (funcA->Eval(x[0])-funcK->Eval(x[0])); }, 0,xA[nA-1],8);
  // TF1 *funcAll = new TF1("funcAll", fittingCathodeAndAnode, 0,xA[nA-1],8);
  // funcAll->SetParameters(funcA->GetParameter(0), funcA->GetParameter(1), funcA->GetParameter(2), funcA->GetParameter(3), funcK->GetParameter(0), funcK->GetParameter(1), funcK->GetParameter(2), funcK->GetParameter(3));
  // funcAll->SetParLimits(1, tauelecA*1E6*0.95, tauelecA*1E6*1.05);
  // funcAll->SetParLimits(5, tauelecK*1E6*0.95, tauelecK*1E6*1.05);
  // TGraph *gAll =  subtractGraphs(gdiff[0], gdiff[1]);
  // gAll->Draw("Al");
  // funcAll->Draw("same");
  // gAll->Fit("funcAll", "R", "", 0, xA[nA-1]);


  // cout << "BEFORE " << endl;
  // cout << fittedK << " " << fittedKtime << " " << fittedA << " " << fittedAtime << endl;

  // fittedK     = funcAll->GetMaximum(0, tTheory[0]+0.1E-6)*(-1);
  // fittedKtime = funcAll->GetMaximumX(0, tTheory[0]+0.1E-6);

  // fittedA     = funcAll->GetMaximum(tTheory[0]+tTheory[1]/2, xA[nA-1]);
  // fittedAtime = funcAll->GetMaximumX(tTheory[0]+tTheory[1]/2, xA[nA-1]);

  // cout << "AFTER " << endl;
  // cout << fittedK << " " << fittedKtime << " " << fittedA << " " << fittedAtime << endl;
  

  
  
  //  return;
  double tK, tGK, tGA, tA, t1, t2, t3;
  double QA, QK, R;
  tK  = fittedKstartTime;
  tGK = fittedKtime;
  tGA  = fittedAstartTime;
  tA = fittedAtime;
            
  QA = fittedA;
  QK = fittedK;
      
  t1 = tGK - tK;
  t2 = tGA - tGK;
  t3 = tA - tGA;
  R = 0;
  
  
  if (tK<tGK && tGK<tGA && tGA<tA){
    // All good
  } else {
    cout << "THESE NUMBERS DON'T MAKE SENSE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << tK << " " << tGK << " " << tGA << " " << tA << endl;
    return -1;
  }
  
  
  
  // if (divname.find("ampSwitch") != std::string::npos){
  //   cout << "AMP SWITCHED !!!!" << endl;   
  //   QA *= gain_AoverB;
  // }else{    
    QK *= gain_AoverB;
  // }
  
  double taulife = 0.001;
  double Kcorrection = 0.;
  double Acorrection = 0.;
  double newQK = QK;
  double newQA = QA;
  
  if (R<1){
    
    int count = 0;
    while(count < 20){
      
      //cout << "Catodo " << endl;
      Kcorrection = getCorrection(t1, tauelecK, taulife);
      //cout << "Anodo " << endl;
      Acorrection = getCorrection(t3, tauelecA, taulife);
      
      newQK = QK/Kcorrection;
      newQA = QA/Acorrection;
      
      R =  TMath::Abs(newQA/newQK);
      
      lifetime[0] = (1/TMath::Abs(TMath::Log(R)))*(t2 + 0.5*(t1+t3));
      
      cout <<" This is my lifetime " <<  lifetime[0] << endl;
      
      if (TMath::Abs(lifetime[0]-taulife)<0.001e-6) break;
      taulife=lifetime[0];
      count++;
    }
    cout << "Number of iterations : " << count << endl;
    
    TF1 f ("lifetime function", "([1]/[3])*(TMath::SinH([3]/(2*x))/TMath::SinH([1]/(2*x)))*TMath::Exp(-([2]+0.5*([1]+[3]))/x) - [0]", -0.1, 0.1);
    f.SetParameters(R, t1, t2, t3);
	
    ROOT::Math::WrappedTF1 wf1(f);
	
    // Create the Integrator
    ROOT::Math::BrentRootFinder brf;
	
    // Set parameters of the method
    brf.SetFunction( wf1, -0.1, 0.1 );
    brf.Solve();
	
    lifetime[1] =  TMath::Abs(brf.Root() ) ;
	

  } else {
    lifetime[0] = lifetime[1] = 0;
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
  printf("QAcorr : %12.4e \n",  newQA  );
  printf("QKcorr : %12.4e \n",  newQK  );
  printf("R      : %12.4e \n",  R   );
  printf("lifetime : %12.4e \n",  lifetime[0]  );
  printf("lifetime2: %12.4e \n",  lifetime[1] );


  return 1;
}
