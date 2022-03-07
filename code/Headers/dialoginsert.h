#ifndef DIALOGINSERT_H
#define DIALOGINSERT_H

#include <QDialog>

namespace Ui {
class DialogInsert;
}

class DialogInsert : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInsert(QWidget *parent = nullptr, const bool isTree = 0);
    ~DialogInsert();

private:
    Ui::DialogInsert *ui;
    bool isTree;
    void closeEvent(QCloseEvent *event);

private slots:
    // 插入数据
    void on_btnInsert_clicked();
    // 初始化数据
    void on_btnInitial_clicked();
};

#endif // DIALOGINSERT_H
