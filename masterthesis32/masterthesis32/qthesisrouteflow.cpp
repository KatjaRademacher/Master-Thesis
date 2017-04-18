#include "qthesisrouteflow.h"

QThesisRouteFlow::QThesisRouteFlow(QObject *parent)
	: QObject(parent)
{

}

QThesisRouteFlow::~QThesisRouteFlow()
{

}

QString QThesisRouteFlow::toString()
{
	//<flow id="f8" begin="0" end="900" period="17" route="routescre_lan1" departLane="1" color="1,0,1" />
	QString flowString = "<flow ";
	flowString += "id=\"" + mId + "\" ";
	flowString += "begin=\"" + QString::number(mBeginTime) + "\" ";
	flowString += "end=\"" + QString::number(mEndTime) + "\" ";
	flowString += "number=\"" + QString::number(mNoOfVehicles) + "\" ";
	flowString += "route=\"" + mRouteId + "\" ";
	flowString += "departLane=\"" + QString::number(mDepartLane) + "\" ";
	if (mVehicleColor.length() != 0)
		flowString += "color=\"" + mVehicleColor + "\" ";
	flowString += " />";
	return flowString;
}

void QThesisRouteFlow::setId(QString id)
{
	mId = id;
}

void QThesisRouteFlow::setRouteId(QString routeId)
{
	mRouteId = routeId;
}

void QThesisRouteFlow::setBeginTime(int start)
{
	mBeginTime = start;
}

void QThesisRouteFlow::setEndTime(int end)
{
	mEndTime = end;
}

void QThesisRouteFlow::setVehicleVolume(double volume)
{
	mNoOfVehicles = (int)(volume + 0.5);
	if (mNoOfVehicles < 0)
		mNoOfVehicles = 0;
}

void QThesisRouteFlow::setDepartLane(int lane)
{
	mDepartLane = lane;
}

void QThesisRouteFlow::setColor(QString color)
{
	mVehicleColor = color;
}
