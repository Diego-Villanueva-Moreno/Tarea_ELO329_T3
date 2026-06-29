#ifndef ELOTELTAGVIEW_H
#define ELOTELTAGVIEW_H

#include <QPainter>
#include "EloTelTag.h"

class EloTelTagView {
public:
    static void draw(QPainter *painter, const EloTelTag *tag) {
        painter->setBrush(QBrush(QColor(240, 50, 50))); // Rojo
        painter->setPen(Qt::NoPen);

        int radio = 8;
        painter->drawEllipse(tag->getX() - radio, tag->getY() - radio, radio * 2, radio * 2);

        painter->setPen(Qt::black);
        painter->drawText(tag->getX() - radio, tag->getY() + radio + 12, tag->getNombre());
    }
};

#endif // ELOTELTAGVIEW_H