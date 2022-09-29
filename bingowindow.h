#ifndef BINGOWINDOW_H
#define BINGOWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "bingoitem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BingoWindow; }
QT_END_NAMESPACE

class BingoWindow : public QMainWindow
{
    Q_OBJECT

public:
    BingoWindow(QWidget *parent = nullptr);
    ~BingoWindow();
    QList<QString>* BingoItems;
    QGraphicsScene* scene;
    QList<BingoItem*>* BI;
    QList<bool>* BingoStates;

    void SetupBingo();
    
signals:
    void Bazooper();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_LoadBingoStuffButton_clicked();

    void on_StartBingoButton_clicked();

    void OnBingoButtonClicked(bool State, int ID);

private:
    Ui::BingoWindow *ui;
};
#endif // BINGOWINDOW_H
