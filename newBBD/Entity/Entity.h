#ifndef ENTITY_H
#define ENTITY_H

#include <QSqlQuery>

class Entity {
protected:
    int id;
public:

    int getId() const;

    void setId(const int id);

    virtual void initFromDB(QSqlQuery &query) = 0;
};

#endif // ENTITY_H
