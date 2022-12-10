#include "JsonEditor.hpp"

#include <QFile>
#include <QJsonArray>

namespace Json
{
    JsonEditor::JsonEditor(const QString& fileName) :
        _fileName{ fileName },
        _jsonSource{ LoadJson() }
    { }

    JsonEditor::JsonEditor(QString&& fileName) :
        _fileName{ std::move(fileName) },
        _jsonSource{ LoadJson() }
    { }

    std::unique_ptr<QJsonDocument> JsonEditor::LoadJson() const
    {
        if (QFile jsonFile{ _fileName }; jsonFile.exists())
        {
            jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
            return std::make_unique<QJsonDocument>(QJsonDocument::fromJson(jsonFile.readAll()));
        }

        return std::make_unique<QJsonDocument>();
    }

    // refresh entire-db
    void JsonEditor::Push(const QJsonObject& json) const
    {
        QJsonArray array = _jsonSource->array();
        array.append(json);
        _jsonSource->setArray(array);

        SaveJson();
    }

    void JsonEditor::SaveJson() const
    {
        QFile jsonFile{ _fileName };
        jsonFile.open(QIODevice::WriteOnly | QFile::Truncate);
        jsonFile.write(_jsonSource->toJson());

        if (jsonFile.error())
        {
            qDebug() << "Json Error:" << jsonFile.errorString();
        }
    }
}