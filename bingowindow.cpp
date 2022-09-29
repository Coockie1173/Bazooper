#include "bingowindow.h"
#include "ui_bingowindow.h"
#include <QFileDialog>
#include <QList>
#include <QTextStream>
#include <QDebug>
#include <algorithm>
#include <random>
#include <time.h>
#include <QtMath>
#include <QPoint>
#include <QMovie>
#include <QTimer>
#include "bingoitem.h"
#define SizeX 128
#define SizeY 128


void BingoWindow::SetupBingo()
{
    scene = new QGraphicsScene();
    ui->BingoBord->setScene(scene);
    BI = new QList<BingoItem*>();
    BingoStates = new QList<bool>();
    ui->stackedWidget->setCurrentIndex(0);
}

BingoWindow::BingoWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BingoWindow)
{
    ui->setupUi(this);
    this->BingoItems = new QList<QString>();
    qsrand(time(NULL));
    SetupBingo();
}

BingoWindow::~BingoWindow()
{
    delete ui;
}


void BingoWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->SliderValueLabel->setText(QString::number(value));
    if(this->BingoItems->count() > ui->horizontalSlider->value() * ui->horizontalSlider->value())
    {
        ui->StartBingoButton->setEnabled(true);
    }
    else
    {
        ui->StartBingoButton->setEnabled(false);
    }
}


void BingoWindow::on_LoadBingoStuffButton_clicked()
{
    QStringList filters({"Text files (*.txt)",
                               "Any files (*)"
                              });
    //load in bingo information
    QFileDialog Diag(this);
    Diag.setNameFilters(filters);
    if(Diag.exec())
    {
        //Diag worked, get input file :)
        ui->FilePath->setText(Diag.selectedFiles().first());
        QFile InputFile(Diag.selectedFiles().first());
        if (InputFile.open(QIODevice::ReadOnly))
        {
            QTextStream in(&InputFile);
            while (!in.atEnd())
            {
                //read every line
               QString line = in.readLine();
               if(line != "")
               {
                   //qDebug() << line;
                   this->BingoItems->append(line);
               }
            }
            InputFile.close();
        }

        if(this->BingoItems->count() > ui->horizontalSlider->value() * ui->horizontalSlider->value())
        {
            ui->StartBingoButton->setEnabled(true);
        }
        else
        {
            ui->StartBingoButton->setEnabled(false);
        }
    }

}


void BingoWindow::on_StartBingoButton_clicked()
{
    //shuffle list, grab top X²
    std::random_shuffle(this->BingoItems->begin(), this->BingoItems->end());
    ui->stackedWidget->setCurrentIndex(1);

    scene->setSceneRect(0,0, ui->horizontalSlider->value() * SizeX, ui->horizontalSlider->value() * SizeY);
    for(int x = 0; x < ui->horizontalSlider->value(); x++)
    {
        for(int y = 0; y < ui->horizontalSlider->value(); y++)
        {
            BingoItem* Bingo = new BingoItem(x,y, ui->horizontalSlider->value() ,this->BingoItems->at(x + y * ui->horizontalSlider->value()));
            BI->append(Bingo);
            BingoStates->append(false);
            ui->BingoBord->scene()->addItem(Bingo);
            connect(Bingo, &BingoItem::MePressed, this, &BingoWindow::OnBingoButtonClicked);
            connect(this, &BingoWindow::Bazooper, Bingo, &BingoItem::Bazoopered);
        }
    }
}

