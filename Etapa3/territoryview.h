#ifndef TERRITORYVIEW_H
#define TERRITORYVIEW_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QRectF>
#include <QPointF>
#include <QHash>
#include <QSet>
#include <QRandomGenerator>
#include <QtMath>
#include "Territory.h"
#include "CellularView.h"
#include "EloTelTagView.h"
#include "TabletView.h"
#include "FindMyWindow.h"

class TerritoryView : public QWidget {
    Q_OBJECT
private:
    struct RadarInfo {
        double inicio;
        double periodo;
        QSet<const Cellular*> celularesReportados;
    };

    Territory *territorio;
    QPixmap fondo;
    QTimer *reloj;
    double tiempoSimulacion;
    QHash<const Equipo*, double> proximaBusqueda;
    QHash<const Equipo*, RadarInfo> radaresActivos;

    static constexpr double RADIO_BUSQUEDA = 50.0;
    static constexpr double DURACION_RADAR = 1.0;
    static constexpr double PERIODO_TAG = 4.0;
    static constexpr double PERIODO_TABLET = 5.0;
    static constexpr double PERIODO_CELULAR = 4.0;

public:
    TerritoryView(Territory *t, QWidget *parent = nullptr)
        : QWidget(parent), territorio(t), tiempoSimulacion(0.0) {
        fondo.load(territorio->getRutaImagen());

        if (!fondo.isNull()) {
            setFixedSize(fondo.size());
        }

        inicializarFasesDeBusqueda();

        reloj = new QTimer(this);

        connect(reloj, &QTimer::timeout, this, [=](){
            tiempoSimulacion += territorio->getDeltaTime();
            territorio->moverTodos(fondo.width(), fondo.height());
            procesarBusquedas();
            update();
        });

        int milisegundos = static_cast<int>(territorio->getDeltaTime() * 1000);
        if (milisegundos <= 0) milisegundos = 100;

        reloj->start(milisegundos);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        if (!fondo.isNull()) {
            painter.drawPixmap(0, 0, fondo);
        }

        dibujarRadares(&painter);

        for (const Cellular *cel : territorio->getCelulares()) {
            CellularView::draw(&painter, cel);
        }

        for (const EloTelTag *tag : territorio->getTags()){
            EloTelTagView::draw(&painter, tag);
        }

        for (const Tablet *tablet : territorio->getTablets()){
            TabletView::draw(&painter, tablet);
        }
    }

    void mousePressEvent(QMouseEvent *event) override {
        QString duenoSeleccionado;

        if (clickEnCelular(event->pos(), duenoSeleccionado) ||
            clickEnTablet(event->pos(), duenoSeleccionado)) {
            QMenu menu(this);
            QAction *findMy = menu.addAction("Find My");
            QAction *seleccion = menu.exec(mapToGlobal(event->pos()));

            if (seleccion == findMy) {
                mostrarFindMy(duenoSeleccionado);
            }
            return;
        }

        QWidget::mousePressEvent(event);
    }

private:
    void inicializarFasesDeBusqueda() {
        for (const EloTelTag *tag : territorio->getTags()) {
            proximaBusqueda.insert(tag, numeroAleatorioEntre(0.0, PERIODO_TAG));
        }

        for (const Tablet *tablet : territorio->getTablets()) {
            proximaBusqueda.insert(tablet, numeroAleatorioEntre(0.0, PERIODO_TABLET));
        }

        for (const Cellular *cel : territorio->getCelulares()) {
            proximaBusqueda.insert(cel, numeroAleatorioEntre(0.0, PERIODO_CELULAR));
        }
    }

    double numeroAleatorioEntre(double minimo, double maximo) const {
        return minimo + QRandomGenerator::global()->generateDouble() * (maximo - minimo);
    }

    void procesarBusquedas() {
        iniciarBusquedasPendientes();
        actualizarRadaresActivos();
        procesarReportesCelulares();
    }

    void iniciarBusquedasPendientes() {
        for (const EloTelTag *tag : territorio->getTags()) {
            iniciarRadarSiCorresponde(tag, PERIODO_TAG);
        }

        for (const Tablet *tablet : territorio->getTablets()) {
            iniciarRadarSiCorresponde(tablet, PERIODO_TABLET);
        }
    }

    void procesarReportesCelulares() {
        for (const Cellular *cel : territorio->getCelulares()) {
            if (tiempoSimulacion >= proximaBusqueda.value(cel)) {
                territorio->getNube()->reportarPosicion(
                    cel->getNombre(),
                    cel->getDueno(),
                    QPointF(cel->getX(), cel->getY()),
                    cel->getNombre(),
                    tiempoSimulacion
                    );

                proximaBusqueda[cel] = tiempoSimulacion + PERIODO_CELULAR;
            }
        }
    }

