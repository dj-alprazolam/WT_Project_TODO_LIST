#include "todo.hpp"
#include <Wt/WEnvironment.h>
#include <memory>
#include <iostream>

int main(int argc, char** argv) {
    std::cerr << "=== START ===" << std::endl;

    const char* const v[8] = {
        argv[0],
        "--docroot", ".",
        "--http-address", "127.0.0.1",
        "--http-port", "8080",
        "--accesslog=-"          
    };

    std::cerr << "Calling WRun..." << std::endl;

    int rc = Wt::WRun(8, const_cast<char**>(v), [](const Wt::WEnvironment& env) {
        return std::make_unique<todo>(env);
    });

    std::cerr << "WRun returned: " << rc << std::endl;
    return rc;
}