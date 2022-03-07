#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QMainWindow>

namespace Ui {
class HelpWindow;
}

class HelpWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HelpWindow(QWidget *parent = nullptr);
    ~HelpWindow();

private:
    Ui::HelpWindow *ui;

private slots:
    // 窗口切换槽
    void backToChooseWin(); // 返回选择窗口

signals:
    void helpHidden();
};

#endif // HELPWINDOW_H
