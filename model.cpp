/*Michael Lu
 *October 5th, 2013
 *model.cpp contains the functionality of the methods defined in model.h
 *model.cpp is the "model" element of the MVC and handles data retrieval
 */

#include "model.h"
#include <QUrlQuery>


/*model
 *Params: none
 *the model constructor
 *Returns: model object
 */
model::model()
{
}


/*getRequest
 *Params: (QUrl link, int index)
 *Gets a network request based on the provided url and the index(which determines the city)
 *Returns: QNetworkRequest
 */
QNetworkRequest model::getRequest(QUrl link, int index){
    QNetworkRequest request(link);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrlQuery query(link);
    query.addQueryItem("APPID","291cc02999b830ce1d7bf176d3d49172"); //our CS3307 APPID

    //index determines the city
    QString location = ("London, ca");

    if(index==1)
        location = ("Hamilton, ca");
    else if(index==2)
        location = ("Toronto, ca");
    else if(index==3)
        location = ("Vancouver, ca");
    else if(index==4)
        location = ("Saskatoon, ca");
    else if(index==5)
        location = ("Halifax, ca");

    query.addQueryItem("q", location);
    query.addQueryItem("units", "metric");
    link.setQuery(query);

    //update the request with the new query information.

    request.setUrl(link);

    return request;
}
