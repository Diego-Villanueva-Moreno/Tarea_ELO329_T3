#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "Territory.h"
#include "territoryview.h"

// Aquí está la función que el compilador no encontraba.
// Tiene que ir ANTES del main()
Territory* cargarConfiguracion(const QString &rutaArchivo) {
    QFile archivo(rutaArchivo);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: No se pudo abrir el archivo de configuración en la ruta:" << rutaArchivo;
        return nullptr;
    }

    QFileInfo fileInfo(archivo);
    QDir directorioConfig = fileInfo.dir();

    QTextStream in(&archivo);
    Territory *territorio = new Territory();

    QString nombreImagen = in.readLine().trimmed();
    QString rutaAbsolutaImagen = directorioConfig.absoluteFilePath(nombreImagen);
    territorio->setRutaImagen(rutaAbsolutaImagen);

    double dt;
    in >> dt;
    territorio->setDeltaTime(dt);

    int numPersonas;
    in >> numPersonas;

    for (int i = 0; i < numPersonas; ++i) {
        QString nombrePersona;
        int numTags;
        int tieneTablet;

        in >> nombrePersona >> numTags >> tieneTablet;

        double celX, celY, rapidez, angulo, deltaAngulo;
        in >> celX >> celY >> rapidez >> angulo >> deltaAngulo;

        Cellular *cel = new Cellular(nombrePersona, celX, celY, rapidez, angulo, deltaAngulo);
        territorio->addCellular(cel);

        for (int t = 0; t < numTags; ++t) {
            QString nombreTag;
            double tagX, tagY, tagRapidez, tagAngulo, tagDeltaAngulo;
            in >> nombreTag >> tagX >> tagY >> tagRapidez >> tagAngulo >> tagDeltaAngulo;

            EloTelTag *tag = new EloTelTag(nombreTag, nombrePersona, tagX, tagY, tagRapidez, tagAngulo, tagDeltaAngulo);
            territorio->addTag(tag);
        }

        if (tieneTablet == 1) {
            double tabX, tabY, tabRapidez, tabAngulo, tabDeltaAngulo;
            in >> tabX >> tabY >> tabRapidez >> tabAngulo >> tabDeltaAngulo;

            Tablet *tablet = new Tablet(nombrePersona, tabX, tabY, tabRapidez, tabAngulo, tabDeltaAngulo);
            territorio->addTablet(tablet);
        }
    }

    archivo.close();
    return territorio;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QString rutaArchivo = QFileDialog::getOpenFileName(
        nullptr,
        "Seleccionar Archivo de Configuración (config.txt)",
        "",
        "Text Files (*.txt);;All Files (*)"
        );

    if (rutaArchivo.isEmpty()) {
        qDebug() << "Selección de archivo cancelada por el usuario.";
        return 0;
    }

    Territory *territorio = cargarConfiguracion(rutaArchivo);
    if (!territorio) {
        return -1;
    }

    // --- LÓGICA DE LA ETAPA 4 ---
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("EloTelTag Simulation - Stage 4");

    TerritoryView *view = new TerritoryView(territorio, &mainWindow);
    mainWindow.setCentralWidget(view);

    QMenu *simMenu = mainWindow.menuBar()->addMenu("Simulation");
    QAction *playAction = simMenu->addAction("Play");
    QAction *pauseAction = simMenu->addAction("Pause");

    // Conexiones para que Play y Pause controlen el timer
    QObject::connect(playAction, &QAction::triggered, view, &TerritoryView::playSimulation);
    QObject::connect(pauseAction, &QAction::triggered, view, &TerritoryView::pauseSimulation);

    mainWindow.show();

    int execResult = a.exec();
    delete territorio;
    return execResult;
}