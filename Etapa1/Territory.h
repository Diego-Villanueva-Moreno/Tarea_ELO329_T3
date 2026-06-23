#ifndef TERRITORY_H
#define TERRITORY_H

#include <QString>
#include <QVector>
#include "Cellular.h"

class Territory {
private:
    QString rutaImagen;
    double deltaTime;
    QVector<Cellular*> celulares;

public:
    Territory() : deltaTime(0.0) {}
    ~Territory() {
        qDeleteAll(celulares);
    }

    void setRutaImagen(const QString& ruta) { rutaImagen = ruta; }
    QString getRutaImagen() const { return rutaImagen; }

    void setDeltaTime(double dt) { deltaTime = dt; }

    void addCellular(Cellular* cel) { celulares.append(cel); }
    const QVector<Cellular*>& getCelulares() const { return celulares; }
};

#endif // TERRITORY_H