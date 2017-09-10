#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
//#include <QtGui>
//#include <QMessageBox>
//#include <QApplication>
//#include <QString>
//#include <QTextCodec>
//#include <QTableView>
//#include <QHeaderView>
//#include <QHBoxLayout>
//#include <QtSql/QSqlQueryModel>
//#include <QtSql/QSqlDatabase>
//#include <QtSql/QSqlError>

///*连接数据库*/
//void OpenDatabase()
//{
//    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
//    db.setDatabaseName(QString("DRIVER={SQL SERVER};"
//                               "SERVER=%1;"
//                               "DATABASE=%2;"
//                               "UID=%3;"
//                               "PWD=%4;").arg("127.0.0.1")
//                       .arg("SalaryDMS")
//                       .arg("sa")
//                       .arg("20153299"));
//    if (!db.open())
//    {

//        QMessageBox::warning(0, qApp->tr("Cannot open database"),
//                db.lastError().databaseText(), QMessageBox::Cancel);
//    }
//    else
//    {
//        qDebug()<<"Connect to Database Success!";
//    }

//}

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);

//    /*设置编码格式*/
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
//    /*
//        数据库连接
//    */
//    OpenDatabase();
//    /*
//        声明变量，按照习惯，都定义成指针
//    */
//    QDialog *mainDialog = new QDialog;
//    QHBoxLayout *layout = new QHBoxLayout;
//    QTableView *tableView = new QTableView;
//    /*
//        设置布局
//    */
////    layout->addWidget(tableView,0,Qt::AlignCenter);
//    layout->addWidget(tableView);
//    mainDialog->setLayout(layout);
//    /*
//        获取数据给tableView
//    */
//    QSqlQueryModel *model = new QSqlQueryModel;
//    model->setQuery(QObject::tr("select * from test"));
//    tableView->setModel(model);
//    /*
//        设置tableView中的数据表格大小随着tableView的大小而自动缩放
//    */
//    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    tableView->verticalHeader()->hide();
//    /*
//        设置tableView的缩放策略为自动缩放
//    */
//    tableView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
//    tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
//    mainDialog->adjustSize();
//    mainDialog->show();

//    return a.exec();
//}
