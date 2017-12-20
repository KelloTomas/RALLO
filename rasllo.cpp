#include "rasllo.h"

Rasllo::Rasllo(QWidget *) : networkSession()
{
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

void Rasllo::Init(KeyboardHandler *keyboardCardRead, bool isEmulator, uint ListenPort, QString progVer)
{
    listenPort = ListenPort;
    layout = new Layout(isEmulator, listenPort);
    tcpServer = new QTcpServer();
    layout->Init(keyboardCardRead, listenPort, progVer);
}

void Rasllo::Start()
{
    layout->Start();
    qDebug() << "Rasllo sa spustil, verzia XML: " << layout->XMLVer();

    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("ISO-8859-15"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));


#ifdef Gestures // pokus o spustenie gest
    ui->centralWidget->grabGesture(Qt::SwipeGesture);
    /*
    TapGesture
    TapAndHoldGesture
    PanGesture
    PinchGesture
    SwipeGesture
    */
#endif

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

    layout->AddEventToQueue(QueueItem(QueueItemEnum::JustStarted, nullptr));
}

void Rasllo::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    if (!tcpServer->listen(QHostAddress::Any, listenPort))
    {
        layout->ShowMessage(UnableToStartServer + tcpServer->errorString());
        listenPort = 0;
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

        SendResponse(layout->GetResponse());

        if (inputData.isEmpty())
            break;
    }
}

bool Rasllo::SendResponse(QString message)
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
