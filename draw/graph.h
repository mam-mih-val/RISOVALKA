//
// Created by mikhail on 2/28/21.
//

#ifndef FLOW_DRAWING_TOOLS_SRC_GRAPH_H_
#define FLOW_DRAWING_TOOLS_SRC_GRAPH_H_

#include "drawable_object.h"

class Graph : public DrawableObject {
public:
  Graph() = default;
  Graph(const std::string &file_name, const std::vector<std::string> &objects,
        const std::string &title);
  Graph(const std::string &file_name, const std::string &title, const std::string &format="%lg %lg %lg %lg");
  explicit Graph( DrawableObject* );
  ~Graph() override = default;
  void RefreshPoints() override;
  void RecalculateXaxis( const std::vector<double>& x_axis );
  void Scale(double s);
  void ScaleXaxis( double scale );
  void TranslateXaxis( double translation );
  void SetPoints( TGraphErrors* graph ){ points_ = graph; }
};

#endif // FLOW_DRAWING_TOOLS_SRC_GRAPH_H_
