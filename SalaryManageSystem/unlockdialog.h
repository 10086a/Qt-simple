#ifndef UNLOCKDIALOG_H
#define UNLOCKDIALOG_H

#include <QDialog>

namespace Ui {
class UnlockDialog;
}

class UnlockDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UnlockDialog(QWidget *parent = 0);
    ~UnlockDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::UnlockDialog *ui;

signals:
    void setFlag(int f);
};

#endif // UNLOCKDIALOG_H
