#include <iostream>

#include "rclst.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: cat inputdata | rclst <numclusters> <modelfname>" << std::endl;
            return 1;
        }

        int numclusters = std::atoi(argv[1]);
        if (numclusters <= 0)
        {
            std::cerr << "Incorrect number of clusters: " << argv[1] << std::endl;
            return 1;
        }

        std::string modelfname = argv[2];

        Rclst rclst(numclusters, modelfname, std::cin);
        rclst.Run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
