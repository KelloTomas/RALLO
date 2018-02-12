#include "serialporthandler.h"

#include <QCoreApplication>

QT_USE_NAMESPACE

SerialPortHandler::SerialPortHandler(QObject *parent)
    : QObject(parent)
{
    isReading = false;

    serialPort.setPortName("/dev/ttyAMA0");
    serialPort.setBaudRate(QSerialPort::Baud9600);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    if (!serialPort.open(QIODevice::ReadOnly)) {
            qDebug() << QObject::tr("Failed to open port /dev/ttyAMA0, error: %2").arg(serialPort.errorString()) << endl;
    }
    else
    {
        qDebug() << "zacalo odposluchat na serial porte ttyAMA0";
    }
    connect(&serialPort, &QSerialPort::readyRead, this, &SerialPortHandler::handleReadyRead);
    connect(&serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialPortHandler::handleError);

}

SerialPortHandler::~SerialPortHandler()
{
    serialPort.close();
}

void SerialPortHandler::handleReadyRead()
{
    char *data = new char[2];
    while (serialPort.read(data, 1) != 0)
    {
        if (data[0] == 2)
        {
            if (isReading)
                qDebug() << "Card is reading and new START symbol detected!!";
            qDebug() << "Card detected";
            cardNumber.clear();
            isReading = true;
        }
        else if (data[0] == 3)
        {
            qDebug() << "Card readed event: " << cardNumber;
            isReading = false;
            emit CardReaded(cardNumber);
            cardNumber.clear();
        }
        else if (isReading)
        {
            cardNumber.append(data[0]);
            qDebug() << "New card symbol: " << data[0];
        }
        else
        {
            qDebug() << "Notreading symbol: " << data[0];
        }
    }
}


void SerialPortHandler::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        qDebug() << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(serialPort.portName()).arg(serialPort.errorString()) << endl;
    }
}

