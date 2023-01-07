#include "donetablemodel.h"

DoneTableModel::DoneTableModel(QList<DataItems>* task_list)
    :tasks(task_list)
{

}

int DoneTableModel::rowCount(const QModelIndex &parent) const
{
    return tasks->count();
}

int DoneTableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant DoneTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || tasks==nullptr) {
        return QVariant();
    }

    if(role == Qt::DisplayRole) {
        if(index.row() < tasks->count() && index.column() < 4) {
            if(index.column()==0)
                return tasks->at(index.row()).task;
            else if(index.column()==1)
                return tasks->at(index.row()).tag;
            else if(index.column()==2)
                return tasks->at(index.row()).startDate;
            else if(index.column()==3)
                return tasks->at(index.row()).endDate;
        }
    }

    return QVariant();
}


QVariant DoneTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Orientation::Horizontal) {
        if(role == Qt::DisplayRole) {
            if(section==0) {
                return "Task";
            }
            else if(section==1) {
                return "Tag";
            }
            else if(section==2) {
                return "Start Date";
            }
            else if(section==3) {
                return "Due Date";
            }
        }
    }
    return QVariant();
}


bool DoneTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row+count-1);

    endInsertRows();

    return true;
}

bool DoneTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    //..
    endRemoveRows();
    return true;
}

void DoneTableModel::onDataAdded(DataItems data_items)
{
    qDebug() << "DoneTableModel message : data updated";
    int rCount = 0;
    qDebug() << "rCount : " << rCount;
    insertRows(rCount, 1, QModelIndex());
    tasks->insert(tasks->begin()+rCount, data_items);

    QModelIndex index_task = index(rCount, 0, QModelIndex());
    setData(index_task, data_items.task, Qt::DisplayRole);
    QModelIndex index_tag = index(rCount, 1, QModelIndex());
    setData(index_tag, data_items.tag, Qt::DisplayRole);
    QModelIndex index_startdate = index(rCount, 2, QModelIndex());
    setData(index_startdate, data_items.startDate, Qt::DisplayRole);
    QModelIndex index_enddate = index(rCount, 3, QModelIndex());
    setData(index_enddate, data_items.endDate, Qt::DisplayRole);
}

bool DoneTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        emit dataChanged(index, index);
        return true;
    }
    return false;
}
