#include "ImageAnalyzer.h"
#include "TFLiteModel.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QImage>
#include <cstdlib>

ImageAnalyzer::ImageAnalyzer(QObject *parent)
    : QObject(parent)
    , m_classificationModel(new TFLiteModel(this))
    , m_modelsLoaded(false)
{
    // Инициализация генератора случайных чисел
    qsrand(QDateTime::currentMSecsSinceEpoch());
}

ImageAnalyzer::~ImageAnalyzer()
{
    // Ресурсы автоматически освобождаются благодаря родительской иерархии Qt
}

bool ImageAnalyzer::loadModels()
{
    qDebug() << "Загрузка моделей...";

    // Загружаем модель классификации
    bool success = m_classificationModel->loadModel("models/apple_freshness.tflite");

    m_modelsLoaded = success;
    emit modelsLoadedChanged();

    qDebug() << "Загрузка моделей:" << (success ? "Успешно" : "Не удалось");
    return success;
}

void ImageAnalyzer::analyzeImage(const QString &imagePath)
{
    qDebug() << "Начало анализа изображения:" << imagePath;

    // Загружаем изображение
    QImage image;
    if (imagePath.startsWith("qrc:/")) {
        // Загрузка из ресурсов
        QString resourcePath = imagePath.mid(3); // Убираем "qrc"
        if (!image.load(resourcePath)) {
            qDebug() << "Ошибка загрузки из ресурсов:" << resourcePath;
            emit analysisFailed("Не удалось загрузить демо-изображение");
            return;
        }
        qDebug() << "Загружено из ресурсов:" << resourcePath;
    } else {
        // Загрузка из файловой системы
        if (!image.load(imagePath)) {
            qDebug() << "Ошибка загрузки из файла:" << imagePath;
            emit analysisFailed("Не удалось загрузить изображение");
            return;
        }
        qDebug() << "Загружено из файла:" << imagePath;
    }

    if (image.isNull()) {
        qDebug() << "Ошибка: изображение пустое";
        emit analysisFailed("Не удалось загрузить изображение");
        return;
    }

    qDebug() << "Изображение загружено успешно, размер:" << image.size();

    // Всегда выполняем анализ
    performRealAnalysis(image);
}

void ImageAnalyzer::performRealAnalysis(const QImage &image)
{
    qDebug() << "Выполнение анализа изображения...";

    if (!m_classificationModel->isLoaded()) {
        qDebug() << "Ошибка: модель не загружена";
        emit analysisFailed("❌ Модель не загружена");
        return;
    }

    QString result = m_classificationModel->classifyImage(image);

    qDebug() << "Анализ завершен, результат:" << result;
    emit analysisComplete(result);
}

bool ImageAnalyzer::modelsLoaded() const
{
    return m_modelsLoaded;
}
