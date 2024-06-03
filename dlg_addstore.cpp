#include "dlg_addstore.h"
#include "ui_dlg_addstore.h"
#include"storesql.h"
#include<QMessageBox>
Dlg_AddStore::Dlg_AddStore(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dlg_AddStore)
{
    ui->setupUi(this);
}

Dlg_AddStore::~Dlg_AddStore()
{
    delete ui;
}



void Dlg_AddStore::setType(bool isadd, proinfo info)
{
    m_isadd=isadd;
    m_info=info;
    ui->le_id->setText(info.proid);
    ui->le_name->setText(info.proname);
    ui->le_storeid->setText(info.storeid);
    ui->price->setValue(info.proprice);
    ui->remain->setValue(info.renum);
    //qDebug()<<"hhhhhhhhhh";


}

//保存
void Dlg_AddStore::on_baocun_clicked()
{
    proinfo info;
    auto ptr = storesql::getinstance();
    info.proid=ui->le_id->text();
    info.proname=ui->le_name->text();
    info.proprice=ui->price->text().toFloat();
    info.renum=ui->remain->text().toInt();
    info.storeid=ui->le_storeid->text();
    if(m_isadd){
        ptr->addPro(info);
        QMessageBox::information(NULL,"信息","存储成功");
    }
    else{
       // info.proid=m_id;
        bool ret=ptr->updateProInfo(info);
       if(ret==false)
            QMessageBox::information(NULL,"警告","商品修改失败");
    }




    this->hide();
}


void Dlg_AddStore::on_quxiao_clicked()
{
    this->hide();
}

