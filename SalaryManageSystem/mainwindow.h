#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QSqlTableModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);
    ~MainWindow();

    void setData(int index);
private slots:
    void on_tabWidget_tabBarClicked(int index);

    void on_actionSalaryDMS_triggered();

    void on_action_close_triggered();

    void on_action_exit_triggered();

    void on_btn_commit_clicked();

    void on_btn_cancle_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
};

#endif // MAINWINDOW_H
