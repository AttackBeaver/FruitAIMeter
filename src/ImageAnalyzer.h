#ifndef IMAGEANALYZER_H
#define IMAGEANALYZER_H

#include <QObject>
#include <QString>
#include <QImage>

class TFLiteModel;

class ImageAnalyzer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool modelsLoaded READ modelsLoaded NOTIFY modelsLoadedChanged)

public:
    explicit ImageAnalyzer(QObject *parent = nullptr);
    ~ImageAnalyzer();

    Q_INVOKABLE void analyzeImage(const QString &imagePath);

    Q_INVOKABLE bool loadModels();

    bool modelsLoaded() const;

signals:
    void analysisComplete(const QString &result);
    void analysisFailed(const QString &error);
    void modelsLoadedChanged();

private:
    TFLiteModel* m_classificationModel;
    bool m_modelsLoaded;

    void performRealAnalysis(const QImage &image);
};

#endif // IMAGEANALYZER_H
