#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QCheckBox>
#include <QtSql/QSqlDatabase>
#include <QLabel>
#include "worktypesettings.h"
#include "nsqlquerymodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();

    QString getNextWorktype(QString part, QString init_work);
    QPair<QString, QDate> getLastWorktype(QString name);
    void readInitFile();
    void writeInitFile();


private:
    Ui::MainWindow *ui;
    WorkTypeSettings *worktype;
    QSqlDatabase db;
    NSqlQueryModel month_model;
    QSqlQueryModel daily_model;
    QSqlQueryModel daily_name_model;



private slots:
    void on_refreshPushButton_clicked();
    void on_deployPushButton_clicked();
    void on_savePushButton_clicked();
    void on_clearPushButton_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_lineEdit_editingFinished();
    void on_aboutPushButton_clicked();
//    void setTodayReport();
    void showMonth();
    void showDaily();
    void setModelDay();
    void clearMonthView();
    void copyToClipboard();
    void customMenuRequested(QPoint);
};

struct WorkDeploy {
    QString name;
    QString part;
    QString work;
};

#endif // MAINWINDOW_H

