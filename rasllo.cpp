#include "rasllo.h"

Rasllo::Rasllo(Config *appConfig, QWidget *) : networkSession()
{
    this->AppConfig = appConfig;
    layout = new Layout(appConfig);
    tcpServer = new QTcpServer();



}
void Rasllo::closeAllConnections()
{
    mutex.lock();
    while (!clientConnection.empty())
    {
        clientConnection.back()->close();
        clientConnection.pop_back();
        //client.close();
    }
    mutex.unlock();
}

Rasllo::~Rasllo()
{
    qDebug() << "Rasllo sa VYPINA................";
    if (layout != nullptr)
        delete layout;
    if (networkSession != nullptr)
    {
        networkSession->close();
        delete networkSession;
    }
    if (tcpServer != nullptr)
    {
        tcpServer->close();
        delete tcpServer;
    }
    closeAllConnections();
}

void Rasllo::Init(KeyboardHandler *keyboardCardRead)
{
    connect(layout, SIGNAL(MessageToSend(QString)), this, SLOT(sendMessage(QString)));
    connect(layout->modalWindow, SIGNAL(MessageToSend(QString)), this, SLOT(sendMessage(QString)));
    connect(keyboardCardRead, SIGNAL(CardReaded(QString)), this, SLOT(sendMessage(QString)));
    layout->Init();
#ifdef ForRaspberryPi
    QProcess proc;
    proc.start("xset s off");
    proc.waitForFinished();
    proc.start("xset -dpms");
    proc.waitForFinished();
    proc.start("xset s noblank");
    proc.waitForFinished();
#endif
}

void Rasllo::Start()
{
    qDebug() << "Rasllo starting TCP server, version: " << AppConfig->programVersion;


    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config);
        QObject::connect(networkSession, &QNetworkSession::opened, this, &Rasllo::sessionOpened);

        // zavola to tiez sessionOpened() cez slot
        networkSession->open();
    } else {
        sessionOpened();
    }
    QObject::connect(tcpServer, &QTcpServer::newConnection, this, &Rasllo::newConnectionHandler);
}

void Rasllo::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration netConfig = networkSession->configuration();
        QString id;
        if (netConfig.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = netConfig.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    if (!tcpServer->listen(QHostAddress::Any, AppConfig->portNumber))
    {
        layout->ShowMessage(UnableToStartServer + tcpServer->errorString());
    }
    else
    {
        layout->ShowMessage(WaitingForServerToConnect);
    }
}


void Rasllo::newConnectionHandler()
{
    closeAllConnections();
    layout->StopTimers();
    inputData.clear();
    mutex.lock();
    clientConnection.push_back(tcpServer->nextPendingConnection());
    qDebug() << "Novy server sa pripojil s IP: " + clientConnection.back()->peerAddress().toString() + " name: " + clientConnection.back()->peerName();

    connect(clientConnection.back(), SIGNAL(readyRead()), this, SLOT(readFromServer()));
    connect(clientConnection.back(), SIGNAL(disconnected()), this, SLOT(clientDisconected()));
    mutex.unlock();
}

void Rasllo::readFromServer()
{
    if (clientConnection.empty())
    {
        inputData = "";
        return;
    }
    while (clientConnection.back()->bytesAvailable())
    {
        inputData += QString::fromLocal8Bit(clientConnection.back()->readAll());
    }
    if (layout->DebugIOData)
        qDebug() << "Receiving data started";
    if (layout->DebugIOData)
        qDebug() << "Stored data + Received: " << inputData;

    while (1)
    {
        int startIndex = inputData.indexOf("<RPI");
        if (startIndex == -1)
        {
            inputData.clear();
            if (layout->DebugWarnings)
                qWarning() << "No START tag <RPI>";
            break;
        }
        else if (startIndex > 0)
        {
            if (layout->DebugWarnings)
                qWarning() << "Removed " << startIndex << "x first characters";
            inputData = inputData.remove(0,startIndex);
        }

        // ak sprava neobsahuje koncovy TAG /RPI
        int endIndex = inputData.indexOf("</RPI>");
        if (endIndex == -1)
            break;

        QXmlStreamReader *xml = new QXmlStreamReader(inputData.mid(0, endIndex));
        layout->ParseXmlData(xml);
        delete(xml);

        // orez priate data ktore boli spracovane a pokracuj so zvyskom spravy
        inputData = inputData.mid(endIndex+6);
        if (inputData.isEmpty())
            break;
    }
}

bool Rasllo::SendMessageToServer(QString message)
{
    if (clientConnection.empty())
        return false;
    if (clientConnection.back()->write(message.toLocal8Bit()) == -1)
    {
        if (layout->DebugIOData)
            qDebug() << "SendResponse Local8Bit error: " << message;
        return false;
    }
    else
    {
        if (layout->DebugIOData)
            qDebug() << "SendResponse Local8Bit: " << message;
        return true;
    }
}


void Rasllo::clientDisconected()
{
    layout->modalWindow->CloseWindow();
    layout->ShowMessage(ServerWasDisconnected);
}
void Rasllo::sendMessage(QString msg)
{
    SendMessageToServer("<RPO>" + msg + "</RPO>");
}
