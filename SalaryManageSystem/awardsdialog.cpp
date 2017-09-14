#include "awardsdialog.h"
#include "ui_awardsdialog.h"

AwardsDialog::AwardsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AwardsDialog)
{
    ui->setupUi(this);
}

AwardsDialog::~AwardsDialog()
{
    delete ui;
}
