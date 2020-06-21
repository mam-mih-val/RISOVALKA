#include <string>

#include <TFile.h>

#include <DataContainer.h>
#include <Stats.h>
//
// Created by mikhail on 6/18/20.
//
int main(int n_args, char** args){
  std::string file_name = args[1];
  auto* file_in = TFile::Open(file_name.data());
  std::string u_vector_name{"sim_tracks"};
  std::string correction_step{"PLAIN"};
  std::string branch_name{"sim_tracks"};
  std::string correlation_name = u_vector_name+"_"+correction_step+ "_psi_rp_PLAIN_YY_ep" ;
  Qn::DataContainer<Qn::Stats>* in_container;
  std::vector<Qn::DataContainer<Qn::Stats>*> containers;
  file_in->GetObject(correlation_name.data(), in_container);
  auto container_bw = in_container->Rebin( {"Centrality", 1, 0.0, 5.0} );
  container_bw = container_bw.Rebin( {branch_name+"_rapidity", 1, -0.15+0.74, -0.05+0.74} );
  container_bw = container_bw.Projection({branch_name+"_pT"});
  container_bw = container_bw*2;
  auto container_fw = in_container->Rebin( {"Centrality", 1, 0.0, 5.0} );
  container_fw = container_fw.Rebin( {branch_name+"_rapidity", 1, 0.05+0.74, 0.15+0.74} );
  container_fw = container_fw.Projection({branch_name+"_pT"});
  container_fw = container_fw*(-2.0);
  auto graph_bw = Qn::DataContainerHelper::ToTGraph(container_bw);
  graph_bw->SetTitle("backward");
  graph_bw->SetLineColor(kRed);
  graph_bw->SetMarkerColor(kRed);
  graph_bw->SetMarkerStyle(kFullCircle);
  auto graph_fw = Qn::DataContainerHelper::ToTGraph(container_fw);
  graph_fw->SetTitle("forward");
  graph_bw->SetLineColor(kBlue);
  graph_bw->SetMarkerColor(kBlue);
  graph_bw->SetMarkerStyle(kFullSquare);
  auto* stack = new TMultiGraph("stack", "; p_{T}, [GeV/c]; v_{1}");
  stack->Add(graph_fw);
  stack->Add(graph_bw);
  auto canvas = new TCanvas( "canv", "", 1000, 1200);
  canvas->cd();
  stack->Draw("AP");
  stack->SetMaximum(0.1);
  stack->SetMinimum(-0.3);
  stack->Draw("AP");
  gPad->BuildLegend();
  TF1 line("zero", "0", 0.0, 2.0);
  line.Draw("same");
  canvas->Print("fuck.png");
  return 0;
}
