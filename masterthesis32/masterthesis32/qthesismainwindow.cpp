#include <qdebug.h>
#include <qsqlquerymodel.h>
#include <qprocess.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qregexp.h>
#include <qmessagebox.h>

#include "qthesismainwindow.h"
#include "qthesisflowdatagenerator.h"

QThesisMainWindow::QThesisMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	mSumoToolsPath = "../../Simulation/tools/";
	mSimulationBasePath = "../../Simulation/";
	mNoOfTestRuns = 1;
	mWeatherDays = "all";
	mSpecialDays = "all";

	//signals in ui
	connect(ui.start_button, SIGNAL(clicked()), this, SLOT(onStartButtonClicked()));
	connect(ui.days_all, SIGNAL(toggled(bool)), this, SLOT(onSpecialDaysAllSelected(bool)));
	connect(ui.days_holidays, SIGNAL(toggled(bool)), this, SLOT(onSpecialDaysHolidaysSelected(bool)));
	connect(ui.days_weekdays, SIGNAL(toggled(bool)), this, SLOT(onSpecialDaysWeekdaysSelected(bool)));
	connect(ui.days_weekends, SIGNAL(toggled(bool)), this, SLOT(onSpecialDaysWeekendsSelected(bool)));
	connect(ui.weather_all, SIGNAL(toggled(bool)), this, SLOT(onWeatherAllSelected(bool)));
	connect(ui.weather_dry, SIGNAL(toggled(bool)), this, SLOT(onWeatherDrySelected(bool)));
	connect(ui.weather_rain, SIGNAL(toggled(bool)), this, SLOT(onWeatherRainSelected(bool)));

	setUpUiWithDataFromDb();
}

QThesisMainWindow::~QThesisMainWindow()
{
	mDbInterface->closeDbConnection();
}

void QThesisMainWindow::setUpUiWithDataFromDb()
{
	//get data from access db
	mDbInterface = new QThesisDbInterface(this);
	bool openDB = mDbInterface->openDbConnection();
	if (openDB)
	{
		// combobox with selection of intersections
		QSqlQueryModel *intersectionModel = mDbInterface->getAvailableIntersections();
		ui.combo_intersection->setModel(intersectionModel);

		//max and min Date values
		QDate maxDate = mDbInterface->getMaximumDate();
		QDate minDate = mDbInterface->getMinimumDate();
		ui.date_start->setMaximumDate(maxDate);
		ui.date_start->setMinimumDate(minDate);
		ui.date_end->setMaximumDate(maxDate);
		ui.date_end->setMinimumDate(minDate);

		ui.date_start->setDate(minDate);
		ui.date_end->setDate(maxDate);
	}
}

void QThesisMainWindow::onStartButtonClicked()
{
	getDataFromUI();
	mSimulationIntersectionPath = mSimulationBasePath + "int" + QString::number(mIntersectionNo) + "/";
	mTestRunPath = mSimulationIntersectionPath + "exp" + QDateTime::currentDateTime().toString("yyyyMMddhhmm") + "/";
	printWindowValues();
	QThesisFlowDataGenerator* flowGenerator = new QThesisFlowDataGenerator(this, mDbInterface);
	QString fileName = flowGenerator->generateSortedFlowDataFile(createSimulationInputDataInfoFromUI(), mTestRunPath, mSimulationIntersectionPath);
	if (fileName != NULL) 
	{
		int simulationEndTime = flowGenerator->getSimulationEndTime();	
		runSimulations(fileName, simulationEndTime);
		transformOutputFiles();
		QMessageBox::information(this, "Information","Simulation Test Run finished");
	}
	else QMessageBox::critical(this, "Error", "Exit on Error when reading data from DB");
}



void QThesisMainWindow::onWeatherAllSelected(bool checked) 
{
	if (checked)
		mWeatherDays = "all";
}

