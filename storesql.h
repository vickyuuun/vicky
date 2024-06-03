#ifndef STORESQL_H
#define STORESQL_H
#include<QSqlDatabase>
#include <QObject>
struct storeinfo{

    QString storeid;
    QString storepass;
    QString storename;
    QString address;
    QString telp;
    int totalprice;
    QString condi;

};

struct proinfo{

    QString proid;
  //  QString storepass;
    QString proname;
    float proprice;
    int renum;
    QString storeid;
    //QString condi;

};



class storesql : public QObject
{
    Q_OBJECT
public:
    static storesql *ptrstoresql;
    static  storesql *getinstance(){
        if(ptrstoresql==NULL){

            ptrstoresql=new storesql;
        }

        return ptrstoresql;

    }

    explicit storesql(QObject *parent = nullptr);

    void init();
    //查询所有商家数量
    quint32 getStoreCnt();


    //查询所有商品数量
    quint32 getProCnt(QString name);

    quint32 gettaotalprice(QString name);


    //删除商品
    bool delPro(QString proname);

    //添加商品
    void addPro();

    void addPro(proinfo info);

    //修改商品信息
    bool updateProInfo(proinfo info);

    //查询所有商品
    QList<proinfo> getAllpro(QString id);

     QList<proinfo> gettruepro(QString specname,QString mainname);
   // QList<proinfo> getAllpro();
    //商品是否存在
   bool isExist(QString proid);

    //添加单个用户
    void adduser();

    //删除单个用户
    bool deluser(QString storeid);


    //查询商家名称
//    QString fstorname(QString storeid);

    // bool ifpass(QString ac,QString pas);

    //清空商品表
    //void clearpro();

    //是否调整
    bool ifchange(QString sname);

    void clearrecord();

    void record();


signals:

    private:
    QSqlDatabase m_db;


};

#endif // STORESQL_H
