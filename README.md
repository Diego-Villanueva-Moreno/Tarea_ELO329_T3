# Tarea 3 - ELO329: Simulador Gráfico en C++ y Qt

## Descripción del Proyecto
El presente proyecto implementa un simulador gráfico desarrollado en C++ y Qt que emula la interacción entre dispositivos de rastreo (EloTelTags), celulares y tablets dentro de un territorio confinado. La ejecución del software está guiada por el paradigma de programación conducida por eventos, utilizando las herramientas nativas de Qt para la interfaz y la gestión del tiempo real.

## Equipo de Trabajo
* Eduardo Canales - rol: 
* Martin Layseca - rol: 202330556-1
* Diego Villanueva - rol: 
* Joaquin Torres - rol:

## Instrucciones de Ejecución
Para compilar y ejecutar el simulador, sigue estos pasos:
1. Abra una terminal de comandos en la raíz de la carpeta `Etapa_4`.
2. Ejecute el comando `make` (o `mingw32-make`) para compilar el proyecto usando el Makefile provisto.
3. Ejecute `make run` para iniciar la simulación.
4. En la ventana de diálogo, seleccione el archivo `confix.txt` ubicado en la carpeta `config/`. Asegúrese de que la imagen del mapa referenciada en la primera línea esté en ese mismo directorio.

## Desarrollo Iterativo e Incremental
El proyecto fue construido y validado a través de las siguientes fases:
* **Etapa 1:** Lectura base del archivo de configuración utilizando `QFileDialog`, instanciando los objetos `Cellular` y sus vistas.
* **Etapa 2:** Integración de las clases `EloTelTag` y `Tablet`, implementando el motor de simulación para el movimiento y rebote en los límites del mapa.
* **Etapa 3:** Incorporación de la clase `ETNube`. Se implementó la lógica de radares de búsqueda, permitiendo a los Tags y Tablets detectar celulares cercanos y reportar su ubicación mediante menús emergentes estáticos.
* **Etapa 4:** Integración de control de tiempo (Play y Pause). La ventana de rastreo (Find My) fue refactorizada para actualizar datos en tiempo real cada 1 segundo. Se incluyó el dibujado dinámico de trayectorias (Traza).
* **Extra-crédito (GFindMy):** Representación gráfica en una ventana independiente que actualiza cada 1 segundo las posiciones registradas en la nube para un usuario específico.

##  Diagrama de Clases
A continuación se expone la arquitectura del sistema final, detallando las relaciones de herencia y dependencia entre los componentes centrales de la simulación:

```mermaid
classDiagram
    %% Relaciones de Herencia
    Equipo <|-- Cellular
    Equipo <|-- EloTelTag
    Equipo <|-- Tablet

    %% Agregación: Territory contiene los dispositivos
    Territory o-- Cellular
    Territory o-- EloTelTag
    Territory o-- Tablet
    Territory *-- ETNube

    %% Dependencias de Vistas
    TerritoryView ..> Territory : usa
    TerritoryView ..> CellularView : dibuja
    TerritoryView ..> EloTelTagView : dibuja
    TerritoryView ..> TabletView : dibuja

    %% Dependencias de las Ventanas de la Etapa 4
    TerritoryView ..> FindMyWindow : instancia
    TerritoryView ..> GFindMyWindow : instancia
    FindMyWindow ..> ETNube : consulta
    GFindMyWindow ..> ETNube : consulta

    %% Definición exacta de clases según los .h
    class Equipo {
        #QString nombre
        #double x
        #double y
        #double rapidez
        #double angulo
        #double deltaAngulo
        +Equipo(QString nombre, double x, double y, double rapidez, double angulo, double deltaAngulo)
        +getNombre() QString
        +getX() double
        +getY() double
        +mover(double dt, int anchoMapa, int altoMapa) void
    }

    class Cellular {
        -QString dueno
        +Cellular(QString dueno, double x, double y, double rapidez, double angulo, double deltaAngulo)
        +getDueno() QString
    }

    class EloTelTag {
        -QString dueno
        +EloTelTag(QString nombreTag, QString dueno, double x, double y, double rapidez, double angulo, double deltaAngulo)
        +getDueno() QString
    }

    class Tablet {
        -QString dueno
        +Tablet(QString dueno, double x, double y, double rapidez, double angulo, double deltaAngulo)
        +getDueno() QString
    }

    class ETNube {
        -QMap reportesPorEquipo
        +reportarPosicion(QString nombreEquipo, QString dueno, QPointF posicionReportada, QString reportadoPor, double tiempoSimulacion) void
        +reportesPorDueno(QString dueno) QVector
    }

    class Territory {
        -double deltaTime
        -QString rutaImagen
        +moverTodos(int ancho, int alto) void
        +addCellular(Cellular* cel) void
        +addTag(EloTelTag* tag) void
        +addTablet(Tablet* tablet) void
    }

    class TerritoryView {
        -QTimer reloj
        -double tiempoSimulacion
        +playSimulation() void
        +pauseSimulation() void
        #paintEvent(QPaintEvent* event) void
        #mousePressEvent(QMouseEvent* event) void
    }

    class FindMyWindow {
        -QTimer timer
        -QString m_dueno
        -ETNube* m_nube
        -actualizarDatos() void
    }

    class GFindMyWindow {
        -QTimer m_timer
        -QString m_dueno
        -ETNube* m_nube
        -QPixmap m_fondo
        #paintEvent(QPaintEvent* event) void
    }
