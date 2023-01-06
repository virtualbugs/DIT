#ifndef TODOTABLEMODEL_H
#define TODOTABLEMODEL_H

#include <QAbstractTableModel>
#include "mainwindow.h"

struct DataItems;

class ToDoTableModel: public QAbstractTableModel
{
public:
    ToDoTableModel();


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

    // QAbstractItemModel interface
public:
    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;

    // QAbstractItemModel interface
public:
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
};

#endif // TODOTABLEMODEL_H
