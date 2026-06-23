#include "Equipo.h"
#include <cmath>
#include <cstdlib>

// Implementación del constructor
Equipo::Equipo(QString nombre, double x, double y, double rapidez, double angulo, double deltaAngulo)
    : nombre(nombre), x(x), y(y), rapidez(rapidez), angulo(angulo), deltaAngulo(deltaAngulo) {}

// Implementación de los getters
QString Equipo::getNombre() const { return nombre; }
double Equipo::getX() const { return x; }
double Equipo::getY() const { return y; }

// Implementación de la lógica de movimiento
void Equipo::mover(double dt, int anchoMapa, int altoMapa) {
    double variacion = ((rand() % 200) / 100.0 - 1.0) * deltaAngulo;
    angulo += variacion;

    double radianes = angulo * M_PI / 180.0;

    x += rapidez * cos(radianes) * dt;
    y += rapidez * sin(radianes) * dt;

    if (x <= 0 || x >= anchoMapa) {
        angulo = 180.0 - angulo;
        x = (x <= 0) ? 0 : anchoMapa;
    }
    if (y <= 0 || y >= altoMapa) {
        angulo = -angulo;
        y = (y <= 0) ? 0 : altoMapa;
    }
}