#include "openmeteoservice.h"
#include "weathercontroller.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, [] { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // Composition root: the ONE place that names a concrete provider. Swap
    // OpenMeteoService for a fake/cached implementation here (e.g. in a test
    // main) without touching the controller or the UI.
    auto *service = new OpenMeteoService(&app);
    auto *controller =
        engine.singletonInstance<WeatherController *>("Weather", "WeatherController");
    controller->setService(service);

    engine.loadFromModule("Weather", "Main");
    return app.exec();
}
