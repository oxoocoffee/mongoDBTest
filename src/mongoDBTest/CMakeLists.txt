cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
set(DESCRIPTION "mongoDB Testt Application")

set(APP_CLIENT_NAME mongoDBTest)
 
message(STATUS "= Building ${DESCRIPTION}")

# Include *.h files to VS or Qt IDE can edit
file(GLOB client_SRCS src/*.cpp include/*.h)

list_files( "${client_SRCS}" )

include_directories(include
                    ${APP_CONFIG_SRC_PATH}) # include buildVersion.h

# add the executable
add_executable(${APP_CLIENT_NAME} ${client_SRCS})

target_link_libraries(${APP_CLIENT_NAME} ${LIBMONGOCXX_LIBRARIES} pthread )

set_target_properties( ${APP_CLIENT_NAME}
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${APP_CONFIG_BIN_PATH} )

install (TARGETS ${APP_CLIENT_NAME} DESTINATION bin)