void QThesisMainWindow::onWeatherRainSelected(bool checked) 
{
	if (checked)
		mWeatherDays = "rain";
}

void QThesisMainWindow::onWeatherDrySelected(bool checked)
{
	if (checked)
		mWeatherDays = "dry";
}

void QThesisMainWindow::onSpecialDaysAllSelected(bool checked) 
{
	if (checked)
		mSpecialDays = "all";
}

void QThesisMainWindow::onSpecialDaysWeekdaysSelected(bool checked) 
{
	if (checked)
		mSpecialDays = "weekdays";
}

void QThesisMainWindow::onSpecialDaysWeekendsSelected(bool checked) 
{
	if (checked)
		mSpecialDays = "weekends";
}

void QThesisMainWindow::onSpecialDaysHolidaysSelected(bool checked) 
{
	if (checked)
		mSpecialDays = "holidays";
}

void QThesisMainWindow::getDataFromUI()
{
	mIntersectionNo = ui.combo_intersection->model()->data(ui.combo_intersection->model()->index(ui.combo_intersection->currentIndex(), 1)).toInt();
	mStartDate = ui.date_start->date();
	mEndDate = ui.date_end->date();
	mStartTime = ui.time_start->time();
	mEndTime = ui.time_end->time();
	mNoOfTestRuns = ui.simulation_runs->value();
	mActuatedTls = ui.tls_actuated->isChecked();
	mDynamicTls = ui.tls_adaptive->isChecked();
	mFixedTls = ui.tls_fixed->isChecked();
}

QThesisSimulationDataInfo* QThesisMainWindow::createSimulationInputDataInfoFromUI()
{
	QThesisSimulationDataInfo* info = new QThesisSimulationDataInfo(this);
	info->setIntersectionNo(mIntersectionNo);
	info->setDateSpan(mStartDate, mEndDate);
	info->setTimeSpan(mStartTime, mEndTime);
	info->setSpecialDaysOption(mSpecialDays);
	info->setWeatherDaysOption(mWeatherDays);
	return info;
}

void QThesisMainWindow::printWindowValues()
{
	QDir directory;
	directory.mkpath(mTestRunPath);
	QFile docFile(mTestRunPath + "a_test_documentation.txt");
	if (docFile.open(QIODevice::WriteOnly  | QIODevice::Text))
	{
		QTextStream out(&docFile);
		out << "Runs of Traffic light Control Simulation Description" << endl;
		out << "Date of Test Run: " +QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm") << endl << endl;
		out << "Intersection: " + QString::number(mIntersectionNo) << endl;
		out << "Start Date: " + mStartDate.toString("yyyy-MM-dd") << endl;
		out << "End Date: " + mEndDate.toString("yyyy-MM-dd") << endl;
		out << "Start Time: " + mStartTime.toString("HH:mm") << endl;
		out << "End Time: " + mEndTime.toString("HH:mm") << endl;
		out << "Special Days: " + mSpecialDays << endl;
		out << "Weather Days: " + mWeatherDays << endl;
		out << "Actuated TLC: " + boolToString(mActuatedTls) << endl;
		out << "Fixed TLC: " + boolToString(mFixedTls) << endl;
		out << "Adaptive TLC: " + boolToString(mDynamicTls) << endl;
		out << "Test Runs: " + QString::number(mNoOfTestRuns) << endl;

		docFile.flush();
		docFile.close();
	}
	else QMessageBox::critical(this, "Error", docFile.errorString());
}

