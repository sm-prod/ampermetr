#include "keithley6487.h"
#include "lib.h"
#include "qcustomplot.h"
#include "uart.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QEventLoop>
#include <QLabel>
#include <QTextCursor>
#include <QDebug>


//	Добавить перегруженный конструктор
//	и функции для передачи указателей
//

Keithley6487::Keithley6487(QTextBrowser *textBrowser, QComboBox *portKeithBox, QCustomPlot *graph, QGridLayout *gridLayotMod,QObject *parent) : QObject(parent)
{
	uiGridLayoutMod=gridLayotMod;
	uiGraphic=graph;
	uiPortKeithBox=portKeithBox;
	uiTextBrowser=textBrowser;
	stepVolt=1;//Шаг по оси X             Перенести
	startLine=0;//Начало отсчёта Оx          в файл
	endLine=10;//Конец Оx                  конфигурации
}

void Keithley6487::connectAmp()
{
	serialKeithley->setName(uiPortKeithBox->currentText());
	writeData(serialKeithley->conUART().toUtf8());
	setAmp();
	writeData("Готово к использованию\n");
}

void Keithley6487::connectTemp()
{
	serialTemp->setName(uiPortTempBox->currentText());
	writeData(serialTemp->conUART().toUtf8());
	startButton->setEnabled(false);
}

void Keithley6487::disconnectAmp()
{
	writeData(serialKeithley->disconUART().toUtf8());
	startButton->setEnabled(false);
}

void Keithley6487::disconnectTemp()
{
	writeData(serialTemp->disconUART().toUtf8());
}

void Keithley6487::mesurT(double volt)
{
	serialKeithley->sendCom("SOUR:VOLT:RANG 50\r\n");
	func.delay(100);
	serialKeithley->sendCom("SOUR:VOLT ");
	func.delay(100);
	serialKeithley->sendCom(QByteArray::number(volt));
	serialKeithley->sendCom("\r\n");
	func.delay(100);
	serialKeithley->sendCom("SOUR:VOLT:ILIM 2.5e-3\r\n");
	func.delay(100);
	serialKeithley->sendCom("SOUR:VOLT:STAT ON\r\n");
	func.delay(100);
	serialKeithley->sendCom("SYST:ZCH OFF\r\n");
	func.delay(100);
	serialTemp->sendCom("MESUR");
	serialKeithley->sendCom("READ?\r\n");
}

void Keithley6487::mesurI(double volt)
{
	serialKeithley->sendCom("SOUR:VOLT:RANG 50\r\n");
	func.delay(100);
	serialKeithley->sendCom("SOUR:VOLT ");
	func.delay(100);
	serialKeithley->sendCom(QByteArray::number(volt));
	serialKeithley->sendCom("\r\n");
	func.delay(100);
	serialKeithley->sendCom("SOUR:VOLT:ILIM 2.5e-3\r\n");
	func.delay(100);
	serialKeithley->sendCom("SOUR:VOLT:STAT ON\r\n");
	func.delay(100);
	serialKeithley->sendCom("SYST:ZCH OFF\r\n");
	func.delay(100);
	//serial->clear(QSerialPort::Input);
	serialKeithley->sendCom("READ?\r\n");
}

void Keithley6487::setAmp()
{
	serialKeithley->sendCom("CAL:UNPR:VOFF\r\n");
	func.delay(5000);
	serialKeithley->sendCom("SYST:ZCH ON\r\n");
	func.delay(100);
	serialKeithley->sendCom("RANG 2e-9\r\n");
	func.delay(100);
	serialKeithley->sendCom("INIT\r\n");
	func.delay(100);
	serialKeithley->sendCom("SYST:ZCOR:STAT OFF\r\n");
	func.delay(100);
	serialKeithley->sendCom("SYST:ZCOR:ACQ\r\n");
	func.delay(100);
	serialKeithley->sendCom("SYST:ZCOR ON\r\n");
	func.delay(100);
	serialKeithley->sendCom("RANG:AUTO ON\r\n");
	func.delay(100);
	serialKeithley->sendCom("FORM:ELEM READ\r\n");
	startButton->setEnabled(true);
}

