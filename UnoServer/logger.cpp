#include "logger.h"

/*
* Legt eine Datei fürs Loggen an
*/
Logger::Logger()
{
    //path = filesystem::current_path() += "/game_" + to_string(rand() % 10000) + ".csv";
    //ofstream ofs(path.make_preferred());
    //ofs << "Player ID,Event,Data,Time\n";
    //ofs.close();
}

/*
* Schreibt eine Zeile in die CSV
*/
void Logger::logToFile(int clientId, QString event, QString data)
{
    //ofstream ofs;
    //ofs.open(path.make_preferred(), ios_base::app);
    //string cId = (clientId == -1) ? "system" : to_string(clientId);

    //auto t = time(nullptr);
    //auto tm = *localtime(&t);
    //std::ostringstream oss;
    //oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    //auto timeStr = oss.str();

    //ofs << cId + "," + event.toStdString() + "," + data.toStdString() + "," + timeStr + "\n";
    //ofs.close();
}
