#ifndef CHOOSEWINDOW_H
#define CHOOSEWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "dswindow.h"
#include "helpwindow.h"
#include "btreewindow.h"

namespace Ui {
class ChooseWindow;
}

class ChooseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChooseWindow(QWidget *parent = nullptr);
    ~ChooseWindow();

private:
    Ui::ChooseWindow *ui;
    // 子窗口
    DSWindow dsWin;         // 综合演示窗口
    HelpWindow helpWin;     // 帮助窗口
    BTreeWindow bTreeWin;   // B-树演示窗口

private slots:
    // 窗口切换槽
    void toDSWin();
    void toHelpWin();
    void toBTreeWin();
    void toChooseWin();
};

#endif // CHOOSEWINDOW_H
