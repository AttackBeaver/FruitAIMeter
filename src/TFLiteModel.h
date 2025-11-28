#ifndef TFLITEMODEL_H
#define TFLITEMODEL_H

#include <QObject>
#include <QString>
#include <QImage>

class TFLiteModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool usingRealModel READ isUsingRealModel NOTIFY modelModeChanged)

public:
    explicit TFLiteModel(QObject *parent = nullptr);
    ~TFLiteModel();

    // Загрузка модели
    Q_INVOKABLE bool loadModel(const QString &modelPath);

    // Классификация изображения
    Q_INVOKABLE QString classifyImage(const QImage &image);

    // Проверка загружена ли модель
    Q_INVOKABLE bool isLoaded() const;

    // Используется ли реальная модель
    Q_INVOKABLE bool isUsingRealModel() const;

signals:
    void modelModeChanged();

private:
    bool m_loaded;
    bool m_usingRealModel;

    // Демо-классификация
    QString classifyWithDemo(const QImage &image);

    // Реальная классификация (будет добавлена позже)
    QString classifyWithRealModel(const QImage &image);
};

#endif // TFLITEMODEL_H
