#include "map.h"
#include <QDebug>

map::map(QWidget *parent, int level):
    stepNum(0),
    grid(45),
    dotboxNum(0),
    dotRboxNum(0)
{
//    level = 6;
    // Read map File
    QString mapFilename = ":/map/map" + QString::number(level);
    QFile mFile(mapFilename);
    if(!mFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Could not open the file: " << mapFilename;
        return;
    }
    QTextStream in(&mFile);
    in >> mapX >> mapY >> orgX >> orgY; in.readLine();  // Skip "\n"
    orgX *= grid;   orgY *= grid;
    qDebug() << "Map size = " << mapX << mapY;
    ground.resize(4);   wall.resize(4);
    dotbox.resize(4);   box.resize(4);

    dotRbox.resize(4);  Rbox.resize(4);
    hole.resize(4);     Hbox.resize(4);
    int px = 0, py = 0;
    char c;
    QVector<int> order = {0,1}; // create object order
    // Generate Layout of map
    for(auto ord=order.begin(); ord < order.end(); ord++) {
        for(int j=0; j < mapY; j++) {
            for(int i=0; i < mapX; i++) {
                in >> c;
                if(*ord == 0 && c == 'P') {  // Player
                        px = i*grid + orgX;
                        py = j*grid + orgY;
                }
                if(*ord == 0 && (c != '1' && c != 'N'))
                    createObject(parent, i, j, ":/img/ground1", ground);
                if(c == '1' && *ord == 0) {
                    createObject(parent, i, j, ":/img/ground", ground);
                    createObject(parent, i, j, ":/img/wall", wall);
                }
                if(*ord == 0 && (c == '2' || c == '8')){  // box Lcn.
                    createObject(parent, i, j, ":/img/dotbox", dotbox);
                    dotboxNum++;
                }
                if(*ord == 0 && c == '4') {
                    createObject(parent, i, j, ":/img/dotRbox", dotRbox);
                    dotRboxNum++;
                }
                if(*ord == 0 && (c-'0' == 6 || c-'0' == 7)) { // c = 6,7
                    createObject(parent, i, j, ":/img/hole"+QString::number(c-'5'), hole, c-'5');
                }
                if(*ord == 1 && (c == '3' || c == '8')) {
                    if(c == '3')
                        createObject(parent, i, j, ":/img/box", box);
                    else
                        createObject(parent, i, j, ":/img/box2", box);
                }
                if(*ord == 1 && c == '5')
                    createObject(parent, i, j, ":/img/Rbox", Rbox);
                if(*ord == 1 && c == '9')
                    createObject(parent, i, j, ":/img/Hbox", Hbox);
            }
            in >> c;
        }
        in.seek(0);     // cursor back to the beginning of file
        in.readLine();  // skip map info
    }
    mFile.close();
    ply = new player(parent, grid, px, py);
}

void map::createObject(QWidget *parent, int gx, int gy, QString file, QVector<QVector<QLabel*>> &obj, int status)
{
    QLabel *lb = new QLabel(parent);
    lb->setGeometry(orgX + gx*grid, orgY + gy*grid, grid, grid);
//    lb->setParent(parent);
    setObjPic(lb, file);
    lb->setLineWidth(status);
    obj[grid2area(gx, gy)].append(lb);
//    lb->setFrameStyle(QFrame::Box);
    //    lb->setText(QString::number(grid2area(gx, gy)));
}

void map::setObjPic(QLabel *obj, QString file)
{
    QPixmap pix(file);
    obj->setPixmap(pix.scaled(obj->width(), obj->height(), Qt::IgnoreAspectRatio));
    obj->show();
}

void map::gaming(QKeyEvent *e)
{
    int dx = ply->getX(), dy = ply->getY(), area;
    dxdy(e, dx, dy, area);

    // 1. Check ply & wall
    if(collWall(dx, dy))
        return;

    // 2. Check ply & box
    if(collBox(e, dx, dy))
        return;

    // S1. Check ply & Rbox
    if(collRBox(e, dx, dy))
        return;

    // S2. Check ply & Hbox
    if(collHbox(e, dx, dy))
        return;

    // S3. Check ply & hole
    if(collhole(dx, dy)) {
        ply->move(e);
        ply->setplayerIcon(1);
        emit gameOver();
        return;
    }
    stepNum++;
    ply->move(e);
    return;
}

bool map::collWall(int dx, int dy)
{
    int area = coord2area(dx, dy);
    for(auto w=wall[area].begin(); w < wall[area].end(); w++) {
        if(dx == (*w)->x() && dy == (*w)->y()) {
//            qDebug() << "wx = " << (*w)->x() << ", wy = " << (*w)->y();
            return true;
        }
    }
    return false;
}

