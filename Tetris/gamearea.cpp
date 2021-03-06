#include "gamearea.h"
#include "ui_gamearea.h"
#include <QKeyEvent>
#include <QSound>
#include <QMessageBox>


GameArea::GameArea(QWidget *parent) :
    QFrame(parent)
{
    this->init_gameArea(6,6,320,230,150,300,15,60,0,270);
    this->init_Game();
}

GameArea::GameArea(int speed,QWidget *parent) :
    QFrame(parent)
{
    this->init_gameArea(6,6,320,230,150,300,15,60,0,270);
    this->init_Game();
    this->moveTimer = new QTimer(this);
    connect(this->moveTimer,SIGNAL(timeout()),this,SLOT(moveTimer_upDate()));
    this->moveSpeed = speed;
    this->gameStart();
    this->moveTimer->start(moveSpeed);
}

GameArea::~GameArea()
{

}



void GameArea::moveOneStep()
{
     startY += step;
}

void GameArea::do_MoveNext()
{
    this->currentItem_to_currentMap();
    this->draw_gameArea();
}

void GameArea::nextItem()
{
    copy_Map(currentMap,copyMap,map_row,map_col);
    this->clearRow();
}

bool GameArea::isGame_Over()
{
    if(this->isGameOver) return true;
    else return false;
}

int GameArea::getFullRowNum()
{
    return fullRowNum;
}

void GameArea::setGameAreaColor(QColor color)
{
    gameAreaColor = color;
}

void GameArea::setBoxBrushColor(QColor color)
{
    boxBrushColor = color;
}

void GameArea::setBoxPenColor(QColor color)
{
    boxPenColor = color;
}

void GameArea::set_draw_box_picture(bool Bool)
{
    this->is_draw_box_picture = Bool;
}

void GameArea::setBoxPicture(QString fileName)
{
    this->boxPicture = fileName;
}

void GameArea::setKey_Down_Move_oneStep(bool Bool)
{
    this->isKey_Down_Move_OneStep = Bool;
}

void GameArea::setDrawGrid(bool Bool)
{
    isDrawGrid = Bool;
}

void GameArea::setDrawNextItem(bool Bool)
{
    isDrawNextItem = Bool;
}

void GameArea::setbox(int row,int col)
{
    *(copyMap+row*map_col+col) = 1;
}

void GameArea::setPlaySound_moveLeft(QString fileName,bool Bool)
{
    this->isPlaySound_moveLeft = Bool;
    this->sound_moveLeft = fileName;
}

void GameArea::setPlaySound_moveRight(QString fileName,bool Bool)
{
    this->isPlaySound_moveRight = Bool;
    this->sound_moveRight = fileName;
}

void GameArea::setPlaySound_moveDown(QString fileName,bool Bool)
{
    this->isPlaySound_moveDown = Bool;
    this->sound_moveDown = fileName;
}

void GameArea::setPlaySound_itemChange(QString fileName,bool Bool)
{
    this->isPlaySound_itemChange = Bool;
    this->sound_itemChange = fileName;
}

void GameArea::setPlaySound(bool Bool)
{
    this->isPlaySound_moveLeft = Bool;
    this->isPlaySound_moveRight = Bool;
    this->isPlaySound_moveDown = Bool;
    this->isPlaySound_itemChange = Bool;
}


void GameArea::setRotate(bool Bool)
{
    this->isRotate = Bool;
}

void GameArea::setRotateAngle(int angle)
{
    this->theAngle = angle;
}

void GameArea::setGameAreaPixOrigin(int x,int y)
{
    this->gameArea_X = x;
    this->gameArea_Y = y;
}

void GameArea::setGameAreaPix(int x,int y)
{
    this->pix_X = x;
    this->pix_Y = y;
}




void GameArea::init_gameArea(int X,int Y,int frame_width,int frame_height,int width,int height,int boxStep,int start_x,int start_y,int Angle)

