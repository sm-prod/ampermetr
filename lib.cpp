#include "lib.h"

#include <QTime>
#include <QEventLoop>
#include <QCoreApplication>

Lib::Lib()
{

}

Lib::~Lib()
{

}

void Lib::delay(int msec)
{
	QTime dieTime = QTime::currentTime().addMSecs( msec );
	while( QTime::currentTime() < dieTime )
	{
		QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
	}
}
