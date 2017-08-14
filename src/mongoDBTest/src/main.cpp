#include <stdexcept>
#include <sstream>
#include <iostream>

#include "mongoDBTest.h"

int main(int argc, char* argv[])
{
    try
    {
        MongoDBTest mongoDbTest;

        mongoDbTest.run(argc, argv);

    }
    catch (const std::exception& ex)
    {
        std::cout << std::endl;

        std::stringstream ss; ss << std::endl;
        ss << "----------------------------------------" << std::endl
           << "!!!             Exception            !!!" << std::endl
           << "----------------------------------------" << std::endl
           << ex.what() << std::endl;
           PRINT_STACK_TRACE(ss);
        ss << "----------------------------------------" << std::endl;
    }

    return 0;
}
