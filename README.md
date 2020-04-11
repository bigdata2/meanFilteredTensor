# Mean Filter on N-dimensional Tensor
### An openCV/TensorFlow like kernel in C++ to return mean value of each element in a Tensor 

* Given an N dimensional tensor, this kernel compute a new tensor where each value is the mean value of its neighbors (including itself) in the input tensor.

* The neighbor set includes every value that is 1 index away in all dimensions.

* Implementation is in C++, which can be compiled using g++ -std=c++11 meanFilteredTensor.cpp  

* The input to the the function is a float* (an input tensor) and a dimensions std::vector that conatins the extents in each dimension
