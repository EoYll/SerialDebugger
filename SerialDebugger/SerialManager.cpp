#include "SerialManager.h"
#include<qdebug.h>
#include<qvariant.h>
Q_DECLARE_METATYPE(QSerialPort::Parity);
SerialManager::SerialManager(QObject* parent)
{

	
}

QStringList SerialManager::availablePorts()
{
	QStringList portList;  
	const auto infos = QSerialPortInfo::availablePorts();
	for (const QSerialPortInfo& info : infos) {
		QString portDetails = QString("%1 (%2) - %3").arg(info.portName()).arg(info.description()).arg(info.manufacturer());

		portList.append(portDetails);
	}

	return portList;
}
bool SerialManager::connectSelectedPort(const QList<QVariant>& parameter,QString str)
{
	QSerialPort* &serialPort = (str == "Btn1" ? serialPort1 : serialPort2);
	
	QString portName;
	const auto infos = QSerialPortInfo::availablePorts();
	for (const QSerialPortInfo& info : infos) {
		if (QString("%1 (%2) - %3").arg(info.portName()).arg(info.description()).arg(info.manufacturer()) == parameter[0]) {
			portName = info.portName();
			break;
		}
	}
	if (portName.isEmpty()) {
		qWarning() << "未找到匹配的串口：" << parameter[0];
		return false;
	}
	if (serialPort && serialPort->isOpen()) {
		serialPort->close();
		delete serialPort;
		serialPort = nullptr;
	}
	if (!openPort(parameter,serialPort)) {
		return false;
	}

	return true;
}
bool SerialManager::openPort(const QList<QVariant>& parameter,QSerialPort*& serialPort)
{

	serialPort = new QSerialPort();
	QString portName = parameter[0].toString().split(" ", Qt::SkipEmptyParts)[0];
	serialPort->setPortName(portName);

	// 配置参数
	//serialPort->setBaudRate(QSerialPort::Baud9600);
	//serialPort->setDataBits(QSerialPort::Data8);
	//serialPort->setParity(QSerialPort::NoParity);
	//serialPort->setStopBits(QSerialPort::OneStop);


	serialPort->setBaudRate(parameter[1].value<QSerialPort::BaudRate>());
	serialPort->setDataBits(parameter[2].value<QSerialPort::DataBits>());
	serialPort->setParity(parameter[3].value<QSerialPort::Parity>());
	serialPort->setStopBits(parameter[4].value<QSerialPort::StopBits>());

	serialPort->setFlowControl(QSerialPort::NoFlowControl);
	if (!serialPort->open(QIODevice::ReadWrite)) {
		qCritical() << "无法打开串口:" << portName << " 错误：" << serialPort->errorString();
		delete serialPort;
		serialPort = nullptr;
		return false;
	}
	connect(serialPort, &QSerialPort::readyRead, this, &SerialManager::handleReadyRead);
	qDebug() << "打开了"<<serialPort1;
	return true;
}

bool SerialManager::closePort(QString str)
{
	QSerialPort* &port = (str == "Btn1" ? serialPort1 : serialPort2);
	qDebug() << port;
	if (port && port->isOpen()) {
		port->close();
		qInfo() << "已关闭连接";
		delete port;
		port = nullptr;
		return true;
	}
	
	return false;
}

void SerialManager::sendData(const QByteArray& data)
{
	if (!serialPort1 || !serialPort1->isOpen()) {
		qWarning() << "串口未打开，无法发送数据";
		return;
	}
	qint64 bytesWritten = serialPort1->write(data);
	if (bytesWritten == -1) {
		qWarning() << "发送失败："<<serialPort1->errorString();
		return;
	}
	if (!serialPort1->waitForBytesWritten(1000)) {
		qWarning() << "发送超时：" << serialPort1->errorString();
		return;
	}
	qDebug() << "成功发送" << bytesWritten << "字节数据";
	return;

}

bool SerialManager::isOpen() const
{
	return serialPort1->isOpen();
}


void SerialManager::handleReadyRead()
{
	QByteArray data = serialPort2->readAll();
	qDebug() << "原始数据：" << data;
	emit  dataReceived(data);

}

