#include "uart.h"
#include "lib.h"
#include <QApplication>
#include <QDebug>


uart::uart(QObject *parent) : QObject(parent)
{
	serialUart = new QSerialPort;
	connect(serialUart, &QSerialPort::readyRead, this, &uart::readSerial);
}

uart::~uart()
{
	delete serialUart;
}

void uart::readSerial()
{
	func.delay(100);
	serialData=serialUart->readAll();
	QApplication::processEvents();
	//if (serialData.toDouble()!=0)
	emit readyRequest();
}

void uart::getData(QByteArray &Data)
{
	Data=serialData;
}

bool uart::getStatus()
{
	return statusCon;
}

void uart::setName(QString name)
{
	portName=name;
}

void uart::sendCom(const QByteArray command)
{
	serialUart->write(command);
}

void uart::sendComForm(const QByteArray command)
{
	serialUart->write('*'+command+'$');
}

QString uart::conUART()
{
	//open
	//select name for our serial port from combobox
	if (serialUart->portName() != portName)
	{
		serialUart->close();
		serialUart->setPortName(portName);
	}

	//setup COM port
	serialUart->setBaudRate(QSerialPort::Baud9600);
	serialUart->setDataBits(QSerialPort::Data8);
	serialUart->setParity(QSerialPort::NoParity);
	serialUart->setStopBits(QSerialPort::OneStop);
	serialUart->setFlowControl(QSerialPort::NoFlowControl);
	serialUart->setReadBufferSize(64);
	if (!serialUart->open(QSerialPort::ReadWrite))
	{
		qDebug()<<serialUart->errorString();
		return("Порт не удалось открыть\n");
	}
	else
	{
		statusCon=true;
		return("Порт открыт\n");
	}
}

QString uart::disconUART()
{
	//close
	serialUart->close();
	statusCon=false;
	return("Порт закрыт\n");
}
