#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialognewtask.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileInfo>
#include <QStandardPaths>
#include "todotablemodel.h"
#include "donetablemodel.h"

class ToDoTableModel;
class DoneTableModel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


struct DataItems {
    QString task;
    QString tag;
    QString startDate;
    QString endDate;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DialogNewTask dialog_new_task;
    void fetchDataToDo();
    void fetchDataDone();
    void firstTimeDBInit();

private slots:
    void on_btn_add_task_clicked();
    void on_btn_remove_task_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
    QSqlQuery *query;
    QList<DataItems> todo_data;
    QList<DataItems> done_data;
    ToDoTableModel* todo_table_model;
    DoneTableModel* done_table_model;

signals:
    void todoDataFetched(DataItems);
    void doneDataFetched(DataItems);
};
#endif // MAINWINDOW_H
