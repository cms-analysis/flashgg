#include "flashgg/Taggers/interface/TensorFlowInterface.h"

#include <boost/algorithm/string.hpp> // boost::contains()

TensorFlowInterface::TensorFlowInterface(const std::string & mvaFileName,
                                         const std::vector<std::string> & mvaInputVariables,
                                         const std::vector<std::string> classes,
                                         const std::vector<double> & mvaInputVariables_mean,
                                         const std::vector<double> & mvaInputVariables_var)
  : mvaFileName_(mvaFileName)
  , graphDef_(nullptr)
  , session_(nullptr)
  , classes_(classes)
  , n_input_layer(0)
  , n_output_layer(0)
  , mvaInputVariables_(mvaInputVariables)
  , mvaInputVariables_mean_(mvaInputVariables_mean)
  , mvaInputVariables_var_(mvaInputVariables_var)
  , isDEBUG_(false)
{
  // loading the model
  tensorflow::SessionOptions options;
  tensorflow::setThreading(options, 1, "no_threads");

  graphDef_ = tensorflow::loadGraphDef(mvaFileName_);
  session_ = tensorflow::createSession(graphDef_);
  std::cout << "Loaded: " << mvaFileName_ << '\n';

  // getting elements to evaluate -- the number of the input/output layer deppends of how the model was exported
  int shape_variables = 0;
  for(int idx_node = 0; idx_node < graphDef_->node_size(); idx_node++)
  {
    input_layer_name  = graphDef_->node(idx_node).name();
    const bool is_input = boost::contains(input_layer_name, "_input");
    if(is_input)
    {
      n_input_layer = idx_node;
      if(isDEBUG_)
      {
        std::cout << "read input layer "<< input_layer_name << " " << n_input_layer << '\n';
      }

      const auto & shape = graphDef_->node(idx_node).attr().at("shape").shape();
      if(isDEBUG_)
      {
        std::cout << "read input layer shape  " << shape.dim_size() << '\n';
      }

      shape_variables = static_cast<int>(shape.dim(1).size());
      if(isDEBUG_)
      {
        std::cout << "read input layer shape length "<< shape_variables << '\n';
      }
      break;
    }
  }

  if(shape_variables != static_cast<int>(mvaInputVariables_.size()))
  {
    std::cout
      << "number of classes declared ("<< mvaInputVariables_.size() << ") does not match the expected inputs for "
         "the given version (" << shape_variables << ')'
    ;
  }

//  int shape_classes = 0;
  for (int idx_node = 0; idx_node < graphDef_->node_size(); idx_node++)
  {
    output_layer_name  = graphDef_->node(idx_node).name();
    const bool is_output = boost::contains(output_layer_name, "/Softmax");
    if(is_output)
    {
      n_output_layer = idx_node;
      if(isDEBUG_)
      {
        std::cout << "read output layer "<< output_layer_name << " " << idx_node << '\n';
      }

//      const auto & shape = graphDef_->node(idx_node-1).attr().at("shape").shape();
//      std::cout << "read output layer shape  " << shape.dim_size() << '\n';
//      shape_classes = static_cast<int>(shape.dim(0).size());
      break;
    }
  }

}

TensorFlowInterface::~TensorFlowInterface()
{
  tensorflow::closeSession(session_);
  delete session_;
  delete graphDef_;
}


std::map<std::string, double>
TensorFlowInterface::operator()(const std::map<std::string, double> & mvaInputs) const
{

  const int nofInputs = mvaInputVariables_.size();
  tensorflow::Tensor inputs(tensorflow::DT_FLOAT, { 1, nofInputs});

  // the order of input variables should be the same as during the training
  for(int idx_input = 0; idx_input < nofInputs; ++idx_input)
  {
    if(mvaInputs.count(mvaInputVariables_[idx_input]))
    {
      if(! mvaInputVariables_mean_.empty())
      {
        inputs.matrix<float>()(0, idx_input) = (
            static_cast<float>(mvaInputs.at(mvaInputVariables_[idx_input])) - mvaInputVariables_mean_[idx_input]
          ) / mvaInputVariables_var_[idx_input];

        if(isDEBUG_)
        {
          std::cout << mvaInputVariables_[idx_input]
            << " = " << mvaInputs.at(mvaInputVariables_[idx_input])
            << " = " << mvaInputVariables_mean_[idx_input]
            << " = " << mvaInputVariables_var_[idx_input]
            << '\n'
          ;
        }
      }
      else
      {
        inputs.matrix<float>()(0, idx_input) = static_cast<float>(mvaInputs.at(mvaInputVariables_[idx_input]));
        if(isDEBUG_)
        {
          std::cout << mvaInputVariables_[idx_input]  << " = " << mvaInputs.at(mvaInputVariables_[idx_input]) << '\n';
        }
      }
    }
    else
    {
      std::cout
        << "Missing value for MVA input variable = '" << mvaInputVariables_[idx_input] << '\''
	<< std::endl;
    }
  }

  // evaluation
  const int node_count = graphDef_->node_size();
  if (isDEBUG_)
  {
    for (int idx_node = 0; idx_node < node_count; ++idx_node)
    {
      const auto node = graphDef_->node(idx_node);
      std::cout << "Names : " << node.name() << '\n';
    }
  }

  std::vector<tensorflow::Tensor> outputs;
  if(isDEBUG_)
  {
    std::cout
      << "start run " << graphDef_->node(n_input_layer).name()
      << " "          << graphDef_->node(n_output_layer).name()
      << '\n'
    ;
  }
  tensorflow::run(
    session_,
    { { graphDef_->node(n_input_layer).name(), inputs } },
    { graphDef_->node(n_output_layer).name() },
    &outputs
  );

  // store the output
  std::map<std::string, double> mvaOutputs;
  for(unsigned int idx_class = 0; idx_class < classes_.size(); idx_class++)
  {
    mvaOutputs[classes_[idx_class]] = outputs[0].matrix<float>()(0, idx_class);
  }

  return mvaOutputs;
}
