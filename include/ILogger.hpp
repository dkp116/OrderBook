#ifndef ILOGGER_HPP
#define ILOGGER_HPP

class ILogger
{
public:
    virtual ~ILogger() = default;

    virtual void logTrade(double bidPrice,
                          double askPrice,
                          int quantity) = 0;
};

#endif