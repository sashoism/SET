#include "Game.h"
#include "Player2.h"
#include "Server.h"
#include <QChildEvent>
#include <ctime>

Game::Game(QObject* parent, Player* creator) : QObject(parent) {
    setObjectName(creator->getName());
    connect(creator, &Player::destroyed, this, &Game::creatorDestroyed);
    this->creator = creator;
    selectingPlayer = nullptr; selectedCount = 0;
    expiryTimer = new QTimer(this);
    connect(expiryTimer, &QTimer::timeout, this, &Game::terminate);
    connect(this, &Game::noSets, this, &Game::terminate);
    expiryTimer->start(EMPTY_GAME_EXPIRIRATION_PERIOD_SEC * 1000);
    connect(this, &Game::newPlayer, this, &Game::newPlayer_slot);
    connect(this, &Game::playerLeft, this, &Game::playerLeft_slot);
    initCards();
}

void Game::childEvent(QChildEvent* e) {
    if (dynamic_cast<Player2*>(e->child()) == nullptr) return;
    if (e->added()) {
        emit newPlayer(static_cast<Player2*>(e->child()));
    } else if (e->removed()) {
        emit playerLeft(static_cast<Player2*>(e->child()));
    }
}

QByteArray Game::message(QString msg) {
    QByteArray r(1, char(7)); r.append(msg);
    return r;
}

void Game::newPlayer_slot(Player2* newPlayer) {
    broadcast(message(newPlayer->getName() + " has joined the room."));
    expiryTimer->stop();
}

void Game::playerLeft_slot(Player2* leavingPlayer) {
    broadcast(message(leavingPlayer->getName() + " has left the room."));
    if (leavingPlayer == selectingPlayer) {
        stopTimeout();
        selectingPlayerTimedOut();
    }
    if (!findChildren<Player2*>().size()) expiryTimer->start();
}

int Game::activePlayers() {
    return findChildren<Player2*>().size();
}

void Game::initCards() {
    for (int count = 1; count <= 3; count ++) {
        for (int shape = 1; shape <= 3; shape ++) {
            for (int fill = 1; fill <= 3; fill ++) {
                for (int color = 1; color <= 3; color ++) {
                    Card* card = new Card(this, count, shape, fill, color);
                    connect(card, &Card::selectionChange, this, &Game::selectionChanged);
                    deck.append(card);
                }
            }
        }
    }
    std::srand(time(0));
    std::random_shuffle(deck.begin(), deck.end());

    for (int i = 0; i < 12; i ++) {
        cards.append(deck.last());
        deck.pop_back();
    }
    while (!Card::setsCount(cards)) {
        for (int i = 0; i < 3; i ++) {
            deck.prepend(cards.at(i));
            cards.replace(i, deck.last());
            deck.pop_back();
        }
    }
}

QByteArray Game::cardsList() {
    QByteArray cardsList(1, char(4));
    for (int i = 0; i < cards.size(); i ++) {
        cardsList.append(cards.at(i)->toByte());
        cardsList.append(cards.at(i)->selected);
    }
    return cardsList;
}

void Game::cardSelected(int row, int column) {
    if (selectedCount == 3) return;
    cards[row * 4 + column]->switchSelection();
}

void Game::selectionChanged(int delta) {
    broadcastCards();
    selectedCount += delta;
    if (selectedCount == 3) {
        emit stopTimeout();
        QList<Card*> selectedCards;
        for (int i = 0; i < cards.size(); i ++) {
            if (cards.at(i)->selected) {
                selectedCards.append(cards.at(i));
            }
        }
        bool set = Card::checkSet(selectedCards);
        if (set) {
            selectingPlayer->foundSet();
            int positions[3];
            for (int i = 0; i < selectedCards.size(); i ++) {
                Card* card = selectedCards[i]; positions[i] = cards.lastIndexOf(selectedCards.at(i));
                if (!deck.isEmpty()) {
                    cards.replace(positions[i], deck.last());
                    deck.pop_back();
                } else cards[positions[i]] = new Card(this, 0, 0, 0, 0);
                card->deleteLater();
            }
            bool flag = false;
            if (!Card::setsCount(cards + deck)) terminate();
            while (!Card::setsCount(cards) && !flag) {
                for (int i = 0; i < 3; i ++) {
                    if (deck.isEmpty()) {
                        flag = true; break;
                    }
                    deck.insert(i, cards.at(positions[i]));
                    cards.replace(positions[i], deck.last());
                    deck.pop_back();
                }
            }
            selectedCount = 0;
        }
        QByteArray buffer(1, char(6));
        buffer.append(set); buffer.append(";" + selectingPlayer->getName());
        setSelectingPlayer(nullptr);
        QThread::sleep(1); emit broadcast(buffer);
        if (set) broadcastCards(); else for (int i = 0; i < selectedCards.size(); i ++) selectedCards.at(i)->switchSelection();
    }
}

void Game::selectingPlayerTimedOut() {
    QByteArray buffer(1, char(6));
    buffer.append(char(0)); buffer.append(";" + selectingPlayer->getName());
    setSelectingPlayer(nullptr);
    for (int i = 0; i < cards.size(); i ++) {
        if (cards.at(i)->selected) {
            cards.at(i)->switchSelection();
        }
    }
    emit broadcast(buffer);
}

bool Game::currentlySelecting() {
    return selectingPlayer != nullptr;
}

void Game::setSelectingPlayer(Player2* player) {
    selectingPlayer = player;
}

QString Game::getName() {
    return objectName();
}

void Game::creatorDestroyed() {
    creator = nullptr;
}

bool lessThan(Player2* a, Player2* b) {
    return a->setsFound > b->setsFound;
}

void Game::terminate() {
    QList<Player2*> players = findChildren<Player2*>();
    qSort(players.begin(), players.end(), lessThan);
    QByteArray buffer(1, char(8)); buffer.append(QString::number(players.size()) + "\n");
    for (int i = 0; i < players.size(); i ++) {
        buffer.append(players.at(i)->getName() + ";" + QString::number(players.at(i)->setsFound) + "\n");
    }
    broadcast(buffer);
    if (creator != nullptr) creator->connection->write(QByteArray(1, char(9)));
    this->deleteLater();
}
