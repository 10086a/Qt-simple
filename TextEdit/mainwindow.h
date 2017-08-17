#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QCloseEvent;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void newFile();      //新建文件
    bool maybeSave();    //是否保存
    bool save();         //保存
    bool saveAs();       //另存为
    bool loadFile(const QString &fileName);  //载入文件
    bool saveFile(const QString &fileName);  //保存文件

private slots:
    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionUndo_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_actionAbout_triggered();

    void on_actionFind_triggered();

    void on_actionClose_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    bool isUntitled;     //true表示文件为未保存状态
    QString curFile;     //当前文件

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
