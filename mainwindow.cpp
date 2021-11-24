#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QtCore/QStandardPaths>
#include <QDir>
//#include <mainwindow.h>
#include <QtSql/QSqlQuery>
#include <QSqlError>
#include <QClipboard>
#include <QMessageBox>
#include <QSettings>

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
    readInitFile();
    if(ui->lineEdit->text().isEmpty()){
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setText("세관_과명을 <span style=color:red><b>공백없이</b></span> 입력하세요");
        msgBox->addButton("&Ok", QMessageBox::ApplyRole);
        msgBox->setAttribute(Qt::WA_DeleteOnClose);
        msgBox->setModal(false);
        msgBox->show();
    }else{
        init();
    }

    worktype = new WorkTypeSettings(ui->lineEdit->text() +"_work_type");
    ui->tabWidget->insertTab(0, worktype, "설정");

}



void MainWindow::init()
{
    //database connection
    //db = QSqlDatabase::addDatabase("QSQLITE");

    //qApp->addLibraryPath(qApp->applicationDirPath());
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("devel.customs.go.kr");
    db.setDatabaseName("rpa");
    db.setPassword("rpa");
    db.setUserName("rpa");
    QString data_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // 데이터 디렉토리가 없으면  생성
    if (!QDir(data_dir).exists())
        QDir().mkdir(data_dir);

    //db.setDatabaseName(data_dir + "/work_db");
    //qDebug() << data_dir;
    if(!db.open()) {
        qDebug() << db.lastError();
        return;
    }

    month_model.setTablename(ui->lineEdit->text());

    /* TODO
     *
     *
     */


    QSqlQuery query;
    QString depart = ui->lineEdit->text();
    //sqlite_master
    //postgresql에서 테이블이 있는지 확인
    query.prepare(QString("select count(*) from pg_tables where schemaname = 'public' and tablename = '%1_work_type'").arg(depart));
    query.exec();

    if(query.next()) {
        if(query.value(0).toInt() == 0){
            // dictionary 테이블이 없으면
            query.exec(QString("create table %1_work_type (name text, part text not null, work_patterns text not null, primary key(name))").arg(depart));
            query.exec(QString("create table %1_daily(day text, name text, work text, primary key(day, name))").arg(depart));

        }

    } else {
     // 뭐지..

    }

    ui->dateEdit->setDate(QDate::currentDate());
    //dateEdit의 날짜를 바꾸면
//    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &MainWindow::setTodayReport);
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &MainWindow::on_refreshPushButton_clicked);
    // QDate 초기값을 오늘 날짜로 설정

    // ᅟshowDaily에서 호출
    //connect(&month_model, &NSqlQueryModel::dataChanged, this, &MainWindow::setModelDay);
    // 설정 탭
    // on_tabWidget_currentChanged로 대체..
    //connect(&ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::tabChanged);

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

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->dailyTableView2->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->dailyTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->dailyTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->dailyTableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->dailyTableView2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



}

// 업무 배치...
void MainWindow::on_deployPushButton_clicked()
{
    WorkDeploy work_deploy;
    QVector<WorkDeploy> work_deploys;

    // work_type 테이블을 work_deploys에 로딩한다.
    QSqlQuery query;
    query.prepare(QString("select name, part, work_patterns from %1_work_type").arg(ui->lineEdit->text()));
    query.exec();
    //auto shape = cursor().shape();
    QGuiApplication::setOverrideCursor(Qt::WaitCursor);
    while(query.next()) {
        work_deploy.name = query.value(0).toString();
        work_deploy.part = query.value(1).toString();
        work_deploy.work = query.value(2).toString();
        work_deploys.append(work_deploy);
    }

    QDate date;

    for (auto &deploy : work_deploys ) {
        QDate today = ui->dateEdit->date();

        auto insert = [&query, this](QDate &date, QString &name, QString &work){
            query.prepare(QString("insert into %1_daily values(:day, :name, :work)").arg(ui->lineEdit->text()));
            query.bindValue(":day", date);
            query.bindValue(":name", name);
            query.bindValue(":work", work);
            query.exec();
        };

        qDebug() << deploy.name << deploy.work;

        if(deploy.work == "정상"){
            // 휴일이면 휴무
            //아니면 정상...
            for(date = QDate(today.year(), today.month(), 1); date.month() <= today.month();date = date.addDays(1)) {
                qDebug() << date.dayOfWeek();
                QString w = (date.dayOfWeek() == 6 || date.dayOfWeek() == 7) ? "휴무" : "정상";
                insert(date, deploy.name, w);
            }
            continue;
        }else {
            auto work_type = getLastWorktype(deploy.name);
            auto w = work_type.first;
            date = work_type.second;
            //qDebug() << "return getLastWorktype=>" << date;

            if(w == nullptr) {
                date = QDate(today.year(), today.month(), 1);
                w = QString("%1%2").arg(deploy.work[0]).arg(1);
                insert(date, deploy.name, w);

            }
            //qDebug() << "date => " << date;
            for(date = date.addDays(1); date <= QDate(today.year(),today.month(), today.daysInMonth());date = date.addDays(1)) {
                w = getNextWorktype(deploy.work, w);
                insert(date, deploy.name, w);
            }
        }
    }

    showMonth();
    showDaily();
    QGuiApplication::restoreOverrideCursor();


}