{

    this->gameArea_width = width;
    this->gameArea_height = height;
    this->step = boxStep;
    this->init_startX = start_x;
    this->init_startY = start_y;
    this->map_row = gameArea_height / step;
    this->map_col = gameArea_width /step;

    this->resize(frame_width,frame_height);

    this->move(X,Y);

    pix_gameArea = new QPixmap(this->gameArea_width,this->gameArea_height);

    this->isKey_Down_Move_OneStep =false;


    this->isDrawGrid = true;
    this->isDrawNextItem=true;
    this->isPlaySound_moveLeft = false;
    this->isPlaySound_moveRight = false;
    this->isPlaySound_moveDown = false;
    this->isPlaySound_itemChange =false;
    this->is_draw_box_picture = false;

    this->gameAreaColor = Qt::white;
    this->boxBrushColor = Qt::red;
    this->boxPenColor = Qt::black;

    this->isRotate = true;
    this->theAngle = Angle;
    this->gameArea_X = 100;
    this->gameArea_Y = 200;
    this->pix_X = -30;
    this->pix_Y = -100;
    //this->draw_gameArea();
    myItem = new MyItem();



    this->currentMap = new unsigned char[map_row*map_col];
    this->copyMap = new unsigned char[map_row*map_col];

}

void GameArea::init_Game()
{
    this->init_Map(currentMap,map_row,map_col);
    this->init_Map(copyMap,map_row,map_col);

    this->currentItem = this->currentMap;
    this->theNextItem = this->currentMap;

    isFirstItem = true;
    isGameOver = false;
    fullRowNum = 0;

    this->draw_gameArea();
}

void GameArea::gameStart()
{
    this->startX = this->init_startX;
    this->startY = this->init_startY;
    fullRowNum = 0;
    if(isFirstItem)
    {
        this->currentItem = myItem->getItem();
        isFirstItem = false;
    }
    else this->currentItem = this->theNextItem;
    this->theNextItem = myItem->getItem();
    this->currentItem_to_currentMap();
    this->draw_gameArea();
}

void GameArea::init_Map(unsigned char *initMap,int row,int col)

{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            *(initMap+i*col+j)= 0;
        }
    }
}

void GameArea::draw_gameArea()
{
    this->pix_gameArea->fill(gameAreaColor);
    if(this->isDrawGrid)
    {
        draw_Grid();
    }
    draw_currentMap();
    update();
}

void GameArea::draw_Grid()
{
    QPainter painter(this->pix_gameArea);
    painter.setPen(Qt::DotLine);
    for(int i=0; i<map_row; i++)
    {
        painter.drawLine(QPoint(0,i*step),QPoint(this->gameArea_width,i*step));
    }
    for(int j=0; j<map_col; j++)
    {
        painter.drawLine(QPoint(j*step,0),QPoint(j*step,this->gameArea_height));
    }
}

void GameArea::draw_currentMap()

{
    QPainter painter(this->pix_gameArea);
    painter.setPen(this->boxPenColor);
    painter.setBrush(this->boxBrushColor);
    for(int i=0;i<map_row;i++)
    {
        for(int j=0;j<map_col;j++)
       {
             if(*(currentMap+i*map_col+j)==1)
             {
                 if(this->is_draw_box_picture)
                 {
                    QPixmap pix;
                    pix.load(this->boxPicture);
                    painter.drawPixmap(j*step,i*step,step,step,pix);
                 }

                    painter.drawRect(j*step,i*step,step,step);
             }
        }
    }
}

void GameArea::currentItem_to_currentMap()

{
    copy_Map(copyMap,currentMap,map_row,map_col);
    int m_row = startY/step;
    int m_col = startX/step;
    for(int i=m_row;i<m_row+4;i++)
    {
        for(int j=m_col;j<m_col+4;j++)
        {
            *(currentMap+i*map_col+j)|=*(currentItem+(i-m_row)*4+(j-m_col));
        }
    }
}

void GameArea::copy_Map(unsigned char *theMap,unsigned char *toMap,int row,int col)

{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            *(toMap+i*col+j)= *(theMap+i*col+j);
        }
    }
}

void GameArea::draw_nextItem()
{
    QPainter painter(this);
    painter.translate(0,75);
    painter.rotate(270);
    //VertLabel lab("example");
    //lab.show();
    painter.drawRect(10,10,4*step,4*step);
    painter.setBrush(this->boxBrushColor);
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
        {
            if(*(theNextItem + i*4 +j)==1)
            {
                //painter.rotate(270);
                if(this->is_draw_box_picture)
                 {
                    QPixmap pix;
                    pix.load(this->boxPicture);
                    painter.drawPixmap(10+j*step,10+i*step,step,step,pix);
                 }
                painter.drawRect(10+j*step,10+i*step,step,step);
            }
        }
}

