#ifndef ELOTELTAG_H
#define ELOTELTAG_H

#include "Equipo.h"
#include <QString>

class EloTelTag : public Equipo {
private:
    QString dueno;

public:
    EloTelTag(QString nombreTag, QString dueno, double x, double y, double rapidez, double angulo, double deltaAngulo)
        : Equipo(dueno + nombreTag, x, y, rapidez, angulo, deltaAngulo), dueno(dueno) {}

    QString getDueno() const { return dueno; }
};

#endif // ELOTELTAG_H