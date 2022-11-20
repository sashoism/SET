#include "Client.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QChar>
#include <QDebug>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QRegExpValidator>
#include <QtMath>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    isAndroid = true;
    #ifndef Q_OS_ANDROID
        isAndroid = false;
        int height = QApplication::desktop()->height(); if (height < 860) height = 860;
        setMinimumSize(0.25 * height, 0.35 * height); setMaximumSize(minimumSize());
    #endif
    QRegExp regex("(([A-Z]|[a-z]|[А-Я]|[а-я])+\\s?)+");
    QValidator* v = new QRegExpValidator(regex, this);
    ui->lineEdit->setValidator(v);
    model = new QStandardItemModel;
    ui->tableView->setModel(model);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_lineEdit_textChanged(QString text) {
    for (int i = 0; i < text.size(); i ++) if (i == 0 || (i > 1 && text[i - 1] == ' ')) text[i] = text[i].toUpper(); else text[i] = text[i].toLower();
    ui->lineEdit->setText(text);

    if (ui->lineEdit->text() == Client::getInstance()->name) {
        ui->pushButton->setEnabled(false);
        if (ui->tableView->currentIndex().row() != -1) ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
    } else {
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
    }
}

void MainWindow::on_lineEdit_returnPressed() {
    on_pushButton_clicked();
}

void MainWindow::on_pushButton_clicked() {
    if (ui->lineEdit->text().endsWith(' ')) {
        QString tmp = ui->lineEdit->text(); tmp.chop(1); ui->lineEdit->setText(tmp);
        if (tmp == Client::getInstance()->name) {
            if (ui->tableView->currentIndex().row() != -1) ui->pushButton_2->setEnabled(true);
            return;
        }
    }
    QByteArray s(1, char(1)); s.append(ui->lineEdit->text());
    Client::getInstance()->write(s);
}

void MainWindow::on_pushButton_2_clicked() {
    if (Client::getInstance()->cooldownTimer->remainingTime() > 0) {
        if (isAndroid) QMessageBox::warning(this, "You can join a room again after " + QString::number(qCeil(Client::getInstance()->cooldownTimer->remainingTime() / 1000.)) + " seconds.", "");
        else QMessageBox::warning(this, " ", "Failed to join room:\nYou can join a room again after " + QString::number(qCeil(Client::getInstance()->cooldownTimer->remainingTime() / 1000.)) + " seconds.");
        return;
    }
    QByteArray s(1, char(2)); s.append(model->item(ui->tableView->currentIndex().row(), 0)->text());
    Client::getInstance()->write(s);
}

void MainWindow::on_pushButton_3_clicked() {
    Client::getInstance()->write(QByteArray(1, char(3)));
}

void MainWindow::setName_response(QByteArray buffer) {
    switch(buffer.at(0)) {
        case 1:
            Client::getInstance()->name = ui->lineEdit->text();
            if (ui->tableView->currentIndex().row() == -1) ui->tableView->selectRow(0); else ui->pushButton_2->setEnabled(true);
            ui->tableView->setFocus();
            ui->pushButton->setEnabled(false);
            ui->pushButton_3->setEnabled(true);
        break;
        case 2:
            if (isAndroid) QMessageBox::warning(this, "Failed to set desired name.", "");
            else QMessageBox::warning(this, " ", "Failed to set desired name:\nThere is already a player with that name.");
        break;
        case 3:
            if (isAndroid) QMessageBox::warning(this, "Failed to set desired name.", "");
            else QMessageBox::warning(this, " ", "Failed to set desired name:\nThere is already a room created by a player with that name.");
        break;
        case 4:
            if (isAndroid) QMessageBox::warning(this, "Failed to set desired name.", "");
            else QMessageBox::warning(this, " ", "Failed to set desired name:\nYou are not allowed to change name while in-game.");
        break;
    }
}

void MainWindow::joinGame_response(QByteArray buffer) {
    if (buffer.at(0)) {
        Client::getInstance()->startGame();
        ui->pushButton_2->setEnabled(false);
        hide();
    }
}

void MainWindow::addGame_response(QByteArray buffer) {
    if (buffer.at(0)) {
        ui->lineEdit->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
    }
}

void MainWindow::pGameOver_response() {
    ui->lineEdit->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
}

void MainWindow::received_lobbyData(QByteArray buffer) {
    int lineHeightScale = 1;
    if (isAndroid) lineHeightScale = 1.5;
    ui->tableView->verticalHeader()->setDefaultSectionSize(ui->lineEdit->height() * lineHeightScale);
    int oldRow = ui->tableView->currentIndex().row();
    QString str(buffer); QTextStream stream(&str);
    QStringList line = stream.readLine().split(';');
    int gamesCount = line.at(0).toInt(), maxPlayers = line.at(1).toInt();
    model->clear();
    for (int i = 0; i < gamesCount; i ++) {
        line = stream.readLine().split(';');
        QString leftText = line.at(0); int rightValue = line.at(1).toInt();
        QStandardItem* left = new QStandardItem;
        QStandardItem* right = new QStandardItem;
        if (rightValue == maxPlayers) {
            left->setFlags(left->flags() & ~Qt::ItemIsEnabled);
            right->setFlags(right->flags() & ~Qt::ItemIsEnabled);
        }
        left->setText(leftText);
        model->setItem(i, 0, left);
        QString rightText = "[" + QString::number(rightValue) + "/" + QString::number(maxPlayers) + "]"; right->setText(rightText);
        model->setItem(i, 1, right);
    }
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnToContents(1);
    ui->tableView->horizontalHeader()->resizeSection(0, ui->tableView->width() - ui->tableView->columnWidth(1) - (!isAndroid * 2));
    int rowToHighlight = (oldRow < ui->tableView->model()->rowCount() && oldRow != -1 ? oldRow : ui->tableView->model()->rowCount() - 1);
    if (rowToHighlight != -1 && model->item(rowToHighlight)->flags().testFlag(Qt::ItemIsEnabled)) {
        ui->tableView->selectRow(rowToHighlight);
    }
    if (ui->tableView->currentIndex().row() != -1 && !Client::getInstance()->name.isEmpty()) ui->pushButton_2->setEnabled(true);
    if (!gamesCount) ui->pushButton_2->setEnabled(false);
}
