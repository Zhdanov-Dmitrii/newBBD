//
// Created by user on 15.03.23.
//

#ifndef NEWBBD_AUTHOR_H
#define NEWBBD_AUTHOR_H

#include <QString>

#include "Entity.h"

class Author : public Entity {
    QString name;

public:
    Author();

    QString getName() const;

    void setName(const QString &name);

    void initFromDB(QSqlQuery& query) override;
};


#endif //NEWBBD_AUTHOR_H
