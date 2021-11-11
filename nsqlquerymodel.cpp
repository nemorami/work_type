#include "nsqlquerymodel.h"

NSqlQueryModel::NSqlQueryModel(QObject *parent) : QSqlQueryModel(parent)
{

}

Qt::ItemFlags NSqlQueryModel::flags(const QModelIndex &index) const
{
    return QSqlQueryModel::flags(index) | Qt::ItemIsEditable;

}

bool NSqlQueryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

}
