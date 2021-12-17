//
// Created by mikhail on 5/6/21.
//

void dv1dy_diff_systems_centrality(){
  gROOT->Macro( "/home/mikhail/flow_drawing_tools/example/style.cc" );
  auto leg1 = new TLegend( 0.5, 0.55, 0.9, 0.75 );
  auto leg2 = new TLegend( 0.5, 0.15, 0.9, 0.4 );
  auto ag123 = dv1dy_container( "/home/mikhail/OwnQnAnalysis/test-directory/ag123_prod_2021_05_04.root",
                               {"obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).x1x1",
                                "obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).y1y1",
                                "obs.protons_RESCALED.W3_RESCALED(W1_RESCALED,Mb_RESCALED).x1x1",
                                "obs.protons_RESCALED.W3_RESCALED(W1_RESCALED,Mb_RESCALED).y1y1"} );
  ag123.SetSliceVariable("p_{T}", "GeV/c");
  ag123.SetSlopeMarker(kFullCircle);
  ag123.SetOffsetMarker(kOpenCircle);
  ag123.SetPalette({kRed});
  ag123.SetRelativeSysError(0.03);
  ag123.SetSystematicsVariationAxes({ {"mdc_vtx_tracks_rapidity_ycm", 3,  -0.45, -0.15}, {"mdc_vtx_tracks_rapidity_ycm", 3, -0.15, 0.15} });
  ag123.SetXSysError(0.5);
  ag123.Calculate({"event_header_selected_tof_rpc_hits_centrality", 8, 0, 40},
                  {"mdc_vtx_tracks_rapidity_pT", {0.29375, 1.5}},
                  {"mdc_vtx_tracks_rapidity_ycm", 6, -0.45, 0.15});
  ag123.SaveToFile("/home/mikhail/flow_drawing_tools/example/dv1dy/pictures/ag123_fits.root");

  std::cout << "damn" << std::endl;
  auto au123 = dv1dy_container( "/home/mikhail/OwnQnAnalysis/test-directory/au123_prod_2021_05_04.root",
                                   {"obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).x1x1",
                                    "obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).y1y1",
                                    "obs.protons_RESCALED.W3_RESCALED(W1_RESCALED,Mb_RESCALED).x1x1",
                                    "obs.protons_RESCALED.W3_RESCALED(W1_RESCALED,Mb_RESCALED).y1y1"} );
  au123.SetSliceVariable("p_{T}", "GeV/c");
  au123.SetSlopeMarker(kFullSquare);
  au123.SetPalette({kGreen+2});
  au123.SetRelativeSysError(0.03);
  au123.SetSystematicsVariationAxes({ {"mdc_vtx_tracks_rapidity_ycm", 3,  -0.45, -0.15}, {"mdc_vtx_tracks_rapidity_ycm", 3, -0.15, 0.15} });
  au123.SetXSysError(0.5);
  au123.SetOffsetMarker(kOpenSquare);
  au123.SetFormula("pol1");
  au123.Calculate({"event_header_selected_tof_rpc_hits_centrality", 8, 0, 40},
                  {"mdc_vtx_tracks_rapidity_pT", {0.29375, 1.5}},
                  {"mdc_vtx_tracks_rapidity_ycm", 6, -0.45, 0.15});
  au123.SaveToFile("/home/mikhail/flow_drawing_tools/example/dv1dy/pictures/au123_fits.root");

  auto ag158 = dv1dy_container( "/home/mikhail/OwnQnAnalysis/test-directory/ag158_prod_2021_05_04.root",
                               {"obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).x1x1",
                                "obs.protons_RESCALED.W1_RESCALED(W3_RESCALED,Mb_RESCALED).y1y1",
                                "obs.protons_RESCALED.W3_RESCALED(W1_RESCALED,Mb_RESCALED).x1x1",
                                "obs.protons_RESCALED.W3_RESCALED(W1_RESCALED,Mb_RESCALED).y1y1"} );
  ag158.SetSliceVariable("p_{T}", "GeV/c");
  ag158.SetSlopeMarker(kFullCircle);
  ag158.SetOffsetMarker(kOpenCircle);
  ag158.SetPalette({kBlue});
  ag158.SetRelativeSysError(0.03);
  ag158.SetSystematicsVariationAxes({ {"mdc_vtx_tracks_rapidity_ycm", 3,  -0.45, -0.15}, {"mdc_vtx_tracks_rapidity_ycm", 3, -0.15, 0.15} });
  ag158.SetXSysError(0.5);
  ag158.Calculate({"event_header_selected_tof_rpc_hits_centrality", 8, 0, 40},
                  {"mdc_vtx_tracks_rapidity_pT", {0.35625, 1.5}},
                  {"mdc_vtx_tracks_rapidity_ycm", 6, -0.45, 0.15});
  ag158.SaveToFile("/home/mikhail/flow_drawing_tools/example/dv1dy/pictures/ag158_fits.root");

  HeapPicture pic( "dv1dy_diff_systems_centrality", {1000, 1000});

  pic.AddText({0.2, 0.9, "HADES Ag+Ag@1.23A GeV"}, 0.025);
  pic.AddText({0.2, 0.87, "p; pol1 fit"}, 0.025);
  pic.AddText({0.2, 0.84, "w. eff; Qn:RTR; Err:bstrap"}, 0.025);
  pic.AddText({0.2, 0.81, "SP:W1,3[Mb;W3,1]"}, 0.025);
  pic.AddText({0.2, 0.78, "w/o occ. corr."}, 0.025);

  for( auto obj : ag123.GetSlopes() ){
    pic.AddDrawable( obj );
  }
  for( auto obj : ag123.GetSlopesSystematics() ){
    obj->SetErrorOption( "2" );
    pic.AddDrawable( obj );
  }
  for( auto obj : au123.GetSlopes() ){
    pic.AddDrawable( obj );
  }
  std::cout << "damn" << std::endl;
  for( auto obj : au123.GetSlopesSystematics() ){
    obj->SetErrorOption( "2" );
    pic.AddDrawable( obj );
  }
  for( auto obj : ag158.GetSlopes() ){
    pic.AddDrawable( obj );
  }
  for( auto obj : ag158.GetSlopesSystematics() ){
    obj->SetErrorOption( "2" );
    pic.AddDrawable( obj );
  }
  leg2->AddEntry( au123.GetSlopes().at(0)->GetPoints(), "Au+Au@1.23A GeV", "P" );
  leg2->AddEntry( ag123.GetSlopes().at(0)->GetPoints(), "Ag+Ag@1.23A GeV", "P" );
  leg2->AddEntry( ag158.GetSlopes().at(0)->GetPoints(), "Ag+Ag@1.58A GeV", "P" );

  pic.SetAxisTitles({"TOF+RPC centrality (%)", "dv_{1}/dy|_{y=0}"});
//  pic.AddLegend(leg1);
  pic.AddLegend(leg2);

//  pic.DrawZeroLine(false);

  pic.SetXRange({0.0, 40.0});
  pic.SetYRange({0.01, 0.85});
  pic.Draw();
  pic.Save("/home/mikhail/flow_drawing_tools/example/dv1dy/pictures/dv1dy_diff_systems_centrality","png");
  pic.Save("/home/mikhail/flow_drawing_tools/example/dv1dy/pictures/dv1dy_diff_systems_centrality","C");
}