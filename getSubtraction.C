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

int smoothing[2] = {5, 15};

void getFields (string fieldname, double fields[3]);

double getCorrection(double fittedDriftTime, double tauelec, double taulife);

void getSubtraction(string basename, string fieldname, string divname){

  bool useTGK = false;
  string tofind = "Long";
  
  if (fieldname.find(tofind) != std::string::npos){
    useTGK=true;
  }

  string outfile    = basename + fieldname + "_" + divname + "_signals.root";
  string outtxtfile = basename + fieldname + "_" + divname + "_lifetime.txt";
  
  double fields[3], distance[3], tTheory[3];
  getFields (fieldname, fields);
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

  
  double tauelec_preampA = 91.5031*1e-6;
  double tauelec_preampB = 43.4835*1e-6;
  double tauelecK, tauelecA;
  double gain_preampA    = 10.;//9.69;//1./0.104421;
  double gain_preampB    = 9.;//7.97;//1./0.102084;
  double gain_AoverB     = 0.89512;
  
  
  if (divname.find("ampSwitch") != std::string::npos){
    cout << "AMP SWITCHED !!!!" << endl;
    tauelecK = tauelec_preampA;
    tauelecA = tauelec_preampB;
        
  }else{
    tauelecK = tauelec_preampB;
    tauelecA = tauelec_preampA;
    
  }
  
  
  TFile *out = new TFile(outfile.c_str(), "recreate");
  
  
  double finalNumbers[2][3]; // [0 anode, 1 cathode] [0 amplitude, 1 start time, 2 peak time]
  
  for (int iavg=0; iavg<3; iavg++){
    if(iavg!=1){
      for (int ich=0; ich<2; ich++){
        string f1 = basename + fieldname + "_FibreIn_" + divname + "." + chname[ich] +".traces_averages.root";
        //      string f2 = basename + fieldname + "_FibreIn_" + divname + "."+ chname[(ich==0)*1] + ".traces_averages.root";
        string f2 = basename + fieldname + "_FibreOut_" + divname + "."+ chname[ich] + ".traces_averages.root";
        
        TFile *file1 = new TFile(f1.c_str(), "read");
	TFile *file2 = new TFile(f2.c_str(), "read");
        
        TGraph *g1 = (TGraph*)file1->Get(whichAvg[iavg].c_str());
        cout << whichAvg[iavg].c_str() << endl;
        g1->SetName("g1");
        
        TGraph *g2;
        bool isFibreOut;
        if (file2->IsZombie()){
	  //          string tempfilename = "2018May02liquefaction/liquid/K-475GK-350GA350A630_70.140.280Vcm_FibreOut_200mVdiv_16.59."+ chname[ich] +".traces_averages.root";
	  string tempfilename = f1; 
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
        
        // TF1 ftemp1("ftemp1",[&](double *x, double *){ return g1->Eval(x[0]); },-0.1E-3, 0.3E-3,0);
        // double integral1 = TMath::Abs(ftemp1.Integral(0., 0.3E-3));
        // TF1 ftemp2("ftemp2",[&](double *x, double *){ return g2->Eval(x[0]); },-0.1E-3, 0.3E-3,0);
        // double integral2 = TMath::Abs(ftemp2.Integral(0., 0.3E-3));
        double dt = x[1] - x[0];
        double integral1 = g1->Integral(0, 0.2E-3/dt);
        double integral2 = g2->Integral(0, 0.2E-3/dt);
        double renorm = integral1/integral2;
        if (ich==1 || (!isFibreOut)) renorm=1;
        cout << "Integral " << integral1 << " " << integral2 << endl;
        if (renorm>10) renorm=1;
        renorm=1;
        
        for (int i=0; i<N; i++){
          if (isFibreOut) newy[i] = y1[i] -  (renorm)*y2[i];
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
        //TGraph *gdiff2 = (TGraph*)gdiff->Clone();//smoothGraph(gdiff, 1);
        TGraph *gdiff2 = smoothGraph(gdiff, smoothing[ich]);
        
        
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
          double icarusTime;
          
          if (ich==1){ // Cathode
            cout << " Cathode field " << fields[0] << endl;
            icarusTime = tTheory[0];
            peak = 99999;
            // loc = TMath::LocMin(nK,yK);
            for (int ip=nK-2; ip>0; ip--){
              if (xK[ip]<0) break;
              if (yK[ip]<peak && (xK[ip]>(icarusTime-0.1E-3)) && (xK[ip]<(icarusTime+0.1E-3)) ){
                peak = yK[ip];
                loc = ip;
              }
            }
            fittedKtime = xK[loc];
            fittedK = yK[loc];

	    if (useTGK){
	      fittedKtime = tTheory[0];
	      fittedK = gdiff2->Eval(fittedKtime);
	    }
	    
            fittedKstartTime = 0.;
          } else { //anode
            cout << " Anode field " << fields[2] << endl;
            icarusTime = tTheory[2];
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
            
            double R, C;
            if (divname.find("ampSwitch") != std::string::npos){
              cout << "AMP SWITCHED !!!!" << endl;
              R = 800.;
              C = 0.102;
            }else{
              R = 500.	;
              C = 0.104;
            }
            
            
            TF1 *func = new TF1("func",fittingFunction,0.,0.9E-3,4);
            func->SetParameters(5, 90, 0.002, (tTheory[0]+tTheory[1])*1e6);
	    // func->FixParameter(0, 5);
	    // func->FixParameter(1, 43);
	    // func->FixParameter(2, 0.002);
	    // func->FixParameter(3, (tTheory[0]+tTheory[1])*1e6);
            // func->SetParLimits(0, 1, 50);
	    func->SetParLimits(1, tauelecA*1E6*0.95, tauelecA*1E6*1.05);
            // func->SetParLimits(2, 0, 0.1);
            // func->SetParLimits(3, 10, 500);
            func->SetParName(0, "#sigma");
            func->SetParName(1, "#tau_{D}");
            func->SetParName(2, "a");
            func->SetParName(3, "t_0");
            gdiff2->Fit("func", "R", "", tTheory[0]+tTheory[1]-0.1E-3, xK[nK-1]);
            
            
            // TF1 *func = new TF1("func",signalFunction,tTheory[0]+tTheory[1]-0.1E-3, xK[nK-1],6);
            // func->SetParName(0, "Q [pC]");
            // func->SetParName(1, "R [MOhm]");
            // func->SetParName(2, "C [pF]");
            // func->SetParName(3, "taulife [us]");
            // func->SetParName(4, "tdrift [us]");
            // func->SetParName(5, "t0 [us]");
            // func->SetParameters(5, R, C, 1000, icarusTime*1E6, (tTheory[0]+tTheory[1])*1E6);
            // // func->FixParameter(0, 5);
            // // func->FixParameter(1, R);
            // // func->FixParameter(2, C);
            // //	  func->FixParameter(3, 1000);
            // // func->FixParameter(4, icarusTime*1E6);
            // // func->FixParameter(5,  (tTheory[0]+tTheory[1])*1E6);
            // gdiff2->Fit("func", "R", "", tTheory[0]+tTheory[1]-0.1E-3, xK[nK-1]);
            
            
            double tempx, tempy;
            for (int ip=0; ip<nK; ip++){
              tempx = xK[ip];
              tempy = func->Eval(xK[ip]);
              if (tempy>0.01*fittedK){
                // cout << ip << " " << tempx << " " << tempy << " " << endl;
                fittedKstartTime = tempx;
                break;
              }
            }
            fittedK = func->GetMaximum();
            //	  delete func;
            // TF1 *straightLine = new TF1("straightLine", "[0]+[1]*x");
            // gdiff2->Fit("straightLine", "RQ0", "", fittedKtime-icarusTime/2, fittedKtime);
            // fittedKstartTime = -straightLine->GetParameter(0)/straightLine->GetParameter(1);
            
            // delete straightLine;
          }
          // cout << chnamenice[ich] << " " << loc << " " << fittedKtime << " " << fittedK << endl;
          
          
          TCanvas *ctemp = new TCanvas();
          gdiff2->Draw("Al");
          
          double fittedDriftTime = fittedKtime - fittedKstartTime;
          cout << " Expected vs measured time at " << chnamenice[ich] << " " << icarusTime << " " << fittedDriftTime << endl;
          
          // cout << "Fitted Cathode  " << fittedK << endl;
          // cout << "Fitted Cathode  time " << fittedKtime << endl;
          // cout << "Fitted Cathode start time " << fittedKstartTime << endl;
          // cout << "Fitted drift time " << fittedDriftTime << endl;
          
          
          finalNumbers[ich][0] = fittedK;
          finalNumbers[ich][1] = fittedKstartTime;
          finalNumbers[ich][2] = fittedKtime;
          
        }
        
        // delete gdiff;
        // delete gdiff2;
        // delete c;
        
        
        
        file1->Close();
        file2->Close();
      }
    }
  }
  
  
  //  return;
  double tK, tGK, tGA, tA, t1, t2, t3;
  double QA, QK, R, lifetime, lifetime2;
  tK  = finalNumbers[1][1];
  tGK = finalNumbers[1][2];
  tA  = finalNumbers[0][2];
  tGA = finalNumbers[0][1];
  t1 = tGK - tK;
  t2 = tGA - tGK;
  t3 = tA - tGA;
  
  if (divname.find("ampSwitch") != std::string::npos){
    // QA /= gain_preampB;
    // QK /= gain_preampA;
    QA *= gain_AoverB;
    
  }else{
    QK *= gain_AoverB;
    // QA /= gain_preampA;
    // QK /= gain_preampB;
  }
  
  
  if (tK<tGK && tGK<tGA && tGA<tA){
    
  } else {
    cout << "THESE NUMBERS DON'T MAKE SENSE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    
  }
  
  QA = finalNumbers[0][0];
  QK = finalNumbers[1][0];
  
  double taulife = 0.001;
  double Kcorrection;
  double Acorrection;
  double newQK = QK;
  double newQA = QA;
  
  if (R<1){
    
    int count = 0;
    while(count<20){
      
      //cout << "Catodo " << endl;
      Kcorrection = getCorrection(t1, tauelecK, taulife);
      //cout << "Anodo " << endl;
      Acorrection = getCorrection(t3, tauelecA, taulife);
      
      newQK = QK/Kcorrection;
      newQA = QA/Acorrection;
      
      R =  TMath::Abs(newQA/newQK);
      
      lifetime = (1/TMath::Abs(TMath::Log(R)))*(t2 + 0.5*(t1+t3));
      
      cout <<" This is my lifetime " <<  lifetime << endl;
      
      if (TMath::Abs(lifetime-taulife)<0.01e-6) break;
      taulife=lifetime;
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
    
    lifetime2 =  brf.Root() ;
  } else {
    lifetime = lifetime2 = 0;
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
  printf("lifetime : %12.4e \n",  lifetime  );
  printf("lifetime2: %12.4e \n",  lifetime2 );
  
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
  fprintf(pFile, "QA corr: %12.4e \n",  newQA  );
  fprintf(pFile, "QK corr: %12.4e \n",  newQK  );
  fprintf(pFile, "R      : %12.4e \n",  R   );
  fprintf(pFile, "lifetime : %12.4e \n",  lifetime  );
  fprintf(pFile, "lifetime2: %12.4e \n",  lifetime2 );
  
  
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
