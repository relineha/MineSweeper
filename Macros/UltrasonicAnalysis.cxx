#include <iostream>
#include <cmath>
#include "TH1F.h"

void UltrasonicAnalysis()
{
  TFile * f1 = new TFile("/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/TG38PreDust_ana.root");
  TFile * f2 = new TFile("/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/TG38Dust_ana.root");
  TFile * f3 = new TFile("/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/TG38PostUS1_ana.root");
  TFile * f4 = new TFile("/Users/ryanlinehan/LZ_Local_Work/Grids/GitlabRepo/GridsMacros/AutomatedGridScanning/MineSweeper/TG38PostUS2_ana.root");
  
  TH1F * h1 = (TH1F*)f1->Get("realClustAreas");
  TH1F * h2 = (TH1F*)f2->Get("realClustAreas");
  TH1F * h3 = (TH1F*)f3->Get("realClustAreas");
  TH1F * h4 = (TH1F*)f4->Get("realClustAreas");

  gStyle->SetOptStat(0);


  //Have to scale up the dust one because there are only 12 pictures here (compared to 17 in all other cases). We want to make sure that
  //we're comparing apples to apples here.
  //First get the histogram integral
  double integral = ((double)h2->Integral());
  std::cout << "Integral of h2 is: " << integral << std::endl;
  for( int iBin = 1; iBin <= h2->GetNbinsX(); ++iBin ){
    h2->SetBinContent(iBin,h2->GetBinContent(iBin)*17./12.);
  }


  std::cout << "Integrals: " << h1->Integral() << ", " << h2->Integral() << ", " << h3->Integral() << ", " << h4->Integral() << std::endl;
  
  /*
  h2->Rebin();
  h1->Rebin();
  h3->Rebin();
  h4->Rebin();
  */  

  TCanvas * c1 = new TCanvas();
  h1->SetLineColor(kGreen);
  h2->SetLineColor(kBlack);
  h3->SetLineColor(kBlue);
  h4->SetLineColor(kViolet);

  
  
  h2->SetTitle("Point-of-interest Area Spectrum");
  h2->GetXaxis()->SetTitle("Point-of-interest area (um^2)");
  h2->GetYaxis()->SetTitle("# of Points-of-interest per 312 um^2");
  
  TLegend * leg = new TLegend(0.6,0.7,0.85,0.85);
  leg->AddEntry(h2,"Initial Dust Deposition");
  leg->AddEntry(h3,"Post-US 1");
  leg->AddEntry(h4,"Post-US 2");
  
  h2->Draw("");
  //  h1->Draw("same");
  h3->Draw("same");
  h4->Draw("same");
  leg->Draw();
  



  TCanvas * c2 = new TCanvas();
  h1->SetTitle("Point-of-interest Area Spectrum");
  h1->GetXaxis()->SetTitle("Area (um^2)");
  h1->GetYaxis()->SetTitle("# of Points-of-interest per 312 um^2");
  h1->Draw();
  h4->Draw("same");
  TLegend *leg2 = new TLegend(0.6,0.7,0.85,0.85);
  leg2->AddEntry(h1,"Just Unbagged");
  leg2->AddEntry(h4,"Post-US 2");
  leg2->Draw();
  
 





}
