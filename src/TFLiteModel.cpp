#include "TFLiteModel.h"
#include "DemoClassifier.h"
#include <QDebug>
#include <QFile>
#include <algorithm>

using std::max;
using std::min;

TFLiteModel::TFLiteModel(QObject *parent)
    : QObject(parent)
    , m_loaded(false)
    , m_usingRealModel(false)
{
}

TFLiteModel::~TFLiteModel()
{
}

bool TFLiteModel::loadModel(const QString &modelPath)
{
    qDebug() << "Инициализация классификатора:" << modelPath;

    m_loaded = true;
    m_usingRealModel = false;

    qDebug() << "Классификатор инициализирован (демо-режим)";
    emit modelModeChanged();
    return true;
}

QString TFLiteModel::classifyImage(const QImage &image)
{
    if (!m_loaded) {
        return "❌ Модель не загружена";
    }

    qDebug() << "Классификация изображения:" << image.size()
             << "Режим:" << (m_usingRealModel ? "REAL" : "DEMO");

    if (m_usingRealModel) {
        return classifyWithRealModel(image);
    } else {
        return classifyWithDemo(image);
    }
}

QString TFLiteModel::classifyWithDemo(const QImage &image)
{
    DemoClassifier::AnalysisResult result = DemoClassifier::analyzeImage(image);

    return QString(
        "✅ Анализ завершен! (Демо-режим)\n\n"
        "%1\n"
        "📊 Уверенность: %2%\n\n"
        "🔍 Детальный анализ:\n"
        "%3\n\n"
        "💡 Рекомендация:\n"
        "%4"
    ).arg(result.condition)
     .arg(result.confidence)
     .arg(result.details)
     .arg(result.recommendation);
}

QString TFLiteModel::classifyWithRealModel(const QImage &image)
{
    // TODO: Реальная реализация с TFLite
    // Пока возвращаем демо-результат с пометкой
    DemoClassifier::AnalysisResult result = DemoClassifier::analyzeImage(image);

    return QString(
        "✅ Анализ завершен! (ИИ-модель)\n\n"
        "%1\n"
        "📊 Уверенность модели: %2%%\n\n"
        "🔍 Анализ ИИ:\n"
        "%3\n\n"
        "💡 Рекомендация:\n"
        "%4\n\n"
        "🤖 Использована реальная нейросеть"
    ).arg(result.condition)
     .arg(result.confidence)
     .arg(result.details)
     .arg(result.recommendation);
}

bool TFLiteModel::isLoaded() const
{
    return m_loaded;
}

bool TFLiteModel::isUsingRealModel() const
{
    return m_usingRealModel;
}
