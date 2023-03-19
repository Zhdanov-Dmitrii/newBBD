#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include "Entity.h"

class Student : public Entity {
private:
    QString name;
    QString surname;
    QString course;
    QString birthDate;
    int countBook;
    QString telephone;
    QString address;


public:
    Student();

    QString getName() const;

    QString getSurname() const;

    QString getCourse() const;

    QString getBirthDate() const;

    int getCountBook() const;

    QString getTelephone() const;

    QString getAddress() const;

    void setName(const QString &name);

    void setSurname(const QString &surname);

    void setCourse(const QString &course);

    void setBirthDate(const QString &birthDate);

    void setCountBook(int countBook);

    void setTelephone(const QString &telephone);

    void setAddress(const QString &address);

    void initFromDB(QSqlQuery &query) override;
};

#endif // STUDENT_H
