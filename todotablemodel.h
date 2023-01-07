#ifndef TODOTABLEMODEL_H
#define TODOTABLEMODEL_H

#include <QAbstractTableModel>
#include "mainwindow.h"

struct DataItems;

class ToDoTableModel: public QAbstractTableModel
{
public:
    ToDoTableModel(QList<DataItems>*);


    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent= QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent) override;

public slots:
    void onDataAdded(DataItems);
    void onDataRemoved(int);

private:
    QList<DataItems>* tasks;

};

#endif // TODOTABLEMODEL_H
