#include <fstream>

#include "logger.h"

Logger::Logger()
{
}



void Logger::log(std::string toLog)
{
    std::ofstream out;
    out.open("axg.out",std::ofstream::out | std::ofstream::app);

    out << toLog << "\n";

    out.close();

}

