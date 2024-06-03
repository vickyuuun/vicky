#ifndef DLG_ADDSTORE_H
#define DLG_ADDSTORE_H

#include <QDialog>
#include "storesql.h"

namespace Ui {
class Dlg_AddStore;
}

class Dlg_AddStore : public QDialog
{
    Q_OBJECT

public:
    explicit Dlg_AddStore(QWidget *parent = nullptr);
    ~Dlg_AddStore();
    //void settype(bool isadd,QString id="0");
    void setType(bool isadd,proinfo info={});

private slots:
    void on_baocun_clicked();

    void on_quxiao_clicked();

private:
    Ui::Dlg_AddStore *ui;
    bool m_isadd;
    //QString m_id;
    proinfo m_info;
};

#endif // DLG_ADDSTORE_H
