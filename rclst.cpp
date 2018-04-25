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
    std::vector<SampleType> samples = ReadInput();

}
