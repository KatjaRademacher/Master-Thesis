/**
* helpful class to define and keep route flow information organised
* includes a function to convert it into a string needed for a SUMO route file
*
* @autor: Katja Rademacher
* @date: 29.11.2016
*/

#ifndef QTHESISROUTEFLOW_H
#define QTHESISROUTEFLOW_H

#include <QObject>

class QThesisRouteFlow : public QObject
{
	Q_OBJECT

public:
	QThesisRouteFlow(QObject *parent);
	~QThesisRouteFlow();
	QString toString();
	void setId(QString id);
	void setRouteId(QString routeId);
	void setBeginTime(int start);
	void setEndTime(int end);
	void setVehicleVolume(double volume);
	void setDepartLane(int lane);
	void setColor(QString color);

private:
	QString mId;
	QString mRouteId;
	int mBeginTime;
	int mEndTime;
	int mNoOfVehicles;
	int mDepartLane;
	QString mVehicleColor;
};

#endif // QTHESISROUTEFLOW_H
