#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QWidget *parent = 0);
    ~ConnectionDialog();

    QString getDbType() const;

    QString getHostName() const;

    int getPort() const;

    QString getDbName() const;

    QString getUid() const;

    QString getPwd() const;

private slots:
    void on_btn_conn_clicked();

    void on_btn_cancle_clicked();

private:
    Ui::ConnectionDialog *ui;
    QString dbType;
    QString hostName;
    int port;
    QString dbName;
    QString uid;
    QString pwd;
};

#endif // CONNECTIONDIALOG_H
