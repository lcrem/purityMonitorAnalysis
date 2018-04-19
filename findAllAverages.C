#include "FFTtools.h"
#include "FFTWComplex.h"
#include "RFSignal.h"
#include "DigitalFilter.h"

#include "functions.h"

TGraph *graphs[1000];

int fillGraphs(string fname);

TGraph *getFilteredAverage(Int_t numGraphs, TGraph **grPtrPtr, TGraph *noiseTemplate, double threshold);

TGraph *getFancyFilteredAverage(Int_t numGraphs, TGraph **grPtrPtr, TGraph *noiseTemplate, double threshold);

TGraph *getSubtractedAverage(Int_t numGraphs, TGraph **grPtrPtr, TGraph *noiseTemplate);

TH2D *getPeriodogram(Int_t numGraphs, TGraph **grPtrPtr, TGraph *noiseTemplate);

TH2D *getVoltsHistogram( Int_t numGraphs, TGraph **grPtrPtr );

TGraph *getAveragePowerSpectrum(Int_t numGraphs, TGraph **graphs);

TGraph *getZeroedAverage(Int_t numGraphs, TGraph **graphs);

void findAllAverages(string filename){

  string finput = filename + ".root";
  string foutput = filename + "_averages.root";
  
  int ngraphs = fillGraphs(finput);

  TGraph *justAvg       = justAverage( ngraphs, graphs );

  TGraph *avgPowerSpectrum  = getAveragePowerSpectrum ( ngraphs, graphs );
  
  TGraph *noiseTemplate = FFTtools::correlateAndAverage( ngraphs, graphs );

  TGraph *noiseTempPS   = FFTtools::makePowerSpectrumVoltsSeconds( noiseTemplate );
  
  TGraph *filteredAvg   = getFilteredAverage( ngraphs, graphs, noiseTempPS, 0.01 );

  TGraph *filteredAvgPS = FFTtools::makePowerSpectrumVoltsSeconds( filteredAvg );

  TGraph *fancyFilteredAvg   = getFancyFilteredAverage( ngraphs, graphs, noiseTempPS, 0.01 );

  TGraph *fancyFilteredAvgPS = FFTtools::makePowerSpectrumVoltsSeconds( fancyFilteredAvg );

  TGraph *zeroedAvg      = getZeroedAverage( ngraphs, graphs);
  
  //  TGraph *subtractedAvg = getSubtractedAverage( ngraphs, graphs, noiseTemplate );

  TH2D *periodogram     = getPeriodogram( ngraphs, graphs, noiseTemplate );

  TH2D *voltsHisto      = getVoltsHistogram( ngraphs, graphs );

  

  TFile *fout = new TFile(foutput.c_str(), "recreate");
  justAvg         ->Write("justAvg");
  avgPowerSpectrum    ->Write("avgPowerSpectrum");
  noiseTemplate   ->Write("noiseTemplate");
  noiseTempPS     ->Write("noiseTemplatePS");
  filteredAvg     ->Write("filteredAvg");
  filteredAvgPS   ->Write("filteredAvgPS");
  fancyFilteredAvg     ->Write("fancyFilteredAvg");
  fancyFilteredAvgPS   ->Write("fancyFilteredAvgPS");
  zeroedAvg       ->Write("zeroedAvg");
  // subtractedAvg   ->Write("subtractedAvg");
  periodogram     ->Write("periodogram");
  voltsHisto      ->Write("voltsHisto");
}