    void iniciarRadarSiCorresponde(const Equipo *equipo, double periodo) {
        if (!proximaBusqueda.contains(equipo)) {
            proximaBusqueda.insert(equipo, tiempoSimulacion + numeroAleatorioEntre(0.0, periodo));
        }

        if (tiempoSimulacion >= proximaBusqueda.value(equipo)) {
            RadarInfo radar;
            radar.inicio = tiempoSimulacion;
            radar.periodo = periodo;
            radaresActivos.insert(equipo, radar);
            proximaBusqueda[equipo] = tiempoSimulacion + periodo;
        }
    }

    void actualizarRadaresActivos() {
        QVector<const Equipo*> finalizados;

        for (auto it = radaresActivos.begin(); it != radaresActivos.end(); ++it) {
            const Equipo *equipoRastreable = it.key();
            RadarInfo &radar = it.value();
            double tiempoActivo = tiempoSimulacion - radar.inicio;

            if (tiempoActivo > DURACION_RADAR) {
                finalizados.append(equipoRastreable);
                continue;
            }

            double radioActual = RADIO_BUSQUEDA * qBound(0.0, tiempoActivo / DURACION_RADAR, 1.0);
            reportarCelularesAlcanzados(equipoRastreable, radar, radioActual);
        }

        for (const Equipo *equipo : finalizados) {
            radaresActivos.remove(equipo);
        }
    }

    void reportarCelularesAlcanzados(const Equipo *equipoRastreable,
                                     RadarInfo &radar,
                                     double radioActual) {
        for (const Cellular *cel : territorio->getCelulares()) {
            if (radar.celularesReportados.contains(cel)) {
                continue;
            }

            double distancia = distanciaEntre(equipoRastreable, cel);
            if (distancia <= radioActual) {
                QString duenoEquipo = duenoDeEquipoRastreable(equipoRastreable);
                territorio->getNube()->reportarPosicion(
                    equipoRastreable->getNombre(),
                    duenoEquipo,
                    QPointF(cel->getX(), cel->getY()),
                    cel->getNombre(),
                    tiempoSimulacion
                );
                radar.celularesReportados.insert(cel);
            }
        }
    }

    static double distanciaEntre(const Equipo *a, const Equipo *b) {
        double dx = a->getX() - b->getX();
        double dy = a->getY() - b->getY();
        return qSqrt(dx * dx + dy * dy);
    }

    QString duenoDeEquipoRastreable(const Equipo *equipo) const {
        if (const EloTelTag *tag = dynamic_cast<const EloTelTag*>(equipo)) {
            return tag->getDueno();
        }

        if (const Tablet *tablet = dynamic_cast<const Tablet*>(equipo)) {
            return tablet->getDueno();
        }

        return QString();
    }

    void dibujarRadares(QPainter *painter) {
        QPen pen(QColor(30, 144, 255, 170));
        pen.setWidth(2);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(pen);

        for (auto it = radaresActivos.constBegin(); it != radaresActivos.constEnd(); ++it) {
            const Equipo *equipo = it.key();
            const RadarInfo &radar = it.value();
            double tiempoActivo = tiempoSimulacion - radar.inicio;

            if (tiempoActivo < 0.0 || tiempoActivo > DURACION_RADAR) {
                continue;
            }

            double radio = RADIO_BUSQUEDA * qBound(0.0, tiempoActivo / DURACION_RADAR, 1.0);
            painter->drawEllipse(QPointF(equipo->getX(), equipo->getY()), radio, radio);
        }
    }

    bool clickEnCelular(const QPoint& punto, QString& dueno) const {
        const int w = 16;
        const int h = 24;

        for (const Cellular *cel : territorio->getCelulares()) {
            QRectF rect(cel->getX() - w / 2.0, cel->getY() - h / 2.0, w, h);
            if (rect.contains(punto)) {
                dueno = cel->getDueno();
                return true;
            }
        }
        return false;
    }

    bool clickEnTablet(const QPoint& punto, QString& dueno) const {
        const int w = 24;
        const int h = 32;

        for (const Tablet *tablet : territorio->getTablets()) {
            QRectF rect(tablet->getX() - w / 2.0, tablet->getY() - h / 2.0, w, h);
            if (rect.contains(punto)) {
                dueno = tablet->getDueno();
                return true;
            }
        }
        return false;
    }

    void mostrarFindMy(const QString& dueno) {
        QVector<ETNube::Reporte> reportes = territorio->getNube()->reportesPorDueno(dueno);
        FindMyWindow *ventana = new FindMyWindow(dueno, reportes, this);
        ventana->show();
    }
};

#endif // TERRITORYVIEW_H
