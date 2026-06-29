#ifndef CELLULAR_H
#define CELLULAR_H

#include "Equipo.h"

class Cellular : public Equipo {
private:
    QString dueno;

public:
    Cellular(QString dueno, double x, double y, double rapidez, double angulo, double deltaAngulo)
        : Equipo(dueno + "_Celular", x, y, rapidez, angulo, deltaAngulo), dueno(dueno) {}

    QString getDueno() const { return dueno; }
};

#endif // CELLULAR_H