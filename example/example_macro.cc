//
// Created by mikhail on 2/28/21.
//

void example_macro(){
  gROOT->Macro( "/home/mikhail/flow_drawing_tools/example/style.cc" );
  auto corr = new Correlation( "/home/mikhail/OwnQnAnalysis/test-directory/au123_uniform_2021_03_03_SP.root",
                               {"obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).x1x1",
                                "obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).y1y1",
                                "obs.protons_RESCALED.W3_RESCALED(W1_RESCALED,Mb_RESCALED).x1x1",
                                "obs.protons_RESCALED.W3_RESCALED(W1_RESCALED,Mb_RESCALED).y1y1"},
                              "analysis");
  corr->Select( {{"selected_tof_rpc_hits_centrality", 2, 20, 30},
                {"mdc_vtx_tracks_rapidity_pT", 5, 1.0, 1.5}} );
  corr->SetStyle( kBlue, kFullSquare );
  corr->Project( {"mdc_vtx_tracks_rapidity_ycm"} );
  corr->Fit( new TF1( "fit", "pol3", -0.25, 0.25 ) );
  auto graph = new Graph( "/home/mikhail/flow_drawing_tools/macro/v1_y_HADES.root",
                          {"v1_y_1_15"}, "publication");
  Picture pic( "shot", {1000, 1000});
  pic.AddText({0.15, 0.8, "HADES Au+AU@1.23 AGeV"});
  pic.SetAxisTitles({"y_{cm}", "v_{1}"});
  pic.AddDrawable( corr );
  pic.AddDrawable( graph );
  pic.SetYRange({-0.6, 0.6});
  pic.SetXRange({-0.75, 0.85});
  pic.Draw();
  pic.Save(".png");
}