#ifndef STATDIALOG_H
#define STATDIALOG_H

#include <QDialog>

namespace Ui {
class StatDialog;
}

class StatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatDialog(QWidget *parent = 0);
    ~StatDialog();

private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::StatDialog *ui;
};

#endif // STATDIALOG_H
