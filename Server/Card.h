#ifndef CARD_H
#define CARD_H

#include "Utility.h"
#include <QObject>
#include <QDebug>

class Card : public QObject {
    Q_OBJECT
public:
    const bool& selected;
    Card(QObject* parent = 0, int count = 1, int shape = 1, int fill = 1, int color = 1, bool selected = false) : QObject(parent), selected(_selected) {
        this->count = count;
        this->shape = shape;
        this->fill = fill;
        this->color = color;
        this->_selected = selected;
    }
    unsigned char toByte() const {
        unsigned char c = color;
        c <<= 2; c += fill;
        c <<= 2; c += shape;
        c <<= 2; c += count;
        return c;
    }
    static bool checkSet(QList<Card*> cards) {
        int a, b, c, d; a = b = c = d = 0;
        for (int i = 0; i < cards.size(); i ++) {
            Card* card = cards.at(i);
            if (!card->count || !card->shape || !card->fill || !card->color) return false;
            a += card->count; b += card->shape; c += card->fill; d += card->color;
        }
        return !(a % 3 || b % 3 || c % 3 || d % 3);
    }
    static int setsCount(QList<Card*> cards) {
        QList<Card*> setOf3; int sets = 0;
        for (int a = 0; a < cards.size() - 2; a ++) {
            setOf3.append(cards.at(a));
            for (int b = a + 1; b < cards.size() - 1; b ++) {
                setOf3.append(cards.at(b));
                for (int c = b + 1; c < cards.size(); c ++) {
                    setOf3.append(cards.at(c));
                    if (checkSet(setOf3)) {
                        if (SHOW_SETS) if (a < 12 && b < 12 && c < 12) qDebug() << ("SET! in " + cards.at(0)->parent()->objectName() + "\'s room") << a + 1 << b + 1 << c + 1;
                        sets ++;
                    }
                    setOf3.removeLast();
                }
                setOf3.removeLast();
            }
            setOf3.removeLast();
        }
        return sets;
    }
public slots:
    void switchSelection() {
        _selected = !_selected;
        emit selectionChange(_selected ? 1 : -1);
    }
private:
    int count, shape, fill, color;
    bool _selected;
signals:
    void selectionChange(int);
};

#endif // CARD_H

