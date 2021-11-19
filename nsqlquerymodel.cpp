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

    //QString work;
   // qDebug() << "month model save " << day;
    //컬럼이이 1이하 즉 계이거나 이름이면 수정하지 않는다.
    if (index.column()<=1)
        return false;

    day = QDate(day.year(), day.month(), index.column()-1);
    name = this->data(index.model()->index(index.row(), 1)).toString();

    if (index.isValid() && role == Qt::EditRole) {
        QSqlQuery query;
        query.prepare("update daily set work = :work where day = :day and name = :name");
        query.bindValue(":work", value.toString());
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
