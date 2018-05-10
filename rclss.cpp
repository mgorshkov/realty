#include <iostream>
#include <vector>
#include <string>

#include "rclss.h"
#include "utils.h"

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

SampleVector Rclss::ReadInput()
{
    SampleVector samples;
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
    dlib::one_vs_one_decision_function<OvoTrainer, dlib::decision_function<PolyKernel>, dlib::decision_function<RbfKernel>> df;
    SampleVector samples;
    LabelVector labels;

    std::ifstream inputFile(mModelFileName);
    dlib::deserialize(df, inputFile);
    dlib::deserialize(samples, inputFile);
    dlib::deserialize(labels, inputFile);

    LabelVector labelsSorted = labels;
    std::sort(labelsSorted.begin(), labelsSorted.end());

    SampleVector input = ReadInput();

    for (auto data : input)
    {
        SampleVector foundPoints;
        double label = df(data);
#ifdef DEBUG_PRINT
        std::cout << label << std::endl;
#endif
        if (std::binary_search(labelsSorted.begin(), labelsSorted.end(), label))
        {
            auto it = labels.begin();
            while ((it = std::find_if(it, labels.end(), [label](const auto& l){ return l == label; })) != labels.end())
            {
                std::size_t index = std::distance(labels.begin(), it);
                foundPoints.push_back(samples[index]);
                it++;
            }
        }
        std::sort(foundPoints.begin(), foundPoints.end(),
            [&data](const auto& pointLeft, const auto& pointRight)
            {
                double longitude = data(0);
                double latitude = data(1);

                double longitudeLeft = pointLeft(0);
                double latitudeLeft = pointLeft(1);

                double longitudeRight = pointRight(0);
                double latitudeRight = pointRight(1);

                double distanceLeft = Distance(latitude, longitude, latitudeLeft, longitudeLeft);
                double distanceRight = Distance(latitude, longitude, latitudeRight, longitudeRight);

                return distanceLeft < distanceRight;
            });
        for (auto point : foundPoints)
        {
            // 86.116781;55.335492;2;4326901.00;54.00;7.00;5;5\n
            std::cout << std::fixed << std::setprecision(6)
                << point(0) << ";"
                << point(1) << ";"
                << std::setprecision(0)
                << point(2) << ";"
                << std::setprecision(2)
                << point(3) << ";"
                << point(4) << ";"
                << point(5) << ";"
                << std::setprecision(0)
                << point(6) << ";"
                << point(7) << std::endl;
        }
    }
}