void Keithley6487::on_voltValue_SpinBox_valueChanged(double arg1)
{
	voltValue=arg1;
}

void Keithley6487::on_StepFirst_SpinBox_valueChanged(double arg1)
{
	startLine=arg1;
	int Step=(endLine-startLine)/stepVolt;
	label_NumbStep->setText(QString::number(Step));
	label_MesurTime->setText(QString::number(Step*1.8)+"с");
}

void Keithley6487::on_StepLast_SpinBox_valueChanged(double arg1)
{
	endLine=arg1;
	int Step=(endLine-startLine)/stepVolt;
	label_NumbStep->setText(QString::number(Step));
	label_MesurTime->setText(QString::number(Step*1.8)+"с");
}

void Keithley6487::on_Step_SpinBox_valueChanged(double arg1)
{
	stepVolt=arg1;
	int Step=(endLine-startLine)/stepVolt;
	label_NumbStep->setText(QString::number(Step));
	label_MesurTime->setText(QString::number(Step*1.8)+"с");
}

void Keithley6487::on_TimeMesur_SpinBox_valueChanged(int arg1)
{
	timeMesur=arg1;
}

void Keithley6487::readSerialAmp()
{
	QByteArray Data;
	func.delay(100);
	serialKeithley->getData(Data);
	Data.remove(13,1);
	//writeData(Data);
	if (Data.toDouble()!=0)
		writeValueAmp(Data);
}

void Keithley6487::readSerialTemp()
{
	QEventLoop eventLoop;
	connect(serialKeithley, &uart::readyRequest, &eventLoop, QEventLoop::quit);
	eventLoop.exec();
	QByteArray DataA;
	func.delay(100);
	serialKeithley->getData(DataA);
	DataA.remove(13,1);
	QByteArray DataT;
	serialTemp->getData(DataT);
	writeValueTemp(DataA, DataT);
}

void Keithley6487::readSerialAmpTime()
{
	QByteArray Data;
	func.delay(100);
	serialKeithley->getData(Data);
	Data.remove(13,1);
	if (Data.toDouble()!=0)
		writeValueAmpTime(Data);
}

void Keithley6487::writeValueAmp(QByteArray Data)
{
	QTextCursor cur = uiTextBrowser->textCursor();
	cur.movePosition(QTextCursor::End);
	uiTextBrowser->setTextCursor(cur);
	currentValues.append(Data.toDouble());
	uiTextBrowser->insertPlainText(QByteArray::number(voltValue,'g',4)+"\t"+Data+"\n");


	//Установим область, которая будет показываться на графике
	uiGraphic->xAxis->setRange(voltValues.first(), voltValues.last());//Для оси Ox
	uiGraphic->yAxis->setRange(currentValues.first(), currentValues.last());//Для оси Oy

	uiGraphic->graph(0)->setData(voltValues, currentValues);
	uiGraphic->replot();
}

void Keithley6487::writeValueAmpTime(QByteArray Data)
{
	QTextCursor cur = uiTextBrowser->textCursor();
	cur.movePosition(QTextCursor::End);
	uiTextBrowser->setTextCursor(cur);
	//QDateTime now = QDate
	double timeElapsed = timeStartMes.secsTo(timeEndMes)-QDateTime::currentDateTime().secsTo(timeEndMes);//time->elapsed()/1000.0;
	tempValues.append(timeElapsed);
	currentValues.append(Data.toDouble());
	uiTextBrowser->insertPlainText(QByteArray::number(timeElapsed)+"\t"+Data+"\n");

	//Установим область, которая будет показываться на графике
	if(!holdXAxis)
		uiGraphic->xAxis->setRange(0, timeElapsed);//Для оси Ox
	if(!holdYAxis)
		uiGraphic->yAxis->setRange(currentValues.first(), currentValues.last());//Для оси Oy

	uiGraphic->graph(0)->setData(tempValues, currentValues);
	uiGraphic->replot();
//	qDebug()<<tempValues;
}

