#include "DBManager.h"

DBManager::DBManager(const QJsonDocument& jsonDocument) {

    QString hostName = jsonDocument["hostName"].toString();
    QString databaseName = jsonDocument["databaseName"].toString();
    QString userName = jsonDocument["userName"].toString();
    QString password = jsonDocument["password"].toString();

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(hostName);
    db.setDatabaseName(databaseName);
    db.setUserName(userName);
    db.setPassword(password);
}

void DBManager::read(QSqlQuery &query, Entity &entity) {
    entity.initFromDB(query);
}

bool DBManager::open() {
    return db.open();
}

void DBManager::createTable() {
    QString queryStr =
            "CREATE TABLE IF NOT EXISTS public.publisher ( "
            "id serial primary key,"
            "name character varying(128) NOT NULL UNIQUE); "

            "CREATE TABLE IF NOT EXISTS public.author ( "
            "id serial primary key, "
            "name character varying(128) NOT NULL UNIQUE); "

            "CREATE TABLE IF NOT EXISTS public.book ( "
            "id integer NOT NULL, "
            "name character varying(128) NOT NULL, "
            "count integer NOT NULL DEFAULT 0, "
            "subsection character varying(64) NOT NULL, "
            "id_publisher integer, "
            "PRIMARY KEY (id), "
            "FOREIGN KEY (id_publisher) REFERENCES public.publisher (id) MATCH SIMPLE "
            "ON UPDATE NO ACTION "
            "ON DELETE NO ACTION "
            "NOT VALID); "

            "CREATE TABLE IF NOT EXISTS public.student( "
            "id serial NOT NULL, "
            "surname character varying(64) NOT NULL, "
            "name character varying(64) NOT NULL, "
            "birthDate character varying(64) NOT NULL,"
            "course character varying(64), "
            "telephone character varying(32), "
            "address character varying(64),  "
            "PRIMARY KEY (id)); "

            "CREATE TABLE IF NOT EXISTS public.bookStudent( "
            "id_book integer NOT NULL, "
            "id_student integer NOT NULL, "
            "PRIMARY KEY (id_book, id_student), "
            "FOREIGN KEY (id_book) REFERENCES public.book (id) MATCH SIMPLE "
            "ON UPDATE NO ACTION "
            "ON DELETE NO ACTION "
            "NOT VALID, "
            "FOREIGN KEY (id_student) REFERENCES public.student (id) MATCH SIMPLE "
            "ON UPDATE NO ACTION "
            "ON DELETE NO ACTION "
            "NOT VALID); "

            "CREATE TABLE IF NOT EXISTS public.bookAuthor ( "
            "id_book integer NOT NULL, "
            "id_author integer NOT NULL, "
            "PRIMARY KEY (id_book, id_author), "
            "FOREIGN KEY (id_author) REFERENCES public.author (id) MATCH SIMPLE "
            "ON UPDATE NO ACTION "
            "ON DELETE NO ACTION "
            "NOT VALID, "
            "FOREIGN KEY (id_book) REFERENCES public.book (id) MATCH SIMPLE "
            "ON UPDATE NO ACTION "
            "ON DELETE NO ACTION "
            "NOT VALID); ";

    qDebug() << queryStr;
    QSqlQuery query;
    if (!query.exec(queryStr)) {
        throw query.lastError();
    }
}

Book DBManager::getBook(const int id) {
    QString queryStr = "SELECT book.id AS id, book.name AS name, count, subsection, publisher.name AS publisher,\n"
                       "\tcount - coalesce(countss.counts, 0) AS countInStock, string_agg(author.name, ', ') AS author \n"
                       "FROM public.book LEFT JOIN publisher ON book.id_publisher = publisher.id \n"
                       "\tLEFT JOIN bookauthor ON book.id = bookauthor.id_book \n"
                       "\tLEFT JOIN author ON bookauthor.id_author = author.id \n"
                       "\tLEFT JOIN (SELECT id_book, COUNT(*) AS countS FROM bookStudent GROUP BY id_book) AS countss ON countss.id_book = book.id \n"
                       "WHERE book.id = ? \n"
                       "GROUP BY book.id, book.name, count, subsection, publisher.name, countInStock \n";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(0, id);

    if (!query.exec()) {
        throw query.lastError();
    }
    query.first();

    Book book;
    read(query, book);
    return book;
}

