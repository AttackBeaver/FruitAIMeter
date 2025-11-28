#include <auroraapp.h>
#include <QtQuick>
#include "ImageAnalyzer.h"
#include "TFLiteModel.h"
#include "ImageProcessor.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.template"));
    application->setApplicationName(QStringLiteral("FruitAIMeter"));

    qmlRegisterType<ImageAnalyzer>("FruitAIMeter", 1, 0, "ImageAnalyzer");
    qmlRegisterType<TFLiteModel>("FruitAIMeter", 1, 0, "TFLiteModel");
    qmlRegisterType<ImageProcessor>("FruitAIMeter", 1, 0, "ImageProcessor");

    QScopedPointer<QQuickView> view(Aurora::Application::createView());
    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/FruitAIMeter.qml")));

    view->setColor(Qt::black);
    view->setResizeMode(QQuickView::SizeRootObjectToView);

    view->show();

    return application->exec();
}
