/*!
 * \file expression.h
 * \brief файл, содержащий описание класса, обрабатывающего выражение для получения названий элементов структуры, к оторым происходит обращение
 */
#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <QHash>
#include <datastruct.h>

/*!
 * \class Expression
 * \brief Класс, обрабатывающий выражение для получения названий элементов структуры, к оторым происходит обращение
 * \author Пехтелев Егор
 */
class Expression
{
public:
    /*!
     * \brief Конструктор класса, получающей выражение из данных
     * \param [in] массив байтов, первая строка которого сожержит выражение
     */
    Expression(QByteArray * byteArray);

    /*!
     * \brief Обрабатывает выражение
     * \param curDataStruct ссылка на объект класса, содержащего данные о структуре
     * \return Признак успешного выполнения
     */
    bool parceExp(DataStruct* curDataStruct);

    QVector<QString> result;        ///< Список названий элементов, к которым совершено обращение

    QString error;                  ///< Сообщение об ошибке
    bool ok = true;                 ///< Признак успешного выполнения


private:
    /// Словарь операций
    enum operation {Add,                    ///< Сложение
                   Sub,                     ///< Вычитание
                   Mul,                     ///< Произведение
                   Div,                     ///< Деление
                   Inc,                     ///< Инкрементация
                   Dec,                     ///< Декрементация
                   Reference,               ///< Обращение к нестатическому элементу
                   ReferenceToTheStatic};   ///< Обращение к статическому элементу

    QString express;                ///< Выражение
    QVector<QString> stack;         ///< Стек
    DataStruct* curStruct;          ///< Ссылка на объект класса, содержащего данные о структуре

    /// Взятие числа из стека. Возвращает true, если получилось взять, иначе false
    bool takeNumber(QString & a);

    /// Взятие чисел из стека. Возвращает true, если получилось взять, иначе false
    bool takeNumber(QString & a, QString & b);

    /// Обработчик ошибки во время вычисления
    void getError(QString str);

    /// Выполнение операций
    void calc(operation cur);

    /*!
     * \brief Проверяет операнд на число
     * \param str строка
     * \return 1 если строка сожержит число типа int
     *         иначе 0
     */
    bool isNum(QString str);
};

#endif // EXPRESSION_H
