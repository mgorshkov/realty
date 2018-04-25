#pragma once

#include <string>
#include <iostream>
#include <dlib/clustering.h>

class Rclst
{
public:
    Rclst(int aNumClusters, const std::string& aModelFileName, std::istream& aStream);

    void Run();

private:
    using SampleType = dlib::matrix<double, 7, 1>;
    std::vector<SampleType> ReadInput();

    int mNumClusters;
    std::string mModelFileName;
    std::istream& mStream;
};
