#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QRegularExpressionValidator>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QTextStream>
#include <QHeaderView>
#include "deleteemployee.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("EDAG Employee Management System");

    QRegularExpression rxEmpId("^[A-Za-z0-9]{4,10}$");
    ui->LE_EmpId->setValidator(new QRegularExpressionValidator(rxEmpId, this));

    QRegularExpression rxName("^[A-Za-z ]{2,50}$");
    ui->LE_Name->setValidator(new QRegularExpressionValidator(rxName, this));

    QRegularExpression rxAddress("^[A-Za-z0-9 ,./-]{5,100}$");
    ui->LE_Address->setValidator(new QRegularExpressionValidator(rxAddress, this));

    QRegularExpression rxGrade("^[A-E]{1}$");
    ui->LE_Grade->setValidator(new QRegularExpressionValidator(rxGrade, this));

    QRegularExpression rxSalary("^[0-9]{1,7}$");
    ui->LE_Salary->setValidator(new QRegularExpressionValidator(rxSalary, this));
    loadEmployeeData();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::loadEmployeeData()
{
/*  QFile file("EmployeeData.txt");
    QStringList list;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while(!in.atEnd())
        {
            QString line = in.readLine();

            // Optional: format nicely
            QStringList data = line.split(",");

            if(data.size() == 5)
            {
                QString formatted =
                    "ID: " + data[0] +
                    " | Name: " + data[1] +
                    " | Addr: " + data[2] +
                    " | Grade: " + data[3] +
                    " | Salary: " + data[4];

                list.append(formatted);
            }
        }
        file.close();
    }

    QStringListModel *model = new QStringListModel(this);
    model->setStringList(list);

    ui->LV_EmployeeData->setModel(model);
*/

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"Emp ID", "Name", "address", "Grade", "Salary"});
    QFile file("EmployeeData.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "File not found!";
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList cols = line.split(",");

        QList<QStandardItem*> items;

        for (const QString &col : cols) {
            items.append(new QStandardItem(col.trimmed()));
        }

        model->appendRow(items);
    }

    file.close();

    ui->TB_Employee_Data->setModel(model);
    ui->TB_Employee_Data->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void Widget::on_PB_Add_clicked()
{
    QString empId   = ui->LE_EmpId->text();
    QString name    = ui->LE_Name->text();
    QString address = ui->LE_Address->text();
    QString grade   = ui->LE_Grade->text();
    QString salary  = ui->LE_Salary->text();

    // Basic validation
    if(empId.isEmpty() || name.isEmpty())
    {
        QMessageBox::warning(this, "Error", "EmpId and Name required");
        return;
    }

    QFile file("EmployeeData.txt");

    if(file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << empId << "," << name << "," << address << ","
            << grade << "," << salary << "\n";

        file.close();
    }

    // Clear fields
    ui->LE_EmpId->clear();
    ui->LE_Name->clear();
    ui->LE_Address->clear();
    ui->LE_Grade->clear();
    ui->LE_Salary->clear();

    // Reload data
    loadEmployeeData();
}


void Widget::on_PB_Search_clicked()
{

}


void Widget::on_PB_Delete_clicked()
{
    DeleteEmployee deleteEmployeeDialog(this);
    if(deleteEmployeeDialog.exec() == QDialog::Accepted)
    {
        loadEmployeeData();
    }
}


void Widget::on_PB_Exit_clicked()
{
    QApplication::quit();
}

