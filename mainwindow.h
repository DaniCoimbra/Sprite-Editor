/*
 * CS3505 A7 - Team International
 * Authors: Daniel Coimbra Salomão, Yanxia Bu, HAJIN JEON, JAKE SUNDAY, JOSEF WALLACE, MATT EMRICH
 * University Of Utah - Spring 2023
 *
 * mainwindow.h reviewed by Josef Wallace
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <preview.h>
#include <QColorDialog>
#include "Model.h"
#include "frame.h"
#include <mydialog.h>
#include <QVector>
#include <QImage>
#include <QWidget>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QColor>
#include <math.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QTextStream>
#include <QJsonDocument>
#include <QAction>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
Frame currentFrame;

private:
    void createButton();

public:
    MainWindow(Model& model, QWidget *parent = nullptr, int width = 0, int height = 0);
    ~MainWindow();
    bool isPressing;
    int width=4;
    int height=4;
    int frameCount = -1;
    int lastClickedButtonIndex;
    QString currentFileName;
    std::vector<QPushButton*> frames;
    QPushButton* lastClickedButton;
    Model* models;

private slots:
    //Ui slots
    void on_pushButton_ColorPalette_clicked();
    void on_pushButton_Eraser_clicked();
    void on_pushButton_Preview_clicked();
    void on_pushButton_Pen_clicked();
    void on_pushButton_Fillbucket_clicked();
    void on_pushButton_AddFrame_clicked();
    void on_pushButton_DeleteFrame_clicked();
    void on_action_Save_triggered();
    void on_action_Load_triggered();
    void on_action_New_triggered();
    void on_actionQuit_triggered();
    void on_actionSave_As_triggered();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    //Custom slots
    void hideFrame(Frame* frame);
    void showFrame(Frame* frame);
    void setFocus(int buttonIndex);
    void updateFrameDimensions(int width, int height);


signals:
    void buttonChange();
    void mouseClick(QPoint click);
    void sendNewFrame(Frame* newFrame);
    void removeFrame();
    void clickAction(QPoint point);
    void mode(std::string mode);
    void brushColor(QColor color);
    void nextFrame(int index);
    void reverseFrame();
    void saveFramesToJsonSlot(int width, int height);
    void saveAsFramesToJsonSlot(int width, int height);
    void loadFromFile();
    void sendFrameDataFromModelToPreview();


private:
    void onAddFrame();
    void onRemoveFrame();
    void onFrameButtonClicked();
    void onButtonCreate();
    Ui::MainWindow *ui;
    Preview preview;
};
#endif // MAINWINDOW_H
