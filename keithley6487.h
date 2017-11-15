#ifndef KEITHLEY6487_H
#define KEITHLEY6487_H

#include "uart.h"
#include "lib.h"
#include "qcustomplot.h"

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QSerialPort>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

class Keithley6487 : public QObject
{
	Q_OBJECT
public:
	explicit Keithley6487(QTextBrowser *textBrowser, QComboBox *portKeithBox, QCustomPlot *graph, QGridLayout *uiGridLayotMod, QObject *parent = nullptr);
	bool getSerialStatus(int dev);
	void sendAmpCom(QString input);
	void clearVect(char unit=0);
signals:
	void processEnd();

public slots:
	void connectAmp();

	void connectTemp();

	void disconnectAmp();

	void disconnectTemp();

	void searchTempButton_clicked();

	void readSerialAmp();

	void readSerialAmpTime();

	void readSerialTemp();

	void writeData(QByteArray Data);

	void writeValueAmp(QByteArray Data);

	void writeValueTemp(QByteArray DataA, QByteArray DataT);

	void writeValueAmpTime(QByteArray Data);

	void setAmp();

	void mesurI(double volt);

	void mesurT(double volt);

	void on_voltValue_SpinBox_valueChanged(double arg1);

	void on_StepFirst_SpinBox_valueChanged(double arg1);

	void on_StepLast_SpinBox_valueChanged(double arg1);

	void on_Step_SpinBox_valueChanged(double arg1);

	void on_TimeMesur_SpinBox_valueChanged(int arg1);

	bool changeMod0();

	bool changeMod1();

	bool changeMod2();

	void startMod0();

	void startMod1();

	void startMod2();

	void cleanMod();

private:
	QGridLayout *uiGridLayoutMod;
	QCustomPlot *uiGraphic;
	QComboBox *uiPortKeithBox;
	QComboBox *uiPortTempBox;
	QTextBrowser *uiTextBrowser;
	uart *serialTemp=0;
	uart *serialKeithley=0;
	Lib func;
	QTime *time;
	double stepVolt;//Шаг по оси X             Перенести
	double startLine;//Начало отсчёта Оx          в файл
	double endLine;//Конец Оx                  конфигурации
	double voltValue;
	double tempValue;
	QVector<double> currentValues;
	QVector<double> voltValues;
	QVector<double> tempValues;
	int timeMesur;

	QLabel *label_FirstStep=0;

	QLabel *label_LastStep=0;

	QLabel *label_ChStep=0;

	QLabel *labelStep=0;

	QLabel *label_NumbStep=0;

	QLabel *labelTime=0;

	QLabel *label_MesurTime=0;

	QDoubleSpinBox *voltValue_SpinBox=0;

	QDoubleSpinBox *StepFirst_SpinBox=0;

	QDoubleSpinBox *StepLast_SpinBox=0;

	QDoubleSpinBox *Step_SpinBox=0;

	QSpinBox *TimeMesur_SpinBox=0;

	QPushButton *startButton=0;

	QPushButton *stopButton=0;

	QPushButton *searchTempButton=0;

	QPushButton *connectTempButton=0;

	QPushButton *disconnectTempButton=0;

	QComboBox *portTempBox=0;
};

#endif // KEITHLEY6487_H
