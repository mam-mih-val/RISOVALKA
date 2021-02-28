//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_GRAPH_H_
#define FLOW_DRAWING_TOOLS_SRC_GRAPH_H_

#include "drawable_object.h"

class Graph : public DrawableObject {
public:
  Graph(const std::string &file_name, const std::vector<std::string> &objects,
        const std::string &title);

  ~Graph() override;
  virtual TGraphErrors *GetPoints() override;
  ClassDefOverride(Graph, 1)
};

#endif // FLOW_DRAWING_TOOLS_SRC_GRAPH_H_
