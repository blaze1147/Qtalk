#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCurrentIndex(0);
    this->setWindowTitle(QObject::tr("Qtalk"));
    ui->passwordEdit->setEchoMode(QLineEdit::Password);     //将密码输入框设置为密码模式
    ui->domainEdit->setText("10.18.59.42");     //设置域名输入框的默认值
    ui->userIcon->setFixedSize(70,70);
    ui->filterIcon->setFixedSize(30,30);
    //client.logger()->setLoggingType(QXmppLogger::StdoutLogging); //在应用程序输出中显示log信息

    //连接信号与槽
    connect(&client,SIGNAL(connected()),this,SLOT(connectSucceed()));       //连接服务器失败的槽

    connect(&client,SIGNAL(disconnected()),this,SLOT(connectfailed()));     //连接服务器成功的槽

    connect(&client.rosterManager(),SIGNAL(rosterReceived()),this,SLOT(refreshContactList()));  //连接服务器后接收到roster的槽

    connect(&client.rosterManager(),SIGNAL(subscriptionReceived(QString)),  //接收到订阅消息（好友请求)的槽
                this,SLOT(subscriptionReceive(QString)));

    connect(&client,SIGNAL(presenceReceived(QXmppPresence)),    //接收到presence消息的槽
                this,SLOT(presenceReceived(QXmppPresence)));

    connect(ui->contactList->m_msgAction,SIGNAL(triggered()),this,SLOT(showChatWindow()));      //选择右键菜单中发送消息选项的槽

    connect(ui->contactList,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(showChatWindow()));    //在好友列表中双击列表项的槽

    connect(ui->contactList->m_delAction,SIGNAL(triggered()),this,SLOT(delContact()));  //选择右键菜单中删除好友选项的槽

    connect(&client,SIGNAL(messageReceived(QXmppMessage)),this,SLOT(messageReceived(QXmppMessage)));   //接受到message消息时的槽

    connect(ui->filterEdit,SIGNAL(textChanged(QString)),this,SLOT(filterChanged(QString)));     //好友过滤
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_register_button_clicked()   //点击注册按钮对应的槽
{
    RegisterWindow *registration = new RegisterWindow(this);
    registration->setModal(true);   //设置为模态对话框
    registration->show();
}

void MainWindow::on_login_button_clicked()      //点击登录按钮对应的槽
{
    QString loginPassword;
    loginUsername = ui->usernameEdit->text();   //获取用户输入
    loginDomain = ui->domainEdit->text();
    loginPassword = ui->passwordEdit->text();
    if(loginUsername == NULL)   //判断输入框是否为空
    {
        ui->status_label->setText("请输入账号！");
    }
    else if(loginPassword == NULL)
    {
        ui->status_label->setText("请输入密码！");
    }
    else if(loginDomain == NULL)
    {
        ui->status_label->setText("请输入域名!");
    }
    else
    {
        loginJid = loginUsername+"@"+loginDomain; //将用户名与域名组合成JID
        client.connectToServer(loginJid,loginPassword); //连接服务器
        ui->comboBox->setCurrentIndex(0);
    }
}

void MainWindow::connectSucceed()
{
    ui->status_label->setText("连接成功!");
    ui->userName->setText(loginJid);
    this->setCurrentIndex(1);   //进入主界面
}

void MainWindow::connectfailed()
{
    ui->status_label->setText("连接失败！");
}

void MainWindow::contactListClean()     //清空联系人列表
{
    int counter =ui->contactList->count();
    for(int index=0;index<counter;index++)
    {
        QListWidgetItem *item = ui->contactList->takeItem(0);
        delete item;
    }
}

QString MainWindow::getCurrentPresence(QXmppPresence contactPresence)   //将presence消息中的在线状态转换为字符输出
{
    QString currentPresence;
    if(contactPresence.type() == QXmppPresence::Unavailable)
    {
        currentPresence = "离线";
    }
    if(contactPresence.type() == QXmppPresence::Available)
    {
        if(contactPresence.availableStatusType() == QXmppPresence::AvailableStatusType::Online)
        {
            currentPresence = "在线";
        }
        else if(contactPresence.availableStatusType() == QXmppPresence::AvailableStatusType::DND)
        {
            currentPresence = "忙碌";
        }
        else if(contactPresence.availableStatusType() == QXmppPresence::AvailableStatusType::Away)
        {
            currentPresence = "离开";
        }
    }
    return currentPresence;
}

void MainWindow::refreshContactList()   //刷新好友列表
{
    contactListClean();
    foreach (const QString &bareJid, client.rosterManager().getRosterBareJids())
    {
        QString currentPresence = getCurrentPresence(client.rosterManager().getPresence(bareJid,"QXmpp"));  //获取登录状态
        QListWidgetItem *item=new QListWidgetItem();
        QIcon contactIcon(":/image/contactIcon.png");
        item->setIcon(contactIcon);
        item->setText(bareJid+"\t"+currentPresence);
        ui->contactList->addItem(item);
    }
}

void MainWindow::on_addContact_clicked()    //点击添加好友按钮对应的槽
{
    bool isOK;
    QString contactName = QInputDialog::getText(this,tr("添加好友"),
                                                tr("请输入好友账号"),QLineEdit::Normal,"",&isOK);  //弹出对话框
    QString contactJid = contactName+"@"+loginDomain;
    if(isOK && !client.rosterManager().getRosterBareJids().contains(contactJid) && contactName != loginUsername)
    {
        client.rosterManager().subscribe(contactJid);   //当用户点击OK时发送订阅请求
    }
    else if(contactName == loginUsername)
    {
        QMessageBox::warning(this,"提示","禁止添加自己！",QMessageBox::Ok);
        return;
    }
    else
    {
        QMessageBox::warning(this,"提示","您已添加过该好友！",QMessageBox::Ok);
        return;
    }
}

