#include "rasllo.h"
#include "keyboardhandler.h"
#include "argumentparser.h"
#include <QApplication>
#include <QtGlobal>


void RemoveOldLogs(int NumberOfStoredLog)
{
    QDir myDir(QDir::currentPath());
    myDir.setNameFilters(QStringList()<<"Rasllo-*");
    QStringList filesList = myDir.entryList();
    for (int i = 0; i < filesList.size()-NumberOfStoredLog; ++i) {
        qDebug() << "removing old LogFile: " << filesList[i];
        myDir.remove(filesList[i]);
    }
}
QString outputFileName;

void Handler(QtMsgType type, const QMessageLogContext &, const QString &msg)
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
    break;
default:
    txt = QString("Unknow%1: %2").arg(QDateTime::currentDateTime().toString("yy-MM-dd hh:mm:ss")).arg(msg);
    qDebug() << msg;
    break;
}
QFile outFile(outputFileName);
outFile.open(QIODevice::WriteOnly | QIODevice::Append);
QTextStream ts(&outFile);
ts << txt << endl;
}


int main(int argc, char *argv[])
{
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
    QString programVersion = "17.11.23"; // New:
    int displayID = 0, logs = 10, portNumber = 15000;
    bool isEmulator = false;
    if(!ArgumentParser::Parse(argc, argv, &portNumber, &displayID, &isEmulator, &logs, Layout::XMLVer() + " - " + programVersion))
        return 0;
    outputFileName= "Rasllo-"+QDate::currentDate().toString("yyyy-MM-dd")+"-p"+QString::number(portNumber)+".log";

    QApplication a(argc, argv);
    qInstallMessageHandler(&Handler);
    KeyboardHandler *keyboardCardRead = new KeyboardHandler;
    Rasllo *rasllo = new Rasllo();
    rasllo->Init(keyboardCardRead, isEmulator, portNumber, Layout::XMLVer() + " - " + programVersion);


    if(isEmulator)
    {
        rasllo->layout->show();
        server->layout->setFixedSize(800, 480);
    }
    else
    {
        // if I set geometry, app window doesn't have border with close button
        QDesktopWidget *desktop = new QDesktopWidget();
        if (displayID < desktop->screenCount())
            server->layout->setGeometry(desktop->screenGeometry(displayID));
        else
            qWarning() << "displayID is greater than number of displays";
        server->layout->showFullScreen();
        QApplication::setOverrideCursor(Qt::BlankCursor);
    }

    a.installEventFilter(keyboardCardRead);
    QTimer::singleShot(0, server, SLOT(Start()));

    RemoveOldLogs(logs);
    return a.exec();
}
