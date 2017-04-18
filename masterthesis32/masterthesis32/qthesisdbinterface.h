#ifndef QTHESISDBINTERFACE_H
#define QTHESISDBINTERFACE_H

#include <QObject>
#include <qsqldatabase.h>
#include <qsqlquerymodel.h>
#include <qsqlquery.h>

#include "qthesissimulationdatainfo.h"

class QThesisDbInterface : public QObject
{
	Q_OBJECT

public:
	QThesisDbInterface(QObject *parent);
	~QThesisDbInterface();
	bool openDbConnection();
	void closeDbConnection();
	QSqlQueryModel* getAvailableIntersections();
	QDate getMaximumDate();
	QDate getMinimumDate();
	QSqlQuery* getDetectorsForIntersection(int intersection);
	QSqlQuery* getDetectorDataForSelectedDays(int detector, QThesisSimulationDataInfo* dataInfo);
	QSqlQuery* getDetectorDataForSpecifiedTime(QTime time, QThesisSimulationDataInfo* dataInfo);
	QSqlQuery* getFlowInformationDataForDetector(int intersection, int detector);
	QSqlQuery* getFlowInformationDataForIntersection(int intersection);

private:
	QSqlDatabase mThesisDb;
	QString createDetectorDataQueryString(QString specialDay, QString weather);
	QString createTimeDataQueryString(QString specialDay, QString weather);
};

#endif // QTHESISDBINTERFACE_H
