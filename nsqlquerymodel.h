#ifndef NSQLQUERYMODEL_H
#define NSQLQUERYMODEL_H

#include <QSqlQueryModel>

class NSqlQueryModel : public QSqlQueryModel
{
public:
    explicit NSqlQueryModel(QObject *parent = nullptr);
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role) Q_DECL_OVERRIDE;

};

#endif // NSQLQUERYMODEL_H
