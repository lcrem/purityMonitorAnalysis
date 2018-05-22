#include "FFTtools.h"

TGraph *justAverage(Int_t numGraphs, TGraph **grPtrPtr);

TGraph *subtractGraphs( TGraph *grA, TGraph *grB) ;

int addAndAverage(TGraph *avg, TGraph *temp, int fNumInAverage);

TGraph *subtractNoise(TGraph *gsig, TGraph *gnoise);

Double_t fittingFunction(Double_t *x, Double_t *par);

TGraph *smoothGraph(TGraph *g, int nnn);

void zeroBaseline(TGraph *g);

TGraph *getZeroedAverage(Int_t numGraphs, TGraph **graphs);

TGraph *getFilteredAverage(Int_t numGraphs, TGraph **graphs, TGraph *powerSpectrumCorr, double threshold);

void removeGarbage(TGraph *g);

TGraph *justAverage(Int_t numGraphs, TGraph **grPtrPtr)
{
  //Assume they are all at same sampling rate

  // Can't correlate and average if there's only one graph.
  // So return 0
  if(numGraphs<2) return NULL;

  // TGraph *grA = grPtrPtr[0];
  TGraph *grA = (TGraph*) grPtrPtr[0]->Clone(); // Make copy of graph rather than using first graph.

  // Copy times from grA into new array.
  Int_t numPoints=grA->GetN();  
  Double_t *timeVals= grA->GetX();
  Double_t *safeTimeVals = new Double_t[numPoints];
  Double_t *sumVolts = new Double_t [numPoints];
  for(int i=0;i<numPoints;i++) 
    safeTimeVals[i]=timeVals[i];  
  

  // Loop over graph array.
  int countWaves=1;
  for(int graphNum=1;graphNum<numGraphs;graphNum++) {
    
    TGraph *grB = grPtrPtr[graphNum];
    if(grB->GetN()<numPoints)
      numPoints=grB->GetN();
 
    Double_t *aVolts = grA->GetY();
    Double_t *bVolts = grB->GetY();

    for(int ind=0;ind<numPoints;ind++) {
      sumVolts[ind]=(aVolts[ind]+bVolts[ind]);
    }
  

    TGraph *grComAB = new TGraph(numPoints,safeTimeVals,sumVolts);

    //    delete grB;
    //    if(graphNum>1)
    delete grA;
    grA=grComAB;
    countWaves++;

  }
  for(int i=0;i<numPoints;i++) {
    sumVolts[i]/=countWaves;
  }
  Double_t meanVal=TMath::Mean(numPoints,sumVolts);
  for(int i=0;i<numPoints;i++) {
    sumVolts[i]-=meanVal;
  }
  delete grA;
  TGraph *grRet = new TGraph(numPoints,safeTimeVals,sumVolts);
  delete [] safeTimeVals;
  delete [] sumVolts;
  return grRet;
}


int addAndAverage(TGraph *avg, TGraph *temp, int fNumInAverage){

   if(avg->GetN()!=temp->GetN()) {
    std::cerr << "Trying to add FFTGraph with different number of points " << avg->GetN() << " instead of " << temp->GetN() << "\n";
    return -1;
  }
   Double_t *newY=temp->GetY();
   for(int bin=0;bin<avg->GetN();bin++) {
     avg->GetY()[bin]=(avg->GetY()[bin]*fNumInAverage + newY[bin])/Double_t(fNumInAverage+1);
   }

   fNumInAverage++;   

   return fNumInAverage;
}



TGraph *subtractGraphs( TGraph *grA, TGraph *grB) 
{
  Int_t N1=grA->GetN();
  Int_t N2=grB->GetN();
  if(N1!=N2) return NULL;

  Double_t *newY = new Double_t [N1];
  Double_t *xVals=grA->GetX();
  Double_t x,yA,yB;
  for(int i=0;i<N1;i++) {
    grA->GetPoint(i,x,yA);
    yB=grB->Eval(x);
    newY[i]=yA-yB;
  }
  TGraph *grDiff = new TGraph(N1,xVals,newY);
  delete [] newY;
  return grDiff;
}



TGraph *subtractNoise(TGraph *gsig, TGraph *gnoise){

  
  TGraph *grCor = FFTtools::getCorrelationGraph(gsig, gnoise);
  double *theTimes = grCor->GetX();
  double *theValues = grCor->GetY();    
  int numPoints=grCor->GetN();
  int maxIndex=TMath::LocMax(numPoints,theValues);
  double maxCorTime=theTimes[maxIndex];  
  TGraph *grNoise = FFTtools::translateGraph(gnoise, maxCorTime);

  TGraph *out = subtractGraphs(gsig, grNoise);

  // TCanvas *c1 = new TCanvas("c1");
  // c1->Divide(2,2);
  // c1->cd(1);
  // // gsig->GetXaxis()->SetRangeUser(0.0005, 0.0006);
  // gsig->Draw();
  // grNoise->SetLineColor(kRed);
  // grNoise->Draw("same");
  // c1->cd(2);
  // grNoise->Draw();
  // c1->cd(3);
  // grCor->Draw();
  // c1->cd(4);
  // out->Draw();

  delete grNoise;
  delete grCor;
  
  return out;
}

