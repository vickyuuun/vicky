#include "storesql.h"
#include "mainwindow.h"
#include "page_log.h"
#include<QMessageBox>
#include <QSqlError>
//#include<QSql>
#include<QSqlDatabase>
#include <QDebug>
#include<QSqlQuery>

storesql *storesql::ptrstoresql=NULL;
storesql::storesql(QObject *parent)
    : QObject{parent}
    //,m_dlglogin(NULL)

    //, m_ptrstoresql(NULL)
{
    init();
    //clearrecord();
}

void storesql::init()
{
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
        qDebug()<<"error open database because"<<m_db.lastError().text();
    }


}

quint32 storesql::getProCnt(QString name)
{
    QSqlQuery sql(m_db);
    sql.exec(QString("select count(商品编号) from 商品 where 商家号 =(select 商家号 from 店家 where 店家.店铺名='%1');").arg(name));
    quint32 countni=0;
    while(sql.next()){

        countni=sql.value(0).toUInt();
        //++;
    }

   // qDebug()<<countni;
    return countni;
}

quint32 storesql::gettaotalprice(QString name)
{
    QSqlQuery sql(m_db);
    qDebug()<<sql.exec(QString("select 营业额 from 店家 where 商家号 =(select 商家号 from 店家 where 店家.店铺名='%1');").arg(name));

    quint32 countni=10;
    while(sql.next()){
        countni=sql.value(0).toUInt();
        qDebug()<<sql.value(0).toUInt();
        //++;
    }

    qDebug()<<countni;
    return countni;


}

bool storesql::delPro(QString proid)
{

    QSqlQuery sql(m_db);


    //return sql.exec(QString("delete from 商品 where 商家号=(select `商家号` from 店家 where 商品.`商家号`=店家.`商家号` and 店家.`店铺名`='%1')").arg(proname));
     return sql.exec(QString("delete from 商品 where 商品编号=%1").arg(proid));

}



void storesql::addPro()
{

    QSqlQuery sql(m_db);
    sql.exec("insert into 商品(商品编号,商品名字,价格,存货数,商家号) values('0002','特色剪发','10','1200','0003')");

}

void storesql::addPro(proinfo info)
{
     QSqlQuery sql(m_db);
    QString strSql=QString("insert into 商品(商品编号,商品名字,价格,存货数,商家号) values('%1','%2','%3','%4','%5');").
                     arg(info.proid).
                     arg(info.proname).
                     arg(info.proprice).
                     arg(info.renum).
                     arg(info.storeid);
   sql.exec(strSql);

    // QString strsql=QString("DROP TRIGGER IF EXISTS a1;  CREATE TRIGGER a1 AFTER INSERT ON 商品  FOR EACH ROW   BEGIN   INSERT INTO 商品(商品编号,商品名字,价格,存货数,商家号) values('%1','%2','%3','%4','%5');").
    //                arg(info.proid).
    //                arg(info.proname).
    //                arg(info.proprice).
    //                arg(info.renum).
    //                arg(info.storeid);

    //  qDebug()<<sql.exec(strsql);

}
//id
bool storesql::updateProInfo(proinfo info)
{
     QSqlQuery sql(m_db);

    qDebug()<<info.proname<<info.proprice<<info.renum;

    QString strSql=QString("UPDATE 商品 AS t1"
                            " LEFT JOIN ("
                            "  SELECT 商家号, 商品名字 "
                            " FROM 商品  "
                            "WHERE 商家号 = '%5' AND 商品名字 = '%1' AND 商品编号 <> '%4'  LIMIT 1  )"
                            " AS t2 ON 1 = 1   "
                            "SET t1.商品名字 = '%1', t1.价格 = '%2', t1.存货数 = '%3' "
                            " WHERE t1.商品编号 = '%4' AND t2.商家号 IS NULL and '%2'>=0  ;").
                           arg(info.proname).
                           arg(info.proprice).
                           arg(info.renum).
                           arg(info.proid).
                           arg(info.storeid);


    //bool ret = sql.exec( QString("update 商品 set 商品.价格=110 where `商品编号`='0003';"));

    bool ret =sql.exec(strSql);
    QSqlError e=sql.lastError();
    if(e.isValid()){
       qDebug()<<e.text();
    }

    sql.exec(QString("DELIMITER $ "
"DROP TRIGGER IF EXISTS emp_update;"
"CREATE TRIGGER emp_update"
"AFTER UPDATE ON 商品"
"FOR EACH ROW"
"BEGIN"
"IF (old.价格 != new.价格 and old.价格>=0 and new.价格>=0) THEN"
   "INSERT INTO 商品价格记录表 ( 商家号, old, new, uptime,商品编号)"
   "VALUES(new.商家号, old.价格, new.价格, NOW(),new.`商品编号`);	"
   "END IF;"
"END$"
 "DELIMITER ;"));



    // qDebug()<<ret;
    return ret;

}

