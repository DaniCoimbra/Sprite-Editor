/*
 * CS3505 A7 - Team International
 * Authors: Daniel Coimbra Salomão, Yanxia Bu, HAJIN JEON, JAKE SUNDAY, JOSEF WALLACE, MATT EMRICH
 * University Of Utah - Spring 2023
*/
#ifndef MODEL_H
#define MODEL_H

#include <QMainWindow>
#include <preview.h>
#include <QColorDialog>
#include <QPixmap>
#include <QImage>
#include <QVector>
#include <QColor>
#include <QJsonDocument>
#include <QMutex>
#include "frame.h"

/*
 * The Model class stores datas for each frame in the frameContainer.
 * The Model class updates the datas in each frames e.g. color based on signal and Slot
*/
class Model : public QObject
{
    Q_OBJECT
public:
    Model(QObject *parent = nullptr);
    //Contains array of frame widgets
    std::vector<Frame*> frameContainer;
    //Sets mode for draw, erase, fill.
    std::string mode = "draw";
    //Sets color for brush, default selected on black
    QColor brushColor = Qt::black;
    //Sets file name
    QString currentFileName;

public slots:
    void mouseClicked(QPoint click);
    void ReceiveFrame(Frame* frame);
    void returnsFrame(int index);
    void removeFrame();
    void clickHandle(int x, int y);
    void newClicked(int buttonIndex);
    void setMode(std::string mode);
    void setBrushColor(QColor color);
    void reverseFrame();
    void Save(int Width, int height);
    void SaveAs(int Width, int height);
    void Load();
    void currentButton();
    void sendFrameContainerFromModelToPreview();

signals:
    void hideFrame(Frame* frame);
    void sendFrame(Frame* frame);
    void sendFocus(int focus);
    void frameUiUpdate();
    void frameDimensionUpdate(int width, int height);
    void addBlankFrame();
    void removeFrameSignal();
    void sendFrameContainer(std::vector<Frame*> const &frameContainer, int currentFrame);

private:
    int currentFrame = -1;
    int fps = 1;
};

#endif // MODEL_H
