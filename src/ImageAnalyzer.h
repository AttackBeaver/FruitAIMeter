#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H

#include <QObject>
#include <QString>
#include <QImage>

// Предварительное объявление чтобы избежать циклических включений
class TFLiteModel;

class ImageAnalyzer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool modelsLoaded READ modelsLoaded NOTIFY modelsLoadedChanged)

public:
    explicit ImageAnalyzer(QObject *parent = nullptr);
    ~ImageAnalyzer();

    // Основной метод анализа изображения
    Q_INVOKABLE void analyzeImage(const QString &imagePath);

    // Загрузка моделей
    Q_INVOKABLE bool loadModels();

    // Проверка загружены ли модели
    bool modelsLoaded() const;

signals:
    void analysisComplete(const QString &result);
    void analysisFailed(const QString &error);
    void modelsLoadedChanged();

private:
    TFLiteModel* m_classificationModel;
    bool m_modelsLoaded;

    // Реальный анализ с использованием моделей
    void performRealAnalysis(const QImage &image);
};

#endif // IMAGEANALYZER_H
