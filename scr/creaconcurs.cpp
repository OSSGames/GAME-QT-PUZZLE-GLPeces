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
#include <QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include "creaconcurs.h"


#include "conjuntpeces.h"
#include "constants.h"
#include "configuracio.h"
#include "funcions.h"


CreaConcurs::CreaConcurs(Configuracio *configJoc, QWidget *parent )
        : QDialog(parent)
{

configuracioJoc=configJoc;
charPathSeparator=QDir().separator();

setMinimumSize (QSize(800,550));
setMaximumSize(QSize(800,550));
setWindowTitle(configuracioJoc->tradueixCadena("Crea.concurs"));
//setWindowIcon(QIcon(":/images/pecesset.png"));
//Això no serveix de res
//setWindowIcon(QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"pecesset.png"));

//arbre de figures
    arbreFiguresTreeWidget= new QTreeWidget(this);
    arbreFiguresTreeWidget->setGeometry(0,0,300,550);
    arbreFiguresTreeWidget->move(QPoint(0,0));
    arbreFiguresTreeWidget->expandAll();
    connect(arbreFiguresTreeWidget,SIGNAL(itemSelectionChanged()),
            this,SLOT(seleccioFiguraJoc()));


//arbre de concurs
    arbreConcursTreeWidget= new QTreeWidget(this);
    arbreConcursTreeWidget->setGeometry(0,0,300,550);
    arbreConcursTreeWidget->move(QPoint(500,0));
    arbreConcursTreeWidget->expandAll();
    connect(arbreConcursTreeWidget,SIGNAL(itemSelectionChanged()),
            this,SLOT(seleccioFiguraConcurs()));

//frame dels botons
    botonsFrame=new QFrame(this);
    botonsFrame->setGeometry(0,0,200,550);
    botonsFrame->move(QPoint(300,0));

    sceneMostra=new QGraphicsScene(0, 0, 180, 180);
    viewMostra = new QGraphicsView(sceneMostra,botonsFrame);
    viewMostra->setRenderHints(QPainter::Antialiasing
                             | QPainter::TextAntialiasing);
    viewMostra->move(QPoint(10,0));

//botons
     QPushButton *afegeixFigura = new QPushButton(">>", botonsFrame);
     afegeixFigura->setToolTip(configuracioJoc->tradueixCadena("Concurs.ToolTip.Afageix"));
     connect(afegeixFigura,SIGNAL(clicked()),this,SLOT(afegeixFiguraSlot()));
     afegeixFigura->setGeometry(0,0,40,40);
     afegeixFigura->move(80,200);

     QPushButton *eliminaFigura = new QPushButton("<<", botonsFrame);
     eliminaFigura->setToolTip(configuracioJoc->tradueixCadena("Concurs.ToolTip.Elimina"));
     connect(eliminaFigura,SIGNAL(clicked()),this,SLOT(eliminaFiguraSlot()));
     eliminaFigura->setGeometry(0,0,40,40);
     eliminaFigura->move(80,240);

     QPushButton *eliminaTotesFigures = new QPushButton(
             QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"cross.png"),"", botonsFrame);
     eliminaTotesFigures->setToolTip(configuracioJoc->tradueixCadena("Concurs.ToolTip.Elimina.Totes"));
     connect(eliminaTotesFigures,SIGNAL(clicked()),this,SLOT(eliminaTotesFiguresSlot()));
     eliminaTotesFigures->setGeometry(0,0,40,40);
     eliminaTotesFigures->move(80,280);

     QPushButton *concursAtzar = new QPushButton(
             QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"conatzar.png"),"", botonsFrame);
     concursAtzar->setToolTip(configuracioJoc->tradueixCadena("Concurs.ToolTip.Atzar"));
     connect(concursAtzar,SIGNAL(clicked()),this,SLOT(concursAtzarSlot()));
     concursAtzar->setGeometry(0,0,40,40);
     concursAtzar->move(80,320);

     QPushButton *obreConcurs = new QPushButton(
             QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"obrecon.png"),"", botonsFrame);
     obreConcurs->setToolTip(configuracioJoc->tradueixCadena("Concurs.ToolTip.Carrega"));
     connect(obreConcurs,SIGNAL(clicked()),this,SLOT(obreConcursSlot()));
     obreConcurs->setGeometry(0,0,40,40);
     obreConcurs->move(80,360);

     QPushButton *desaConcurs = new QPushButton(
             QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"save.png"),"", botonsFrame);
     desaConcurs->setToolTip(configuracioJoc->tradueixCadena("Concurs.ToolTip.Desa"));
     connect(desaConcurs,SIGNAL(clicked()),this,SLOT(desaConcursSlot()));
     desaConcurs->setGeometry(0,0,40,40);
     desaConcurs->move(80,400);

     QPushButton *tanca = new QPushButton(
             QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"surt.png"),"", botonsFrame);
     tanca->setToolTip(configuracioJoc->tradueixCadena("Tanca"));
     connect(tanca,SIGNAL(clicked()),this,SLOT(close()));
     tanca->setGeometry(0,0,40,40);
     tanca->move(80,440);
     tanca->setDefault(true);

