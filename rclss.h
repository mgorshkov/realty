#pragma once

#include <iostream>
#include "defines.h"

class Rclss
{
public:
    Rclss(const std::string& aModelFileName, std::istream& aStream);

    void Run();

private:
    SampleVector ReadInput();

    std::string mModelFileName;
    std::istream& mStream;
};
