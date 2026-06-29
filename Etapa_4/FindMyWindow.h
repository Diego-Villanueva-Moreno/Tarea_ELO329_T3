#ifndef FINDMYWINDOW_H
#define FINDMYWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPushButton>
#include <QTimer>
#include <QMap>
#include <QString>
#include "ETNube.h"

class FindMyWindow : public QDialog {
    Q_OBJECT
public:
    // NOTA: Se cambia el parámetro QVector por un puntero a ETNube
    FindMyWindow(const QString& dueno, ETNube *nube, QWidget *parent = nullptr)
        : QDialog(parent), m_dueno(dueno), m_nube(nube) {
        setAttribute(Qt::WA_DeleteOnClose);
        setWindowTitle("Find My - " + dueno);
        resize(750, 320); // Se amplía un poco para la nueva columna

        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *titulo = new QLabel("Equipos rastreables de " + dueno, this);
        layout->addWidget(titulo);

        m_tabla = new QTableWidget(0, 6, this); // Ahora son 6 columnas
        m_tabla->setHorizontalHeaderLabels({"Equipo", "Dueño/a", "Posición reportada", "Reportado por", "Tiempo", "Traza"});
        m_tabla->verticalHeader()->setVisible(false);
        m_tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_tabla->setSelectionMode(QAbstractItemView::SingleSelection);
        m_tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        layout->addWidget(m_tabla);

        // MOTOR DE ACTUALIZACIÓN: QTimer configurado a 1000 ms (1 segundo)
        QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &FindMyWindow::actualizarDatos);
        timer->start(1000);

        // Llamada inicial para no esperar 1 segundo al abrir la ventana
        actualizarDatos();
    }

signals:
    // Emitimos esta señal cuando el usuario presiona el botón de traza.
    // Debes conectar esta señal en TerritoryView para dibujar la línea en la escena.
    void toggleTraza(const QString& nombreEquipo, bool estado);

private slots:
    void actualizarDatos() {
        QVector<ETNube::Reporte> reportes = m_nube->reportesPorDueno(m_dueno);

        for (const ETNube::Reporte& r : reportes) {
            int fila;
            // Verificamos si el equipo ya está en la tabla para no recrear la fila
            if (m_filaPorEquipo.contains(r.nombreEquipo)) {
                fila = m_filaPorEquipo[r.nombreEquipo];
            } else {
                fila = m_tabla->rowCount();
                m_tabla->insertRow(fila);
                m_filaPorEquipo[r.nombreEquipo] = fila;

                // Crear el botón de traza solo una vez al descubrir el equipo
                QPushButton *btnTraza = new QPushButton("Ver Traza", this);
                btnTraza->setCheckable(true); // Funciona como interruptor

                // Conectamos el click del botón a nuestra señal
                connect(btnTraza, &QPushButton::toggled, this, [this, r](bool checked) {
                    emit toggleTraza(r.nombreEquipo, checked);
                });

                m_tabla->setCellWidget(fila, 5, btnTraza);
            }

            // Actualizamos los textos con la información más reciente
            m_tabla->setItem(fila, 0, new QTableWidgetItem(r.nombreEquipo));
            m_tabla->setItem(fila, 1, new QTableWidgetItem(r.dueno));
            m_tabla->setItem(fila, 2, new QTableWidgetItem(QString("(%1, %2)")
                                                           .arg(r.posicionReportada.x(), 0, 'f', 1)
                                                           .arg(r.posicionReportada.y(), 0, 'f', 1)));
            m_tabla->setItem(fila, 3, new QTableWidgetItem(r.reportadoPor));
            m_tabla->setItem(fila, 4, new QTableWidgetItem(QString::number(r.tiempoSimulacion, 'f', 1) + " s"));
        }
    }

private:
    QString m_dueno;
    ETNube *m_nube;
    QTableWidget *m_tabla;
    QMap<QString, int> m_filaPorEquipo; // Diccionario para rastrear la fila de cada equipo
};

#endif // FINDMYWINDOW_H