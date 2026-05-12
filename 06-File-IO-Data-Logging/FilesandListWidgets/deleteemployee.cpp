#include "deleteemployee.h"
#include "ui_deleteemployee.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QStringListModel>
DeleteEmployee::DeleteEmployee(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DeleteEmployee)
{
    ui->setupUi(this);
    setWindowTitle("Delete Empployee");
}

DeleteEmployee::~DeleteEmployee()
{
    delete ui;
}




void DeleteEmployee::on_PB_Cancel_clicked()
{
    reject();
}


void DeleteEmployee::on_PB_DeleteEmployee_clicked()
{
    QString empId = ui->LE_DeleteEmpId->text().trimmed();

    if(empId.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter Employee ID");
        return;
    }

    QFile infile("EmployeeData.txt");
    QFile tempFile("temp.txt");

    if(!infile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Unable to open file");
        return;
    }

    if(!tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Unable to create temp file");
        return;
    }

    QTextStream in(&infile);
    QTextStream out(&tempFile);

    bool found = false;

    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList data = line.split(",");

        if(data.size() > 0)
        {
            if(data[0] == empId)
            {
                found = true;
                continue;
            }
        }

        out << line << "\n";
    }

    infile.close();
    tempFile.close();

    if(!found)
    {
        QFile::remove("temp.txt");
        QMessageBox::warning(this, "Error", "Employee ID not found");
        return;
    }

    QFile::remove("EmployeeData.txt");
    QFile::rename("temp.txt", "EmployeeData.txt");

    QMessageBox::information(this, "Success", "Employee deleted successfully");

    accept();
}

