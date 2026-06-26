#ifndef TABLET_H
#define TABLET_H

#include "Equipo.h"
#include <QString>

class Tablet : public Equipo {
private:
    QString dueno;

public:
    Tablet(QString dueno, double x, double y, double rapidez, double angulo, double deltaAngulo)
        : Equipo(dueno + "_Tablet", x, y, rapidez, angulo, deltaAngulo), dueno(dueno) {}

    QString getDueno() const { return dueno; }
};

#endif // TABLET_H