#include "nsqlquerymodel.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
NSqlQueryModel::NSqlQueryModel(QObject *parent) : QSqlQueryModel(parent)
{

}

Qt::ItemFlags NSqlQueryModel::flags(const QModelIndex &index) const
{
    return QSqlQueryModel::flags(index) | Qt::ItemIsEditable;

}

bool NSqlQueryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    QString name;


    QString work;
   // qDebug() << "month model save " << day;
    //컬럼이이 1이하 즉 계이거나 이름이면 수정하지 않는다.
    if (index.column()<=1)
        return false;

    day = QDate(day.year(), day.month(), index.column()-1);
    name = this->data(index.model()->index(index.row(), 1)).toString();
    work = value.toString();

    // work가 2자가 아니거나 전비주보휴로 시작하지 않으면 입력하지 않는다.
    if(work.size() != 2 or !QString("전비주보휴야").contains(work[0]))
        return false;
    //TODO
    // 테이블 이름 수정
    if (index.isValid() && role == Qt::EditRole) {
        QSqlQuery query;
        query.prepare(QString("insert into %1(day, name, work) values(:day, :name, :work) on conflict(day, name) do update set work = :work").arg(tablename));
        query.bindValue(":work", work);
        query.bindValue(":name", name);
        query.bindValue(":day", day);

        if(!query.exec())
            return false;
        setQuery(query.lastQuery());
        emit dataChanged(index,index);
        return true;
    }
    return false;

}

void NSqlQueryModel::setDay(QDate date)
{
    day = date;    

}

void NSqlQueryModel::setTablename(QString name)
{
    tablename = name + "_daily";
}

bool NSqlQueryModel::clearMonth(QDate date)
{
    QSqlQuery query;
    query.prepare(QString("delete from %1 where day like '%2-%3%'").arg(tablename).arg(date.year()).arg(date.month()));


    if(!query.exec())
        qDebug() << query.lastError() << query.lastQuery();
        return false;
    setQuery(query.lastQuery());

    //emit dataChanged();
    return true;

}