void MainWindow::on_savePushButton_clicked()
{

    //ui->tableView->co
    //qDebug() << "on save " << month_model.headerData(2, Qt::Horizontal);
    month_model.submit();
}

void MainWindow::on_clearPushButton_clicked()
{
    clearMonthView();
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

void MainWindow::on_lineEdit_editingFinished()
{
    init();    
    worktype->loadData(QString("%1_work_type").arg(ui->lineEdit->text()));

}

void MainWindow::on_aboutPushButton_clicked()
{
    //todo: 버전 빌드 연동...
    QMessageBox::about(this, "Work Dploy", QString("version: %1").arg("0.0.2"));

}


QString MainWindow::getNextWorktype(QString part, QString init_work)
{
//    QSqlQuery query;

//    query.prepare("select work_patterns from work_type where name = :name");
//    query.bindValue(":name", name);
//    query.exec();

//    query.next();

    //QString work_list = query.value(0).toString();
    // init_work를 work와 index로 분리 ex) 주2: work => 주, index => 2

    if(part.isEmpty())
        return nullptr;
    QChar work = init_work[0];
    int index = init_work.remove(0,1).toInt();
    // 다음 근무일 위치를 구한다.
    int search = 0;
    //index번째 work의 위치를 찾는다.
    while(index--){
        search = part.indexOf(work, search);
        search++;
    }

    if(search >= part.size())
        search = 0;
    work = part[search];
    index = part.left(search+1).count(work);
    return QString("%1%2").arg(part[search]).arg(index);

}
/**
 * @brief MainWindow::getLastWorktype
 * @param name
 * @return
 * 마지막 Work type이 없으면 nulltype리턴....
 */
QPair<QString, QDate> MainWindow::getLastWorktype(QString name)
{
    QSqlQuery query;
    QString work = nullptr;
    QDate date;
    query.prepare(QString("select day, work from %1_daily where name = :name order by day desc").arg(ui->lineEdit->text()));
    query.bindValue(":name", name);
    query.exec();

    while(query.next()){
        date = query.value(0).toDate();
        work = query.value(1).toString();
        if(QString("주전비보").contains(work[0]))
                break;
    }

    return QPair<QString, QDate>(work, date);
}

void MainWindow::readInitFile()
{
    QSettings settings("nemorami", "workdploy");
    ui->lineEdit->setText(settings.value("division/name").toString());
}

void MainWindow::writeInitFile()
{
    QSettings settings("nemorami", "workdploy");
    settings.setValue("division/name", ui->lineEdit->text());

}

void MainWindow::on_refreshPushButton_clicked()
{
    showMonth();
    showDaily();
}


void MainWindow::showMonth()
{
    // 한달 근무 배치표 탭 만들기
    QString query = QString("select part \"계\", name \"이름\",\
                            max(case when day = '01' then work end) \"01\",\
                            max(case when day = '02' then work end) \"02\",\
                            max(case when day = '03' then work end) \"03\",\
                            max(case when day = '04' then work end) \"04\",\
                            max(case when day = '05' then work end) \"05\",\
                            max(case when day = '06' then work end) \"06\",\
                            max(case when day = '07' then work end) \"07\",\
                            max(case when day = '08' then work end) \"08\",\
                            max(case when day = '09' then work end) \"09\",\
                            max(case when day = '10' then work end) \"10\",\
                            max(case when day = '11' then work end) \"11\",\
                            max(case when day = '12' then work end) \"12\",\
                            max(case when day = '13' then work end) \"13\",\
                            max(case when day = '14' then work end) \"14\",\
                            max(case when day = '15' then work end) \"15\",\
                            max(case when day = '16' then work end) \"16\",\
                            max(case when day = '17' then work end) \"17\",\
                            max(case when day = '18' then work end) \"18\",\
                            max(case when day = '19' then work end) \"19\",\
                            max(case when day = '20' then work end) \"20\",\
                            max(case when day = '21' then work end) \"21\",\
                            max(case when day = '22' then work end) \"22\",\
                            max(case when day = '23' then work end) \"23\",\
                            max(case when day = '24' then work end) \"24\",\
                            max(case when day = '25' then work end) \"25\",\
                            max(case when day = '26' then work end) \"26\",\
                            max(case when day = '27' then work end) \"27\",\
                            max(case when day = '28' then work end) \"28\",\
                            max(case when day = '29' then work end) \"29\",\
                            max(case when day = '30' then work end) \"30\",\
                            max(case when day = '31' then work end) \"31\"\
                     from %1_work_type left join (select substr(day, 9, 2) as day, name, work from %1_daily where substr(day, 1, 7) = '%2-%3') ta using(name) \
                     group by 1, 2 order by 1, 2").arg(ui->lineEdit->text()).arg(ui->dateEdit->date().year()).arg(ui->dateEdit->date().month());
    month_model.setQuery(query);

    //qDebug() << month_model.lastError();

    ui->tableView->setModel(&month_model);

}

void MainWindow::clearMonthView()
{

  month_model.clearMonth(ui->dateEdit->date());
  showMonth();
}

void MainWindow::copyToClipboard()
{
    QString selected_text;
    auto indexes = ui->tableView->selectionModel()->selectedIndexes();
    auto previous = indexes.first();

    for (auto index : indexes) {
        auto data = ui->tableView->model()->data(index).toString();
        if(index.row() != previous.row())
            selected_text.append('\n');
        else
            selected_text.append('\t');
        selected_text.append(data);
        previous = index;
    }
    selected_text.remove(0,1);
    qApp->clipboard()->setText(selected_text);
}

void MainWindow::customMenuRequested(QPoint pos)
{
    QModelIndex index = ui->tableView->indexAt(pos);
    QMenu *menu = new QMenu(this);

    menu->addAction(tr("&Copy"), [this](){this->copyToClipboard();});
    menu->addAction(tr("Clear"), [this](){this->clearMonthView();});

    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));

}

