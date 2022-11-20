#include "Client.h"
#include "IPDialog.h"
#include "ui_IPDialog.h"
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QRegExpValidator>

IPDialog::IPDialog(QWidget* parent) : QWidget(parent), ui(new Ui::IPDialog) {
    ui->setupUi(this);
    #ifndef Q_OS_ANDROID
        int height = QApplication::desktop()->height() / 7.5; if (height < 120) height = 120;
        setMinimumWidth(1.66 * height); setMaximumWidth(minimumWidth());
    #endif
    // QR scanner on android?
    ui->horizontalWidget->setMinimumHeight(ui->lineEdit_2->sizeHint().height() * 1.5);
    ui->horizontalWidget->setMaximumHeight(ui->horizontalWidget->minimumHeight());
    setWindowFlags(Qt::WindowStaysOnTopHint);
    QRegExp regex("^(0|[1-9][0-9]{0,2})$");
    QValidator* v = new QRegExpValidator(regex, this);
    QLineEdit* tmp; for (int i = 0; i < ui->horizontalLayout->count(); i += 2) {
        tmp = static_cast<QLineEdit*>(ui->horizontalLayout->itemAt(i)->widget());
        tmp->setValidator(v);
        if (i > 0) ui->horizontalLayout->itemAt(i - 1)->widget()->setMaximumHeight(tmp->sizeHint().height());
        tmp->installEventFilter(this);
    }
}

bool IPDialog::eventFilter(QObject* object, QEvent* event) {
    if (event->type() == QEvent::FocusIn) {
        QApplication::inputMethod()->show();
    }
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* e = static_cast<QKeyEvent*>(event); QWidget* tmp;
        if (e->key() == Qt::Key_Space || e->key() == Qt::Key_Period || e->key() == Qt::Key_Right) {
            for (int i = 0; i < ui->horizontalLayout->count() - 1; i += 2) {
                tmp = ui->horizontalLayout->itemAt(i)->widget();
                if (object == tmp) ui->horizontalLayout->itemAt(i + 2)->widget()->setFocus();
            }
        }
        if (e->key() == Qt::Key_Backspace || e->key() == Qt::Key_Left) {
            for (int i = 2; i < ui->horizontalLayout->count(); i += 2) {
                tmp = ui->horizontalLayout->itemAt(i)->widget();
                if (object == tmp && !static_cast<QLineEdit*>(tmp)->cursorPosition()) ui->horizontalLayout->itemAt(i - 2)->widget()->setFocus();
            }
        }
    }
    return QObject::eventFilter(object, event);
}

IPDialog::~IPDialog() {
    delete ui;
}

bool IPDialog::ipReady() {
    for (int i = 0; i < ui->horizontalLayout->count(); i += 2) {
        if (static_cast<QLineEdit*>(ui->horizontalLayout->itemAt(i)->widget())->text().isEmpty()) return false;
    }
    return true;
}

void IPDialog::on_pushButton_clicked() {
    QStringList values; for (int i = 0; i < ui->horizontalLayout->count(); i += 2) {
        values << static_cast<QLineEdit*>(ui->horizontalLayout->itemAt(i)->widget())->text();
    }
    Client::getInstance()->start(values.join('.'), SERVER_PORT);
    hide();
    deleteLater();
}

void IPDialog::on_lineEdit_2_textChanged(const QString &arg1) {
    if (arg1.toInt() > 223) ui->lineEdit_2->setText("223");
    if (ui->lineEdit_2->text().size() == 3 || arg1 == "0") ui->lineEdit_3->setFocus();
    ui->pushButton->setEnabled(ipReady());
}

void IPDialog::on_lineEdit_3_textChanged(const QString &arg1) {
    if (arg1.toInt() > 255) ui->lineEdit_3->setText("255");
    if (ui->lineEdit_3->text().size() == 3 || arg1 == "0") ui->lineEdit_4->setFocus();
    ui->pushButton->setEnabled(ipReady());
}

void IPDialog::on_lineEdit_4_textChanged(const QString &arg1) {
    if (arg1.toInt() > 255) ui->lineEdit_4->setText("255");
    if (ui->lineEdit_4->text().size() == 3 || arg1 == "0") ui->lineEdit_5->setFocus();
    ui->pushButton->setEnabled(ipReady());
}

void IPDialog::on_lineEdit_5_textChanged(const QString &arg1) {
    if (arg1.toInt() > 255) ui->lineEdit_5->setText("255");
    ui->pushButton->setEnabled(ipReady());
}

void IPDialog::on_lineEdit_5_returnPressed() {
    on_pushButton_clicked();
}
