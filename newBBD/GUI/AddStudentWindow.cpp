#include "AddStudentWindow.h"
#include "./ui_addstudentwindow.h"

AddStudentWindow::AddStudentWindow(QWidget *parent, DBManager *dbManager) :
    QDialog(parent),
    dbManager(dbManager),
    ui(new Ui::AddStudentWindow)
{
    ui->setupUi(this);
}

AddStudentWindow::~AddStudentWindow()
{
    delete ui;
}

void AddStudentWindow::on_save_clicked() {
    QString surname = ui->surname->text();
    QString name = ui->name->text();
    QString course = ui->course->text();
    QString birthDate = ui->birthDate->text();
    QString telephone = ui->telephone->text();
    QString address = ui->address->text();

    try {
        dbManager->insertStudent(surname, name, course, birthDate, telephone, address);
    } catch(QSqlError error) {
        QMessageBox::critical(this, "Ошибка", error.databaseText());
    }

    int ret = QMessageBox::information(this, "Уcпех",
                                       "Вы успешно добавили запись!\nЖелаете продолжить",
                                       QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::No) {
        close();
    }
}
