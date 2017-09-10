#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QSqlTableModel>

#include <QTextCodec>
#include <QTableView>
#include <QHeaderView>
#include <QMessageBox>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QHBoxLayout>
#include <QHeaderView>

/*
    连接数据库
*/
bool createConnection()
{
    /*连接数据库*/
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName(QString("DRIVER={SQL SERVER};"
                               "SERVER=%1;"
                               "DATABASE=%2;"
                               "UID=%3;"
                               "PWD=%4;").arg("127.0.0.1")
                       .arg("SalaryDMS")
                       .arg("sa")
                       .arg("20153299"));
    if (!db.open())
    {

        QMessageBox::warning(0, qApp->tr("Cannot open database"),
                db.lastError().databaseText(), QMessageBox::Cancel);
        return false;
    }
    else
    {
        qDebug()<<"Connect to Database Success!";
        return true;
    }
}
/*
    向ui->tableView设置model数据
*/
void MainWindow::setData(int index)
{
    model->setTable(ui->tabWidget->tabText(index));
    model->select();
    // 设置编辑策略为：所有改变都会在模型中缓存，等待submitAll或者revertAll
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if(index == 0)
    {
        ui->tableView_1->setModel(model);
        ui->tableView_1->verticalHeader()->hide();

        ui->tableView_1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
    else if(index == 1)
    {
        ui->tableView_2->setModel(model);
        ui->tableView_2->verticalHeader()->hide();

        ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
    else if(index == 2)
    {
        ui->tableView_3->setModel(model);
        ui->tableView_3->verticalHeader()->hide();

        ui->tableView_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
    else if(index == 3)
    {
        ui->tableView_4->setModel(model);
        ui->tableView_4->verticalHeader()->hide();

        ui->tableView_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
    else
    {
        ui->tableView_5->setModel(model);
        ui->tableView_5->verticalHeader()->hide();

        ui->tableView_5->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
}
/*
    构造函数
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    model = new QSqlQueryModel(this);

    ui->label->show();
    ui->tabWidget->hide();
    ui->btn_cancle->hide();
    ui->btn_commit->hide();

}
/*
    重构关闭函数
*/
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!QMessageBox::information(this,tr("关闭窗口"),tr("确定要退出程序？"),tr("确定"),tr("取消")))
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}
/*
    析构函数
*/
MainWindow::~MainWindow()
{
    delete ui;
}
/*
    @SLOT
    在tab上点击触发的信号槽
*/
void MainWindow::on_tabWidget_tabBarClicked(int index)
{

    this->setData(index);
}
/*
    @SLOT
    在菜单栏上点击触发的信号槽-----SalaryDMS
*/
void MainWindow::on_actionSalaryDMS_triggered()
{
    createConnection();
    model = new QSqlTableModel(this);
    this->setData(ui->tabWidget->currentIndex());

    ui->label->hide();
    ui->tabWidget->show();
    ui->btn_cancle->show();
    ui->btn_commit->show();
}
/*
    @SLOT
    在菜单栏上点击触发的信号槽-----断开
*/
void MainWindow::on_action_close_triggered()
{
    /*
        关闭数据库
    */
    QStringList dbList = QSqlDatabase::connectionNames();
    for(int i = 0; i < dbList.count(); ++i) {
    QSqlDatabase::removeDatabase(dbList[i]);
    }


//    QString name;
//    {
//        name = QSqlDatabase::database().connectionName();
//        QSqlDatabase::database().close();
//    }//超出作用域，隐含对象QSqlDatabase::database()被删除。
//    QSqlDatabase::removeDatabase(name);

    ui->label->show();
    ui->tabWidget->hide();
    ui->btn_commit->hide();
    ui->btn_cancle->hide();
}
/*
    @SLOT
    在菜单栏上点击触发的信号槽-----退出
*/
void MainWindow::on_action_exit_triggered()
{
    // close会发送一个信号给closeEvent
    this->close();
}
/*
    @SLOT
    在“确定“按钮上点击触发的信号槽
    作用：提交更改
*/
void MainWindow::on_btn_commit_clicked()
{
    model->database().transaction();
    if(model->submitAll())
    {
        if(model->database().commit())
        {
            this->setData(ui->tabWidget->currentIndex());       // 重新向ui->tableview设置数据,因为提交成功之后所有数据都空白了
            QMessageBox::information(this,tr("commit"),tr("提交成功！"));
        }
    }
    else
    {
        model->database().rollback();
        QMessageBox::information(this,tr("tableModel"),tr("提交失败：1%").arg(model->database().lastError().text()),QMessageBox::Ok);
    }

}
/*
    @SLOT
    在“取消“按钮上点击触发的信号槽
    作用：取消更改
*/
void MainWindow::on_btn_cancle_clicked()
{
    model->revertAll();
}
