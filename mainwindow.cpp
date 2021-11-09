#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QtCore/QStandardPaths>
#include <QDir>
#include <mainwindow.h>
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
    // 설정 탭
    worktype = new WorkTypeSettings();
    ui->tabWidget->insertTab(0, worktype, "설정");
    ui->month_tab->setLayout(ui->monthVerticalLayout);
    ui->daily_tab->setLayout(ui->todayVerticalLayout);
    // 한달 근무 배치표 탭 만들기
    QString query = QString("select part '계', name '이름',\
                            max(case when day = '01' then work end) '01',\
                            max(case when day = '02' then work end) '02',\
                            max(case when day = '03' then work end) '03',\
                            max(case when day = '04' then work end) '04',\
                            max(case when day = '05' then work end) '05',\
                            max(case when day = '06' then work end) '06',\
                            max(case when day = '07' then work end) '07',\
                            max(case when day = '08' then work end) '08',\
                            max(case when day = '09' then work end) '09',\
                            max(case when day = '10' then work end) '10',\
                            max(case when day = '11' then work end) '11',\
                            max(case when day = '12' then work end) '12',\
                            max(case when day = '13' then work end) '13',\
                            max(case when day = '14' then work end) '14',\
                            max(case when day = '15' then work end) '15',\
                            max(case when day = '16' then work end) '16',\
                            max(case when day = '17' then work end) '17',\
                            max(case when day = '18' then work end) '18',\
                            max(case when day = '19' then work end) '19',\
                            max(case when day = '20' then work end) '20',\
                            max(case when day = '21' then work end) '21',\
                            max(case when day = '22' then work end) '22',\
                            max(case when day = '23' then work end) '23',\
                            max(case when day = '24' then work end) '24',\
                            max(case when day = '25' then work end) '25',\
                            max(case when day = '26' then work end) '26',\
                            max(case when day = '27' then work end) '27',\
                            max(case when day = '28' then work end) '28',\
                            max(case when day = '29' then work end) '29',\
                            max(case when day = '30' then work end) '30',\
                            max(case when day = '31' then work end) '31'\
                     from (select substr(day, 9, 2) as day, part, name, work from daily join work_type using(name) where substr(day, 6, 2) = '%1')\
                     group by 1").arg(ui->dateEdit->date().month());
    month_model.setQuery(query);


    ui->tableView->setModel(&month_model);




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
    //qDebug() << data_dir;
    db.open();

    QSqlQuery query;
    //sqlite_master
    query.prepare("select count(*) from sqlite_master where name = 'work_type'");
    query.exec();

    if(query.next()) {
        if(query.value(0).toInt() != 0){
           // query.prepare("select name, part, work_patterns from work_type");
           // query.exec();
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

// 업무 배치...
void MainWindow::on_deployPushButton_clicked()
{
    WorkDeploy work_deploy;
    QVector<WorkDeploy> work_deploys;

    // work_type 테이블을 work_deploys에 로딩한다.
    QSqlQuery query;
    query.prepare("select name, part, work_patterns from work_type");
    query.exec();

    while(query.next()) {
        work_deploy.name = query.value(0).toString();
        work_deploy.part = query.value(1).toString();
        work_deploy.work = query.value(2).toString();
        work_deploys.append(work_deploy);
    }

    QDate date;

    for (auto &deploy : work_deploys ) {
        auto work_type = getLastWorktype(deploy.name);
        auto w = work_type.first;
        date = work_type.second;
        QDate today = ui->dateEdit->date();

        if(date.year() == 1970) {
            date = QDate(today.year(), today.month(), 1);

            query.prepare("insert into daily values(:day, :name, :work)");
            query.bindValue(":day", date);
            query.bindValue(":name", deploy.name);
            query.bindValue(":work", w);
            query.exec();

        }

        for(date= date.addDays(1); date <= QDate(today.year(),today.month(), today.daysInMonth());date = date.addDays(1)) {
            w = getNextWorktype(deploy.name, w);
            query.prepare("insert into daily values(:day, :name, :work)");
            query.bindValue(":day", date);
            query.bindValue(":name", deploy.name);
            query.bindValue(":work", w);
            query.exec();

        }



    //        ";

       // qDebug() <<"get next work =>" << getNextWorktype(deploy.name, work_type.first);
    }

}


QString MainWindow::getNextWorktype(QString name, QString init_work)
{
    QSqlQuery query;

    query.prepare("select work_patterns from work_type where name = :name");
    query.bindValue(":name", name);
    query.exec();

    query.next();

    QString work_list = query.value(0).toString();
    // init_work를 work와 index로 분리 ex) 주2: work => 주, index => 2
    QChar work = init_work[0];
    int index = init_work.remove(0,1).toInt();
    // 다음 근무일 위치를 구한다.
    int search = work_list.indexOf(work);
    search = search + index;
    if(search >= work_list.size())
        search = 0;
    // 다음일 근무도 형태가 갈으면 인덱스를 증가, 아니면 ...
    //qDebug() << "work=> " << work << "work_list[search]=> " << work_list[search] << "search=> " << search << "\n";
    if (work == work_list[search])
        index++;
    else
        index = 1;
    qDebug() << "work list=> " << work_list << "search => " << search << "work => " << work << "\n";
    return QString("%1%2").arg(work_list[search]).arg(index);

}

QPair<QString, QDate> MainWindow::getLastWorktype(QString name)
{
    QSqlQuery query;
    QString work = nullptr;
    QDate date;
    query.prepare("select day, work from daily where name = :name order by desc");
    query.bindValue(":name", name);

    while(query.next()){
        date = query.value(0).toDate();
        work = query.value(1).toString();
        if(QString("주전비보").contains(work[0]))
                break;
    }
    if(work == nullptr) {
        query.prepare("select work_patterns from work_type where name = :name");
        query.bindValue(":name", name);
        query.exec();
        query.next();

        work = QString("%1%2").arg(query.value(0).toString()[0]).arg(1);
        date = QDate(1970,1,1);//ui->dateEdit->date().addDays(-ui->dateEdit->date().day());

    }

    return QPair<QString, QDate>(work, date);
}

void MainWindow::setTodayReport()
{
    QSqlQuery query;

    QString qstr = "select count(*) 현원,\
            sum(case when substr(work, 1,1) in('전', '주', '보') then work end) 근무,\
            sum(case when substr(work, 1,1) = '비' then work end) 비번,\
            sum(case when substr(work, 1,1) = '연' then work end) 연가,\
     from daily where day = '2021-11-01'";

}

MainWindow::~MainWindow()
{
    delete ui;
}


