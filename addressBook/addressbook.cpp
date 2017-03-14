#include "addressbook.h"

/*
 * updateInterface() : 通过在同一函数中设置模式和更新用户界面，我们可以避免用户界面与应用程序内部状态不同步的可能性
 * 将传递进的参数mode更新当前的模式，将参数分配给 currentMode
 * 每个按钮根据当前模式进行启用或禁用
 */
void AddressBook::updateInterface(Mode mode)
{
    currentMode = mode;

    switch(currentMode) {
    case AddingMode :
        nameLine->setReadOnly(false);
        nameLine->setFocus(Qt::OtherFocusReason);
        addressText->setReadOnly(false);

        addButton->setEnabled(false);
        submitButton->show();
        cancelButton->show();

        nextButton->setEnabled(false);
        previousButton->setEnabled(false);

        editButton->setEnabled(false);
        removeButton->setEnabled(false);
        break;
    case EditingMode :
        nameLine->setReadOnly(false);
        nameLine->setFocus(Qt::OtherFocusReason);
        addressText->setReadOnly(false);

        addButton->setEnabled(false);
        editButton->setEnabled(false);
        removeButton->setEnabled(false);
        nextButton->setEnabled(false);
        previousButton->setEnabled(false);

        submitButton->show();
        cancelButton->show();
        break;
    case NavigationMode :
        if(contacts.isEmpty()) {
            nameLine->clear();
            addressText->clear();
        }

        nameLine->setReadOnly(true);
        addressText->setReadOnly(true);
        addButton->setEnabled(true);

        editButton->setEnabled(contacts.size() >= 1);
        removeButton->setEnabled(contacts.size() >= 1);
        nextButton->setEnabled(contacts.size() > 1);
        previousButton->setEnabled(contacts.size() > 1);

        submitButton->hide();
        cancelButton->hide();
        break;
    }
}


/*
 * 我们使用oldName和oldAddress储存最后显示的联系人详细信息。
 * 然后，我们清空这些输入字段并关闭只读模式。
 * 输入焦点设置在nameLine，
 * 显示 submitButton 和 cancelButton
 * 禁用浏览按钮
 */
void AddressBook::addContact()
{
    oldName = nameLine->text();
    oldAddress = addressText->toPlainText();

    nameLine->clear();
    addressText->clear();

    updateInterface(AddingMode);     // 切换到编辑模式
}


/*
 * submitContact():
 * 从nameLine 和 addressText 提取联系人的详细信息，
 * 然后将其存储在QString对象中。
 * 验证确保用户没有在输入字段为空时点击提交， 否则，会显示 QMessageBox 提示用户输入姓名和地址
 * 然后，检查是否联系人已存在。如果不存在就添加至contacts，并显示QMessageBox 提示用户已添加联系人
 * 如果已存在，则会显示QMessageBox 以提示用户
 * 按钮回复常态
 */
void AddressBook::submitContact()
{
    QString name = nameLine->text();
    QString address = addressText->toPlainText();

    if(name == "" || address == "") {
        QMessageBox::information(this,tr("Empty Field"),
                                 tr("Please enter a name and address."));
        return;
    }

    if(currentMode == AddingMode) {

        if(!contacts.contains(name)) {
            contacts.insert(name,address);
            QMessageBox::information(this,tr("Add Successful"),
                                     tr("\"%1 %2\" has been added to your address book.").arg(name).arg(address));
        } else {
            QMessageBox::information(this,tr("Add Unsuccessful"),
                                     tr("Sorry,\"%1\" is already in your address book.").arg(name));
            return;
        }
    } else if(currentMode == EditingMode) {

        if(oldName != name) {
            if(!contacts.contains(name)) {
                QMessageBox::information(this,tr("Edit Successful"),
                                         tr("\"%1\" has been edited to your address book.").arg(oldName));
                contacts.remove(oldName);
                contacts.insert(name,address);
            } else {
                QMessageBox::information(this,tr("Edit Unsuccessful"),
                                         tr("\"%1\" is already in your address book.").arg(name));
                return;
            }
        } else if(oldAddress != address) {
            QMessageBox::information(this,tr("Edit Successful"),
                                     tr("\"%1\" has been edited to your address book.").arg(name));
            contacts[name] = address;
        } else {
            QMessageBox::information(this,tr("Edit Unsuccessful"),
                                     tr("It is not changed."));
            return;
        }
    }

    updateInterface(NavigationMode);           // 切换到导航模式

//    if(contacts.isEmpty()) {         // ?
//        nameLine->clear();
//        addressText->clear();
//    }

//    nameLine->setReadOnly(true);
//    addressText->setReadOnly(true);
//    addButton->setEnabled(true);     // ?
//    submitButton->hide();
//    cancelButton->hide();
//    // 多个联系人时才启用浏览按钮
//    nextButton->setEnabled(contacts.size() > 1);
//    previousButton->setEnabled(contacts.size() > 1);
}

/*
 * cancel()
 * 恢复上次显示的联系人详细信息，并启用addButton，隐藏其他按钮
 */
void AddressBook::cancel()
{
    nameLine->setText(oldName);
    addressText->setText(oldAddress);

    updateInterface(NavigationMode);
}

/*
 * next()
 * QMap对象模拟一个循环链表。
 * 获取contacts的迭代器
 *      迭代器未达到contacts结尾，就加1
 *      已达到结尾，就移至起始位置。
 * 通过nameLine 和 addressText 显示对象的内容
 */
