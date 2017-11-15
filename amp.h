#ifndef AMP_H
#define AMP_H

#include "uart.h"
#include "lib.h"
#include "keithley6487.h"
#include "qcustomplot.h"

#include <QMainWindow>
#include <QByteArray>
#include <QSettings>
#include <QTimer>
#include <QLabel>
#include <QGridLayout>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QComboBox>

namespace Ui {
class AMP;
}

class AMP : public QMainWindow
{
    Q_OBJECT

public:
    explicit AMP(QWidget *parent = 0);
    ~AMP();

private slots:
    void slotTimerAlarm();

    void cleanLayoutMod();

	void on_sendAmpButton_clicked();

	void on_searchAmpButton_clicked();

    void on_setMod_1_triggered();

    void on_setMod_0_triggered();

    void on_setMod_2_triggered();

	void on_actionClean_triggered();

	void on_saveGraph_triggered();

	void on_saveText_triggered();

private:
    Ui::AMP *ui;

	QSettings *settings;

	QTime *time;

	Lib func;

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

    QPushButton *search1Button=0;

    QPushButton *connect1Button=0;

    QPushButton *disconnect1Button=0;

    QComboBox *port1Box=0;

//	uart *Arduino=0;

//	QSerialPort *serial=0;

	Keithley6487 *ammeter=0;

};

#endif // AMP_H
