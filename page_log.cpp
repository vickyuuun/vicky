#include "page_log.h"
#include "ui_page_log.h"
#include<QSqlQuery>
#include<QMessageBox>
#include"storesql.h"

page_log::page_log(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::page_log)
{
    ui->setupUi(this);

    m_db = QSqlDatabase::addDatabase("QODBC");
    m_db.setHostName("127.0.0.1");
    m_db.setPort(3306);
    m_db.setDatabaseName("mysql"); //不是Database,记得别填错
    m_db.setUserName("root");
    m_db.setPassword("root");
    bool ok = m_db.open();
    if (ok){
        qDebug()<<"数据库连接成功";
    }
    else {
        qDebug()<<"数据库连接失败";
        //qDebug()<<"error open database because"<<m_db.lastError().text();
    }



}

page_log::~page_log()
{
    delete ui;
}



void page_log::on_button_login_clicked()
{
//数据库查找用户账号与密码
   // QStringList l;
    llog<< ui->store_pass->text()<<ui->store_user->text();
    QSqlQuery sql(m_db);
   // qDebug()<<llog[0];

    QString query = QString("SELECT * FROM 店家 WHERE 店铺名='%1' AND 登陆密码='%2'").arg(llog[1], llog[0]);
   // qDebug()<<sql.exec(query);
    if(sql.exec(query)){

        if(sql.next()){ // 通常你需要检查记录是否存在，但这里假设只有一个结果
             emit sendLoginSucess(llog[1]);
        }

        else{
           QMessageBox::information(NULL,"警告","登录失败");
        }
    }

    //如果失败提示；

//成功进入；

}


void page_log::on_button_exit_clicked()
{
    //退出商家登录界面
    exit(0);
}







void page_log::on_tatal_exit_clicked()
{


}

