/*
 * CS3505 A7 - Team International
 * Authors: Daniel Coimbra Salomão, Yanxia Bu, HAJIN JEON, JAKE SUNDAY, JOSEF WALLACE, MATT EMRICH
 * University Of Utah - Spring 2023
*/
#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

/*
 * The MyDialog class is the first pop up that will be viewed when the user runs the sprite editor, or makes a new sprite.
 * The MyDialog class inherits from QDialog, and when it pops up, it will ask the user for the desired width and height in
 * pixels of their sprite.
 */
class MyDialog : public QDialog
{
    Q_OBJECT

public:
    MyDialog(QWidget *parent = nullptr);
    int getWidth() const;
    int getHeight() const;

private:
    QLineEdit *widthLineEdit;
    QLineEdit *heightLineEdit;
};

#endif // MYDIALOG_H