void Keithley6487::writeValueTemp(QByteArray DataA, QByteArray DataT)
{
	QTextCursor cur = uiTextBrowser->textCursor();
	cur.movePosition(QTextCursor::End);
	uiTextBrowser->setTextCursor(cur);
	tempValues.append(DataT.toDouble());
	currentValues.append(DataA.toDouble());
	uiTextBrowser->insertPlainText(DataT+"\t"+DataA+"\n");

	//Установим область, которая будет показываться на графике
	uiGraphic->xAxis->setRange(tempValues.first(), tempValues.last());//Для оси Ox
	uiGraphic->yAxis->setRange(currentValues.first(), currentValues.last());//Для оси Oy

	uiGraphic->graph(0)->setData(tempValues, currentValues);
	uiGraphic->replot();
}

void Keithley6487::writeData(QByteArray Data)
{
	QTextCursor cur = uiTextBrowser->textCursor();
	cur.movePosition(QTextCursor::End);
	uiTextBrowser->setTextCursor(cur);
	uiTextBrowser->insertPlainText(Data);
}

bool Keithley6487::changeMod0()
{
	cleanMod();

	serialKeithley = new uart();
	connect(serialKeithley, &uart::readyRequest, this, &Keithley6487::readSerialAmp);

	label_FirstStep = new QLabel();
	label_FirstStep->setText("От");
	label_LastStep = new QLabel();
	label_LastStep->setText("До");
	label_ChStep = new QLabel();
	label_ChStep->setText("С шагом");
	labelStep = new QLabel();
	labelStep->setText("Количество шагов:");
	label_NumbStep = new QLabel();
	labelTime = new QLabel();
	labelTime->setText("Снятие займёт:");
	label_MesurTime = new QLabel();
	StepFirst_SpinBox = new QDoubleSpinBox();
	connect(StepFirst_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_StepFirst_SpinBox_valueChanged(double)));
	StepFirst_SpinBox->setValue(startLine);//Перенести в сет
	StepFirst_SpinBox->setSuffix("В");
	StepFirst_SpinBox->setDecimals(3);
	StepFirst_SpinBox->setMaximum(200);
	StepFirst_SpinBox->setSingleStep(0.05);
	StepLast_SpinBox = new QDoubleSpinBox();
	connect(StepLast_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_StepLast_SpinBox_valueChanged(double)));
	StepLast_SpinBox->setValue(endLine);//Перенести в сет
	StepLast_SpinBox->setSuffix("В");
	StepLast_SpinBox->setDecimals(3);
	StepLast_SpinBox->setMaximum(200);
	StepLast_SpinBox->setSingleStep(0.05);
	Step_SpinBox = new QDoubleSpinBox();
	connect(Step_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_Step_SpinBox_valueChanged(double)));
	Step_SpinBox->setValue(stepVolt);//Перенести в сет
	Step_SpinBox->setSuffix("В");
	Step_SpinBox->setDecimals(3);
	Step_SpinBox->setMaximum(200);
	Step_SpinBox->setSingleStep(0.05);
	startButton = new QPushButton();
	if(!serialKeithley->getStatus())
		startButton->setEnabled(false);
	connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startMod0()));
	startButton->setText("Запуск");

	//Подписываем оси Ox и Oy
	uiGraphic ->xAxis->setLabel("U, В");
	uiGraphic->yAxis->setLabel("I, А");

	uiGridLayoutMod->addWidget(label_FirstStep,0,0);
	uiGridLayoutMod->addWidget(StepFirst_SpinBox,0,1,1,2);
	uiGridLayoutMod->addWidget(label_LastStep,1,0);
	uiGridLayoutMod->addWidget(StepLast_SpinBox,1,1,1,2);
	uiGridLayoutMod->addWidget(label_ChStep,2,0);
	uiGridLayoutMod->addWidget(Step_SpinBox,2,1,1,2);
	uiGridLayoutMod->addWidget(labelStep,3,0,1,2);
	uiGridLayoutMod->addWidget(label_NumbStep,3,2);
	uiGridLayoutMod->addWidget(labelTime,4,0,1,2);
	uiGridLayoutMod->addWidget(label_MesurTime,4,2);
	uiGridLayoutMod->addWidget(startButton,5,0,1,3);

	selectedMod=0;
	return true;
}

