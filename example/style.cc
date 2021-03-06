//
// Created by mikhail on 3/5/21.
//
{
gStyle->SetPadLeftMargin(0.15);
gStyle->SetPadRightMargin(0.05);
gStyle->SetPadBottomMargin(0.1);
gStyle->SetLegendBorderSize(0);
gStyle->SetFrameLineWidth(4);
gStyle->SetMarkerSize(2);
gStyle->SetLineWidth(3);

gStyle->SetTitleSize(0.035, "X");
gStyle->SetTitleSize(0.035, "Y");
gStyle->SetTitleSize(0.01, "Z");

gStyle->SetLabelSize(0.04, "X");
gStyle->SetLabelSize(0.04, "Y");
gStyle->SetLabelSize(0.04, "Z");

gStyle->SetLabelOffset(0.003, "X");
gStyle->SetLabelOffset(0.01, "Y");
gStyle->SetLabelOffset(0.05, "Z");

gStyle->SetTitleOffset(1.0, "X");
gStyle->SetTitleOffset(0.0, "Y");
gStyle->SetTitleOffset(0.05, "Z");

gStyle->SetOptStat(0);
}