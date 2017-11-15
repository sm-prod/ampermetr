#define N_POINTS

#include "amp.h"
#include "ui_amp.h"
#include "lib.h"
#include "qcustomplot.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>
#include <QByteArray>
#include <QIODevice>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>
#include <QDesktopServices>
#include <QSettings>
#include <QCoreApplication>
#include <QTimer>
#include <QLabel>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QDebug>
#include <QEventLoop>
#include <QFlags>
#include <QMouseEvent>


//QString fname;
//QByteArray tmp;

bool flaggDelay=false;
//bool statusPort=false;

AMP::AMP(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::AMP)
{
	ui->setupUi(this);

	//read avaible comports
	foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
		ui->AmpPortBox->addItem(info.portName());

	settings = new QSettings("set",QSettings::NativeFormat);

	ui->sendString->setPlaceholderText("Команда");

	ui->AmpPortBox->setCurrentIndex(ui->AmpPortBox->findText(settings->value("PORT0",ui->AmpPortBox->currentText()).toString()));
	//settings->setValue("GRAPH","");

	ui->graphic->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


	ammeter = new Keithley6487(ui->textBrowser,ui->AmpPortBox,ui->graphic, ui->gridLayoutMod);
	//on_setMod_0_triggered();


	connect(ui->connectAmpButton, &QPushButton::clicked, ammeter, &Keithley6487::connectAmp);
	connect(ui->disconnectAmpButton, &QPushButton::clicked, ammeter, &Keithley6487::disconnectAmp);
	connect(ammeter,&Keithley6487::processEnd,this,AMP::activateWindow);
}

AMP::~AMP()
{
	delete ui;
}

void AMP::slotTimerAlarm()
{
	flaggDelay=true;
}

void AMP::on_sendAmpButton_clicked()
{
	ammeter->sendAmpCom(ui->sendString->text());
}

void AMP::on_searchAmpButton_clicked()
{
	ui->AmpPortBox->clear();
	foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
		ui->AmpPortBox->addItem(info.portName());
}

void AMP::cleanLayoutMod()
{
	QLayoutItem *ch;
	while ((ch = ui->gridLayoutMod->takeAt(0))!= 0)
	{
		ui->gridLayoutMod->removeItem(ch);
		ch->widget()->deleteLater();
		delete ch;
	}
}

void AMP::on_setMod_0_triggered()
{
	cleanLayoutMod();
	if(ammeter->changeMod0())
	{
		ui->connectAmpButton->setEnabled(true);
		ui->disconnectAmpButton->setEnabled(true);
		ui->labelModChange->setText("ВАХ");
	}
}

void AMP::on_setMod_1_triggered()
{
	cleanLayoutMod();
	if(ammeter->changeMod1())
	{
		ui->connectAmpButton->setEnabled(true);
		ui->disconnectAmpButton->setEnabled(true);
		ui->labelModChange->setText("ТАХ");
	}
}

void AMP::on_setMod_2_triggered()
{
	cleanLayoutMod();
	if(ammeter->changeMod2())
	{
		ui->connectAmpButton->setEnabled(true);
		ui->disconnectAmpButton->setEnabled(true);
		ui->labelModChange->setText("АВХ");
	}
}

void AMP::on_actionClean_triggered()
{
	ui->textBrowser->clear();
	ui->graphic->clearGraphs();
	ui->graphic->replot();
	ammeter->clearVect();
}


void AMP::on_saveGraph_triggered()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::homePath() + "/Documents/graphic.png", tr("Png (*png)") );
	QPixmap plotToPix;
	if (filename.at(filename.size()-2)=='p')
		ui->graphic->saveJpg(filename,0,0,1.0,100,1200);
	if (filename.at(filename.size()-2)=='n')
		ui->graphic->savePng(filename,0,0,1.0,100,1200);
}

void AMP::on_saveText_triggered()
{
	QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) );
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Document"), QDir::homePath() + "/Documents/graphic_tab.txt", tr("Text (*.txt)") );
	QFile out(filename);
	if( out.open( QIODevice::WriteOnly ) )
	{
		QTextStream stream( &out );
		stream << ui->textBrowser->toPlainText();//QObject::trUtf8( "Проверка!" );
		out.close();
	}
}
