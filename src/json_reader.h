//
// Created by mikhail on 8/21/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_JSON_READER_H_
#define FLOW_DRAWING_TOOLS_SRC_JSON_READER_H_

#include <boost/property_tree/ptree.hpp>

#include "draw_functions.h"

namespace JsonConfig{

Draw::Picture GetPictureConfig( const std::string& json_file );

Draw::Histogram2D GetHistogram2DConfig( const std::string& json_file );

std::vector<Draw::Correlation>
GetCorrelationConfigs( const std::string& json_file );

Draw::Style GetSyleConfig( const std::string& json_file );

}

#endif // FLOW_DRAWING_TOOLS_SRC_JSON_READER_H_
