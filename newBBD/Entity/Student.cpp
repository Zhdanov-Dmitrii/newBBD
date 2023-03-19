#include "Student.h"

Student::Student() {

}

QString Student::getSurname() const {
    return surname;
}

QString Student::getCourse() const {
    return course;
}

QString Student::getBirthDate() const {
    return birthDate;
}

int Student::getCountBook() const {
    return countBook;
}

QString Student::getTelephone() const {
    return telephone;
}

void Student::setSurname(const QString &surname) {
    this->surname = surname;
}

void Student::setCourse(const QString &course) {
    this->course = course;
}

void Student::setBirthDate(const QString &birthDate) {
    this->birthDate = birthDate;
}

void Student::setCountBook(int countBook) {
    this->countBook = countBook;
}

void Student::setTelephone(const QString &telephone) {
    this->telephone = telephone;
}

void Student::setAddress(const QString &address) {
    this->address = address;
}

QString Student::getName() const {
    return name;
}

void Student::setName(const QString &name) {
    this->name = name;
}

void Student::initFromDB(QSqlQuery &query) {
    assert(query.isValid());

    id = query.value("id").toInt();
    surname = query.value("surname").toString();
    name = query.value("name").toString();
    course = query.value("course").toString();
    telephone = query.value("telephone").toString();
    birthDate = query.value("birthdate").toString();
    address = query.value("address").toString();
    countBook = query.value("countbook").toInt();
}

QString Student::getAddress() const {
    return address;
}
