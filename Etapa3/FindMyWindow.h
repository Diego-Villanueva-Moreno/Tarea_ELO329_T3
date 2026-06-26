#ifndef FINDMYWINDOW_H
#define FINDMYWINDOW_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QString>
#include "ETNube.h"

class FindMyWindow : public QDialog {
public:
    FindMyWindow(const QString& dueno,
                 const QVector<ETNube::Reporte>& reportes,
                 QWidget *parent = nullptr)
        : QDialog(parent) {
        setAttribute(Qt::WA_DeleteOnClose);
        setWindowTitle("Find My - " + dueno);
        resize(620, 320);

        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *titulo = new QLabel("Equipos rastreables de " + dueno, this);
        layout->addWidget(titulo);

        if (reportes.isEmpty()) {
            QLabel *sinDatos = new QLabel(
                "Sin reportes disponibles.\n"
                "Espere a que un Tag o Tablet detecte un celular cercano y vuelva a abrir Find My.",
                this
            );
            sinDatos->setWordWrap(true);
            layout->addWidget(sinDatos);
            return;
        }

        QTableWidget *tabla = new QTableWidget(reportes.size(), 5, this);
        tabla->setHorizontalHeaderLabels({"Equipo", "Dueño/a", "Posición reportada", "Reportado por", "Tiempo"});
        tabla->verticalHeader()->setVisible(false);
        tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
        tabla->setSelectionMode(QAbstractItemView::SingleSelection);

        for (int i = 0; i < reportes.size(); ++i) {
            const ETNube::Reporte& r = reportes[i];
            tabla->setItem(i, 0, new QTableWidgetItem(r.nombreEquipo));
            tabla->setItem(i, 1, new QTableWidgetItem(r.dueno));
            tabla->setItem(i, 2, new QTableWidgetItem(QString("(%1, %2)")
                                                       .arg(r.posicionReportada.x(), 0, 'f', 1)
                                                       .arg(r.posicionReportada.y(), 0, 'f', 1)));
            tabla->setItem(i, 3, new QTableWidgetItem(r.reportadoPor));
            tabla->setItem(i, 4, new QTableWidgetItem(QString::number(r.tiempoSimulacion, 'f', 1) + " s"));
        }

        tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        layout->addWidget(tabla);
    }
};

#endif // FINDMYWINDOW_H