bool map::collBox(QKeyEvent *e, int dx, int dy)
{
    int area = coord2area(dx, dy);
    for(auto b=box[area].begin(); b < box[area].end(); b++)
        if(dx == (*b)->x() && dy == (*b)->y()) {
            // check box destination is empty?
            int bdx = dx, bdy = dy, b_area = 0;
            dxdy(e, bdx, bdy, b_area);
            if(collWall(bdx, bdy))
                return true;
            // No wall
            for(auto b=box[b_area].begin(); b < box[b_area].end(); b++)
                if(bdx == (*b)->x() && bdy == (*b)->y())
                    return true;
            // No box
            for(auto rb=Rbox[b_area].begin(); rb < Rbox[b_area].end(); rb++)
                if(bdx == (*rb)->x() && bdy == (*rb)->y())
                    return true;
            // No Rbox
            for(auto hb=Hbox[b_area].begin(); hb < Hbox[b_area].end(); hb++)
                if(bdx == (*hb)->x() && bdy == (*hb)->y())
                    return true;
            // No Hbox
            for(auto h=hole[b_area].begin(); h < hole[b_area].end(); h++)
                if(bdx == (*h)->x() && bdy == (*h)->y() && (*h)->lineWidth() != 0) {
                    (*b)->hide();
                    if((*h)->lineWidth() == 2) {
                        setObjPic(*h, ":/img/box3");
                    }
                    else if((*h)->lineWidth() == 1) {
                        setObjPic(*h, ":/img/box4");
                    }
                    emit gameOver();
                    return false;
                }
            // No hole & boxes & wall @ destination
            moveBox(e, b);
            return false;   // Stop searching for next!!
        }
    return false;   // No box
}

bool map::collRBox(QKeyEvent *e, int dx, int dy)
{
    int area = coord2area(dx, dy);
    for(auto rb=Rbox[area].begin(); rb < Rbox[area].end(); rb++)
        if(dx == (*rb)->x() && dy == (*rb)->y()) {
            // check Rbox destination is empty?
            int rbdx = dx, rbdy = dy, rb_area = 0;
            dxdy(e, rbdx, rbdy, rb_area);
            if(collWall(rbdx, rbdy))
                return true;
            // No wall
            for(auto b=box[rb_area].begin(); b < box[rb_area].end(); b++)
                if(rbdx == (*b)->x() && rbdy == (*b)->y())
                    return true;
            // No box
            for(auto rb=Rbox[rb_area].begin(); rb < Rbox[rb_area].end(); rb++)
                if(rbdx == (*rb)->x() && rbdy == (*rb)->y())
                    return true;
            // No Rbox
            for(auto hb=Hbox[rb_area].begin(); hb < Hbox[rb_area].end(); hb++)
                if(rbdx == (*hb)->x() && rbdy == (*hb)->y())
                    return true;
            // No Hbox
            for(auto h=hole[rb_area].begin(); h < hole[rb_area].end(); h++)
                if(rbdx == (*h)->x() && rbdy == (*h)->y() && (*h)->lineWidth() != 0) {
                    (*rb)->hide();
                    if((*h)->lineWidth() == 2) {
                        setObjPic(*h, ":/img/Rbox3");
                    }
                    else if((*h)->lineWidth() == 1) {
                        setObjPic(*h, ":/img/Rbox4");
                    }
                    emit gameOver();
                    return false;
                }
            // No hole & boxes & wall @ destination
            moveRBox(e, rb);
            return false;   // Stop searching for next!!
        }
    return false;   // No Rbox
}

bool map::collHbox(QKeyEvent *e, int dx, int dy)
{
    int area = coord2area(dx, dy);
    for(auto hb=Hbox[area].begin(); hb < Hbox[area].end(); hb++)
        if(dx == (*hb)->x() && dy == (*hb)->y()) {
            // check Hbox destination is empty?
            int bdx = dx, bdy = dy, b_area = 0;
            dxdy(e, bdx, bdy, b_area);
            if(collWall(bdx, bdy))
                return true;
            // No wall
            for(auto b=box[b_area].begin(); b < box[b_area].end(); b++)
                if(bdx == (*b)->x() && bdy == (*b)->y())
                    return true;
            // No box
            for(auto rb=Rbox[b_area].begin(); rb < Rbox[b_area].end(); rb++)
                if(bdx == (*rb)->x() && bdy == (*rb)->y())
                    return true;
            // No Rbox
            for(auto hhb=Hbox[b_area].begin(); hhb < Hbox[b_area].end(); hhb++)
                if(bdx == (*hhb)->x() && bdy == (*hhb)->y())
                    return true;
            // No HHbox
            bool ishole = false;
            for(auto h=hole[b_area].begin(); h < hole[b_area].end(); h++)
                if(bdx == (*h)->x() && bdy == (*h)->y()) {
                    int status = (*h)->lineWidth();
                    if(status > 0) {
                        setObjPic(*h, ":/img/hole" + QString::number(status-1));
                        (*h)->setLineWidth(status-1);
                        qDebug() << ":/img/hole"+QString::number(status-1);
                        delete *hb;
                        Hbox[area].erase(hb);
                        qDebug() << "delete *hb;";
                    }
                    if(status == 1 || status == 0) {
                        ground[b_area].push_back(*h);
                        hole[b_area].erase(h);
                    }
                    ishole = true;
                    break;
                }
            if(!ishole)
                 moveHBox(e, hb);   // No hole & boxes & wall @ destination
            return false;       // Stop searching for next!!
        }
    return false;   // No box
}

