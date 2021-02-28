//
// Created by mikhail on 2/28/21.
//

void example_macro(){
  auto corr = new Correlation( "/home/mikhail/OwnQnAnalysis/test-directory/agag123_standard_2021_02_23_EP.root",
                               {"obs.protons_RESCALED.R1_RESCALED(R1_RESCALED,R2_RESCALED).x1cos1",
                                "obs.protons_RESCALED.R1_RESCALED(R1_RESCALED,R2_RESCALED).y1sin1"},
                              "shit");

}