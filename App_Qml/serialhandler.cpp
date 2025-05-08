#include "serialhandler.h"
#include <QDebug>

SerialHandler::SerialHandler(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    serial->setPortName("COM3"); // Cập nhật lại COM nếu khác
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->open(QIODevice::ReadOnly);
}

void SerialHandler::readData()
{
    while (serial->canReadLine()) {
        QByteArray line = serial->readLine().trimmed();
        QList<QByteArray> parts = line.split(',');

        if (parts.size() == 5) {
            int speed = parts[0].toInt();
            int rpm = parts[1].toInt();
            int odo = parts[2].toInt();
            int trip = parts[3].toInt();
            QString gear = QString(parts[4]);

            emit updateData(speed, rpm, odo, trip, gear);
        }
    }
}
