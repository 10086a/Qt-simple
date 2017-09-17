#include "statdialog.h"
#include "ui_statdialog.h"

#include <QMessageBox>
#include <QSqlQuery>

StatDialog::StatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatDialog)
{
    ui->setupUi(this);

    /*初始化TableWidget表头*/
    QStringList header;

    header << tr("编号")
           << tr("姓名")
           << tr("所属部门")
           << tr("本月工资")
           << tr("本月津贴");

    ui->tableWidget->setColumnCount(5);  // 设置表头为5列
    ui->tableWidget->setRowCount(1);     // 设置表头为1行
    ui->tableWidget->setHorizontalHeaderLabels(header);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 设置数据表宽自适应于TableView的宽
    ui->tableWidget->verticalHeader()->hide(); // 隐藏行表头

    /*初始化TableWidget_2表头*/
    QStringList header_2;

    header_2 << tr("部门编号")
           << tr("部门名称")
           << tr("月总工资")
           << tr("月总津贴");

    ui->tableWidget_2->setColumnCount(4);  // 设置表头为5列
    ui->tableWidget_2->setRowCount(1);     // 设置表头为1行
    ui->tableWidget_2->setHorizontalHeaderLabels(header_2);

    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 设置数据表宽自适应于TableView的宽
    ui->tableWidget_2->verticalHeader()->hide(); // 隐藏行表头
}

StatDialog::~StatDialog()
{
    delete ui;
}

void StatDialog::on_pushButton_1_clicked()
{
    QString id;
    int year, month;
    int nCol = ui->tableWidget->columnCount();
    id = ui->lineEdit_11->text();  // 获取用户输入的编号
    year = ui->spinBox_3->value(); // 获取用户输入的年份
    month = ui->spinBox_4->value(); // 获取用户输入的月份

    if(id.isEmpty())
    {// 如果输入为空，则结束查询，并弹出提醒框
        QMessageBox::warning(this,tr("请输入正确的内容!"),tr("您输入的值为空!"),
                             QMessageBox::Ok);
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT 信息.编号,信息.姓名,部门.部门名称,津贴.津贴,工资.工资 FROM 员工基本信息表 AS 信息 JOIN 员工工种情况表 AS 工种 ON 信息.所属工种=工种.编号 "
                  "JOIN 部门信息表 AS 部门 ON 工种.所属部门=部门.编号 JOIN 员工津贴信息表 AS 津贴 ON 信息.编号=津贴.员工编号 "
                  "JOIN 员工月工资表 AS 工资 ON 信息.编号=工资.员工编号 "
                  "WHERE 信息.编号='" + id + "' AND 津贴.加班时间='" + QVariant(year).toString() + "." + QString("%1").arg(month,2,10,QLatin1Char('0')) + "' AND 工资.月份='" + QVariant(year).toString() + "." + QString("%1").arg(month,2,10,QLatin1Char('0')) + "'");
    query.exec();

    query.first();   // 刚查寻后，结果集的指针指向第一个元素的前一个地址，需要调用first或者next让指针指到第一条记录的位置上，否则会出现错误

    if(!query.isValid())
    { // 无效记录
        QMessageBox::warning(this,tr("请输入正确的内容!"),tr("未查询到任何内容!"),
                             QMessageBox::Ok);
        return;
    }

    for(int i=0; i<nCol; i++)
    {
        // 设置单元格元素数据
        ui->tableWidget->setItem(0, i, new QTableWidgetItem(query.value(i).toString()));
        // 设置单元格元素水平垂直居中
        ui->tableWidget->item(0, i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
}

void StatDialog::on_pushButton_2_clicked()
{
    QString name;
    int year, month;
    int nCol = ui->tableWidget_2->columnCount();

    name = ui->comboBox->currentText(); // 获取用户所选择的部门
    year = ui->spinBox->value(); // 获取用户输入的年份
    month = ui->spinBox_2->value(); // 获取用户输入的月份

    if(name.isEmpty())
    {// 如果输入为空，则结束查询，并弹出提醒框
        QMessageBox::warning(this,tr("请输入正确的内容!"),tr("您输入的值为空!"),
                             QMessageBox::Ok);
        return;
    }

    QSqlQuery query;

    query.prepare("select 编号,部门名称 FROM 部门信息表 WHERE 部门名称='" + name + "'");
    query.exec();
    query.first();

    if(!query.isValid())
    { // 无效记录
        QMessageBox::warning(this,tr("请输入正确的内容!"),tr("未查询到任何内容!"),
                             QMessageBox::Ok);
        return;
    }

    for(int i=0; i<2; i++)
    {
        // 设置单元格元素数据
        ui->tableWidget_2->setItem(0, i, new QTableWidgetItem(query.value(i).toString()));
        // 设置单元格元素水平垂直居中
        ui->tableWidget_2->item(0,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }

    query.prepare("SELECT SUM(工资.工资),SUM(津贴.津贴) FROM 员工基本信息表 AS 信息 JOIN 员工工种情况表 AS 工种 ON 信息.所属工种=工种.编号 "
                  "JOIN 部门信息表 AS 部门 ON 工种.所属部门=部门.编号 JOIN 员工津贴信息表 AS 津贴 ON 信息.编号=津贴.员工编号 "
                  "JOIN 员工月工资表 AS 工资 ON 信息.编号=工资.员工编号 "
                  "WHERE 部门.部门名称='" + name + "' AND 津贴.加班时间='" + QVariant(year).toString() + "." + QString("%1").arg(month,2,10,QLatin1Char('0')) + "' AND 工资.月份='" + QVariant(year).toString() + "." + QString("%1").arg(month,2,10,QLatin1Char('0')) + "'");
    query.exec();

    query.first();   // 刚查寻后，结果集的指针指向第一个元素的前一个地址，需要调用first或者next让指针指到第一条记录的位置上，否则会出现错误

    if(!query.isValid())
    { // 无效记录
        QMessageBox::warning(this,tr("请输入正确的内容!"),tr("未查询到任何内容!"),
                             QMessageBox::Ok);
        return;
    }

    for(int i=2; i<nCol; i++)
    {
        // 设置单元格元素数据
        ui->tableWidget_2->setItem(0, i, new QTableWidgetItem(query.value(i-2).toString()));
        // 设置单元格元素水平垂直居中
        ui->tableWidget_2->item(0,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
}

void StatDialog::on_pushButton_3_clicked()
{
    int year, month;
    year = ui->spinBox_5->value(); // 获取用户输入的年份
    month = ui->spinBox_6->value(); // 获取用户输入的月份

    QSqlQuery query;

    query.prepare("select sum(工资) FROM 员工月工资表 WHERE 月份='" + QVariant(year).toString() + "." + QString("%1").arg(month,2,10,QLatin1Char('0')) + "'");
    query.exec();
    query.first();

    ui->label_3->setText(tr("企业月度总工资（元）：%1").arg(query.value(0).toString()));
}
