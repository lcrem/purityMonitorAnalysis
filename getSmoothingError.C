

string fieldNice[] = {
  //"nofield"
  "20-40-80 V/cm",
  
  "25-50-100 V/cm",
  
  "30-60-120 V/cm",
  
  "40-80-160 V/cm",
  
  "50-100-200 V/cm",
  
  "60-120-240 V/cm",
  
  "70-140-280 V/cm",
  
  "80-140-280 V/cm",
  
  "90-180-360 V/cm",
  
  "100-200-400 V/cm"
};

void getSmoothingError(){

  TGraph *g[5];
  TGraph *gratio[5];
  
  int numFields = (sizeof(fieldNice)/sizeof(*fieldNice));

  for (int i=0; i<5; i++){
    TFile *f = new TFile(Form("Smoothing_%02itimes.root", i+8), "read");
    g[i] = (TGraph*)f->Get("lifetime");
    delete f;
  }
  int colors[5] = {kBlue+1, kCyan+1, kBlack, kOrange+1, kViolet+1};


  for (int i=0; i<5; i++){
    double *x = g[i]->GetX();
    double *num = g[i]->GetY();
    double *den = g[2]->GetY();
    int n = g[i]->GetN();
    double newy[20000];
    for (int ip=0; ip<n; ip++) newy[ip] = num[ip]/den[ip];
    gratio[i] = new TGraph(n, x, newy);
  }
  
  TCanvas *c = new TCanvas("c", "", 650, 800);
  c->Divide(1,2);

  c->cd(1);

  TLegend *leg = new TLegend(0.21, 0.11, 0.43, 0.43);
  
  gStyle->SetOptStat(0);
  for (int i=0; i<5; i++){
    g[i]->SetTitle("Lifetime;;Lifetime");
    g[i]->GetHistogram()->GetXaxis()->Set(numFields, -0.5, numFields-0.5);
    g[i]->GetHistogram()->GetYaxis()->SetTitle("Lifetime [us]");
    g[i]->SetMarkerStyle(kCircle);
    g[i]->SetMarkerSize(0.8);
    g[i]->SetFillColor(colors[i]);
    g[i]->SetMarkerColor(colors[i]);
    g[i]->SetLineColor(colors[i]);
    g[i]->SetMaximum(1300);
    leg->AddEntry(g[i], Form("Smoothing fraction 1/%d", i+8), "l");
    if (i==0){
      for (int ifi=1;ifi<=numFields;ifi++) g[i]->GetHistogram()->GetXaxis()->SetBinLabel(ifi,fieldNice[ifi-1].c_str());
      g[i]->Draw("Alp text");
    } else g[i]->Draw("lp text");
  }
  
  leg->Draw();
  c->cd(2);

  
  for (int i=0; i<5; i++){
    gratio[i]->SetTitle("Ratio of lifetimes;;Ratio");
    gratio[i]->GetHistogram()->GetXaxis()->Set(numFields, -0.5, numFields-0.5);
    gratio[i]->GetHistogram()->GetYaxis()->SetTitle("Ratio of lifetimes");
    gratio[i]->SetMarkerStyle(kCircle);
    gratio[i]->SetMarkerSize(0.8);
    gratio[i]->SetFillColor(colors[i]);
    gratio[i]->SetMarkerColor(colors[i]);
    gratio[i]->SetLineColor(colors[i]);
    if (i==0){
      for (int ifi=1;ifi<=numFields;ifi++) gratio[i]->GetHistogram()->GetXaxis()->SetBinLabel(ifi,fieldNice[ifi-1].c_str());
      gratio[i]->Draw("Alp text");
    } else gratio[i]->Draw("lp text");
  }

  c->Print("SmoothingErrorPlot.png");
  c->Print("SmoothingErrorPlot.pdf");
  c->Print("SmoothingErrorPlot.root");
}
