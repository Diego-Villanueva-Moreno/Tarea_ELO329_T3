#ifndef ETNUBE_H
#define ETNUBE_H

#include <QString>
#include <QPointF>
#include <QVector>
#include <QMap>
#include <algorithm>

class ETNube {
public:
    struct Reporte {
        QString nombreEquipo;
        QString dueno;
        QPointF posicionReportada;
        QString reportadoPor;
        double tiempoSimulacion;
    };

    ETNube() = default;

    void reportarPosicion(const QString& nombreEquipo,
                          const QString& dueno,
                          const QPointF& posicionReportada,
                          const QString& reportadoPor,
                          double tiempoSimulacion) {
        Reporte r{nombreEquipo, dueno, posicionReportada, reportadoPor, tiempoSimulacion};
        // Se guarda el último reporte conocido por equipo.
        reportesPorEquipo[clave(dueno, nombreEquipo)] = r;
    }

    QVector<Reporte> reportesPorDueno(const QString& dueno) const {
        QVector<Reporte> resultado;
        for (auto it = reportesPorEquipo.constBegin(); it != reportesPorEquipo.constEnd(); ++it) {
            if (it.value().dueno == dueno) {
                resultado.append(it.value());
            }
        }

        std::sort(resultado.begin(), resultado.end(), [](const Reporte& a, const Reporte& b){
            return a.nombreEquipo < b.nombreEquipo;
        });
        return resultado;
    }

private:
    QMap<QString, Reporte> reportesPorEquipo;

    static QString clave(const QString& dueno, const QString& nombreEquipo) {
        return dueno + "::" + nombreEquipo;
    }
};

#endif // ETNUBE_H
