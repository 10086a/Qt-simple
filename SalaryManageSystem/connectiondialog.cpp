#include "connectiondialog.h"
#include "ui_connectiondialog.h"

#include <QMessageBox>

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::on_btn_conn_clicked()
{
    if(ui->lineEdit_dbName->text().isEmpty() || ui->lineEdit_hostName->text().isEmpty() || ui->lineEdit_uid->text().isEmpty())
    {
        QMessageBox::warning(this, tr("输入为空！"),
                             tr("请输入有效的值！"), QMessageBox::Ok);
        return;
    }

    this->dbType = ui->comboBox_db->currentText();
    this->hostName = ui->lineEdit_hostName->text();
    this->port = ui->spinBox_port->value();
    this->dbName = ui->lineEdit_dbName->text();
    this->uid = ui->lineEdit_uid->text();
    this->pwd = ui->lineEdit_pwd->text();
    this->accept();
}

QString ConnectionDialog::getPwd() const
{
    return pwd;
}

QString ConnectionDialog::getUid() const
{
    return uid;
}

QString ConnectionDialog::getDbName() const
{
    return dbName;
}

int ConnectionDialog::getPort() const
{
    return port;
}

QString ConnectionDialog::getHostName() const
{
    return hostName;
}

QString ConnectionDialog::getDbType() const
{
    return dbType;
}

void ConnectionDialog::on_btn_cancle_clicked()
{
    this->close();
}
