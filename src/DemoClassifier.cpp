#include "DemoClassifier.h"
#include <QDebug>
#include <cmath>
#include <algorithm>

using std::max;
using std::min;

DemoClassifier::AnalysisResult DemoClassifier::analyzeImage(const QImage &image)
{
    float freshnessScore = calculateFreshnessScore(image);

    AnalysisResult result;
    result.confidence = static_cast<int>(freshnessScore * 100);

    // Определяем состояние на основе скора
    if (freshnessScore > 0.75f) {
        result.condition = "🍎 Свежее яблоко";
        result.recommendation = "Отличное качество! Можно смело употреблять.";
    } else if (freshnessScore > 0.5f) {
        result.condition = "🍏 Нормальное яблоко";
        result.recommendation = "Хорошее качество. Рекомендуется употребить в ближайшие дни.";
    } else if (freshnessScore > 0.25f) {
        result.condition = "🍎⚠️ Начинает портиться";
        result.recommendation = "Следует употребить в течение 1-2 дней.";
    } else {
        result.condition = "🍎❌ Порченое яблоко";
        result.recommendation = "Не рекомендуется к употреблению.";
    }

    // Генерируем детализированный анализ
    float colorScore = analyzeColorFeatures(image);
    float textureScore = analyzeTextureFeatures(image);
    float brightnessScore = analyzeBrightnessFeatures(image);

    result.details = generateDetails(colorScore, textureScore, brightnessScore);

    return result;
}

float DemoClassifier::calculateFreshnessScore(const QImage &image)
{
    QImage rgbImage = image.convertToFormat(QImage::Format_RGB888);

    float colorScore = analyzeColorFeatures(rgbImage);
    float textureScore = analyzeTextureFeatures(rgbImage);
    float brightnessScore = analyzeBrightnessFeatures(rgbImage);

    // Взвешенная сумма факторов
    float freshness = (colorScore * 0.5f) + (textureScore * 0.3f) + (brightnessScore * 0.2f);

    // Добавляем детерминированную вариативность для реалистичности
    float hash = 0;
    int pixelsToCheck = min(100, rgbImage.width() * rgbImage.height());
    for (int i = 0; i < pixelsToCheck; i++) {
        const uchar* pixel = rgbImage.bits() + (i * 3);
        hash += pixel[0] + pixel[1] * 2 + pixel[2] * 3;
    }
    float variation = std::fmod(hash, 0.2f) - 0.1f;

    return min(0.95f, max(0.05f, freshness + variation));
}

float DemoClassifier::analyzeColorFeatures(const QImage &image)
{
    double redSum = 0, greenSum = 0, blueSum = 0;
    int pixelCount = image.width() * image.height();

    for (int y = 0; y < image.height(); y++) {
        const uchar* scanLine = image.scanLine(y);
        for (int x = 0; x < image.width(); x++) {
            redSum += scanLine[x * 3];
            greenSum += scanLine[x * 3 + 1];
            blueSum += scanLine[x * 3 + 2];
        }
    }

    double redAvg = redSum / pixelCount;
    double greenAvg = greenSum / pixelCount;
    double blueAvg = blueSum / pixelCount;

    // Свежие яблоки имеют насыщенные красные/зеленые цвета
    float colorScore = 0.0f;

    // Красные яблоки
    if (redAvg > 150 && redAvg > greenAvg * 1.2 && redAvg > blueAvg * 1.5) {
        colorScore = 0.8f + static_cast<float>((redAvg - 150) / 105.0f * 0.2f);
    }
    // Зеленые яблоки
    else if (greenAvg > 120 && greenAvg > redAvg * 1.1 && greenAvg > blueAvg * 1.3) {
        colorScore = 0.7f + static_cast<float>((greenAvg - 120) / 135.0f * 0.3f);
    }
    // Тусклые цвета - вероятно порченое
    else {
        colorScore = max(0.1f, static_cast<float>((redAvg + greenAvg) / 510.0f));
    }

    return min(1.0f, colorScore);
}

float DemoClassifier::analyzeTextureFeatures(const QImage &image)
{
    // Анализ контрастности как признак текстуры
    QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);

    double brightnessSum = 0;
    int pixelCount = grayImage.width() * grayImage.height();

    // Первый проход - средняя яркость
    for (int y = 0; y < grayImage.height(); y++) {
        const uchar* scanLine = grayImage.scanLine(y);
        for (int x = 0; x < grayImage.width(); x++) {
            brightnessSum += scanLine[x];
        }
    }
    double meanBrightness = brightnessSum / pixelCount;

    // Второй проход - контрастность (стандартное отклонение)
    double variance = 0;
    for (int y = 0; y < grayImage.height(); y++) {
        const uchar* scanLine = grayImage.scanLine(y);
        for (int x = 0; x < grayImage.width(); x++) {
            double diff = scanLine[x] - meanBrightness;
            variance += diff * diff;
        }
    }
    double stdDev = std::sqrt(variance / pixelCount);

    // Высокая контрастность = хорошая текстура = свежее яблоко
    float textureScore = min(1.0f, static_cast<float>(stdDev / 80.0f));

    return textureScore;
}

float DemoClassifier::analyzeBrightnessFeatures(const QImage &image)
{
    double brightnessSum = 0;
    int pixelCount = image.width() * image.height();

    for (int y = 0; y < image.height(); y++) {
        const uchar* scanLine = image.scanLine(y);
        for (int x = 0; x < image.width(); x++) {
            uchar r = scanLine[x * 3];
            uchar g = scanLine[x * 3 + 1];
            uchar b = scanLine[x * 3 + 2];
            brightnessSum += r * 0.299 + g * 0.587 + b * 0.114;
        }
    }

    double avgBrightness = brightnessSum / pixelCount;

    // Свежие яблоки обычно яркие
    if (avgBrightness > 180) return 1.0f;
    if (avgBrightness > 140) return 0.7f;
    if (avgBrightness > 100) return 0.4f;
    return 0.2f;
}

QString DemoClassifier::generateDetails(float colorScore, float textureScore, float brightnessScore)
{
    QStringList details;

    // Анализ цвета
    if (colorScore > 0.7f) {
        details << "• Цвет: насыщенный и яркий";
    } else if (colorScore > 0.4f) {
        details << "• Цвет: в пределах нормы";
    } else {
        details << "• Цвет: тусклый, возможные признаки порчи";
    }

    // Анализ текстуры
    if (textureScore > 0.6f) {
        details << "• Текстура: четкая и контрастная";
    } else if (textureScore > 0.3f) {
        details << "• Текстура: умеренная контрастность";
    } else {
        details << "• Текстура: размытая, возможная мягкость";
    }

    // Анализ яркости
    if (brightnessScore > 0.7f) {
        details << "• Блеск: присутствует, поверхность отражает свет";
    } else if (brightnessScore > 0.4f) {
        details << "• Блеск: умеренный";
    } else {
        details << "• Блеск: отсутствует, поверхность матовая";
    }

    return details.join("\n");
}
