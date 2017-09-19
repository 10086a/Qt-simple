#include "unlockdialog.h"
#include "ui_unlockdialog.h"

UnlockDialog::UnlockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UnlockDialog)
{
    ui->setupUi(this);
    ui->label_error->hide();
    ui->label_error->setText(tr("密码错误!"));
    ui->lineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
}

UnlockDialog::~UnlockDialog()
{
    delete ui;
}

void UnlockDialog::on_pushButton_clicked()
{
    QString pass = ui->lineEdit->text();
    if( pass == "123")
    {
        ui->label_error->setText(tr("密码正确!"));
        emit setFlag(1);
        this->close();
    }
    else
    {
        ui->label_error->setText(tr("密码错误!"));
        ui->label_error->show();
    }
}