bool Keithley6487::changeMod1()
{
	cleanMod();

	serialKeithley = new uart();
	serialTemp = new uart();
	connect(serialTemp, &uart::readyRequest, this, &Keithley6487::readSerialTemp);

	label_FirstStep = new QLabel();
	label_FirstStep->setText("Напряжение");
	voltValue_SpinBox = new QDoubleSpinBox();
	voltValue_SpinBox->setSuffix("В");
	voltValue_SpinBox->setDecimals(3);
	voltValue_SpinBox->setMaximum(200);
	voltValue_SpinBox->setSingleStep(0.5);
	connect(voltValue_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_voltValue_SpinBox_valueChanged(double)));
	startButton = new QPushButton();
	if(!serialKeithley->getStatus() && serialTemp->getStatus())
		startButton->setEnabled(false);
	startButton->setText("Запуск");
	connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startMod1()));
	connectTempButton = new QPushButton();
	connectTempButton->setText("Подключиться");
	connect(connectTempButton, SIGNAL(clicked(bool)), this, SLOT(connectTemp()));
	disconnectTempButton = new QPushButton();
	disconnectTempButton->setText("Отключиться");
	connect(disconnectTempButton, SIGNAL(clicked(bool)), this, SLOT(disconnectTemp()));
	searchTempButton = new QPushButton();
	searchTempButton->setText("Поиск");
	connect(searchTempButton, SIGNAL(clicked(bool)), this, SLOT(searchTempButton_clicked()));
	portTempBox = new QComboBox();
	foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
		portTempBox->addItem(info.portName());
	//portTempBox->setCurrentIndex(portTempBox->findText(settings->value("PORT1",portTempBox->currentText()).toString()));

	//Подписываем оси Ox и Oy
	uiGraphic->xAxis->setLabel("T,°C");
	uiGraphic->yAxis->setLabel("I, А");

	uiGridLayoutMod->addWidget(label_FirstStep,0,0);
	uiGridLayoutMod->addWidget(voltValue_SpinBox,0,1);
	uiGridLayoutMod->addWidget(portTempBox,1,0);
	uiGridLayoutMod->addWidget(connectTempButton,1,1);
	uiGridLayoutMod->addWidget(searchTempButton,2,0);
	uiGridLayoutMod->addWidget(disconnectTempButton,2,1);
	uiGridLayoutMod->addWidget(startButton,3,0,1,2);

	selectedMod=1;
	return true;
}

bool Keithley6487::changeMod2()
{
	cleanMod();

	serialKeithley = new uart;
	connect(serialKeithley, &uart::readyRequest, this, &Keithley6487::readSerialAmpTime);

	label_FirstStep = new QLabel();
	label_FirstStep->setText("Напряжение");
	labelTime = new QLabel();
	labelTime->setText("Время");
	voltValue_SpinBox = new QDoubleSpinBox();
	connect(voltValue_SpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_voltValue_SpinBox_valueChanged(double)));
	voltValue_SpinBox->setSuffix("В");
	voltValue_SpinBox->setDecimals(3);
	voltValue_SpinBox->setMaximum(200);
	voltValue_SpinBox->setMinimum(-200);
	voltValue_SpinBox->setSingleStep(0.05);
	TimeMesur_SpinBox = new QSpinBox();
	connect(TimeMesur_SpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_TimeMesur_SpinBox_valueChanged(int)));
	TimeMesur_SpinBox->setSuffix("c");
	TimeMesur_SpinBox->setMaximum(300000);
	TimeMesur_SpinBox->setSingleStep(1);
	startButton = new QPushButton();
	if(!serialKeithley->getStatus())
		startButton->setEnabled(false);
	connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startMod2()));
	startButton->setText("Запуск");

	//Подписываем оси Ox и Oy
	uiGraphic->xAxis->setLabel("T, c");
	uiGraphic->yAxis->setLabel("I, А");

	uiGridLayoutMod->addWidget(label_FirstStep,0,0);
	uiGridLayoutMod->addWidget(voltValue_SpinBox,0,1,1,2);
	uiGridLayoutMod->addWidget(labelTime,1,0);
	uiGridLayoutMod->addWidget(TimeMesur_SpinBox,1,1,1,2);
	uiGridLayoutMod->addWidget(startButton,2,0,1,3);

	selectedMod=2;
	return true;
}

