#ifndef TERRITORY_H
#define TERRITORY_H

#include <QString>
#include <QVector>
#include "Cellular.h"
#include "EloTelTag.h"
#include "Tablet.h"
#include "ETNube.h"

class Territory {
private:
    QString rutaImagen;
    double deltaTime;
    QVector<Cellular*> celulares;
    QVector<EloTelTag*> tags;
    QVector<Tablet*> tablets;
    ETNube nube;

public:
    Territory();
    ~Territory();

    void setRutaImagen(const QString& ruta);
    QString getRutaImagen() const;

    void setDeltaTime(double dt);
    double getDeltaTime() const; // Asegúrate de tener este getter para el QTimer

    void addCellular(Cellular* cel);
    const QVector<Cellular*>& getCelulares() const;

    void addTag(EloTelTag* tag);
    const QVector<EloTelTag*>& getTags() const;

    void addTablet(Tablet* tablet);
    const QVector<Tablet*>& getTablets() const;

    ETNube* getNube();
    const ETNube* getNube() const;

    void moverTodos(int ancho, int alto);
};

#endif // TERRITORY_H