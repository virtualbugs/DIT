#include "todotablemodel.h"

ToDoTableModel::ToDoTableModel(QList<DataItems>* task_list)
    :tasks(task_list)
{

}

int ToDoTableModel::rowCount(const QModelIndex &parent) const
{
    return tasks->count();
}

int ToDoTableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant ToDoTableModel::data(const QModelIndex &index, int role) const
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

void ToDoTableModel::onDataAdded(DataItems data_items)
{
    qDebug() << "ToDoTableModel message : data updated";
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

void ToDoTableModel::onDataRemoved(int selectedRow)
{
    removeRows(selectedRow, 1, QModelIndex());
    QList<DataItems>::iterator it = tasks->begin() + selectedRow;
    tasks->erase(it);
}


bool ToDoTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row+count-1);

    endInsertRows();

    return true;
}

bool ToDoTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row);
    //..
    endRemoveRows();
    return true;
}

bool ToDoTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        emit dataChanged(index, index);
        return true;
    }
    return false;
}


void ToDoTableModel::sort(int column, Qt::SortOrder order)
{
    for(int i=0; i<rowCount()-1; i++) {
        for(int j=i+1; j<rowCount(); j++) {

            QModelIndex index1 = index(i,column,QModelIndex());
            QModelIndex index2 = index(j,column,QModelIndex());
            QString val1 = index1.data(Qt::DisplayRole).toString();
            QString val2 = index2.data(Qt::DisplayRole).toString();

            for(int k=0; k<val1.size() && k<val2.size() ; k++) {
                if(val1[k].toLatin1() > val2[k].toLatin1())
                {
                    DataItems temp = (*tasks)[i];
                    (*tasks)[i] = (*tasks)[j];
                    (*tasks)[j] = temp;

                    setData(index1, val2, Qt::DisplayRole);
                    setData(index2, val1, Qt::DisplayRole);
                    break;
                }
                else if(val1[k] == val2[k]) {
                    continue;
                }
            }
        }
    }
}
