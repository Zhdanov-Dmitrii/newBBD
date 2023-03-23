#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include "Student.h"
#include "Entity.h"

class Book : public Entity {
private:
    QString name;
    QString author;
    QString subsection;
    int count;
    int countInStock;
    QString publisher;

public:
    Book();

    QString getName() const;

    QString getAuthor() const;

    QString getSubsection() const;

    int getCount() const;

    QString getPublisher() const;

    int getCountInStock() const;

    void setName(const QString &name);

    void setAuthor(const QString &author);

    void setSubsection(const QString &subsection);

    void setCount(const int count);

    void setPublisher(const QString &publisher);

    void setCountInStock(const int countInStock);

    virtual void initFromDB(QSqlQuery &query) override;
};

#endif // BOOK_H
