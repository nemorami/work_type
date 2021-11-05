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

private:
    Ui::MainWindow *ui;
    WorkTypeSettings *worktype;
     QSqlDatabase db;

private slots:
    void on_pushButton_clicked();

#endif // MAINWINDOW_H
};
