#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Do It Today"));
    database = QSqlDatabase::addDatabase("QSQLITE");
    QString dbFilePath = "~/" + QStandardPaths::displayName(QStandardPaths::DocumentsLocation) + "/DIT.sqlite3";
    qDebug() << "-> " << dbFilePath;
    database.setDatabaseName("/home/felix/bla.sqlite3");
    query = new QSqlQuery(database);

    // open database,
    if(!database.open()) {
        qDebug() << "Database open is failed";
    }

    // if DB is created first time, add tables to DB
    firstTimeDBInit();

    fetchDataToDo();
    fetchDataDone();

    ui->tableView_ToDo->setModel(todo_table_model);

    connect(this, &MainWindow::todoDataAdded, todo_table_model, &ToDoTableModel::onDataAdded);
    connect(this, &MainWindow::todoDataRemoved, todo_table_model, &ToDoTableModel::onDataRemoved);
    connect(&dialog_new_task, &DialogNewTask::dialogAccepted, this, &MainWindow::on_task_add_accepted);
}

MainWindow::~MainWindow()
{
    delete ui;
    database.close();
}

void MainWindow::firstTimeDBInit()
{
    /* create Table ToDo
     *
     */
    query->prepare("CREATE TABLE " + QString("ToDo") + " ("
                  "Task TEXT, "
                  "Tag TEXT, "
                  "StartDate TEXT, "
                  "EndDate TEXT"
                  ");");
    bool i = query->exec();
    //i ? qDebug() << "success" : qDebug() << "fail : " << query->lastError();


    /* create Table Done
     *
     */
    query->prepare("CREATE TABLE " + QString("Done") + " ("
                  "Task TEXT, "
                  "Tag TEXT, "
                  "StartDate TEXT, "
                  "EndDate TEXT"
                  ");");
    i = query->exec();
    //i ? qDebug() << "success" : qDebug() << "fail : " << query->lastError();

}

void MainWindow::fetchDataToDo()
{
    todo_data.clear();
    // read ToDo data from database
    query->prepare("SELECT * FROM ToDo");
    bool i = query->exec();
    i ? qDebug() << "success" : qDebug() << "fail :" << query->lastError();

    /*** Fetch Data  ***/
    while(query->next()) {
        DataItems data_items;
        data_items.task = query->value(0).toString();
        data_items.tag = query->value(1).toString();
        data_items.startDate = query->value(2).toString();
        data_items.endDate = query->value(3).toString();
        todo_data.push_back(data_items);
    }
    todo_table_model = new ToDoTableModel(&todo_data);
}

void MainWindow::fetchDataDone()
{
    done_data.clear();
    // read Done data from database
    query->prepare("SELECT * FROM Done");
    bool j = query->exec();
    j ? qDebug() << "success" : qDebug() << "fail :" << query->lastError();

    /*** Fetch Data  ***/
    while(query->next()) {
        DataItems data_items;
        data_items.task = query->value(0).toString();
        data_items.tag = query->value(1).toString();
        data_items.startDate = query->value(2).toString();
        data_items.endDate = query->value(3).toString();
        done_data.push_back(data_items);
    }

}


void MainWindow::on_btn_add_task_clicked()
{
    dialog_new_task.clearTextLines();
    dialog_new_task.exec();
}


void MainWindow::on_btn_remove_task_clicked()
{
    // get clicked index from Table View
    int selectedIndex = ui->tableView_ToDo->currentIndex().row();
    QString selectedTask = todo_data.at(selectedIndex).task;

    // remove data from DB
    query->prepare("DELETE FROM " + QString("ToDo") + " "
                  "WHERE Task = :task_name; ");
    query->bindValue(":task_name", selectedTask);
    int i = query->exec();
    i ? qDebug() << "db remove success" : qDebug() << "fail : " << query->lastError();


    emit todoDataRemoved(selectedIndex);
}

void MainWindow::on_task_add_accepted()
{
    // Get user inputs
    DataItems item;
    item.task = dialog_new_task.getTask();
    item.tag = dialog_new_task.getTag();
    item.startDate = dialog_new_task.getStartDate();
    item.endDate = dialog_new_task.getEndDate();

    // Insert data to DB
    query->prepare("INSERT INTO " + QString("ToDo") +
                  "(Task,Tag,StartDate,EndDate) \
                  VALUES(:task_name,:tag_name,:start_date,"
                        ":end_date"
                  "); ");
    query->bindValue(":task_name", item.task);
    query->bindValue(":tag_name", item.tag);
    query->bindValue(":start_date", item.startDate);
    query->bindValue(":end_date", item.endDate);

    int i = query->exec();
    i ? qDebug() << "db write success" : qDebug() << "fail : " << query->lastError();

    todo_data.push_back(item);
    emit todoDataAdded(item);
}

