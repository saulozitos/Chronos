#include "datetime.h"

#include <array>
#include <sstream>
#include <iomanip>

namespace Constants
{
constexpr auto oneDayInSeconds{86400};
constexpr auto msToSec{1000.00};
}

DateTime::DateTime(const DateTime::DateFormat dt)
{
    switch (dt) {
    case DateFormat::YYYYMMDD:
        m_dateFormat = "%Y-%m-%d";
        break;
    case DateFormat::DDMMYYYY:
        m_dateFormat = "%d-%m-%Y";
        break;
    case DateFormat::YYMMDD:
        m_dateFormat = "%y-%m-%d";
        break;
    case DateFormat::DDMMYY:
        m_dateFormat = "%d-%m-%y";
        break;
    default:
        break;
    }
}

DateTime::DateTime(std::chrono::nanoseconds amountTime)
{
    using namespace std::chrono;
    using days = duration<int, std::ratio<Constants::oneDayInSeconds>>;

    m_day = duration_cast<days>(amountTime);
    amountTime -= m_day;
    m_hour = duration_cast<hours>(amountTime);
    amountTime -= m_hour;
    m_minute = duration_cast<minutes>(amountTime);
    amountTime -= m_minute;
    m_second = duration_cast<seconds>(amountTime);
    amountTime -= m_second;
    m_mSecond = duration_cast<milliseconds>(amountTime);
}

std::string DateTime::getCurrentDateTimeToString()
{
    loadValues();

    std::array<char, sizeof (m_bt)>buffer{};
    buffer.fill(0);

    std::strftime(buffer.data(), sizeof(buffer), "%Y-%m-%d ", &m_bt);

    std::ostringstream oss;
    oss << buffer.data();
    oss << std::put_time(&m_bt, "%H:%M:%S"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << m_ms.count();

    return oss.str();
}

std::string DateTime::getCurrentTimeToString()
{
    loadValues();
    std::ostringstream oss;
    oss << std::put_time(&m_bt, "%H:%M:%S"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << m_ms.count();
    return oss.str();
}

std::string DateTime::getCurrentDateToString()
{
    loadValues();
    std::array<char, sizeof (m_bt)>buffer{};
    buffer.fill(0);

    std::strftime(buffer.data(), sizeof(buffer), m_dateFormat.c_str(), &m_bt);

    return buffer.data();
}

bool DateTime::isLeapYear(const unsigned int year)
{
    return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
}

std::chrono::system_clock::time_point DateTime::addDays(std::chrono::system_clock::time_point date, const int value)
{
    return date + std::chrono::hours(value * 24);
}

void DateTime::loadValues()
{
    // get current time
    m_now = std::chrono::system_clock::now();
    // get number of milliseconds for the current second (remainder after division into seconds)
    constexpr auto msToSec{1000};
    m_ms = std::chrono::duration_cast<std::chrono::milliseconds>(m_now.time_since_epoch()) % msToSec;
    // convert to std::time_t in order to convert to std::tm (broken time)
    const auto timer = std::chrono::system_clock::to_time_t(m_now);
    // convert to broken time
    m_bt = *std::localtime(&timer);
}

int64_t DateTime::currentMSecsSinceEpoch()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::chrono::duration<int, std::ratio<86400, 1> > DateTime::day() const
{
    return m_day;
}

std::chrono::duration<long, std::ratio<3600, 1> > DateTime::hour() const
{
    return m_hour;
}

std::chrono::duration<long, std::ratio<60, 1> > DateTime::minute() const
{
    return m_minute;
}

std::chrono::duration<long, std::ratio<1, 1> > DateTime::second() const
{
    return m_second;
}

std::chrono::duration<long, std::ratio<1, 1000> > DateTime::mSecond() const
{
    return m_mSecond;
}

