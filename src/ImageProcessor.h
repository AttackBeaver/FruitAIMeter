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

    Q_INVOKABLE QImage segmentApple(const QImage &image);

    Q_INVOKABLE QRect detectApple(const QImage &image);

    Q_INVOKABLE QVector<float> extractFeatures(const QImage &image);

private:
    bool isAppleColor(QRgb pixel);

    QImage applyMorphology(const QImage &binaryImage);
};

#endif // IMAGEPROCESSOR_H