void QThesisMainWindow::runSimulations(QString flowFile, int endTime) 
{
	QString tlsType = "";
	if (mFixedTls)
	{
		tlsType = "static";
		QString outputDef = createOutputDefinitions(tlsType);
		QStringList files;
		files << QFileInfo(flowFile).fileName();
		files << QFileInfo(outputDef).fileName();
		QString configFile = createSumoConfigFile(files, endTime, tlsType);
		for (int i = 0; i < mNoOfTestRuns; i++) 
		{
			simulateOneTest(configFile, tlsType);
		}
	}

	if (mActuatedTls)
	{
		tlsType = "actuated";
		QString outputDef = createOutputDefinitions(tlsType);
		QStringList files;
		files << QFileInfo(flowFile).fileName();
		files << QFileInfo(outputDef).fileName();
		QString configFile = createSumoConfigFile(files, endTime, tlsType);
		for (int i = 0; i < mNoOfTestRuns; i++) 
		{
			simulateOneTest(configFile, tlsType);
		}
	}

	if (mDynamicTls)
	{
		tlsType = "adaptive";
		QString outputDef = createOutputDefinitions(tlsType);
		QString loopsV1FileName = "int" + QString::number(mIntersectionNo) + "-loops25m.det.xml";
		QFile::copy(mSimulationIntersectionPath + loopsV1FileName, mTestRunPath + loopsV1FileName);
		QStringList filesV1;
		filesV1 << QFileInfo(flowFile).fileName();
		filesV1 << QFileInfo(outputDef).fileName() + "," + mTestRunPath + loopsV1FileName;
		QString configFileV1 = createSumoConfigFile(filesV1, endTime, tlsType);
		for (int i = 0; i < mNoOfTestRuns; i++) 
		{
			simulateOneAdaptiveTlsTest(configFileV1, endTime);
		}
	}
}

QString QThesisMainWindow::createOutputDefinitions(QString tlsType)
{
	QString fileName = mTestRunPath + "output_def_" + tlsType + ".add.xml";
	QFile outputAdd(fileName);
	if (! outputAdd.exists()) 
	{
		if ( outputAdd.open(QIODevice::WriteOnly | QIODevice::Text) )
		{
			QTextStream stream( &outputAdd );
			stream << "<additional>" << endl;
			stream << "<edgeData id=\"edgedump1\" file=\"edgedata.out.xml\" freq=\"60\" excludeEmpty=\"true\" />" << endl;
			stream << "<timedEvent type=\"SaveTLSSwitchStates\" source=\"int" + QString::number(mIntersectionNo) + "\" dest=\"tlsdata.out.xml\"/>" << endl;
			stream << "</additional>" << endl;
			outputAdd.close();
		return fileName;
		}
		return NULL;
	}
	return fileName;
}

QString QThesisMainWindow::createSumoConfigFile(QStringList files, int endTime, QString tlsType)
{
	QString networkFile = mSimulationIntersectionPath + "int" + QString::number(mIntersectionNo) + "-" + tlsType + ".net.xml";
	QString fileName = mTestRunPath + "int" + QString::number(mIntersectionNo) + "_" + tlsType + ".sumocfg";
	QFile configFile(fileName);
	if ( configFile.open(QIODevice::WriteOnly) )
	{
		QTextStream stream( &configFile );
		stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl << endl;
		stream << "<configuration xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"http://sumo.dlr.de/xsd/sumoConfiguration.xsd\">" << endl;
		stream << "<input>" << endl;
		stream << "<net-file value=\"" + networkFile + "\" />" << endl;
		stream << "<route-files value=\""+ files.at(0) +"\"/>" << endl;
		if (files.at(1) != NULL)
			stream << "<additional-files value=\"" + files.at(1) + "\" />" << endl;
		stream << "</input>" << endl;
		//stream << "<output>" << endl;
		//stream << "<queue-output value=\"queues.out.xml\" />" << endl;
		//stream << "</output>" << endl;
		stream << "<time>" << endl;
		stream << "<begin value=\"0\" />" << endl;
		stream << "<end value=\""+ QString:: number(endTime) + "\" />" << endl;
		stream << "</time>" << endl;
		stream << "<processing>" << endl;
		stream << "<time-to-teleport value=\"-1\" />" << endl;
		stream << "</processing>" << endl;
		stream << "</configuration>" << endl;
		configFile.close();
	return fileName;
	}
	return NULL;
}

