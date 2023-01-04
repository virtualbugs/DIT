#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Do It Today");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_add_task_clicked()
{
    qDebug() << "Add New Task";
    //dialog_new_task.show();
    dialog_new_task.exec();
    QString task = dialog_new_task.getTask();
    QString tag = dialog_new_task.getTag();
    QString startDate = dialog_new_task.getStartDate();
    QString endDate = dialog_new_task.getEndDate();


}


void MainWindow::on_btn_remove_task_clicked()
{
    qDebug() << "Remove Task";
}

