/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *month_tab;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *monthVerticalLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *clearPushButton;
    QPushButton *savePushButton;
    QWidget *daily_tab;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *todayVerticalLayout;
    QTableView *dailyTableView;
    QTableView *dailyTableView2;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QDateEdit *dateEdit;
    QPushButton *deployPushButton;
    QPushButton *refreshPushButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(30, 30, 581, 481));
        month_tab = new QWidget();
        month_tab->setObjectName(QString::fromUtf8("month_tab"));
        verticalLayoutWidget = new QWidget(month_tab);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 170, 136));
        monthVerticalLayout = new QVBoxLayout(verticalLayoutWidget);
        monthVerticalLayout->setObjectName(QString::fromUtf8("monthVerticalLayout"));
        monthVerticalLayout->setContentsMargins(0, 0, 0, 0);
        tableView = new QTableView(verticalLayoutWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy);

        monthVerticalLayout->addWidget(tableView);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        clearPushButton = new QPushButton(verticalLayoutWidget);
        clearPushButton->setObjectName(QString::fromUtf8("clearPushButton"));

        horizontalLayout_2->addWidget(clearPushButton);

        savePushButton = new QPushButton(verticalLayoutWidget);
        savePushButton->setObjectName(QString::fromUtf8("savePushButton"));

        horizontalLayout_2->addWidget(savePushButton);


        monthVerticalLayout->addLayout(horizontalLayout_2);

        tabWidget->addTab(month_tab, QString());
        daily_tab = new QWidget();
        daily_tab->setObjectName(QString::fromUtf8("daily_tab"));
        verticalLayoutWidget_2 = new QWidget(daily_tab);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 10, 551, 302));
        todayVerticalLayout = new QVBoxLayout(verticalLayoutWidget_2);
        todayVerticalLayout->setObjectName(QString::fromUtf8("todayVerticalLayout"));
        todayVerticalLayout->setContentsMargins(0, 0, 0, 0);
        dailyTableView = new QTableView(verticalLayoutWidget_2);
        dailyTableView->setObjectName(QString::fromUtf8("dailyTableView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(dailyTableView->sizePolicy().hasHeightForWidth());
        dailyTableView->setSizePolicy(sizePolicy1);

        todayVerticalLayout->addWidget(dailyTableView);

        dailyTableView2 = new QTableView(verticalLayoutWidget_2);
        dailyTableView2->setObjectName(QString::fromUtf8("dailyTableView2"));
        QFont font;
        font.setPointSize(16);
        font.setBold(false);
        font.setItalic(false);
        dailyTableView2->setFont(font);
        dailyTableView2->setStyleSheet(QString::fromUtf8("font: 25 16pt ;"));

        todayVerticalLayout->addWidget(dailyTableView2);

        tabWidget->addTab(daily_tab, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 19));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        dateEdit = new QDateEdit(dockWidgetContents);
        dateEdit->setObjectName(QString::fromUtf8("dateEdit"));
        dateEdit->setCurrentSection(QDateTimeEdit::DaySection);

        verticalLayout->addWidget(dateEdit);

        deployPushButton = new QPushButton(dockWidgetContents);
        deployPushButton->setObjectName(QString::fromUtf8("deployPushButton"));

        verticalLayout->addWidget(deployPushButton);

        refreshPushButton = new QPushButton(dockWidgetContents);
        refreshPushButton->setObjectName(QString::fromUtf8("refreshPushButton"));

        verticalLayout->addWidget(refreshPushButton);


        verticalLayout_2->addLayout(verticalLayout);

        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "WORK DEPLOY -- R&D and Equipment Division", nullptr));
        clearPushButton->setText(QCoreApplication::translate("MainWindow", "&Clear", nullptr));
        savePushButton->setText(QCoreApplication::translate("MainWindow", "&Save", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(month_tab), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(daily_tab), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        deployPushButton->setText(QCoreApplication::translate("MainWindow", "&Deploy", nullptr));
        refreshPushButton->setText(QCoreApplication::translate("MainWindow", "&Refresh", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
