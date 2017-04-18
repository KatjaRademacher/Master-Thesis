#ifndef QTHESISMAINWINDOW_H
#define QTHESISMAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <qsqldatabase.h>
#include "ui_qthesismainwindow.h"
#include "qthesisdbinterface.h"
#include "qthesissimulationdatainfo.h"

class QThesisMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QThesisMainWindow(QWidget *parent = 0);
	~QThesisMainWindow();

private slots:
	void onStartButtonClicked();
	void onWeatherAllSelected(bool checked);
	void onWeatherRainSelected(bool checked);
	void onWeatherDrySelected(bool checked);
	void onSpecialDaysAllSelected(bool checked);
	void onSpecialDaysWeekdaysSelected(bool checked);
	void onSpecialDaysWeekendsSelected(bool checked);
	void onSpecialDaysHolidaysSelected(bool checked);

private:
	Ui::QThesisMainWindowClass ui;
	QString mSimulationIntersectionPath;
	QString mSimulationBasePath;
	QString mSumoToolsPath;
	QString mTestRunPath;

	//simulation Data Info
	int mIntersectionNo;
	QDate mStartDate;
	QDate mEndDate;
	QTime mStartTime;
	QTime mEndTime;
	QString mSpecialDays;
	QString mWeatherDays;

	//simulation config
	bool mFixedTls;
	bool mActuatedTls;
	bool mDynamicTls;
	int mNoOfTestRuns;
	QThesisDbInterface *mDbInterface;

	void setUpUiWithDataFromDb();
	void runSimulations(QString flowFile, int endTime);
	QString createOutputDefinitions(QString tlsType);
	QString createSumoConfigFile(QStringList files, int endTime, QString tlsType);
	void simulateOneTest(QString config, QString tlsType);
	void simulateOneAdaptiveTlsTest(QString config, int endTime);
	void transformOutputFiles();
	void transformOutputFilesIntoCsv();
	void trimQueueOutputFiles();
	void trimEdgeOutputFiles();
	void trimLoopOutputFiles();
	void trimTlsStatesOutputFiles();
	void getDataFromUI();
	QThesisSimulationDataInfo* createSimulationInputDataInfoFromUI();
	void printWindowValues();
	QString boolToString(bool value);
};

#endif // QTHESISMAINWINDOW_H
