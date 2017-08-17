#include "aboutdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isUntitled = true;     //初始化为未保存状态
    curFile = tr("未命名.txt");  //当前文件

    setWindowTitle(curFile); //设置窗口标题
    setWindowIcon(QIcon(":/images/windowIcon"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        isUntitled = true;
        curFile = tr("未命名.txt");

        setWindowTitle(curFile);
        ui->textEdit->clear();
        ui->textEdit->setVisible(true);
    }
}

bool MainWindow::maybeSave()
{
    //如果文档被更改了
    if (ui->textEdit->document()->isModified()) {
        //自定义一个警告对话框
        QMessageBox box;
        box.setWindowTitle(tr("警告"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile + tr(" 尚未保存，是否保存？"));
        QPushButton *yesBtn = box.addButton(tr("是(&Y)"),
                                            QMessageBox::YesRole);
        box.addButton(tr("否(&N)"), QMessageBox::NoRole);
        QPushButton *cancelBtn = box.addButton(tr("取消"),
                                               QMessageBox::RejectRole);
        box.exec();
        if (box.clickedButton() == yesBtn) {
            return save();
        } else if (box.clickedButton() == cancelBtn) {
            return false;
        } else {
            return true;
        }
    }

    //如果文档没有被更改，则直接返回true
    return true;
}

bool MainWindow::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("另存为"), curFile,
                                                    tr("Document (*.txt);;All Files (*.*)"));
    if (fileName.isEmpty()) {
        return false;
    }
    return saveFile(fileName);
}

bool MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);    //新建QFile对象
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("多文档编辑器"),
                             tr("无法读取文件 %1: \n%2.")
                             .arg(fileName).arg(file.errorString()));
        return false;    //只读方式打开文件，出错则提示，并返回false
    }
    QTextStream in(&file);   //新建文本流对象
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //读取文本的全部文本内容，并添加到编辑器中
    ui->textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    //设置当前文件
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    ui->textEdit->document()->setModified(false);     // 如果载入文件成功后，则设置文本框的状态为未修改状态
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("多文档编辑器"),
                             tr("无法写入文件 %1: /n %2")
                             .arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    //鼠标指针变为等待状态
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << ui->textEdit->toPlainText();
    //鼠标指针恢复原来的状态
    QApplication::restoreOverrideCursor();
    isUntitled = false;
    //获得文件的标准路径
    curFile = QFileInfo(fileName).canonicalFilePath();
    setWindowTitle(curFile);
    ui->textEdit->document()->setModified(false);   // 如果保存文件成功后，则设置文本框的状态为未修改状态
    return true;
}

void MainWindow::on_actionOpen_triggered()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), curFile,
                                                        tr("Document (*.txt *.html);;All File (*.*)"));
        //如果文件名不为空，则加载文件
        if (!fileName.isEmpty()) {
            loadFile(fileName);
            ui->textEdit->setVisible(true);
        }
    }
}

void MainWindow::on_actionNew_triggered()
{
    newFile();
}

void MainWindow::on_actionSave_triggered()
{
    save();
}

void MainWindow::on_actionSave_As_triggered()
{
    saveAs();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *aboutDlg = new AboutDialog(this);
    aboutDlg->setWindowModality(Qt::NonModal);
    aboutDlg->show();
}

void MainWindow::on_actionFind_triggered()
{

}

void MainWindow::on_actionClose_triggered()
{
    setWindowTitle(tr("Notepad"));
    isUntitled = true;
    if (maybeSave()) {
        ui->textEdit->clear();
        ui->textEdit->setVisible(false);
    }
}

void MainWindow::on_actionExit_triggered()
{
    //先执行关闭操作，在执行退出操作
    //qApp为全局变量
    on_actionClose_triggered();
    qApp->quit();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    //如果已经保存了文本文档，则关闭程序
    if (maybeSave()) {
        event->accept();
    } else {  //否则忽略该事件
        event->ignore();
    }
}
