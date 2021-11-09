#ifndef WORKTYPESETTINGS_H
#define WORKTYPESETTINGS_H

#include <QDialog>
#include <QPushButton>
#include <QtSql/QSqlTableModel>

namespace Ui {
class WorkTypeSettings;
}

class WorkTypeSettings : public QDialog
{
    Q_OBJECT

public:
    explicit WorkTypeSettings(QWidget *parent = nullptr);
    ~WorkTypeSettings();

private:
    Ui::WorkTypeSettings *ui;
    QSqlTableModel model;

private slots:
    void customMenuRequested(QPoint pos);
    void on_addPushButton_clicked(bool checked=false);
    void on_savePushButton_clicked(bool checked=false);
};

#endif // WORKTYPESETTINGS_H
