#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialognewtask.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    DialogNewTask dialog_new_task;

private slots:
    void on_btn_add_task_clicked();
    void on_btn_remove_task_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
