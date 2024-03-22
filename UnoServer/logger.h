#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <filesystem>
#include <fstream>

using namespace std;


class Logger
{
public:
    Logger();

    void logToFile(int clientId, QString event, QString data = "");

private:
    filesystem::path path;
};

#endif // LOGGER_H
