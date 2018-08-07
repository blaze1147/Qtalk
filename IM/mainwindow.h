#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <registerwindow.h>
#include <QStackedWidget>
#include <QXmppClient.h>
#include <QString>
#include <QDebug>
#include <QXmppLogger.h>
#include "QXmppMessage.h"
#include "QXmppRosterManager.h"
#include <QInputDialog>
#include <qmylistwidget.h>
#include <chatwindow.h>
#include <QXmppPresence.h>
#include <string.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QStackedWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QMap<QString, ChatWindow*> m_chatDlgsList;  //定义一个jid与聊天窗口对应的map
    ChatWindow* getChatDialog(const QString& bareJid);
    void contactListClean();
    QString getCurrentPresence(QXmppPresence contactPresence);

private slots:
    void on_login_button_clicked();
    void on_register_button_clicked();
    void connectSucceed();
    void connectfailed();
    void on_addContact_clicked();
    void subscriptionReceive(QString senderName);
    void refreshContactList();
    void presenceReceived(QXmppPresence presence);
    void showChatWindow();
    void delContact();
    void on_exitButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void closeEvent(QCloseEvent *event);

    void messageReceived(const QXmppMessage& msg);

    void filterChanged(QString filter);
    void on_quitButton_clicked();

private:
    Ui::MainWindow *ui;
    QString loginUsername,loginDomain,loginJid;
    QXmppClient client;
};

#endif // MAINWINDOW_H