void QThesisMainWindow::simulateOneTest(QString sumoConfigFile, QString tlsType)
{
	QString sumoProgram = "sumo";
	QStringList arguments;
	arguments << "-c" << sumoConfigFile << "--output-prefix=" + QTime::currentTime().toString("HHmmss") + "_" + tlsType + "_";
	QProcess *sumoProcess = new QProcess(this);
	sumoProcess->setWorkingDirectory(mTestRunPath);
	sumoProcess->execute(sumoProgram, arguments);
}

void QThesisMainWindow::simulateOneAdaptiveTlsTest(QString sumoConfigFile, int endTime)
{
	//call python script with given parameters.
	QString python = "python";
	QString programPath = mSumoToolsPath + "adaptiveTLC_v1.py";

	QStringList arguments;
	arguments << programPath << "-c" << sumoConfigFile << "-i" << QString::number(mIntersectionNo) << "-e" << QString::number(endTime);
	QProcess *sumoProcess = new QProcess(this);
	sumoProcess->setWorkingDirectory(mTestRunPath);
	int returnId = sumoProcess->execute(python, arguments);
	if (returnId == -2) 
		qDebug() << sumoProcess->errorString();
}

void QThesisMainWindow::transformOutputFiles()
{
	transformOutputFilesIntoCsv();
	trimEdgeOutputFiles();
	trimLoopOutputFiles();
	//trimQueueOutputFiles();
	trimTlsStatesOutputFiles();
}

void QThesisMainWindow::transformOutputFilesIntoCsv()
{
	QStringList nameFilter("*.out.xml");
	QDir directory(mTestRunPath);
	QStringList outputFiles = directory.entryList(nameFilter);
	QString python = "python";
	QString programPath = mSumoToolsPath + "xml2csv.py";
	for (int i = 0; i < outputFiles.count(); i++)
	{
		QStringList arguments;
		arguments << programPath << mTestRunPath + outputFiles.at(i);
		QProcess *sumoProcess = new QProcess(this);
		sumoProcess->setWorkingDirectory(mTestRunPath);
		sumoProcess->execute(python, arguments);
	}
}

void QThesisMainWindow::trimQueueOutputFiles()
{
	QString fileStartData = "intersection_no;simulation_datetime;simulation_type;simulation_days;simulation_rain;analysis_type;";
	QString addInfoToLine = QString::number(mIntersectionNo) + ';';
	QString simulationDesc = mSpecialDays + ";" + mWeatherDays + ";variableTC;";
	QStringList nameFilter("*queues.out.csv");
	QRegExp noQueuesRegistered("^.*;{4}$");
	QRegExp fileInfoRexp("\\d*_(.*)_queues.out.csv");
	QDir directory(mTestRunPath);
	QStringList queueFiles = directory.entryList(nameFilter);
	//                       yyyy    MM       dd       hh     mm
	QRegExp dirInfoRexp("exp(\\d{4})(\\d{2})(\\d{2})(\\d{2})(\\d{2})");
	QString dirName = directory.dirName();
	if (dirInfoRexp.indexIn(dirName) != -1) {
		addInfoToLine += dirInfoRexp.cap(3) + "." + dirInfoRexp.cap(2) + "." + dirInfoRexp.cap(1) + " ";
		addInfoToLine += dirInfoRexp.cap(4) + ":" + dirInfoRexp.cap(5) + ":00;";
	}

	//output filename
	QFile outputFile(mTestRunPath + "queuedata_formatted.out.csv");
	if (outputFile.open(QIODevice::WriteOnly))
	{
		QTextStream out(&outputFile);
		int firstFile = true;
		for (int i = 0; i < queueFiles.count(); i++)
		{
			QString fileName = queueFiles.at(i);
			QString testInfoStart = addInfoToLine;
			if (fileInfoRexp.indexIn(fileName) != -1)
				testInfoStart += fileInfoRexp.cap(1) + ";";
			testInfoStart += simulationDesc;

			//trim file
			QFile inputFile(mTestRunPath + fileName);
			if ( inputFile.open(QIODevice::ReadOnly))
			{
				QTextStream in(&inputFile);
				QString line;
				int j = 0;
				while(!in.atEnd()) {
					line = in.readLine();
					if (noQueuesRegistered.indexIn(line) == -1) 
					{
						if (j != 0)
							out << testInfoStart << line.replace(".",",") << endl;
						if (j == 0 && firstFile) 
						{
							out << fileStartData << line << endl;
							j++;
							firstFile = false;
						}
						if(!firstFile && j == 0)
							j++;
					}
				}
				inputFile.flush();
				inputFile.close();
				inputFile.remove();
			}
			else QMessageBox::critical(this, "Error", inputFile.errorString());
		}
		outputFile.flush();
		outputFile.close();
	}
	else QMessageBox::critical(this, "Error", outputFile.errorString());
}

