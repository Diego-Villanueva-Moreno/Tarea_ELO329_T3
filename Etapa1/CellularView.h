#ifndef CELLULARVIEW_H
#define CELLULARVIEW_H

#include <QPainter>
#include "Cellular.h"

class CellularView {
public:
    static void draw(QPainter *painter, const Cellular *cellular) {
        // En la captura se ven como rectángulos redondeados azules o verdes
        painter->setBrush(QBrush(QColor(50, 120, 240))); // Azul por defecto
        painter->setPen(Qt::NoPen);

        // Dibujamos el celular (por ejemplo, un rectángulo de 16x24)
        int w = 16, h = 24;
        painter->drawRoundedRect(cellular->getX() - w/2, cellular->getY() - h/2, w, h, 4, 4);

        // Dibujamos el texto del dueño abajo
        painter->setPen(Qt::black);
        painter->drawText(cellular->getX() - w/2, cellular->getY() + h, cellular->getDueno());
    }
};

#endif // CELLULARVIEW_H