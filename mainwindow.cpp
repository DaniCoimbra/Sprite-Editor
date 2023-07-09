/*
 * CS3505 A7 - Team International
 * Authors: Daniel Coimbra Salomão, Yanxia Bu, Hajin Jeon, JAKE SUNDAY, JOSEF WALLACE, MATT EMRICH
 * University Of Utah - Spring 2023
 *
 * mainwindow.cpp reviewed by Josef Wallace
*/
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(Model& model, QWidget *parent, int w, int h)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    //Connections between signals from mainwindow and slots in model
    connect(this, &MainWindow::sendNewFrame, &model, &Model::ReceiveFrame);
    connect(this, &MainWindow::removeFrame, &model, &Model::removeFrame);
    connect(this, &MainWindow::clickAction, &model, &Model::mouseClicked);
    connect(this, &MainWindow::nextFrame, &model, &Model::newClicked);
    connect(this, &MainWindow::mode, &model, &Model::setMode);
    connect(this, &MainWindow::brushColor, &model, &Model::setBrushColor);
    connect(this, &MainWindow::reverseFrame, &model, &Model::reverseFrame);
    connect(this, &MainWindow::buttonChange, &model, &Model::currentButton);
    connect(this, &MainWindow::saveFramesToJsonSlot, &model, &Model::Save);
    connect(this, &MainWindow::saveAsFramesToJsonSlot, &model, &Model::SaveAs);
    connect(this, &MainWindow::sendFrameDataFromModelToPreview, &model, &Model::sendFrameContainerFromModelToPreview);
    connect(this, &MainWindow::loadFromFile, &model, &Model::Load);

    //Connections between signals from model and slots in mainwindow
    connect(&model, &Model::sendFocus, this, &MainWindow::setFocus);
    connect(&model, &Model::hideFrame, this, &MainWindow::hideFrame);
    connect(&model, &Model::sendFrame, this, &MainWindow::showFrame);
    connect(&model, &Model::frameDimensionUpdate, this, &MainWindow::updateFrameDimensions);
    connect(&model, &Model::addBlankFrame, this, &MainWindow::onAddFrame);
    connect(&model, &Model::removeFrameSignal, this, &MainWindow::onRemoveFrame);
    connect(&model, &Model::sendFrameContainer, &preview, &Preview::receivedFrameToDrawOnPreview);

    //Connection between signals from ui elements and slots in mainwindow
    connect(ui->pushButton_AddFrame, &QPushButton::clicked, this, &MainWindow::onAddFrame);
    connect(ui->pushButton_DeleteFrame, &QPushButton::clicked, this, &MainWindow::onRemoveFrame);

    //Ensure width and height are greater than 0
    if(w>0 && h>0)
    {
        width = w;
        height = h;
    }

    ui->pushButton_color->setStyleSheet(QString("background-color: %1;").arg("black"));

    this->setWindowTitle("Sprite Editor TEAM International");

    onAddFrame();
}

/**
 * @brief Handles the creation of new buttons. Specifically used for the addition of frames.
 */
void MainWindow::onButtonCreate()
{
    QHBoxLayout* hlayout = new QHBoxLayout;

    if(!ui->framesWidget->layout())
    {
        ui->framesWidget->setLayout(hlayout);
    }

    ui->scrollArea->setWidgetResizable(true);

    QString buttonText = tr("Frame: %0").arg(++frameCount+1);

    QPushButton* button = new QPushButton(buttonText, ui->scrollArea);

    frames.push_back(button);

    hlayout = dynamic_cast<QHBoxLayout*>(ui->framesWidget->layout());
    for (auto& btn : frames)
    {
        hlayout->addWidget(btn);
    }
    button->setObjectName(tr("%0").arg(frameCount));
    emit buttonChange();

    lastClickedButtonIndex = frameCount;
    connect(button, &QPushButton::clicked, this, &MainWindow::onFrameButtonClicked);
}

/**
 * @brief Sets the focus to the button of the input index.
 * @param buttonIndex button to focus
 */
void MainWindow::setFocus(int buttonIndex)
{
    for(auto& button : frames)
    {
        if(button->objectName().toInt() == buttonIndex)
        {
            button->setFocus();
        }
    }
}

/**
 * @brief Handles the addition of new frames. Called to add a blank frame.
 */
void MainWindow::onAddFrame()
{
    Frame* newFrame = new Frame(ui->centralwidget);
    QString frameName = tr("frameWidget_%0").arg(frameCount);
    newFrame->setObjectName(frameName);

    newFrame->resize(ui->frameWidget->width(), ui->frameWidget->height());
    newFrame->generateNewFrame(width, height);
    newFrame->move(300,20);
    newFrame->setVisible(true);

    emit sendNewFrame(newFrame);
    onButtonCreate();
}

/**
 * @brief Handles the removal of frames. Called to remove a frame.
 */
