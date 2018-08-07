#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QXmppClient.h>
#include <QXmppMessage.h>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();
    //QString username,contactname;
    void setQXmppClient(QXmppClient* client);
    void setContactName(QString contactName,QString userName);
    QXmppClient* m_client;
    QString m_contactJid;
    QString m_userJid;
    void messageReceived(QXmppMessage receivedMsg);

private slots:
    void on_sendButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::ChatWindow *ui;
};

#endif // CHATWINDOW_H
