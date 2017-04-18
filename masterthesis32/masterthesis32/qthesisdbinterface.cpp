#include <QDate>
#include "qthesisdbinterface.h"

QThesisDbInterface::QThesisDbInterface(QObject *parent)
	: QObject(parent)
{

}

QThesisDbInterface::~QThesisDbInterface()
{

}

bool QThesisDbInterface::openDbConnection()
{
	mThesisDb = QSqlDatabase::addDatabase("QODBC");
	mThesisDb.setDatabaseName("Thesis_DB");
	mThesisDb.setUserName("dit");
	mThesisDb.setPassword("dit");
	return mThesisDb.open();
}

void QThesisDbInterface::closeDbConnection()
{

}

QSqlQueryModel* QThesisDbInterface::getAvailableIntersections()
{
	QSqlQueryModel *intersectionModel = new QSqlQueryModel();
	intersectionModel->setQuery("SELECT intersection_name, intersection_id FROM intersections_tab", mThesisDb);
	intersectionModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
	intersectionModel->setHeaderData(1, Qt::Horizontal, tr("ID"));
	return intersectionModel;
}

QDate QThesisDbInterface::getMaximumDate()
{
	QSqlQuery maxDateQuery("SELECT MAX(traffic_date) FROM thesis_tab");
	maxDateQuery.first();
	QString dateString = maxDateQuery.value(0).toString();
	return QDateTime::fromString(dateString, "yyyy-MM-ddThh:mm:ss").date();
}

QDate QThesisDbInterface::getMinimumDate()
{
	QSqlQuery minDateQuery("SELECT MIN(traffic_date) FROM thesis_tab");
	minDateQuery.first();
	QString dateString = minDateQuery.value(0).toString();
	return QDateTime::fromString(dateString, "yyyy-MM-ddThh:mm:ss").date();
}

QSqlQuery* QThesisDbInterface::getDetectorsForIntersection(int intersection)
{
	QSqlQuery* query = new QSqlQuery(mThesisDb);
	query->prepare("SELECT DISTINCT traffic_detector FROM thesis_tab WHERE traffic_intersection = :intNo");
	query->bindValue(":intNo",intersection);
	bool validQuery = query->exec();
	return query;
}

QSqlQuery* QThesisDbInterface::getDetectorDataForSelectedDays(int detector, QThesisSimulationDataInfo* dataInfo)
{
	QString queryString = createDetectorDataQueryString(dataInfo->getSpecialDaysOption(), dataInfo->getWeatherDaysOption());
	QSqlQuery* query = new QSqlQuery(mThesisDb);
	query->prepare(queryString);
	query->bindValue(":intNo", dataInfo->getIntersectionNo());
	query->bindValue(":detector", detector);
	query->bindValue(":startDate", dataInfo->getStartDate());
	query->bindValue(":endDate", dataInfo->getEndDate());
	query->bindValue(":startTime", dataInfo->getStartTime());
	query->bindValue(":endTime", dataInfo->getEndTime());
	bool validQuery = query->exec();
	if (validQuery)
		return query;
	else return NULL;
}

QSqlQuery* QThesisDbInterface::getDetectorDataForSpecifiedTime(QTime time, QThesisSimulationDataInfo* dataInfo)
{
	QString queryString = createTimeDataQueryString(dataInfo->getSpecialDaysOption(), dataInfo->getWeatherDaysOption());
	QSqlQuery* query = new QSqlQuery(mThesisDb);
	query->prepare(queryString);
	query->bindValue(":intNo", dataInfo->getIntersectionNo());
	query->bindValue(":time", time);
	query->bindValue(":startDate", dataInfo->getStartDate());
	query->bindValue(":endDate", dataInfo->getEndDate());
	bool validQuery = query->exec();
	if (validQuery)
		return query;
	else return NULL;
}

