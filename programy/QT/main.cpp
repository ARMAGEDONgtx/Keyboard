/*!
    @author Michal Wróblewski
    @file main.cpp
    @date 22-12-2017
    @brief SM PROJECT

*/

#include "mainwindow.h"
#include <QApplication>

/*!
 * \brief main
 * \param argc
 * \param argv
 * \return zwraca uruchomione okno
 *
 *
 *
 * główna funckja
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
