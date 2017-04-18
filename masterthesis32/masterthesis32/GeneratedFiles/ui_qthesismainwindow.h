/********************************************************************************
** Form generated from reading UI file 'qthesismainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTHESISMAINWINDOW_H
#define UI_QTHESISMAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QThesisMainWindowClass
{
public:
    QWidget *centralWidget;
    QWidget *widget;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QComboBox *combo_intersection;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout_2;
    QLabel *label_2;
    QTimeEdit *time_start;
    QLabel *label_3;
    QTimeEdit *time_end;
    QGroupBox *groupBox_3;
    QSpinBox *simulation_runs;
    QGroupBox *group_specialdays;
    QVBoxLayout *verticalLayout;
    QRadioButton *days_all;
    QRadioButton *days_weekdays;
    QRadioButton *days_weekends;
    QRadioButton *days_holidays;
    QGroupBox *trafficlights_group;
    QVBoxLayout *verticalLayout_3;
    QCheckBox *tls_fixed;
    QCheckBox *tls_actuated;
    QCheckBox *tls_adaptive;
    QGroupBox *group_weather;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *weather_all;
    QRadioButton *weather_rain;
    QRadioButton *weather_dry;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *label_4;
    QDateEdit *date_start;
    QLabel *label_5;
    QDateEdit *date_end;
    QSpacerItem *horizontalSpacer;
    QPushButton *start_button;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QThesisMainWindowClass)
    {
        if (QThesisMainWindowClass->objectName().isEmpty())
            QThesisMainWindowClass->setObjectName(QStringLiteral("QThesisMainWindowClass"));
        QThesisMainWindowClass->resize(486, 522);
        centralWidget = new QWidget(QThesisMainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 462, 443));
        gridLayout_3 = new QGridLayout(widget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(102, 0));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        combo_intersection = new QComboBox(widget);
        combo_intersection->setObjectName(QStringLiteral("combo_intersection"));
        combo_intersection->setMinimumSize(QSize(349, 35));

        gridLayout_2->addWidget(combo_intersection, 0, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 2);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox_2 = new QGroupBox(widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        formLayout_2 = new QFormLayout(groupBox_2);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_2);

        time_start = new QTimeEdit(groupBox_2);
        time_start->setObjectName(QStringLiteral("time_start"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, time_start);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_3);

        time_end = new QTimeEdit(groupBox_2);
        time_end->setObjectName(QStringLiteral("time_end"));
        time_end->setLocale(QLocale(QLocale::English, QLocale::Ireland));
        time_end->setMaximumTime(QTime(23, 45, 0));
        time_end->setCurrentSection(QDateTimeEdit::HourSection);
        time_end->setCalendarPopup(false);
        time_end->setTime(QTime(23, 45, 0));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, time_end);


        gridLayout->addWidget(groupBox_2, 0, 1, 1, 1);

        groupBox_3 = new QGroupBox(widget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        simulation_runs = new QSpinBox(groupBox_3);
        simulation_runs->setObjectName(QStringLiteral("simulation_runs"));
        simulation_runs->setGeometry(QRect(10, 30, 61, 31));
        simulation_runs->setMinimum(1);
        simulation_runs->setMaximum(100);

        gridLayout->addWidget(groupBox_3, 2, 1, 1, 1);

        group_specialdays = new QGroupBox(widget);
        group_specialdays->setObjectName(QStringLiteral("group_specialdays"));
        verticalLayout = new QVBoxLayout(group_specialdays);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        days_all = new QRadioButton(group_specialdays);
        days_all->setObjectName(QStringLiteral("days_all"));
        days_all->setChecked(true);

        verticalLayout->addWidget(days_all);

        days_weekdays = new QRadioButton(group_specialdays);
        days_weekdays->setObjectName(QStringLiteral("days_weekdays"));

        verticalLayout->addWidget(days_weekdays);

        days_weekends = new QRadioButton(group_specialdays);
        days_weekends->setObjectName(QStringLiteral("days_weekends"));

        verticalLayout->addWidget(days_weekends);

        days_holidays = new QRadioButton(group_specialdays);
        days_holidays->setObjectName(QStringLiteral("days_holidays"));

        verticalLayout->addWidget(days_holidays);


        gridLayout->addWidget(group_specialdays, 1, 0, 1, 1);

        trafficlights_group = new QGroupBox(widget);
        trafficlights_group->setObjectName(QStringLiteral("trafficlights_group"));
        verticalLayout_3 = new QVBoxLayout(trafficlights_group);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        tls_fixed = new QCheckBox(trafficlights_group);
        tls_fixed->setObjectName(QStringLiteral("tls_fixed"));

        verticalLayout_3->addWidget(tls_fixed);

        tls_actuated = new QCheckBox(trafficlights_group);
        tls_actuated->setObjectName(QStringLiteral("tls_actuated"));

        verticalLayout_3->addWidget(tls_actuated);

        tls_adaptive = new QCheckBox(trafficlights_group);
        tls_adaptive->setObjectName(QStringLiteral("tls_adaptive"));

        verticalLayout_3->addWidget(tls_adaptive);


        gridLayout->addWidget(trafficlights_group, 2, 0, 1, 1);

        group_weather = new QGroupBox(widget);
        group_weather->setObjectName(QStringLiteral("group_weather"));
        verticalLayout_2 = new QVBoxLayout(group_weather);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        weather_all = new QRadioButton(group_weather);
        weather_all->setObjectName(QStringLiteral("weather_all"));
        weather_all->setChecked(true);

        verticalLayout_2->addWidget(weather_all);

        weather_rain = new QRadioButton(group_weather);
        weather_rain->setObjectName(QStringLiteral("weather_rain"));

        verticalLayout_2->addWidget(weather_rain);

        weather_dry = new QRadioButton(group_weather);
        weather_dry->setObjectName(QStringLiteral("weather_dry"));

        verticalLayout_2->addWidget(weather_dry);


        gridLayout->addWidget(group_weather, 1, 1, 1, 1);

        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        formLayout = new QFormLayout(groupBox);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_4);

        date_start = new QDateEdit(groupBox);
        date_start->setObjectName(QStringLiteral("date_start"));

        formLayout->setWidget(0, QFormLayout::FieldRole, date_start);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_5);

        date_end = new QDateEdit(groupBox);
        date_end->setObjectName(QStringLiteral("date_end"));

        formLayout->setWidget(1, QFormLayout::FieldRole, date_end);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout, 1, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(348, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 2, 0, 1, 1);

        start_button = new QPushButton(widget);
        start_button->setObjectName(QStringLiteral("start_button"));

        gridLayout_3->addWidget(start_button, 2, 1, 1, 1);

        QThesisMainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QThesisMainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 486, 26));
        QThesisMainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QThesisMainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QThesisMainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QThesisMainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QThesisMainWindowClass->setStatusBar(statusBar);

        retranslateUi(QThesisMainWindowClass);

        QMetaObject::connectSlotsByName(QThesisMainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *QThesisMainWindowClass)
    {
        QThesisMainWindowClass->setWindowTitle(QApplication::translate("QThesisMainWindowClass", "Dublin Traffic Light Simulator", 0));
        label->setText(QApplication::translate("QThesisMainWindowClass", "Intersection: ", 0));
        groupBox_2->setTitle(QApplication::translate("QThesisMainWindowClass", "Time Span", 0));
        label_2->setText(QApplication::translate("QThesisMainWindowClass", "Start Time:", 0));
        label_3->setText(QApplication::translate("QThesisMainWindowClass", "End Time: ", 0));
        groupBox_3->setTitle(QApplication::translate("QThesisMainWindowClass", "Number of Test Runs", 0));
        group_specialdays->setTitle(QApplication::translate("QThesisMainWindowClass", "Special Days", 0));
        days_all->setText(QApplication::translate("QThesisMainWindowClass", "All", 0));
        days_weekdays->setText(QApplication::translate("QThesisMainWindowClass", "Weekdays", 0));
        days_weekends->setText(QApplication::translate("QThesisMainWindowClass", "Weekends", 0));
        days_holidays->setText(QApplication::translate("QThesisMainWindowClass", "Holidays", 0));
        trafficlights_group->setTitle(QApplication::translate("QThesisMainWindowClass", "Traffic Light Control", 0));
        tls_fixed->setText(QApplication::translate("QThesisMainWindowClass", "Fixed-time", 0));
        tls_actuated->setText(QApplication::translate("QThesisMainWindowClass", "Actuated", 0));
        tls_adaptive->setText(QApplication::translate("QThesisMainWindowClass", "Adaptive", 0));
        group_weather->setTitle(QApplication::translate("QThesisMainWindowClass", "Weather Conditions", 0));
        weather_all->setText(QApplication::translate("QThesisMainWindowClass", "All", 0));
        weather_rain->setText(QApplication::translate("QThesisMainWindowClass", "Rain", 0));
        weather_dry->setText(QApplication::translate("QThesisMainWindowClass", "No Rain", 0));
        groupBox->setTitle(QApplication::translate("QThesisMainWindowClass", "Date Span", 0));
        label_4->setText(QApplication::translate("QThesisMainWindowClass", "Start Date: ", 0));
        label_5->setText(QApplication::translate("QThesisMainWindowClass", "End Date: ", 0));
        start_button->setText(QApplication::translate("QThesisMainWindowClass", "Start", 0));
    } // retranslateUi

};

namespace Ui {
    class QThesisMainWindowClass: public Ui_QThesisMainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTHESISMAINWINDOW_H
