#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"storesql.h"
#include"page_log.h"
#include"dlg_addstore.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *event);

public slots:
    void onLoginSuccess(const QString &account);

private slots:
  //  void on_pushButton_6_clicked();

   // void on_simulation_clicked();



    void on_btn_add_clicked();




    // void on_bu_clear_clicked();

    void on_but_del_clicked();

    void on_but_update_clicked();

    void on_but_search_clicked();

    void on_exit_clicked();

    //bool storeifpass();



    void on_ifchange_clicked();

private:
    void updateTable();

private:
    Ui::MainWindow *ui;
    page_log m_dlglogin;

    storesql *m_ptrstoresql;
    Dlg_AddStore m_algaddstore;
    QString mainname;

};
#endif // MAINWINDOW_H
