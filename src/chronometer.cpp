#include "chronometer.h"

#include <sstream>
#include <iomanip>

using namespace std::chrono;

namespace Constants
{
    constexpr auto oneDayInSeconds{86400};
    constexpr auto msToSec{1000.00};
}

Chronometer::Chronometer()
{
    start();
}

std::string Chronometer::formateTime(std::chrono::nanoseconds ns)
{
    using days = duration<int, std::ratio<Constants::oneDayInSeconds>>;

    auto d = duration_cast<days>(ns);
    ns -= d;
    auto h = duration_cast<hours>(ns);
    ns -= h;
    auto m = duration_cast<minutes>(ns);
    ns -= m;
    auto s = duration_cast<seconds>(ns);
    ns -= s;
    auto ms = duration_cast<milliseconds>(ns);

    std::ostringstream os;
    os << std::setfill('0') << std::setw(2) << d.count() << "d:"
       << std::setfill('0') << std::setw(2) << h.count() << ":"
       << std::setfill('0') << std::setw(2) << m.count() << ":"
       << std::setfill('0') << std::setw(2) << s.count() << "."
       << std::setfill('0') << std::setw(3) << ms.count();

    return os.str();
}

std::chrono::time_point<std::chrono::system_clock> Chronometer::getTimePointNow()
{
    return m_isRunning ? system_clock::now() : m_EndTime;
}

void Chronometer::start()
{
    if(!m_isRunning)
    {
        m_StartTime = system_clock::now();
        m_isRunning = true;
    }
}

void Chronometer::stop()
{
    if(m_isRunning)
    {
        m_EndTime = system_clock::now();
        m_isRunning = false;
    }
}

void Chronometer::restart()
{
    if(m_isRunning)
    {
        stop();
        start();
    }
}

double Chronometer::elapsedMilliseconds()
{
    return duration_cast<milliseconds>(getTimePointNow() - m_StartTime).count();
}

double Chronometer::elapsedSeconds()
{
    return elapsedMilliseconds() / Constants::msToSec;
}

std::string Chronometer::elapsedTimeToString()
{
    return formateTime(duration_cast<nanoseconds>(getTimePointNow() - m_StartTime));
}
