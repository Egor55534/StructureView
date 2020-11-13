#include "datastruct.h"

DataStruct::DataStruct(QByteArray * data)
{

    json = QJsonDocument::fromJson(* data);

    if(json.isNull())
        getError("Некоhректный json");
}

void DataStruct::getError(QString nameError)
{
    error = nameError;
    ok = false;
}

void DataStruct::parceStruct()
{
    bool isPresentMethodOrParam = false;
    QJsonObject jsn = json.object();

    if(jsn.value("structName").isString())
        structName = jsn.value("structName").toString();
    else
        getError("Отсутствует название структуры");

    if(jsn.value("objectName").isString() && ok)
        objectName = jsn.value("objectName").toString();
    else if(ok)
        getError("Отсутствует имя объекта структуры");

    if(jsn.value("properties").isObject() && ok)
    {
        isPresentMethodOrParam = true;
        jsn = json.object().value("properties").toObject();
        int count = jsn.count();
        QJsonObject::const_iterator iter = jsn.constBegin();
        for(int i = 0; i < count; i++)
        {
            elemType.append(iter.value().toObject().take(QString::fromStdString("type")).toString());
            elemName.append(iter.value().toObject().take(QString::fromStdString("name")).toString());
            elemValue.append(iter.value().toObject().take(QString::fromStdString("value")).toInt());
            elemIsStatic.append(iter.value().toObject().take(QString::fromStdString("isStatic")).toBool());
            iter++;
        }
    }
    if(json.object().value("methods").isObject() && ok)
    {
        isPresentMethodOrParam = true;
        jsn = json.object().value("methods").toObject();
        int count = jsn.count();
        QJsonObject::const_iterator iter = jsn.constBegin();
        for(int i = 0; i < count; i++)
        {
            elemType.append(iter.value().toObject().take(QString::fromStdString("type")).toString());
            elemName.append(iter.value().toObject().take(QString::fromStdString("name")).toString());
            elemValue.append(1);
            elemIsStatic.append(iter.value().toObject().take(QString::fromStdString("isStatic")).toBool());
            iter++;
        }
    }
    if(!isPresentMethodOrParam && ok)
        getError("Отсутствуют методы или параметры");
}
