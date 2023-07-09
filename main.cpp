/*
 * CS3505 A7 - Team International
 * Authors: Daniel Coimbra Salomão, Yanxia Bu, HAJIN JEON, JAKE SUNDAY, JOSEF WALLACE, MATT EMRICH
 * University Of Utah - Spring 2023
*/
#include <QApplication>
#include <QInputDialog>
#include "mydialog.h"
#include "mainwindow.h"
#include "Model.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // ask user for dialog size

bool ok;
    // make a dialog that runs in its own event loop
    MyDialog dlg;
    dlg.setFixedSize(400, 400); // set the fixed size of the dialog
    if (!dlg.exec())
    {
        return 0;
    }

    Model model;

    MainWindow window(model, nullptr, dlg.getHeight(),dlg.getWidth());

   window.show();

   return app.exec();
}

