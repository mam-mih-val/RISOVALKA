//
// Created by mikhail on 2/28/21.
//

{
  gROOT->Macro( "/home/mikhail/flow_drawing_tools/example/style.cc" );
  auto corr_au123 = new Correlation( "/home/mikhail/OwnQnAnalysis/test-directory/au123_uniform_2021_03_03_SP.root",
                               {"obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).x1x1",
                                "obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).y1y1",
                                "obs.protons_RESCALED.W3_RESCALED(W1_RESCALED,Mb_RESCALED).x1x1",
                                "obs.protons_RESCALED.W3_RESCALED(W1_RESCALED,Mb_RESCALED).y1y1"},
                              "Au+Au@1.23A GeV");
  corr_au123->Select( {{"selected_tof_rpc_hits_centrality", 2, 10, 20},
                {"mdc_vtx_tracks_rapidity_pT", 5, 1.0, 1.5}} );
  corr_au123->SetStyle( kBlack, -1 );
  corr_au123->Project( {"mdc_vtx_tracks_rapidity_ycm"} );
  auto corr_ag123 = new Correlation( "/home/mikhail/OwnQnAnalysis/test-directory/ag123_uniform_2021_03_03_SP.root",
                               {"obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).x1x1",
                                "obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).y1y1",
                                "obs.protons_RESCALED.W3_RESCALED(W1_RESCALED,Mb_RESCALED).x1x1",
                                "obs.protons_RESCALED.W3_RESCALED(W1_RESCALED,Mb_RESCALED).y1y1"},
                              "Ag+Ag@1.23A GeV");
  corr_ag123->Select( {{"selected_tof_rpc_hits_centrality", 2, 10, 20},
                {"mdc_vtx_tracks_rapidity_pT", 5, 1.0, 1.5}} );
  corr_ag123->SetStyle( kBlue, kFullSquare );
  corr_ag123->Project( {"mdc_vtx_tracks_rapidity_ycm"} );

  Ratio<Correlation> pic( "ratio_example", {1000, 1100});
  pic.AddText({0.2, 0.9, "HADES protons"}, 0.035);
  pic.AddText({0.2, 0.86, "0<p_{T}<2.0; -0.25<y_{cm}<0.25"}, 0.03);
  pic.AddText({0.2, 0.82, "w/o eff; Qn:RTR; Err:bstrap"}, 0.03);
  pic.AddText({0.2, 0.78, "SP:W1,3[Mb;W3,1]"}, 0.03);
  pic.SetAxisTitles({"y_{cm}", "v_{1}", "ratio"});
  pic.SetReference( corr_au123 );
  pic.AddObject( corr_ag123 );
  pic.SetYRange({-0.599, 0.599});
  pic.SetXRange({-0.75, 0.85});
  pic.SetRatioRange({0.751, 1.249});
  pic.Draw();
  pic.Save("png");
}