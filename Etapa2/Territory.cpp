#include "Territory.h"

Territory::Territory() : deltaTime(0.0) {}

Territory::~Territory() {
    qDeleteAll(celulares);
    qDeleteAll(tags);
    qDeleteAll(tablets);
}

void Territory::setRutaImagen(const QString& ruta) { rutaImagen = ruta; }
QString Territory::getRutaImagen() const { return rutaImagen; }

void Territory::setDeltaTime(double dt) { deltaTime = dt; }
double Territory::getDeltaTime() const { return deltaTime; }

void Territory::addCellular(Cellular* cel) { celulares.append(cel); }
const QVector<Cellular*>& Territory::getCelulares() const { return celulares; }

void Territory::addTag(EloTelTag* tag) { tags.append(tag); }
const QVector<EloTelTag*>& Territory::getTags() const { return tags; }

void Territory::addTablet(Tablet* tablet) { tablets.append(tablet); }
const QVector<Tablet*>& Territory::getTablets() const { return tablets; }

void Territory::moverTodos(int ancho, int alto) {
    for(Cellular* c : celulares) c->mover(deltaTime, ancho, alto);
    for(EloTelTag* t : tags) t->mover(deltaTime, ancho, alto);
    for(Tablet* tb : tablets) tb->mover(deltaTime, ancho, alto);
}