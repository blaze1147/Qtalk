#include "registerdialog.h"
#include "ui_registerdialog.h"

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_pushButton_2_clicked()
{
    close();
}

void RegisterDialog::on_pushButton_clicked()
{
    //新用户注册

    QMessageBox::information(NULL,"提示","注册成功！",QMessageBox::Ok);
    close();
}
