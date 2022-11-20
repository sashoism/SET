#ifndef QLABEL2_H
#define QLABEL2_H

#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QBitmap>
#include <QDebug>

class QLabel2 : public QLabel
{
    Q_OBJECT
public:
    const int &row, &column;
    explicit QLabel2(QWidget* parent = 0, int row = -1, int column = -1) : QLabel(parent), row(_row), column(_column) {
        _row = row; _column = column;
        count = shape = fill = color = -1;
        selected = dontDraw = false;
        pmap = nullptr;
        layout = new QVBoxLayout(this);
        layout->addWidget(new QLabel, 0, Qt::AlignHCenter | Qt::AlignBottom);
        layout->addWidget(new QLabel, 0, Qt::AlignHCenter | Qt::AlignVCenter);
        layout->addWidget(new QLabel, 0, Qt::AlignHCenter | Qt::AlignTop);
    }

    void setup(int count, int shape, int fill, int color, bool selected) {
        if (count == -1 && shape == -1 && fill == -1 && color == -1) { hide(); dontDraw = true; return; }
        if (this->count != count || this->shape != shape || this->fill != fill || this->color != color) {
            if (count < 2) layout->setSpacing(static_cast<QGridLayout*>(static_cast<QWidget*>(parent())->layout())->rowMinimumHeight(row) * 4 / 50); else layout->setSpacing(1);
            for (int i = 0; i < layout->count(); i ++) {
                layout->itemAt(i)->widget()->setVisible(count - i % 2);
            }
            borderMask = QPixmap(":/cards/border" + QString::number(shape) + ".png").createMaskFromColor(Qt::black, Qt::MaskOutColor);
            pmap = new QPixmap(borderMask.width(), borderMask.height()); pmap->fill(Qt::white);
            QPainter* colorChange = new QPainter(pmap);
            QColor penColor((color == 0) * 255, (color == 1) * 160, (color == 2) * 225);
            colorChange->setPen(penColor);
            colorChange->drawPixmap(pmap->rect(), borderMask, borderMask.rect());
            fillMask = QPixmap(":/cards/fill" + QString::number(shape) + ".png").createMaskFromColor(Qt::black, Qt::MaskOutColor);
            penColor.setAlpha(fill == 2 ? 255 : (fill == 1 ? 63 : 0)); colorChange->setPen(penColor);
            colorChange->drawPixmap(pmap->rect(), fillMask, fillMask.rect());
            *pmap = pmap->scaledToWidth(static_cast<QGridLayout*>(static_cast<QWidget*>(parent())->layout())->columnMinimumWidth(column) / 2, Qt::SmoothTransformation);
        }
        this->count = count; this->shape = shape; this->fill = fill; this->color = color; this->selected = selected;
        update();
    }

    void setup(int* data) {
        setup(data[0], data[1], data[2], data[3], bool(data[4]));
    }
protected:
    void paintEvent(QPaintEvent*) {
        if (dontDraw) return;
        QPainter p(this); p.setPen(QPen(selected ? Qt::red : Qt::gray, 2)); p.setRenderHint(QPainter::Antialiasing);
        QPainterPath path; path.addRoundedRect(QRectF(2, 2, width() - 4, height() - 4), 5, 5);
        p.fillPath(path, Qt::white); p.drawPath(path);
        if (pmap == nullptr) return;
        for (int i = 0; i < layout->count(); i ++) {
            static_cast<QLabel*>(layout->itemAt(i)->widget())->setPixmap(*pmap);
        }
    }
private:
    int _row, _column;
    int count, shape, fill, color;
    bool selected, dontDraw;
    QVBoxLayout* layout;
    QBitmap borderMask, fillMask;
    QPixmap* pmap;
};

#endif // QLABEL2_H
