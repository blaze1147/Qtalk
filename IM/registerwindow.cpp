#include "registerwindow.h"
#include "ui_registerwindow.h"

RegisterWindow::RegisterWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QObject::tr("注册"));
    registerClient.connectToServer("admin@10.18.59.42","1234");
    registerClient.configuration().setUseSASLAuthentication(false);
    registerClient.configuration().setUseNonSASLAuthentication(false);
    ui->registerPasswordEdit->setEchoMode(QLineEdit::Password);
    ui->registerPasswordConfirm->setEchoMode(QLineEdit::Password);
    ui->registerDomainEdit->setText("10.18.59.42");
    connect(&registerClient,SIGNAL(iqReceived(QXmppIq)),this,SLOT(iqReceived(QXmppIq)));
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::on_pushButton_clicked()
{
    //新用户注册
    registerUserName = ui->registerUsernameEdit->text();        //获取用户输入
    registerPassword = ui->registerPasswordEdit->text();
    passwordConfirm = ui->registerPasswordConfirm->text();
    registerDoamin = ui->registerDomainEdit->text();
    if(registerUserName == NULL)        //判断输入是否为空
    {
        ui->statusLabel->setText("请输入用户名!");
    }
    else if(registerPassword == NULL)
    {
        ui->statusLabel->setText("请输入密码！");
    }
    else if(registerPassword != passwordConfirm)
    {
        ui->statusLabel->setText("您输入的两次密码不一致！");
    }
    else if(registerDoamin == NULL)
    {
        ui->statusLabel->setText("请输入域名!");
    }
    else
    {
        QXmppRegisterIq registerIq;
        registerIq.setType(QXmppRegisterIq::Set);
        registerIq.setUsername(registerUserName);
        registerIq.setPassword(registerPassword);
        registerIq.setTo(registerDoamin);
        registerClient.sendPacket(registerIq);  //发送注册iq消息
    }
}

void RegisterWindow::on_pushButton_2_clicked()
{
    close();
}

void RegisterWindow::iqReceived(QXmppIq recIq)
{
    if(recIq.error().type() == QXmppIq::Error::Modify)  //根据服务器返回的iq消息类型判断注册是否成功
    {
        QMessageBox::critical(NULL,"错误","注册失败！",QMessageBox::Ok);
    }
    if(recIq.type() == QXmppIq::Result)
    {
        QMessageBox::information(NULL,"提示","注册成功！",QMessageBox::Ok);
        registerClient.disconnectFromServer();
        close();
    }
}