void QThesisMainWindow::trimEdgeOutputFiles()
{
	QString fileStartData = "intersection_no;simulation_datetime;simulation_type;simulation_days;simulation_rain;analysis_type;";
	QString addInfoToLine = QString::number(mIntersectionNo) + ';';
	QString simulationDesc = mSpecialDays + ";" + mWeatherDays + ";variableTC;";
	QStringList nameFilter("*edgedata.out.csv");
	QRegExp noQueuesRegistered("^([^;]*;[^;]*;)[^;]*;([^;]*;[^;]*;[^;]*;[^;]*;[^;]*;)[^;]*;[^;]*;([^;]*;[^;]*;)[^;]*;(.*)$");
	QRegExp fileInfoRexp("\\d*_(.*)_edgedata.out.csv");
	QRegExp fileBaseRexp("(.*).out.csv");
	QDir directory(mTestRunPath);
	QStringList queueFiles = directory.entryList(nameFilter);

	//                       yyyy    MM       dd       hh     mm
	QRegExp dirInfoRexp("exp(\\d{4})(\\d{2})(\\d{2})(\\d{2})(\\d{2})");
	QString dirName = directory.dirName();
	if (dirInfoRexp.indexIn(dirName) != -1) {
		addInfoToLine += dirInfoRexp.cap(3) + "." + dirInfoRexp.cap(2) + "." + dirInfoRexp.cap(1) + " ";
		addInfoToLine += dirInfoRexp.cap(4) + ":" + dirInfoRexp.cap(5) + ":00;";
	}

	//output filename
	QFile outputFile(mTestRunPath + "edgedata_formatted.out.csv");
	if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&outputFile);
		int firstFile = true;
		for (int i = 0; i < queueFiles.count(); i++)
		{
			QString fileName = queueFiles.at(i);
			QString testInfoStart = addInfoToLine;
			if (fileInfoRexp.indexIn(fileName) != -1)
				testInfoStart += fileInfoRexp.cap(1) + ";";
			testInfoStart += simulationDesc;

			//trim file
			QFile inputFile(mTestRunPath + fileName);
			if ( inputFile.open(QIODevice::ReadOnly))
			{
				QTextStream in(&inputFile);
				QString line;
				int j = 0;
				while(!in.atEnd()) {
					line = in.readLine();
					if (noQueuesRegistered.indexIn(line) != -1)
					{
						line = noQueuesRegistered.cap(1) + noQueuesRegistered.cap(2) + noQueuesRegistered.cap(3) + noQueuesRegistered.cap(4);
						if (j != 0)
							out << testInfoStart << line.replace(".",",") << endl;
						if (j == 0 && firstFile) 
						{
							out << fileStartData << line << endl;
							j++;
							firstFile = false;
						}
						if(!firstFile && j == 0)
							j++;
					}
				}
				inputFile.flush();
				inputFile.close();
				inputFile.remove();
			}
			else QMessageBox::critical(this, "Error", inputFile.errorString());
		}
		outputFile.flush();
		outputFile.close();
	}
	else QMessageBox::critical(this, "Error", outputFile.errorString());
}

