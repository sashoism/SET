#ifndef IPDIALOG_H
#define IPDIALOG_H

#include <QWidget>

namespace Ui {
    class IPDialog;
}

class IPDialog : public QWidget {
    Q_OBJECT
public:
    explicit IPDialog(QWidget* parent = 0);
    ~IPDialog();
    bool eventFilter(QObject*, QEvent*);
    bool ipReady();
private:
    Ui::IPDialog* ui;
private slots:
    void on_pushButton_clicked();
    void on_lineEdit_2_textChanged(const QString &arg1);
    void on_lineEdit_3_textChanged(const QString &arg1);
    void on_lineEdit_4_textChanged(const QString &arg1);
    void on_lineEdit_5_textChanged(const QString &arg1);
    void on_lineEdit_5_returnPressed();
};

#endif // IPDIALOG_H
