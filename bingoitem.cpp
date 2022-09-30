#include "bingoitem.h"
#include <QGraphicsView>
#include <QDebug>
#include <QGraphicsProxyWidget>
#include <QLabel>
#define MaxLength 20

BingoItem::BingoItem(int PosX, int PosY, int XAmm,  QString Text)
{
    setRect(0,0,SizeX,SizeY);
    setPos(PosX * SizeX, PosY * SizeY);
    QLabel* MyText = new QLabel(Text);

    ID = PosX + (PosY * XAmm);
    qDebug() << ID;

    if(Text.length() > MaxLength)
    {
        QString buf = "";
        for (int i = 0; i < Text.length(); i++)
        {
            buf += Text[i];
            if(i % MaxLength == 0 && i != 0)
            {
                buf += "\n";
            }
        }
        MyText->setText(buf);
    }

    MyText->setAlignment(Qt::AlignCenter);
    MyText->setGeometry(PosX, PosY, SizeX, SizeY);
    MyText->setWordWrap(true);
    QPalette palette = MyText->palette();
    palette.setColor(MyText->backgroundRole(), Qt::transparent);
    palette.setColor(MyText->foregroundRole(), Qt::black);
    MyText->setPalette(palette);
    MyProxy = new QGraphicsProxyWidget(this);
    MyProxy->setPos(PosX * (SizeX / 2), PosY * (SizeY / 2));
    MyProxy->setWidget(MyText);
}

void BingoItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(ClickEnabled)
    {
        this->Checked = !this->Checked;
        if(this->Checked)
        {
            QBrush supercool(QPixmap(":/Bazoopie/BazooTile.png"));
            setBrush(supercool);
        }
        else
        {
            setBrush(Qt::white);
        }

        emit MePressed(this->Checked, this->ID);
    }
}

void BingoItem::Bazoopered()
{
    this->ClickEnabled = false;
}
