#include "statdialog.h"
#include "ui_statdialog.h"

StatDialog::StatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatDialog)
{
    ui->setupUi(this);
}

StatDialog::~StatDialog()
{
    delete ui;
}