QList<Book> DBManager::searchBooks(const QString &author, const QString &name, const QString &id, bool showOutOfStock) {
    QList<Book> res;
    QString queryStr = "SELECT book.id AS id, book.name AS name, count, subsection, publisher.name AS publisher,\n"
                       "\tcount - coalesce(countss.counts, 0) AS countInStock, string_agg(author.name, ', ') AS author \n"
                       "FROM public.book LEFT JOIN publisher ON book.id_publisher = publisher.id \n"
                       "\tLEFT JOIN bookauthor ON book.id = bookauthor.id_book \n"
                       "\tLEFT JOIN author ON bookauthor.id_author = author.id \n"
                       "\tLEFT JOIN (SELECT id_book, COUNT(*) AS countS FROM bookStudent GROUP BY id_book) AS countss ON countss.id_book = book.id \n"
                       "WHERE CAST(book.id AS TEXT) LIKE '%'||?||'%' \n"
                       "\tAND LOWER(book.name) LIKE '%'||?||'%' \n";
    if (!showOutOfStock){
        queryStr += "\tAND count - countss.counts > 0 \n";
    }
    queryStr += "GROUP BY book.id, book.name, count, subsection, publisher.name, countInStock \n"
                "HAVING LOWER(string_agg(author.name, ', ')) LIKE '%'||?||'%' \n";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(0, id);
    query.bindValue(1, name.toLower());
    query.bindValue(2, author.toLower());

    qDebug() << query.lastQuery().remove('\n').remove('\t');

    if (!query.exec())
        throw query.lastError();

    while (query.next()) {
        Book book;
        read(query, book);
        res.push_back(book);
    }

    return res;
}

void DBManager::insertBook(const int id, const QString &name, int count, const QList<Author> &authors,
                           const QString &subsection, std::optional<Publisher> publisher) {
    QString queryStr = "BEGIN;\n"
                       "INSERT INTO book (id, name, count, subsection, id_publisher) VALUES ("
                       + QString::number(id) + ", '" + name + "', " + QString::number(count) + ", '" + subsection + "', ";
    queryStr +=  publisher ? "'" +publisher.value().getName() + "'" : "null";
    queryStr += ");\n";

    queryStr += "INSERT INTO bookAuthor (id_book, id_author) VALUES ";
    for (auto author: authors) {
        queryStr += "(" + QString::number(id) + ", " + QString::number(author.getId()) + "),";
    }
    queryStr.erase(queryStr.begin() + queryStr.size() - 1, queryStr.end()); // удаление последней запятой \(*.*)/

    queryStr += ";\nCOMMIT;";
    qDebug() << queryStr.remove('\n').remove('\t');

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        throw query.lastError();
    }
}

void DBManager::deleteBook(int id) {
    QString queryStr = "BEGIN; "
                       "DELETE FROM bookStudent WHERE id_book = "+QString::number(id)+";"
                                                                                      "DELETE FROM bookAuthor WHERE id_book = "+QString::number(id)+";"
                                                                                                                                                    "DELETE FROM book WHERE id = "+QString::number(id)+";"
                                                                                                                                                                                                       "COMMIT;";
    qDebug() << queryStr;
    QSqlQuery query;
    if(!query.exec(queryStr)) {
        throw query.lastError();
    }
}

Student DBManager::getStudent(const int id) {
    QString queryStr = "SELECT id, surname, name, course, telephone, address, birthdate, \n"
                       "\t(SELECT COUNT(*) FROM bookStudent WHERE id_student = id) AS countbook \n"
                       "FROM student \n"
                       "WHERE id = ?";
    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(0, id);

    if(!query.exec()) {
        throw query.lastError();
    }
    query.first();
    Student student;
    read(query, student);
    return student;
}

QList<Student> DBManager::searchStudents(const QString &surname, const QString &name, const QString &course) {
    QList<Student> res;
    QString queryStr = "SELECT id, surname, name, course, telephone, address, birthdate, \n"
                       "\t(SELECT COUNT(*) FROM bookStudent WHERE id_student = id) AS countbook \n"
                       "FROM student \n"
                       "WHERE surname LIKE '%'||:surname||'%' \n"
                       "\tAND name LIKE '%'||:name||'%' \n"
                       "\tAND course LIKE '%'||:course||'%' \n";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":surname", surname);
    query.bindValue(":name", name);
    query.bindValue(":course", course);

    qDebug() << query.lastQuery();
    if (!query.exec()) {
        throw query.lastError();
    }

    while (query.next()) {
        Student student;
        read(query, student);
        res.push_back(student);
    }
    return res;
}

void DBManager::insertStudent(const QString &surname, const QString &name, const QString &course,
                              const QString &birthDate, const QString &telephone, const QString &address) {
    QString queryStr = "INSERT INTO student (surname, name, course, birthDate, telephone, address) "
                       "VALUES (:surname, :name, :course, :birthDate, :telephone, :address);";
    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":surname", surname);
    query.bindValue(":name", name);
    query.bindValue(":course", course);
    query.bindValue(":birthDate", birthDate);
    query.bindValue(":telephone", telephone);
    query.bindValue(":address", address);

    if (!query.exec()) {
        qDebug() << query.lastQuery();
        //throw query.lastError();
    }
}

