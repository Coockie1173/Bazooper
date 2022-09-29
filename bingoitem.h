#ifndef BINGOITEM_H
#define BINGOITEM_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QWidget>
#include <QGraphicsProxyWidget>

#define SizeX 128
#define SizeY 128

class BingoItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    ///
    /// \brief BingoItem
    /// \param PosX index in grid
    /// \param PosY index in grid
    ///
    BingoItem(int PosX, int PosY, int XAmm, QString Text); //not a fan of vectors for this stuff :)
    QGraphicsProxyWidget* MyProxy;

public slots:
    void Bazoopered();

signals:
    void MePressed(bool State, int ID);

private:
    bool Checked = false;
    bool ClickEnabled = true;
    int ID = 0;
};

#endif // BINGOITEM_H
