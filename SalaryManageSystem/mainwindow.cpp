#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "statdialog.h"
#include "awardsdialog.h"

#include <QClipboard>
#include <QCloseEvent>
#include <QDateTime>
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
#include <QTimer>


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

    // 获取当前活动TableView
    if(index == 0)
    {
        curTableView = ui->tableView_0;
    }
    else if(index == 2)
    {
        curTableView = ui->tableView_1;
    }
    else if(index == 3)
    {
        curTableView = ui->tableView_2;
    }
    else if(index == 4)
    {
        curTableView = ui->tableView_3;
    }
    else if(index == 5)
    {
        curTableView = ui->tableView_4;
    }
    else
    {
        curTableView = ui->tableView;
    }


    curTableView->setModel(model);
    curTableView->verticalHeader()->hide();   // 隐藏列头
    curTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 设置数据表宽自适应于TableView的宽

    curTableView->setSelectionMode(QAbstractItemView::SingleSelection);  // 设置选择模式为单选
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

    updateInterface(CloseMode);

    // 显示sod_one图片
    ui->label_pic->setPixmap(QPixmap::fromImage(QImage(":/images/sod_one").scaled(100,100,Qt::KeepAspectRatio)));
    ui->label_pic->setAlignment(Qt::AlignHCenter);
