#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QtCore/QStandardPaths>
#include <QDir>
#include <mainwindow.h>
#include <QtSql/QSqlQuery>
#include <QSqlError>

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
    readDataFile();
    ui->dateEdit->setDate(QDate::currentDate());
    //dateEdit의 날짜를 바꾸면
//    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &MainWindow::setTodayReport);
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &MainWindow::showDaily);
    // QDate 초기값을 오늘 날짜로 설정

    // ᅟshowDaily에서 호출
    //connect(&month_model, &NSqlQueryModel::dataChanged, this, &MainWindow::setModelDay);
    // 설정 탭
    // on_tabWidget_currentChanged로 대체..
    //connect(&ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);
    worktype = new WorkTypeSettings();
    ui->tabWidget->insertTab(0, worktype, "설정");
    ui->month_tab->setLayout(ui->monthVerticalLayout);
    ui->daily_tab->setLayout(ui->todayVerticalLayout);



    //일일근무 탭을 보여준다.
    ui->tabWidget->setCurrentIndex(2);
    showDaily();

    //ui->dailyTableView->verticalHeader()->height();
    //setTodayReport();

    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &MainWindow::customMenuRequested);
    connect(&month_model, &NSqlQueryModel::dataChanged, this, &MainWindow::showMonth);




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
        //qDebug() << "return getLastWorktype=>" << date;
        QDate today = ui->dateEdit->date();

        if(date.year() == 1970) {
            date = QDate(today.year(), today.month(), 1);

            query.prepare("insert into daily values(:day, :name, :work)");
            query.bindValue(":day", date);
            query.bindValue(":name", deploy.name);
            query.bindValue(":work", w);
            query.exec();

        }
        //qDebug() << "date => " << date;
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

void MainWindow::on_savePushButton_clicked()
{

    //ui->tableView->co
    //qDebug() << "on save " << month_model.headerData(2, Qt::Horizontal);
    month_model.submit();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case 1:
        showMonth();
        break;
    default:
        break;
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
    //qDebug() << "work list=> " << work_list << "search => " << search << "work => " << work << "\n";
    return QString("%1%2").arg(work_list[search]).arg(index);

}

