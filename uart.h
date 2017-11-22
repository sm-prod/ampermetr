#ifndef UART_H
#define UART_H

#include "lib.h"

#include <QObject>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QByteArray>


class uart : public QObject
{
	Q_OBJECT
public:
	explicit uart(QObject *parent = nullptr);
	~uart();
	void getData(QByteArray &Data);
	void setName(QString name);
	bool getStatus();


signals:
	void readyRequest();

public slots:
	void sendComForm(const QByteArray command);

	void sendCom(const QByteArray command);

	QString conUART();

	QString disconUART();

private slots:
	void readSerial();

private:
	QByteArray serialData;
	QSerialPort *serialUart;
	QString portName;
	bool statusCon=false;
	Lib func;

};

#endif // UART_H
