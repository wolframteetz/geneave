#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QtSerialPort/QSerialPort>

#include <QTextStream>
#include <QTimer>
#include <QByteArray>
#include <QObject>

QT_USE_NAMESPACE


class SerialPortReader : public QObject
{
    Q_OBJECT

public:
    SerialPortReader(QSerialPort *serialPort, QObject *parent = 0);
    ~SerialPortReader();

private slots:
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *m_serialPort;
    QByteArray  m_readData;
    QTextStream m_standardOutput;
    QTimer      m_timer;
};

#endif