bool map::collhole(int dx, int dy)
{
    int area = coord2area(dx, dy);
    for(auto h=hole[area].begin(); h < hole[area].end(); h++)
        if(dx == (*h)->x() && dy == (*h)->y())
            return true;
    return false;
}

void map::moveBox(QKeyEvent *e, QVector<QLabel*>::iterator b)
{
    int bx = (*b)->x(), by = (*b)->y(), area = coord2area(bx, by);
    int nbx = bx, nby = by, narea = 0;
    dxdy(e, nbx, nby, narea);
    (*b)->setGeometry(nbx,nby,grid,grid);
    setObjPic(*b, isdotBox(b, dotbox)? ":/img/box2":":/img/box");
    // Update area for moved box
    if(narea != area) {
        box[narea].push_back(*b);
        box[area].erase(b);
    }
}

void map::moveRBox(QKeyEvent *e, QVector<QLabel*>::iterator b)
{
    int bx = (*b)->x(), by = (*b)->y(), area = coord2area(bx, by);
    int nbx = bx, nby = by, narea = 0;
    dxdy(e, nbx, nby, narea);
    (*b)->setGeometry(nbx,nby,grid,grid);
    bool isfix = isdotBox(b, dotRbox);
    setObjPic(*b, isfix? ":/img/Rbox2":":/img/Rbox");
    // Update area for moved Rbox and fixed >> wall
    if(isfix) {
        wall[narea].push_back(*b);
        dotRboxNum--;
    }
    else if(area == narea) {
        return;
    }
    else
        Rbox[narea].push_back(*b);
    Rbox[area].erase(b);
}

void map::moveHBox(QKeyEvent *e, QVector<QLabel*>::iterator hb)
{
    int bx = (*hb)->x(), by = (*hb)->y(), area = coord2area(bx, by);
    int nbx = bx, nby = by, narea = 0;
    dxdy(e, nbx, nby, narea);
    (*hb)->setGeometry(nbx,nby,grid,grid);
    (*hb)->show();
    // Update area for moved Hbox
    if(narea != area) {
        Hbox[narea].push_back(*hb);
        Hbox[area].erase(hb);
    }
}

bool map::isdotBox(QVector<QLabel*>::iterator b, const QVector<QVector<QLabel*>> &DOTBOX)
{
    int bx = (*b)->x(), by = (*b)->y(), area = coord2area(bx, by);
    for(auto d=DOTBOX[area].begin(); d < DOTBOX[area].end(); d++)
        if(bx == (*d)->x() && by == (*d)->y())
            return true;
    return false;
}

bool map::checkPass()
{
    int passNum = 0;
    for(int i=0; i < 4; i++)
        for(auto d=dotbox[i].begin(); d < dotbox[i].end(); d++)
            for(auto b=box[i].begin(); b < box[i].end(); b++)
                if((*d)->x() == (*b)->x() && (*d)->y() == (*b)->y()) {
                    passNum++;
                    break;
                }
    return passNum == dotboxNum && dotRboxNum == 0;
}

void map::gameover()
{

}

int map::coord2area(int x, int y)
{
    return ((x-orgX)/grid)/qCeil(mapX/2.0) + ((y-orgY)/grid)/qCeil(mapY/2.0)*2;
}

int map::grid2area(int gx, int gy)
{
    return gx/qCeil(mapX/2.0) + gy/qCeil(mapY/2.0)*2;
}

// According KeyEvent to move "dx", "dy", and modify "area"
void map::dxdy(QKeyEvent *e, int &dx, int &dy, int &area)
{
    if(e->key() == Qt::Key_Up)
        dy -= grid;
    else if(e->key() == Qt::Key_Down)
        dy += grid;
    else if(e->key() == Qt::Key_Left)
        dx -= grid;
    else if(e->key() == Qt::Key_Right)
        dx += grid;
    area = coord2area(dx, dy);
}

 // Called when object deleted: changing level, restart
 map::~map() {
     // 1. player *ply
     delete ply;
     qDebug() << "player clear";

     // 2. QVector<QVector<QLabel*>> wall, auto = QVector<QLabel*>::iterator
     for(int i=0; i < 4; i++)
         for(auto w=wall[i].begin(); w < wall[i].end(); w++)
             delete *w;
     wall.clear();
     qDebug() << "wall clear";

     // 3. QVector<QVector<QLabel*>> box
     for(int i=0; i < 4; i++)
         for(auto b=box[i].begin(); b < box[i].end(); b++)
             delete *b;
      qDebug() << "box clear";

     // 4. another items...
      for(int i=0; i < 4; i++)
          for(auto db=dotbox[i].begin(); db < dotbox[i].end(); db++)
              delete *db;
     // 5. ground
      for(int i=0; i < 4; i++)
          for(auto g=ground[i].begin(); g < ground[i].end(); g++)
              delete *g;
 }
