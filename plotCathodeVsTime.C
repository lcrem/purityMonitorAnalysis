#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>

int lengthOf100=18;

int findAllTimes(string folder, string times[100], int minutes[100]);

string findFilename(string folder, string times);

void plotCathodeVsTime(string basename="2018August17Vacuum"){

  string material[] = {"Gold", "Silver"};

  string times[100];
  int minutes[100];
  double x[2][200];
  double y[2][200];

  TGraph *g[2];

  TCanvas *c = new TCanvas("c");
  TLegend *leg = new TLegend(0.6, 0.11, 0.9, 0.25);

  
  for (int imat=0; imat<2; imat++){
    string folder = basename + "/" + material[imat];
    int ntimes = findAllTimes(folder, times, minutes);

    int countpoints = 0;

    
    for (int it=0; it<ntimes; it++){

      string filename = findFilename(folder, times[it]);
      
      TFile *fin = new TFile((folder+"/"+filename).c_str(), "read");



      for (int igraph=0; igraph<10; igraph++){
	TGraph *gtemp = (TGraph*)fin->Get(Form("avg100/gavg100_%d", igraph));
	y[imat][countpoints] = TMath::MinElement(gtemp->GetN(), gtemp->GetY())*(-1);
	x[imat][countpoints] = it*60*3 + igraph*lengthOf100;
	// cout <<it << " " <<  x[imat][countpoints] << " " << y[imat][countpoints] << endl;
	countpoints++;
      }

      
      delete fin;
      
      

    }
    g[imat] = new TGraph(countpoints, x[imat], y[imat]);
    g[imat]->SetLineColor(imat+1);
    g[imat]->SetLineWidth(2);
    leg->AddEntry(g[imat], material[imat].c_str(), "l");
    
    if (imat==0){
      g[imat]->GetYaxis()->SetRangeUser(0, 1);
      g[imat]->Draw("Al");
    }else{
      g[imat]->Draw("l");
    }
  }

  leg->Draw();

  
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
	    // cout << times[count] << " " << minutes[count] << endl;
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
	    return temps;
	  }
       } 
    	
     }
  return "nothing";
  
}
