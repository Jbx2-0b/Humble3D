#ifndef ISCRIPTABLE_H
#define ISCRIPTABLE_H

// Qt
#include <QMap>
#include <QString>
#include <QVariant>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QDebug>

typedef QMap<QString, QVariant> TParameters;


class IScriptable
{
public:

    virtual void init(const TParameters& parameters) = 0;

protected:

    template<typename TypeData> static TypeData get(const TParameters& parameters, const QString& typeData)
    {
        if (parameters.contains(typeData))
        {
            return parameters[typeData].value<TypeData>();
        }

        return TypeData();
    }
};

template<>
inline QVector2D IScriptable::get<QVector2D>(const TParameters& parameters, const QString& typeData)
{
    QVector2D result;
    if (parameters.contains(typeData))
    {
        QString data = parameters[typeData].toString();
        QStringList dataSplit = data.split(";");

        if (dataSplit.size() == 2)
        {
            result = QVector2D(
                        dataSplit[0].toFloat(),
                    dataSplit[1].toFloat());
        }
        else
        {
            qDebug() << typeData << " : Error parsing.";
        }
    }

    return result;
}

template<>
inline QVector3D IScriptable::get<QVector3D>(const TParameters& parameters, const QString& typeData)
{
    QVector3D result;
    if (parameters.contains(typeData))
    {
        QString data = parameters[typeData].toString();
        QStringList dataSplit = data.split(";");

        if (dataSplit.size() == 3)
        {
            result = QVector3D(
                        dataSplit[0].toFloat(),
                    dataSplit[1].toFloat(),
                    dataSplit[2].toFloat());
        }
        else
        {
            qDebug() << typeData << " : Error parsing.";
        }
    }

    return result;
}

template<>
inline QVector4D IScriptable::get<QVector4D>(const TParameters& parameters, const QString& typeData)
{
    QVector4D result;
    if (parameters.contains(typeData))
    {
        QString data = parameters[typeData].toString();
        QStringList dataSplit = data.split(";");
        if (dataSplit.size() == 4)
        {
            result = QVector4D(
                        dataSplit[0].toFloat(),
                    dataSplit[1].toFloat(),
                    dataSplit[2].toFloat(),
                    dataSplit[3].toFloat());
        }
        else if (dataSplit.size() == 3)
        {
            result = QVector4D(
                        dataSplit[0].toFloat(),
                    dataSplit[1].toFloat(),
                    dataSplit[2].toFloat(),
                    1.0);
        }
        else
        {
            qDebug() << typeData << " : Error parsing.";
        }
    }

    return result;
}


#endif // ISCRIPTABLE_H
