#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QStandardItemModel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
public slots:
    void received_lobbyData(QByteArray);
    void setName_response(QByteArray);
    void joinGame_response(QByteArray);
    void addGame_response(QByteArray);
    void pGameOver_response();
private:
    Ui::MainWindow* ui;
    QStandardItemModel* model;
    bool isAndroid;
private slots:
    void on_lineEdit_textChanged(QString);
    void on_lineEdit_returnPressed();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // MAINWINDOW_H
