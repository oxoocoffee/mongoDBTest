#ifndef MONGO_DB_TEST_H
#define MONGO_DB_TEST_H

#include "buildVersion.h"
#include <string>
#include <vector>

#if STACK_TRACING_ENABLED
    #define PRINT_STACK_TRACE(os)           \
        os << "--------- Stack---------\n"  \
           << Stack::GetTraceString() << std::endl;
#else
    #define PRINT_STACK_TRACE(os)
#endif

class MongoDBTest
{
    public:
                 MongoDBTest(void);
        virtual ~MongoDBTest(void);

        void    run(int argc, char* argv[]);

    private:
        void    printUdate(const std::string& appName);

        bool    findArg(const std::string& param ) const;
        bool    findArg(const std::string& param,
                        std::string&       paramVal) const;

        void    doMongoDB(const std::string& host,
                          const std::string& port,
                          const std::string& user,
                          const std::string& password);

    private:
        std::vector<std::string> _argVec;
};

#endif // MONGO_DB_TEST_H
