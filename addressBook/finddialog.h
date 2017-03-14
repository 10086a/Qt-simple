#ifndef FINDDIALOG_H
#define FINDDIALOG_H
#include <QDialog>

class QLineEdit;
class QPushButton;
/*
 * 前置声明：
 *      不必关心其内部结构；
 *      节省编译时间；
 */

class FindDialog:public QDialog
{
    Q_OBJECT
        // 推荐头文件中都包含这个宏，有了这个宏我们就可以不出错地去使用信号槽了
public:
    FindDialog(QWidget *parent = 0);
    QString getFindText();

public slots:
    void findClicked();

private:
    QPushButton *findButton;
    QLineEdit *lineEdit;
    QString findText;
};

#endif // FINDDIALOG_H
