#ifndef DIALOGDELETE_H
#define DIALOGDELETE_H

#include <QDialog>

namespace Ui {
class DialogDelete;
}

class DialogDelete : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDelete(QWidget *parent = nullptr, const bool isTree = 0);
    ~DialogDelete();

private:
    Ui::DialogDelete *ui;
    bool isTree;
    void closeEvent(QCloseEvent *event);

private slots:
    // 删除数据
    void on_btnDelete_clicked();
    // 清空数据
    void on_btnClear_clicked();
    // 更新信息
    void on_spinBox_valueChanged(int id);
};

#endif // DIALOGDELETE_H