QSqlQuery* QThesisDbInterface::getFlowInformationDataForDetector(int intersection, int detector)
{
	QSqlQuery* query = new QSqlQuery(mThesisDb);
	QString queryString = "SELECT route_id, departure_lane, vehicle_color FROM simulation_flow_tab";
	queryString += " WHERE intersection_no = :intNo AND detector_no = :detector";
	query->prepare(queryString);
	query->bindValue(":intNo",intersection);
	query->bindValue(":detector",detector);
	bool validQuery = query->exec();
	if (validQuery)
		return query;
	else return NULL;
}

QSqlQuery* QThesisDbInterface::getFlowInformationDataForIntersection(int intersection)
{
	QSqlQuery* query = new QSqlQuery(mThesisDb);
	QString queryString = "SELECT detector_no, route_id, departure_lane, vehicle_color FROM simulation_flow_tab";
	queryString += " WHERE intersection_no = :intNo ORDER BY detector_no";
	query->prepare(queryString);
	query->bindValue(":intNo",intersection);
	bool validQuery = query->exec();
	return query;
}

QString QThesisDbInterface::createDetectorDataQueryString(QString specialDay, QString weather)
{
	QString query = "";
	QStringList specialDayOptions;
	specialDayOptions << "all" << "weekdays" << "weekends" << "holidays";
	
	switch(specialDayOptions.indexOf(specialDay))
	{
		case 0: //all days
			if (weather == "rain") 
			{
				query = "SELECT traffic_time AS TrafficTime, AVG(traffic_volume) AS Volume FROM alldays_rainy_view ";
				query += "WHERE traffic_intersection = :intNo AND traffic_detector = :detector ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "AND traffic_time >= :startTime AND traffic_time <= :endTime ";
				query += "GROUP BY traffic_time ORDER BY traffic_time";
			}
			else if (weather == "dry") 
			{
				query = "SELECT traffic_time AS TrafficTime, AVG(traffic_volume) AS Volume FROM alldays_dry_view ";
				query += "WHERE traffic_intersection = :intNo AND traffic_detector = :detector ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "AND traffic_time >= :startTime AND traffic_time <= :endTime ";
				query += "GROUP BY traffic_time ORDER BY traffic_time";
			}
			else { //all weather
				query = "SELECT traffic_time AS TrafficTime, AVG(traffic_volume) AS Volume FROM thesis_tab ";
				query += "WHERE traffic_intersection = :intNo AND traffic_detector = :detector ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "AND traffic_time >= :startTime AND traffic_time <= :endTime ";
				query += "GROUP BY traffic_time ORDER BY traffic_time";
			}
			break;
		case 1: //weekdays
			if (weather == "rain") 
			{
				query = "SELECT traffic_time AS TrafficTime, AVG(traffic_volume) AS Volume FROM weekday_volumes ";
				query += "LEFT JOIN weather_rainfall_daily ON weather_rainfall_daily.weather_date = weekday_volumes.traffic_date ";
				query += "WHERE traffic_intersection = :intNo AND traffic_detector = :detector ";
				query += "AND weekday_volumes.traffic_date = weather_rainfall_daily.weather_date AND weather_rainfall_daily.daily_rainfall >= 2 ";
				query += "AND weekday_volumes.traffic_date >= :startDate AND weekday_volumes.traffic_date <= :endDate ";
				query += "AND weekday_volumes.traffic_time >= :startTime AND weekday_volumes.traffic_time <= :endTime ";
				query += "GROUP BY weekday_volumes.traffic_time ORDER BY weekday_volumes.traffic_time";
			}
			else if (weather == "dry") 
			{
				query = "SELECT traffic_time AS TrafficTime, AVG(traffic_volume) AS Volume FROM weekday_volumes ";
				query += "LEFT JOIN weather_rainfall_daily ON weather_rainfall_daily.weather_date = weekday_volumes.traffic_date ";
				query += "WHERE traffic_intersection = :intNo AND traffic_detector = :detector ";
				query += "AND (weather_rainfall_daily.weather_date IS NULL ";
				query += "OR (weekday_volumes.traffic_date = weather_rainfall_daily.weather_date AND weather_rainfall_daily.daily_rainfall < 2)) ";
				query += "AND weekday_volumes.traffic_date >= :startDate AND weekday_volumes.traffic_date <= :endDate ";
				query += "AND weekday_volumes.traffic_time >= :startTime AND weekday_volumes.traffic_time <= :endTime ";
				query += "GROUP BY weekday_volumes.traffic_time ORDER BY weekday_volumes.traffic_time";
			}
			else { //all weather
				query = "SELECT traffic_time AS TrafficTime, AVG(traffic_volume) AS Volume FROM weekday_volumes ";
				query += "WHERE traffic_intersection = :intNo AND traffic_detector = :detector ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "AND traffic_time >= :startTime AND traffic_time <= :endTime ";
				query += "GROUP BY traffic_time ORDER BY traffic_time";
			}
			break;
		case 2: //weekends
			if (weather == "rain") 
			{
				query = "SELECT traffic_time AS TrafficTime, AVG(traffic_volume) AS Volume FROM weekend_volumes ";
				query += "LEFT JOIN weather_rainfall_daily ON weather_rainfall_daily.weather_date = weekend_volumes.traffic_date ";
				query += "WHERE traffic_intersection = :intNo AND traffic_detector = :detector ";
				query += "AND weekend_volumes.traffic_date = weather_rainfall_daily.weather_date AND weather_rainfall_daily.daily_rainfall >= 2 ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "AND traffic_time >= :startTime AND traffic_time <= :endTime ";
				query += "GROUP BY traffic_time ORDER BY traffic_time";
			}
			else if (weather == "dry") 
			{
				query = "SELECT traffic_time AS TrafficTime, AVG(traffic_volume) AS Volume FROM weekend_volumes ";
				query += "LEFT JOIN weather_rainfall_daily ON weather_rainfall_daily.weather_date = weekend_volumes.traffic_date ";
				query += "WHERE traffic_intersection = :intNo AND traffic_detector = :detector ";
				query += "AND (weather_rainfall_daily.weather_date IS NULL ";
				query += "OR (weekend_volumes.traffic_date = weather_rainfall_daily.weather_date AND weather_rainfall_daily.daily_rainfall < 2)) ";
				query += "AND weekend_volumes.traffic_date >= :startDate AND weekend_volumes.traffic_date <= :endDate ";
				query += "AND weekend_volumes.traffic_time >= :startTime AND weekend_volumes.traffic_time <= :endTime ";
				query += "GROUP BY weekend_volumes.traffic_time ORDER BY weekend_volumes.traffic_time";
			}
			else { //all weather
				query = "SELECT traffic_time AS TrafficTime, AVG(traffic_volume) AS Volume FROM weekend_volumes ";
				query += "WHERE traffic_intersection = :intNo AND traffic_detector = :detector ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "AND traffic_time >= :startTime AND traffic_time <= :endTime ";
				query += "GROUP BY traffic_time ORDER BY traffic_time";
			}
			break;
		case 3: //holidays
			if (weather == "rain") 
			{
				query = "SELECT traffic_time AS TrafficTime, AVG(traffic_volume) AS Volume FROM holiday_volumes ";
				query += "LEFT JOIN weather_rainfall_daily ON weather_rainfall_daily.weather_date = holiday_volumes.traffic_date ";
				query += "WHERE traffic_intersection = :intNo AND traffic_detector = :detector ";
				query += "AND holiday_volumes.traffic_date = weather_rainfall_daily.weather_date AND weather_rainfall_daily.daily_rainfall >= 2 ";
				query += "AND holiday_volumes.traffic_date >= :startDate AND holiday_volumes.traffic_date <= :endDate ";
				query += "AND holiday_volumes.traffic_time >= :startTime AND holiday_volumes.traffic_time <= :endTime ";
				query += "GROUP BY holiday_volumes.traffic_time ORDER BY holiday_volumes.traffic_time";
			}
			else if (weather == "dry") 
			{
				query = "SELECT traffic_time AS TrafficTime, AVG(traffic_volume) AS Volume FROM holiday_volumes ";
				query += "LEFT JOIN weather_rainfall_daily ON weather_rainfall_daily.weather_date = holiday_volumes.traffic_date ";
				query += "WHERE traffic_intersection = :intNo AND traffic_detector = :detector ";
				query += "AND (weather_rainfall_daily.weather_date IS NULL ";
				query += "OR (holiday_volumes.traffic_date = weather_rainfall_daily.weather_date AND weather_rainfall_daily.daily_rainfall < 2)) ";
				query += "AND holiday_volumes.traffic_date >= :startDate AND holiday_volumes.traffic_date <= :endDate ";
				query += "AND holiday_volumes.traffic_time >= :startTime AND holiday_volumes.traffic_time <= :endTime ";
				query += "GROUP BY holiday_volumes.traffic_time ORDER BY holiday_volumes.traffic_time";
			}
			else { //all weather
				query = "SELECT traffic_time AS TrafficTime, AVG(traffic_volume) AS Volume FROM holiday_volumes ";
				query += "WHERE traffic_intersection = :intNo AND traffic_detector = :detector ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "AND traffic_time >= :startTime AND traffic_time <= :endTime ";
				query += "GROUP BY traffic_time ORDER BY traffic_time";
			}
			break;
	}
	return query;
}

