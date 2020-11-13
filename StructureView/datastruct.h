/*!
 * \file datastruct.h
 * \brief файл, содержащий описание класса DataStruct
 */
#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QVector>
#include <QVariant>

/*!
 * \class DataStruct
 * \brief Класс, обрабатывающий и хранящий данные о структуре
 * \author Пехтелев Егор
 */
class DataStruct
{
public:
    /*!
     * \brief Конструктор класса, создаёт json документ из вводных данных
     * \param [in] data данные json формата
     */
    DataStruct(QByteArray * data);

    /// Парсинг данных структуры
    void parceStruct();

    QString error;                  ///< Сообщение об ошибке
    bool ok = true;                 ///< Признак успешного выполнения

    QVector<QString> elemType;      ///< Список типов элементов структуры
    QVector<QString> elemName;      ///< Список названий элементов структуры
    QVector<int> elemValue;         ///< Список значений элементов структуры
    QVector<bool> elemIsStatic;     ///< Список признаков статичности элементов структуры

    QJsonDocument json;             ///< json документ
    QString structName;             ///< Название структуры
    QString objectName;             ///< Название объекта структуры

    /// Обработчик ошибки во время выполнения
    void getError(QString nameError);
};

#endif // DATASTRUCT_H
