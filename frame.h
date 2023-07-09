/*
 * CS3505 A7 - Team International
 * Authors: Daniel Coimbra Salomão, Yanxia Bu, HAJIN JEON, JAKE SUNDAY, JOSEF WALLACE, MATT EMRICH
 * University Of Utah - Spring 2023
*/
#ifndef FRAME_H
#define FRAME_H

#include <QWidget>
#include <QColor>
//#include <QVector2D>
#include <tuple>
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>
#include "ui_frame.h"
#include <QMap>
#include <QPainter>
#include <QMouseEvent>


namespace Ui {
class Frame;
}

class Frame : public QWidget
{
    Q_OBJECT

public:
    int labelHeight;

    int labelWidth;

    QImage pixImage;

    float pixelHeightRatio;

    float pixelWidthRatio;


    QMap<std::tuple<int, int>, QColor> imageRectangles;

    explicit Frame(QWidget *parent = nullptr);
    ~Frame();
    void drawPixel(QColor color, int x, int y);

    void fillBucket(QColor color, int x,int y);

    void generateNewFrame(int pixelHeight, int pixelWidth);

    QJsonArray toJsonObject();

    void fillBucketRecursive(QPainter& painter, QMap<std::tuple<int, int>, QColor>& imageRectangles, std::tuple<int, int> index, QColor oldColor, QColor brushColor);

    void fromJsonObject(const QJsonArray& jsonArray);

signals:

private:
    Ui::frame *ui;
};

#endif // FRAME_H
