#pragma once
#include "qobject.h"
#include<qserialport.h>
#include<qserialportinfo.h>
#include <SerialRingBuffer.h>
#include <qtimer.h>
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
 
public slots:
    void dataReceivedToUi();
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

    SerialRingBuffer* buffer;
    //Qtimer timer;
};

