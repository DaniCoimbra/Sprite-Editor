#include "model.h"
#include <QMainWindow>
#include <preview.h>
#include <QColorDialog>
#include <QPixmap>
#include <QImage>
#include <QObject>
#include <iostream>
#include <QRect>
#include <QPainter>
#include <QColor>
#include <QPoint>

Model::Model(QObject *parent) : QObject(parent)
{
}

void Model::mouseClicked(QPoint click, int gridX, int gridY){
    int x = click.rx() - 150;
    int y = click.ry() - 43;


    emit draw(x,y);
}
