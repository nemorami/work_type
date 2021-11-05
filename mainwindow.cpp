#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QtCore/QStandardPaths>
#include <QDir>
#include <QtSql/QSqlQuery>

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



    // QDate 초기값을 오늘 날짜로 설정
    ui->dateEdit->setDate(QDate::currentDate());
    readDataFile();

    worktype = new WorkTypeSettings();
}


void MainWindow::readDataFile()
{
    //database connection
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString data_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // 데이터 디렉토리가 없으면  생성
    if (!QDir(data_dir).exists())
        QDir().mkdir(data_dir);

    db.setDatabaseName(data_dir + "/work_db");
    qDebug() << data_dir;
    db.open();

    QSqlQuery query;
    //sqlite_master
    query.prepare("select count(*) from sqlite_master where name = 'work_type'");
    query.exec();

    if(query.next()) {
        if(query.value(0).toInt() != 0){
            query.prepare("select name, part, work_patterns from work_type");
            query.exec();
            while(query.next()){
                qDebug() << "list\n";
                qDebug() << query.value(0) << query.value(1) << query.value(2);
            }
        }else {
            // dictionary 테이블이 없으면
            query.prepare("create table work_type(name text, part text, work_patterns text)");
            query.exec();
            //테이블을 생성 day 날짜, 이름 이름, work 근무형태를 나타내는 한글자와 숫자로 구성 전일 비번 비번 주간 일때 전1 비1 비2 주1이 입력됨
            query.prepare("create table daily(day text, name text, work text)");
            query.exec();
        }

    } else {


    }
    //db.commit();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    worktype->show();
}


