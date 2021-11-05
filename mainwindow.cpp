#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   // ui->webView->setUrl(QUrl("http://10.188.214.126"));
    setCentralWidget(ui->tabWidget);
    //Q
   // ui->dockWidgetContents->setLayout(ui->verticalLayout);
    //connect(ui->pushButton, &QPushButton::clicked, this, );

    worktype = new WorkTypeSettings();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    worktype->show();
}


