/*
 * CS3505 A7 - Team International
 * Authors: Daniel Coimbra Salomão, Yanxia Bu, HAJIN JEON, JAKE SUNDAY, JOSEF WALLACE, MATT EMRICH
 * University Of Utah - Spring 2023
*/
#include "preview.h"

// the constructor of the preview class
/// \brief Preview::Preview
/// \param parent
///
Preview::Preview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Preview)
{
    ui->setupUi(this);
    this->setWindowTitle("Preview");
}
///
/// \brief Preview::~Preview
///deconstructor of the preview
Preview::~Preview()
{
    delete ui;
}
///
/// \brief Preview::onPushButtonCloseClicked
///Closes the window and resets the button before close
void Preview::on_pushButton_Close_clicked()
{
    // knowing that initializing the play and reverse button which reset this two button
    ui->pushButton_Play->setChecked(false);
    ui->pushButton_Reverse->setChecked(false);
    this->hide();
}
///
/// \brief Preview::onSpinBoxFrameRateValueChanged
/// \param arg1
///send signal that the FPS rate is change
void Preview::on_spinBox_FrameRate_valueChanged(int arg1)
{
    emit changeFPS(arg1);
}
///
/// \brief Preview::onPushButtonPlayToggled
/// \param checked
///trigger the play button
void Preview::on_pushButton_Play_toggled(bool checked)
{
    // Send a signal to the mainWindow to get a reference from the model to iterate over
    if (checked == true)
    {
        this->animate();
    }
}
///
/// \brief Preview::activatePushButtonPlay
/// Slot to slow play button down to not speed up framerate
void Preview::activatePushButtonPlay()
{
    ui->pushButton_Play->setEnabled(true);
}
///
/// \brief Preview::receivedFrameToDrawOnPreview
/// \param frameContainer
/// \param currentFrameIndex
/// reveive the frame and start animated
void Preview::receivedFrameToDrawOnPreview(std::vector<Frame*> const &frameContainer,
                                           int currentFrameIndex)
{
    frames = frameContainer;
    // start the animation loop at the currently selected frame, draw that frame first
    if (frameContainer.size() > 0)
    {
        frameToAnimate = currentFrameIndex;
        //Slot callback function, now that the frames have been received we can start the animation loop
        QPixmap resizedPixmap =
                QPixmap::fromImage(frames[frameToAnimate]->pixImage).scaled(ui->drawLabel->width(),
                                                                            ui->drawLabel->height());
        ui->drawLabel->setPixmap(resizedPixmap);
    }
}
///
/// \brief Preview::onPushButtonPauseClicked
///Slot to stop the animation loop when paused
void Preview::on_pushButton_Pause_clicked()
{
    ui->pushButton_Play->setChecked(false);
    ui->pushButton_Play->setDisabled(true);
    QTimer::singleShot(1000, this, &Preview::activatePushButtonPlay);
}
///
/// \brief Preview::animate
///Animation loop, will update a frame in forward or reverse order, depending on the user's
///desired framerate
void Preview::animate()
{ // Stop the loop if play is unselected
    if (ui->pushButton_Play->isChecked() == false || frames.empty())
        return;
    // Checks to loop frameToAnimate pointer to start or end of frames vector
    if (frameToAnimate >= (int)frames.size())
        frameToAnimate = 0;
    if (frameToAnimate < 0)
        frameToAnimate = frames.size() - 1;
    // Draw the pixmap of the frame to the label
    QPixmap resizedPixmap =
            QPixmap::fromImage(frames[frameToAnimate]->pixImage).scaled(ui->drawLabel->width(),
                                                                        ui->drawLabel->height());
    ui->drawLabel->setPixmap(resizedPixmap);
    // update the pointer to the next frame to animate
    frameToAnimate = (ui->pushButton_Reverse->isChecked() == true) ? frameToAnimate - 1:
                                                                     frameToAnimate + 1;
    // Calcuate the desires miliseconds from the given FPS then start another animate when ready
    int MiliSecFromFPS = 1000 / ui->spinBox_FrameRate->value();
    QTimer::singleShot(MiliSecFromFPS, this, &Preview::animate);
}