void DBManager::deleteStudent(const int id_student) {
    QString queryStr = "BEGIN; \n"
                       "DELETE FROM bookStudent WHERE id_student = ?; \n"
                       "DELETE FROM student WHERE id = ?"
                       "COMMIT;";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(0, id_student);
    query.bindValue(1, id_student);

    if(!query.exec()) {
        throw query.lastError();
    }
}

void DBManager::insertAuthor(const QString &author) {
    QString queryStr = "INSERT INTO author (name) VALUES (:author)";
    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(0, author);
    if (!query.exec()) {
        throw query.lastError();
    }
    qDebug() << query.lastQuery();
}

QList<Author> DBManager::searchAuthors(const QString &name) {
    QString queryStr = "SELECT id, name FROM author ";
    if (!name.isEmpty()) {
        queryStr += "WHERE name LIKE '%" + name + "%';";
    }
    qDebug() << queryStr;

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        throw db.lastError();
    }

    QList<Author> res;
    while (query.next()) {
        Author author;
        read(query, author);
        res.push_back(author);
    }

    return res;
}

void DBManager::insertPublisher(const QString &publisher) {
    QString queryStr = "INSERT INTO publisher VALUES (" + publisher + ")";
    QSqlQuery query;
    if (!query.exec(queryStr)) {
        throw query.lastError();
    }
}

QList<Publisher> DBManager::searchPublisher(const QString &name) {
    QString queryStr = "SELECT id, name FROM publisher";
    if (!name.isEmpty()) {
        queryStr += " WHERE name LIKE '%" + name + "%';";
    }

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        throw db.lastError();
    }

    QList<Publisher> res;
    while (query.next()) {
        Publisher publisher;
        read(query, publisher);

        res.push_back(publisher);
    }

    return res;
}

void DBManager::giveBook(int id_book, int id_student) {
    QString queryStr = "INSERT INTO bookstudent (id_book, id_student) VALUES (?, ?);";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(0, id_book);
    query.bindValue(1, id_student);

    qDebug() << query.lastQuery().remove('\n').remove('\t');
    if (!query.exec()) {
        throw query.lastError();
    }
}

void DBManager::takeBook(int id_book, int id_student) {
    QString queryStr = "DELETE FROM bookstudent WHERE id_book = ? AND id_student = ?";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(0, id_book);
    query.bindValue(1, id_student);

    qDebug() << query.lastQuery().remove('\n').remove('\t');
    if (!query.exec()) {
        throw query.lastError();
    }
}

QList<Student> DBManager::getReaders(int id_book) {
    QString queryStr = "SELECT id, surname, name, course, telephone, address, birthdate, \n"
                       "\t(SELECT COUNT(*) FROM bookStudent WHERE id_student = id) AS countbook \n"
                       "FROM student LEFT JOIN bookstudent ON id = id_student \n"
                       "WHERE id_book = ?;";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(0, id_book);

    qDebug() << query.lastQuery().remove('\n').remove('\t');
    if(!query.exec()) {
        throw query.lastError();
    }

    QList<Student> res;
    while (query.next()) {
        Student student;
        read(query, student);
        res.push_back(student);
    }

    return res;
}

QList<Book> DBManager::getStudentBooks(int id_student) {
    QString queryStr = "SELECT book.id AS id, book.name AS name, count, subsection, publisher.name AS publisher,\n"
                       "\tcount - coalesce(countss.counts, 0) AS countInStock, string_agg(author.name, ', ') AS author \n"
                       "FROM public.book LEFT JOIN publisher ON book.id_publisher = publisher.id \n"
                       "\tLEFT JOIN bookauthor ON book.id = bookauthor.id_book \n"
                       "\tLEFT JOIN author ON bookauthor.id_author = author.id \n"
                       "\tLEFT JOIN (SELECT id_book, COUNT(*) AS countS FROM bookStudent GROUP BY id_book) AS countss ON countss.id_book = book.id \n"
                       "\tLEFT JOIN bookstudent ON bookstudent.id_book = book.id \n"
                       "WHERE id_student = ? \n"
                       "GROUP BY book.id, book.name, count, subsection, publisher.name, countInStock \n";

    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(0, id_student);

    qDebug() << query.lastQuery().remove('\n').remove('\t');

    if(!query.exec()) {
        throw query.lastError();
    }

    QList<Book> res;
    while (query.next()) {
        Book book;
        read(query, book);
        res.push_back(book);
    }

    return res;
}

