#include "mxpredict.h"
#include <QObject>
#include <QImage>
#include <QString>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <Qt>
#include <fstream>
#include <iostream>
#include "bufferfile.h"
#include <QVariantMap>
#include <QVariant>
#include <QVariantList>

static bool QPairSecondComparer(const QPair<int, double>& first, const QPair<int, double>& second)
{
    return first.second >= second.second;
}

MXPredict::MXPredict(QObject *parent)
{
}
void MXPredict::setData(QVariantList list)
{
    list = list;
}

bool MXPredict::modelExists() {

    QDir dir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    return dir.exists("resnet-imagenet-101-0-symbol.json")
                    && dir.exists("resnet-imagenet-101-0-0123.params")
                    && dir.exists("offline-data.db")
                    && dir.exists("synset.txt");
}
void MXPredict::PrintOutputResult(const std::vector<float>& data, const std::vector<std::string>& synset) {
    QList<QPair<int, double>> results;

    for ( int i = 0; i < static_cast<int>(data.size()); i++ ) {

        QPair<int, double> pair;
        pair.first = QString::fromStdString(synset[i].c_str()).toInt();
        pair.second = data[i];
        results.append(pair);
    }
    std::sort(results.begin(), results.end(), QPairSecondComparer);

    list.clear();
    for (int i = 0; i < 10; ++i) {
        list.append(database.ask(results.at(i).first, i));
    }
        emit dataChanged();
    results.clear();
}
void MXPredict::GetImageFile(QString image_file, mx_float* image_data, const int channels, const int width) {
    QImage im(image_file);
    im = im.scaled(width, width, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    int size = width * width * channels;

    mx_float* ptr_image_r = image_data;
    mx_float* ptr_image_g = image_data + size / 3;
    mx_float* ptr_image_b = image_data + size / 3 * 2;

    for (int i = 0; i < im.height(); i++) {
        for (int j = 0; j < im.width(); j++) {
            QRgb bits = im.pixel(i, j);
            *ptr_image_r++ = static_cast<mx_float>(qRed(bits)) ;
            *ptr_image_g++ = static_cast<mx_float>(qGreen(bits));
            *ptr_image_b++ = static_cast<mx_float>(qBlue(bits)) ;
        }
    }
}
std::vector<std::string> MXPredict::LoadSynset(std::string synset_file) {
    std::ifstream fi(synset_file.c_str());

    if ( !fi.is_open() ) {
        qDebug()  << "Error opening synset file ";
    }

    std::vector<std::string> output;

    std::string synset, lemma;
    while ( fi >> synset ) {
        getline(fi, lemma);
        output.push_back(lemma);
    }

    fi.close();

    return output;
}
int MXPredict::predict(QString path) {

    QDir dir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!synset_loaded) {
        synset = LoadSynset(dir.absoluteFilePath("synset.txt").toStdString());
        synset_loaded = true;
    } if (!params_loaded) {
        param_data.load(dir.absoluteFilePath("resnet-imagenet-101-0-0123.params").toStdString());
        params_loaded = true;
    } if (!json_loaded) {
        json_data.load(dir.absoluteFilePath("resnet-imagenet-101-0-symbol.json").toStdString());
        json_loaded = true;
    }

    mx_uint num_input_nodes = 1;  // 1 for feedforward
    const char* input_key[1] = {"data"};
    const char** input_keys = input_key;

    // Image size and channels
    int width = 224;
    int channels = 3;

    const mx_uint input_shape_indptr[2] = { 0, 4 };
    const mx_uint input_shape_data[4] = { 1,
                                          static_cast<mx_uint>(channels),
                                          static_cast<mx_uint>(width),
                                          static_cast<mx_uint>(width)};
    PredictorHandle pred_hnd = 0;
    if (json_data.GetLength() == 0 ||
                    param_data.GetLength() == 0) {
        return -1;
    }


    // Create Predictor
    MXPredCreate((const char*)json_data.GetBuffer(),
                 (const char*)param_data.GetBuffer(),
                 static_cast<size_t>(param_data.GetLength()),
                 1,
                 0,
                 num_input_nodes,
                 input_keys,
                 input_shape_indptr,
                 input_shape_data,
                 &pred_hnd);
    int image_size = width * width * channels;
    std::vector<mx_float> image_data = std::vector<mx_float>(image_size);
    GetImageFile(path, image_data.data(), channels, width);
    MXPredSetInput(pred_hnd, "data", image_data.data(), image_size);

    // Do Predict Forward
    MXPredForward(pred_hnd);

    mx_uint output_index = 0;

    mx_uint *shape = 0;
    mx_uint shape_len;

    // Get Output Result
    MXPredGetOutputShape(pred_hnd, output_index, &shape, &shape_len);

    size_t size = 1;
    for (mx_uint i = 0; i < shape_len; ++i) size *= shape[i];

    std::vector<float> data(size);

    MXPredGetOutput(pred_hnd, output_index, &(data[0]), size);

    // Release Predictor
    MXPredFree(pred_hnd);
    image_data.clear();

    // Print Output Data
    PrintOutputResult(data, synset);
    return 1;
}
