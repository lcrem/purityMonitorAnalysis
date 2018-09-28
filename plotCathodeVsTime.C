#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include "functions.h"

int lengthOf100=20;

int findAllTimes(string folder, string times[100], int minutes[100]);

string findFilename(string folder, string times);

//void plotCathodeVsTime(string basename="2018September11Vacumm/NewLampPosition/Gold/ContinuousExposition/"){
void plotCathodeVsTime(string basename="2018September11Vacumm/NewLampPosition/"){

  string title="Cathode data";
  string titleout = "BothCathodes_Sept11_NewLampPosition";


  string material[] = {"Gold", "Silver"};

  string times[3000];
  int minutes[3000];
  double x[2][3000];
  double yPeak[2][3000];
  double yFitPeak[2][3000];
  double yIntegral[2][3000];

  TGraph *gPeak[2];
  TGraph *gFitPeak[2];
  TGraph *gIntegral[2];

  TCanvas *c = new TCanvas("c");
  TLegend *leg = new TLegend(0.6, 0.11, 0.9, 0.25);
  double tauelecK=43.4835*1e-6;
  
  for (int imat=1; imat<2; imat++){

    int countpoints = 0;
    
    int allt=0;
    int iday=1;
    //   for (int iday=1; iday<4; iday++){
    string folder = basename +"/" + material[imat];//+ "/Day" + Form("%d", iday); //+ material[imat];
    //string folder = basename + "/Day" + Form("%d", iday); //+ material[imat];
      cout << folder << endl;
      int ntimes = findAllTimes(folder, times, minutes);
            
      
      for (int it=0; it<ntimes; it++){
	
	string filename = findFilename(folder, times[it]);
	cout << filename << endl;
	
	TFile *fin = new TFile((folder+"/"+filename).c_str(), "read");
	
	double waveTimes[10];

	string inputnametime = filename;
	std::size_t pos1 = inputnametime.find("_aver");
	inputnametime.erase(pos1, inputnametime.length());
	ifstream f0 (Form("%s/%s.times", folder.c_str(), inputnametime.c_str()));
	string line;
	double acqTot = 0.;
	double acq0;
	int iline=0;
	while ( getline(f0, line)){
	  struct tm tm;
	  std::size_t pos = line.find(",");
	  line.erase(0, 1);
	  string tokens[10];
	  int itoken=0;
	  while ((pos = line.find(",")) != std::string::npos) {
	    tokens[itoken] = line.substr(0, pos);
	    //	    std::cout << tokens[itoken] << std::endl;
	    line.erase(0, pos + 1);
	    itoken++;
	  }
	  tm.tm_sec  = atof(tokens[0].c_str());
	  tm.tm_min  = atoi(tokens[1].c_str());
	  tm.tm_hour = atoi(tokens[2].c_str());
	  tm.tm_mday = atoi(tokens[3].c_str());
	  tm.tm_mon  = atoi(tokens[4].c_str())-1;
	  tm.tm_year = atoi(tokens[5].c_str())-1900;
	  
	  char buff[80];
	  strftime(buff, 80, "%Y.%m.%d %H:%M:%S", &tm);
	  std::cout<<"should be: "<<std::string(buff)<<"\n";
	  

	  waveTimes[iline] = mktime(&tm);
	  iline++;
	  //     cout << add0 << " " << line << endl;
	}
	f0.close();
	
	
	for (int igraph=0; igraph<10; igraph++){
	  TGraph *gtemp = (TGraph*)fin->Get(Form("avg100/gavg100_%d", igraph));

	  TF1 *func = new TF1("func",fittingFunction,0.,0.9E-3,4);
	  func->SetParameters(5, 90, -10, 1);
	  func->SetParLimits(1, tauelecK*1E6*0.95, tauelecK*1E6*1.05);
	  func->SetParName(0, "#sigma");
	  func->SetParName(1, "#tau_{D}");
	  func->SetParName(2, "a");
	  func->SetParName(3, "t_0");
	  gtemp->Fit("func", "", "RQN0", -0.1E-3, 0.5E-3);

	  yFitPeak[imat][countpoints] = func->GetMinimum()*(-1);
	  yIntegral[imat][countpoints] = gtemp->Integral();
	  yPeak[imat][countpoints] = TMath::MinElement(gtemp->GetN(), gtemp->GetY())*(-1);
	   //	  x[imat][countpoints] = allt*(1800) + igraph*lengthOf100;
	  //	  x[imat][countpoints] = allt*(lengthOf100*10) + igraph*lengthOf100;
	  x[imat][countpoints] = waveTimes[igraph];
	  cout <<it << " " <<  x[imat][countpoints] << " " << yPeak[imat][countpoints] << endl;
	  countpoints++;
	  delete func;
	}
	
	
	delete fin;
	
	allt++;
	//      } // end of day
      
    }
    gPeak[imat] = new TGraph(countpoints, x[imat], yPeak[imat]);
    gPeak[imat]->SetLineColor(imat+1);
    gPeak[imat]->SetMarkerColor(imat+1);
    gPeak[imat]->SetMarkerStyle(2);
    gPeak[imat]->SetLineWidth(2);
    leg->AddEntry(gPeak[imat], material[imat].c_str(), "l");
    gPeak[imat]->SetTitle((title+";Time [s];Peak [V]").c_str());
    //    g[imat]->GetYaxis()->SetRangeUser(0, 4);

    gFitPeak[imat] = new TGraph(countpoints, x[imat], yFitPeak[imat]);
    gFitPeak[imat]->SetLineColor(imat+1);
    gFitPeak[imat]->SetLineWidth(2);
    gFitPeak[imat]->SetMarkerColor(imat+1);
    gFitPeak[imat]->SetMarkerStyle(2);
    gFitPeak[imat]->SetTitle((title+";Time [s];Fitted Peak [V]").c_str());

    gIntegral[imat] = new TGraph(countpoints, x[imat], yIntegral[imat]);
    gIntegral[imat]->SetLineColor(imat+1);
    gIntegral[imat]->SetLineWidth(2);
    gIntegral[imat]->SetMarkerColor(imat+1);
    gIntegral[imat]->SetMarkerStyle(2);
    gIntegral[imat]->SetTitle((title+";Time [s];Integral [V]").c_str());
  

  }

  cout << "Out here" << endl;

  double xmax, xmin, ymax;

  if (gPeak[0] && gPeak[1]){
    xmin = TMath::MinElement(gPeak[0]->GetN(), gPeak[0]->GetX()) < TMath::MinElement(gPeak[1]->GetN(), gPeak[1]->GetX()) ? TMath::MinElement(gPeak[0]->GetN(), gPeak[0]->GetX()) : TMath::MinElement(gPeak[1]->GetN(), gPeak[1]->GetX()) ;

    xmax = TMath::MaxElement(gPeak[0]->GetN(), gPeak[0]->GetX()) > TMath::MaxElement(gPeak[1]->GetN(), gPeak[1]->GetX()) ? TMath::MaxElement(gPeak[0]->GetN(), gPeak[0]->GetX()) : TMath::MaxElement(gPeak[1]->GetN(), gPeak[1]->GetX()) ;

    ymax = TMath::MaxElement(gPeak[0]->GetN(), gPeak[0]->GetY()) > TMath::MaxElement(gPeak[1]->GetN(), gPeak[1]->GetY()) ? TMath::MaxElement(gPeak[0]->GetN(), gPeak[0]->GetY()) : TMath::MaxElement(gPeak[1]->GetN(), gPeak[1]->GetY()) ;

    
    cout <<"DONE?" << endl;
    gStyle->SetOptStat(0);
    TH2D *h = new TH2D("h", "", 100, xmin, xmax, 100, 0, ymax*1.1);
    h->Draw();
    h->SetTitle(";Time [s];Peak [V]");
    gPeak[0]->Draw("p");
    gPeak[1]->Draw("p");
    leg->Draw();
    
    c->Print((titleout+".png").c_str());
    c->Print((titleout+".pdf").c_str());
    
  }


  TFile *fout = new TFile((titleout+".root").c_str(), "recreate");
  if (gPeak[0]){
    gPeak[0]->Write("gPeak_gold");
    gFitPeak[0]->Write("gFitPeak_gold");
    gIntegral[0]->Write("gIntegralPeak_gold");
  }
  if (gPeak[1]){
    gPeak[1]->Write("gPeak_silver");
    gFitPeak[1]->Write("gFitPeak_silver");
    gIntegral[1]->Write("gIntegralPeak_silver");
  }
  
}