void Keithley6487::startMod0()
{
	if(serialKeithley->getStatus())
	{
		//
		//Достаю значения из setting'ов
		//
		//Добавляем один график в widget
		uiGraphic->addGraph();
		delete startButton;
		stopButton = new QPushButton();
		stopButton->setText("Стоп");
		stopButton->setCheckable(true);
		uiGridLayoutMod->addWidget(stopButton,5,0,1,3);
		for(voltValue=startLine;voltValue<=endLine;voltValue+=stepVolt)
		{
			if (!stopButton->isChecked())
			{
				voltValues.append(voltValue);
				mesurI(voltValue);
				//readSerial();
				func.delay(1000);
				QApplication::processEvents();
			}
			else
				break;
		}
		delete stopButton;
		QApplication::beep();
		emit processEnd();
		startButton = new QPushButton();
		startButton->setText("Запуск");
		connect(startButton, SIGNAL(clicked()), this, SLOT(startMod0()));
		uiGridLayoutMod->addWidget(startButton,5,0,1,3);
		serialKeithley->sendCom("SOUR:VOLT:STAT OFF\r\n");
	}
	else
	{
		writeData("Порт не открыт\n");
	}
}

void Keithley6487::startMod1()
{
	if (serialTemp->getStatus() && serialKeithley->getStatus())
	{
		//
		//Достаю значения из setting'ов
		//
		//Добавляем один график в widget
		uiGraphic->addGraph();
		delete startButton;
		stopButton = new QPushButton();
		stopButton->setText("Стоп");
		stopButton->setCheckable(true);
		uiGridLayoutMod->addWidget(stopButton,3,0,1,3);
		while (!stopButton->isChecked())
		{
			mesurT(voltValue);
			QApplication::processEvents();
		}
		delete stopButton;
		QApplication::beep();
		emit processEnd();
		startButton = new QPushButton();
		startButton->setText("Запуск");
		connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startMod1()));
		uiGridLayoutMod->addWidget(startButton,3,0,1,3);
		serialKeithley->sendCom("SOUR:VOLT:STAT OFF\r\n");
	}
	else
	{
		writeData("Порт не открыт\n");
	}
}