void BingoWindow::OnBingoButtonClicked(bool State, int ID)
{
    //separate array????? yes :chad:
    BingoStates->replace(ID,State);
    //iterate over these BAD BOYS and see if there's any lines or some idk
    int Size = ui->horizontalSlider->value();
    int BingoType = 0;

    int StartPos = -1;
    int EndPos = -1;

    //horrible and ugly code but I'm lazy so deal with it
    bool Bazooper = false;
    //go over X first I guess aye
    for(int i = 0; i < BingoStates->count(); i+= Size)
    {
        if(!Bazooper)
        {
            StartPos = i;
            Bazooper = true;
            for(int x = i; x < i + Size; x++)
            {
                BingoType = 0;
                Bazooper &= BingoStates->at(x);
                EndPos = x;
            }
        }
    }

    if(!Bazooper) //skip if already bazoopered
    {
        //do Y now
        for(int i = 0; i < Size; i++)
        {
            if(!Bazooper)
            {
                Bazooper = true;
                for(int y = 0; y < Size; y++)
                {
                    StartPos = i;
                    Bazooper &= BingoStates->at(y * Size + i);
                    BingoType = 1;
                    EndPos = (y * Size + i);
                }
            }
        }
    }

    if(!Bazooper) //skip AGAIN if already bazoopered
    {
        //now do schuine zijdekes
        Bazooper = true;
        StartPos = 0;
        for(int i = 0; i < Size * Size; i += Size + 1)
        {
            Bazooper &= BingoStates->at(i);
            EndPos = i;
            BingoType = 2;
        }
        if(!Bazooper) //break didn't want to work here so I'm crying :(
        {
            Bazooper = true;
            StartPos = Size * Size - (Size);
            for(int i = Size * Size - (Size); i > 0; i -= Size - 1)
            {
                Bazooper &= BingoStates->at(i);
                EndPos = i;
                BingoType = 3;
            }
        }
    }

    if(Bazooper)
    {
        QGraphicsLineItem *line;
        //I really do be writing jank doe
        QPen pen;
        pen.setWidth(8);
        pen.setCapStyle(Qt::RoundCap);
        switch(BingoType)
        {
            case 0: //horizontal
            {
                line = scene->addLine((StartPos % Size) * SizeX, qFloor((float)StartPos / (float)Size) * SizeY + SizeY / 2
                                  , (EndPos % Size) * SizeX + SizeX, qFloor((float)EndPos / (float)Size) * SizeY + SizeY / 2, pen);
                break;
            }
            case 1: //vertigo
            {
                line = scene->addLine((StartPos % Size) * SizeX + SizeX / 2, qFloor((float)StartPos / (float)Size) * SizeY
                              , (EndPos % Size) * SizeX + SizeX / 2, qFloor((float)EndPos / (float)Size) * SizeY + SizeY, pen);
                break;
            }
            case 2:
            {
                line = scene->addLine((StartPos % Size) * SizeX, qFloor((float)StartPos / (float)Size) * SizeY
                                  , (EndPos % Size) * SizeX + SizeX, qFloor((float)EndPos / (float)Size) * SizeY + SizeY, pen);
                break;
            }
            case 3:
            {
                line = scene->addLine((StartPos % Size) * SizeX, qFloor((float)StartPos / (float)Size) * SizeY + SizeY
                                  , (EndPos % Size) * SizeX + SizeX, qFloor((float)EndPos / (float)Size) * SizeY, pen);
                break;
            }
        }

        QLabel *gif_anim = new QLabel();
        QMovie *movie = new QMovie(":/Bazoopie/out.GIF");
        movie->setBackgroundColor(Qt::white);
        if (!movie->isValid())
        {
            // Something went wrong :(
            qDebug() << "oops";
        }
        gif_anim->setMovie(movie);
        movie->start();
        connect(movie, &QMovie::frameChanged, this, [=](){
        if(movie->currentFrameNumber() == (movie->frameCount() - 1))
            {
                movie->stop();
                QTimer::singleShot(2000, this, [=](){
                    gif_anim->hide();
                    delete gif_anim;
                    QTimer::singleShot(5000, this, [=]{
                        SetupBingo();
                    });
                });
            }
        });


        QGraphicsProxyWidget *proxy = scene->addWidget(gif_anim);
        //ui->BingoBord->fitInView(proxy);
        QPointF center = ui->BingoBord->mapToScene(ui->BingoBord->viewport()->rect().center());
        proxy->setPos(center.x()-movie->frameRect().width()/2, center.y()-movie->frameRect().height()/3);
        gif_anim->setAttribute(Qt::WA_NoSystemBackground);

        emit this->Bazooper();
    }

    //qDebug() << Bazooper;
}

