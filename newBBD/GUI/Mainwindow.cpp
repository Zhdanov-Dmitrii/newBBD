#include "Mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFile file("../dbConfig.json");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл");
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
    file.close();

    dbManager = new DBManager(jsonDocument);

    if(!dbManager->open())
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к базе данных");
    try {
        dbManager->createTable();
    } catch(QSqlError error) {
        QMessageBox::critical(this, "Ошибка", error.databaseText());
    }
}

MainWindow::~MainWindow()
{
    delete dbManager;
    delete ui;
}

void MainWindow::showBook(int i)
{
    if (i >= booksShown.size() || i >= ui->tableBook->rowCount())
        return;

    Book& book = booksShown[i];

    ui->tableBook->setItem(i, 0, new QTableWidgetItem(book.getName()));
    ui->tableBook->setItem(i, 1, new QTableWidgetItem(book.getAuthor()));
    ui->tableBook->setItem(i, 2, new QTableWidgetItem(QString::number(book.getId())));
    ui->tableBook->setItem(i, 3, new QTableWidgetItem(QString::number(book.getCount())));
    ui->tableBook->setItem(i, 4, new QTableWidgetItem(QString::number(book.getCountInStock())));
    ui->tableBook->setItem(i, 5, new QTableWidgetItem(book.getSubsection()));
    ui->tableBook->setItem(i, 6, new QTableWidgetItem(book.getPublisher()));
}

void MainWindow::showAllBooks()
{
    ui->tableBook->clearContents();
    ui->tableBook->setRowCount(booksShown.size());

    for(int i = 0; i < booksShown.size(); ++i)
        showBook(i);

    ui->tableBook->resizeColumnsToContents();
}

void MainWindow::showStudent(int i)
{
    if(i >= studentsShown.size() || i >= ui->tableStudent->rowCount())
        return;

    Student& student = studentsShown[i];

    ui->tableStudent->setItem(i, 0, new QTableWidgetItem(student.getSurname()));
    ui->tableStudent->setItem(i, 1, new QTableWidgetItem(student.getName()));
    ui->tableStudent->setItem(i, 2, new QTableWidgetItem(student.getCourse()));
    ui->tableStudent->setItem(i, 3, new QTableWidgetItem(QString::number(student.getCountBook())));
    ui->tableStudent->setItem(i, 4, new QTableWidgetItem(student.getBirthDate()));
    ui->tableStudent->setItem(i, 5, new QTableWidgetItem(student.getTelephone()));
    ui->tableStudent->setItem(i, 6, new QTableWidgetItem(student.getAddress()));
}

void MainWindow::showAllStudent()
{
    ui->tableStudent->clearContents();
    ui->tableStudent->setRowCount(studentsShown.size());

    for(int i = 0; i < studentsShown.size(); ++i)
        showStudent(i);

    ui->tableStudent->resizeColumnsToContents();
}
void MainWindow::on_searchBook_clicked()
{
    QString author = ui->authorBook->displayText();
    QString name = ui->nameBook->displayText();
    QString id = ui->numberBook->displayText();
    QString subsection = ui->subsectionBook->currentText();
    bool inStock = ui->inStock;

    booksShown = dbManager->searchBooks(author, name, id, inStock);

    showAllBooks();
}


void MainWindow::on_searchStudent_clicked()
{
    QString surname = ui->surnameStudent->text();
    QString name = ui->nameStudent->text();
    QString course = ui->courseStudent->text();
    studentsShown = dbManager->searchStudents(surname, name, course);

    showAllStudent();
}


void MainWindow::on_addBook_clicked()
{
    AddBookWindow* addBookWindow = new AddBookWindow(this, dbManager);
    addBookWindow->show();
}

void MainWindow::on_deleteBook_clicked() {
    int ret = QMessageBox::warning(this, "Предупреждение",
                         "Данное действие приведет к изъятию книг у пользователей\n"
                         "Вы хотите продолжить?",
                         QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::No) {
        return;
    }

    int ind = ui->tableBook->currentRow();
    try {
        dbManager->deleteBook(booksShown[ind].getId());
    } catch (QSqlError error) {
        QMessageBox::critical(this, "Error", error.databaseText());
    }
    booksShown.remove(ind);
    ui->tableBook->removeRow(ind);
}

void MainWindow::on_addStudent_clicked() {
    AddStudentWindow *addStudentWindow = new AddStudentWindow(this, dbManager);
    addStudentWindow->show();
}

void MainWindow::on_takeBook_clicked() {
    int currentBookRow = ui->tableBook->currentRow();
    int currentStudentRow = ui->tableStudent->currentRow();
    Book book = booksShown[currentBookRow];
    Student student = studentsShown[currentStudentRow];

    try {
        dbManager->takeBook(book.getId(), student.getId());

        booksShown[currentBookRow] = dbManager->getBook(book.getId());
        showBook(currentBookRow);

        studentsShown[currentStudentRow] = dbManager->getStudent(student.getId());
        showStudent(currentStudentRow);
    } catch (QSqlError error) {

    }
}

void MainWindow::on_giveBook_clicked() {
    int currentBookRow = ui->tableBook->currentRow();
    int currentStudentRow = ui->tableStudent->currentRow();
    Book book = booksShown[currentBookRow];
    Student student = studentsShown[currentStudentRow];

    try {
        dbManager->giveBook(book.getId(), student.getId());

        booksShown[currentBookRow] = dbManager->getBook(book.getId());
        showBook(currentBookRow);

        studentsShown[currentStudentRow] = dbManager->getStudent(student.getId());
        showStudent(currentStudentRow);
    } catch (QSqlError error) {

    }
}

void MainWindow::on_bookReaders_clicked() {
    Book book = booksShown[ui->tableBook->currentRow()];
    try {
        studentsShown = dbManager->getReaders(book.getId());
    } catch (QSqlError error) {

    }

    showAllStudent();
}

void MainWindow::on_studentBooks_clicked() {
    Student student = studentsShown[ui->tableStudent->currentRow()];
    try {
        booksShown = dbManager->getStudentBooks(student.getId());
    } catch (QSqlError error) {

    }

    showAllBooks();
}

void MainWindow::on_clear_clicked() {
    ui->tableBook->clearContents();
    ui->tableBook->resizeColumnsToContents();

    ui->tableStudent->clearContents();
    ui->tableStudent->resizeColumnsToContents();
}

void MainWindow::on_update_clicked() {
    QList<Book> books;
    for(auto book : booksShown) {
        try {
            books.push_back(dbManager->getBook(book.getId()));
        } catch (QSqlError error) {

        }
    }
    booksShown = books;

    QList<Student> students;
    for(auto student : studentsShown) {
        try {
            students.push_back(dbManager->getStudent(student.getId()));
        } catch (QSqlError error) {

        }
    }
    studentsShown = students;

    showAllBooks();
    showAllStudent();
}