tradueixGUI();

//carregam els arbres
  carregaArbreFigures( arbreFiguresTreeWidget,configuracioJoc->directoriArxiuDadesFigures);
  carregaArbreFigures( arbreConcursTreeWidget,"");

creaTangram(configuracioJoc->tipusTangram);

}

//Carrega les figures als arbres
void CreaConcurs::carregaArbreFigures(QTreeWidget *arbre, QString nomArxiu){

QApplication::setOverrideCursor(Qt::WaitCursor);

//netejam
arbre->clear();
arbre->setColumnCount(4);
arbre->setColumnHidden(3,true);
arbre->setColumnHidden(4,true);
//posam la capçalera de l'arbre
QTreeWidgetItem *item=new QTreeWidgetItem();
// 13/02/11 Canviat per eliminar icones de resources.qrc
item->setIcon(0,QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+configuracioJoc->nomIcone));
item->setText(0,configuracioJoc->tradueixCadena(configuracioJoc->nomJocArbreFigures));
arbre->setHeaderItem(item);

llegeixArxiuFigures(arbre,nomArxiu);

QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void CreaConcurs::llegeixArxiuFigures(QTreeWidget *arbre, QString nomArxiu){
  QTreeWidgetItem *arrel=new QTreeWidgetItem();
  //obrim l'arxiu de figures
  QFile arxiuFigures(nomArxiu);
  if(!nomArxiu.isEmpty()){
  arxiuFigures.open(QIODevice::ReadOnly | QIODevice::Text);
   /* if (!arxiuFigures.open(QIODevice::ReadOnly | QIODevice::Text)){
         return ;
    }
*/
  QSettings *controlArxiu;
  controlArxiu=new QSettings(QSettings::IniFormat, QSettings::UserScope,"De Marchi", "NomFigures");
  controlArxiu->clear();

     QTextStream entrada(&arxiuFigures);
     QString linia0 = entrada.readLine();
     QString linia1,linia2;
     bool figuraComposta;

     //QTreeWidgetItem *arrel=new QTreeWidgetItem();
     int numFig, intPosDarrerPunt;
     while (!linia0.isNull()) {
         linia1=entrada.readLine();
         linia2=entrada.readLine();
            //Controla si cal fer visible la figura al nivell expert
            //(a les figures "compostes")
            if (linia2.length()==5){
               if (linia2.mid(4,1)=="1"){
                   figuraComposta=true;}
               else figuraComposta=false;
           }
            else figuraComposta=false;

         if ( ( (linia2.mid(0,1).toInt()<=configuracioJoc->nivellJoc) ||
              configuracioJoc->nivellJoc== NIVELL_INICIAT ) &&
              //les figures "compostes" no son visibles al nivell expert
              !(figuraComposta && configuracioJoc->nivellJoc==NIVELL_EXPERT)  ) {
            QTreeWidgetItem *item=new QTreeWidgetItem();
            intPosDarrerPunt=linia0.lastIndexOf(".");
            if (intPosDarrerPunt<=0){intPosDarrerPunt=linia0.length();}
            numFig=controlArxiu->value(linia0.mid(0,intPosDarrerPunt),"1").toInt();
          QString cadena;
          //Només cal fer la traducció dels noms de figura, quan es carrega
          //un arxiu de figures (no de concurs)
          if(arxiuFigures.fileName().mid(arxiuFigures.fileName().lastIndexOf("."),
                 arxiuFigures.fileName().length())==".fig"){
            //posam espais al principi per a què les cadenes tenguin
            //la mateixa longitud i quedin ben ordenades
            cadena=QString::number(numFig);
            while (cadena.length()<4){cadena=" "+cadena;}
            cadena=configuracioJoc->tradueixCadena(linia0.mid(0,intPosDarrerPunt))+cadena;
             }
          else cadena=linia0;
            item->setText(0,cadena);
            numFig++;
            controlArxiu->setValue(linia0.mid(0,intPosDarrerPunt),numFig);
            item->setText(1,linia1);
            item->setText(2,linia2);
            //controla si  la figura ja s'ha resolt
            item->setText(3,"0");
            item->setIcon(0,iconeFigura(linia2.mid(2,1).toInt()));

           arrel->addChild(item);
         }
         linia0 = entrada.readLine();
     } 
controlArxiu->clear();
 }//Final del If de comprovació del nom de l'arxiu
arbre->addTopLevelItem(arrel);
arbre->sortItems(0,Qt::AscendingOrder);
//arbreFiguresTreeWidget->sortByColumn(0,Qt::AscendingOrder);
arrel->setText(0,configuracioJoc->tradueixCadena(
                   /* 08/06/14 Eliminat .toAscii-> deixa d'existir
                    * a Qt 5.0*/
                                  configuracioJoc->nomNivellArbreFigures) //.toAscii())
                                  +" ("+QString::number(arrel->childCount()) +")");
arbre->expandAll();

}


