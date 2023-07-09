/*
 * CS3505 A7 - Team International
 * Authors: Daniel Coimbra Salomão, Yanxia Bu, HAJIN JEON, JAKE SUNDAY, JOSEF WALLACE, MATT EMRICH
 * University Of Utah - Spring 2023
*/
#ifndef PREVIEW_H
#define PREVIEW_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include "ui_preview.h"
#include "frame.h"

/*
 * The preview class is the popup window that displays the sprite animation of the frames.
 * It uses the frame data from the model to display in the popup all the frames, at intervals
 * in FPS set by the user.
*/
class QMenuBar;
namespace Ui {
class Preview;
}

class Preview : public QWidget
{
    Q_OBJECT

public:
    explicit Preview(QWidget *parent = nullptr);
    ~Preview();

public slots:
    void receivedFrameToDrawOnPreview(std::vector<Frame*> const &frameContainer, int currentFrameIndex);

signals:

private slots:
    void on_pushButton_Close_clicked();
    void on_spinBox_FrameRate_valueChanged(int arg1);
    void on_pushButton_Play_toggled(bool checked);
    void on_pushButton_Pause_clicked();
    void animate();
    void activatePushButtonPlay();

signals:
    void changeFPS(int value);

private:
    Ui::Preview *ui;
    void createMenuBar();
    QMenuBar *menuBar();
    std::vector<Frame*> frames;
    int frameToAnimate;
};

#endif // PREVIEW_H
