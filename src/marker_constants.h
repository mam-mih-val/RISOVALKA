//
// Created by mikhail on 6/21/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_MARKER_CONSTANTS_H_
#define FLOW_DRAWING_TOOLS_SRC_MARKER_CONSTANTS_H_

#include <TAttMarker.h>
#include <TColor.h>

namespace MarkerConstants{
const std::map<std::string, int> COLORS{
    {"red", kRed},
    {"orange", kOrange},
    {"yellow", kYellow},
    {"green", kGreen+2},
    {"blue", kBlue},
    {"deep sea", kDeepSea},
    {"violet", kViolet},
    {"black", kBlack},
    {"magenta", kMagenta},
};
const std::map<std::string, int> MARKERS{
    {"open circle", kOpenCircle},
    {"full circle", kFullCircle},
    {"open square", kOpenSquare},
    {"full square", kFullSquare},
    {"open triangle up", kOpenTriangleUp},
    {"full triangle up", kFullTriangleUp},
    {"open triangle down", kOpenTriangleDown},
    {"full triangle down", kFullTriangleDown},
    {"open cross", kOpenCross},
    {"full cross", kFullCross},
    {"open cross x", kOpenCrossX},
    {"full cross x", kFullCrossX},
    {"open star", kOpenStar},
    {"full star", kFullStar},
    {"four squares x", kFourSquaresX},
    {"four squares +", kFourSquaresPlus},
};
}

#endif // FLOW_DRAWING_TOOLS_SRC_MARKER_CONSTANTS_H_
