#include "Addbookwindow.h"
#include "./ui_addbookwindow.h"

AddBookWindow::AddBookWindow(QWidget *parent, DBManager *dbManager) :
        QDialog(parent),
        dbManager(dbManager),
        ui(new Ui::AddBookWindow) {
    ui->setupUi(this);
}

AddBookWindow::~AddBookWindow() {
    delete ui;
}

void AddBookWindow::showResultSearchAuthors() {
    ui->resultSearchAuthors->clear();
    for (auto author: resultSearchAuthor) {
        ui->resultSearchAuthors->addItem(author.getName());
    }
}

void AddBookWindow::showResultSearchPublisher() {
    ui->resultSearchPublisher->clear();
    for (auto publisher: resultSearchPublisher) {
        ui->resultSearchPublisher->addItem(publisher.getName());
    }
}


void AddBookWindow::on_searchAuthor_clicked() {
    QString author = ui->authorLE->text();

    try {
        resultSearchAuthor = dbManager->searchAuthors(author);
    } catch (QSqlError error) {
        QMessageBox::critical(this, "Ошибка", error.databaseText());
    }

    showResultSearchAuthors();
}


void AddBookWindow::on_searchPublisher_clicked() {
    QString publisher = ui->publisherLE->text();

    try {
        resultSearchPublisher = dbManager->searchPublisher(publisher);
    } catch (QSqlError error) {
        QMessageBox::critical(this, "Ошибка", error.databaseText());
    }

    showResultSearchPublisher();
}

void AddBookWindow::on_addPublisher_clicked() {
    QString publisher = ui->publisherLE->text();

    try {
        dbManager->insertPublisher(publisher);
    } catch (QSqlError error) {
        QMessageBox::critical(this, "Ошибка", error.databaseText());
    }
}

void AddBookWindow::on_addAuthor_clicked() {
    QString author = ui->authorLE->text();

    try {
        dbManager->insertAuthor(author);
    } catch (QSqlError error) {
        QMessageBox::critical(this, "Ошибка", error.databaseText());
    }
}

int AddBookWindow::getIndex(QListWidgetItem *item, QListWidget *widget) {
    for (int i = 0; i < widget->count(); ++i)
        if (widget->item(i) == item)
            return i;

    return -1;
}


void AddBookWindow::on_resultSearchAuthors_itemDoubleClicked(QListWidgetItem *item) {
    //qt возвращает только указатель на элемент так что так(
    int i = getIndex(item, ui->resultSearchAuthors);
    if (i == -1) return;

    if (ui->bookAuthors->findItems(resultSearchAuthor[i].getName(), Qt::MatchExactly).isEmpty()) {
        ui->bookAuthors->addItem(resultSearchAuthor[i].getName());
        bookAuthor.push_back(resultSearchAuthor[i]);
    }
}


void AddBookWindow::on_resultSearchPublisher_itemDoubleClicked(QListWidgetItem *item) {
    int i = getIndex(item, ui->resultSearchPublisher);
    if (i == -1) return;

    try {
        ui->bookPublisher->setText(resultSearchPublisher[i].getName());
    } catch (QSqlError error) {
        QMessageBox::critical(this, "Ошибка", error.databaseText());
    }
}


void AddBookWindow::on_save_clicked() {
    if (bookAuthor.empty()) {
        QMessageBox::critical(this, "Ошибка", "Не выбран не один автор!");
        return;
    }

    int id = ui->idLE->text().toInt();
    QString name = ui->name->text();
    int count = ui->count->text().toInt();
    QString subsection = ui->subsectionBook->currentText();

    try {
        dbManager->insertBook(id, name, count, bookAuthor, subsection, bookPublisher);
    } catch (QSqlError error) {
        QMessageBox::critical(this, "Ошибка", error.databaseText());
    }
}

