#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;
    void loadEmployeeData();

private slots:
    void on_PB_Add_clicked();

    void on_PB_Search_clicked();

    void on_PB_Delete_clicked();

    void on_PB_Exit_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
