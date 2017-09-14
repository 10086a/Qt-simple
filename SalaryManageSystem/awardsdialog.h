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

private:
    Ui::AwardsDialog *ui;
};

#endif // AWARDSDIALOG_H