//    ui->label_pic->setScaledContents(true);    // 设置图片自适应label控件的大小


    // 设置状态栏
    QTimer *timer = new QTimer(this);
    timer->start(1000); // 每次发射timeout信号时间间隔为1秒
    connect(timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
    curTimeLabel = new QLabel;
    ui->statusBar->addPermanentWidget(curTimeLabel);

    // 剪切板
    clipboard = QApplication::clipboard();
}
/*
    重构关闭函数
*/
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(!QMessageBox::information(this,tr("关闭窗口"),tr("确定要退出程序？"),tr("确定"),tr("取消")))
    {
        if(QSqlDatabase::database().isOpen())
        {
            qDebug() << "Database is opening!" << endl;
//            emit ui->action_close->trigger();    // 发送一个关闭数据库信号
            QSqlDatabase::removeDatabase(QSqlDatabase::database().connectionName());
        }
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
  使用enum型的mode来设置当前的模式，更统一的控制所有QPushButton和QAction等控件的可用状态
*/
void MainWindow::updateInterface(Mode mode)
{
    currentMode = mode;
    switch(currentMode)
    {
    case CloseMode:
        ui->label->show();
        ui->tabWidget->hide();
        ui->btn_commit->hide();
        ui->btn_cancle->hide();
        ui->btn_add->hide();
        ui->btn_delete->hide();
        ui->menu_conn->setEnabled(true);
        ui->action_close->setEnabled(false);
        ui->action_commit->setEnabled(false);
        ui->action_cut->setEnabled(false);
        ui->action_copy->setEnabled(false);
        ui->action_find->setEnabled(false);
        ui->action_paste->setEnabled(false);
        ui->action_undo->setEnabled(false);
        ui->menu_edit_sort->setEnabled(false);
        ui->action_stat->setEnabled(false);
        break;
    case ConnectMode:
        ui->label->hide();
        ui->tabWidget->show();
        ui->btn_commit->show();
        ui->btn_cancle->show();
        ui->btn_add->show();
        ui->btn_delete->show();
        ui->menu_conn->setEnabled(false);
        ui->action_close->setEnabled(true);
        ui->action_commit->setEnabled(true);
        ui->action_cut->setEnabled(true);
        ui->action_copy->setEnabled(true);
        ui->action_find->setEnabled(true);
        ui->action_paste->setEnabled(true);
        ui->action_undo->setEnabled(true);
        ui->menu_edit_sort->setEnabled(true);
        ui->action_stat->setEnabled(true);
        break;
    }
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
void MainWindow::on_action_conn_triggered()
{
    if(createConnection()) { // 如果连接成功执行
        model = new QSqlTableModel(this);
        this->setData(ui->tabWidget->currentIndex());
        ui->statusBar->showMessage(tr("数据库已连接！"));

        updateInterface(ConnectMode);
    } else {  // 连接失败
        ui->statusBar->showMessage(tr("SalaryDMS数据库连接失败，请检查服务器可用性！"));
    }
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

    qDebug() << "已经关闭数据库连接！";
    ui->statusBar->showMessage(tr("已关闭数据库连接！"));
//    QString name;
//    {
//        name = QSqlDatabase::database().connectionName();
//        QSqlDatabase::database().close();
//    }//超出作用域，隐含对象QSqlDatabase::database()被删除。
//    QSqlDatabase::removeDatabase(name);
    updateInterface(CloseMode);
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
        QString errorInfo = model->lastError().text();  // 设置数据之前要先将提交失败的错误信息保存到字符串，否则在下面操作中将丢失
        this->setData(ui->tabWidget->currentIndex());          // 设置数据放在弹出对话框之前，否则的话在关闭对话框之前表格都是没有数据的。
        QMessageBox::information(this,tr("tableModel"),tr("提交失败：请检查数据填写是否有误！\n\n错误信息如下：\n\t%1").arg(errorInfo),
                                 QMessageBox::Ok);
    }
    ui->statusBar->showMessage(tr("已提交修改"));
}
/*
    @SLOT
    在“取消“按钮上点击触发的信号槽
    作用：取消更改
*/
void MainWindow::on_btn_cancle_clicked()
{
    model->revertAll();
    ui->statusBar->showMessage(tr("用户已取消操作"));

}
/*
    @SLOT
    在“添加纪录“按钮上点击触发的信号槽
    作用：添加一行记录
*/
void MainWindow::on_btn_add_clicked()
{
    // 获取当前表的行数
    int rowNum = model->rowCount();
    // 添加一行
    model->insertRow(rowNum);
    ui->statusBar->showMessage(tr("已修改，但未提交！"));
    // 等待提交按钮按下
    //model->submitAll();
}
/*
    @SLOT
    在“删除选中行“按钮上点击触发的信号槽
    作用：删除一条记录
*/
void MainWindow::on_btn_delete_clicked()
{
    // 获取选中行
    int curRow = curTableView->currentIndex().row();
    // 删除该行
    model->removeRow(curRow);

    if(QMessageBox::warning(this,tr("删除当前行！"),tr("你确定删除当前行吗？"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::No)
    {
        // 不删除，撤销
        model->revertAll();
    }
    else
    {
        ui->statusBar->showMessage(tr("已提交修改！"));
        // 否则，提交更改
        model->submitAll();
    }
}
/*
    @SLOT
    在“提交“菜单栏上点击触发的信号槽
    作用：提交所有更改
*/
void MainWindow::on_action_commit_triggered()
{
    ui->btn_commit->click();
}
/*
    @SLOT
    在窗口右下角出现的时间更新
    作用：更新时间
*/
void MainWindow::timeUpdate()
{
    QDateTime curTime = QDateTime::currentDateTime();
    QString Timestr = curTime.toString("  yyyy年-MM月-dd日  hh:mm:ss  "); // 设置显示的格式
    curTimeLabel->setText(Timestr);
}
/*
    @SLOT
    在“编辑“菜单栏上点击触发的信号槽
    作用：排序
*/
void MainWindow::on_action_sortById_triggered()
{
    model->setSort(0,Qt::DescendingOrder);
    model->select();
//    curTableView->setModel(model);
}
/*
    @SLOT
    在“编辑“菜单栏上点击触发的信号槽
    作用：提交所有更改
*/
void MainWindow::on_action_stat_triggered()
{
    StatDialog *statDialog = new StatDialog(this);
    statDialog->exec();
}
/*
    @SLOT
    在“帮助“菜单栏上点击触发的信号槽
*/
void MainWindow::on_action_about_triggered()
{
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->exec();
}
/*
    @SLOT
    在“功能“菜单栏上点击触发的信号槽
    作用：奖学金生成
*/
void MainWindow::on_action_find_triggered()
{
    AwardsDialog *awardsDialog = new AwardsDialog(this);
    awardsDialog->exec();
}
/*
    @SLOT
    在“编辑“菜单栏上点击触发的信号槽
    作用：复制
*/
void MainWindow::on_action_copy_triggered()
{
//    QModelIndex index = curTableView->selectionModel()->selectedIndexes()
    QModelIndex index = curTableView->currentIndex();  // 获取当前选择的位置
    QVariant data = model->data(index);         // 获取当前位置的数据
    clipboard->setText(data.toString());        // 将当前数据转化成字符串添加到剪切板
}
/*
    @SLOT
    在“编辑“菜单栏上点击触发的信号槽
    作用：剪切
*/
void MainWindow::on_action_cut_triggered()
{
    QModelIndex index = curTableView->currentIndex();  // 获取当前选择的位置
    QVariant data = model->data(index);         // 获取当前位置的数据
    clipboard->setText(data.toString());        // 将当前数据转化成字符串添加到剪切板
    data = QVariant("");
    model->setData(index, data);
//    curTableView->setModel(model);
}
/*
    @SLOT
    在“编辑“菜单栏上点击触发的信号槽
    作用：粘贴
*/
void MainWindow::on_action_paste_triggered()
{
    if(!clipboard->text().isNull() && !clipboard->text().isEmpty())
    {
        QModelIndex index = curTableView->currentIndex();  // 获取当前选择的位置
        QString str = clipboard->text();
        QVariant data(str);
        model->setData(index, data);
//        curTableView->setModel(model);
    }
}
/*
    @SLOT
    在“编辑“菜单栏上点击触发的信号槽
    作用：撤销所有操作
*/
void MainWindow::on_action_undo_triggered()
{
    model->revertAll();  // 撤销所有操作
//    curTableView->setModel(model);
}
