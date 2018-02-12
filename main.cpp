#include "rallo.h"
#include "keyboardhandler.h"
#include "argumentparser.h"
#include <QApplication>
#include <QtGlobal>

const QString RPI_LOGFILE_PATHPREFIX = "/home/rallo/logs/";
QString outputFileName;

void RemoveOldLogs(int NumberOfStoredLog)
{
#ifdef ForRaspberryPi
    QDir myDir(RPI_LOGFILE_PATHPREFIX);
#else
    QDir myDir(QDir::currentPath());
#endif
    myDir.setNameFilters(QStringList()<<"Rallo-*");
    QStringList filesList = myDir.entryList();
    for (int i = 0; i < filesList.size()-NumberOfStoredLog; ++i) {
        qDebug() << "removing old LogFile: " << filesList[i];
        myDir.remove(filesList[i]);
    }
}

void LogHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg: // qDebug()
        txt = QString("Debug %1: %2").arg(QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")).arg(msg);
        qDebug() << txt;
        break;
    case QtWarningMsg: // qWarning()
        txt = QString("Warni %1: %2").arg(QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")).arg(msg);
        qWarning() << txt;
        break;
    case QtCriticalMsg: // qCritical()
        txt = QString("Criti %1: %2").arg(QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")).arg(msg);
        qCritical() << msg;
        break;
    case QtFatalMsg: // qFatal()
        txt = QString("Fatal %1: %2").arg(QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")).arg(msg);
        qFatal(txt.toStdString().c_str());
        abort();
    default:
        txt = QString("Unknow%1: %2").arg(QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")).arg(msg);
        qDebug() << msg;
        break;
    }
    QFile outFile(outputFileName);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
    return;
}


int main(int argc, char *argv[])
{
    Config *config = new Config();
    //verzia - 003"; // odtesotavan na RPi dlhodobo
    //verzia - 010"; // implementovane styly aplikacie
    //verzia - 020"; // asynchronny qmessage box
    //verzia - 031"; // pridany argument nastavenia portu (-p 123 / --port 123), pridany DNS nazov
    //verzia - 040"; // nastavovanie rowstretch a columnstretch
    //verzia - 050"; // nastavovanie zmena casu (root) + rozdelenie LOG
    // 17.04.05"; // nova init obrazovka
    // 17.04.06"; // opravena chyba ak sa pripajali dva zariadenia naraz tak server spadol
    // 17.04.14"; // opravena pri nacitani Init obrazovky padala aplikacia
    // 17.05.07"; // oprava SIGSEG u nastavovania VBoxLayout
    // 17.07.14";
    // 17.08.20"; // Pridany parameter -d [display], -e [emulator], Log subor obsahuje port v nazve, layout pridana modifikacia pre QWidget, pri BEEP sa nezobrazuje MSGbox na Windows
    // 17.09.12"; // Pre windows sa beep zvuk nasobi 11x
    // 17.09.19"; // pre Windows prehrava zvuky beep.vaw a error.vaw
    // 17.11.23"; // Implementacia Modalneho okna
    // 17.12.25"; // New: not acknowledge to messages RPO->RLO
    // 17.12.27"; // New: SerialRead, location of LOGs in RPI
    // 18.01.29"; // New: Build for Windows
    config->programVersion = "18.02.12"; // New: PRO file specified for Windows/Linux
    outputFileName = "";
#ifdef UseAbsoluteLocation
    outputFileName = RPI_LOGFILE_PATHPREFIX;
#endif
    outputFileName += "Rallo-"+QDate::currentDate().toString("yyyy-MM-dd")+"-p"+QString::number(config->portNumber)+".log";
    qInstallMessageHandler(LogHandler);

    if(!ArgumentParser::Parse(argc, argv, config))
        return 0;
    QApplication a(argc, argv);
    KeyboardHandler *keyboardCardRead = new KeyboardHandler;
    Rallo *rallo = new Rallo(config);
    rallo->Init(keyboardCardRead);
    a.installEventFilter(keyboardCardRead);
    QTimer::singleShot(0, rallo, SLOT(Start()));
    RemoveOldLogs(config->logs);
    return a.exec();
}
