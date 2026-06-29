#ifndef GFINDMYWINDOW_H
#define GFINDMYWINDOW_H

#include <QDialog>
#include <QPainter>
#include <QTimer>
#include <QPixmap>
#include <QVector>
#include <QString>
#include "ETNube.h"

class GFindMyWindow : public QDialog {
    Q_OBJECT
private:
    QString m_dueno;
    ETNube *m_nube;
    QPixmap m_fondo;
    QTimer *m_timer;

public:
    GFindMyWindow(const QString& dueno, ETNube *nube, const QString& rutaFondo, QWidget *parent = nullptr)
        : QDialog(parent), m_dueno(dueno), m_nube(nube) {

        // La ventana se destruye automáticamente al cerrarse para no consumir RAM
        setAttribute(Qt::WA_DeleteOnClose);
        setWindowTitle("Graphics Find My - " + dueno);

        // Cargamos la imagen de fondo
        m_fondo.load(rutaFondo);
        if (!m_fondo.isNull()) {
            setFixedSize(m_fondo.size());
        } else {
            setFixedSize(800, 600); // Tamaño de respaldo
        }

        // Timer de 1 segundo (1000 ms) para actualizar el mapa
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, [this](){
            update(); // Esto fuerza a Qt a llamar a paintEvent() nuevamente
        });
        m_timer->start(1000);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);

        // 1. Dibujar el mapa de fondo
        if (!m_fondo.isNull()) {
            painter.drawPixmap(0, 0, m_fondo);
        }

        // 2. Obtener la última información de la nube para ESTE dueño
        QVector<ETNube::Reporte> reportes = m_nube->reportesPorDueno(m_dueno);

        // 3. Dibujar los dispositivos basándose en los datos de la nube
        for (const ETNube::Reporte& r : reportes) {

            // Dibujamos un círculo para representar la última posición conocida
            painter.setBrush(QBrush(QColor(220, 20, 60))); // Rojo oscuro para que resalte en el mapa
            painter.setPen(Qt::white);

            int radio = 8;
            QPointF pos = r.posicionReportada;

            painter.drawEllipse(pos, radio, radio);

            // Dibujar el nombre del equipo y un fondo semi-transparente para que el texto se lea bien
            QFont font = painter.font();
            font.setBold(true);
            painter.setFont(font);

            QString texto = r.nombreEquipo;

            // Pequeño rectángulo debajo del texto
            painter.setBrush(QColor(255, 255, 255, 180));
            painter.setPen(Qt::NoPen);
            painter.drawRect(pos.x() + 12, pos.y() - 10, 80, 16);

            painter.setPen(Qt::black);
            painter.drawText(pos.x() + 15, pos.y() + 3, texto);
        }
    }
};

#endif // GFINDMYWINDOW_H