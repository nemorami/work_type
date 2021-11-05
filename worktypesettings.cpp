#include "worktypesettings.h"
#include "ui_worktypesettings.h"
#include "worktypesettings.h"
#include "worktypesettings.h"
#include <QPushButton>
#include <QStandardItemModel>
#include <QMenu>


WorkTypeSettings::WorkTypeSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WorkTypeSettings)
{
    ui->setupUi(this);
    setLayout(ui->verticalLayout);

    // ui->buttonBox->button(QDialogButtonBox::Apply)->setText("&Add");
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &WorkTypeSettings::customContextMenuRequested);
    model = new QStandardItemModel(0,3);


    //테이블뷰 헤더데이터
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("이름"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("구분"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("근무형태"));

    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

WorkTypeSettings::~WorkTypeSettings()
{
    delete ui;
}

void WorkTypeSettings::customConextMenuRequested(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    menu->addAction(new QAction("Add", this));
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void WorkTypeSettings::on_buttonBox_clicked(QAbstractButton *button)
{
    switch (ui->buttonBox->standardButton(button)) {
    case QDialogButtonBox::Apply:
        model->insertRow(ui->tableView->model()->rowCount());
        break;

    default:
        break;

    }
}
