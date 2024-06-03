#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "page_log.h"

#include<QTableWidget>
#include<QKeyEvent>
#include<QFile>
#include<QMessageBox>
#include<QDir>
#include<QCoreApplication>
MainWindow::MainWindow(QWidget *parent)//
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_ptrstoresql(NULL)
{
    ui->setupUi(this);

    m_dlglogin.show();

   // connect(&m_dlglogin,&page_log::sendLoginSucess,this,f);
    connect(&m_dlglogin, &page_log::sendLoginSucess, this, &MainWindow::onLoginSuccess);

   // this->hide();
    ui->treeWidget->setColumnCount(1);//列
    QStringList l;
    l<<"商家信息";
    QTreeWidgetItem *pf=new QTreeWidgetItem(ui->treeWidget,l);
    ui->treeWidget->addTopLevelItem(pf);
    l.clear();
    l<<"基本信息";
    QTreeWidgetItem *p1=new QTreeWidgetItem(pf,l);
    l.clear();
    l<<"商品简介";
    QTreeWidgetItem *p2=new QTreeWidgetItem(pf,l);
    pf->addChild(p1);
    pf->addChild(p2);
  //  ui->treeWidget->addTopLevelItem(new QTreeWidgetItem(ui->treeWidget,l));

    ui->treeWidget->expandAll();
   // ui->stackedWidget->setCurrentIndex();
    m_ptrstoresql=storesql::getinstance();
    m_ptrstoresql->init();
   // qDebug()<<this->mainid;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)//css
{
    if(event->key()==Qt::Key_F6){
        QFile f;
        auto str=QCoreApplication::applicationDirPath();
        f.setFileName(str+QDir::separator()+"storeqss.css");
        f.open(QIODevice::ReadOnly);
        QString strQss=f.readAll();
        this->setStyleSheet(strQss);
        m_dlglogin.setStyleSheet(strQss);
    }

}

void MainWindow::onLoginSuccess(const QString &account)
{
    QMessageBox::information(this, "登录成功", "欢迎，" + account + "!");
        // 或者更新MainWindow的某个成员变量或UI元素
    mainname = account;


    qDebug()<<mainname;

    //QString sname=m_ptrstoresql->fstorname(mainname);
    //qDebug()<<sname;
    ui->store_name->setText(QString("商店名称：%1").arg(mainname));
    auto cnt=m_ptrstoresql->getProCnt(mainname);
    ui->k_cnt->setText(QString("类别数：%1").arg(cnt));

    this->show();
    updateTable();
}



// void MainWindow::on_simulation_clicked()
// {
//     //制作1个菜单
//         m_ptrstoresql->addPro();
//         updateTable();
// }




void MainWindow::on_btn_add_clicked()
{
    m_algaddstore.setType(true,{0});//import
    m_algaddstore.exec();
    updateTable();
//模态化
}



// void MainWindow::on_bu_clear_clicked()
// {
//     m_ptrstoresql->clearpro();
//     updateTable();
// }

void MainWindow::updateTable()
{
   // qDebug()<<cnt;
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(6);//have to!!!!!!

    QStringList l;
    l<<"序号"<<"商品编号"<<"商品名字"<<"价格"<<"存货数"<<"商家编号";
    //qDebug()<<l;
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不允许自行修改

    ui->tableWidget->setHorizontalHeaderLabels(l);

    qDebug()<<mainname;
    QList<proinfo> lproinfo= m_ptrstoresql->getAllpro(mainname);

   // QList<proinfo> lproinfo= m_ptrstoresql->getAllpro();
    ui->tableWidget->setRowCount(lproinfo.size());
    qDebug()<<lproinfo.size();
    for(int i = 0;i < lproinfo.size();i++){

      //  qDebug()<<lproinfo[i].proid;
        //important
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(i+1)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(lproinfo[i].proid));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(lproinfo[i].proname));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(lproinfo[i].proprice)));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(lproinfo[i].renum)));//把全变成varchar
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(lproinfo[i].storeid));
      //  qDebug()<<lproinfo[i].proname;
    }

    auto cnt=m_ptrstoresql->getProCnt(mainname);
    ui->k_cnt->setText(QString("类别数：%1").arg(cnt));

    auto cnprice=m_ptrstoresql->gettaotalprice(mainname);
    qDebug()<<cnprice;
    ui->pricetotal->setText(QString("营业额：%1").arg(cnprice));

}


void MainWindow::on_but_del_clicked()
{
    int i = ui->tableWidget->currentRow();
    if(i>=0){
        QString id= ui->tableWidget->item(i,1)->text();
        m_ptrstoresql->delPro(id);
        updateTable();
        QMessageBox::information(NULL,"信息","删除成功");

    }
}


void MainWindow::on_but_update_clicked()
{
    proinfo info;
    int i = ui->tableWidget->currentRow();
   // qDebug()<<i;
    if(i>=0){
        info.proid= ui->tableWidget->item(i,1)->text();
        info.proname= ui->tableWidget->item(i,2)->text();
        info.proprice= ui->tableWidget->item(i,3)->text().toFloat();
        info.renum= ui->tableWidget->item(i,4)->text().toUInt();
        info.storeid= ui->tableWidget->item(i,5)->text();


        m_algaddstore.setType(false,info);
        m_algaddstore.exec();
       // QMessageBox::information(NULL,"信息","修改成功");


    }


    updateTable();


}


void MainWindow::on_but_search_clicked()
{
    QString strsearch=ui->line_search->text();
    if(strsearch.isEmpty()){

        QMessageBox::information(NULL,"警告","未输入商品名字");
        updateTable();
        return;
    }
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(6);//have to!!!!!!

    QStringList l;
    l<<"序号"<<"商品编号"<<"商品名字"<<"价格"<<"存货数"<<"商家编号";
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不允许自行修改
    ui->tableWidget->setHorizontalHeaderLabels(l);

    QList<proinfo> lproinfo= m_ptrstoresql->gettruepro(strsearch,mainname);
    //QList<proinfo> lproinfo= m_ptrstoresql->getAllpro();
    ui->tableWidget->setRowCount(lproinfo.size());
    qDebug()<<lproinfo.size();
    int index=1;
    for(int i = 0;i < lproinfo.size();i++){
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(index+1)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(lproinfo[i].proid));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(lproinfo[i].proname));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(lproinfo[i].proprice)));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(lproinfo[i].renum)));//把全变成varchar
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(lproinfo[i].storeid));
    }
}


void MainWindow::on_exit_clicked()
{
    exit(0);
}







void MainWindow::on_ifchange_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("确认");
    msgBox.setText("你确定要执行这个操作吗？");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No); // 设置默认按钮为“否”
    int ret = msgBox.exec(); // 显示对话框并等待用户响应

    // 检查用户的响应
    if (ret == QMessageBox::Yes) {
         qDebug() << "用户点击了是，执行操作...";
        m_ptrstoresql->ifchange(mainname);
        updateTable();
        QMessageBox::information(NULL,"信息","清除成功");

    } else {
        // 用户点击了“否”或关闭了对话框，不执行操作
        qDebug() << "用户点击了否或取消了操作";
    }





}

