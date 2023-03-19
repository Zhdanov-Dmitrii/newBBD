#ifndef ADDBOOKWINDOW_H
#define ADDBOOKWINDOW_H

#include <QDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <optional>
#include "../DBManager.h"

namespace Ui {
    class AddBookWindow;
}

class AddBookWindow : public QDialog {
Q_OBJECT

public:
    explicit AddBookWindow(QWidget *parent = nullptr, DBManager *dbManager = nullptr);

    ~AddBookWindow();

private slots:

    void on_addAuthor_clicked();

    void on_searchAuthor_clicked();

    void on_searchPublisher_clicked();

    void on_addPublisher_clicked();


    void on_resultSearchAuthors_itemDoubleClicked(QListWidgetItem *item);

    void on_resultSearchPublisher_itemDoubleClicked(QListWidgetItem *item);

    void on_save_clicked();

private:
    DBManager *dbManager;
    QList<Author> resultSearchAuthor;
    QList<Author> bookAuthor;
    QList<Publisher> resultSearchPublisher;
    std::optional<Publisher> bookPublisher = std::nullopt;
    Ui::AddBookWindow *ui;

    void showResultSearchAuthors();

    void showResultSearchPublisher();

    int getIndex(QListWidgetItem *item, QListWidget *widget);
};

#endif // ADDBOOKWINDOW_H
