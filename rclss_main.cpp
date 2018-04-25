#include <iostream>
#include <string>

#include "rclss.h"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: cat inputdata | rclss <modelfname>" << std::endl;
            return 1;
        }

        std::string modelfname = argv[1];

        Rclss rclss(modelfname, std::cin);
        rclss.Run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