void MainWindow::onRemoveFrame()
{
    QHBoxLayout* hlayout = new QHBoxLayout;
    if(!ui->framesWidget->layout())
    {
        ui->framesWidget->setLayout(hlayout);
    }
    if(frameCount > 0)
    {
        delete frames.back();
        frames.pop_back();
        --frameCount;
    }
    else if(frameCount==0)
    {
        frameCount=-1;
        delete frames.back();
        frames.pop_back();
    }

    hlayout = dynamic_cast<QHBoxLayout*>(ui->framesWidget->layout());
    for (auto& btn : frames)
    {
        hlayout->addWidget(btn);
    }

    emit removeFrame();
}

/**
 * @brief Destructor for MainWindow.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Hides the provided frame.
 * @param frame (Frame to hide)
 */
void MainWindow::hideFrame(Frame* frame)
{
    frame->hide();
}

/**
 * @brief Shows the provided frame.
 * @param frame (Frame to show)
 */
void MainWindow::showFrame(Frame* frame)
{
    frame->setVisible(true);
    emit buttonChange();

}

/**
 * @brief Handles the internal workings of frame buttons being clicked.
 */
void MainWindow::onFrameButtonClicked()
{

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    int buttonIndex = (button->objectName()).toInt();
    lastClickedButtonIndex = buttonIndex;
    setStyleSheet("QPushButton:focus { border: 2px solid #0000ff }");
    button->setFocus();

    emit nextFrame(buttonIndex);
}

/**
 * @brief Slot for pushButton_Pen_clicked.
 */
void MainWindow::on_pushButton_Pen_clicked()
{
    setStyleSheet("QPushButton:focus { border: 2px solid #0000ff }");
    ui->pushButton_Pen->setFocus();
    emit mode("draw");
}

/**
 * @brief Slot for pushButton_Eraser_clicked.
 */
void MainWindow::on_pushButton_Eraser_clicked()

{
    setStyleSheet("QPushButton:focus { border: 2px solid #0000ff }");
    ui->pushButton_Eraser->setFocus();

    emit mode("erase");
}

/**
 * @brief Slot for pushButton_Fillbucket_clicked.
 */
void MainWindow::on_pushButton_Fillbucket_clicked()
{
    setStyleSheet("QPushButton:focus { border: 2px solid #0000ff }");
    ui->pushButton_Fillbucket->setFocus();
    emit mode("fillBucket");
}

/**
 * @brief Slot for pushButton_AddFrame_clicked.
 */
void MainWindow::on_pushButton_AddFrame_clicked()
{
    setStyleSheet("QPushButton:focus { border: 2px solid #0000ff }");
    ui->pushButton_AddFrame->setFocus();

}

/**
 * @brief Slot for pushButton_Preview_clicked.
 */
void MainWindow::on_pushButton_Preview_clicked()
{
    setStyleSheet("QPushButton:focus { border: 2px solid #0000ff }");
    ui->pushButton_Preview->setFocus();

    emit sendFrameDataFromModelToPreview();

    preview.show();
}

/**
 * @brief Slot for pushButton_ColorPalette_clicked.
 */
void MainWindow::on_pushButton_ColorPalette_clicked()
{
    QColor color = QColorDialog::getColor(Qt::yellow,this);
    ui->pushButton_color->setStyleSheet(QString("background-color: %1;").arg(color.name()));
    emit brushColor(color);
}

/**
 * @brief Slot for mousePressEvent.
 */
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        isPressing = true;
        emit clickAction(event->pos());
    }
}

/**
 * @brief Slot for mouseMoveEvent.
 */
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() == Qt::LeftButton) && isPressing)
        emit clickAction(event->pos());
}

/**
 * @brief Slot for mouseReleaseEvent.
 */
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) && isPressing)
    {
        isPressing = false;
    }
}

/**
 * @brief Slot for pushButton_DeleteFrame_clicked.
 */
void MainWindow::on_pushButton_DeleteFrame_clicked()
{
    ui->pushButton_DeleteFrame->setFocus();
}

/**
 * @brief Slot for on_action_Save_triggered.
 */
void MainWindow::on_action_Save_triggered()
{
    emit saveFramesToJsonSlot(width,height);
}

/**
 * @brief Slot for on_action_Load_triggered.
 */
void MainWindow::on_action_Load_triggered()
{
    emit loadFromFile();
}

/**
 * @brief Slot for on_actionSave_As_triggered.
 */
void MainWindow::on_actionSave_As_triggered()
{
    emit saveAsFramesToJsonSlot(width,height);
}

/**
 * @brief Slot for updateFrameDimensions. Updates the dimensions of frame objects within the model.
 */
void MainWindow::updateFrameDimensions(int width, int height)
{
    this->width = width;
    this->height = height;
}

/**
 * @brief Slot for on_action_New_triggered.
 */
void MainWindow::on_action_New_triggered()
{
    MyDialog dlg;
    dlg.setFixedSize(400, 400); // set the fixed size of the dialog


    // create a new model and main window
    if (dlg.exec() == QDialog::Accepted)
    {
        Model* model = new Model();
        MainWindow* window = new MainWindow(*model, nullptr, dlg.getHeight(),dlg.getWidth());
        window->show();
    }
}

/**
 * @brief Slot for on_acitonQuit_triggered.
 */
void MainWindow::on_actionQuit_triggered()
{
   close();
}
