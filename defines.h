#pragma once

#include <dlib/clustering.h>
#include <dlib/svm_threaded.h>

using SampleType = dlib::matrix<double, 8, 1>;
using SampleVector = std::vector<SampleType>;

using LabelType = double;
using LabelVector = std::vector<LabelType>;

using OvoTrainer = dlib::one_vs_one_trainer<dlib::any_trainer<SampleType>>;
using PolyKernel = dlib::polynomial_kernel<SampleType>;
using RbfKernel = dlib::radial_basis_kernel<SampleType>;
