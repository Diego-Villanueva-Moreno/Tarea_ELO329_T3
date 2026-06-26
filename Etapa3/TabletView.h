#ifndef TABLETVIEW_H
#define TABLETVIEW_H

#include <QPainter>
#include "Tablet.h"

class TabletView {
public:
    static void draw(QPainter *painter, const Tablet *tablet) {
        painter->setBrush(QBrush(QColor(50, 240, 50))); // Verde
        painter->setPen(Qt::NoPen);

        int w = 24, h = 32;
        painter->drawRoundedRect(tablet->getX() - w/2, tablet->getY() - h/2, w, h, 4, 4);

        painter->setPen(Qt::black);
        painter->drawText(tablet->getX() - w/2, tablet->getY() + h, tablet->getDueno());
    }
};

#endif // TABLETVIEW_H