#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QDesktopServices>
#include <QUrl>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("关于SalaryManageSystem");
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_clicked()
{
    this->close();
}

void AboutDialog::on_label_3_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));
}
