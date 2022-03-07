#ifndef DIALOGUPDATE_H
#define DIALOGUPDATE_H

#include <QDialog>

namespace Ui {
class DialogUpdate;
}

class DialogUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit DialogUpdate(QWidget *parent = nullptr, const bool isTree = 0);
    ~DialogUpdate();

private:
    Ui::DialogUpdate *ui;
    bool isTree;
    void closeEvent(QCloseEvent *event);

private slots:
    // 更新数据
    void on_btnUpdate_clicked();
    // 更新信息
    void on_spinBox_valueChanged(int id);
};
#endif // DIALOGUPDATE_H
