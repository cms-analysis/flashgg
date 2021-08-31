#ifndef flashgg_Taggers_TensorFlowInterface_h
#define flashgg_Taggers_TensorFlowInterface_h
#include "PhysicsTools/TensorFlow/interface/TensorFlow.h"

#include <assert.h>
#include <string> // std::string
#include <vector> // std::vector<>
#include <map> // std::map<,>
#include <Rtypes.h> // Float_t

// forward declarations
namespace tensorflow
{
  class Reader;
  class GraphDef;
  class Session;
  class Status;
  //class MemmappedEnv;
}

class TensorFlowInterface
{
public:
  TensorFlowInterface(const std::string & mvaFileName,
                      const std::vector<std::string> & mvaInputVariables,
                      const std::vector<std::string> classes,
                      const std::vector<double> & mvaInputVariables_mean = {},
                      const std::vector<double> & mvaInputVariables_var = {});
  ~TensorFlowInterface();

  /**
   * @brief Calculates MVA output.
   * @param mvaInputs Values of MVA input variables (stored in std::map with key = MVA input variable name)
   * @return          MVA output
   */
  std::map<std::string, double>
  operator()(const std::map<std::string, double> & mvaInputs) const;

  //tensorflow::Session & getSession() const { return *session_; }

  //using GraphPtr = std::shared_ptr<tensorflow::GraphDef>;
  //const tensorflow::GraphDef & getGraph() const { return *graph_; }

private:
  std::string mvaFileName_;
  tensorflow::GraphDef * graphDef_;
  //int NumberOfInputs;
  //tensorflow::Session * session_;
  //GraphPtr graph_;
  tensorflow::Session * session_;
  const std::vector<std::string> classes_;
  std::string input_layer_name;
  std::string output_layer_name;
  int n_input_layer;
  int n_output_layer;

  std::vector<std::string>  mvaInputVariables_; // key = MVA input variable name
  std::vector<double>  mvaInputVariables_mean_; // key = MVA input variable name
  std::vector<double>  mvaInputVariables_var_; // key = MVA input variable name
  // we do not really care about variables declared as "spectators" during TMVA training,
  // but TMVA requires that we keep track of these variables...
  mutable std::map<std::string, Float_t> spectators_;

  bool isDEBUG_;
};

#endif // TensorFlowInterface_h
