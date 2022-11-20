#ifndef GAME_H
#define GAME_H

#include "Card.h"
#include "Player2.h"
#include "Utility.h"
#include <QTimer>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject* parent = 0, Player* creator = 0);
    ~Game() { qDebug() << LOG_TIME << qPrintable(getName() + "\'s game was terminated successfully"); }
    int activePlayers();
    QByteArray cardsList();
    bool currentlySelecting();
    void setSelectingPlayer(Player2*);
    QString getName();
public slots:
    void broadcastCards() { emit broadcast(cardsList()); }
    void cardSelected(int, int);
    void selectingPlayerTimedOut();
protected:
    void childEvent(QChildEvent*);
private slots:
    QByteArray message(QString);
    void newPlayer_slot(Player2*);
    void playerLeft_slot(Player2*);
    void selectionChanged(int);
    void creatorDestroyed();
    void terminate();
private:
    Player* creator;
    Player2* selectingPlayer;
    int selectedCount;
    QList<Card*> deck, cards;
    QTimer* expiryTimer;
    void initCards();
signals:
    void broadcast(QByteArray);
    void newPlayer(Player2*);
    void playerLeft(Player2*);
    void stopTimeout();
    void noSets();
};

#endif