void MainWindow::showDaily()
{
    setModelDay();
    QString daily = QString("select count(*) 현원,\
            sum(case when substr(work, 1,1) in('전', '주', '보') then 1 end) 근무,\
            sum(case when substr(work, 1,1) = '주' then 1 end) 주간, \
            sum(case when substr(work, 1,1) = '야' then 1 end) 야간, \
            sum(case when substr(work, 1,1) = '비' then 1 end) 비번, \
            sum(case when substr(work, 1,1) = '휴' then 1 end) 휴무, \
            sum(case when substr(work, 1,1) = '연' then 1 end) 연가 \
            from %1_daily where day = '%2'").arg(ui->lineEdit->text()).arg(ui->dateEdit->date().toString("yyyy-MM-dd"));
    daily_model.setQuery(daily);
    ui->dailyTableView->setModel(&daily_model);

   QString daily_name = QString("select part \"구분\", \
                                        string_agg(case when work = '주' then d.name end, ', ') \"주간\",\
                                        string_agg(case when work = '전' then d.name end, ', ') \"전일\",\
                                        string_agg(case when work = '정' then d.name end, ', ') \"정상\",\
                                        string_agg(case when work = '야' then d.name end, ', ') \"야간\",\
                                        string_agg(case when work = '비' then d.name end, ', ') \"비번\", \
                                        string_agg(case when work = '휴' then d.name end, ', ') \"휴무\", \
                                        string_agg(case when work = '연' then d.name end, ', ') \"연가\" \
                                  from (select name, substr(work, 1, 1) \"work\" from %1_daily where day = '%2') d \
                                   join %1_work_type wt on d.name = wt.name \
                                  group by 1").arg(ui->lineEdit->text()).arg(ui->dateEdit->date().toString("yyyy-MM-dd"));
   daily_name_model.setQuery(daily_name);
   qDebug() << daily_name_model.lastError();
   ui->dailyTableView2->setModel(&daily_name_model);

}

void MainWindow::setModelDay()
{
    //qDebug() << "in set Model day";
    month_model.setDay(ui->dateEdit->date());

}

MainWindow::~MainWindow()
{
    writeInitFile();
    delete ui;
}


