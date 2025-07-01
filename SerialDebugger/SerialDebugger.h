#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SerialDebugger.h"
#include<qserialport.h>
#include"SerialManager.h"
#include<qmap.h>
#include<qstring.h>
#include<qfile.h>
class SerialDebugger : public QMainWindow
{
    Q_OBJECT

public:
   
    SerialDebugger(QWidget *parent = nullptr);
    ~SerialDebugger();

    


private slots:
    void refreshPorts(QString str);
    void toggleConnection(QString );
    void sendData();
    void handleReceivedData(QByteArray& data);
    void saveReceivedData(QString&text);
    void loadSendHistory();

    void clearReceiveData();
    void clearSendData();
private:
    Ui::SerialDebuggerClass ui;
    SerialManager serialManager;
    QString receivedDataBuffer;
    //QThread* workerThread;

    QVector<int>baudRate{ 300,1200,2400,4800,9600,19200,38400,57600,115200,239400,460800,921600 };
    QVector<int>dataBits{ 5,6,7,8 };
    QMap<QString, QSerialPort::StopBits> stopBits = {
     {"OneStop", QSerialPort::OneStop},
     {"OneAndHalfStop", QSerialPort::OneAndHalfStop},
     {"TwoStop", QSerialPort::TwoStop}
    };
    QMap<QString, QSerialPort::Parity> parityBit = {
        {"无校验",QSerialPort::NoParity},
        {"偶校验",QSerialPort::EvenParity},
        {"奇校验",QSerialPort::OddParity},
        {"空位校验",QSerialPort::SpaceParity},
        {"标志校验",QSerialPort::MarkParity}

    };
   
    QList<QVariant>parameter;
    QList<QVariant>parameter2;

    void initBaudRate();
    void initDataBits();
    void initStopBits();
    void initParityBit();

    void updateUiState(bool connected);
    void setupParityOptions();
    void setupStopBits();
    void setupFlowControl();
    void processLine(const QByteArray& line);
    void setSerialPortParameter(QList<QVariant>&,QString str);
    bool isConnected;
    bool isConnected2;
    
    QTimer readDataTimer;
    
};

