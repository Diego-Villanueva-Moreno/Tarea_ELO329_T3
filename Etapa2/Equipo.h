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
    Equipo(QString nombre, double x, double y, double rapidez, double angulo, double deltaAngulo);
    virtual ~Equipo() = default;

    QString getNombre() const;
    double getX() const;
    double getY() const;

    void mover(double dt, int anchoMapa, int altoMapa);
};

#endif // EQUIPO_H