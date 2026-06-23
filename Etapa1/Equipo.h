#ifndef EQUIPO_H
#define EQUIPO_H

#include <QString>

class Equipo {
protected:
    QString nombre;
    double x, y;
    double rapidez;
    double angulo;
    double deltaAngulo;

public:
    Equipo(QString nombre, double x, double y, double rapidez, double angulo, double deltaAngulo)
        : nombre(nombre), x(x), y(y), rapidez(rapidez), angulo(angulo), deltaAngulo(deltaAngulo) {}

    virtual ~Equipo() = default;

    QString getNombre() const { return nombre; }
    double getX() const { return x; }
    double getY() const { return y; }
};

#endif // EQUIPO_H