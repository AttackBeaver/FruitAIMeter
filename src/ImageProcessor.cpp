#include "ImageProcessor.h"
#include <QDebug>
#include <QColor>
#include <cmath>

ImageProcessor::ImageProcessor(QObject *parent)
    : QObject(parent)
{
}

QImage ImageProcessor::segmentApple(const QImage &image)
{
    qDebug() << "Сегментация яблока на изображении";

    QImage result = image.convertToFormat(QImage::Format_ARGB32);
    QImage binary(image.size(), QImage::Format_Mono);

    // Создаем бинарную маску по цвету
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QRgb pixel = image.pixel(x, y);
            if (isAppleColor(pixel)) {
                binary.setPixel(x, y, 1); // Белый - яблоко
            } else {
                binary.setPixel(x, y, 0); // Черный - фон
            }
        }
    }

    // Применяем морфологические операции для улучшения маски
    binary = applyMorphology(binary);

    // Создаем результат с выделением яблока
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            if (binary.pixelIndex(x, y) == 0) {
                // Фон - затемняем
                QRgb original = image.pixel(x, y);
                int r = qRed(original) * 0.3;
                int g = qGreen(original) * 0.3;
                int b = qBlue(original) * 0.3;
                result.setPixel(x, y, qRgb(r, g, b));
            }
            // Яблоко оставляем как есть
        }
    }

    return result;
}

QRect ImageProcessor::detectApple(const QImage &image)
{
    qDebug() << "Обнаружение яблока на изображении";

    // Простой алгоритм обнаружения по цвету
    int minX = image.width(), minY = image.height();
    int maxX = 0, maxY = 0;
    int applePixels = 0;

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            if (isAppleColor(image.pixel(x, y))) {
                applePixels++;
                if (x < minX) minX = x;
                if (x > maxX) maxX = x;
                if (y < minY) minY = y;
                if (y > maxY) maxY = y;
            }
        }
    }

    if (applePixels > 100) { // Минимальный размер яблока
        return QRect(minX, minY, maxX - minX, maxY - minY);
    }

    return QRect();
}

QVector<float> ImageProcessor::extractFeatures(const QImage &image)
{
    QVector<float> features;

    // Извлекаем простые признаки для классификации
    double redSum = 0, greenSum = 0, blueSum = 0;
    double redVar = 0, greenVar = 0, blueVar = 0;
    int pixelCount = image.width() * image.height();

    // Первый проход - средние значения
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color(image.pixel(x, y));
            redSum += color.red();
            greenSum += color.green();
            blueSum += color.blue();
        }
    }

    double redMean = redSum / pixelCount;
    double greenMean = greenSum / pixelCount;
    double blueMean = blueSum / pixelCount;

    // Второй проход - дисперсия
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color(image.pixel(x, y));
            redVar += std::pow(color.red() - redMean, 2);
            greenVar += std::pow(color.green() - greenMean, 2);
            blueVar += std::pow(color.blue() - blueMean, 2);
        }
    }

    redVar /= pixelCount;
    greenVar /= pixelCount;
    blueVar /= pixelCount;

    // Нормализуем признаки
    features.append(redMean / 255.0f);
    features.append(greenMean / 255.0f);
    features.append(blueMean / 255.0f);
    features.append(std::sqrt(redVar) / 255.0f);
    features.append(std::sqrt(greenVar) / 255.0f);
    features.append(std::sqrt(blueVar) / 255.0f);

    qDebug() << "Извлечено признаков:" << features.size();

    return features;
}

bool ImageProcessor::isAppleColor(QRgb pixel)
{
    QColor color(pixel);
    int r = color.red();
    int g = color.green();
    int b = color.blue();

    // Фильтр для красных и зеленых яблок
    bool isRedApple = (r > g * 1.5) && (r > b * 1.5) && (r > 100);
    bool isGreenApple = (g > r * 1.2) && (g > b * 1.2) && (g > 80);

    return isRedApple || isGreenApple;
}

QImage ImageProcessor::applyMorphology(const QImage &binaryImage)
{
    // Простая морфологическая операция - закрытие
    QImage result = binaryImage;

    // Применяем небольшое расширение
    for (int y = 1; y < binaryImage.height() - 1; ++y) {
        for (int x = 1; x < binaryImage.width() - 1; ++x) {
            if (binaryImage.pixelIndex(x, y) == 1) {
                // Расширяем область
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        result.setPixel(x + dx, y + dy, 1);
                    }
                }
            }
        }
    }

    return result;
}
