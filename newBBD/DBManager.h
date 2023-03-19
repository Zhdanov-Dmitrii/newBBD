#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTextEdit>
#include <optional>

#include "Entity/Book.h"
#include "Entity/Student.h"
#include "Entity/Entity.h"
#include "Entity/Author.h"
#include "Entity/Publisher.h"

class DBManager {
    QSqlDatabase db;

    void read(QSqlQuery &query, Entity &entity);

public:
    DBManager(QString filePath);

    bool open();

    void createTable();

    Book getBook(const int id);

    QList<Book> searchBooks(const QString &author, const QString &name, const QString &id, bool showOutOfStock);

    void insertBook(const int id, const QString &name, int count, const QList<Author>& authors, const QString &subsection,
                    std::optional<Publisher> publisher);

    void deleteBook(int id);

    Student getStudent(const int id);

    QList<Student> searchStudents(const QString &surname, const QString &name, const QString &course);

    void insertStudent(const QString &surname, const QString &name, const QString &course,
                       const QString &birthDate, const QString &telephone, const QString &address);

    void insertAuthor(const QString &author);

    void deleteStudent(const int id_student);

    QList<Author> searchAuthors(const QString &author);

    void insertPublisher(const QString &publisher);

    QList<Publisher> searchPublisher(const QString &publisher);

    void giveBook(int id_book, int id_student);

    void takeBook(int id_book, int id_student);

    QList<Student> getReaders(int id_book);

    QList<Book> getStudentBooks(int id_student);
};


#endif
