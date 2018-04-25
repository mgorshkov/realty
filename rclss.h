#pragma once

#include <iostream>
#include <dlib/clustering.h>

class Rclss
{
public:
    Rclss(const std::string& aModelFileName, std::istream& aStream);

    void Run();

private:
    using SampleType = dlib::matrix<double, 6, 1>;
    std::vector<SampleType> ReadInput();

    std::string mModelFileName;
    std::istream& mStream;
};
