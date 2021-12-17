//
// Created by mikhail on 5/6/21.
//

void v1_au123_pi_neg_w1_cross_components_pT_y(){
  gROOT->Macro( "/home/mikhail/flow_drawing_tools/example/style.cc" );
  auto leg1 = new TLegend( 0.5, 0.75, 0.9, 0.945 );
  auto leg2 = new TLegend( 0.5, 0.65, 0.9, 0.75 );

  auto v1_xy = DoubleDifferentialCorrelation( "~/Correlations/au123_pi_neg_2021_08_11.root",
                                              {"SP/uQ/pi_neg_RESCALED.W1_RESCALED.x1y1"} );
  v1_xy.SetSliceVariable("p_{T}", "GeV/c");
  v1_xy.SetMarker(kFullCircle);
  v1_xy.SetPalette( {kOrange+1, kSpring-4, kGreen+2, kAzure-4, kRed, kViolet, kBlue } );
  v1_xy.Select({{"event_header_selected_tof_rpc_hits_centrality", 2, 20, 30}});
  v1_xy.SetProjectionAxis({"mdc_vtx_tracks_extra_ycm", 17, -0.65, 1.05});
  v1_xy.SetSliceAxis({"mdc_vtx_tracks_extra_pT", {0, 0.105, 0.155, 0.21, 0.315, 0.535, 1.0}});
  v1_xy.Calculate();

  auto v1_yx = DoubleDifferentialCorrelation( "~/Correlations/au123_pi_neg_2021_08_11.root",
                                              {"SP/uQ/pi_neg_RESCALED.W1_RESCALED.y1x1"} );
  v1_yx.SetSliceVariable("p_{T}", "GeV/c");
  v1_yx.SetMarker(kOpenSquare);
  v1_yx.SetPalette( {kOrange+1, kSpring-4, kGreen+2, kAzure-4, kRed, kViolet, kBlue } );
  v1_yx.Select({{"event_header_selected_tof_rpc_hits_centrality", 2, 20, 30}});
  v1_yx.SetProjectionAxis({"mdc_vtx_tracks_extra_ycm", 17, -0.65, 1.05});
  v1_yx.SetSliceAxis({"mdc_vtx_tracks_extra_pT", {0, 0.105, 0.155, 0.21, 0.315, 0.535, 1.0}});
  v1_yx.Calculate();

  HeapPicture pic( "v1_au123_corrected_y_slices_pT", {1000, 1000});

  pic.AddText({0.2, 0.9, "HADES Au+Au@1.23A GeV"}, 0.025);
  pic.AddText({0.2, 0.87, "#pi^{+}; TOF+RPC 10-20%"}, 0.025);
  pic.AddText({0.2, 0.84, "w. eff; Qn:RTR; Err:bstrap"}, 0.025);
  pic.AddText({0.2, 0.81, "SP:W1[Mb,W3]"}, 0.025);
  pic.AddText({0.2, 0.78, "w/o occ. corr."}, 0.025);

  for( auto obj : v1_xy.GetProjections() ){
    pic.AddDrawable( obj );
    leg1->AddEntry( obj->GetPoints(), obj->GetTitle().c_str(), "P" );
  }

  for( auto obj : v1_yx.GetProjections() ){
    pic.AddDrawable( obj );
//    leg1->AddEntry( obj->GetPoints(), obj->GetTitle().c_str(), "P" );
  }

  leg2->AddEntry( v1_xy.GetProjections().at(0)->GetPoints(), "X1Y1", "P" );
  leg2->AddEntry( v1_yx.GetProjections().at(0)->GetPoints(), "Y1X1", "P" );

  pic.SetAxisTitles({"y", "<uQ>"});
  pic.AddLegend(leg1);
  pic.AddLegend(leg2);

  pic.SetXRange({-0.65, 1.05});
  pic.SetYRange({-0.01, 0.015});
  pic.Draw();
  pic.Save("/home/mikhail/flow_drawing_tools/example/occupancy_correction/pictures/v1_au123_pi_neg_w1_cross_components_pT_y","png");
}