void QThesisMainWindow::trimLoopOutputFiles()
{
	QString fileStartData = "intersection_no;simulation_datetime;simulation_type;simulation_days;simulation_rain;analysis_type;";
	QString addInfoToLine = QString::number(mIntersectionNo) + ';';
	QString simulationDesc = mSpecialDays + ";" + mWeatherDays + ";variableTC;";
	QStringList nameFilter("*loops.out.csv");
	QRegExp noQueuesRegistered("^([^;]*;[^;]*;)[^;]*;([^;]*;)[^;]*;[^;]*;(.*)$");
	QRegExp fileInfoRexp("\\d*_(.*)_loops.out.csv");
	QDir directory(mTestRunPath);
	QStringList queueFiles = directory.entryList(nameFilter);

	//                       yyyy    MM       dd       hh     mm
	QRegExp dirInfoRexp("exp(\\d{4})(\\d{2})(\\d{2})(\\d{2})(\\d{2})");
	QString dirName = directory.dirName();
	if (dirInfoRexp.indexIn(dirName) != -1) {
		addInfoToLine += dirInfoRexp.cap(3) + "." + dirInfoRexp.cap(2) + "." + dirInfoRexp.cap(1) + " ";
		addInfoToLine += dirInfoRexp.cap(4) + ":" + dirInfoRexp.cap(5) + ":00;";
	}

	//output filename
	QFile outputFile(mTestRunPath + "loopdata_formatted.out.csv");
	if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&outputFile);
		int firstFile = true;
		for (int i = 0; i < queueFiles.count(); i++)
		{
			QString fileName = queueFiles.at(i);
			QString testInfoStart = addInfoToLine;
			if (fileInfoRexp.indexIn(fileName) != -1)
				testInfoStart += fileInfoRexp.cap(1) + ";";
			testInfoStart += simulationDesc;

			//trim file
			QFile inputFile(mTestRunPath + fileName);
			if ( inputFile.open(QIODevice::ReadOnly))
			{
				QTextStream in(&inputFile);
				QString line;
				int j = 0;
				while(!in.atEnd()) {
					line = in.readLine();
					if (noQueuesRegistered.indexIn(line) != -1) 
					{
						line = noQueuesRegistered.cap(1) + noQueuesRegistered.cap(2) + noQueuesRegistered.cap(3);
						if (j != 0)
							out << testInfoStart << line.replace(".",",") << endl;
						if (j == 0 && firstFile) 
						{
							out << fileStartData << line << endl;
							j++;
							firstFile = false;
						}
						if(!firstFile && j == 0)
							j++;
					}
				}
				inputFile.flush();
				inputFile.close();
				inputFile.remove();
			}
			else QMessageBox::critical(this, "Error", inputFile.errorString());
		}
		outputFile.flush();
		outputFile.close();
	}
	else QMessageBox::critical(this, "Error", outputFile.errorString());
}

