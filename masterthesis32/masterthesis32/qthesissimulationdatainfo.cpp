#include "qthesissimulationdatainfo.h"

QThesisSimulationDataInfo::QThesisSimulationDataInfo(QObject *parent)
	: QObject(parent)
{

}

QThesisSimulationDataInfo::~QThesisSimulationDataInfo()
{

}

void QThesisSimulationDataInfo::setIntersectionNo(int intNo)
{
	mIntersectionNo = intNo;
}

void QThesisSimulationDataInfo::setDateSpan(QDate startD, QDate endD)
{
	mStartDate = startD;
	mEndDate = endD;
}

void QThesisSimulationDataInfo::setTimeSpan(QTime startT, QTime endT)
{
	mStartTime = startT;
	mEndTime = endT;
}

void QThesisSimulationDataInfo::setSpecialDaysOption(QString specDays)
{
	mSpecialDays = specDays;
}

void QThesisSimulationDataInfo::setWeatherDaysOption(QString weatherDays)
{
	mWeatherDays = weatherDays;
}

int QThesisSimulationDataInfo::getIntersectionNo() { return mIntersectionNo; }
QDate QThesisSimulationDataInfo::getStartDate() { return mStartDate; }
QDate QThesisSimulationDataInfo::getEndDate() { return mEndDate; }
QTime QThesisSimulationDataInfo::getStartTime() { return mStartTime; }
QTime QThesisSimulationDataInfo::getEndTime() { return mEndTime; }
QString QThesisSimulationDataInfo::getSpecialDaysOption() { return mSpecialDays; }
QString QThesisSimulationDataInfo::getWeatherDaysOption() { return mWeatherDays; }