int findAllTimes(string folder, string times[100], int minutes[100]){

  DIR *dp;
  dirent *d;
  int count = 0;
  int secs0, secs1;
  int h, m;
  string pattern1=".ch4.traces_averages.root";
  string pattern2="FibreIn_";
  if((dp = opendir(folder.c_str())) == NULL)
    perror("opendir");
  
  while((d = readdir(dp)) != NULL)
    {
      if(!strcmp(d->d_name,".") || !strcmp(d->d_name,".."))
	continue;

	std::string temps = d->d_name;
	if (temps.find(pattern1) != std::string::npos) {
	  std::string::size_type i = temps.find(pattern1);
	  temps.erase(i, pattern1.length());
	  i = temps.find(pattern2);
	  temps.erase(0, i+pattern2.length());
	  times[count] = temps;
	  if (count==0) minutes[count]=0;
	  else {
	    sscanf(times[count].c_str(), "%d.%d", &h, &m);
	    secs0 = h*60 + m;
	    sscanf(times[count-1].c_str(), "%d.%d", &h, &m);
	    secs1 = h*60 + m;
	    minutes[count] = secs0-secs1 + minutes[count-1];
	     cout << times[count] << " " << minutes[count] << endl;
	  }
	  count++;
      } 
      	
    }
  return count;
  
  
}


string findFilename(string folder, string time){

 DIR *dp;
  dirent *d;
  int count = 0;
  int secs0, secs1;
  int h, m;
  string pattern1=".ch4.traces_averages.root";
  string pattern2="FibreIn_";
  if((dp = opendir(folder.c_str())) == NULL)
    perror("opendir");
  
   while((d = readdir(dp)) != NULL)
     {
       if(!strcmp(d->d_name,".") || !strcmp(d->d_name,".."))
   	continue;

   	std::string temps = d->d_name;
   	if (temps.find(pattern1) != std::string::npos) {
	  if (temps.find(time) != std::string::npos) {
	    cout << temps << endl;
	    return temps;
	  }
       } 
    	
     }
  return "nothing";
  
}
