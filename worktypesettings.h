#ifndef WORKTYPESETTINGS_H
#define WORKTYPESETTINGS_H

#include <QDialog>
#include <QPushButton>
#include <QStandardItemModel>
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
    QStandardItemModel *model;

private slots:
    void customConextMenuRequested(QPoint pos);
    void on_buttonBox_clicked(QAbstractButton *button);
};

#endif // WORKTYPESETTINGS_H
