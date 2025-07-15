#include <auroraapp.h>
#include <QtQuick>

#include "bleadvertiser.h"
#include "blescanner.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> app(Aurora::Application::application(argc, argv));
    app->setOrganizationName(QStringLiteral("ru.template"));
    app->setApplicationName(QStringLiteral("BleTest"));

    BleAdvertiser advertiser;
    BleScanner    scanner;

    QScopedPointer<QQuickView> view(Aurora::Application::createView());
    // перед отдачей QML даём ему доступ к backend-объектам
    view->rootContext()->setContextProperty("bleAdvertiser", &advertiser);
    view->rootContext()->setContextProperty("bleScanner",    &scanner);

    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/BleTest.qml")));
    view->show();

    return app->exec();
}
