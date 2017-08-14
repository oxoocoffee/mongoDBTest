#include "mongoDBTest.h"
#include "buildVersion.h"
#include <iostream>
#include <sstream>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

MongoDBTest::MongoDBTest(void)
{
    STACK
}

MongoDBTest::~MongoDBTest(void)
{
    STACK
}

void MongoDBTest::run(int argc, char* argv[])
{
    STACK

    if(argc < 2)
    {
        printUdate(argv[0]);
        return;
    }

    for(int32_t i = 0; i < argc; i++)
        _argVec.push_back( argv[i] );

    std::string host;
    std::string port;
    std::string user;
    std::string password;

    if(findArg("-h", host) == false )
        host = "127.0.0.1";

    if(findArg("-p", port) == false )
        port = "27017";

    if(findArg("-u", user) == false )
    {
        printUdate(argv[0]);
        std::cout << std::endl << "Missing -u switch" << std::endl;
        return;
    }

    if( user.empty() )
    {
        printUdate(argv[0]);
        std::cout << std::endl << "Missing -u parameter switch" << std::endl;
        return;
    }

    if(findArg("-s", password) == false )
    {
        printUdate(argv[0]);
        std::cout << std::endl << "Missing -p switch" << std::endl;
        return;
    }

    if( password.empty() )
    {
        printUdate(argv[0]);
        std::cout << std::endl << "Missing -p parameter switch" << std::endl;
        return;
    }

    std::string        dbName("testdb");
    std::ostringstream uri;

    uri << "mongodb://" << user << ":" << password << "@" << host << ":" << port
        << "/" << dbName << "?authSource=" << dbName;

    std::string b = uri.str();

    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{ uri.str() }};

    bsoncxx::builder::stream::document document{};

    auto collection = conn["testdb"]["testcollection"];
    document << "hello" << "world";

    collection.insert_one(document.view());
    auto cursor = collection.find({});

    for (auto&& doc : cursor) {
        std::cout << bsoncxx::to_json(doc) << std::endl;
    }

}

void MongoDBTest::printUdate(const std::string& appName)
{
    std::cout << "Usage: " << appName << " -h host -p port -u user -s password" << std::endl;
}

bool MongoDBTest::findArg(const std::string& param ) const
{
    STACK

    auto iter = find( _argVec.begin(), _argVec.end(), param);

    if( iter != _argVec.end() )
        return true;

    return false;
}

bool MongoDBTest::findArg(const std::string& param, std::string& paramVal) const
{
    STACK

    paramVal.clear();

    auto iter = find( _argVec.begin(), _argVec.end(), param);

    if( iter == _argVec.end() )
        return false;

    ++iter;

    // Check if there is another item in list (paramVal)
    if( iter != _argVec.end() )
    {
        if( iter->empty() == false)
        {
            if( (*iter)[0] != '-' )
            {
                paramVal = *iter;

                if(paramVal.empty())
                    throw std::runtime_error("Failed to parse " + param +
                                             " parameter: empty");
            }
        }
    }

    return true;
}
