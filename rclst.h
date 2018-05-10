#pragma once

#include <string>
#include <iostream>
#include <dlib/clustering.h>
#include "defines.h"

class Rclst
{
public:
    Rclst(int aNumClusters, const std::string& aModelFileName, std::istream& aStream);

    void Run();

private:
    SampleVector ReadInput();

    int mNumClusters;
    std::string mModelFileName;
    std::istream& mStream;
};
