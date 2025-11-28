#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QImage>
#include <QRect>

class ImageProcessor : public QObject
{
    Q_OBJECT

public:
    explicit ImageProcessor(QObject *parent = nullptr);

    // Базовая сегментация яблока по цвету
    Q_INVOKABLE QImage segmentApple(const QImage &image);

    // Обнаружение яблока на изображении
    Q_INVOKABLE QRect detectApple(const QImage &image);

    // Извлечение признаков для классификации
    Q_INVOKABLE QVector<float> extractFeatures(const QImage &image);

private:
    // Фильтр по цвету для яблок (красные/зеленые)
    bool isAppleColor(QRgb pixel);

    // Морфологические операции
    QImage applyMorphology(const QImage &binaryImage);
};

#endif // IMAGEPROCESSOR_H