void MainWindow::subscriptionReceive(QString senderName)
{
    if(client.rosterManager().getRosterBareJids().contains(senderName))
    {
        client.rosterManager().acceptSubscription(senderName);  //当roster中包含发出订阅请求的用户时，自动同意订阅时订阅状态为both
    }
    else
    {
        QMessageBox::StandardButton isYes = QMessageBox::information(this,"好友请求",senderName+"请求添加您为好友",
                                                                     QMessageBox::Yes|QMessageBox::No);
        if(isYes == QMessageBox::Yes)
        {
            client.rosterManager().acceptSubscription(senderName);  //当点击Yes按钮时同意对方的订阅请求，并向对方发送订阅
            client.rosterManager().subscribe(senderName);
            QListWidgetItem *item=new QListWidgetItem();
            QIcon contactIcon(":/image/contactIcon.png");
            item->setIcon(contactIcon);
            item->setText(senderName);
            ui->contactList->addItem(item);
        }
        else
        {
            client.rosterManager().refuseSubscription(senderName);  //不是点击Yes按钮则拒绝订阅
        }
    }
}

void MainWindow::presenceReceived(QXmppPresence presence)
{
    if(presence.type() == QXmppPresence::Unsubscribed)
    {
        client.rosterManager().unsubscribe(presence.from());    //收到取消订阅的presence消息则也取消对对方的订阅
        client.rosterManager().removeItem(presence.from());
    }
    refreshContactList();   //刷新好友列表
}

void MainWindow::delContact()
{
    QString contactName = ui->contactList->currentItem()->text().split("\t")[0];

    QMessageBox::StandardButton isYes = QMessageBox::warning(this,"提示","您要删除好友"+contactName+"吗？",
                                            QMessageBox::Yes|QMessageBox::No);
    if(isYes == QMessageBox::Yes)
    {
        client.rosterManager().unsubscribe(contactName);
        client.rosterManager().removeItem(contactName);
        delete ui->contactList->currentItem();
    }
}

void MainWindow::on_exitButton_clicked()
{
    exit(0);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    QXmppPresence selectedPresence;     //选择不同登录状态则发送对应presence消息
    switch (index) {
    case 0:
        selectedPresence.setAvailableStatusType(QXmppPresence::AvailableStatusType::Online);
        client.setClientPresence(QXmppPresence::Available);
        break;
    case 1:
        selectedPresence.setAvailableStatusType(QXmppPresence::AvailableStatusType::DND);
        client.setClientPresence(selectedPresence);
        break;
    case 2:
        selectedPresence.setAvailableStatusType(QXmppPresence::AvailableStatusType::Away);
        client.setClientPresence(selectedPresence);
        break;
    case 3:
        client.setClientPresence(QXmppPresence::Unavailable);
        ui->status_label->setText("");
        ui->filterEdit->setText("");
        setCurrentIndex(0);
        break;
    default:
        break;
    }
}

ChatWindow* MainWindow::getChatDialog(const QString& bareJid)
{
    if(!m_chatDlgsList.contains(bareJid))   //如果不存在Jid对应的聊天对话框则新建一个
    {
        m_chatDlgsList[bareJid] = new ChatWindow();
        m_chatDlgsList[bareJid]->setWindowTitle("chat with "+bareJid);
        m_chatDlgsList[bareJid]->setContactName(bareJid,loginJid);
        m_chatDlgsList[bareJid]->setQXmppClient(&client);
    }
    return m_chatDlgsList[bareJid];
}

void MainWindow::showChatWindow()
{
    QString bareJid = ui->contactList->currentItem()->text().split("\t")[0];
    if(!bareJid.isEmpty())
        getChatDialog(bareJid)->show();
}

void MainWindow::messageReceived(const QXmppMessage& msg)
{
    if (msg.body().isEmpty())
        return;

    ChatWindow *dialog = getChatDialog(msg.from().split("/")[0]);   //从from（）中分离出Jid并获取对应的聊天对话框
    if (dialog) {
        dialog->show();
        dialog->messageReceived(msg);
    }
}

void MainWindow::filterChanged(QString filter)
{
    contactListClean();
    foreach (const QString &bareJid, client.rosterManager().getRosterBareJids())
    {
        if(bareJid.split("@")[0].contains(filter))      //将满足过滤条件的联系人显示出来
        {
            QString currentPresence = getCurrentPresence(client.rosterManager().getPresence(bareJid,"QXmpp"));
            QListWidgetItem *item=new QListWidgetItem();
            QIcon contactIcon(":/image/contactIcon.png");
            item->setIcon(contactIcon);
            item->setText(bareJid+"\t"+currentPresence);
            ui->contactList->addItem(item);
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)     //当关闭程序时断开client与服务器的连接
{
    client.disconnectFromServer();
}

void MainWindow::on_quitButton_clicked()    //点击退出按钮的槽
{
    QMessageBox::StandardButton isYes = QMessageBox::warning(this,"提示","您要退出程序吗？",
                                            QMessageBox::Yes|QMessageBox::No);
    if(isYes == QMessageBox::Yes)
    {
        exit(0);
    }
}
