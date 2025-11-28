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

    Q_INVOKABLE bool loadModel(const QString &modelPath);

    Q_INVOKABLE QString classifyImage(const QImage &image);

    Q_INVOKABLE bool isLoaded() const;

    Q_INVOKABLE bool isUsingRealModel() const;

signals:
    void modelModeChanged();

private:
    bool m_loaded;
    bool m_usingRealModel;

    QString classifyWithDemo(const QImage &image);

    QString classifyWithRealModel(const QImage &image);
};

#endif // TFLITEMODEL_H
