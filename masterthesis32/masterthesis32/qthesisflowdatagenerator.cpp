#include "qthesisflowdatagenerator.h"



#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>

QThesisFlowDataGenerator::QThesisFlowDataGenerator(QObject *parent, QThesisDbInterface* dbInterface)
	: QObject(parent)
{
	mDbInterface = dbInterface;
	mEndTime = 0;
}

QThesisFlowDataGenerator::~QThesisFlowDataGenerator()
{

}

QString QThesisFlowDataGenerator::generateSortedFlowDataFile(QThesisSimulationDataInfo* info, QString testPath, QString routePath)
{
	QList<QList<QThesisRouteFlow*>*>* detectorFlows = new QList<QList<QThesisRouteFlow*>*>();
	
	int i = 0;
	int duration = 900;
	QTime currentTime = info->getStartTime();
	QTime endTime = info->getEndTime();
	while (currentTime <= endTime)
	{
		QSqlQuery *detectorData = mDbInterface->getDetectorDataForSpecifiedTime(currentTime, info);
		QSqlQuery* flowData = mDbInterface->getFlowInformationDataForIntersection(info->getIntersectionNo());
		if (flowData == NULL || detectorData == NULL)
			return NULL;
		detectorFlows->append(generateFlowsForTime(detectorData, flowData, i*duration));
		i++;
		if (currentTime == endTime) break;
		currentTime = currentTime.addSecs(duration);
		
	}
	mEndTime = i*duration;

	QDir workingDir;
	if (workingDir.mkpath(testPath))
	{
		QString fileName = testPath + "int" + QString::number(info->getIntersectionNo()) + "_vflows.rou.xml";
		QString routeDescFileName = routePath + "int" + QString::number(info->getIntersectionNo()) + "-routedesc.txt";
		bool fileCreated = generateFile(detectorFlows, fileName, routeDescFileName);
		if (fileCreated)
			return fileName;
	}
	
	return NULL;
}

QList<QThesisRouteFlow*>* QThesisFlowDataGenerator::generateFlowsForTime(QSqlQuery* detectorData, QSqlQuery* flowData, int beginTime)
{
	int timeSpan = 900;
	QList<QThesisRouteFlow*>* flowList = new QList<QThesisRouteFlow*>();
	if (detectorData->size() == flowData->size())
	{
		while (detectorData->next())
		{
			flowData->next();
			int detFromLoopData = detectorData->value(0).toInt();
			int detFromFlowData = flowData->value(0).toInt();
			if (detFromLoopData < detFromFlowData)
				while (detFromLoopData < detFromFlowData) 
				{
					detectorData->next();
					detFromLoopData = detectorData->value(0).toInt();
				}
			double vehVolume = detectorData->value(1).toDouble();
			QThesisRouteFlow* flow = new QThesisRouteFlow(this);
			flow->setId("flow" + QString::number(beginTime) + "_det" + flowData->value(0).toString());
			flow->setBeginTime(beginTime);
			flow->setEndTime(beginTime + timeSpan);
			flow->setVehicleVolume(vehVolume);
			flow->setRouteId(flowData->value(1).toString());
			flow->setColor(flowData->value(3).toString());
			flow->setDepartLane(flowData->value(2).toDouble());
			flowList->append(flow);
		}
	}
	return flowList;
}

bool QThesisFlowDataGenerator::generateFile(QList<QList<QThesisRouteFlow*>*>* detectorFlows, QString fileName, QString routeDescFile)
{
	QFile routeFlows( fileName );
	if ( routeFlows.open(QIODevice::ReadWrite) )
	{
		QTextStream stream( &routeFlows );
		stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
		stream << "<routes>" << endl;

		//add route descriptions - needed in one file
		QFile routeDesc(routeDescFile);
		if(!routeDesc.open(QIODevice::ReadOnly)) {
			return false;
		}

		QTextStream in(&routeDesc);

		while(!in.atEnd()) {
			stream << in.readLine() << endl;       
		}

		routeDesc.close();
		stream << endl;
		//add flows
		for (int det = 0; det < detectorFlows->count(); det++)
		{
			QList<QThesisRouteFlow*>* flows = detectorFlows->at(det);
			for (int i = 0; i < flows->count(); i++)
			{
				stream << flows->at(i)->toString() << endl;
			}
		}
		stream << "</routes>" << endl;
		routeFlows.close();
	return true;
	}
	return false;
}