void GameArea::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    if(this->isRotate)
    {
        painter.translate(gameArea_X,gameArea_Y);
        painter.rotate(this->theAngle);
        painter.drawPixmap(QPoint(pix_X,pix_Y),*pix_gameArea);
    }
    else
    {
        painter.drawPixmap(QPoint(pix_X,pix_Y),*pix_gameArea);
    }
    if(this->isDrawNextItem)
    {
        draw_nextItem();
    }
}


void GameArea::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Left :
        {
            startX=startX - step;
            if(isMoveLeft()) startX = startX+step;
            else
            {
                currentItem_to_currentMap();
                this->draw_gameArea();

            }
            break;
        }
        case Qt::Key_Right :    
        {
            startX=startX + step;
            if(isMoveRight()) startX = startX-step;
            else
            {
                currentItem_to_currentMap();
                this->draw_gameArea();

            }

            break;
        }
        case Qt::Key_Up :     
        {
            do_itemChange();
            currentItem_to_currentMap();
            this->draw_gameArea(); 
           break;
        }
        case Qt::Key_Down :
        {
            if(this->isKey_Down_Move_OneStep)
            {
                startY+=step;
                if(this->isMoveEnd()) startY-=step;
                else
                {
                    currentItem_to_currentMap();
                    this->draw_gameArea();
                }
            }
            else
            {
                int i=0;
                while(!this->isMoveEnd())
                {
                    startY +=step;
                    i++;
                }
                startY -=step;
                if(i>0)
                {
                    currentItem_to_currentMap();
                    this->draw_gameArea();

                }
            }

            break;
        }
        default: QFrame::keyPressEvent(event);
    }

}


bool GameArea::isMoveEnd()

{
    int item_endRow = myItem->currentItem_endPos(currentItem,'d');

    int itemRow = startY/step;
    int itemCol = startX/step;


    if(itemRow+item_endRow>=map_row)
    {
        return true;
    }
    for(int i=item_endRow;i>=0;i--)
    {
        for(int j=3;j>=0;j--)
        {
           if(*(copyMap+(itemRow+i)*map_col+itemCol+j) && *(currentItem+i*4+j)==1)
            {

               if(startX==init_startX&&startY<=15) isGameOver = true;


               return true;
            }
        }
    }
    return false;
}

bool GameArea::isMoveLeft()

{
    int item_endLeft = myItem->currentItem_endPos(currentItem,'l');
    int item_Col = startX/step;
    if(item_Col+item_endLeft<0)
        return true;

     if(isMoveEnd())
    {
        return true;
    }
    return false;
}

bool GameArea::isMoveRight()

{
    int item_endRight = myItem->currentItem_endPos(currentItem,'r');
    int item_Col = startX/step;
    if(item_Col+item_endRight >= map_col)
        return true;
    if(isMoveEnd())
    {
        return true;
    }
    return false;
}

void GameArea::do_itemChange()
{
    unsigned char *tempItem;
    tempItem = currentItem;
    currentItem = myItem->itemChange(currentItem);
    if(startX<0) startX = 0;
    if((startX + 4*step)> gameArea_width) startX = gameArea_width - 4*step;

    if(isMoveEnd())

    {
        currentItem = tempItem;
    }
}

void GameArea::clearRow()
{
    int rownum=0;
    int temp1 = 1;
    for(int i=map_row-1;i>0;i--)
    {
        for(int j=0;j<map_col;j++)
        {
            temp1 = *(copyMap+i*map_col+j) && temp1;
        }
        if(temp1)
        {
            //fullRowNum++;
            rownum++;
            for(int row = i;row>0;row--)
            for(int col = 0;col<map_col;col++)
            {
                *(copyMap+row*map_col+col)=*(copyMap+(row-1)*map_col+col);
            }
            i=i+1;
        }
        temp1 =1;
    }
    switch(rownum){
    case 0: break;
    case 1: fullRowNum++;break;
    case 2: fullRowNum+=3;break;
    case 3: fullRowNum+=6;break;
    case 4: fullRowNum+=10;break;

    }
}

void GameArea::playSound(QString fileName)
{
    QSound sound(fileName);
    sound.play();
}

void GameArea::moveTimer_upDate()
{
    this->moveOneStep();
    if(this->isMoveEnd())
    {
        if(this->isGameOver)
        {
            this->moveTimer->stop();
            QMessageBox::warning(this,tr("warning"),tr("Game Over!"),QMessageBox::Yes);
            this->init_Game();
            this->gameStart();
            this->moveTimer->start(moveSpeed);
        }
        else
        {
            this->nextItem();
            this->gameStart();
        }
    }
    else
    {
        this->do_MoveNext();
    }
}
