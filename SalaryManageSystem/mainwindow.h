#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QSqlTableModel;
class QTableView;
class QLabel;
class QTimer;

enum Mode {ConnectMode, CloseMode};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    ~MainWindow();
    /*
     * 引入updateInterface() 函数控制所有的QPushButton 对象的启用和禁用、
     */
    void updateInterface(Mode mode);

    void setData(int index);
    bool createConnection(QString dbType, QString hostName, int port, QString dbName, QString uid, QString pwd);
private slots:
    void on_tabWidget_tabBarClicked(int index);

    void on_action_conn_triggered();

    void on_action_close_triggered();

    void on_action_exit_triggered();

    void on_btn_commit_clicked();

    void on_btn_cancle_clicked();

    void on_btn_add_clicked();

    void on_btn_delete_clicked();

    void on_action_commit_triggered();

    void timeUpdate();

    void on_action_sortById_triggered();

    void on_action_stat_triggered();

    void on_action_about_triggered();

    void on_action_find_triggered();

    void on_action_copy_triggered();

    void on_action_cut_triggered();

    void on_action_paste_triggered();

    void on_action_undo_triggered();

    void on_action_update_triggered();

    void on_unlock_btn_clicked();

    void on_action_lock_triggered();

    void getFlag(int f);
private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    Mode currentMode;
    QTableView *curTableView;
    QLabel *curTimeLabel;
    QClipboard *clipboard;
    int flag = 0;
};

#endif // MAINWINDOW_H
