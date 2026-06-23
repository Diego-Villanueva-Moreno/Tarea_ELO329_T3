#ifndef TERRITORYVIEW_H
#define TERRITORYVIEW_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include "Territory.h"
#include "CellularView.h"

class TerritoryView : public QWidget {
    Q_OBJECT
private:
    Territory *territorio;
    QPixmap fondo;

public:
    TerritoryView(Territory *t, QWidget *parent = nullptr)
        : QWidget(parent), territorio(t) {
        fondo.load(territorio->getRutaImagen());
        // Ajustamos el tamaño de la ventana al tamaño de la imagen de fondo
        if (!fondo.isNull()) {
            setFixedSize(fondo.size());
        }
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);

        // 1. Pintar fondo
        if (!fondo.isNull()) {
            painter.drawPixmap(0, 0, fondo);
        }

        // 2. Pintar cada celular cargado
        for (const Cellular *cel : territorio->getCelulares()) {
            CellularView::draw(&painter, cel);
        }
    }
};

#endif // TERRITORYVIEW_H