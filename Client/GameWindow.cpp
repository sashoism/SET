#include "Client.h"
#include "GameWindow.h"
#include "ui_GameWindow.h"
#include <QDesktopWidget>
#include <QGraphicsOpacityEffect>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QScreen>
#include "QLabel2.h"
#include "packets/C_GAMECARDS.h"
#include "packets/C_FOUNDSET.h"
#include "packets/C_DONESELECTING.h"
#include "packets/C_MESSAGE.h"
#include "packets/C_GAMEOVER.h"

GameWindow::GameWindow(QWidget *parent) : QWidget(parent), ui(new Ui::GameWindow) {
    ui->setupUi(this);
    setWindowState(Qt::WindowFullScreen);
    QRect screen = QApplication::screens().at(0)->geometry();
    int height = screen.height();
    #ifdef Q_OS_ANDROID
        height = screen.width() * 1.21;
    #endif
    ui->gridLayout->setSpacing(height / 60);
    for (int row = 0; row < 3; row ++) {
        ui->gridLayout->setRowMinimumHeight(row, 0.25 * height);
        for (int column = 0; column < 4; column ++) {
            ui->gridLayout->setColumnMinimumWidth(column, 0.166 * height);
            ui->gridLayout->addWidget(new QLabel2(this, row, column), row, column);
            ui->gridLayout->itemAtPosition(row, column)->widget()->installEventFilter(this);
        }
    }
    ui->gridLayoutWidget->setGeometry(QRect((screen.width() - ui->gridLayout->minimumSize().width()) / 2, (screen.height() - ui->gridLayout->minimumSize().height()) / 2, ui->gridLayout->minimumSize().width(), ui->gridLayout->minimumSize().height()));
    ui->pushButton_2->setGeometry(ui->gridLayoutWidget->x(), ui->gridLayoutWidget->y() + ui->gridLayoutWidget->height() + height / 60, ui->gridLayoutWidget->width(), height / 15);
    ui->verticalLayoutWidget->setGeometry(ui->gridLayoutWidget->x(), ui->gridLayoutWidget->y() - (height / 60 + ui->pushButton_2->height()), ui->pushButton_2->width(), ui->pushButton_2->height());
    Client::getInstance()->runnables[4] = new C_GAMECARDS(); Client::getInstance()->runnables[4]->setAutoDelete(false);
    Client::getInstance()->runnables[5] = new C_FOUNDSET(); Client::getInstance()->runnables[5]->setAutoDelete(false);
    Client::getInstance()->runnables[6] = new C_DONESELECTING(); Client::getInstance()->runnables[6]->setAutoDelete(false);
    Client::getInstance()->runnables[7] = new C_MESSAGE(); Client::getInstance()->runnables[7]->setAutoDelete(false);
    Client::getInstance()->runnables[8] = new C_GAMEOVER(); Client::getInstance()->runnables[8]->setAutoDelete(false);
    connect(Client::getInstance()->runnables[4], &Task::response, this, &GameWindow::updateState);
    connect(Client::getInstance()->runnables[5], &Task::response, this, &GameWindow::setFound);
    connect(static_cast<C_DONESELECTING*>(Client::getInstance()->runnables[6]), &C_DONESELECTING::_response, this, &GameWindow::selectingOver);
    connect(static_cast<C_MESSAGE*>(Client::getInstance()->runnables[7]), &C_MESSAGE::_response, this, &GameWindow::addMessage);
    connect(Client::getInstance()->runnables[8], &Task::response, this, &GameWindow::gameOver);
    Client::getInstance()->write(QByteArray(1, char(4)));
    connect(this, &GameWindow::clicked, this, &GameWindow::sendClicked);
    canSelect = false;
}

bool GameWindow::eventFilter(QObject* object, QEvent* event) {
    QLabel2* label = static_cast<QLabel2*>(object);
    if (event->type() == QEvent::MouseButtonRelease) {
        if(label->rect().contains(static_cast<QMouseEvent*>(event)->pos())) {
            if (canSelect) emit clicked(label->row, label->column);
        }
    }
    return QObject::eventFilter(object, event);
}

void GameWindow::sendClicked(int row, int column) {
    QByteArray buffer(1, char(6));
    buffer.append(QString::number(row) + ";" + QString::number(column));
    Client::getInstance()->write(buffer);
}

GameWindow::~GameWindow() {
    delete ui;
}

void GameWindow::closeEvent(QCloseEvent* event) {
    Client::getInstance()->write(QByteArray(1, char(7)));
    Client::getInstance()->w->show();
    emit joinCooldown();
    disconnect(this, 0, 0, 0); deleteLater();
    event->accept();
}

void GameWindow::updateState(QByteArray buffer) {
    int data[5];
    QByteArrayList list = buffer.split(';'); if (list.size() > 1) ui->pushButton_2->setEnabled(!list.at(1).at(0));
    for (int i = 0; i < ui->gridLayout->count(); i ++) {
        unsigned char cardByte = buffer.at(i * 2);
        for (int j = 0; j < 4; j ++, cardByte >>= 2) {
            data[j] = (cardByte % 4) - 1;
        }
        data[4] = int(buffer.at(i * 2 + 1));
        static_cast<QLabel2*>(ui->gridLayout->itemAt(i)->widget())->setup(data);
    }
    if (isHidden()) show();
}

void GameWindow::setFound(QByteArray buffer) {
    ui->pushButton_2->setEnabled(false);
    if (Client::getInstance()->name == QString::fromUtf8(buffer)) {
        canSelect = true;
    }
    addMessage(QString::fromUtf8(buffer) + " is now trying to select a set.");
}

void GameWindow::selectingOver(bool successful, QString playerName) {
    if (Client::getInstance()->name != playerName || successful) ui->pushButton_2->setEnabled(true);
    canSelect = false;
    addMessage(playerName + (successful ? " succesfully found " : " failed to find ") + "a set.");
}

void GameWindow::on_pushButton_2_clicked() {
    Client::getInstance()->write(QByteArray(1, char(5)));
}

void GameWindow::addMessage(QString msg) {
    QLabel* label = new QLabel(msg, this); label->setAlignment(Qt::AlignCenter);
    ui->verticalLayout->addWidget(label);
    if (ui->verticalLayout->count() > 2) ui->verticalLayout->itemAt(0)->widget()->deleteLater();

    QTimer* timer = new QTimer(label);
    connect(timer, &QTimer::timeout, this, &GameWindow::fadeoutMessage);
    timer->setSingleShot(true); timer->start(10000);
}

void GameWindow::fadeoutMessage() {
    QLabel* label = static_cast<QLabel*>(sender()->parent());
    QGraphicsOpacityEffect* e = new QGraphicsOpacityEffect(this);
    label->setGraphicsEffect(e);
    QPropertyAnimation* a = new QPropertyAnimation(e, "opacity");
    a->setDuration(5000); a->setStartValue(1); a->setEndValue(0); a->setEasingCurve(QEasingCurve::OutBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(a, &QPropertyAnimation::finished, label, &QLabel::deleteLater);
}

void GameWindow::gameOver(QByteArray buffer) {
    QString str(buffer); QTextStream stream(&str);
    int players = stream.readLine().toInt();
    for (int i = 0; i < players; i ++) {
        QStringList list = stream.readLine().split(';');
        qDebug() << list.at(0) << "sets:" << list.at(1);
        // make a window for this
    }
    Client::getInstance()->w->show();
    disconnect(this, 0, 0, 0); deleteLater();
}
