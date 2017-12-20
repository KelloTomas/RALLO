#ifndef LAYOUT_H
#define LAYOUT_H

#include <QMap>
#include <QQueue>
#include <typeinfo>
#include <QUiLoader>
#include <QLabel>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QPushButton>
#include <QToolButton>
#include <QMainWindow>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QXmlStreamReader>

#include "errors.h"
#include "queueitem.h"
#include "beepsound.h"
#include "rfid_rc522.h"
#include "serialcardread.h"
#include "keyboardhandler.h"
#include "modalwindow.h"
#include "queueactions.h"
#include "ui_rasllo.h"

namespace Ui {
class Layout;
}

class Layout : public QMainWindow
{
    Q_OBJECT

public:
    Layout(bool isEmulator, int port, QWidget *parent = 0);
    ~Layout();
    static QString XMLVer() { return "1.0.0.0"; }
    ModalWindow *modalWindow;
    void Init(KeyboardHandler *keyboardCardRead, quint16 listenPort, QString progVersion);
    void Start();
    void ParseXmlData(QXmlStreamReader *xml);
    void StopTimers();
    QString GetResponse();

#ifdef QT_DEBUG
    const bool Debug = false;
    const bool DebugErrors = true;
    const bool DebugIOData = false;
    const bool DebugFunction = true;
    const bool DebugWarnings = true;
    const bool DebugFindButton = false;
    const bool DebugStoredData = false;
    const bool DebugFunctionAll = false;
    const bool DebugFunctionGetQueue = false;
#else
    // ForRaspberryPi & RELEASE
    const bool Debug = false;
    const bool DebugErrors = true;
    const bool DebugIOData = false;
    const bool DebugFunction = true;
    const bool DebugWarnings = true;
    const bool DebugFindButton = false;
    const bool DebugStoredData = false;
    const bool DebugFunctionAll = false;
    const bool DebugFunctionGetQueue = false;
#endif

signals:
    void MessageToSend(QString msg);

public slots:
    void UpdateTime();
    void UpdateIpAddressOnLayout();
    void ShowMessage(QString message);
    void AddEventToQueue(QueueItem item);
    void AddCardReadedEvent(QString CardNumber);

private:
    Ui::Layout *ui;
    SerialCardRead *serialCard;
    KeyboardHandler *KeyboardCardRead;
    QVBoxLayout *mainLayout;
    bool showPortNumber;

    quint16 ListenPort;
    BeepSound *beep;
    Errors errors;
    Errors warnings;
    QueueActions *Queue;
    QString CurrentLayout;
    QString ProgramVersion;
    QString messageToSend;
    QString initLayoutName = "InitLayout.ui";
    QString VerziaTitle = "Verzia: ";
    QString InitInProgress = "Prebehla inicializacia";
    QTimer *CurrTimeTimer;
    QTimer *IPUpdateTimer;
    QMap<QString, QString> Layouts;
    QMap<QString, QString> References;
    QString ProcessLayoutButtons(QString inputLayout);
    bool EditQObjectAtribute(QString id, QString attribute, QString newValue);
    void SetTextsOnLayout(QString message);
    QString ModifyAttributeFromXML(QXmlStreamReader *xml);
    void ProcessXmlBodyOfShowLayout(QXmlStreamReader *xml);
    void ShowInitLayout();
    void SetTimersForTime();
    void xmlInitFunction(QXmlStreamReader *xml);
    void xmlGetQueueFunction();
    void xmlMessageFunction(QXmlStreamReader *xml);
    void xmlDisplayFunction(QXmlStreamReader *xml);
    void xmlStoreRefFunction();
    void xmlStoreLayoutFunction(QXmlStreamReader *xml);
    void xmlStoreDataFunction(QXmlStreamReader *xml);
    void xmlShowFunction(QXmlStreamReader *xml);
    void xmlModalFunction(QXmlStreamReader *xml);
    void ShowLayout(QString layoutName);
    void xmlBeepFunction(QXmlStreamReader *xml);
};

#endif // LAYOUT_H