QString QThesisDbInterface::createTimeDataQueryString(QString specialDay, QString weather)
{
	QString query = "";
	QStringList specialDayOptions;
	specialDayOptions << "all" << "weekdays" << "weekends" << "holidays";
	
	switch(specialDayOptions.indexOf(specialDay))
	{
		case 0: //all days
			if (weather == "rain") 
			{
				query = "SELECT traffic_detector, AVG(traffic_volume) FROM alldays_rainy_view ";
				query += "WHERE traffic_intersection = :intNo AND traffic_time = :time ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "GROUP BY traffic_detector ORDER BY traffic_detector";
			}
			else if (weather == "dry") 
			{
				query = "SELECT traffic_detector, AVG(traffic_volume) FROM alldays_dry_view ";
				query += "WHERE traffic_intersection = :intNo AND traffic_time = :time ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "GROUP BY traffic_detector ORDER BY traffic_detector";
			}
			else { //all weather
				query = "SELECT traffic_detector, AVG(traffic_volume) FROM thesis_tab ";
				query += "WHERE traffic_intersection = :intNo AND traffic_time = :time ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "GROUP BY traffic_detector ORDER BY traffic_detector";
			}
			break;
		case 1: //weekdays
			if (weather == "rain") 
			{
				query = "SELECT traffic_detector, AVG(traffic_volume) AS Volume FROM weekday_volumes ";
				query += "LEFT JOIN weather_rainfall_daily ON weather_rainfall_daily.weather_date = weekday_volumes.traffic_date ";
				query += "WHERE traffic_intersection = :intNo AND weekday_volumes.traffic_time = :time ";
				query += "AND weekday_volumes.traffic_date = weather_rainfall_daily.weather_date AND weather_rainfall_daily.daily_rainfall >= 5 ";
				query += "AND weekday_volumes.traffic_date >= :startDate AND weekday_volumes.traffic_date <= :endDate ";
				query += "GROUP BY traffic_detector ORDER BY traffic_detector";
			}
			else if (weather == "dry") 
			{
				query = "SELECT traffic_detector, AVG(traffic_volume) FROM weekday_volumes ";
				query += "LEFT JOIN weather_rainfall_daily ON weather_rainfall_daily.weather_date = weekday_volumes.traffic_date ";
				query += "WHERE traffic_intersection = :intNo AND weekday_volumes.traffic_time = :time ";
				query += "AND (weather_rainfall_daily.weather_date IS NULL ";
				query += "OR (weekday_volumes.traffic_date = weather_rainfall_daily.weather_date AND weather_rainfall_daily.daily_rainfall < 5)) ";
				query += "AND weekday_volumes.traffic_date >= :startDate AND weekday_volumes.traffic_date <= :endDate ";
				query += "GROUP BY traffic_detector ORDER BY traffic_detector";
			}
			else { //all weather
				query = "SELECT traffic_detector, AVG(traffic_volume) FROM weekday_volumes ";
				query += "WHERE traffic_intersection = :intNo AND traffic_time = :time ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "GROUP BY traffic_detector ORDER BY traffic_detector";
			}
			break;
		case 2: //weekends
			if (weather == "rain") 
			{
				query = "SELECT traffic_detector, AVG(traffic_volume) FROM weekend_volumes ";
				query += "LEFT JOIN weather_rainfall_daily ON weather_rainfall_daily.weather_date = weekend_volumes.traffic_date ";
				query += "WHERE traffic_intersection = :intNo AND traffic_time = :time ";
				query += "AND weekend_volumes.traffic_date = weather_rainfall_daily.weather_date AND weather_rainfall_daily.daily_rainfall >= 5 ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "GROUP BY traffic_detector ORDER BY traffic_detector";
			}
			else if (weather == "dry") 
			{
				query = "SELECT traffic_detector, AVG(traffic_volume) FROM weekend_volumes ";
				query += "LEFT JOIN weather_rainfall_daily ON weather_rainfall_daily.weather_date = weekend_volumes.traffic_date ";
				query += "WHERE traffic_intersection = :intNo AND weekend_volumes.traffic_time = :time ";
				query += "AND (weather_rainfall_daily.weather_date IS NULL ";
				query += "OR (weekend_volumes.traffic_date = weather_rainfall_daily.weather_date AND weather_rainfall_daily.daily_rainfall < 5)) ";
				query += "AND weekend_volumes.traffic_date >= :startDate AND weekend_volumes.traffic_date <= :endDate ";
				query += "GROUP BY traffic_detector ORDER BY traffic_detector";
			}
			else { //all weather
				query = "SELECT traffic_detector, AVG(traffic_volume) FROM weekend_volumes ";
				query += "WHERE traffic_intersection = :intNo AND traffic_time = :time ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "GROUP BY traffic_detector ORDER BY traffic_detector";
			}
			break;
		case 3: //holidays
			if (weather == "rain") 
			{
				query = "SELECT traffic_detector, AVG(traffic_volume) FROM holiday_volumes ";
				query += "LEFT JOIN weather_rainfall_daily ON weather_rainfall_daily.weather_date = holiday_volumes.traffic_date ";
				query += "WHERE traffic_intersection = :intNo AND holiday_volumes.traffic_time = :time ";
				query += "AND holiday_volumes.traffic_date = weather_rainfall_daily.weather_date AND weather_rainfall_daily.daily_rainfall >= 5 ";
				query += "AND holiday_volumes.traffic_date >= :startDate AND holiday_volumes.traffic_date <= :endDate ";
				query += "GROUP BY traffic_detector ORDER BY traffic_detector";
			}
			else if (weather == "dry") 
			{
				query = "SELECT traffic_detector, AVG(traffic_volume) FROM holiday_volumes ";
				query += "LEFT JOIN weather_rainfall_daily ON weather_rainfall_daily.weather_date = holiday_volumes.traffic_date ";
				query += "WHERE traffic_intersection = :intNo AND holiday_volumes.traffic_time = :time ";
				query += "AND (weather_rainfall_daily.weather_date IS NULL ";
				query += "OR (holiday_volumes.traffic_date = weather_rainfall_daily.weather_date AND weather_rainfall_daily.daily_rainfall < 5)) ";
				query += "AND holiday_volumes.traffic_date >= :startDate AND holiday_volumes.traffic_date <= :endDate ";
				query += "GROUP BY traffic_detector ORDER BY traffic_detector";
			}
			else { //all weather
				query = "SELECT traffic_detector, AVG(traffic_volume) FROM holiday_volumes ";
				query += "WHERE traffic_intersection = :intNo AND traffic_time = :time ";
				query += "AND traffic_date >= :startDate AND traffic_date <= :endDate ";
				query += "GROUP BY traffic_detector ORDER BY traffic_detector";
			}
			break;
	}
	return query;
}
