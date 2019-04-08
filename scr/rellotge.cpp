/*
 * Copyright (C) 2000-2016  Innocent De Marchi
 * email: tangram.peces@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <QtGui>
#include <QTimer>
 #include <math.h>

#include "rellotge.h"

Rellotge::Rellotge(QWidget * parent , Qt::WindowFlags)
       : QLabel(parent)
{
    rellotgeQTimer = new QTimer();
    connect(rellotgeQTimer, SIGNAL(timeout()), this, SLOT(actualitzaRellotge()));
    p_rellotgeEnMarxa=false;
    hores=0;
    minuts=0;
    segons=0;
}

void Rellotge::actualitzaRellotge(){
    segons=segons+1;
    if (segons>=60){
        segons=segons-60;
        minuts=minuts+1;
    }
    if (minuts>=60){
        minuts=minuts-60;
        hores=hores+1;
    }
    setText(retornaTemps());
}

void Rellotge::iniciaRellotge(){
  rellotgeQTimer->start(1000);
  p_rellotgeEnMarxa=true;

}

void Rellotge::aturaRellotge(){
  rellotgeQTimer->stop();
  setText(retornaTemps());
  p_rellotgeEnMarxa=false;
  update();
}

void Rellotge::estableixTemps(QString temps){
   QStringList list1 = temps.split(":");
   hores=list1.at(0).toInt();
   minuts=list1.at(1).toInt();
   segons=list1.at(2).toInt();
   setText(retornaTemps());
   update();
}

QString Rellotge::retornaTemps(int tipus){

    QString sHores,sMinuts,sSegons;

    if (hores<10){
      sHores= QString("0%1:").arg(hores);
    }
    else sHores= QString("%1:").arg(hores);

    if (minuts<10){
      sMinuts= QString("0%1:").arg(minuts);
    }
    else sMinuts= QString("%1:").arg(minuts);

    if (segons<10){
      sSegons= QString("0%1").arg(segons);
    }
    else sSegons= QString("%1").arg(segons);

if (tipus==0){
    if(rellotgeQTimer->isActive()){
       return QString("<h2><font color=red>%1%2%3</font></h2>").arg(sHores,sMinuts,sSegons);
    }
    else {
        return QString("<h2><font color=blue>%1%2%3</font></h2>").arg(sHores,sMinuts,sSegons);
    }
}
else  {
    return QString("%1%2%3").arg(sHores,sMinuts,sSegons);
  }
}

bool Rellotge::rellotgeEnMarxa(){
    return p_rellotgeEnMarxa;
}
