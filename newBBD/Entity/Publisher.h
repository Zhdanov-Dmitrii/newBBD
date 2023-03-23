#ifndef NEWBBD_PUBLISHER_H
#define NEWBBD_PUBLISHER_H


#include <QString>
#include <QSqlQuery>
#include "Entity.h"

class Publisher : public Entity {
    QString name;

public:
    Publisher();

    QString getName() const;

    void setName(const QString &name);

    virtual void initFromDB(QSqlQuery &query);
};


#endif //NEWBBD_PUBLISHER_H
