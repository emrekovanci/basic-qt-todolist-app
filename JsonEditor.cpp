#include "JsonEditor.hpp"

#include <QFile>
#include <QJsonArray>

namespace Json
{
    JsonEditor::JsonEditor(const QString& fileName) : _FileName{ fileName }
    { }

    JsonEditor::JsonEditor(QString&& fileName) : _FileName{ std::move(fileName) }
    { }

    std::unique_ptr<QJsonDocument> JsonEditor::LoadJson() const
    {
        if (QFile jsonFile{ _FileName }; jsonFile.exists())
        {
            jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
            return std::make_unique<QJsonDocument>(QJsonDocument::fromJson(jsonFile.readAll()));
        }

        return std::make_unique<QJsonDocument>();
    }

    // refresh entire-db
    void JsonEditor::Push(const QJsonObject& json) const
    {
        QJsonArray array = _JsonSource->array();
        array.append(json);
        _JsonSource->setArray(array);

        SaveJson();
    }

    void JsonEditor::SaveJson() const
    {
        QFile jsonFile{ _FileName };
        jsonFile.open(QIODevice::WriteOnly | QFile::Truncate);
        jsonFile.write(_JsonSource->toJson());

        if (jsonFile.error())
        {
            qDebug() << "Json Error:" << jsonFile.errorString();
        }
    }
}