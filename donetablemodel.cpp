#include "donetablemodel.h"

DoneTableModel::DoneTableModel()
{

}


int DoneTableModel::rowCount(const QModelIndex &parent) const
{
}

int DoneTableModel::columnCount(const QModelIndex &parent) const
{
}

QVariant DoneTableModel::data(const QModelIndex &index, int role) const
{
}

QVariant DoneTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
}

void DoneTableModel::onDataUpdated(QList<DataItems> *data_items)
{
    tasks = data_items;
}
