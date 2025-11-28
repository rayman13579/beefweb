#ifdef MSRV_OS_WINDOWS
#include "../safe_windows.h"
#endif

#include "../log.hpp"
#include "project_info.hpp"

namespace msrv { int testMain(int argc, char** argv); }

int main(int argc, char* argv[])
{
    using namespace msrv;

    StderrLogger logger;
    LoggerScope loggerScope(&logger);

    return testMain(argc, argv);
}
