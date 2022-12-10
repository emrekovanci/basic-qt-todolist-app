#pragma once

#include <QString>
#include <QJsonDocument>
#include <QJsonValue>

#include <memory>

namespace Json
{
    class JsonEditor
    {
    public:
        explicit JsonEditor(const QString& fileName);
        explicit JsonEditor(QString&& fileName);
        std::unique_ptr<QJsonDocument> LoadJson() const;
        void Push(const QJsonObject& json) const;
        void SaveJson() const;
        ~JsonEditor() = default;
    private:
        const QString _FileName;
        std::unique_ptr<QJsonDocument> _JsonSource;
    };
}