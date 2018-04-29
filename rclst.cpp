#include <iostream>
#include <vector>

#include <dlib/svm_threaded.h>

#include "rclst.h"

Rclst::Rclst(int aNumClusters, const std::string& aModelFileName, std::istream& aStream)
    : mNumClusters(aNumClusters)
    , mModelFileName(aModelFileName)
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

std::vector<Rclst::SampleType> Rclst::ReadInput()
{
    std::vector<SampleType> samples;
    std::string line;
    while (std::getline(mStream, line))
    {
        auto splitted = split(line, ';');
        SampleType sample;
        for (int i = 0; i < 6; ++i)       
            sample(i) = std::atof(splitted[i].c_str());
        double flag = (splitted[6] == splitted[7] || splitted[6] == "1") ? 0 : 1;
        sample(6) = flag;
        samples.push_back(sample);
    }
    return samples;
}

void Rclst::Run()
{
    using OvoTrainer = dlib::one_vs_one_trainer<dlib::any_trainer<SampleType>>;
    OvoTrainer trainer;
    using PolyKernel = dlib::polynomial_kernel<SampleType>;
    using RbfKernel = dlib::radial_basis_kernel<SampleType>;
    dlib::krr_trainer<RbfKernel> rbfTrainer;
    rbfTrainer.set_kernel(RbfKernel(0.1));
    trainer.set_trainer(rbfTrainer);
    std::vector<SampleType> samples = ReadInput();

    using LabelsType = std::vector<double>;
    LabelsType labels;
    for (auto sample : samples)
    {
        // 86.116781;55.335492;2;4326901.00;54.00;7.00;5;5 
        double longitude = sample(0);
        double latitude = sample(1);
        int rooms = sample(2);
        int priceCategory;
        double price = sample(3);
        if (price <= 1000000.0)
            priceCategory = 1;
        else if (price <= 5000000.0)
            priceCategory = 2;
        else if (price <= 10000000.0)
            priceCategory = 3;
        else
            priceCategory = 4;
        double totalArea = sample(4);
        int totalAreaCategory;
        if (totalArea <= 10.0)
            totalAreaCategory = 1;
        else if (totalArea <= 20.0)
            totalAreaCategory = 2;
        else if (totalArea <= 30.0)
            totalAreaCategory = 3;
        else if (totalArea <= 40.0)
            totalAreaCategory = 4;
        else if (totalArea <= 50.0)
            totalAreaCategory = 5;
        else if (totalArea <= 60.0)
            totalAreaCategory = 6;
        else if (totalArea <= 70.0)
            totalAreaCategory = 7;
        else if (totalArea <= 100.0)
            totalAreaCategory = 8;
        else
            totalAreaCategory = 9;
        double kitchenArea = sample(5);
        int kitchenAreaCategory;
        if (kitchenArea <= 6.0)
            kitchenAreaCategory = 1;
        else if (kitchenArea <= 8.0)
            kitchenAreaCategory = 2;
        else if (kitchenArea <= 10.0)
            kitchenAreaCategory = 3;
        else if (kitchenArea <= 20.0)
            kitchenAreaCategory = 4;
        else if (kitchenArea <= 30.0)
            kitchenAreaCategory = 5;
        else
            kitchenAreaCategory = 6;
        int isFirstLast = sample(6) ? 1 : 0;

        double label = rooms + priceCategory * 10 + totalAreaCategory * 100 + kitchenAreaCategory * 1000 + isFirstLast * 10000;
        labels.push_back(label);
    }
    dlib::one_vs_one_decision_function<OvoTrainer, dlib::decision_function<PolyKernel>, dlib::decision_function<RbfKernel>> df = trainer.train(samples, labels);

    dlib::serialize(mModelFileName) << df;
}
