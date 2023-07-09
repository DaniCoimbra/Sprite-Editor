/*
 * CS3505 A7 - Team International
 * Authors: Daniel Coimbra Salomão, Yanxia Bu, HAJIN JEON, JAKE SUNDAY, JOSEF WALLACE, MATT EMRICH
 * University Of Utah - Spring 2023
*/
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
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>

Model::Model(QObject *parent) : QObject(parent)
{
}

//mouseClicked functiion receives clicked position and handles click using clickHandle
void Model::mouseClicked(QPoint click)
{
    //Mac UI interface differs from Windows, I had to verify the operating system and adjust the behavior accordingly. This sensure that both opperating sytems work in parlell.
    #ifdef Q_OS_MAC
        int x = click.rx() - 301;
        int y = click.ry() - 22;
    #elif defined(Q_OS_WIN)
        int x = click.rx() - 299;
        int y = click.ry() - 43;
    #else
        // Default values if none of the above are defined
        int x = click.rx() - 299;
        int y = click.ry() - 43;
    #endif
    if(!frameContainer.empty())
        clickHandle(x, y);
}
//currentButton function indicates the currently clicked button and sets button border
void Model::currentButton()
{
    emit sendFocus(currentFrame);
}

//Send frames to preview
void Model::sendFrameContainerFromModelToPreview()
{
    emit sendFrameContainer(frameContainer, currentFrame);
}

//Receives frame and update the frame container
void Model::ReceiveFrame(Frame* receivedFrame)
{
    if(!frameContainer.empty()){
        emit hideFrame(frameContainer[currentFrame]);
    }
    frameContainer.insert(frameContainer.begin()+(++currentFrame),receivedFrame);
}

//Returns frame to the view
void Model::returnsFrame(int index)
{
    emit sendFrame(frameContainer[index]);

}

//Receives the button index and updates the previous frame and loads new frame
void Model::newClicked(int buttonIndex)
{

    emit hideFrame(frameContainer[currentFrame]);
    currentFrame = buttonIndex;

    emit sendFrame(frameContainer[currentFrame]);
}

//Removes the clicked frame
void Model::removeFrame()
{
    if(frameContainer.size() > 1)
    {
        emit hideFrame(frameContainer[currentFrame]);

        frameContainer.erase(frameContainer.begin()+currentFrame);

        if(currentFrame==0)
            currentFrame = 0;
        else if(currentFrame==int(frameContainer.size()))
            currentFrame = frameContainer.size()-1;

        emit sendFrame(frameContainer[currentFrame]);
    }
    else if(frameContainer.size() == 1){
        emit hideFrame(frameContainer[0]);
        frameContainer.pop_back();
        currentFrame = frameContainer.size()-1;
    }
    else{
        currentFrame = -1;
    }
}

//Reverses Frame on Preivew
void Model::reverseFrame()
{
    emit hideFrame(frameContainer[currentFrame]);
    std::reverse(frameContainer.begin(), frameContainer.end());
    emit sendFrame(frameContainer[currentFrame]);

}

//Changes mode for draw, erase, fillbucket
void Model::setMode(std::string mode)
{
    this->mode = mode;
}

//Sets color for brush based on Color Picker
void Model::setBrushColor(QColor color)
{
    this->brushColor = color;
}

//Handles click received from mainwindow ui
void Model::clickHandle(int x, int y)
{
    if(x < 0 || y < 0 || x > 512 || y > 512)
    {
        return;
    }
    if(mode == "draw")
    {
        frameContainer[currentFrame]->drawPixel(brushColor,x,y);
    }
    else if(mode == "erase")
    {
        frameContainer[currentFrame]->drawPixel(Qt::transparent,x,y);
    }
    else if(mode == "fillBucket")
    {
        frameContainer[currentFrame]->fillBucket(brushColor,x,y);
    }

}

//Saves file Json
void Model::Save(int Width, int height)
{

    QMainWindow* mainWindow = dynamic_cast<QMainWindow*>(this->parent());

    QJsonObject frameObj;
    QJsonArray framesArray;
    int frameCount= 0;
    for (Frame* frame : frameContainer)
    {
        framesArray.append(frame->toJsonObject());
        frameCount +=1;

    }
    frameObj["Frames"] = framesArray;
    frameObj["Width"] = Width;
    frameObj["Height"] = height;
    frameObj["frameCount"] = frameCount;

    QJsonDocument jsonDoc(frameObj);
    if (!currentFileName.isEmpty())
    {
        QFile file((currentFileName));
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            file.write(jsonDoc.toJson());
            file.close();
        }
    }

    else {
        QString filename = QFileDialog::getSaveFileName(mainWindow, "Save ", "", "SSP files (*.ssp)");


        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            file.write(jsonDoc.toJson());
            file.close();
            currentFileName = filename;
        }
    };
}

//Save As file Json
void Model::SaveAs(int Width, int height)
{
    QMainWindow* mainWindow = dynamic_cast<QMainWindow*>(this->parent());
    QJsonObject frameObj;
    QJsonArray framesArray;
    int frameCount= 0;
    for (Frame* frame : frameContainer)
    {
        framesArray.append(frame->toJsonObject());
      frameCount +=1;
    }
    frameObj["Frames"] = framesArray;
    frameObj["Width"] = Width;
    frameObj["Height"] = height;
    frameObj["frameCount"] = frameCount;
    QJsonDocument jsonDoc(frameObj);
        // Write the JSON document to a file
        QString filename = QFileDialog::getSaveFileName(mainWindow, "Save As", "", "SSP files (*.ssp)");
            QFile file(filename);
            //Open the file
            if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::NewOnly))
            {
                file.write(jsonDoc.toJson());
                file.close();
        }
            currentFileName = filename;
}

//Loads file Json
void Model::Load()
{
    //Prompt the user to select a file
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Select a file", "/", "SSP files (*.ssp);;All files (*.*)");
    if (!fileName.isEmpty())
    {
        int containerSize = frameContainer.size();
        //Clear the frame vector before adding to it.
        for (int i = 0; i < containerSize; i++)
        {
            emit removeFrameSignal();
        }
        currentFrame = -1;
        //Load file data and store it in a QByteArray
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return;
        }
        QByteArray fileData = file.readAll();
        file.close();
        //Convert from file to QJsonDocument
        QJsonDocument jsonDocument = QJsonDocument::fromJson(fileData);
        //Convert from QJsonDocument to QJsonObject
        QJsonObject jsonObject = jsonDocument.object();
        //Store and emit dimensions of frames
        int frameWidth = jsonObject.value("Width").toInt();
        int frameHeight = jsonObject.value("Height").toInt();
        emit frameDimensionUpdate(frameWidth, frameHeight);
        //Grab the array of frames from the QJsonObject
        QJsonArray jsonFrameArray = jsonObject.value("Frames").toArray();
        //Loop through all frames
        for (const auto& jsonPixelArrayAsRef : jsonFrameArray)
        {
            //Grab the array of pixels within the frame
            QJsonArray jsonPixelArray = jsonPixelArrayAsRef.toArray();
            emit addBlankFrame();
            frameContainer.at(currentFrame)->fromJsonObject(jsonPixelArray);
        }
    }
}
