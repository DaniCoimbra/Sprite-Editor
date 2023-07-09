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

void Model::mouseClicked(QPoint click){
    int x = click.rx() - 150;
    int y = click.ry() - 42;
    emit draw(x,y);
}
