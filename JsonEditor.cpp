#include "JsonEditor.hpp"

#include <QFile>

namespace Json
{
    JsonEditor::JsonEditor(const QString& fileName) : _fileName{fileName}
    { }

    JsonEditor::JsonEditor(QString&& fileName) : _fileName{ std::move(fileName) }
    { }

    QJsonDocument JsonEditor::LoadJson() const
    {
        QFile jsonFile{ _fileName };
        jsonFile.open(QIODevice::ReadOnly);

        return QJsonDocument::fromJson(jsonFile.readAll());
    }

    void JsonEditor::SaveJson(const QJsonDocument& document) const
    {
        QFile jsonFile{ _fileName };
        jsonFile.open(QIODevice::WriteOnly | QFile::Truncate);
        jsonFile.write(document.toJson());

        if (jsonFile.error())
        {
            qDebug() << "Error:" << jsonFile.errorString();
        }
    }
}