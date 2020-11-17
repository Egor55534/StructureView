#include <QGuiApplication>
#include <QFile>
#include <iostream>
#include "expression.h"
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTextStream>

bool isOk = true;

/// Обработчик ошибки во время вычисления
void getError(QString str)
{
    QTextStream(stdout) << str << endl;
    isOk = false;
}

bool openFile(QString filename, QByteArray * data)
{
    bool isOpen = false;
    if(!filename.isEmpty())
    {
        QFile file(filename);
        isOpen = file.open(QIODevice::ReadOnly);

        if(isOpen)
        {
            *data = file.readAll();
            file.close();
        }
    }
    return isOpen;
}

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    setlocale(LC_ALL, "Russian");

    QByteArray b;

    QTextStream cin(stdin);
    QTextStream cout(stdout);

    QString filename = cin.readLine();

    // Открытие файла
    bool isOk = openFile(filename, &b);

    if(isOk)
    {
        // Получение выражения
        Expression curExp(&b);
        if(!curExp.ok)
        {
            getError(curExp.error);
            return 0;
        }

        // Получениее данных о структуре
        DataStruct curStruct(&b);
        if(!curStruct.ok)
        {
            getError(curStruct.error);
            return 0;
        }

        // Обработка данных о структуре
        curStruct.parceStruct();
        if(!curStruct.ok)
        {
            getError(curStruct.error);
            return 0;
        }

        // Обработка выражения
        isOk = curExp.parceExp(&curStruct);

        //Отрисовка изображения
        if(isOk)
        {
            int countElem = curStruct.elemName.size();
            int WIGTH = 40 * 10;
            int HEIGHT = 21 * (countElem + 3);

            QImage img(WIGTH,HEIGHT,QImage::Format_RGB16);
            img.fill(Qt::white);

            QPainter paint(&img);
            paint.setPen(Qt::black);
            paint.setFont(QFont("Times", 16));

            int x = 0;
            int y = 20;

            QString outString;

            paint.drawText(x,y, "struct " + curStruct.structName);
            y+=20;
            paint.drawText(x,y,"{");
            y+=20;
            x+=40;
            for(int i = 0; i < curStruct.elemName.size(); i++)
            {
                if(curExp.result.contains(curStruct.elemName[i]))
                    paint.setPen(Qt::green);
                outString = "";
                if(curStruct.elemIsStatic[i])
                    outString += "static ";
                outString += curStruct.elemType[i] + " ";
                outString += curStruct.elemName[i];
                if(!curStruct.elemName[i].contains("()"))
                    outString += " = " + QString::number(curStruct.elemValue[i]);
                outString += ";";
                paint.drawText(x,y,outString);
                y += 20;
                paint.setPen(Qt::black);
            }
            x=0;
            paint.drawText(x,y,"}; " + curStruct.structName + " " + curStruct.objectName + ";");

            if(filename.indexOf("\\") != -1)
                filename.remove(0,filename.lastIndexOf("\\") + 1);
            filename.remove(filename.indexOf("."),4);
            img.save(filename + ".png");
        }
        else
        {
            getError(curExp.error);
            return 0;
        }
    }
    else
    {
        getError("Неверный путь к файлу");
        return 0;
    }

    return a.exec();
}
