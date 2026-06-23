#ifndef TERRITORYVIEW_H
#define TERRITORYVIEW_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include "Territory.h"
#include "CellularView.h"
#include "EloTelTagView.h"
#include "TabletView.h"

class TerritoryView : public QWidget {
    Q_OBJECT
private:
    Territory *territorio;
    QPixmap fondo;
    QTimer *reloj;

public:
    TerritoryView(Territory *t, QWidget *parent = nullptr)
        : QWidget(parent), territorio(t) {
        fondo.load(territorio->getRutaImagen());
        // Ajustamos el tamaño de la ventana al tamaño de la imagen de fondo
        if (!fondo.isNull()) {
            setFixedSize(fondo.size());
        }

        //Configuracion de motor de movimiento
        reloj = new QTimer(this);

        connect(reloj, &QTimer::timeout, this, [=](){
            territorio->moverTodos(fondo.width(), fondo.height());
            update();
        });

        //Arrancamos el reloj
        int milisegundos = static_cast<int>(territorio->getDeltaTime() * 1000);
        if (milisegundos <= 0) milisegundos = 100;

        reloj->start(milisegundos);
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

        //3. Pintar cada EloTelTag cargado
        for (const EloTelTag *tag : territorio->getTags()){
            EloTelTagView::draw(&painter, tag);
        }

        //4. Pintar cada Tabler cargada
        for (const Tablet *tablet : territorio->getTablets()){
            TabletView::draw(&painter, tablet);
        }
    }
};

#endif // TERRITORYVIEW_H
