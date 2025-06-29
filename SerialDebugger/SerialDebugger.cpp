#include "SerialDebugger.h"

SerialDebugger::SerialDebugger(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.btnRefresh, &QPushButton::clicked, this, [this]() {
        refreshPorts("Btn1");
        
        });
    connect(ui.btnRefresh2, &QPushButton::clicked, this, [this]() {

        refreshPorts("Btn2");

        });
    connect(ui.btnConnect, &QPushButton::clicked, this, [this]() {
        toggleConnection("Btn1");
        });
    connect(ui.btnConnect2, &QPushButton::clicked, this, [this]() {
        toggleConnection("Btn2");
        });
    connect(ui.btnSend, &QPushButton::clicked, this, &SerialDebugger::sendData);
    connect(&serialManager, &SerialManager::dataReceived, this, &SerialDebugger::handleReceivedData);

    connect(ui.btnClearReceive, &QPushButton::clicked, this, &SerialDebugger::clearReceiveData);
    connect(ui.btnClearSend, &QPushButton::clicked, this, &SerialDebugger::clearSendData);
    initBaudRate();
    initDataBits();
    initParityBit();
    initStopBits();
    ui.cmbDataMode->addItems({ "ASCII","HEX","AutoSend" });
    isConnected = false;
    isConnected2 = false;
}

SerialDebugger::~SerialDebugger()
{}

void SerialDebugger::refreshPorts(QString str)
{
   QStringList portList = serialManager.availablePorts();
   QComboBox*& cmb = (str == "Btn1" ? ui.cmbPorts : ui.cmbPorts2);
   cmb->clear();
   if (portList.isEmpty()) {
       cmb->addItem("未检测到可用串口");
   }
   else {
       for (const QString& info : portList) {
           cmb->addItem(info);
       }
   }
  
}

void SerialDebugger::toggleConnection(QString str)
{   
    qDebug() << str;

    QPushButton*& btn = (str == "Btn1" ? ui.btnConnect : ui.btnConnect2);
    bool& isCon = (str == "Btn1" ? isConnected : isConnected2);
    QComboBox*& cmb = (str == "Btn1" ? ui.cmbPorts : ui.cmbPorts2);
    QLabel*& label = (str == "Btn1" ? ui.portStatus : ui.portStatus2);
    QList<QVariant>& para = (str == "Btn1" ? parameter : parameter2);

    if (isCon) {
        isCon = !serialManager.closePort(str);
        if (!isCon) {
            label->clear();
            btn->setText("连接");
        }
    }
    else {
        setSerialPortParameter(para, str);
        isCon = serialManager.connectSelectedPort(para,str);
        if (isCon) {
            label->setText(cmb->currentText());
            btn->setText("断开连接");
        }
        
       
    }
    
   
    
}

void SerialDebugger::sendData()
{
    QString text = ui.textSend->toPlainText();
    QByteArray byteText;
    if (ui.cmbDataMode->currentText() == "ASCII") {
       
         byteText = text.toUtf8();
       
    }
    else if (ui.cmbDataMode->currentText() == "HEX") {
      
         byteText = QByteArray::fromHex(text.replace(" ", "").toLatin1());
      
    }
    else if (ui.cmbDataMode->currentText() == "AutoSend") {
        
    
    }
    
    serialManager.sendData(byteText);
}

void SerialDebugger::handleReceivedData(QByteArray& data)
{
    QString text;
    
    if (ui.cmbDataMode->currentText() == "ASCII") {

        text = QString::fromUtf8(data);

    }
    else if (ui.cmbDataMode->currentText() == "HEX") {

        text = data.toHex();

    }
    else if (ui.cmbDataMode->currentText() == "AutoSend") {


    }
    ui.textReceive->append(text);
}

void SerialDebugger::saveReceivedData()
{

}

void SerialDebugger::loadSendHistory()
{

}

void SerialDebugger::clearReceiveData()
{
    ui.textReceive->clear();
   
}

void SerialDebugger::clearSendData()
{
    ui.textSend->clear();
}

void SerialDebugger::initBaudRate()
{
    for (int& i : baudRate) {
        ui.cmbBaudtate->addItem(QString::number(i));
        ui.cmbBaudtate2->addItem(QString::number(i));
       
    }
    ui.cmbBaudtate->setCurrentText(QString::number(9600));
    ui.cmbBaudtate2->setCurrentText(QString::number(9600));
}

void SerialDebugger::initDataBits()
{
    for (int& i : dataBits) {
        ui.cmbDataBits->addItem(QString::number(i));
        ui.cmbDataBits2->addItem(QString::number(i));
    }
    ui.cmbDataBits->setCurrentText(QString::number(8));
    ui.cmbDataBits2->setCurrentText(QString::number(8));
}

void SerialDebugger::initStopBits()
{
    for (const auto& [text, stopBits] : stopBits.asKeyValueRange()) {
        ui.cmbStopBits->addItem(text);
        ui.cmbStopBits2->addItem(text);
    }
    ui.cmbStopBits->setCurrentText("OneStop");
    ui.cmbStopBits2->setCurrentText("OneStop");
}

void SerialDebugger::initParityBit()
{
    for (const auto& [text, stopBits] : parityBit.asKeyValueRange()) {
        ui.cmbParity->addItem(text);
        ui.cmbParity2->addItem(text);
    }
    ui.cmbParity->setCurrentText("无校验");
    ui.cmbParity2->setCurrentText("无校验");

}

void SerialDebugger::updateUiState(bool connected)
{

}

void SerialDebugger::setupParityOptions()
{
}

void SerialDebugger::setupStopBits()
{
}

void SerialDebugger::setupFlowControl()
{
}

void SerialDebugger::processLine(const QByteArray& line)
{
    //// 示例1：解析CSV数据 (e.g., "23.5,60.2,1024")
    //QList<QByteArray> values = line.split(',');
    //if (values.size() == 3) {
    //    float temp = values[0].toFloat();
    //    float hum = values[1].toFloat();
    //    int pressure = values[2].toInt();
    //   // emit newSensorData(temp, hum, pressure); // 发送信号更新UI
    //}

    //// 示例2：十六进制转十进制（二进制协议）
    //// 假设收到4字节大端序数据: 0x00 0xFF 0x13 0x88
    //if (line.size() >= 4) {
    //    uint32_t value = (line[0] << 24) | (line[1] << 16) | (line[2] << 8) | line[3];
    //    qDebug() << "解码值:" << value;
    //}
   
   
}

void SerialDebugger::setSerialPortParameter(QList<QVariant>&parameter,QString str)
{
    QComboBox*& cmbPort = (str == "Btn1" ? ui.cmbPorts: ui.cmbPorts2);
    QComboBox*& cmbBaudtate = (str == "Btn1" ? ui.cmbBaudtate: ui.cmbBaudtate2);
    QComboBox*& cmbDataBits = (str == "Btn1" ? ui.cmbDataBits: ui.cmbDataBits2);
    QComboBox*& cmbParity = (str == "Btn1" ? ui.cmbParity: ui.cmbParity2);
    QComboBox*& cmbStopBits = (str == "Btn1" ? ui.cmbStopBits: ui.cmbStopBits2);
    parameter.clear();
    parameter.append(cmbPort->currentText());
    parameter.append(cmbBaudtate->currentText());
    parameter.append(cmbDataBits->currentText());
    parameter.append(parityBit[cmbParity->currentText()]);
    parameter.append(stopBits[ cmbStopBits->currentText()]);
    

}

