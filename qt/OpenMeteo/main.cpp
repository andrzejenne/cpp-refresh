#include <QApplication>
#include <QLoggingCategory>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlNetworkAccessManagerFactory>
#include <QQuickStyle>
#include <QStringList>

// QtQuick's Image element fetches the OSM map tiles over the network. The public
// OpenStreetMap tile servers reject requests that use a generic library
// User-Agent, so we inject a descriptive one onto every QML network request.
namespace {
class UANetworkManager : public QNetworkAccessManager
{
public:
    using QNetworkAccessManager::QNetworkAccessManager;

protected:
    QNetworkReply* createRequest(Operation op, const QNetworkRequest& request,
                                 QIODevice* outgoingData) override
    {
        QNetworkRequest req(request);
        req.setHeader(QNetworkRequest::UserAgentHeader,
                      QByteArrayLiteral("OpenMeteoQtDemo/1.0 (cpp-refresh learning project)"));
        return QNetworkAccessManager::createRequest(op, req, outgoingData);
    }
};

class UAFactory : public QQmlNetworkAccessManagerFactory
{
public:
    QNetworkAccessManager* create(QObject* parent) override
    {
        return new UANetworkManager(parent);
    }
};
} // namespace

int main(int argc, char* argv[])
{
    // QApplication (not QGuiApplication) because QtCharts pulls in the widgets stack.
    QApplication app(argc, argv);

    // Use the Material style; individual colors are themed from QML via the
    // Theme singleton (see Main.qml). Must be set before the engine loads QML.
    QQuickStyle::setStyle(QStringLiteral("Material"));

    // -v / --verbose enables the "openmeteo" debug category and QML-side logging.
    const QStringList args = app.arguments();
    const bool verbose = args.contains(QStringLiteral("-v"))
                         || args.contains(QStringLiteral("--verbose"));
    QLoggingCategory::setFilterRules(verbose ? QStringLiteral("openmeteo.debug=true")
                                             : QStringLiteral("openmeteo.debug=false"));
    if (verbose)
        qInfo("OpenMeteo: verbose mode on");

    QQmlApplicationEngine engine;

    UAFactory namFactory;
    engine.setNetworkAccessManagerFactory(&namFactory);

    engine.rootContext()->setContextProperty(QStringLiteral("appVerbose"), verbose);

    engine.load(QUrl(QStringLiteral("qrc:/App/Main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
