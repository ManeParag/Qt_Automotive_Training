#ifndef DELETEEMPLOYEE_H
#define DELETEEMPLOYEE_H

#include <QDialog>

namespace Ui {
class DeleteEmployee;
}

class DeleteEmployee : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteEmployee(QWidget *parent = nullptr);
    ~DeleteEmployee();

private slots:

    void on_PB_Cancel_clicked();


    void on_PB_DeleteEmployee_clicked();

private:
    Ui::DeleteEmployee *ui;
};

#endif // DELETEEMPLOYEE_H
