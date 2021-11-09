#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QCheckBox>
#include <QtSql/QSqlDatabase>
#include "worktypesettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void readDataFile();

    QString getNextWorktype(QString name, QString init_work);
    QPair<QString, QDate> getLastWorktype(QString name);
    void setTodayReport();

private:
    Ui::MainWindow *ui;
    WorkTypeSettings *worktype;
    QSqlDatabase db;
    QSqlQueryModel month_model;

private slots:
   // void on_pushButton_clicked();
    void on_deployPushButton_clicked();
};

struct WorkDeploy {
    QString name;
    QString part;
    QString work;
};

#endif // MAINWINDOW_H