/*
Gestiona la selecció d'una figura a l'arbre
(conectat amb arbreFiguresTreeWidget::itemSelectionChanged ()
*/
void CreaConcurs::seleccioFiguraJoc(){
    seleccioFigura(arbreFiguresTreeWidget);
}
void CreaConcurs::seleccioFiguraConcurs(){
    seleccioFigura(arbreConcursTreeWidget);
    conjuntMostra->colorPecesConjunt(Qt::green);
}

void CreaConcurs::seleccioFigura(QTreeWidget *arbre){

if (!arbre->selectedItems().isEmpty()){
 QTreeWidgetItem *item;
 item=arbre->selectedItems().first();
 //S'ha clicat el nus arrel
 if (item->childCount()>0){
    conjuntMostra->conjuntVisible(false);
     }
 else if (!item->text(1).isEmpty()){
    conjuntMostra->colocaPeces(item->text(1));
    conjuntMostra->conjuntVisible(true);
    conjuntMostra->conjuntDeMostra();
  }
else {
    conjuntMostra->conjuntVisible(false);
   }
 }
}

//botons
void CreaConcurs::afegeixFiguraSlot(){
 QApplication::setOverrideCursor(Qt::WaitCursor);
 if( arbreFiguresTreeWidget->selectedItems().count()>0){
 QTreeWidgetItem *item=arbreFiguresTreeWidget->selectedItems().first();

 arbreFiguresTreeWidget->topLevelItem(0)->removeChild(item);
 arbreConcursTreeWidget->topLevelItem(0)->addChild(item);
 arbreConcursTreeWidget->sortItems(0,Qt::AscendingOrder);
 actualitzaTextItemArrel();
// arbreConcursTreeWidget->sortByColumn(0);
 }
 QApplication::setOverrideCursor(Qt::ArrowCursor);
}


