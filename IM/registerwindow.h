#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QXmppRegisterIq.h>
#include <QXmppClient.h>

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = 0);
    ~RegisterWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void iqReceived(QXmppIq recIq);
private:
    Ui::RegisterWindow *ui;
    QXmppClient registerClient;
    QString registerUserName,registerPassword,passwordConfirm,registerDoamin;
};

#endif // REGISTERWINDOW_H