Double_t fittingFunction(Double_t *x, Double_t *par)
{
  
  double t = x[0]*1E6 - par[3];
  double sigma = par[0];
  double tau   = par[1];
  double a     = par[2];

  double y = a*exp((sigma*sigma - 2*t*tau)/(2*tau*tau))*(1 + erf((-sigma*sigma+t*tau)/(sqrt(2)*tau*sigma)));
  
  return y;
}

Double_t ICARUSpolynomial(Double_t E)
{

  // transform E in kV/cm
  E*=1e-3;
  
  Double_t Et = 0.5;
  Double_t T  = 89;      // Kelvin
  Double_t T0 = 90.371;  // Kelvin
  Double_t p0 = -0.03229;
  Double_t p1 = 6.231;
  Double_t p2 = -10.62;
  Double_t p3 = 12.74;
  Double_t p4 = -9.112;
  Double_t p5 = 2.83;
  Double_t w1 = -0.01481;
  Double_t w2 = -0.0075;
  Double_t w3 = 0.141;
  Double_t w4 = 12.4;
  Double_t w5 = 1.627;
  Double_t w6 = 0.317;

  Double_t K1 = p0 + p1*Et + p2*Et*Et + p3*Et*Et*Et + p4*Et*Et*Et*Et + p5*Et*Et*Et*Et*Et;
  Double_t K2 = ( w1*(T-T0) + 1 )*( w3*Et*TMath::Log(1+w4/Et) + w5*TMath::Power(Et,w6) ) + w2*(T-T0);

  Double_t vE = (p0 + p1*E + p2*E*E + p3*E*E*E + p4*E*E*E*E + p5*E*E*E*E*E)*(K2/K1);

  // convert vE from mm/us to m/s

  vE *= (1e-3/1e-6);
  
  return vE;
   

}
TGraph *smoothGraph(TGraph *g, int nnn){

  int n = g->GetN();
  double *x = g->GetX();
  double *y = g->GetY();
  double newy[20000];

  int count=0;

  for (int i=nnn; i<(n-nnn); i++){
    newy[i]=0;
    
    for (int j=i-nnn; j<i+nnn; j++){
      newy[count]+=(y[j]/(nnn+nnn));
    }
    count++;
  }

  TGraph *gnew = new TGraph(count, x, newy);

  return gnew;

}


void zeroBaseline(TGraph *g){


  double *y = g->GetY();
  Double_t meanVal=TMath::Mean(200,g->GetY());
  for(int ip=0;ip<g->GetN();ip++) {
    y[ip] -=  meanVal;
  }

}


TGraph *getZeroedAverage(Int_t numGraphs, TGraph **graphs){
  
  Double_t newY[1000000];
  Int_t numPoints;
  
  TGraph *graphsZeroed[1000];
  
  for (int i=0; i<numGraphs; i++){
    
    numPoints = graphs[i]->GetN();
    Double_t meanVal=TMath::Mean(numPoints,graphs[i]->GetY());
    for(int ip=0;ip<numPoints;ip++) {
      newY[ip] = graphs[i]->GetY()[ip] - meanVal;
    }
    graphsZeroed[i] = new TGraph(numPoints,graphs[i]->GetX(), newY);
  }

  
  TGraph *zeroedAverage = justAverage( numGraphs,
				       graphsZeroed);


  return zeroedAverage;
}


TGraph *getFilteredAverage(Int_t numGraphs, TGraph **graphs, TGraph *powerSpectrumCorr, double threshold){

  int n = powerSpectrumCorr->GetN();
  double df = powerSpectrumCorr->GetX()[1]-powerSpectrumCorr->GetX()[0];
  
  double maxPeak = threshold * TMath::MaxElement(n,powerSpectrumCorr->GetY());
  int nFilters = 0;
  double notchmin[1000], notchmax[1000];

  double notchwidth = df/2;
  
  for (int i=0; i<n; i++){
    if (powerSpectrumCorr->GetY()[i]>maxPeak){
      notchmin[nFilters] = (powerSpectrumCorr->GetX()[i] - notchwidth/2)*1e9;
      notchmax[nFilters] = (powerSpectrumCorr->GetX()[i] + notchwidth/2)*1e9;
      cout << nFilters << " " <<   notchmin[nFilters] << " " <<   notchmax[nFilters] << endl;
      nFilters++;
    } 
  }

  
  TGraph *graphsFiltered[1000];
  
  for (int i=0; i<numGraphs; i++){
    graphsFiltered[i] = FFTtools::multipleSimpleNotchFilters(graphs[i], nFilters, notchmin, notchmax);
  }

  TGraph *filteredAverage = justAverage(   numGraphs,
					   graphsFiltered);


  return filteredAverage;
  

}



void removeGarbage(TGraph *g){


  double *y = g->GetY();
  double deltay = 0;
  double maxdelta = 0.1*TMath::MaxElement(g->GetN(), g->GetY());
  for(int ip=2;ip<g->GetN()-1;ip++) {
    deltay = y[ip] - y[ip-1];
    if (TMath::Abs(deltay)>maxdelta) {
      //      cout << "in here " << deltay << " " << maxdelta << endl;
      y[ip] = y[ip-1];// + maxdelta;
    }
  }

}