void CreaConcurs::eliminaFiguraSlot(){
 QApplication::setOverrideCursor(Qt::WaitCursor);

if( arbreConcursTreeWidget->selectedItems().count()>0){
 QTreeWidgetItem *item=arbreConcursTreeWidget->selectedItems().first();
 arbreConcursTreeWidget->topLevelItem(0)->removeChild(item);
 arbreFiguresTreeWidget->topLevelItem(0)->addChild(item);
 //això enlenteix molt!!!
 arbreFiguresTreeWidget->sortItems(0,Qt::AscendingOrder);
// arbreFiguresTreeWidget->sortByColumn(0);
 actualitzaTextItemArrel();
 }

 QApplication::setOverrideCursor(Qt::ArrowCursor);
}


void CreaConcurs::eliminaTotesFiguresSlot(){
QApplication::setOverrideCursor(Qt::WaitCursor);
   while (arbreConcursTreeWidget->topLevelItem(0)->childCount()>0  ){
       QTreeWidgetItem *item=arbreConcursTreeWidget->topLevelItem(0)->child(0);
       arbreConcursTreeWidget->setCurrentItem(item);
       arbreConcursTreeWidget->topLevelItem(0)->removeChild(item);
       arbreFiguresTreeWidget->topLevelItem(0)->addChild(item);
   }

 arbreFiguresTreeWidget->sortItems(0,Qt::AscendingOrder);
// arbreFiguresTreeWidget->sortByColumn(0);
 actualitzaTextItemArrel();
 QApplication::setOverrideCursor(Qt::ArrowCursor);
}


void CreaConcurs::actualitzaTextItemArrel(){
arbreConcursTreeWidget->topLevelItem(0)->setText(0,configuracioJoc->tradueixCadena(
                                 /* 08/06/14 Eliminat .toAscii-> deixa d'existir
                                  * a Qt 5.0*/
                                  configuracioJoc->nomNivellArbreFigures) //.toAscii())
                                  +" ("+QString::number(arbreConcursTreeWidget->topLevelItem(0)->childCount()) +")");

arbreFiguresTreeWidget->topLevelItem(0)->setText(0,configuracioJoc->tradueixCadena(
                                  configuracioJoc->nomNivellArbreFigures) //.toAscii())
                                  +" ("+QString::number(arbreFiguresTreeWidget->topLevelItem(0)->childCount()) +")");
}


void CreaConcurs::concursAtzarSlot(){
bool val;
/* 27/11/11
  Canviat degut a un canvi a Qt
  */
/*int nombreFigures = QInputDialog::getInteger(this, configuracioJoc->tradueixCadena("nombre.Figures"),
                                configuracioJoc->tradueixCadena("Nombre.figures.seleccionar"),
                                qMin(25,arbreFiguresTreeWidget->topLevelItem(0)->childCount()),
                                1,arbreFiguresTreeWidget->topLevelItem(0)->childCount(), 1, &val);*/
int nombreFigures = QInputDialog::getInt(this, configuracioJoc->tradueixCadena("nombre.Figures"),
                                configuracioJoc->tradueixCadena("Nombre.figures.seleccionar"),
                                qMin(25,arbreFiguresTreeWidget->topLevelItem(0)->childCount()),
                                1,arbreFiguresTreeWidget->topLevelItem(0)->childCount(), 1, &val);

if (val){
  if(arbreFiguresTreeWidget->topLevelItem(0)->childCount()>0){
   for(int j=0;j<=nombreFigures-1;j++){
   int nus=qrand() % arbreFiguresTreeWidget->topLevelItem(0)->childCount();
   QTreeWidgetItem *itemSel=arbreFiguresTreeWidget->topLevelItem(0)->child(nus);
   arbreFiguresTreeWidget->setCurrentItem(itemSel);
   afegeixFiguraSlot();
    }
   }
  }
}


