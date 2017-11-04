#ifndef MXPREDICT_H
#define MXPREDICT_H

#include <QObject>
#include <QVariantList>
#include <mxnet/c_predict_api.h>
#include "bufferfile.h"
#include "DB.h"
class MXPredict : public QObject
{

    Q_PROPERTY(QVariantList things READ getData NOTIFY dataChanged)
    Q_OBJECT
public:
    explicit MXPredict(QObject *parent = nullptr);
    Q_INVOKABLE int predict(QString path);
    Q_INVOKABLE bool modelExists();

    Q_INVOKABLE void setData(QVariantList list);
    Q_INVOKABLE QVariantList getData() const {return list;};
private:
    QVariantList list;
    std::vector<std::string> LoadSynset(std::string synset_file);
    void GetImageFile(QString image_file, mx_float* image_data, const int channels, const int width);
    void PrintOutputResult(const std::vector<float>& data, const std::vector<std::string>& synset);
    std::vector<std::string> synset;
    DB database;
    BufferFile param_data;
    BufferFile json_data;
    bool synset_loaded = false;
    bool params_loaded = false;
    bool json_loaded = false;
signals:
    void dataChanged();
public slots:
};

#endif // MXPREDICT_H
