#include "mainwindow.h"
//#include"page_log.h"
#include <QApplication>
#include<QApplication>
#include"storesql.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
 //   w.show();

   // page_log p;
    //p.show();

   // connect(p,&page_log::sendLoginSucess(),this,)

    storesql sql;


    return a.exec();
}