void QThesisMainWindow::trimTlsStatesOutputFiles()
{
	QString fileStartData = "intersection_no;simulation_datetime;simulation_type;simulation_days;simulation_rain;analysis_type;";
	QString addInfoToLine = QString::number(mIntersectionNo) + ';';
	QString simulationDesc = mSpecialDays + ";" + mWeatherDays + ";variableTC;";
	QStringList nameFilter("*tlsdata.out.csv");
	QRegExp noQueuesRegistered("^([^;]*;[^;]*;)[^;]*;(.*)$");
	QRegExp fileInfoRexp("\\d*_(.*)_tlsdata.out.csv");
	QRegExp fileBaseRexp("(.*).out.csv");
	QDir directory(mTestRunPath);
	QStringList queueFiles = directory.entryList(nameFilter);

	// date from testdir      yyyy    MM       dd       hh     mm
	QRegExp dirInfoRexp("exp(\\d{4})(\\d{2})(\\d{2})(\\d{2})(\\d{2})");
	QString dirName = directory.dirName();
	if (dirInfoRexp.indexIn(dirName) != -1) {
		addInfoToLine += dirInfoRexp.cap(3) + "." + dirInfoRexp.cap(2) + "." + dirInfoRexp.cap(1) + " ";
		addInfoToLine += dirInfoRexp.cap(4) + ":" + dirInfoRexp.cap(5) + ":00;";
	}

	//output filename
	QFile outputFile(mTestRunPath + "tlsdata_formatted.out.csv");
	if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&outputFile);
		int firstFile = true;
		for (int i = 0; i < queueFiles.count(); i++)
		{
			QString fileName = queueFiles.at(i);
			QString testInfoStart = addInfoToLine;
			if (fileInfoRexp.indexIn(fileName) != -1)
				testInfoStart += fileInfoRexp.cap(1) + ";";
			testInfoStart += simulationDesc;

			//trim file
			QFile inputFile(mTestRunPath + fileName);
			if ( inputFile.open(QIODevice::ReadOnly))
			{
				QTextStream in(&inputFile);
				QString line;
				int j = 0;
				int startTime = -1;
				QString tlsState;
				QString stateDesc;
				QString tlsId;
				while(!in.atEnd()) 
				{
					line = in.readLine();
					if (noQueuesRegistered.indexIn(line) != -1)
					{
						line = noQueuesRegistered.cap(1) + noQueuesRegistered.cap(2);
						if (j != 0) 
						{
							QRegExp normalLinesRexp("^[^;]*;([^;]*;)([^;]*;)(.*)$");
							//the input document describes the times of states being switched at the traffic lights
							//more interesting would be the length of the individual states
							//read first line with data and set the starting point
							if (startTime == -1) 
							{
								QRegExp firstLineRexp("^([^;]*;)([^;]*;)([^;]*;)(.*)$");
								if (firstLineRexp.indexIn(line) != -1) 
								{
									tlsId = firstLineRexp.cap(1); //only needs to be read once as it is right now
									tlsState = firstLineRexp.cap(2);
									stateDesc = firstLineRexp.cap(3);
									startTime = (int) firstLineRexp.cap(4).toDouble();
								}

								//read next line and assign it to the line variable to calculate the duration below
								QString line2  = in.readLine();
								if (noQueuesRegistered.indexIn(line2) != -1)
									line2 = noQueuesRegistered.cap(1) + noQueuesRegistered.cap(2);
								line = line2;
							}

							//use state from last line and the switching time from this line to get duration
							if (normalLinesRexp.indexIn(line) != -1)
							{
								int time = (int)normalLinesRexp.cap(3).toDouble();
								int duration = time - startTime;
								line = tlsId + tlsState + stateDesc + QString::number(duration);
								out << testInfoStart << line << endl;

								//set state info new
								startTime += duration;
								tlsState = normalLinesRexp.cap(1);
								stateDesc = normalLinesRexp.cap(2);
							}
						}
						//first line overall
						if (j == 0 && firstFile) 
						{
							out << fileStartData << line << endl;
							j++;
							firstFile = false;
						}
						//first line in following files - ignore the line
						if(!firstFile && j == 0)
							j++;
					}
				}
				inputFile.flush();
				inputFile.close();
				inputFile.remove();
			}
			else QMessageBox::critical(this, "Error", inputFile.errorString());
		}
		outputFile.flush();
		outputFile.close();
	}
	else QMessageBox::critical(this, "Error", outputFile.errorString());
}

QString QThesisMainWindow::boolToString(bool value)
{
	return value ? "true" : "false";
}

