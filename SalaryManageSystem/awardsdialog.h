#ifndef AWARDSDIALOG_H
#define AWARDSDIALOG_H

#include <QDialog>

namespace Ui {
class AwardsDialog;
}

class AwardsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AwardsDialog(QWidget *parent = 0);
    ~AwardsDialog();

private slots:
    void on_pushButton_1_clicked();

private:
    Ui::AwardsDialog *ui;
    QString id;   // 输入的员工编号
    int year;     // 输入的年份
};

#endif // AWARDSDIALOG_H
