/********************************************************************************
** Form generated from reading UI file 'worktypesettings.ui'
**
** Created by: Qt User Interface Compiler version 6.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORKTYPESETTINGS_H
#define UI_WORKTYPESETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WorkTypeSettings
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTableView *tableView;
    QHBoxLayout *horizontalLayout;
    QPushButton *addPushButton;
    QPushButton *savePushButton;

    void setupUi(QDialog *WorkTypeSettings)
    {
        if (WorkTypeSettings->objectName().isEmpty())
            WorkTypeSettings->setObjectName(QString::fromUtf8("WorkTypeSettings"));
        WorkTypeSettings->resize(400, 300);
        verticalLayoutWidget = new QWidget(WorkTypeSettings);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(20, 10, 311, 221));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tableView = new QTableView(verticalLayoutWidget);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        verticalLayout->addWidget(tableView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        addPushButton = new QPushButton(verticalLayoutWidget);
        addPushButton->setObjectName(QString::fromUtf8("addPushButton"));

        horizontalLayout->addWidget(addPushButton);

        savePushButton = new QPushButton(verticalLayoutWidget);
        savePushButton->setObjectName(QString::fromUtf8("savePushButton"));

        horizontalLayout->addWidget(savePushButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(WorkTypeSettings);

        QMetaObject::connectSlotsByName(WorkTypeSettings);
    } // setupUi

    void retranslateUi(QDialog *WorkTypeSettings)
    {
        WorkTypeSettings->setWindowTitle(QCoreApplication::translate("WorkTypeSettings", "Dialog", nullptr));
        addPushButton->setText(QCoreApplication::translate("WorkTypeSettings", "&Add", nullptr));
        savePushButton->setText(QCoreApplication::translate("WorkTypeSettings", "&Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WorkTypeSettings: public Ui_WorkTypeSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORKTYPESETTINGS_H
