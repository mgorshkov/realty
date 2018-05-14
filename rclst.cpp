#include <iostream>
#include <vector>

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

SampleVector Rclst::ReadInput()
{
    SampleVector samples;
    std::string line;
    while (std::getline(mStream, line))
    {
        // 86.116781;55.335492;2;4326901.00;54.00;7.00;5;5
        auto splitted = split(line, ';');
#ifdef DEBUG_PRINT
        std::cout << splitted.size() << std::endl;
#endif
        SampleType sample;
        for (int i = 0; i < 8; ++i)
            sample(i) = std::atof(splitted[i].c_str());
        double firstLastFloor = (splitted[6] == splitted[7] || splitted[6] == "1") ? 0 : 1;
#ifdef DEBUG_PRINT
        std::cout << splitted[6] << " " << splitted[7] << " " << firstLastFloor << std::endl;
#endif
        sample(8) = firstLastFloor;
        samples.push_back(sample);
    }
    return samples;
}

void Rclst::Run()
{
    OvoTrainer trainer;

    dlib::svr_linear_trainer<LinearKernel> linearTrainer;

    LinearKernel kernel;
    trainer.set_trainer(linearTrainer);

    SampleVector samples = ReadInput();

    std::vector<SampleType> initialCenters;

    dlib::pick_initial_centers(mNumClusters, initialCenters, samples, kernel);

    std::vector<unsigned long> assignments = spectral_cluster(kernel, samples, mNumClusters);

    LabelVector labels;
    for (auto a : assignments)
    {
        std::cout << a << std::endl;
        labels.push_back(a);
    }

    dlib::one_vs_one_decision_function<OvoTrainer, dlib::decision_function<LinearKernel>> df = trainer.train(samples, labels);
#ifdef DEBUG_PRINT
    for (int i = 0; i < samples.size(); ++i)
        std::cout << "predicted label: "<< df(samples[i])  << ", true label: "<< labels[i] << std::endl;
#endif
    std::ofstream output(mModelFileName);
    dlib::serialize(df, output);
    dlib::serialize(samples, output);
    dlib::serialize(labels, output);
}
