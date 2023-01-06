#include "todotablemodel.h"

ToDoTableModel::ToDoTableModel()
{
    tasks = nullptr;
}

int ToDoTableModel::rowCount(const QModelIndex &parent) const
{
    return 1;
}

int ToDoTableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant ToDoTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || tasks==nullptr) {
        return QModelIndex();
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

    return QModelIndex();
}


QVariant ToDoTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

void ToDoTableModel::onDataUpdated(QList<DataItems> *data_items)
{
    qDebug() << "ToDoTableModel message : data updated";
    tasks = data_items;
    insertRows(0,1,QModelIndex());
}


bool ToDoTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, rowCount(parent), rowCount(parent)+1);
    insertRows(rowCount(parent), 1, parent);
    endInsertRows();

    return true;
}


bool ToDoTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
}
