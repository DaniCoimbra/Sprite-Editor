/*
 * CS3505 A7 - Team International
 * Authors: Daniel Coimbra Salomão, Yanxia Bu, HAJIN JEON, JAKE SUNDAY, JOSEF WALLACE, MATT EMRICH
 * University Of Utah - Spring 2023
*/
#include "frame.h"
#include "ui_frame.h"



Frame::Frame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frame)
{
    ui->setupUi(this);

}

Frame::~Frame()
{
    delete ui;
}

///
/// \brief Frame::generateNewFrame T
/// This function generates a new Frame object with the specified pixel height and width. The generated Frame consists of a checkerboard pattern of white and gray pixels.
/// \param pixelHeight The desired height of the Frame in pixels.

/// \param pixelWidth
/// \param pixelWidth The desired width of the Frame in pixels.
///
void Frame::generateNewFrame(int pixelHeight, int pixelWidth)
{
    imageRectangles.clear();
    this->labelHeight = ui->drawingLabel->height();
    this->labelWidth = ui->drawingLabel->width();

    // Calculate the pixel height and width ratio based on the label dimensions.
    this->pixelHeightRatio = (float)labelHeight / (float)pixelHeight;
    this->pixelWidthRatio = (float)labelWidth / (float)pixelWidth;

    this->pixImage = QImage(labelWidth, labelHeight, QImage::Format_RGB32);
    QPainter painter = QPainter(&pixImage);

    painter.fillRect(pixImage.rect(), Qt::transparent);

    // Loop over the label dimensions and create rectangles for each pixel.
    for (float i = 0; i < labelWidth; i+= pixelWidthRatio)
    {
        for (float j = 0; j < labelHeight; j+=pixelHeightRatio)
        {
            // Create a rectangle for the pixel and calculate the index.

            QRectF rect = QRectF(i, j, pixelWidthRatio, pixelHeightRatio);
            std::tuple<int, int> index (round(i/pixelWidthRatio), round(j/pixelHeightRatio));

            // Alternate between white and gray.
            if(int(round(i/pixelWidthRatio)+round(j/pixelHeightRatio))%2==0)
            {
                painter.fillRect(rect, Qt::white);
                imageRectangles[index] = Qt::transparent;
            }
            else
            {
                painter.fillRect(rect, Qt::gray);
                imageRectangles[index] = Qt::transparent;
            }
        }

    }

    ui->drawingLabel->setPixmap(QPixmap::fromImage(pixImage.scaled(labelWidth, labelHeight)));
    ui->drawingLabel->setFixedSize(QPixmap::fromImage(pixImage.scaled(labelWidth, labelHeight)).size());

 }

void Frame::drawPixel(QColor brushColor, int x, int y){
    int blockX=floor(x/pixelWidthRatio);
    int blockY=floor(y/pixelHeightRatio);

    float xPosition = (float)blockX*(float)pixelWidthRatio;
    float yPosition= (float)blockY*(float)pixelHeightRatio;

    QPainter painter = QPainter(&pixImage);

    QRectF rect = QRectF(xPosition, yPosition, pixelWidthRatio, pixelHeightRatio);

    std::tuple<int, int> index (blockX, blockY);

    if(brushColor == Qt::transparent)
    {
        if(int(round(xPosition/pixelWidthRatio)+round(yPosition/pixelHeightRatio))%2==0)
        {
            painter.fillRect(rect, Qt::white);
            imageRectangles[index] = Qt::transparent;
        }
        else
        {
            painter.fillRect(rect, Qt::gray);
            imageRectangles[index] = Qt::transparent;
        }
    }
    else
    {
        imageRectangles[index] = brushColor;
        painter.fillRect(rect, brushColor);
    }
    ui->drawingLabel->setPixmap(QPixmap::fromImage(pixImage.scaled(labelWidth, labelHeight)));
    ui->drawingLabel->setFixedSize(QPixmap::fromImage(pixImage.scaled(labelWidth, labelHeight)).size());
}

