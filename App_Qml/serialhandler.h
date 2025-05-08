#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QObject>
#include <QSerialPort>

class SerialHandler : public QObject
{
    Q_OBJECT
public:
    explicit SerialHandler(QObject *parent = nullptr);
    Q_INVOKABLE void readData();

signals:
    void updateData(int speed, int rpm, int odo, int trip, QString gear);

private:
    QSerialPort *serial;
};

#endif // SERIALHANDLER_H
