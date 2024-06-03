#ifndef PAGE_LOG_H
#define PAGE_LOG_H

#include"storesql.h"
#include <QWidget>

namespace Ui {
class page_log;
}

class page_log : public QWidget
{
    Q_OBJECT

public:
    explicit page_log(QWidget *parent = nullptr);
    ~page_log();




    QStringList llog;

private slots:
    void on_button_login_clicked();

    void on_button_exit_clicked();





    void on_tatal_exit_clicked();

signals:
    void sendLoginSucess(const QString &account);

private:
    Ui::page_log *ui;
    QSqlDatabase m_db;
};

#endif // PAGE_LOG_H