void Keithley6487::startMod2()
{
	if (serialKeithley->getStatus())
	{
		//
		//Достаю значения из setting'ов
		//
		//Добавляем один график в widget
		uiGraphic->addGraph();
		//time = new QTime();
		delete startButton;
		stopButton = new QPushButton();
		stopButton->setText("Стоп");
		stopButton->setCheckable(true);
		uiGridLayoutMod->addWidget(stopButton,2,0,1,3);
		//time->start();
//		while(time->elapsed()<timeMesur*1000 && !stopButton->isChecked())
//		{
//			mesurI(voltValue);
//			QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
//		}
		int timeMesurMonth=timeMesur/60/60/24/QDate::currentDate().daysInMonth();
		int timeMesurDay=(timeMesur/60/60/24)-(timeMesurMonth*QDate::currentDate().daysInMonth());
		int timeMesurHour=(timeMesur/60/60)-(timeMesurDay*24)-(timeMesurMonth*QDate::currentDate().daysInMonth()*24);
		int timeMesurMinute=(timeMesur/60)-(timeMesurHour*60)-(timeMesurDay*1440)-(timeMesurMonth*QDate::currentDate().daysInMonth()*24*60);
		int timeMesurSec=timeMesur-(timeMesurMinute*60)-(timeMesurHour*3600)-(timeMesurDay*86400)-(timeMesurMonth*QDate::currentDate().daysInMonth()*24*60*60);
		if ((QTime::currentTime().second()+timeMesurSec)>59)
		{
			timeMesurSec-=60;
			timeMesurMinute++;
		}
		if ((QTime::currentTime().minute()+timeMesurMinute)>59)
		{
			timeMesurMinute-=60;
			timeMesurHour++;
		}
		if ((QTime::currentTime().hour()+timeMesurHour)>23)
		{
			timeMesurHour-=24;
			timeMesurDay++;
		}
		if (timeMesurDay!=0 && (QDate::currentDate().day()+timeMesurDay)>(timeMesurMonth*QDate::currentDate().daysInMonth()))
		{
			timeMesurDay-=timeMesurMonth*QDate::currentDate().daysInMonth();
			timeMesurMonth++;
		}
		timeStartMes = QDateTime::currentDateTime();
		timeEndMes = QDateTime(QDate(timeStartMes.date().year(), timeStartMes.date().month()+timeMesurMonth, timeStartMes.date().day()+timeMesurDay),
							   QTime(timeStartMes.time().hour()+timeMesurHour, timeStartMes.time().minute()+timeMesurMinute,
									 timeStartMes.time().second()+timeMesurSec));

		while(QDateTime::currentDateTime().secsTo(timeEndMes)!=0 && !stopButton->isChecked())
		{
			mesurI(voltValue);
			QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
		}

		func.delay(1000);
		QApplication::beep();
		emit processEnd();
		delete stopButton;
		startButton = new QPushButton();
		startButton->setText("Запуск");
		connect(startButton, SIGNAL(clicked(bool)), this, SLOT(startMod2()));
		serialKeithley->sendCom("SOUR:VOLT:STAT OFF\r\n");
		uiGridLayoutMod->addWidget(startButton,2,0,1,3);
		//delete time;
	}
	else
	{
		writeData("Порт не открыт\n");
	}
}

void Keithley6487::sendAmpCom(QString input)
{
	if (serialKeithley->getStatus())
	{
		serialKeithley->sendCom(input.toUtf8());
		serialKeithley->sendCom("\r\n");
	}
	else
		writeData("Порт не открыт\n");
}

bool Keithley6487::getSerialStatus(int dev)
{
	if(dev==0)
		return serialKeithley->getStatus();
	else
		return serialTemp->getStatus();
}

void Keithley6487::searchTempButton_clicked()
{
	portTempBox->clear();
	foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
		portTempBox->addItem(info.portName());
}

void Keithley6487::clearVect(char unit)
{
	switch (unit)
	{
	case 'I':
		currentValues.clear();
		break;
	case 'U':
		voltValues.clear();
		break;
	case 'T':
		tempValues.clear();
		break;
	default:
		currentValues.clear();
		voltValues.clear();
		tempValues.clear();
	}

}

void Keithley6487::cleanMod()
{
	if (serialKeithley)
	{
		delete serialKeithley;
		serialKeithley=0;
	}
	if (serialTemp)
	{
		delete serialTemp;
		serialTemp=0;
	}
}

void Keithley6487::getAxisRange(const char axis, double *axisRange)
{
	if(axis=='X')
	{
		switch(selectedMod)
		{
		case 0:
		{
			axisRange[0]=voltValues.first();
			axisRange[1]=voltValues.last();
			break;
		}
		case 1:
		{
			axisRange[0]=tempValues.first();
			axisRange[1]=tempValues.last();
			break;
		}
		case 2:
		{
			axisRange[0]=tempValues.first();
			axisRange[1]=tempValues.last();
			break;
		}
		}
	}
	else if(axis=='Y')
	{
		axisRange[0]=currentValues.first();
		axisRange[1]=currentValues.last();
	}
}

void Keithley6487::holdAxisRange(char axis)
{
	if(axis=='X')
		holdXAxis=true;
	if(axis=='Y')
		holdYAxis=true;
	if(axis=='A')
	{
		holdXAxis=true;
		holdYAxis=true;
	}
}
void Keithley6487::unholdAxisRange(char axis)
{
	if(axis=='X')
		holdXAxis=false;
	if(axis=='Y')
		holdYAxis=false;
}
