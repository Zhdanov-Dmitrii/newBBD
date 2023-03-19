#include "Book.h"

Book::Book() {

}

QString Book::getAuthor() const {
    return author;
}

QString Book::getSubsection() const {
    return subsection;
}

int Book::getCount() const {
    return count;
}

QString Book::getPublisher() const {
    return publisher;
}

void Book::setAuthor(const QString &author) {
    this->author = author;
}

void Book::setSubsection(const QString &subsection) {
    this->subsection = subsection;
}

void Book::setCount(const int count) {
    this->count = count;
}

void Book::setPublisher(const QString &publisher) {
    this->publisher = publisher;
}

void Book::initFromDB(QSqlQuery &query) {
    assert(query.isValid());

    id = query.value("id").toInt();
    name = query.value("name").toString();
    author = query.value("author").toString();
    subsection = query.value("subsection").toString();
    publisher = query.value("publisher").toString();
    count = query.value("count").toInt();
    countInStock = query.value("countinstock").toInt();
}

QString Book::getName() const {
    return name;
}

void Book::setName(const QString &name) {
    this->name = name;
}

int Book::getCountInStock() const {
    return countInStock;
}

void Book::setCountInStock(const int countInStock) {
    this->countInStock = countInStock;
}





