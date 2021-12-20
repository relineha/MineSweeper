#include <iostream>

void CorrelationsMacro()
{
  TFile * f1 = new TFile("/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/CorrelationsFile_CathodePrototype_PrePhase2_ana.root");
  TH2F * h17 = (TH2F*)f1->Get("rate_17kV");
  TH2F * h16 = (TH2F*)f1->Get("rate_16kV");
  TH2F * h14 = (TH2F*)f1->Get("rate_14kV");
  TH2F * h12 = (TH2F*)f1->Get("rate_12kV");
  TH2F * h10 = (TH2F*)f1->Get("rate_10kV");
  TH2F * h8 = (TH2F*)f1->Get("rate_8kV");
  TH2F * h6 = (TH2F*)f1->Get("rate_6kV");
  TH2F * h4 = (TH2F*)f1->Get("rate_4kV");
  TH2F * h0 = (TH2F*)f1->Get("rate_0kV");
  TH2F * h_fibers = (TH2F*)f1->Get("h_XYPositions_hotPics;1");
  TH2F * h_PMTs = (TH2F*)f1->Get("pmtPositions");
  h_fibers->SetLineColor(kRed);
  h_fibers->SetLineWidth(6);
  
  TCanvas * c17 = new TCanvas();
  h17->Draw("colz");
  h_fibers->Draw("boxSAME");

  TCanvas * c16 = new TCanvas();
  h16->Draw("colz");
  h_fibers->Draw("boxSAME");

  TCanvas * c14 = new TCanvas();
  h14->Draw("colz");
  h_fibers->Draw("boxSAME");

  TCanvas * c12 = new TCanvas();
  h12->Draw("colz");
  h_fibers->Draw("boxSAME");

  TCanvas * c10 = new TCanvas();
  h10->Draw("colz");
  h_fibers->Draw("boxSAME");

  TCanvas * c8 = new TCanvas();
  h8->Draw("colz");
  h_fibers->Draw("boxSAME");

  TCanvas * c6 = new TCanvas();
  h6->Draw("colz");
  h_fibers->Draw("boxSAME");

  TCanvas * c4 = new TCanvas();
  h4->Draw("colz");
  h_fibers->Draw("boxSAME");

  TCanvas * c0 = new TCanvas();
  h0->Draw("colz");
  h_fibers->Draw("boxSAME");


  TCanvas * cNum = new TCanvas();
  h17->Draw("colz");
  h_PMTs->SetMarkerSize(2);
  h_PMTs->SetMarkerColor(kRed);
  h_PMTs->Draw("textSAME");
  


}
