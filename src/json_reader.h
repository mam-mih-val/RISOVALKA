//
// Created by mikhail on 8/21/20.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_JSON_READER_H_
#define FLOW_DRAWING_TOOLS_SRC_JSON_READER_H_

#include <boost/property_tree/ptree.hpp>

#include "draw_functions.h"

namespace JsonConfig{

Draw::PictureConfig GetPictureConfig( const std::string& json_file );

Draw::Histogram2DConfig GetHistogram2DConfig( const std::string& json_file );

std::vector<Draw::CorrelationConfig>
GetCorrelationConfigs( const std::string& json_file,
                      const std::string& branch_name="correlations" );

Draw::StyleConfig GetStyleConfig( const std::string& json_file );

}

#endif // FLOW_DRAWING_TOOLS_SRC_JSON_READER_H_
