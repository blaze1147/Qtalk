#include "chatwindow.h"
#include "ui_chatwindow.h"

ChatWindow::ChatWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::setQXmppClient(QXmppClient* client)
{
    m_client = client;
}

void ChatWindow::setContactName(QString contactJid,QString userJid)
{
    m_contactJid = contactJid;
    m_userJid = userJid;
    ui->titleLabel->setText("chat with "+contactJid);
}

void ChatWindow::on_sendButton_clicked()
{
    QString msgBody = ui->msgEdit->toPlainText();
    if(m_client)
    {
        m_client->sendMessage(m_contactJid,msgBody);    //发送message
        ui->msgEdit->clear();
    }
    QListWidgetItem *item=new QListWidgetItem();    //向消息列表中添加自己发送的消息
    QIcon userIcon(":/image/userIcon.png");
    item->setIcon(userIcon);
    item->setText(m_userJid+":"+msgBody);
    ui->msgList->addItem(item);
}

void ChatWindow::on_closeButton_clicked()
{
    close();
}

void ChatWindow::messageReceived(QXmppMessage receivedMsg)
{
    QListWidgetItem *item=new QListWidgetItem();    //收到好友消息，向消息列表中添加对方的消息
    QIcon contactIcon(":/image/contactIcon.png");
    item->setIcon(contactIcon);
    item->setText(m_contactJid+":"+receivedMsg.body());
    ui->msgList->addItem(item);
}
