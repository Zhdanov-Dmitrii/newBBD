#ifndef ADDSTUDENTWINDOW_H
#define ADDSTUDENTWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include "DBManager.h"

namespace Ui {
class AddStudentWindow;
}

class AddStudentWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddStudentWindow(QWidget *parent = nullptr, DBManager *dbManager = nullptr);
    ~AddStudentWindow();
private slots:
    void on_save_clicked();

private:
    Ui::AddStudentWindow *ui;
    DBManager *dbManager;
};

#endif // ADDSTUDENTWINDOW_H
