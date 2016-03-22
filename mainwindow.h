/*Michael Lu
 *October 5th, 2013
 *mainwindow.h contains the function and variable declarations for this class
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkReply>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QString converter(int type, double temp);

public slots:
    void replyFinished(QNetworkReply * reply);
    void getWeatherClicked();

    //temperature buttons
    void celsiusButtonClicked();
    void fahrenheitButtonClicked();
    void kelvinButtonClicked();
    
private:
    double tempInCelsius;
    double minTempInCelsius;
    bool dataRetrieved;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
