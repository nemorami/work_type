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
    void readDataFile();

    QString getNextWorktype(QString name, QString init_work);
    QPair<QString, QDate> getLastWorktype(QString name);


private:
    Ui::MainWindow *ui;
    WorkTypeSettings *worktype;
    QSqlDatabase db;
    NSqlQueryModel month_model;

    QVector<QLabel *> lbnPart;
    QVector<QLabel *> lbnNormal;
    QVector<QLabel *> lbnHome;
    QVector<QLabel *> lbnAll;
    QVector<QLabel *> lbnOff;

private slots:
    void on_refreshPushButton_clicked();
    void on_deployPushButton_clicked();
    void on_savePushButton_clicked();
    void on_tabWidget_clicked(int index);
    void setTodayReport();
    void showMonth();
    void setModelDay();
};

struct WorkDeploy {
    QString name;
    QString part;
    QString work;
};

#endif // MAINWINDOW_H

