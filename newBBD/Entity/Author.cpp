#include "Author.h"

Author::Author() {

}

QString Author::getName() const {
    return name;
}

void Author::setName(const QString &name) {
    this->name = name;
}

void Author::initFromDB(QSqlQuery &query) {
    assert(query.isValid());

    id = query.value("id").toInt();
    name = query.value("name").toString();
}
