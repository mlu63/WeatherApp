/*Michael Lu
 *October 5th, 2013
 *main.cpp is the main method for this application; it runs the program
 */
#include "mainwindow.h"
#include <QApplication>


/*main
 *Params: int argc, char *argv[]
 *This method runs the program
 *Returns: int
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
