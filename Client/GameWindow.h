#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QLabel>
#include <QWidget>

namespace Ui {
    class GameWindow;
}

class GameWindow : public QWidget {
    Q_OBJECT
public:
    explicit GameWindow(QWidget* parent = 0);
    ~GameWindow();
protected:
    bool eventFilter(QObject*, QEvent*);
    void closeEvent(QCloseEvent*);
private:
    Ui::GameWindow* ui;
    bool canSelect;
private slots:
    void updateState(QByteArray);
    void setFound(QByteArray);
    void selectingOver(bool, QString);
    void on_pushButton_2_clicked();
    void sendClicked(int, int);
    void addMessage(QString);
    void fadeoutMessage();
    void gameOver(QByteArray);
signals:
    void clicked(int, int);
    void joinCooldown();
};

#endif // GAMEWINDOW_H