void AddressBook::next()
{
    QString name = nameLine->text();
    QMap<QString, QString>::iterator i = contacts.find(name);

    if(i != contacts.end())
        i++;

    if(i == contacts.end())
        i = contacts.begin();

    nameLine->setText(i.key());
    addressText->setText(i.value());
}
/*
 * previous()
 * 获取contacts 的迭代器：
 *      达到结尾，就清除显示内容，然后返回
 *      如果迭代器在起始位置，就将其移至结尾
 *      迭代器减一
 */
void AddressBook::previous()
{
    QString name = nameLine->text();
    QMap<QString,QString>::iterator i = contacts.find(name);

    if(i == contacts.end()) {          // ?没啥用啊
        nameLine->clear();
        addressText->clear();
        return;
    }

    if(i == contacts.begin()) {
        i = contacts.end();
    }

    i--;

    nameLine->setText(i.key());
    addressText->setText(i.value());
}
/*
 * editContact()
 * 切换到EditingMode 模式之前，先保存旧的联系人信息
 */
void AddressBook::editContact()
{
    oldName = nameLine->text();
    oldAddress = addressText->toPlainText();

    updateInterface(EditingMode);
}
/*
 * removeContact()
 * 弹出 QMessageBox 向用户询问是否删除，一旦确认则先调用next()显示下一个联系人，在删除刚才的联系人即可
 */
void AddressBook::removeContact()
{
    QString name = nameLine->text();

    if(contacts.contains(name)) {
        int choose = QMessageBox::question(this,tr("Confirm Remove"),
                                           tr("Are you sure you want to remove \"%1\" ?").arg(name),
                                           QMessageBox::Yes|QMessageBox::No);
        if(choose == QMessageBox::Yes) {
            next();
            contacts.remove(name);
            QMessageBox::information(this,tr("Remove Successful"),
                                     tr("\%1\" has been removed from your address book.").arg(name));
        }
    }

    updateInterface(NavigationMode);      // 切换到导航模式
}


AddressBook::AddressBook(QWidget *parent)
    : QWidget(parent)
{
    QLabel *nameLabel = new QLabel(tr("Name:"));
    nameLine = new QLineEdit;
    nameLine->setReadOnly(true);         // 将nameLine设置为只读

    QLabel *addressLabel = new QLabel(tr("Address:"));
    addressText = new QTextEdit;
    addressText->setReadOnly(true);      // 将addressText设置为只读

    // 实例化一下按钮： addButton、submitButton、cancelButton、nextButton、previousButton
    addButton = new QPushButton(tr("&Add"));
    addButton->show();
    submitButton = new QPushButton(tr("&Submit"));
    submitButton->hide();
    cancelButton = new QPushButton(tr("&Cancel"));
    cancelButton->hide();

    nextButton = new QPushButton(tr("&Next"));
    nextButton->setEnabled(false);        // 仅当地址簿有多个联系人的时候才会启用浏览功能
    previousButton = new QPushButton(tr("&Previous"));
    previousButton->setEnabled(false);

    /*
     * editButton 和 removeButton 实例化为默认禁用
     * 因为地址簿启动时在内存中没有联系人
     */
    editButton = new QPushButton(tr("&Edit"));
    editButton->setEnabled(false);
    removeButton = new QPushButton(tr("&Remove"));
    removeButton->setEnabled(false);

    /*
     * 显示addButton是通过调用show()函数实现的，
     * 而隐藏submitButton和cancelButton则需调用hide()。这两个按钮仅当用户点击  添加 时才会显示，
     * 而此操作是通过addContact()函数处理的。
     */

    //将按钮的clicked()信号与其相应的槽关联
    connect(addButton,SIGNAL(clicked()),this,SLOT(addContact()));
    connect(submitButton,SIGNAL(clicked()),this,SLOT(submitContact()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(nextButton,SIGNAL(clicked()),this,SLOT(next()));
    connect(previousButton,SIGNAL(clicked()),this,SLOT(previous()));
    connect(editButton,SIGNAL(clicked()),this,SLOT(editContact()));
    connect(removeButton,SIGNAL(clicked()),this,SLOT(removeContact()));

    // 使用QVBoxLayout将其进行纵向排列
    QVBoxLayout *buttonLayout1 = new QVBoxLayout;
    buttonLayout1->addWidget(addButton,Qt::AlignTop);
    buttonLayout1->addWidget(submitButton);
    buttonLayout1->addWidget(cancelButton);
    buttonLayout1->addWidget(editButton);       // 编辑按钮
    buttonLayout1->addWidget(removeButton);     // 删除按钮
    buttonLayout1->addStretch();   // 此函数是用来确保按钮并不是采用均匀间隔排列的，而是更靠近widget的顶部
    // 使用QHBoxLayout将nextButton 按钮和 previousButton 按钮横向放置
    QHBoxLayout *buttonLayout2 = new QHBoxLayout;
    buttonLayout2->addWidget(previousButton);
    buttonLayout2->addWidget(nextButton);

    // 我们使用QGridLayout以结构化的方式放置标签和输入字段
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel,0,0);
    mainLayout->addWidget(nameLine,0,1);
    mainLayout->addWidget(addressLabel,1,0,Qt::AlignTop);
    mainLayout->addWidget(addressText,1,1);
    mainLayout->addLayout(buttonLayout1,1,2);  // 竖排按钮位置
    mainLayout->addLayout(buttonLayout2,3,1);  // 横排按钮位置


    // 在widget上安装布局对象，必须调用widget的setLayout()函数
    setLayout(mainLayout);
    setWindowTitle(QString("Simple Address Book"));
}

AddressBook::~AddressBook()
{

}
