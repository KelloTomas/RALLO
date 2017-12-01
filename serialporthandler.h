#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QtSerialPort/QSerialPort>

#include <QTextStream>
#include <QTimer>
#include <QByteArray>
#include <QObject>
#include <QDebug>

QT_USE_NAMESPACE


class SerialPortHandler : public QObject
{
    Q_OBJECT

public:
    SerialPortHandler(QObject *parent = 0);
    ~SerialPortHandler();

signals:
    void CardReaded(QString cardNumber);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort serialPort;
    QString cardNumber;
    bool isReading;
};

#endif