void Frame::fillBucket(QColor brushColor, int x, int y)
{
        int blockX=floor(x/pixelWidthRatio);
        int blockY=floor(y/pixelHeightRatio);

        QPainter painter = QPainter(&pixImage);

        std::tuple<int, int> index (blockX, blockY);

        if(imageRectangles[index]!=brushColor)
            fillBucketRecursive(painter, imageRectangles, index, imageRectangles[index], brushColor);
        ui->drawingLabel->setPixmap(QPixmap::fromImage(pixImage.scaled(labelWidth, labelHeight)));
        ui->drawingLabel->setFixedSize(QPixmap::fromImage(pixImage.scaled(labelWidth, labelHeight)).size());
}

void Frame::fillBucketRecursive(QPainter& painter, QMap<std::tuple<int, int>, QColor>& imageRectangles, std::tuple<int, int> index, QColor oldColor, QColor brushColor){
        if (std::get<0>(index) < 0 || std::get<0>(index) >= labelWidth || std::get<1>(index) < 0 || std::get<1>(index) >= labelHeight || imageRectangles[index] != oldColor || imageRectangles[index] == brushColor)
        {
        return;
        }

    imageRectangles[index] = brushColor;

    float xPosition = (float)std::get<0>(index)*(float)pixelWidthRatio;
    float yPosition= (float)std::get<1>(index)*(float)pixelHeightRatio;

    QRectF rect = QRectF(xPosition, yPosition, pixelWidthRatio, pixelHeightRatio);
    painter.fillRect(rect, brushColor);

    fillBucketRecursive(painter, imageRectangles, std::tuple<int, int> (std::get<0>(index), std::get<1>(index)-1), oldColor, brushColor);
    fillBucketRecursive(painter, imageRectangles, std::tuple<int, int> (std::get<0>(index), std::get<1>(index)+1), oldColor, brushColor);
    fillBucketRecursive(painter, imageRectangles, std::tuple<int, int> (std::get<0>(index)+1, std::get<1>(index)), oldColor, brushColor);
    fillBucketRecursive(painter, imageRectangles, std::tuple<int, int> (std::get<0>(index)-1, std::get<1>(index)), oldColor, brushColor);
}
///
/// \brief This function returns a QJsonArray representation of a Frame object. The QJsonArray contains information about the position and color of each pixel in the Frame.
/// \return A QJsonArray containing information about the position and color of each pixel in the Frame.
///
QJsonArray Frame::toJsonObject() {

    QJsonArray jsonArray;

    for (const auto& key : imageRectangles.keys())
    {
        // Get the color value for the current key.
        const QColor& color= imageRectangles[key];

        QJsonObject pixel;
        pixel["positionX"] = std::get<0>(key);
        pixel["positionY"] = std::get<1>(key);
        pixel["red"] = color.red();
        pixel["green"] = color.green();
        pixel["blue"] = color.blue();
        pixel["alpha"] = color.alpha();

        jsonArray.append(pixel);

    }
    return jsonArray;
}
///
/// \brief Frame::fromJsonObject
///     This method is a helper method for load. It gits an index and draws the coreessponding rectangle,
/// \param jsonArray A QJsonArray containing information about the position and color of each pixel in the Frame.
///

void Frame::fromJsonObject(const QJsonArray& jsonArray)
{
    imageRectangles.clear(); // clear the map

    for (const auto& pixelAsConstRef : jsonArray)
    {
        QJsonObject pixel = pixelAsConstRef.toObject();

        int positionX = pixel.value("positionX").toInt();
        int positionY = pixel.value("positionY").toInt();
        QColor color(pixel["red"].toInt(), pixel["green"].toInt(), pixel["blue"].toInt(), pixel["alpha"].toInt());
        drawPixel(color, positionX*pixelWidthRatio, positionY*pixelHeightRatio);
    }
}
