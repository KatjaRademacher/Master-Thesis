#ifndef QTHESISFLOWDATAGENERATOR_H
#define QTHESISFLOWDATAGENERATOR_H

#include <QObject>
#include <qsqlquery.h>

#include "qthesisdbinterface.h"
#include "qthesissimulationdatainfo.h"
#include "qthesisrouteflow.h"

class QThesisFlowDataGenerator : public QObject
{
	Q_OBJECT

public:
	QThesisFlowDataGenerator(QObject *parent, QThesisDbInterface* dbInterface);
	~QThesisFlowDataGenerator();
	QString generateSortedFlowDataFile(QThesisSimulationDataInfo* info, QString testPath, QString routePath);
	int getSimulationEndTime() { return mEndTime; };

private:
	int mEndTime;
	QThesisDbInterface* mDbInterface;
	QList<QThesisRouteFlow*>* generateFlowsForTime(QSqlQuery* detectorData, QSqlQuery* flowData, int beginTime);
	bool generateFile(QList<QList<QThesisRouteFlow*>*>* detectorFlows, QString fileName, QString routePath); 
	
};

#endif // QTHESISFLOWDATAGENERATOR_H
