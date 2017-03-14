#ifndef ABBRESSBOOK_H
#define ABBRESSBOOK_H

#include <QWidget>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qgridlayout.h>
#include <qpushbutton.h>
#include <qmap.h>
#include <qmessagebox.h>

/*
 * 枚举三种模式：Navigation、AddingMode、EditingMode
 */
enum Mode { NavigationMode,AddingMode,EditingMode };

class AddressBook : public QWidget
{
    Q_OBJECT        // Q_OBJECT宏表明该类只用国际化功能与Qt信号和槽功能

public:
    AddressBook(QWidget *parent = 0);
    /*
     * 引入updateInterface() 函数控制所有的QPushButton 对象的启用和禁用、
     */
    void updateInterface(Mode mode);
    ~AddressBook();


private:
    // 七个按钮
    QPushButton *addButton;
    QPushButton *submitButton;
    QPushButton *cancelButton;
    QPushButton *nextButton;
    QPushButton *previousButton;
    QPushButton *editButton;
    QPushButton *removeButton;


    Mode currentMode;

    // 输入字段
    QLineEdit *nameLine;
    QTextEdit *addressText;

    // 存储地址簿的联系人的容器
    /*
     * 因为其带有一个键-值对：联系人姓名作为键，而联系人地址作为值
     */
    QMap<QString, QString>contacts;
    QString oldName;
    QString oldAddress;


public slots:
    // 七个个对应的公共槽
    void addContact();
    void submitContact();
    void cancel();

    void next();
    void previous();

    void editContact();
    void removeContact();
};

#endif // ABBRESSBOOK_H
