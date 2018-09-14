#include "myvisualizer.h"

MyVisualizer::MyVisualizer()
{
  wrapper = &(GMlibWrapper::instance());
  if (wrapper == nullptr)
  {
    //If this happens, we tried to get the wrapper too early
    throw std::runtime_error("Wrapper should not be nullptr, check if the visualizer was constructed too early");
  }
}