QPair<QString, QDate> MainWindow::getLastWorktype(QString name)
{
    QSqlQuery query;
    QString work = nullptr;
    QDate date;
    query.prepare("select day, work from daily where name = :name order by day desc");
    query.bindValue(":name", name);
    query.exec();

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

void MainWindow::on_refreshPushButton_clicked()
{
    showMonth();
    showDaily();
}

//void MainWindow::setTodayReport()
//{
//    QSqlQuery query;


//    query.prepare("select count(*) 현원,\
//            sum(case when substr(work, 1,1) in('전', '주', '보') then 1 end) 근무,\
//            sum(case when substr(work, 1,1) = '비' then 1 end) 비번,\
//            sum(case when substr(work, 1,1) = '전' then 1 end) 전일, \
//            sum(case when substr(work, 1,1) = '연' then 1 end) 연가\
//            from daily where day = :date");
//    query.bindValue(":date", ui->dateEdit->date().toString("yyyy-MM-dd"));
//    query.exec();

//    if(query.next()) {
//        ui->lbFix->setText(query.value("현원").toString());
//        ui->lbCurrent->setText(query.value("근무").toString());
//        ui->lbOff->setText(query.value("비번").toString());
//        ui->lbVacation->setText(query.value("연가").toString());
//        ui->lbAll->setText(query.value("전일").toString());

//    }

//    query.prepare("select part 구분, work, d.name 이름\
//           from (select name, substr(work, 1, 1) work from daily where day = :date) d \
//             join work_type wt on d.name = wt.name");
//    query.bindValue(":date", ui->dateEdit->date().toString("yyyy-MM-dd"));
//    query.exec();

//    QString part=nullptr;
//    QString normal_str = nullptr;
//    QString home_str = nullptr;
//    QString all_str = nullptr;
//    QString off_str = nullptr;

//    /**
//     *리포트 이름 출력 부분 초기화
//     */




//    for (auto &l :{ui->partVerticalLayout, ui->normalVerticalLayout, ui->homeVerticalLayout, ui->allVerticalLayout, ui->offVerticalLayout} ) {
//       if(l->count() <= 1)
//           break;
//        QLayoutItem *item;
//        while ((item = l->takeAt(1)) != nullptr) {
//            //Q_ASSERT(!item->layout());
//            delete item->widget();
//            delete item;
//        }

//    }


//    lbnPart.clear();
//    lbnNormal.clear();
//    lbnHome.clear();
//    lbnAll.clear();
//    lbnOff.clear();

//    while(query.next()) {

//        QString w = query.value("work").toString();
//        if(part != query.value("구분").toString()) {
//            qDebug() << "part:" << part << ":" << query.value("구분").toString() << " all => " << normal_str;

//            if(!part.isNull()){
//                lbnPart.last()->setText(part);
//                lbnNormal.last()->setText(normal_str);
//                lbnAll.last()->setText(all_str);
//                lbnHome.last()->setText(home_str);
//                lbnOff.last()->setText(off_str);
//            }

//            lbnPart.append(new QLabel());
//            lbnNormal.append(new QLabel());
//            lbnHome.append(new QLabel());
//            lbnAll.append(new QLabel());
//            lbnOff.append(new QLabel());

//            part = query.value("구분").toString();


//            ui->partVerticalLayout->addWidget(lbnPart.last());
//            ui->normalVerticalLayout->addWidget(lbnNormal.last());
//            ui->homeVerticalLayout->addWidget(lbnHome.last());
//            ui->allVerticalLayout->addWidget(lbnAll.last());
//            ui->offVerticalLayout->addWidget(lbnOff.last());

//            normal_str = all_str = home_str = off_str = nullptr;
//            if( w == "주")
//                normal_str = query.value("이름").toString().append("\n");
//            else if(w == "전")
//                all_str = query.value("이름").toString().append("\n");
//            else if(w == "비")
//                off_str = query.value("이름").toString().append("\n");

//        }
//        else {
//            if( w == "주")
//                normal_str += query.value("이름").toString().append("\n");
//            else if(w == "전")
//                all_str += query.value("이름").toString().append("\n");
//            else if(w == "비")
//                off_str += query.value("이름").toString().append("\n");

//        }
//    }

//    lbnPart.last()->setText(part);
//    lbnNormal.last()->setText(normal_str);
//    lbnAll.last()->setText(all_str);
//    lbnHome.last()->setText(home_str);
//    lbnOff.last()->setText(off_str);




//}

void MainWindow::showMonth()
{
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
                     group by 1, 2").arg(ui->dateEdit->date().month());
    month_model.setQuery(query);



    ui->tableView->setModel(&month_model);

}

void MainWindow::clearMonthView()
{

   for(auto index: ui->tableView->selectionModel()->selectedIndexes()){
       qDebug() << index;
   }
}

void MainWindow::customMenuRequested(QPoint pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);
    QMenu *menu = new QMenu(this);

    menu->addAction(tr("Clear"), [this](){this->clearMonthView();});

    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));

}

void MainWindow::showDaily()
{
    setModelDay();
    QString daily = QString("select count(*) 현원,\
            sum(case when substr(work, 1,1) in('전', '주', '보') then 1 end) 근무,\
            sum(case when substr(work, 1,1) = '비' then 1 end) 비번,\
            sum(case when substr(work, 1,1) = '전' then 1 end) 전일, \
            sum(case when substr(work, 1,1) = '휴' then 1 end) 휴가\
            from daily where day = '%1'").arg(ui->dateEdit->date().toString("yyyy-MM-dd"));
    daily_model.setQuery(daily);
    ui->dailyTableView->setModel(&daily_model);

   QString daily_name = QString("select part 구분, case when work = '주' then d.name end 주간,\
                                case when work = '전' then d.name end 전일,\
                                case when work = '비' then d.name end 비번\
                         from (select name, substr(work, 1, 1) work from daily where day = '%1') d\
                                  join work_type wt on d.name = wt.name").arg(ui->dateEdit->date().toString("yyyy-MM-dd"));
   daily_name_model.setQuery(daily_name);
   ui->dailyTableView2->setModel(&daily_name_model);

}

void MainWindow::setModelDay()
{
    //qDebug() << "in set Model day";
    month_model.setDay(ui->dateEdit->date());

}

MainWindow::~MainWindow()
{
    delete ui;
}


