#ifndef TFLITEPARSER_H
#define TFLITEPARSER_H

#include <QByteArray>
#include <QVector>
#include <QMap>

class TFLiteParser
{
public:
    struct Tensor {
        QVector<int> shape;
        QVector<float> data;
        int type;
    };

    struct Operator {
        int opcode;
        QVector<int> inputs;
        QVector<int> outputs;
    };

    bool loadModel(const QByteArray& modelData);
    QVector<float> runInference(const QVector<float>& input);
    QString getModelInfo() const;

private:
    bool parseModel(const QByteArray& modelData);
    float simulateInference(const QVector<float>& input);

    QByteArray m_modelData;
    QVector<Tensor> m_tensors;
    QVector<Operator> m_operators;
    QMap<int, QString> m_opcodeNames;
    bool m_loaded = false;
};

#endif // TFLITEPARSER_H
