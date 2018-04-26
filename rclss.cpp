#include <iostream>
#include <vector>
#include <string>

#include <dlib/svm_threaded.h>

#include "rclss.h"

Rclss::Rclss(const std::string& aModelFileName, std::istream& aStream)
    : mModelFileName(aModelFileName)
    , mStream(aStream)
{
}

static std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    auto stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

std::vector<Rclss::SampleType> Rclss::ReadInput()
{
    std::vector<SampleType> samples;
    std::string line;
    while (std::getline(mStream, line))
    {
        auto splitted = split(line, ';');
        SampleType sample;
        for (int i = 0; i < 7; ++i)
            sample(i) = std::atof(splitted[i].c_str());
        samples.push_back(sample);
    }
    return samples;
}

void Rclss::Run()
{
    using OvoTrainer = dlib::one_vs_one_trainer<dlib::any_trainer<SampleType> >;
    OvoTrainer trainer;
    using RbfKernel = dlib::radial_basis_kernel<SampleType>;
    dlib::krr_trainer<RbfKernel> rbfTrainer;
    rbfTrainer.set_kernel(RbfKernel(0.1));
    trainer.set_trainer(rbfTrainer);
    std::vector<SampleType> samples = ReadInput();

    using LabelsType = dlib::matrix<double, 6, 1>;
    std::vector<LabelsType> labels;
    for (auto sample : samples)
    {
        LabelsType label;
        if (sample(6) == 1)
           label(0) = 1;
        else
           label(0) = 0;
        labels.push_back(label);
    }
    dlib::one_vs_one_decision_function<OvoTrainer> df = trainer.train(samples, labels);

    dlib::serialize(mModelFileName) << df;
}