QList<proinfo> storesql::getAllpro(QString name)//有问题
{

    QList<proinfo> l;
    QSqlQuery sql(m_db);

    // sql.exec(QString("select * from 商品  where 商家号 in (select 商家号 from 店家 where 店家.商家号=商品.商家号 and 店家.店铺名='老徐烧烤';"));
    qDebug()<<sql.exec(QString("select *"
                                 " from 商品"
                                 "  where 商家号=(select 商家号 "
                                 "from 店家"
                                 " where 店家.商家号=商品.商家号 and 店家.店铺名='%1');").arg(name));

    proinfo info;
    while(sql.next()){

        info.proid=sql.value(0).toString();
        info.proname=sql.value(1).toString();
        info.proprice=sql.value(2).toFloat();
        info.renum=sql.value(3).toInt();
        info.storeid=sql.value(4).toString();
        l.push_back(info);
    }
    return l;

}

QList<proinfo> storesql::gettruepro(QString specname,QString mainname)
{
    QList<proinfo> l;
    QSqlQuery sql(m_db);

    // sql.exec(QString("select * from 商品  where 商家号 in (select 商家号 from 店家 where 店家.商家号=商品.商家号 and 店家.店铺名='老徐烧烤';"));
    qDebug()<<sql.exec(QString("select *"
                                 " from 商品"
                                 "  where 商品.商品名字='%1' and 商家号=(select 商家号 "
                                 "from 店家"
                                 " where 店家.店铺名='%2');").
                                 arg(specname).
                                 arg(mainname)
                                );

    proinfo info;
    while(sql.next()){

        info.proid=sql.value(0).toString();
        info.proname=sql.value(1).toString();
        info.proprice=sql.value(2).toFloat();
        info.renum=sql.value(3).toUInt();
        info.storeid=sql.value(4).toString();
        l.push_back(info);
    }
    return l;


}



bool storesql::isExist(QString proid)
{
    QSqlQuery sql(m_db);

    sql.exec(QString("select * from 商品 where 商品编号='%1'").arg(proid));

//    bool ret=false;
    return sql.next();
}

void storesql::adduser()
{
    QSqlQuery sql(m_db);
    sql.exec("insert into 店家(商家号,登陆密码,店铺名,地址,电话,营业额,状态) values('0003','0123','小袁美发店','理科组团','181234',200,'开业')");
}

bool storesql::deluser(QString storeid)
{
    QSqlQuery sql(m_db);
    return sql.exec(QString("delete from 店家 where 商家号=%1").arg(storeid));
}

// QString storesql::fstorname(QString storeid)
// {
//     QSqlQuery sql(m_db);
//     sql.exec(QString("select * from 店家 where 商家号='%1'").arg(storeid));

//     while(sql.next()){
//         //qDebug()<<sql.value(2).toString();
//         return sql.value(2).toString();
//     }
// }

// bool storesql::ifpass(QString ac, QString pas)
// {
//     QSqlQuery sql(m_db);
//     return sql.exec("select * from 店家 where 商家号=ac and 登录密码=pas");

// }



// void storesql::clearpro()
// {
//     QSqlQuery sql(m_db);
//     sql.exec("delete from 商品" );

// }

bool storesql::ifchange(QString sname)
{

    QSqlQuery sql(m_db);
    qDebug()<<"wwwwwwww";
    qDebug()<<sql.exec(QString("delete from 商品 where 商家号 in (select `商家号` from 店家 where 店家.`店铺名`='%1' and 店家.营业额< 20)").arg(sname));

    //return sql.exec(QString("delete from 商品 where 商家号=(select `商家号` from 店家 where 商品.`商家号`=店家.`商家号` and 店家.`店铺名`='%1')").arg(proname));
    return sql.exec(QString("delete from 商品 "
                            "where 商家号 in "
                            "(select `商家号` from 店家 where 店家.`店铺名`='%1' and 店家.营业额<1700)").arg(sname));

}

void storesql::clearrecord()
{

    QSqlQuery sql(m_db);
       sql.exec("delete from 商品价格记录表" );

}

void storesql::record()
{

}



