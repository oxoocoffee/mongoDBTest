#include <stdexcept>
#include <sstream>
#include <iostream>
#include <mongocxx/exception/exception.hpp>

#include "mongoDBTest.h"

int main(int argc, char* argv[])
{
    std::stringstream ss;

    try
    {
        MongoDBTest mongoDbTest;

        mongoDbTest.run(argc, argv);

    }
    catch (const mongocxx::exception& ex)
    {
        ss << std::endl;
        ss << "----------------------------------------" << std::endl
           << "!!!             Exception            !!!" << std::endl
           << "----------------------------------------" << std::endl
           << ex.what() << std::endl;
           PRINT_STACK_TRACE(ss);
        ss << "----------------------------------------" << std::endl;
    }
    catch (const std::exception& ex)
    {
        ss << std::endl;
        ss << "----------------------------------------" << std::endl
           << "!!!             Exception            !!!" << std::endl
           << "----------------------------------------" << std::endl
           << ex.what() << std::endl;
           PRINT_STACK_TRACE(ss);
        ss << "----------------------------------------" << std::endl;
    }
    catch (...)
    {
        ss << std::endl;
        ss << "----------------------------------------" << std::endl
           << "!!!             Exception            !!!" << std::endl
           << "----------------------------------------" << std::endl
           PRINT_STACK_TRACE(ss);
        ss << "----------------------------------------" << std::endl;
    }

    std::cout << ss.str() << std::endl;

    return 0;
}
