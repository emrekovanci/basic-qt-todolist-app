#pragma once

#include <QString>
#include <QJsonDocument>
#include <QJsonValue>

namespace Json
{
    class JsonEditor
    {
    public:
        explicit JsonEditor(const QString& fileName);
        explicit JsonEditor(QString&& fileName);
        QJsonDocument LoadJson() const;
        void SaveJson(const QJsonDocument& document) const;
        ~JsonEditor() = default;
    private:
        const QString _fileName;
    };
}