void CreaConcurs::obreConcursSlot() {
QString directoriSeleccionat = QFileDialog::getOpenFileName(this,
                                 configuracioJoc->tradueixCadena("Carrega.concurs"),
                                 directoriFiguresPersonals(),
                                 filtreArxiusConcurs(configuracioJoc) );
    if(!directoriSeleccionat.isEmpty()){
        carregaArbreFigures(arbreConcursTreeWidget,directoriSeleccionat);
           //Ara cal eliminar les figures carregades de l'arbre de figures
           if(arbreConcursTreeWidget->topLevelItem(0)->childCount()>0){
               QString nomFigura;
                int elimina;
               for(int j=0;j<=arbreConcursTreeWidget->topLevelItem(0)->childCount()-1;j++){
                   nomFigura=arbreConcursTreeWidget->topLevelItem(0)->child(j)->text(0);
                   elimina=-1;
                    for(int i=0;i<=arbreFiguresTreeWidget->topLevelItem(0)->childCount()-1;i++){
                        if(arbreFiguresTreeWidget->topLevelItem(0)->child(i)->text(0)==nomFigura){
                           elimina=i;
                        }
                    }
                    if(elimina>-1){
                        QTreeWidgetItem *item=arbreFiguresTreeWidget->topLevelItem(0)->child(elimina);
                      arbreFiguresTreeWidget->topLevelItem(0)->removeChild(item);
                    }
               }
    }
 actualitzaTextItemArrel();
}
}

void CreaConcurs::desaConcursSlot() {
QString directoriSeleccionat = directoriFiguresPersonals();
QString nomArxiu = QFileDialog::getSaveFileName(this,
                   configuracioJoc->tradueixCadena("Concurs.ToolTip.Desa"),
                   directoriSeleccionat,
                   filtreArxiusConcurs(configuracioJoc) );
    if (!nomArxiu.isEmpty()){
        if(nomArxiu.indexOf(extensioArxiuConcurs(configuracioJoc))==-1){
           nomArxiu=nomArxiu.append(extensioArxiuConcurs(configuracioJoc));
         }

      QFile arxiu(nomArxiu);
      if (!arxiu.open(QIODevice::WriteOnly)){
          QMessageBox::information(this, configuracioJoc->tradueixCadena("desa.dades.figura"),
                                 configuracioJoc->tradueixCadena("concurs.error")+"\n"+
                                 configuracioJoc->tradueixCadena("arxiuNoCreat").arg(arxiu.fileName()) );
         }
      else {
         QTextStream entrada(&arxiu);
         for(int j=0;j<=arbreConcursTreeWidget->topLevelItem(0)->childCount()-1;j++){
            QTreeWidgetItem *item;
            item=arbreConcursTreeWidget->topLevelItem(0)->child(j);
            entrada<<(QString("%1\n").arg(item->text(0)));
            entrada<<(QString("%1\n").arg(item->text(1)));
            entrada<<(QString("%1\n").arg(item->text(2)));
         }
      arxiu.close();
      }
    }

    }
void CreaConcurs::tancaSlot() {
    close();
    }


void CreaConcurs::creaTangram(int tipusTangram){

        //eliminam les peces actuals
        sceneMostra->clear();
        conjuntMostra=new ConjuntPeces(tipusTangram,ESCALA_MOSTRA);
        conjuntMostra->conjuntDeMostra();
        conjuntMostra->conjuntVisible(false);
       //ara afegim les peces a l'scena
        for(int i=0;i<=conjuntMostra->arrayPeces.size()-1;i++){
            conjuntMostra->arrayPeces[i]->setZValue(0);
            sceneMostra->addItem(conjuntMostra->arrayPeces[i]);
            }
}//final CreaConcurs::creaTangram(int tipusTangram)


void CreaConcurs::tradueixGUI(){
setWindowTitle(configuracioJoc->tradueixCadena("Crea.concurs"));
/*
  Si la traducció de totes les cadenes es posa aqui
  el programa no funciona
 */

}
