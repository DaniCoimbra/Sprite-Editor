/*
 * CS3505 A7 - Team International
 * Authors: Daniel Coimbra Salomão, Yanxia Bu, HAJIN JEON, JAKE SUNDAY, JOSEF WALLACE, MATT EMRICH
 * University Of Utah - Spring 2023
*/
#include "mydialog.h"

///
/// \brief MyDialog box constructor. Asks the user for a width and height for their sprite.
/// \param parent
///
MyDialog::MyDialog(QWidget *parent)
    : QDialog(parent)
{
    // Make a height label for the UI
    QLabel *heightLabel = new QLabel("Height:");
    heightLineEdit = new QLineEdit;
    heightLineEdit->setText("");

    // Make a width label for the UI
    QLabel *widthLabel = new QLabel("Width:");
    widthLineEdit = new QLineEdit;
    widthLineEdit->setText("");

    // Make an ok button for the UI
    QPushButton *okButton = new QPushButton("OK");
    connect(okButton, &QPushButton::clicked, this, &MyDialog::accept);

    // Add all the widgets into a HBoxLayout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(heightLabel);
    layout->addWidget(heightLineEdit);
    layout->addWidget(widthLabel);
    layout->addWidget(widthLineEdit);
    layout->addWidget(okButton);

    setLayout(layout);
}

///
/// \brief MyDialog::getWidth Getter for width
/// \return The width input of the user
///
int MyDialog::getWidth() const
{
    return widthLineEdit->text().toInt();
}


///
/// \brief MyDialog::getHeight Getter for height
/// \return The height input of the user
///
int MyDialog::getHeight() const
{
    return heightLineEdit->text().toInt();
}
