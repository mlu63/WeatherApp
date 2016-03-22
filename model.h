/*Michael Lu
 *October 5th, 2013
 *model.h contains the function and variable declarations for this class
 */

#ifndef MODEL_H
#define MODEL_H

#include <QNetworkRequest>
#include <QUrl>


class model
{
public:
    model();

public slots:
    static QNetworkRequest getRequest(QUrl link, int index);
};

#endif // MODEL_H
