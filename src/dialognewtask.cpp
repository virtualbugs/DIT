#include "dialognewtask.h"
#include "ui_dialognewtask.h"

DialogNewTask::DialogNewTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNewTask)
{
    ui->setupUi(this);

    // get today's date and set startDate and endDate according to this date
    QDateTime today = QDateTime::currentDateTime();
    QString formattedTime = today.toString("dd.MM.yyyy");
    ui->dateEdit_start->setDateTime(today);
    ui->dateEdit_end->setDateTime(today);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccepted()));
}

DialogNewTask::~DialogNewTask()
{
    delete ui;
}

QString DialogNewTask::getTask()
{
    return ui->lineEdit_task->text();
}

QString DialogNewTask::getStartDate()
{
    return ui->dateEdit_start->text();
}

QString DialogNewTask::getEndDate()
{
    return ui->dateEdit_end->text();
}

QString DialogNewTask::getTag()
{
    return ui->lineEdit_tag->text();
}

void DialogNewTask::clearTextLines()
{
    ui->lineEdit_task->clear();
    ui->lineEdit_tag->clear();
}

void DialogNewTask::onAccepted()
{
    emit dialogAccepted();
}
