/**
* helpful class to keep information on a simulation test 
* organised and transfer it between elements
*
* @autor: Katja Rademacher
* @date: 29.11.2016
*/

#ifndef QTHESISSIMULATIONDATAINFO_H
#define QTHESISSIMULATIONDATAINFO_H

#include <QObject>
#include <QDate>
#include <QTime>

class QThesisSimulationDataInfo : public QObject
{
	Q_OBJECT

public:
	QThesisSimulationDataInfo(QObject *parent);
	~QThesisSimulationDataInfo();
	void setIntersectionNo(int intNo);
	void setDateSpan(QDate startD, QDate endD);
	void setTimeSpan(QTime startT, QTime endT);
	void setSpecialDaysOption(QString specDays);
	void setWeatherDaysOption(QString weatherDays);
	int getIntersectionNo();
	QDate getStartDate();
	QDate getEndDate();
	QTime getStartTime();
	QTime getEndTime();
	QString getSpecialDaysOption();
	QString getWeatherDaysOption();

private:
	int mIntersectionNo;
	QDate mStartDate;
	QDate mEndDate;
	QTime mStartTime;
	QTime mEndTime;
	QString mSpecialDays;
	QString mWeatherDays;
};

#endif // QTHESISSIMULATIONDATAINFO_H
