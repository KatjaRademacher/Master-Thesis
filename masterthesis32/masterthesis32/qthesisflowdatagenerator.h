/**
* class to create the vehicle flows and the route file for the simulation using the 
* default route information and the number of approaching vehicles for each 15 minute interval that is defined in the database
* gets the information from the database and creates the flow objects 
* using those flow objects to create the route file and saving it to the simulation test folder
*
* @autor: Katja Rademacher
* @date: 29.11.2016
*/
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
