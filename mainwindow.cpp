/*Michael Lu
 *October 5th 2013
 *mainwindow.cpp contains the functionality of the methods defined in mainwindow.h
 *mainwindow is the controller and interacts with and updates the ui
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "model.h"

#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QDebug>
#include <QVector>


/*MAINWINDOW
 *Constructor
 *Creates a mainwindow object and sets up the UI
 *Also connects buttons and functions
 *Returns: MainWindow
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Custom aesthetic settings
    ui->celsius_button->setChecked(true);
    ui->dropdown_menu->setCurrentIndex(0);
    ui->dropdown_menu->setStyleSheet("background-color: white;");
    ui->getWeather->setStyleSheet("background-color: white");

    this->setStyleSheet("background-color: yellow;");

    //ensures that no weather information can be shown until "get weather" is pressed
    dataRetrieved = false;

    minTempInCelsius = 0;
    tempInCelsius = 0;

    //links the buttons for temperature conversion
    QObject::connect(ui->celsius_button, SIGNAL(clicked()), this, SLOT(celsiusButtonClicked()));
    QObject::connect(ui->fahrenheit_button, SIGNAL(clicked()), this, SLOT(fahrenheitButtonClicked()));
    QObject::connect(ui->kelvin_button, SIGNAL(clicked()), this, SLOT(kelvinButtonClicked()));

    //links the "get weather" button
    QObject::connect(ui->getWeather, SIGNAL(clicked()), this, SLOT(getWeatherClicked()));
}


/*converter
 *Params: int type, double temp
 *converts the provided temperature (in celsius) to another unit and returns it as a string statement
 *Returns: QString
 */
QString MainWindow::converter(int type, double ttemp){
    if(type==1){ //celsius
        QString display(QString::number(ttemp, 'f', 1) + " degrees Celsius");
        return display;
    }
    else if(type==2){ //kelvin
        double kelvin = ttemp + 273.15;
        QString display(QString::number(kelvin, 'f', 1) + " degrees Kelvin");
        return display;
    }
    double fahrenheit = ttemp * 1.8 + 32; //fahrenheit
    QString display(QString::number(fahrenheit, 'f', 1) + " degrees Fahrenheit");
    return display;
}


/*replyFinished
 *Params: QNetworkReply reply
 *verifies if information was successfully received, then takes out certain elements from the JsonObject to display
 *Returns: void
 */
void MainWindow::replyFinished(QNetworkReply * reply){
    if ( reply->error() != QNetworkReply::NoError){
        // A communication error has occurred
        ui->tempLabel->setText("Network Error");
        return;
    }

    //enables the display of weather information
    dataRetrieved = true;

    //We read the JSON response into a QJsonObject
    QJsonObject obj = QJsonDocument::fromJson(reply->readAll()).object();

    //We read the "wind" component of the Json Object and get wind speed and direction
    QJsonObject wind = obj["wind"].toObject();
    double speed = wind["speed"].toDouble();
    double direction = wind["deg"].toDouble();

    //We create strings containing what we want to display
    QString sLabel = ("Wind Speed: " + QString::number(speed, 'f', 1) + " km/h");
    QString dLabel = ("Wind Direction: " + QString::number(direction, 'f', 1) + " degrees");

    //Display aforementioned strings on the designated labels
    ui->windSpeedLabel->setText(sLabel);
    ui->windDirectionLabel->setText(dLabel);

    //We find the "main" object from the full response (see the format in the assingment description)
    QJsonObject main = obj["main"].toObject();

    //retrieve pressue and humidity elements
    double pressure = main["pressure"].toDouble();
    double humidity = main["humidity"].toDouble();

    //set pressure and humidity labels
    QString pLabel("Pressure of " + QString::number(pressure, 'f', 1) + " mb");
    ui->pressureLabel->setText(pLabel);

    QString hLabel("Humidity of " + QString::number(humidity, 'f', 1) + "%");
    ui->humidityLabel->setText(hLabel);

    //Retrieve the minimum temperature element and assign it to the instance variable
    double minTemp = main["temp_min"].toDouble();
    minTempInCelsius = minTemp;

    //and get out the temp element
    double temp = main["temp"].toDouble();
    tempInCelsius = temp;

    //checks for initial conversion unit to display

    if(ui->kelvin_button->isChecked()){
        kelvinButtonClicked();
        ui->minTempLabel->setText("Min: " + converter(2, minTemp));}

    else if(ui->fahrenheit_button->isChecked()){
        fahrenheitButtonClicked();
        ui->minTempLabel->setText("Min: " + converter(0, minTemp));}

    else{
        ui->minTempLabel->setText("Min: " + converter(1, minTemp));
        celsiusButtonClicked();}
}


/*getWeatherClicked
 *Params: None
 *Retrieves / Updates the information shown
 *Returns: None
 */
void MainWindow::getWeatherClicked(){
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
    QUrl url("http://openweathermap.org/data/2.5/weather");

    //gets the current city
    int index = ui->dropdown_menu->currentIndex();

    //get the request for a particular city
    QNetworkRequest request = model::getRequest(url, index);

    //post the request
    manager->post(request, url.toEncoded());
}


/*celsiusButtonClicked
 *Params: None
 *Coverts temperature and min temperature to celsius
 *Returns: None
 */
void MainWindow::celsiusButtonClicked(){
    //Check if we have hit "get weather" yet
    if(dataRetrieved){
        ui->tempLabel->setText(converter(1, tempInCelsius));
        ui->minTempLabel->setText("Min: " + converter(1, minTempInCelsius));
}}


/*fahrenheitButtonClicked
 *Params: None
 *Coverts temperature and min temperature to fahrenheit
 *Returns: None
 */
void MainWindow::fahrenheitButtonClicked(){
    //Check if we have hit "get weather" yet
    if(dataRetrieved){
        ui->tempLabel->setText(converter(0, tempInCelsius));
        ui->minTempLabel->setText("Min: " + converter(0, minTempInCelsius));
}}


/*kelvinButtonClicked
 *Params: None
 *Coverts temperature and min temperature to kelvin
 *Returns: None
 */
void MainWindow::kelvinButtonClicked(){
    //Check if we have hit "get weather" yet
    if(dataRetrieved){
       ui->tempLabel->setText(converter(2, tempInCelsius));
       ui->minTempLabel->setText("Min: " + converter(2, minTempInCelsius));
}}


/*~MainWindow
 *Params: None
 *Destructor, deletes the current ui
 *Returns: None
 */
MainWindow::~MainWindow()
{
    delete ui;
}
