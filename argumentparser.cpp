#include "argumentparser.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
ArgumentParser::ArgumentParser()
{
}

bool ArgumentParser::Parse(int argc, char *argv[], int* portNumber, int* displayID, bool* emulator, int* logs, QString programVersion)
{
        for (int i = 1; i < argc; i++) // prvy argument je nazov programu
        {
            if (!strcmp(argv[i], "-h"))
            {
                QFile f(":/help.txt");
                if (f.open(QFile::ReadOnly))
                {
                    QTextStream in(&f);
#ifdef ForRaspberryPi
                    qDebug() << in.readAll();
#else
                    qDebug().noquote() << in.readAll();
#endif
                }
                return false;
            }
            else if (!strcmp(argv[i], "-p"))
            {
                if (argc > i+1)
                {
                    *portNumber = atoi(argv[++i]);
                }
                else
                {
                    qWarning() << "Port number is missing after argument -p";
                    return false;
                }
            }
            else if (!strcmp(argv[i], "-d"))
            {
                if (argc > i+1)
                {
                    *displayID = atoi(argv[++i]);
                }
                else
                {
                    qWarning() << "Display number is missing after argument -v";
                    return false;
                }
            }
            else if (!strcmp(argv[i], "-v"))
            {
                qDebug() << "Verzia programu: " << programVersion;
                return false;
            }
            else if (!strcmp(argv[i], "-e"))
            {
                *emulator = true;
            }
            else if (!strcmp(argv[i], "-l"))
            {
                if (argc > i+1)
                {
                    *logs = atoi(argv[++i]);
                }
                else
                {
                    qWarning() << "Number of stored logs is missing after argument -l";
                    return false;
                }
            }
            else
            {
                qWarning() << "Unknown argument: " << argv[i];
            }
        }
        return true;
}