int fillGraphs(string filename){

  TFile *f = new TFile(filename.c_str(), "read");

  int count = 0;
  for (int i=0; i<1000; i++){
    // cout << i << endl;
    graphs[i] = (TGraph*) f->Get(Form("graph%i", i+1));
    if(!graphs[i]) break;
    count++;
  }

  f->Close();

  cout << count << endl;
  return count;
  
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



TGraph *getFancyFilteredAverage(Int_t numGraphs, TGraph **graphs, TGraph *powerSpectrumCorr, double threshold){

  int n = powerSpectrumCorr->GetN();
  double df = powerSpectrumCorr->GetX()[1]-powerSpectrumCorr->GetX()[0];
  
  double maxPeak = threshold * TMath::MaxElement(n,powerSpectrumCorr->GetY());
  int nFilters = 0;
  double notchmin[1000], notchmax[1000];

  double notchwidth = df/2;
  double lastFreq = 25.;
  double maxFreq = powerSpectrumCorr->GetX()[n-1];
  
  FFTtools::DigitalFilterSeries *digFilts = new FFTtools::DigitalFilterSeries();

  double *x = powerSpectrumCorr->GetX();
  double *y = powerSpectrumCorr->GetY();
  
  for (int i=2; i<n; i++){

    if (powerSpectrumCorr->GetY()[i]>maxPeak && powerSpectrumCorr->GetX()[i] < lastFreq){
    //  cout << y[i] << " " << threshold << " " << avg << endl;
    //if (y[i]*threshold > (y[i-2]+y[i+2]) && powerSpectrumCorr->GetX()[i] < lastFreq){
      notchmin[nFilters] = (powerSpectrumCorr->GetX()[i] - notchwidth/2)*1e9;
      notchmax[nFilters] = (powerSpectrumCorr->GetX()[i] + notchwidth/2)*1e9;
      cout << nFilters << " " <<   notchmin[nFilters] << " " <<   notchmax[nFilters] << endl;
      nFilters++;

      digFilts->add(new FFTtools::RCFilter(FFTtools::NOTCH, powerSpectrumCorr->GetX()[i], notchwidth/2), false);
      
    } else if (powerSpectrumCorr->GetX()[i] > lastFreq) {

      // notchmin[nFilters] = lastFreq*1e9;
      // notchmax[nFilters] = powerSpectrumCorr->GetX()[n-1]*1e9;
      // //      cout << nFilters << " " <<   notchmin[nFilters] << " " <<   notchmax[nFilters] << endl;
      // nFilters++;
      break;
    }
  }

  
  TGraph *graphsFiltered[1000];

  cout << (lastFreq+(maxFreq-lastFreq)/2) << " " <<  ((maxFreq-lastFreq)/2) << endl;
  FFTtools::RCFilter *filt = new FFTtools::RCFilter(FFTtools::LOWPASS     , (lastFreq+(maxFreq-lastFreq)/2), ((maxFreq-lastFreq)/2));
  digFilts->add(filt, false);
  
  for (int i=0; i<numGraphs; i++){
    graphsFiltered[i] = (TGraph*)graphs[i]->Clone();
    digFilts->filterGraph(graphsFiltered[i]);
  }

  TGraph *filteredAverage = justAverage(   numGraphs,
					   graphsFiltered);


  return filteredAverage;
  

}



TGraph *getSubtractedAverage(Int_t numGraphs, TGraph **graphs, TGraph *noiseTemplate){

  
  TGraph *graphsSubtracted[1000];
  
  for (int i=0; i<numGraphs; i++){
    graphsSubtracted[i] = subtractNoise(graphs[i], noiseTemplate);
  }
  
  TGraph *gaverage = justAverage(    numGraphs,
				     graphsSubtracted);

  return gaverage;

}

TH2D *getPeriodogram(Int_t numGraphs, TGraph **graphs, TGraph *noiseTemplate){

  TGraph *powerSpectrumCorr = FFTtools::makePowerSpectrumVoltsSeconds(noiseTemplate);
  powerSpectrumCorr->SetTitle("Average power spectrum;Frequency [GHz];Amplitude");
  int n = powerSpectrumCorr->GetN();
  double df = powerSpectrumCorr->GetX()[1]-powerSpectrumCorr->GetX()[0];
  
  // TGraph *powerSpectrumAvg = NULL;
  
  TH2D *hPeriodogram = new TH2D ("hPeriodogram", "", numGraphs, 0, numGraphs, n, 0, n*df);
  
  int navg=0;
  
  for (int isample=0; isample<numGraphs; isample++){
    
    TGraph *gtemp = FFTtools::makePowerSpectrumVoltsSeconds(graphs[isample]);
    // if (isample==0) powerSpectrumAvg = (TGraph*)gtemp->Clone();
    // else {
    //   navg = addAndAverage(powerSpectrumAvg, gtemp, navg);
    // }
    int N = gtemp->GetN();
    double *x = gtemp->GetX();
    double *y = gtemp->GetY();
    for (int i=0; i<N; i++) hPeriodogram->Fill(isample, x[i], y[i]);

  }
  
  hPeriodogram->SetTitle(";Sample Number;Frequency [GHz]");

  return hPeriodogram;
}

TGraph *getAveragePowerSpectrum(Int_t numGraphs, TGraph **graphs){

  TGraph *powerSpectrumAvg = NULL;
    
  int navg=0;
  
  for (int isample=0; isample<numGraphs; isample++){
    
    TGraph *gtemp = FFTtools::makePowerSpectrumVoltsSeconds(graphs[isample]);
    if (isample==0) powerSpectrumAvg = (TGraph*)gtemp->Clone();
    else {
      navg = addAndAverage(powerSpectrumAvg, gtemp, navg);
    }

  }
  
  return powerSpectrumAvg;
}

TH2D *getVoltsHistogram( Int_t numGraphs, TGraph **grPtrPtr ){

  double min = graphs[0]->GetMinimum();
  double max = graphs[0]->GetMaximum();

  TH2D *h = new TH2D("h", "", numGraphs, 0, numGraphs, 100, min, max );
  
  for (int isample=0; isample<numGraphs; isample++){

    int N = graphs[isample]->GetN();
    double *y = graphs[isample]->GetY();
    
    for (int i=0; i<N; i++) h->Fill(isample, y[i]);

  }

  return h;
}