
TGraph *subtractGraphs( TGraph *grA, TGraph *grB) ;

TGraph *justAverage(Int_t numGraphs, TGraph **grPtrPtr);

int addAndAverage(TGraph *avg, TGraph *temp, int fNumInAverage);

TGraph *subtractNoise(TGraph *gsig, TGraph *gnoise);

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
