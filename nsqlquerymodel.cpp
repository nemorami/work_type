#include "nsqlquerymodel.h"
#include <QSqlQuery>
#include <QDebug>
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

   // day += index.column();

    if (index.isValid() && role == Qt::EditRole) {
        QSqlQuery query;
        query.prepare("update daily set work = :work where day = :day and name = :name");
        query.bindValue(":work", work);
        query.bindValue(":name", name);
        query.bindValue(":day", day);
        if(!query.exec())
            return false;
        setQuery(query.lastQuery());
        return true;
    }
    return false;

}

void NSqlQueryModel::setDay(QDate date)
{
    day = date.toString("yyyy-MM-dd");

}
