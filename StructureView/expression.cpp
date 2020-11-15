#include "expression.h"

Expression::Expression(QByteArray * byteArray)
{
    int indexNewString = byteArray->indexOf('\n');
    if(indexNewString == -1)
        getError("В файле присутствует только выражение");
    else
    {
        express = QString::fromStdString(byteArray->left(indexNewString - 1).toStdString());

        byteArray->remove(0, indexNewString + 1);
    }
}

void Expression::getError(QString str)
{
    error = str;
    ok = false;
    stack.clear();
}

bool Expression::parceExp(DataStruct* curDataStruct)
{
    QStringList psExpress = {};
    if(!express.isEmpty())
    {
        express = express.trimmed();
        psExpress = express.split(" ");
        curStruct = curDataStruct;
    }
    else
        getError("Выражение пустое");

    while (psExpress.size() !=0 && ok)
    {
        QString curOper = psExpress.takeFirst();

        if(curOper == "+")
            calc(Add);
        else if(curOper == "-")
            calc(Sub);
        else if(curOper == "*")
            calc(Mul);
        else if(curOper == "/")
            calc(Div);
        else if(curOper == "++")
            calc(Inc);
        else if(curOper == "--")
            calc(Dec);
        else if(curOper == ".")
            calc(Reference);
        else if(curOper == "::")
            calc(ReferenceToTheStatic);
        else
            stack.append(curOper);
    }

    return ok;
}

/// Взятие значение из стека. Возвращает true, если получилось взять, иначе false
bool Expression::takeNumber(QString & a)
{
    if(stack.isEmpty())
        return false;
    else
    {
        a = stack.takeLast();
        return true;
    }
}

/// Взятие значений из стека. Возвращает true, если получилось взять, иначе false
bool Expression::takeNumber(QString & a, QString & b)
{
    if(stack.size() < 2)
        return false;
    else
    {
        b = stack.takeLast();
        a = stack.takeLast();
        return true;
    }
}

/// Выполнение операций
void Expression::calc(operation cur)
{
    QString a,b;
    switch (cur) {
    case Add:
        if(takeNumber(a,b))
            if(isNum(a) && isNum(b))
            {
                a = QString::number(a.toInt(nullptr ,10) + b.toInt(nullptr ,10));
                stack.append(a);
            }
            else getError("Неизвестные переменные");
        else
            getError("Недостаточно операндов для сложения");
        break;
    case Sub:
        if(takeNumber(a,b))
            if(isNum(a) && isNum(b))
            {
                a = QString::number(a.toInt(nullptr ,10) - b.toInt(nullptr ,10));
                stack.append(a);
            }
            else getError("Неизвестные переменные");
        else
            getError("Недостаточно операндов для разности");
        break;
    case Mul:
        if(takeNumber(a,b))
            if(isNum(a) && isNum(b))
            {
                a = QString::number(a.toInt(nullptr ,10) * b.toInt(nullptr ,10));
                stack.append(a);
            }
            else getError("Неизвестные переменные");
        else
            getError("Недостаточно операндов для произведения");
        break;
    case Div:
        if(takeNumber(a,b))
            if(isNum(a) && isNum(b))
            {
                a = QString::number(a.toInt(nullptr ,10) / b.toInt(nullptr ,10));
                stack.append(a);
            }
            else getError("Неизвестные переменные");
        else
            getError("Недостаточно операндов для деления");
        break;
    case Inc:
        if(takeNumber(a))
            if(isNum(a))
            {
                a = QString::number(a.toInt(nullptr, 10) + 1);
                stack.append(a);
            }
            else getError("Неизвестная переменная");
        else
            getError("Недостаточно операндов для инкрементации");
        break;
    case Dec:
        if(takeNumber(a))
            if(isNum(a))
            {
                a = QString::number(a.toInt(nullptr, 10) - 1);
                stack.append(a);
            }
            else getError("Неизвестная переменная");
        else
            getError("Недостаточно операндов для декрементации");
        break;
    case Reference:
        if(takeNumber(a,b))
            if(a != curStruct->objectName)
                getError("Обращение по неверному имени объекта");
            else if(!curStruct->elemName.contains(b))
                getError(b + " не является элементом структуры");
            else if(curStruct->elemIsStatic[curStruct->elemName.indexOf(b)] == true)
                getError("Обращение к статическому элементу");
            else
            {
                if(!result.contains(b))
                    result.append(b);
                stack.append("1");
            }
        else
            getError("Недостаточно операндов для обращения к элементу структуры");
        break;
    case ReferenceToTheStatic:
        if(takeNumber(a,b))
            if(a != curStruct->structName)
                getError("Обращение по неверному имени структуры");
            else if(!curStruct->elemName.contains(b))
                getError(b + " не является элементом структуры");
            else if(curStruct->elemIsStatic[curStruct->elemName.indexOf(b)] == false)
                getError("Обращение к нестатическому элементу");
            else
            {
                if(!result.contains(b))
                    result.append(b);
                stack.append("1");
            }
        else
            getError("Недостаточно операндов для обращения к элементу структуры");
        break;
    }
}

bool Expression::isNum(QString str)
{
    bool isNumber = false;
    str.toInt(&isNumber, 10);
    return isNumber;
}
