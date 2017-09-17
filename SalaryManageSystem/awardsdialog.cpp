#include "awardsdialog.h"
#include "ui_awardsdialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

/*
 * 构造函数
*/
AwardsDialog::AwardsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AwardsDialog)
{
    ui->setupUi(this);
    /*初始化TableWidget表头*/
    ui->tableWidget->setColumnCount(4);
    QStringList header;
    header << tr("员工编号")
           << tr("员工姓名")
           << tr("所属部门")
           << tr("年终奖数额");
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 设置数据表宽自适应于TableView的宽
    ui->tableWidget->verticalHeader()->hide();
}
/*
 * 析构函数
*/
AwardsDialog::~AwardsDialog()
{
    delete ui;
}

void AwardsDialog::on_pushButton_1_clicked()
{
    id = ui->lineEdit_11->text();   // 获取用户输入的编号
    year = ui->spinBox_3->value();  // 获取用户输入的年份
    qDebug() << "year = " << year;
    if(id.isEmpty())
    {// 如果输入为空，则结束查询，并弹出提醒框
        QMessageBox::warning(this,tr("请输入正确的内容!"),tr("您输入的值为空!"),
                             QMessageBox::Ok);
        return;
    }

    int nRow,nCol,i=0;
    int yearEndAwards;
    QSqlQuery query;
    query.prepare("select 信息.编号,信息.姓名,部门.部门名称 from 员工基本信息表 as 信息 join 员工工种情况表 as 工种 on 信息.所属工种=工种.编号 join 部门信息表 as 部门 on 部门.编号=工种.所属部门 where 信息.编号='" + id + "'");
    query.exec();
    query.last();
    nRow = query.at() + 1;
    qDebug() << "nRow = " << nRow;
    nCol = ui->tableWidget->columnCount();
    qDebug() << "nCol = " << nCol;
    ui->tableWidget->setRowCount(nRow);

    query.first();

    if(!query.isValid())
    { // 无效记录
        QMessageBox::warning(this,tr("请输入正确的内容!"),tr("未查询到任何内容!"),
                             QMessageBox::Ok);
        return;
    }

    for (i = 0; i<nCol-1; i++)
    {
        qDebug() << query.value(i);
        ui->tableWidget->setItem(0, i, new QTableWidgetItem(query.value(i).toString()));
        ui->tableWidget->item(0,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }


    query.prepare("select sum(工资) from 员工月工资表 "
                  "where 员工编号='" + id + "' and 月份 like '" + QVariant(year).toString() + "%'");
    query.exec();
    query.first();
    yearEndAwards = query.value(0).toInt();
    qDebug() << "总工资：" << yearEndAwards;

    query.prepare("select sum(津贴) from 员工津贴信息表 "
                  "where 员工编号='" + id + "' and 加班时间 like '" + QVariant(year).toString() + "%'");
    query.exec();
    query.first();
    yearEndAwards = (yearEndAwards + query.value(0).toInt()) / 12;
    qDebug() << "年终奖：" << yearEndAwards;

    ui->tableWidget->setItem(0, 3, new QTableWidgetItem(QVariant(yearEndAwards).toString()));
    ui->tableWidget->item(0,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

}
