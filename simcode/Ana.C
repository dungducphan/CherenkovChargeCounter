TGraph* GetLight(std::string file) {
  TFile* inroot = new TFile(Form("Chev.%s.root", file.c_str()), "READ");
  TH1D* hwl = (TH1D*) inroot->Get("Wavelength");

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

void Ana() {
  TGraph* F2 = GetLight("2GeV");
  TGraph* F5 = GetLight("5GeV");
  TGraph* L2 = GetLight("200M");
  TGraph* TL2 = GetLight("T_200M");
  TGraph* TF5 = GetLight("T_5GeV");
  TGraph* TF2 = GetLight("T_2GeV");

  TMultiGraph* mgr = new TMultiGraph();
  F2->SetLineColor(kRed);
  mgr->Add(F2, "L");
  F5->SetLineColor(kBlue);
  mgr->Add(F5, "L");
  L2->SetLineColor(kOrange);
  mgr->Add(L2, "L");
  TL2->SetLineColor(kCyan);
  mgr->Add(TL2, "L");
  TF5->SetLineColor(kViolet);
  mgr->Add(TF5, "L");
  TF2->SetLineColor(kBlack);
  mgr->Add(TF2, "L");

  mgr->GetXaxis()->SetTitle("Wavelength (nm)");
  mgr->GetYaxis()->SetTitle("No. of detected photons (a.u.)");

  TLegend* leg = new TLegend(0.5, 0.8, 0.5, 0.8);
  leg->SetBorderSize(0);
  leg->AddEntry(L2, "200 MeV", "L");
  leg->AddEntry(F2, "2 GeV", "L");
  leg->AddEntry(F5, "5 GeV", "L");
  leg->AddEntry(TL2, "200 MeV, 5#times charge", "L");
  leg->AddEntry(TF2, "2 GeV, 5#times charge", "L");
  leg->AddEntry(TF5, "5 GeV, 5#times charge", "L");

  mgr->Draw("AL");
  leg->Draw();
}
