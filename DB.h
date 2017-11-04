/*
# Copyright (C) 2016 Michal Jamroz
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include <QObject>
#include <QString>
#include <QtSql>
#include <QStandardPaths>
#include <QDir>
#include <QVariantMap>
class DB : public QObject
{
    Q_OBJECT
private:
    QDir dir;
    bool connected = false;
    void connect() {
        if (!connected) {
            db = QSqlDatabase::addDatabase("QSQLITE");
            dir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
            db.setDatabaseName(DB::dir.absoluteFilePath("offline-data.db"));
            if (!db.open())
                qDebug() << "Error: connection with database fail";
            else {
                connected = true;
                qDebug() << "Database: connection ok";
            }
        }
    }
public:
    QSqlDatabase db;
    QSqlQuery query;
    QVariantMap ask(int plant_id, int i) {
        connect();
        QSqlQuery query;
        query.prepare("SELECT name_lat,name_pol,author,licence,img,plant_id FROM plants WHERE plant_id=(:id)");
        query.bindValue(":id", plant_id);
        if (query.exec() && query.next()) {
            QVariantMap map;
            map.insert("name_lat",  query.value(0).toString());
            map.insert("name_pol",  query.value(1).toString());
            map.insert("author",  query.value(2).toString());
            map.insert("lic",  query.value(3).toString());
            map.insert("plant_id",  QVariant(plant_id));

            // write blob data to local image file
            QString img_path =  dir.absoluteFilePath(QString::number(i) + ".jpg");
            QString img_path_ =  "file://" + img_path;
            map.insert("img", img_path_);
            QByteArray outByteArray = query.value( 4 ).toByteArray();
            QFile img(img_path);
            img.open(QIODevice::WriteOnly);
            img.write(outByteArray);
            img.close();
            return map;

        }
    }
};
