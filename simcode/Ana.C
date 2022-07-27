#include "TGraph.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TMultiGraph.h"
#include "TLegend.h"

#include <fstream>
#include <iostream>
#include <string>

TGraph* GetLight(std::string file) {
  TFile* inroot = new TFile(Form("Chev.%s.root", file.c_str()), "READ");
  TH1D* hwl = (TH1D*) inroot->Get("Wavelength");

  std::cout << file.c_str() << ": " << hwl->Integral(hwl->FindBin(300.), hwl->FindBin(500.)
  ) / 5. << std::endl;

  std::ifstream infile("QuantumEfficiency_PMT.csv");
  double tmp_wl;
  double tmp_qe;

  TGraph* gr = new TGraph();
  while (infile >> tmp_wl >> tmp_qe) {
    gr->AddPoint(tmp_wl, tmp_qe / 100.);
  }
  infile.close();

  TGraph* gr_detected = new TGraph();
  for (unsigned int i = 1; i < hwl->GetSize() - 1; i++) {
    double val = hwl->GetBinContent(i);
    double wl_i = hwl->GetBinCenter(i);
    double scale = 0.;
    if (wl_i < 720 && wl_i > 201) scale = gr->Eval(wl_i);
    val = val * scale;
    gr_detected->AddPoint(wl_i, val);
  }
  inroot->Close();

  return gr_detected;
}

void Add(std::string file, TMultiGraph* mgr, TLegend* leg, Color_t color, std::string legendStr) {
  TGraph* gr = GetLight(file.c_str());
  gr->SetLineColor(color);
  mgr->Add(gr, "L");
  leg->AddEntry(gr, legendStr.c_str(), "L");
}

void Ana() {
    TMultiGraph* mgr = new TMultiGraph();
    TLegend* leg = new TLegend(0.4, 0.5, 0.8, 0.8);
    leg->SetBorderSize(0);

    Add("2GeV", mgr, leg, kRed, "2 GeV");

    TCanvas* c = new TCanvas();
    mgr->Draw("A");
    mgr->GetXaxis()->SetTitle("Wavelength (nm)");
    mgr->GetYaxis()->SetTitle("No. of detected photons (a.u.)");
    c->SaveAs("CherenkovSpectrum.pdf");
}
