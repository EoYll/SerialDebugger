#pragma once
#include "E:\Qt\Qt6.8.0\6.8.0\msvc2022_64\include\QtCore\qobject.h"
#include<qserialport.h>
#include<qserialportinfo.h>
class SerialManager :
    public QObject
{
    Q_OBJECT
public:
    explicit SerialManager(QObject* parent = nullptr);

    QStringList availablePorts();
    bool connectSelectedPort(const QList<QVariant>& parameter, QString str);
   
   
    bool openPort(const QList<QVariant>& parameter, QSerialPort*& serialPort);
    bool closePort(QString);
    void sendData(const QByteArray& data);
    bool isOpen() const;
 
signals:
    void dataReceived(QByteArray& data);
    void errorOccurred(QString error);

private slots:
    void handleReadyRead();

private:
    //发送方
    QSerialPort* serialPort1 = nullptr;
    //接收方
    QSerialPort* serialPort2 = nullptr;
};

