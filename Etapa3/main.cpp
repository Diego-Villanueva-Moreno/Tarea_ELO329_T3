#include <QApplication>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include "Territory.h"
#include "territoryview.h"

Territory* cargarConfiguracion(const QString &rutaArchivo) {
    QFile archivo(rutaArchivo);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: No se pudo abrir el archivo de configuración en la ruta:" << rutaArchivo;
        return nullptr;
    }

    // 1. Obtener el directorio donde reside el archivo .txt seleccionado
    QFileInfo fileInfo(archivo);
    QDir directorioConfig = fileInfo.dir();

    QTextStream in(&archivo);
    Territory *territorio = new Territory();

    // Línea 1: Usar readLine() para extraer limpiamente el nombre de la imagen
    QString nombreImagen = in.readLine().trimmed();

    // Combinar la ruta del directorio del .txt con el nombre de la imagen
    QString rutaAbsolutaImagen = directorioConfig.absoluteFilePath(nombreImagen);
    territorio->setRutaImagen(rutaAbsolutaImagen);

    // Línea 2: <delta_tiempo>
    double dt;
    in >> dt;
    territorio->setDeltaTime(dt);

    // Línea 3: <número de personas>
    int numPersonas;
    in >> numPersonas;

    // Iterar para procesar cada persona del archivo
    for (int i = 0; i < numPersonas; ++i) {
        QString nombrePersona;
        int numTags;
        int tieneTablet;

        // Leer datos de la cabecera de la persona
        in >> nombrePersona >> numTags >> tieneTablet;

        // Leer coordenadas y movimiento del celular
        double celX, celY, rapidez, angulo, deltaAngulo;
        in >> celX >> celY >> rapidez >> angulo >> deltaAngulo;

        // Crear e instanciar el celular
        Cellular *cel = new Cellular(nombrePersona, celX, celY, rapidez, angulo, deltaAngulo);
        territorio->addCellular(cel);

        // --- Leer los EloTelTags ---
        for (int t = 0; t < numTags; ++t) {
            QString nombreTag;
            double tagX, tagY, tagRapidez, tagAngulo, tagDeltaAngulo;
            // Extraemos los datos separados por espacio [cite: 30]
            in >> nombreTag >> tagX >> tagY >> tagRapidez >> tagAngulo >> tagDeltaAngulo;

            // Creamos el tag y lo agregamos a la lista
            EloTelTag *tag = new EloTelTag(nombreTag, nombrePersona, tagX, tagY, tagRapidez, tagAngulo, tagDeltaAngulo);
            territorio->addTag(tag);
        }

        // --- Leer la Tablet (si tiene) ---
        if (tieneTablet == 1) {
            double tabX, tabY, tabRapidez, tabAngulo, tabDeltaAngulo;
            // Extraemos los datos separados por espacio [cite: 31]
            in >> tabX >> tabY >> tabRapidez >> tabAngulo >> tabDeltaAngulo;

            // Creamos la tablet y la agregamos a la lista
            Tablet *tablet = new Tablet(nombrePersona, tabX, tabY, tabRapidez, tabAngulo, tabDeltaAngulo);
            territorio->addTablet(tablet);
        }
    }

    archivo.close();
    return territorio;
}

int main(int argc, char *argv[]) {
    // Inicializar la infraestructura de la aplicación gráfica de Qt
    QApplication a(argc, argv);

    // Desplegar QFileDialog de manera estática para seleccionar el archivo .txt
    QString rutaArchivo = QFileDialog::getOpenFileName(
        nullptr,
        "Seleccionar Archivo de Configuración (confix.txt)",
        "",
        "Text Files (*.txt);;All Files (*)"
        );

    // Si el usuario cierra la ventana o cancela la selección, salimos limpiamente
    if (rutaArchivo.isEmpty()) {
        qDebug() << "Selección de archivo cancelada por el usuario.";
        return 0;
    }

    // Cargar los datos utilizando la función con resolución de ruta absoluta
    Territory *territorio = cargarConfiguracion(rutaArchivo);
    if (!territorio) {
        return -1;
    }

    // Crear la vista del territorio pasando el modelo cargado
    // Por requerimiento de la Etapa 1, se muestra directo sin menú de barra superior
    TerritoryView view(territorio);
    view.setWindowTitle("EloTelTag Simulation - Stage 3");
    view.show();

    // Iniciar el loop de eventos de Qt
    int execResult = a.exec();

    // Liberar la memoria del modelo al cerrar la aplicación
    delete territorio;

    return execResult;
}