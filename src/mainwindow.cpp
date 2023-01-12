#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("Do It Today"));

    menu = new QMenu(this);
    action_done = new QAction("Done", this);
    menu->addAction(action_done);
    connect(action_done, SIGNAL(triggered(bool)),
            this, SLOT(moveItemToDone(bool)));

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

    ui->tableView_ToDo->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView_ToDo->setModel(todo_table_model);
    ui->tableView_Done->setModel(done_table_model);


    connect(this, &MainWindow::todoTableDataAdded, todo_table_model, &ToDoTableModel::onDataAdded);
    connect(this, &MainWindow::todoTableDataRemoved, todo_table_model, &ToDoTableModel::onDataRemoved);
    connect(this, &MainWindow::doneTableDataAdded, done_table_model, &DoneTableModel::onDataAdded);
    connect(&dialog_new_task, &DialogNewTask::dialogAccepted, this, &MainWindow::on_task_add_accepted);
    connect(ui->tableView_ToDo, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(toDoTableMenuRequested(QPoint)));


    auto* header = ui->tableView_ToDo->horizontalHeader();
    connect(header, SIGNAL(sectionClicked(int)), this, SLOT(onHeaderClicked(int)));

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

void MainWindow::moveItemToDone(bool)
{
    DataItems item = todo_data.at(selectedRow_toMarkAsDone);

    // Insert data to DB
    query->prepare("INSERT INTO " + QString("Done") +
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

    // add item to Done data structure
    done_data.push_back(item);
    emit doneTableDataAdded(item);


    // remove item from ToDo Table
    on_btn_remove_task_clicked(selectedRow_toMarkAsDone);
}

void MainWindow::onHeaderClicked(int column)
{
    todo_table_model->sort(column, Qt::SortOrder::AscendingOrder);
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

    done_table_model = new DoneTableModel(&done_data);
}


void MainWindow::on_btn_add_task_clicked()
{
    dialog_new_task.clearTextLines();
    dialog_new_task.exec();
}


void MainWindow::on_btn_remove_task_clicked(int selectedIndex)
{
    // get clicked index from Table View
    if(selectedIndex == -1)
        selectedIndex = ui->tableView_ToDo->currentIndex().row();
    QString selectedTask = todo_data.at(selectedIndex).task;

    // remove data from DB
    query->prepare("DELETE FROM " + QString("ToDo") + " "
                  "WHERE Task = :task_name; ");
    query->bindValue(":task_name", selectedTask);
    int i = query->exec();
    i ? qDebug() << "db remove success" : qDebug() << "fail : " << query->lastError();

    // remove item from ToDo data structure
    todo_data.erase(todo_data.begin()+selectedIndex);

    emit todoTableDataRemoved(selectedIndex);
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
    emit todoTableDataAdded(item);
}

void MainWindow::toDoTableMenuRequested(QPoint pos)
{
    QModelIndex index = ui->tableView_ToDo->indexAt(pos);
    selectedRow_toMarkAsDone = index.row();
    menu->popup(ui->tableView_ToDo->viewport()->mapToGlobal(pos));

}

