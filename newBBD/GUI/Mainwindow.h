#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTableWidget>
#include <QMessageBox>
#include <QFile>

#include "../Entity/Book.h"
#include "../Entity/Student.h"
#include "../DBManager.h"
#include "../GUI/Addbookwindow.h"
#include "../GUI/AddStudentWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_searchBook_clicked();

    void on_searchStudent_clicked();

    void on_addBook_clicked();

    void on_deleteBook_clicked();

    void on_addStudent_clicked();

    void on_takeBook_clicked();

    void on_giveBook_clicked();

    void on_bookReaders_clicked();

    void on_studentBooks_clicked();

    void on_clear_clicked();

    void on_update_clicked();
private:
    Ui::MainWindow *ui;

    DBManager* dbManager;

    QList<Book> booksShown;
    QList<Student> studentsShown;

    QStringList bookSortingCriteria = {"book.name", "author", "id", "count", "countInStock",
                                       "subsection", "publisher"};

    QStringList studentSortingCriteria = {"surname", "name", "course", "countBook", "brithDate",
                                          "telephone", "address"};

    void showBook(int i);
    void showAllBooks();

    void showStudent(int i);
    void showAllStudent();

    int getIndex(QTableWidget *qTableWidget, QTableWidgetItem *item);

};
#endif // MAINWINDOW_H
