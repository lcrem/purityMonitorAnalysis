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

Double_t greenFunction(Double_t *x, Double_t *par);

Double_t greenFunction2(Double_t *x, Double_t *par);

Double_t signalFunction(Double_t *x, Double_t *par);

int avgSomeGraphs(string filename, int nmax, TGraph **g);

TF1 *getPreampA();

TF1 *getPreampB();

int getSmoothingNumber(double deltat, double tdrift);

double attachmentCoefficientPoly(double E);

Double_t purityAttachmentCoefficient(Double_t *x, Double_t *par);

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
  Double_t meanVal=TMath::Mean(50,sumVolts);
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

Double_t signalFunction(Double_t *x, Double_t *par)
{

  Double_t t = x[0]*1e6 - par[5];
  Double_t Q = par[0];
  // par[1] is in pF so we need to convert it in Farad
  double C = par[1];
  // par[2] is in MOhm so we need to convert it in Ohm
  double R = par[2];
  Double_t taulife = par[3];
  Double_t tdrift = par[4];

  Double_t tauelec = R*C;
  
  double heavisidet;
  if (t<0) heavisidet = 0;
  else heavisidet = 1;
  
  double heavisidet2;
  if ((tdrift-t)<0) heavisidet2=0;
  else heavisidet2 = 1;
  
  //Double_t Vout = (1/C)*(Q/tdrift)*heavisidet*(tautot)*(1-exp(-t/(tautot)))*heavisidet2;
  //  Double_t Vout = (1/C)*(Q/tdrift)*(tautot)*(exp(-t/(tautot)))*heavisidet;
  // Double_t Vout = (1/C)*(Q/tdrift)*tautot*(exp(-(t-tdrift)/tautot)-exp(-t/tautot))*heavisidet;
  Double_t Vout = (Q/C)*(exp(-t/tauelec)-exp(-t/taulife))*heavisidet2*heavisidet/(tdrift*(1./taulife - 1./tauelec));
  
  return Vout;
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


Double_t greenFunction2(Double_t *x, Double_t *par)
{
  
  // x[0] is in seconds, so we need to conver par[3] from musec to sec
  double t = x[0] - par[3]*1e-6;
  // 
  double Q = par[0]*1e-12;
  // par[1] is in pF so we need to convert it in Farad
  double C = par[1]*1e-12;
  // par[2] is in MOhm so we need to convert it in Ohm
  double R = par[2]*1e6;

  
  double heaviside;
  if (t<0) heaviside=0;
  else heaviside = 1;
  
  double y = (-Q/C)*exp(-t/(R*C))*heaviside;
  
  return y;
}


Double_t greenFunction(Double_t *x, Double_t *par)
{
  
  // x[0] is in seconds, so we need to convert par[3] from musec to sec
  double t = x[0];
  double GQ = par[0];
  double tau = par[1]*1e-6;
  double rise = par[2]*1e-6;
  double shift = par[3]*1e-6;
  
  double heaviside;
  if (t<0) heaviside=0;
  else heaviside = 1;
  
  //double y = (-GQ)*exp(-t/(tau))*heaviside;
  
  double y;
  double tmp1 = GQ*(-1-erf((0+shift)/rise));
  double tmp2 = -GQ*exp(-0/tau);
  if (t>0) y = -GQ*exp(-t/tau);
  else y = GQ*(-1-erf((t+shift)/rise))*tmp2/tmp1;
  
  return y;
}

TF1 *getPreampA(){

  TF1 *func = new TF1("preampA",greenFunction,-0.1E-3,0.7E-3,4);  
  func->SetParName(0, "Q (pC)");
  //	func->SetParLimits(0, 1e-6, 2e-6);
  func->SetParName(1, "C (pF)");
  func->SetParLimits(1, 0.1, 0.2);
  func->SetParName(2, "R (M#Omega)");
  //func->SetParLimits(2, 490, 510);
  func->SetParName(3, "t_0 (#mus)");

  func->FixParameter(1, 0.103212);
  func->FixParameter(2, 475.078); 
  /* func->FixParameter(3, 5.904); */

  return func;
  
}

TF1 *getPreampB(){

  TF1 *func = new TF1("preampB",greenFunction,-0.1E-3,0.7E-3,4);  
  func->SetParName(0, "Q (pC)");
  //	func->SetParLimits(0, 1e-6, 2e-6);
  func->SetParName(1, "C (pF)");
  //func->SetParLimits(1, 0.1, 0.2);
  func->SetParName(2, "R (M#Omega)");
  //func->SetParLimits(2, 490, 510);
  func->SetParName(3, "t_0 (#mus)");

  func->FixParameter(1, 0.12547);
  func->FixParameter(2, 722.664);
  func->FixParameter(3, 7.675);

  return func;
}


TGraph *smoothGraph(TGraph *g, int nnn){

  int n = g->GetN();
  double *x = g->GetX();
  double *y = g->GetY();
  double newy[100010];
  double newx[100010];

  int count=0;
  int insidecount=0;
  
  for (int i=nnn; i<(n-nnn); i++){
    newy[count]=0;
    
    insidecount=0;
    for (int j=i-nnn; j<i+nnn; j++){
      newy[count]+=(y[j]);
      insidecount++;
    }
    newy[count]/=(insidecount*1.);
    newx[count] = x[i];
    count++;
  }

  TGraph *gnew = new TGraph(count, newx, newy);

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
  
  Double_t newY[100010];
  Int_t numPoints;
  
  TGraph *graphsZeroed[1000];

  int count = 0;
  
  for (int i=0; i<numGraphs; i++){
    
    numPoints = graphs[i]->GetN();

    /* TGraph *gtemp = smoothGraph(graphs[i], 20); */
    /* Double_t minAll = TMath::Abs(TMath::MinElement(gtemp->GetN(), gtemp->GetY()));   */
    /* if (minAll < 0.1 ) continue;   */
    /* delete gtemp; */
    
    Double_t meanVal=TMath::Mean(50, graphs[i]->GetY());

    for(int ip=0;ip<numPoints;ip++) {
      newY[ip] = graphs[i]->GetY()[ip] - meanVal;
    }
    
    graphsZeroed[count] = new TGraph(numPoints,graphs[i]->GetX(), newY);
    count++;
  }

  
  TGraph *zeroedAverage = justAverage( count,
				       graphsZeroed );

  Double_t mean=TMath::Mean(50, zeroedAverage->GetY());

  for(int ip=0;ip<numPoints;ip++) {
    newY[ip] = zeroedAverage->GetY()[ip] - mean;
  }

  zeroedAverage = new TGraph(numPoints,zeroedAverage->GetX(), newY);

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



int avgSomeGraphs(string filename, int nmax, TGraph **g){

  TFile *f = new TFile(filename.c_str(), "read");
  TGraph *graphs[1000];
  int count = 0;
  int ntot = 0;

  double newy[100010];
  double newx[100010];

  for (int i=0; i<1000; i++){
    // cout << i << endl;
    graphs[count] = (TGraph*) f->Get(Form("graph%i", i+1));
    if(!graphs[count]) break;
    count++;
    if (count==nmax){
      
      g[ntot] = getZeroedAverage( count, graphs );
      // //TGraph *gtemp = getFilteredAverage( count, graphs, noiseTemplatePS, 0.1 );
      /* g[ntot] = subtractGraphs(g[ntot], fibreOut); */
      zeroBaseline(g[ntot]);
      g[ntot]  = smoothGraph(g[ntot],  5); 
      
      count = 0;
      ntot++;
      // delete gtemp;
    }
  }

  f->Close();

  // cout << ntot << endl;
  return ntot;
  
}


int getSmoothingNumber(double deltat, double tdrift){

  int nsigma = 10;
  int nsmooth = tdrift/(nsigma*2*deltat);
  if (nsmooth>20) nsmooth=20;
  return  nsmooth;

}


double attachmentCoefficientPoly(double E){

  double C0 =  9.06962616E10;
  double C1 = -4.07856249E7;
  double C2 = 5809.220018;

  return (C0 + C1*E + C2*E*E)*31.25E-6;

}



Double_t purityAttachmentCoefficient(Double_t *x, Double_t *par)
{

  double attCoeff = (par[0] + par[1]*x[0] + par[2]*x[0]*x[0])*31.25E-6;

  double units = 1.E9*1000;
  
  return units/(attCoeff*par[3]);
}
