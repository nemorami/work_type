#include "worktypesettings.h"
#include "ui_worktypesettings.h"
#include "worktypesettings.h"
#include "worktypesettings.h"
#include <QPushButton>
#include <QMenu>
#include <QDebug>
#include <QSqlError>

void WorkTypeSettings::loadData(QString tablename)
{

    model = new QSqlTableModel;
    model->setTable(tablename);
    model->select();

    //테이블뷰 헤더데이터

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("이름"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("구분"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("근무형태"));

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
    qDebug() << model->lastError();
}

WorkTypeSettings::WorkTypeSettings(QString tablename, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkTypeSettings)
{
    ui->setupUi(this);
    setLayout(ui->verticalLayout);



    // ui->buttonBox->button(QDialogButtonBox::Apply)->setText("&Add");
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &WorkTypeSettings::customMenuRequested);

    loadData(tablename);


}

WorkTypeSettings::~WorkTypeSettings()
{
    delete ui;
}

void WorkTypeSettings::customMenuRequested(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QAction *addAction = new QAction("Add", this);
    connect(addAction, &QAction::triggered, this, [this]() {model->insertRow(ui->tableView->model()->rowCount());});
    menu->addAction(addAction);
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void WorkTypeSettings::on_addPushButton_clicked(bool checked)
{
    model->insertRow(ui->tableView->model()->rowCount());
}

void WorkTypeSettings::on_savePushButton_clicked(bool checked)
{
    model->submit();
}


//void WorkTypeSettings::on_buttonBox_clicked(QAbstractButton *button)
//{
//    switch (ui->buttonBox->standardButton(button)) {
//    case QDialogButtonBox::Apply:
//        //model.insertRow(ui->tableView->model()->rowCount());
//        model.select();
//        break;

//    default:
//        break;

//    }
//}
