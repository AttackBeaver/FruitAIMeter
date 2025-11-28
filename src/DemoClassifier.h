#ifndef DEMOCLASSIFIER_H
#define DEMOCLASSIFIER_H

#include <QImage>
#include <QString>

class DemoClassifier
{
public:
    struct AnalysisResult {
        QString condition;
        int confidence;
        QString details;
        QString recommendation;
    };

    static AnalysisResult analyzeImage(const QImage &image);

private:
    static float calculateFreshnessScore(const QImage &image);
    static float analyzeColorFeatures(const QImage &image);
    static float analyzeTextureFeatures(const QImage &image);
    static float analyzeBrightnessFeatures(const QImage &image);
    static QString generateDetails(float colorScore, float textureScore, float brightnessScore);
};

#endif // DEMOCLASSIFIER_H
