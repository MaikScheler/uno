#include "logger.h"

Logger::Logger()
{
    path = filesystem::current_path() += "/game_" + to_string(rand() % 10000) + ".csv";
    ofstream ofs(path.make_preferred());
    ofs << "Player ID,Event,Data\n";
    ofs.close();
}

void Logger::logToFile(int clientId, QString event, QString data)
{
    ofstream ofs;
    ofs.open(path.make_preferred(), ios_base::app);
    string cId = (clientId == -1) ? "System" : to_string(clientId);

    ofs << cId + "," + event.toStdString() + "," + data.toStdString() + "\n";
    ofs.close();
}
