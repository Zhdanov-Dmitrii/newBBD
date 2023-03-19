//
// Created by user on 15.03.23.
//

#include "Publisher.h"

Publisher::Publisher() {

}

QString Publisher::getName() const {
    return name;
}

void Publisher::setName(const QString &name) {
    this->name = name;
}

void Publisher::initFromDB(QSqlQuery &query) {
    assert(query.isValid());

    id = query.value("id").toInt();
    name = query.value("name").toString();
}
