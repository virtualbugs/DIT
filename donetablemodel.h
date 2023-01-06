#ifndef DONETABLEMODEL_H
#define DONETABLEMODEL_H

#include <QAbstractTableModel>
#include "mainwindow.h"

struct DataItems;

class DoneTableModel: public QAbstractTableModel
{
public:
    DoneTableModel();

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    void onDataUpdated(QList<DataItems>*);

private:
    QList<DataItems>* tasks;
};

#endif // DONETABLEMODEL_H
