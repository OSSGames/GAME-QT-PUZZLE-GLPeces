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
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QStyleOptionGraphicsItem>
#include <QtWidgets/QStyle>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>






#include "constants.h"
#include "funcions.h"
#include "conjuntpeces.h"
#include "peca.h"

double distanciaAferraPeca2=50;

/*
	Constructor per defecte
	Crea el quadrat del tangram clàssic
*/
Peca::Peca()
{
	colorsAtzarPeca();	
	setEscala(ESCALA_GENERAL);
        creaPeca(1);
        //creaPeca(T7_QUADRAT);
}
/* 
Construeix la Peça segons el paràmetre passat
*/
Peca::Peca(int tipusDePeca){
	colorsAtzarPeca();
	setEscala(ESCALA_GENERAL);
	creaPeca(tipusDePeca);
}

/* Eliminat 26/02/11
Peca::Peca(int tipusDePeca,double pescala){
        colorsAtzarPeca();
        setEscala(pescala);
        creaPeca(tipusDePeca);
}*/

//Crea la peça
Peca::Peca(int tipusDePeca,double pescala){
	setEscala(pescala);
        colorsAtzarPeca();
        creaPeca(tipusDePeca);
}

//Constructor per les Peces que formen una figura
Peca::Peca(int tipusPeca,float posicio_X,float posicio_Y,float angle,
            int psimetria,double pescala ){
   setEscala(pescala);
   creaPeca(tipusPeca);
   setPos(posicio_X,posicio_Y);
   setAngleRotacio(angle);
   if(psimetria){
       simetriaPeca();
   }
}

/* 15/10/11
   Aquest procediment no es fa servir
*/
void Peca::colocaPecaAtzar(int maxX,int maxY){
//Cal evitar que la Peça quedi en algun
//angle "impossible"
//reiniciaAnglesVertex(tipusPeca);
qsrand(pos().x());
reiniciaAnglesVertex();
setPos(qrand() % maxX,qrand() % maxY);
     while  (!pecaEnLimits() || pecaSuperposada(false) ){
        setPos(qrand() % maxX,qrand() % maxY);
    }
}

void Peca::colocaPecaAtzar(){
//Cal evitar que la Peça quedi en algun
//angle "impossible"
//reiniciaAnglesVertex(tipusPeca);
qsrand(pos().x());
reiniciaAnglesVertex();
//setAngleRotacio(normalitzaAngle(angleGirPeca*(rand() % 360)+1));
setAngleRotacio(normalitzaAngle((rand() % 5+1) * PI/4));
setPos(qrand() % (int) scene()->width() ,qrand() %  (int) scene()->height());
//18/03/14 posat el paràmetre false a pecaSuperposada
    while  (!pecaEnLimits() || pecaSuperposada(false) ){
       setPos(qrand() % (int) scene()->width() ,qrand() %  (int) scene()->height());
    }
//qDebug("angleGir-colocaPecaAtzar %f",angleGirPeca);
}

//Determina el rectangle que cal actualitzar quan cal
//redibuixar la Peça
QRectF Peca::boundingRect() const
{	   
    const int marge = 1;
    QRectF rect;

    rect.adjust(-escala*modulsVertex.value(modulMesGran),
		-escala*modulsVertex.value(modulMesGran),
		+escala*modulsVertex.value(modulMesGran),
		+escala*modulsVertex.value(modulMesGran));
    return rect.adjusted(-marge, -marge, +marge, +marge);

}


void Peca::mousePressEvent(QGraphicsSceneMouseEvent *event){
 if(!pecaMostra){
  if (shape().contains(event->pos()) && !pecaMostra){
      /* Activam les funcions tàctils si es donen les condicions
       */
      if(distanciaEntrePunts(event->pos(),centreRectanglePeca())>6.0  &&
            isSelected() && event->button()==Qt::LeftButton ){
          movimentPecaTactil=true;
          setSelected(true);
          controlPecaSeleccionada();
      }
      else {
          movimentPecaTactil=false;
          setSelected(false);
      }

	//Controlam el botó que s'ha clicat
	//Amb el botó esquerra+ Ctrl---> simetria de la peça
    if (event->button()==Qt::LeftButton ){
	    if (event->modifiers().testFlag ( Qt::ControlModifier )){
		simetriaPeca();
                //comprova si la peça està en els limits de la zona de joc
                /* 15/10/11
                afegit el control de superposició */
                if(!pecaEnLimits() || pecaSuperposada()) {
                    event->ignore();
                    simetriaPeca();
                }
		movimentPeca=0;
        movimentPecaTactil=false;
	    }
        else if (event->modifiers().testFlag(Qt::ShiftModifier)){
                //Personalització del color de la peça
                setColorPeca(
                        personalitzaColorPeca(indexEnElConjunt,
                                              getColorPeca(),conjuntDePeces->tipusTangram));
                setColorLimits(getColorPeca());
       }
      /* No s'ha fet servir el botó Crtl (no és simetria)
       * la peça es pot moure
       */
      else {
          if (movimentPecaTactil){
             //giram la peça i permetem que giri si xoca
             if (rotaPeca(sentitDelGir(centreRectanglePeca(),event->pos())*angleGirPeca*PI/180,true)){
                event->ignore();
                movimentPeca=2;
             }
            }
          //else{
          movimentPeca=1;
          //movimentPecaTactil=true;
          setZValue(20);
          setFlag(QGraphicsItem::ItemIsMovable,true);
          // setSelected(true);
         // }
         }
      }
	//Si es clica el botó mitjancer, es fa la simetria de la peça
	else if (event->button()==Qt::MidButton){
            setSelected(true);
            controlPecaSeleccionada();
            simetriaPeca();
            //comprova si la peça està en els limits de la zona de joc
            /* 15/10/11
            afegit el control de superposició */
            if(!pecaEnLimits() || pecaSuperposada()) {
                    event->ignore();
                    simetriaPeca();
                }
     movimentPeca=0;
     movimentPecaTactil=false;
    } //es fa servir el botó de la dreta
    else if ( (event->button()==Qt::RightButton)  ){
            if (event->modifiers().testFlag(Qt::ShiftModifier)){
                //Elimina la personalització del color de la peça
                eliminaPersonalitzaColorPeca(indexEnElConjunt,
                                              conjuntDePeces->tipusTangram);
                colorsAtzarPeca();
                return;
            }
            //setSelected(true);
            //rotació lliure de la peça
            if (angleGirPeca==ROTACIO_EXPERT){
               setZValue(20);
               movimentPeca=2;
               }
            else { //rotació de la peça amb angle fix (amb el botó dret)
                //QPointF finalGir=event->pos()-centrePeca();
                controlPecaSeleccionada();
                QPointF finalGir=event->pos()-centreRectanglePeca();
                float angleFinal=atan2(finalGir.y(),finalGir.x());
                int acelera=1;
                setZValue(20);
                if (angleFinal<0){angleFinal=angleFinal+2*PI;}
                //Això és per fer més ràpid el gir en el casos que
                //l'angle sigui petit;
                if (event->modifiers().testFlag ( Qt::ControlModifier ) &&
                    angleGirPeca<=15){
                  acelera=2;}
                //gira en un sentit o altre segons el punt del
                //clic
                double girFet;
                if (angleFinal>PI){
                   rotaPeca(acelera*angleGirPeca*PI/180);
                   girFet=acelera*angleGirPeca*PI/180;
                   }
                else {
                    /* 15/10/11
                       Canviat el signe a girFet per evitar que
                       a vegades, no funcini correctament
                       la tornada enrera si la peça queda superposada
                      */
                    rotaPeca(-acelera*angleGirPeca*PI/180);
                    girFet=-acelera*angleGirPeca*PI/180;
                      }
                //comprova si la peça està en els limits de la zona de joc
                /* 15/10/11
                afegit el control de superposició */
                if(!pecaEnLimits() || pecaSuperposada()) {
                //if(!pecaEnLimits()){
                    event->ignore();
                    rotaPeca(-girFet);
                    movimentPeca=0;
                }
                /* 15/10/11
                afegit per permetre que les peces girin automàticament
                quan xoquin */
                movimentPeca=2;
                }
	}
 }//final if contains
//el clic és  fora de la silueta de la Peça
  else {
    setSelected(false);
    setZValue(0);
    setFlag(QGraphicsItem::ItemIsMovable,false);
    movimentPeca=0;
    movimentPecaTactil=false;
    event->ignore();
  }
}
 else {//la peça és de la mostra
     setSelected(false);
     event->ignore();
     setZValue(-1);
     movimentPeca=0;
     movimentPecaTactil=false;
 }

}//Peca::mousePressEvent

void Peca::mouseMoveEvent ( QGraphicsSceneMouseEvent *event){
if(!pecaMostra){
    controlPecaSeleccionada();
  /* 23/07/11
     Això feia que en moure la peça ràpidament,
     sovint queda aturada. Pareix què també ara permet
     que en girar, el cursor surti de la peça i fa més fàcil
     el gir
    */
  //if (shape().contains(event->pos())){|| (movimentPecaTactil)

    /* La peça es pot moure però no està activat
     * el gir tàctil
     */
    if ( (movimentPeca==1) && !movimentPecaTactil ){
        QPointF darreraPos=pos();
        setFlag(QGraphicsItem::ItemIsMovable,true);
        QGraphicsItem::mouseMoveEvent(event);
        /* 15/10/11
        afegit el control de superposició */
        if(!pecaEnLimits() || pecaSuperposada()){
        /*Com què la peça està superposada o fora del límits
         *es desfà el moviment i es permet que la peça giri
         */
           //
           setPos(darreraPos);
           event->ignore();
           /* 03/03/14 innecessari*/
          /* if (angleGirPeca==ROTACIO_EXPERT){
               movimentPeca=2;
           }*/
           movimentPeca=2;
         }        
      }
    else if (  ((movimentPeca==2) || (movimentPecaTactil)) &&  (angleGirPeca==ROTACIO_EXPERT) ) {
        QPointF centre=centrePeca();//centreRectanglePeca();
       // QPointF centre=centreRectanglePeca();
        QPointF iniciGir=event->lastPos()-centre;
        QPointF finalGir=event->pos()-centre;
        float angleInici=atan2(iniciGir.y(),iniciGir.x());
        float angleFinal=atan2(finalGir.y(),finalGir.x());

       // if (angleInici<0){angleInici=angleInici+2*PI;}
       // if (angleFinal<0){angleFinal=angleFinal+2*PI;}
        //Si la tecla crtl està pitjada, llavors
        //la rotació es fa més "lenta"
        double precisio=1.0;
        if (event->modifiers().testFlag ( Qt::ControlModifier )){
                  precisio=0.2;}
        //28/12/15 Eliminat el següent if atès que la comprovació
        //de la superposició ja ho fa el proc. rotapeca()
        if(!rotaPeca(precisio*(angleFinal-angleInici),true)){
            event->ignore();
            movimentPeca=1;
          }
            ;
        // 15/10/11
        //afegit el control de superposició
       /* if(!pecaEnLimits() || pecaSuperposada() ){
           event->ignore();
           rotaPeca(precisio*(-angleFinal+angleInici),true);
           movimentPeca=1;
         };*/
       } // final else if moviment expert

      /* 15/10/11
         Afegit això per permetre que les peces girin automàticament
         quan xoquin
        */
    else if ( angleGirPeca != ROTACIO_EXPERT && !movimentPecaTactil ){
          //rotació de la peça amb angle fix
         // QPointF finalGir=event->pos()-centrePeca();
          QPointF finalGir=event->lastPos()-centrePeca();
          float angleFinal=atan2(finalGir.y(),finalGir.x());
          int acelera=1;
          setZValue(20);
          if (angleFinal<0){angleFinal=angleFinal+2*PI;}
          //Això és per fer més ràpid el gir en el casos que
          //l'angle sigui petit;
          if (event->modifiers().testFlag ( Qt::ControlModifier ) &&
              angleGirPeca<=15){
              acelera=2;}
          //gira en un sentit o altre segons el punt del
          //clic
          double girFet;
          if (angleFinal>PI){
              girFet=acelera*angleGirPeca*PI/180;
              rotaPeca(girFet,true);
          }
          else {
              girFet=-acelera*angleGirPeca*PI/180;
              rotaPeca(girFet,true);
          }
          //comprova si la peça està en els limits de la zona de joc
          /* 15/10/11
             afegit el control de superposició
          28/12/15 Eliminat ja que la comprovació de la superposició
          ja es fa a rotapeca()*/
          /*if(!pecaEnLimits() || pecaSuperposada()) {
              //if(!pecaEnLimits()){
              event->ignore();
              rotaPeca(-girFet);
             // movimentPeca=1;
          }*/
          movimentPeca=1;
          movimentPecaTactil=false;
      }
 /*
   }//final if contains  23/07/11
  else {
     //Això per evitar que la peça quedi "seleccionada" quan el cursor està fora de la peça
     setSelected(false);
     event->ignore();
    // setZValue(-1);
     movimentPeca=0;
 }*/
}
else {
     event->ignore();
     movimentPeca=0;
     movimentPecaTactil=false;
 }
}//final Peca::mouseMoveEvent

void Peca::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
 //controlam si cal "aferrar" la peça
 //13/02/15 Aquest if pareix que ja no és necessari amb el sistema
 //de moviment tàctil
 //if (event->button()==Qt::LeftButton){
   if ( !event->modifiers().testFlag ( Qt::ControlModifier )){
       QPointF distancia;
       distancia= aferraPeca();
       QPointF posicioActual=pos();
       setPos(pos()-distancia);
       /*04/08/15 Afagit per evitar que les peces
        * quedin una mica superposades i millorar el tema de les dades
        * de posicions de les peces per a la generació automàtica de figures*/
       float reduccio=1.0;
       int contador=0;
       while(pecaSuperposada(false) && contador<10){
           setPos(posicioActual-distancia*reduccio);
           reduccio=reduccio-0.01;
           contador++;
       }
       /* 15/10/11
          Es controla que la peça no quedi superposada
          allunyant-la desprès d'aferrar-la.
          13/11/11
          Això provoca un bucle infinit en
          algunes situacions
       while(pecaSuperposada()){
        setPos(pos()+distancia*0.01);
       }*/
   }
//}

 //deseleccionam la Peça
 //13/02/15 Això no és convenient. És millor deixar
 //la peça seleccionada per permetre que es pugi seguir el
 //moviment
// setSelected(false);
 setFlag(QGraphicsItem::ItemIsMovable,false);
 //Canviat 25/06/10 La peça seleccionada sempre queda
 //a sobre de les altres
 //setZValue(0);
 //Deixam que "continuï" l'event
 QGraphicsItem::mouseReleaseEvent(event);
}//final Peca::mouseReleaseEvent

/* Assegura que la peça actualment seleccionada amb el ratolí
 * sigui l'única seleccionada
 */
void Peca::controlPecaSeleccionada(){
 for(int i=0;i<=conjuntDePeces->arrayPeces.size()-1;i++){
  if(!(this->indexEnElConjunt == conjuntDePeces->arrayPeces[i]->indexEnElConjunt) ){
    conjuntDePeces->arrayPeces[i]->setSelected(false);
  }
 }
}

/*
  Calcula el trasllat necessari per
  "aferrar" la peça a la més propera.
  Primer es comproven els vertex de la peça
  i, si no és possible "aferrar-la" llavors
  s'intenta fer el mateix amb els costats
  (amb el procediment aferraPeca2)
*/
QPointF Peca::aferraPeca(){

 QVector<QPointF> aquestaPeca;
 QVector<QPointF> altraPeca;
 QPointF desplacament, despActual;
 float distancia, distanciaActual;
 puntsVertexPeca(aquestaPeca);
 desplacament=QPointF(0,0);
 //QPointF posicioInicial=pos();
 bool aferrada=false;
 for(int i=0;i<=conjuntDePeces->arrayPeces.size()-1;i++){
     if(!(this->indexEnElConjunt == conjuntDePeces->arrayPeces[i]->indexEnElConjunt) ){
        conjuntDePeces->arrayPeces[i]->puntsVertexPeca(altraPeca);
        distancia=10000;
        for(int j=0;j<=aquestaPeca.size()-1;j++){
            for(int k=0;k<=altraPeca.size()-1;k++){
                despActual=aquestaPeca[j]-altraPeca[k];
                distanciaActual=sqrt(pow(despActual.x(),2)+pow(despActual.y(),2));
           /* 20/03/14 Afegit && !pecaSuperposada(false): alguns casos podian quedar peces
            * superposades */
                if( (distanciaActual<distancia) && (distanciaActual<=5.0) && !pecaSuperposada(false)){
                   //!pecaSuperposadaEscala(false,0.90)){
                   distancia=distanciaActual;
                   desplacament=despActual;
                   aferrada=true;
                }
            }
        }
     }
    }
 if(aferrada){     
     return desplacament;
   }
 else return aferraPeca2();
//return desplacament;
}

/*
  "Aferra" la peça actual pels costats
  */
QPointF Peca::aferraPeca2(){
 QPointF desplacament, despActual;
 float distancia, distanciaActual,distanciaRecorreguda;

 desplacament=QPointF(0,0);
 distanciaActual=0.0;
 distanciaRecorreguda=0.0;
 QPointF posicioInicial=pos();
 for(int i=0;i<conjuntDePeces->arrayPeces.size();i++){
     float vector=0.0;
     int valor=0;
     distanciaRecorreguda=0.0;
     distancia=100000;
     QPointF vectorCentres=(conjuntDePeces->arrayPeces[i]->centreRectanglePeca()+
                           conjuntDePeces->arrayPeces[i]->pos())-
                           (centreRectanglePeca()+pos());
     if(indexEnElConjunt != conjuntDePeces->arrayPeces[i]->indexEnElConjunt){
         while( (distanciaRecorreguda<8.0) &&  !pecaSuperposada(false) ){
                //!pecaSuperposadaEscala(false,1.1)){
         setPos(pos()+vectorCentres*vector);
         distanciaRecorreguda=sqrt(pow((posicioInicial-pos()).x(),2)+pow((posicioInicial-pos()).y(),2));
         valor++;
         vector=((float) valor)/((float) 10000);
        }
         if(pecaSuperposada(false)){
             //El 0.90 per evitar que quedin una mica superposades
             //Això també depén del 10000 del vector
             despActual=(posicioInicial-pos())*0.90;
             distanciaActual=sqrt(pow(despActual.x(),2)+pow(despActual.y(),2));
         }
   }//final if del index del conjunt
 setPos(posicioInicial);
 if(distanciaActual<distancia){
   desplacament=despActual ;
   distancia=distanciaActual;
  }
 }//final for de les peces del conjunt

return desplacament;
}

//determina si la peça està en els límits de l'escena
bool Peca::pecaEnLimits(){
    QVector<QPointF> vertexPeca;
    puntsVertexPeca(vertexPeca);
    bool enLimits=true;
    for(int j=0;j<=vertexPeca.size()-1;j++){
        if (!scene()->sceneRect().contains(vertexPeca.value(j))){
           enLimits=false;
        }
    }
return enLimits;
}//final Peca::pecaEnLimits()

/* 15/10/11
determina si la peça xoca amb una altra
*/
bool Peca::pecaSuperposada(bool controlInfantil){
    //això restringeix la funció als nivells
    //iniciat i expert
    //if (angleGirPeca != ROTACIO_EXPERT){ return false;}
    if( (nivellJoc==NIVELL_INFANTIL) &&
        controlInfantil){return false;}
    bool xoca=false;
    for(int j=0;j<conjuntDePeces->arrayPeces.size();j++){
        if (conjuntDePeces->arrayPeces[j] != this){
            if(conjuntDePeces->arrayPeces[j]->shape().translated(
                  conjuntDePeces->arrayPeces[j]->pos())
                .intersects(
                    this->shape().translated(
                          this->pos()))){
            xoca=true;
            }
        }
    }
  return xoca;
}//final Peca::pecaSuperposada()

/* El mateix que l'anterior, però es pot fer servir
 * el paràmetre escala per augmentar o minvar el tamany
 * de la peça actual*/
bool Peca::pecaSuperposadaEscala(bool controlInfantil, float escala){
    //això restringeix la funció als nivells
    //iniciat i expert
    if( (nivellJoc==NIVELL_INFANTIL) &&
        controlInfantil){return false;}
    bool xoca=false;
    for(int j=0;j<conjuntDePeces->arrayPeces.size();j++){
        if (conjuntDePeces->arrayPeces[j] != this){
            if(conjuntDePeces->arrayPeces[j]->shape().translated(
                  conjuntDePeces->arrayPeces[j]->pos())
                .intersects(
                    this->shapeEscala(escala).translated(
                          this->pos()))){
            xoca=true;
           // qDebug("La peça xoca amb escala!");
            }
        }
    }
  return xoca;
}//final Peca::pecaSuperposada()

/* Controla si la peça està superposada a
 * les peçes passades per paràmetre
 * Si hi ha una peça que no és de les passades per paràmetre
 * que es superposa, llavors es recol·loca a l'atzar.
 */
bool Peca::pecaSuperposada(QList<int> indexPeces){
bool xoca=false;
/*07/09/15 Afegit la segona condició al 1r if
 * i comentat el 3er if*/
for(int j=0;j<conjuntDePeces->arrayPeces.size();j++){
 if (  (conjuntDePeces->arrayPeces[j] != this)  ){ //&&
       //(indexPeces.contains(j)) ){
 if(conjuntDePeces->arrayPeces[j]->shape().translated(
       conjuntDePeces->arrayPeces[j]->pos())
     .intersects(
         this->shape().translated(
               this->pos()))){
     if(indexPeces.contains(j) || !pecaEnLimits()){
            xoca=true;}
     else {
     /* Si està superposada a una peça que no és de les col·locades,
      * es mou la peça i es retorna false per permetre que segueixi
      * el gir en el mateix sentit
      */
         conjuntDePeces->arrayPeces[j]->colocaPecaAtzar();
         xoca=false;}
    // xoca=true;
    }
 }
}
/*for(int j=0;j<indexPeces.size();j++){
 if(conjuntDePeces->arrayPeces[indexPeces.value(j)]->shape().translated(
       conjuntDePeces->arrayPeces[indexPeces.value(j)]->pos())
     .intersects(
         this->shape().translated(
               this->pos()))){
 xoca=true;
 }
}*/
return xoca;
}

/* Igual que l'anterior, però amb el paràmetre escala
 */
bool Peca::pecaSuperposadaEscala(QList<int> indexPeces, double escala){
bool xoca=false;

for(int j=0;j<conjuntDePeces->arrayPeces.size();j++){
 if (conjuntDePeces->arrayPeces[j] != this){
 if(conjuntDePeces->arrayPeces[j]->shape().translated(
       conjuntDePeces->arrayPeces[j]->pos())
     .intersects(
         this->shapeEscala(escala).translated(
               this->pos()))){
     if(indexPeces.contains(j) || !pecaEnLimits()){
         xoca=true;
       }
    }
 }
}
return xoca;
}



/*
Retorna una cadena amb les dades de la peça
per tal de formar figures
*/
QString Peca::cadenaPropietatsPeca( QPointF vertexSuperiorEsquerra){
    QString dades;
    dades=QString("%1").arg(tipusPeca)+" ";
    dades=dades+QString("%1").arg((pos().x()-vertexSuperiorEsquerra.rx()))+" "+
                QString("%1").arg((pos().y()-vertexSuperiorEsquerra.ry()))+" ";
    dades=dades+QString("%1").arg((anglesVertex[0]-angleInicial))+" ";
    if (simetriaBool){dades=dades+"1 ";}
       else {dades=dades+"0 ";}
 // printf(dades.toAscii()+ " dades peça \n");
  return dades;
}

void Peca::setColorPeca(const QColor &color)
{
    colorPeca = color;
    update();
}

QColor Peca::getColorPeca() const
{
    return colorPeca;
}

void Peca::colorsAtzarPeca(){
static bool primerCop = true;
//això per generar "aleatòriament" la llavor dels nombres aleatoris
     if (primerCop) {
         primerCop = false;
         QTime rellotge(0, 0, 0);
         qsrand(rellotge.secsTo(QTime::currentTime()));
     }
	setColorPeca(QColor(qrand() % 256, qrand() % 256, qrand() % 256,255));
	setColorLimits(getColorPeca());
}

void Peca::setColorLimits(const QColor &color)
{
    colorLimits = color;
    update();
}

QColor Peca::getColorLimits() const
{
    return colorLimits;
}

/**
  * Estableix l'angle de rotació de la Peça
  * (ho fa com a valor "absolut")
  *
  * L'angleRotacio nomès es fa servir per dur
  * "el compte" del que ha rotat la Peça i poder
  * passar aquest valor a la cadena de les propietats
  * angleRotacio està en radiants
  */
void Peca::setAngleRotacio(double angle){
   //reiniciaAnglesVertex();
    angleRotacio=-angle;
	//cal actualitzar els angles dels vertex
	actualitzaAnglesVertex(angle);
	//Actualitzam la peça
   // update();
}

bool Peca::rotaPeca(double angle, bool controlaLimitsSuperposicio){
   angleRotacio=angleRotacio-angle; 
   actualitzaAnglesVertex(angle);
   //qDebug("angle %f",angle);
   bool retorn=false;
   /* 02/03/14
    * Això podria estar aqui, però caldria
    * revisar a fons les conseqüències
    */
   if (controlaLimitsSuperposicio){
   if(!pecaEnLimits() || pecaSuperposada(false) ){
       angleRotacio=angleRotacio+angle;
       actualitzaAnglesVertex(-angle);
       retorn=true;
      }
     }
return retorn;
}

//actualitza els angles dels vèrtex quan canvia
//angleRotacio
void Peca::actualitzaAnglesVertex(double angle){
//Actualitzam els angles dels vertex
for(int i=0;i<=numeroVertex-1;i++){
   anglesVertex.replace(i,normalitzaAngle(anglesVertex.value(i)+angle));
   }
if(ambArc){
 angleCentreArc=normalitzaAngle(angleCentreArc+angle);  
 }
 update();
}

//Normalitza l'angle entre 0 i 2PI
double Peca::normalitzaAngle(double angle) {
// printf ("Angle actual (normalitzaAngle) %lf  \n",angle );
	if ( angle < 0) {
	    angle += 2 * PI;
	}  
	while (angle>2*PI)      
       angle -= 2 * PI;
       
// printf ("Angle retornat (normalitzaAngle) %lf  \n",angle );
       return angle;
    }
 
 /*
 Normalitza l'angle entre 0 i 360
 */   
double Peca::normalitzaAngleGraus(double angle){
  if ( angle < 0){
  	 angle=angle+360;
 	}
   while (angle>360) 
   	 angle=angle-360;
   return angle;
}

//Retorna l'angle de rotació actual de la peça
double Peca::getAngleRotacio () {
	return angleRotacio;
}

void Peca::setEscala(double pescala){
	escala=pescala;
}


void Peca::estableixAngleGirPeca(double angle){
    angleGirPeca=angle;
    /*
    if (tipusPeca==T9T_TRAPEZI){
        angleGirPeca=ROTACIO_EXPERT;
    }
    */
}
double Peca::emetAngleGirPeca(){
    return angleGirPeca;
}

double Peca::getEscala() const{
	return escala;
}

/*
  Retorna (en el vector del parámetre) les coordenades
  dels vèrtex de la peça
*/
void Peca::puntsVertexPeca(QVector<QPointF> &pVertex){
    pVertex.clear();
    pVertex.resize(numeroVertex);
  // QPointF puntCentre = boundingRect().center();
   for(int i=0;i<=numeroVertex-1;i++){
       pVertex.replace(i, pos()+QPointF(
           escala*modulsVertex.value(i)*cos(anglesVertex.value(i)),
           escala*modulsVertex.value(i)*sin(anglesVertex.value(i))));
   }
}

/* Fa el mateix que l'anterior, però permet
 * fer ampliació/reducció dels vèrtex
 */
void Peca::puntsVertexPecaReducccio(QVector<QPointF> &pVertex, double reduccio){
    pVertex.clear();
    pVertex.resize(numeroVertex);
  // QPointF puntCentre = boundingRect().center();
   for(int i=0;i<=numeroVertex-1;i++){
       pVertex.replace(i, pos()+QPointF(
          reduccio*escala*modulsVertex.value(i)*cos(anglesVertex.value(i)),
          reduccio*escala*modulsVertex.value(i)*sin(anglesVertex.value(i))));
   }
}

/* Punts per controlar la solució*/
void Peca::puntsSolucio(QVector<QPointF> &pSol,float reduccio){
  /* QTime rellotgeQTime (0,0,0);
    rellotgeQTime.currentTime();
    rellotgeQTime.start();*/

   for(int i=0;i<=numeroVertex-1;i++){
       pSol.append(pos()+QPointF(
           reduccio*escala*modulsVertex.value(i)*cos(anglesVertex.value(i)),
           reduccio*escala*modulsVertex.value(i)*sin(anglesVertex.value(i))));
    //  printf(QString("Punt peca %1-%2 \n").arg(pSol.value(i).x()).arg(pSol.value(i).y()).toAscii());       
       pSol.append(pos()+QPointF(
          reduccio*reduccio*escala*modulsVertex.value(i)*cos(anglesVertex.value(i)),
          reduccio*reduccio*escala*modulsVertex.value(i)*sin(anglesVertex.value(i))));
          }

   /* 4.0.2
      Afegits punts addicionals de control de la solució. S'afegeixen
      10 punts a l'atzar a l'interior de la peca
   */
   /* 22/02/11
      Per alguna raó encara per determinar, amb aquestes peces
      no funciona això (es queda dins el while degut a què
      no s'aconsegueixen els punt addicionals)
     */
   /* 01/01/12  Eliminat: no pareix que serveixi de gran cosa això
   if( (tipusPeca !=62) && (tipusPeca != 67) && (tipusPeca != 68)){
   int puntsAddicionals=3;
   //for(int i=0;i<=9;i++){
  qsrand(rellotgeQTime.msecsTo(QTime::currentTime()));
  for(int i=0;i<=numeroVertex-1;i++){
   while (puntsAddicionals<3){      
      int numVertex=qrand() % numeroVertex;
      QPointF puntAddicional(
           (1/pow(5,puntsAddicionals+1))*reduccio*escala*modulsVertex.value(numVertex)*cos(anglesVertex.value(numVertex)),
           (1/pow(5,puntsAddicionals+1))*reduccio*escala*modulsVertex.value(numVertex)*sin(anglesVertex.value(numVertex)));

      if (shape().contains(puntAddicional)){
       pSol.append(pos()+puntAddicional);
       puntsAddicionals++;
      }
   }
  }
}*/

 //es pot afegir un punt de l'arc per millorar la correcció
 if (ambArc){
        float iniciArc;
        QPainterPath polygonPath;
        iniciArc=0.0;
        QRectF rectangleArc;
        rectangleArc=emetArcPeca(iniciArc);
        QPointF puntActual(escala*modulsVertex.value(numeroVertex-1)*cos(anglesVertex.value(numeroVertex-1)),
                           escala*modulsVertex.value(numeroVertex-1)*sin(anglesVertex.value(numeroVertex-1)));
        polygonPath.moveTo(boundingRect().center()+puntActual);
        for(int i=1;i<=2;i++){
          polygonPath.arcMoveTo(rectangleArc,(iniciArc-i*angleArc/5));
          //El 0.7 és empiric
          QPointF puntArc(0.2*reduccio*polygonPath.currentPosition().x(),
                          0.2*reduccio*polygonPath.currentPosition().y());
          pSol.append(pos()+puntArc);
        }
       }
}

void Peca::puntsSolucio(QVector<QPointF> &pSol,QPointF vertSupEsq,float reduccio){

   for(int i=0;i<=numeroVertex-1;i++){
       pSol.append(pos()-vertSupEsq+QPointF(
           reduccio*escala*modulsVertex.value(i)*cos(anglesVertex.value(i)),
           reduccio*escala*modulsVertex.value(i)*sin(anglesVertex.value(i))));
   }
}

QPainterPath Peca::shape() 
{   
    //la silueta que retorna la funció i que dibuixa la peça
    QPainterPath polygonPath;
    /*calculam el punt central de la peça, en funció del centre
    del rectangle que ocupa*/
    QPolygonF polygon;
    for(int i=0;i<=numeroVertex-1;i++){
        polygon<<boundingRect().center()+QPointF(
    	   escala*modulsVertex.value(i)*cos(anglesVertex.value(i)),
    	   escala*modulsVertex.value(i)*sin(anglesVertex.value(i)));
        }

     polygonPath.addPolygon(polygon);

     /*afegim l'arc de la peça (si en té)*/
     if (ambArc){
     	float iniciArc;
     	iniciArc=0.0;
     	QRectF rectangleArc;
        rectangleArc=emetArcPeca(iniciArc);
     	QPointF puntActual(polygonPath.currentPosition());
     	polygonPath.arcMoveTo(rectangleArc,(iniciArc-angleArc/2));
     	//polygonPath.arcMoveTo(rectangleArc,(45-iniciArc-angleArc/2));
     	QPointF punt(polygonPath.currentPosition());
        polygonPath.moveTo(puntActual);
        QPointF puntFinal(escala*modulsVertex.value(0)*cos(anglesVertex.value(0)),
                          escala*modulsVertex.value(0)*sin(anglesVertex.value(0)) );
        polygonPath.quadTo(ajustBezier*punt,boundingRect().center()+puntFinal);
	//aquest moviment és per aconseguir que es tanqui be el dibuix
        polygonPath.moveTo(puntFinal);
        }
    /*tancam el dibuix de la silueta*/
    polygonPath.closeSubpath();
/*
    QVector<QPointF> pSolucio;
    puntsSolucio(pSolucio,0.9);
    QPolygonF polygon1(pSolucio);
    polygonPath.addPolygon(polygon1);
    polygonPath.seSubpath();
    */

    return polygonPath;
}//Final Peca::shape

/*
  Retorna la silueta de la peça però fent servir
  l'escala passada per paràmetre
  */
QPainterPath Peca::shapeEscala(float pescala)
{
    //la silueta que retorna la funció i que dibuixa la peça
    QPainterPath polygonPath;
    /*calculam el punt central de la peça, en funció del centre
    del rectangle que ocupa*/
    QPolygonF polygon;
    for(int i=0;i<=numeroVertex-1;i++){
        polygon<<boundingRect().center()+QPointF(
           pescala*escala*modulsVertex.value(i)*cos(anglesVertex.value(i)),
           pescala*escala*modulsVertex.value(i)*sin(anglesVertex.value(i)));
        }

     polygonPath.addPolygon(polygon);

     /*afegim l'arc de la peça (si en té)*/
     if (ambArc){
        float iniciArc;
        iniciArc=0.0;
        QRectF rectangleArc;
        rectangleArc=emetArcPeca(iniciArc);
        QPointF puntActual(polygonPath.currentPosition());
        polygonPath.arcMoveTo(rectangleArc,(iniciArc-angleArc/2));
        //polygonPath.arcMoveTo(rectangleArc,(45-iniciArc-angleArc/2));
        QPointF punt(polygonPath.currentPosition());
        polygonPath.moveTo(puntActual);
        QPointF puntFinal(pescala*escala*modulsVertex.value(0)*cos(anglesVertex.value(0)),
                          pescala*escala*modulsVertex.value(0)*sin(anglesVertex.value(0)) );
        polygonPath.quadTo(ajustBezier*punt,boundingRect().center()+puntFinal);
        //aquest moviment és per aconseguir que es tanqui be el dibuix
        polygonPath.moveTo(puntFinal);
        }
    /*tancam el dibuix de la silueta*/
    polygonPath.closeSubpath();

    return polygonPath;
}//Final Peca::shape

/*
Fa els càlculs per dibuixar l'arc de la Peça.
Retorna el rectangle i modifica els paràmetres
*/
QRectF Peca::emetArcPeca(float &angleInici){

      /* centre de la peça*/
    //  QPointF puntCentre =boundingRect().center();
      //centre de l'arc
      QVector<float> centre;
      centre.resize(2);
      centre.replace(0,escala*centreArc*cos((angleCentreArc)));           
      centre.replace(1,escala*centreArc*sin((angleCentreArc)));

      QVector<float> iniciArc  ;
      iniciArc.resize(2);
      iniciArc.replace(0,escala*modulsVertex.value(0)*cos(anglesVertex.value(0)));
      iniciArc.replace(1,escala*modulsVertex.value(0)*sin(anglesVertex.value(0)));

      QVector<float> finalArc;
      finalArc.resize(2);
      finalArc.replace(0,escala*modulsVertex.value(numeroVertex-1)*cos(anglesVertex.value(numeroVertex-1)));
      finalArc.replace(1,escala*modulsVertex.value(numeroVertex-1)*sin(anglesVertex.value(numeroVertex-1)));

      //càlcul del radi de l'arc
      QVector<float> vec_radi;
      vec_radi.resize(2);
      vec_radi.replace(0,iniciArc.value(0) - centre.value(0));
      vec_radi.replace(1,iniciArc.value(1) - centre.value(1));

      //el radi és el mòdul del vec_radi
      float radi;
      radi=sqrt(pow(vec_radi.value(0),2)+pow(vec_radi.value(1),2));

      QVector<float> centreInici; 
        centreInici.resize(2);    
      	centreInici.replace(0,iniciArc.value(0)-centre.value(0));
      	centreInici.replace(1,iniciArc.value(1)-centre.value(1));

      QVector<float> centreFinal; 
        centreFinal.resize(2);     
      	centreFinal.replace(0,finalArc.value(0)-centre.value(0));
      	centreFinal.replace(1,finalArc.value(1)-centre.value(1));  

      
      //angle format pel radi amb l'inici (la recta de les 3)
      float anglePelRadi=0.0;
      if(simetriaBool){
           anglePelRadi=angleRadi(centreFinal);
        }
        else{           
           anglePelRadi=angleRadi(centreInici); 
        }
      angleInici=anglePelRadi;


  QRectF rectangleArc(centre.value(0)-radi,centre.value(1)-radi,2*radi,2*radi);
  return rectangleArc;

}//final emetArcPeca

/*
Retorna l'angle format entre l'eix OX positiu i el vector
del centre de l'arc al punt d'inici de l'arc
*/
float Peca::angleRadi(QVector<float> VectorCentreInici){
	float angle;	
	//angle=atan(VectorCentreInici.value(1)/VectorCentreInici.value(0));	
	angle=atan2(VectorCentreInici.value(1),VectorCentreInici.value(0));
  //l'angle ho tornam en graus
 return (-angle*180/PI);
}
/*
Retorna l'angle (fitxe) de l'arc de la Peça
(es carrega quan es crea la peça)
*/
float Peca::angleArcPeca(){
 float angle;
 //Casos especials
 if (tipusPeca==67){return 60;}
 if (tipusPeca==68){return 120;}
 if (tipusPeca==69){return 180;}
 //centre de l'arc
      QVector<float> centre;
      centre.resize(2);
      centre.replace(0,escala*centreArc*
                        cos((angleCentreArc)));           
      centre.replace(1,escala*centreArc*
                        sin((angleCentreArc)));
      QVector<float> iniciArc  ;
      iniciArc.resize(2);
      iniciArc.replace(0,escala*modulsVertex.value(0)*cos(anglesVertex.value(0)));
      iniciArc.replace(1,escala*modulsVertex.value(0)*sin(anglesVertex.value(0)));
      QVector<float> finalArc;
      finalArc.resize(2);
      finalArc.replace(0,escala*modulsVertex.value(numeroVertex-1)*cos(anglesVertex.value(numeroVertex-1)));
      finalArc.replace(1,escala*modulsVertex.value(numeroVertex-1)*sin(anglesVertex.value(numeroVertex-1)));
      QVector<float> centreInici; 
        centreInici.resize(2);    
      	centreInici.replace(0,iniciArc.value(0)-centre.value(0));
      	centreInici.replace(1,iniciArc.value(1)-centre.value(1));
      QVector<float> centreFinal; 
        centreFinal.resize(2);     
      	centreFinal.replace(0,finalArc.value(0)-centre.value(0));
      	centreFinal.replace(1,finalArc.value(1)-centre.value(1));      
  angle=angleRadi(centreInici)-angleRadi(centreFinal);
 return angle;
}

/*funció obsoleta 01/01/12 ????*/
QPointF Peca::centrePeca() const{
if(numeroVertex<20){
    QPointF centre;
    double accX,accY;
    accX=0;
    accY=0;
    for(int i=0;i<=numeroVertex-1;i++){
    accX =accX+escala*modulsVertex.value(i)*cos(anglesVertex.value(i));
    accY =accY+escala*modulsVertex.value(i)*sin(anglesVertex.value(i)); 
     }
    centre.setX(accX/numeroVertex);
    centre.setY(accY/numeroVertex);
    return centre;
}
else return boundingRect().center();
}

QPointF Peca::centreRectanglePeca(){
    QPointF centre = boundingRect().center();
    return centre;
}

void Peca::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option,
                 QWidget * /* widget */){
if (pecaVisible){
    QPen pen(colorLimits);
    pen.setWidth(1);
    if(solucio ){
      painter->setBrush(Qt::white);
      //Aquesta és l'opció amb la qual s'obtenen els arxius d'imatge més petits
      //painter->setBrush(Qt::black); //setBrush(Qt::NoBrush); //setBrush(Qt::white);
      pen.setColor(Qt::red);}
    else  {
      painter->setBrush(colorPeca);
      //painter->setBrush(Qt::white);
      pen.setColor(colorLimits);
      }
    painter->setPen(pen);    
    painter->setRenderHint(QPainter::Antialiasing);

    
    //transparent si està seleccionada
   // if (isSelected()){
    if (option->state & QStyle::State_Selected) {
       painter->setOpacity(transparencia);
     }

    /* color negre si està superposada
    if (pecaSuperposada(false)) {
       //painter->setOpacity(transparencia);
       painter->setBrush(Qt::white);
     }*/

    /*Fons blanc si s'ha fet la simetria a la peça
    if (simetriaBool){
        painter->setBrush(Qt::white);
    }*/

    painter->drawPath(shape());

    /* 02/03/14
     * Dibuixa un cercle al centre del rectangle
     * de dibuix de la peça
     */
    if (option->state & QStyle::State_Selected) {
    painter->setBrush(Qt::white);
    painter->setPen(Qt::black);
    // 20/02/15 Augmentat el tamany de 4 a 6.
    // Cal caviar-ho a la mousePressEvent que ho fa servir
    painter->drawEllipse(boundingRect().center(),6,6);
    }

    /* Dibuixa el índex a l'array de peces i el codi de la peça
painter->setPen(Qt::black);
painter->drawText(1,1,QString("%1 (%2)").arg(indexEnElConjunt).arg(tipusPeca));*/

    /* Dibuixa el rectangle que envolta la peça
     */
   // painter->drawPolyline(boundingRect());


    //Dibuixa els punts de solució
    /*
    pen.setColor(Qt::red);
        painter->setPen(pen);
        painter->drawEllipse(centrePeca(),1,1);
        QVector<QPointF> pSolucio;
        puntsSolucio(pSolucio,0.9);

    for(int i=0;i<pSolucio.size();i++){
       painter->drawEllipse(pSolucio.value(i)-pos(),1,1);
       }
       */

      /*  painter->drawText(1,1,QString("%1 (%2)").arg(indexEnElConjunt).arg(tipusPeca));
        painter->drawLine(centrePeca(),
                          QPointF(escala*modulsVertex.value(0)*cos(anglesVertex.value(0)),
                                  escala*modulsVertex.value(0)*sin(anglesVertex.value(0))));*/
  //  painter->drawRect(boundingRect());

 //per dibuixar el centre de la peça
    /*
 QPointF centreD;
QPointF centreF=boundingRect().center();
   centreD=centrePeca();
   painter->setBrush(Qt::red);
   painter->drawPoint(centreD);
   painter->setBrush(Qt::black);
   painter->drawPoint(centreF);*/

  }
}//Final Peca::Paint


     /**
     * Canvia el valor de la propietat simetriaBool
     */
void Peca::canviaSimetriaPeca(){
     //canviam el valor de la propietat
     simetriaBool = !simetriaBool; 
    }
    
    /**
     * Fa la simetria de la figura, canviant
     * els angles
     */ 
    void Peca::simetriaPeca(){ 
         for(int i=0;i<=numeroVertex-1;i++){
            if (anglesVertex.value(i)<=PI && anglesVertex.value(i)>=0){
                anglesVertex.replace(i,PI-(anglesVertex.value(i)));
            } 
            else {
                anglesVertex.replace(i,3*PI-(anglesVertex.value(i))); 
            }
         }
        //Rectificam tambè l'angle inicial
        if (angleInicial<=PI && angleInicial>=0){
                angleInicial=PI-(angleInicial);
            } 
            else {
                angleInicial=3*PI-(angleInicial); 
            }
        //i l'angle de l'arc en el cas de les peces
        //amb arc
        if(ambArc){
        if (angleCentreArc<=PI && angleCentreArc>=0){
                angleCentreArc=PI-(angleCentreArc);
            } 
            else {
                angleCentreArc=3*PI-(angleCentreArc); 
            }           
        }
         
       //canviam el valor de la propietat
       simetriaBool = !simetriaBool; 

    //  update();
}//final Peca::simetriaPeca

/*
Simplement, fa update()
*/
void Peca::actualitzaPeca(){
    update();
}

/*
Crea la Peça segons el parámetre.
Estableix els valors
*/
void Peca::creaPeca(int pintPeca){
  setFlags(ItemIsMovable | ItemIsSelectable);
  tipusPeca=pintPeca;
  //establix l'angle de rotació
  angleRotacio=0.0;
  simetriaBool=false;
  ambArc=false;
  solucio=false;
  pecaMostra=false;
  pecaVisible=true;
  centreArc=0.0;
  angleCentreArc=0.0;
  //inicialment, serà el primer
  modulMesGran=0;
  //inicialment 1.0
  ajustBezier=1.0;
  //inicialment, la peça no es mou
  movimentPeca=0;
  movimentPecaTactil=false;
  //inicialment el gir és lliure
  /*24/04/14 Cal tenir present que, quan es carrega una figura a partir
   * de les dades, llavors tot el conjunt de peces queda amb aquest
   * angle de gir*/
  angleGirPeca=1.0;
  //Nivell del joc actual
  nivellJoc=NIVELL_INFANTIL;

  QString dadesPeces=recuperaDadesPeca();
  //QString dadesPeces=llegeixLiniaArxiu(
    //   directoriLinux()+QDir().separator()+"figures"+QDir().separator()+"peces.dat",
    //   tipusPeca*2);
  QStringList dadesDeLaPeca=dadesPeces.split(";");
  if (dadesDeLaPeca.value(0).toInt()==0){ambArc= false;}
  else {
      ambArc=true;
  }
  centreArc=dadesDeLaPeca.value(1).toFloat();
  modulMesGran=dadesDeLaPeca.value(2).toInt();
  ajustBezier=dadesDeLaPeca.value(3).toFloat();
  angleInicial=dadesDeLaPeca.value(4).toFloat();
  angleCentreArc=dadesDeLaPeca.value(5).toFloat();
  numeroVertex=dadesDeLaPeca.value(6).toInt();
  modulsVertex.resize(numeroVertex);
  anglesVertex.resize(numeroVertex);
  for(int i=0;i<numeroVertex;i++){
   modulsVertex.replace(i,dadesDeLaPeca.value(7+i).toFloat());
   anglesVertex.replace(i,dadesDeLaPeca.value(7+numeroVertex+i).toFloat());
  }
  if (ambArc){
      angleArc=angleArcPeca();}
}

void Peca::anglesInicialsPeca(){
   QString dadesPeces=recuperaDadesPeca();
    QStringList dadesDeLaPeca=dadesPeces.split(";");
    centreArc=dadesDeLaPeca.value(1).toFloat();
    modulMesGran=dadesDeLaPeca.value(2).toInt();
    ajustBezier=dadesDeLaPeca.value(3).toFloat();
    angleInicial=dadesDeLaPeca.value(4).toFloat();
    angleCentreArc=dadesDeLaPeca.value(5).toFloat();
    numeroVertex=dadesDeLaPeca.value(6).toInt();
    modulsVertex.resize(numeroVertex);
    anglesVertex.resize(numeroVertex);
    for(int i=0;i<numeroVertex;i++){
     modulsVertex.replace(i,dadesDeLaPeca.value(7+i).toFloat());
     anglesVertex.replace(i,dadesDeLaPeca.value(7+numeroVertex+i).toFloat());
    }
    if (ambArc){
        angleArc=angleArcPeca();}
}

/**
  * Resta als anglesVertex el valor actual de
  * angleRotacio amb la qual cosa, la Peça es queda
  * de la mateixa manera en que està quan es crea
  * l'objecte
  */
void Peca::reiniciaAnglesVertex(){
	//la simetria a false
       simetriaBool=false; 
       //reiniciam els valors dels anglesVertex
       anglesInicialsPeca();
       //reiniciam angleRotacio
       setAngleRotacio(0.0);	
}

/* Generació de figures a l'atzar
 */

/* Es una variant de aferraPeca() però treballa nomès amb
 * les peces ja col·locades per generar una figura nova
 */
QPointF Peca::generaFiguraAtzar_aferraPeca(QList<int> indexPecesColocades,double minDist, bool continua){
    QVector<QPointF> aquestaPeca;
    QVector<QPointF> altraPeca;
    QPointF desplacament, despActual;
    float distancia, distanciaActual;
    puntsVertexPeca(aquestaPeca);
    desplacament=QPointF(0,0);
    bool aferrada=false;
    for(int i=0;i<indexPecesColocades.size();i++){
       if(!(this->indexEnElConjunt ==
            conjuntDePeces->arrayPeces[indexPecesColocades.value(i)]->indexEnElConjunt) ){
      //  conjuntDePeces->arrayPeces[i]->puntsVertexPeca(altraPeca);
       conjuntDePeces->arrayPeces[indexPecesColocades.value(i)]->puntsVertexPeca(altraPeca);
           distancia=10000;
           for(int j=0;j<=aquestaPeca.size()-1;j++){
               for(int k=0;k<=altraPeca.size()-1;k++){
                   despActual=aquestaPeca[j]-altraPeca[k];
                   distanciaActual=sqrt(pow(despActual.x(),2)+pow(despActual.y(),2));
              /* 20/03/14 Afegit && !pecaSuperposada(false): alguns casos podian quedar peces
               * superposades
               */
                 //  if( (distanciaActual<distancia) && (distanciaActual<=10.0)
                   if( (distanciaActual<distancia) && (distanciaActual<=minDist)
                           && !pecaSuperposada(indexPecesColocades)){
                      distancia=distanciaActual;
                      desplacament=despActual;
                      minDist=distanciaActual;
                      aferrada=true;
                   }
               }
           }
}
       }

 if(aferrada){
     return desplacament;
   }
 else if(continua) return generaFiguraAtzar_aferraPeca2(indexPecesColocades);
 return desplacament;

 //return generaFiguraAtzar_aferraPeca2(indexPecesColocades);
}

QPointF Peca::generaFiguraAtzar_aferraPeca2(QList<int> indexPecesColocades){
    QPointF desplacament, despActual;
    float distancia, distanciaActual,distanciaRecorreguda;

    desplacament=QPointF(0,0);
    distanciaActual=0.0;
    distanciaRecorreguda=0.0;
    QPointF posicioInicial=pos();
    for(int i=0;i<indexPecesColocades.size();i++){
        float vector=0.0;
        int valor=0;
        distanciaRecorreguda=0.0;
        distancia=100000;
        QPointF vectorCentres=(conjuntDePeces->arrayPeces[indexPecesColocades.value(i)]->centreRectanglePeca()+
                              conjuntDePeces->arrayPeces[indexPecesColocades.value(i)]->pos())-
                              (centreRectanglePeca()+pos());
        //if(indexEnElConjunt != conjuntDePeces->arrayPeces[indexPecesColocades.value(i)]->indexEnElConjunt){
         //   while( (distanciaRecorreguda<8.0) &&  !pecaSuperposada(false) ){
          int contador=0;
          while( (distanciaRecorreguda<10.0) &&  !pecaSuperposada(indexPecesColocades)
                 && contador<200){
            setPos(pos()+vectorCentres*vector);
            distanciaRecorreguda=sqrt(pow((posicioInicial-pos()).x(),2)+pow((posicioInicial-pos()).y(),2));
            valor++;
            vector=((float) valor)/((float) 10000);
            contador++;

            //qDebug("while generaFiguraAtzar_aferraPeca2");
           }
           // if(pecaSuperposada(false)){
            if(pecaSuperposada(indexPecesColocades)){
                //El 0.90 per evitar que quedin una mica superposades
                //Això també depén del 10000 del vector
                despActual=(posicioInicial-pos())*0.85;
                distanciaActual=sqrt(pow(despActual.x(),2)+pow(despActual.y(),2));
            }
      //}//final if del index del conjunt
    setPos(posicioInicial);
    if(distanciaActual<distancia){
      desplacament=despActual ;
      distancia=distanciaActual;
     }
    }//final for de les peces del conjunt
   return desplacament;
}

/* Aferra la peça a la més pròxima de les peces ja col·locades
 * Es fa una aproximació lenta
 */
bool Peca::generaFiguraAtzar_aferraPecaLentament(QList<int> indexPecesColocades, bool continua){
    bool fet=false;
    if (generaFiguraAtzar_aferraPeca(indexPecesColocades,distanciaAferraPeca2,continua) !=
         QPointF(0,0)){
    int contador=0;
    int sentitRotacio=pow(-1,qrand()% 2);
   //while( contador<200){
   while( !pecaSuperposadaEscala(indexPecesColocades,1.01) && contador<200){
        /* !pecaSuperposada(indexPecesColocades) ){// && contador<100){*/
        generaFiguraAtzar_mouPeca(pos()-0.01*generaFiguraAtzar_aferraPeca(
                                     indexPecesColocades,distanciaAferraPeca2,continua),
                                     indexPecesColocades,sentitRotacio);

        //pausa(2);
        if(pecaSuperposadaEscala(indexPecesColocades,1.01)){
       // if(pecaSuperposada(indexPecesColocades)){
            contador=900;
            //qDebug("superposada");
        }
        contador++;
        //qDebug("aferra %d",contador);
    }
   fet=true;
   }
return fet;
}

/* Separa lentament la peça de les peces ja col·locades
 */
bool Peca::generaFiguraAtzar_separaPecaLentament(QList<int> indexPecesColocades,
                                                 QPointF vectorMoviment){
    movimentPeca=1;
   // int contador=0;
    QPointF darreraPosicio;
   // while(pecaSuperposada(indexPecesColocades) && contador<100){
    while (pecaSuperposadaEscala(indexPecesColocades,0.99)){
       setPos(pos()-0.01*vectorMoviment);
       if(darreraPosicio !=pos()){
       darreraPosicio=pos();}
     //  contador++;
       //qDebug("separa %d",contador);
    }
    setPos(darreraPosicio);
    //pausa(5);
return true;
}


/* Mou la peça al punt passat per paràmetre seguint
 * el procediment del moviment amb el ratolí
 */
bool Peca::generaFiguraAtzar_mouPeca(QPointF punt, QList<int> indexPecesColocades, int sentitRotacio){
   bool valorRetorn=true;
    /* La peça es pot moure però no està activat
     * el gir tàctil
     * 10/09/15 El paràmetre movimentPecaTactil ja no es considera
     * (per a les peces de mostra no te cap sentit fer-ho servir)
     */
    if ( (movimentPeca==1) ){  //&& !movimentPecaTactil ){
        QPointF darreraPos=pos();
        setPos(punt);

        /* 15/10/11
        afegit el control de superposició
        */
        if(!pecaEnLimits() || pecaSuperposada(indexPecesColocades) ){
        /*Com què la peça està superposada o fora dels límits
         *es desfà el moviment i es permet que la peça giri
         */
           setPos(darreraPos);
           movimentPeca=2;
         }
      }
    /*  Aquesta opció comentada no te cap sentit
     */
    //else if (  ((movimentPeca==2) || (movimentPecaTactil)) &&  (angleGirPeca==ROTACIO_EXPERT) ) {
   // else if (  (movimentPeca==2)  &&  (angleGirPeca==ROTACIO_EXPERT) ) {
        // rotaPeca(angleGirPeca*PI/180*sentitRotacio);
        /* 15/10/11
        afegit el control de superposició */
      //  if(!pecaEnLimits() || pecaSuperposada(indexPecesColocades) ){
       //    rotaPeca(-angleGirPeca*PI/180*sentitRotacio);
       //    valorRetorn=false;
       //  };
       // movimentPeca=1;
      // } // final else if moviment expert

      /* 15/10/11
         Afegit això per permetre que les peces girin automàticament
         quan xoquin
        */
   else if (  (movimentPeca==2) ){ // && (angleGirPeca != ROTACIO_EXPERT) ){  //&& !movimentPecaTactil){
            rotaPeca(angleGirPeca*PI/180*sentitRotacio);
          //comprova si la peça està en els limits de la zona de joc
          /* 15/10/11
             afegit el control de superposició */
          if(!pecaEnLimits()  || pecaSuperposada(indexPecesColocades)) {
              rotaPeca(-angleGirPeca*PI/180*sentitRotacio);
              valorRetorn=false;
          }
         // movimentPeca=1;
         // movimentPecaTactil=false;
      }
 return valorRetorn;
}

/*
  Retorna les dades de la peça
  */
QString Peca::recuperaDadesPeca(){
 /* Dades:
    - ambArc (1)
    - centreArc
    - modulMesGran
    - ajustBezier
    - angleInicial //el primer angle vertex
    - angleCentreArc
    - numeroVertex
    - modulsVertex
    - anglesVertex

    NOU TANGRAM 6
  */
 QStringList dades;
//## 1
dades.append("0;0;0;1;0.785398;0;4;0.707107;0.707107;0.707107;0.707107;0.785398;2.35619;-2.35619;-0.785398");
//## 2
dades.append("0;0;0;1;0.463648;0;3;0.745356;0.471405;0.745356;0.463648;2.35619;4.24874");
//## 3
dades.append("0;0;0;1;0.463648;0;3;1.05409;0.666667;1.05409;0.463648;2.35619;4.24874");
//## 4
dades.append("0;0;0;1;0.463648;0;3;1.49071;0.942809;1.49071;0.463648;2.35619;4.24874");
//## 5
dades.append("0;0;1;1;1.5708;0;4;0.5;1.11803;0.5;1.11803;1.5708;2.67795;4.71239;5.81954");
//## 6
dades.append("0;0;1;1;1.5708;0;3;1;1.58114;1.58114;1.5708;-0.321751;3.46334");
//## 7
dades.append("0;0;1;1;1.5708;0;4;0.707107;1.58114;0.707107;1.58114;1.5708;2.67795;4.71239;5.81954");
//## 8
dades.append("0;0;0;1;0.785398;0;4;1;1;1;1;0.785398;2.35619;3.92699;5.49779");
//## 9
dades.append("0;0;1;1;1.5708;0;4;0.666667;1.49071;0.666667;1.49071;1.5708;2.67795;4.71239;5.81954");
//## 10
dades.append("0;0;0;1;0.463648;0;3;0.702728;0.444444;0.702728;0.463648;2.35619;4.24874");
//## 11
dades.append("0;0;0;1;0.463648;0;3;1.40546;0.888889;1.40546;0.463648;2.35619;4.24874");
//## 12
dades.append("0;0;1;1;1.5708;0;4;0.707107;1.58114;0.707107;1.58114;1.5708;2.67795;4.71239;5.81954");
//## 13
dades.append("0;0;0;1;0.463648;0;4;1.11803;1.11803;0.5;1.11803;0.463648;2.67795;4.71239;5.81954");
//## 14
dades.append("0;0;0;1;-2.81984;0;4;1.58114;1.58114;0.707107;1.58114;-2.81984;-0.321751;0.785398;2.81984");
//## 15
dades.append("0;0;0;1;0.463648;0;4;0.790569;0.790569;0.353553;0.790569;0.463648;2.67795;4.71239;5.81954");
//## 16
dades.append("0;0;3;1;0.463648;0;5;0.790569;1.06066;0.790569;1.27475;1.27475;0.463648;1.5708;2.67795;4.12439;5.30039");
//## 17
dades.append("0;0;2;1;0.785398;0;4;0.707107;0.707107;1.58114;1.58114;0.785398;2.35619;3.46334;5.96143");
//## 18
dades.append("0;0;0;1;1.5708;0;3;0.816497;0.816497;0.816497;1.5708;3.66519;5.75959");
//## 19
dades.append("0;0;0;1;1.85183;0;3;0.849837;0.471405;0.62361;1.85183;4.18879;5.56946");
//## 20
dades.append("0;0;1;1;2.0944;0;4;0.707107;1.22474;0.707107;1.22474;2.0944;3.66519;5.23599;0.523599");
//## 21
dades.append("0;0;0;1;1.5708;0;3;1.1547;1.1547;1.1547;1.5708;3.66519;5.75959");
//## 22
dades.append("0;0;0;1;1.5708;0;3;0.583095;0.583095;0.583095;1.5708;3.66519;5.75959");
//## 23
dades.append("0;0;1;1;2.0944;0;4;0.5;0.866025;0.5;0.866025;2.0944;3.66519;5.23599;0.523599");
//## 24
dades.append("0;0;0;1;1.85183;0;3;1.20185;0.666667;0.881917;1.85183;4.18879;5.56946");
//## 25
dades.append("0;0;1;1;1.5708;0;3;0.333333;0.881917;0.881917;1.5708;3.33172;6.09306");
//## 26
dades.append("0;0;0;1;0.463648;0;3;0.527046;0.333333;0.527046;0.463648;2.35619;4.24874");
//## 27
dades.append("0;0;2;1;0.463648;0;4;0.559017;0.559017;1.03078;1.03078;0.463648;2.67795;3.38657;6.03821");
//## 28
dades.append("0;0;2;1;2.13727;0;3;1.58016;1.07869;1.82235;2.13727;3.80783;5.90866");
//## 29
dades.append("0;0;2;1;2.13727;0;3;1.24224;0.848013;1.43264;2.13727;3.80783;5.90866");
//## 30
dades.append("0;0;2;1;2.13727;0;3;0.976592;0.666667;1.12627;2.13727;3.80783;5.90866");
//## 31
dades.append("0;0;1;1;2.0944;0;4;1;1.73205;1;1.73205;2.0944;3.66519;5.23599;0.523599");
//## 32
dades.append("0;0;1;1;2.0944;0;4;0.5;0.866025;0.5;0.866025;2.0944;3.66519;5.23599;0.523599");
//## 33  vuit elements: trapezi gran
dades.append("0;0;1;1;0,281034902;0;4;1.5612495;2.046338193;2.046338193;1.5612495;0,281034902;6.0699698;3.3548082;2.8605578");
//## 34
dades.append("0;0;0;1;1.5708;0;3;0.583095;0.583095;0.583095;1.5708;3.66519;5.75959");
//## 35
dades.append("0;0;0;1;1.0472;0;6;1;1;1;1;1;1;1.0472;2.0944;3.14159;4.18879;5.23599;0");
//## 36
dades.append("0;0;2;1;0.713724;0;4;0.661438;0.661438;1.08972;1.08972;0.713724;2.42787;3.55023;5.87455");
//## 37
dades.append("1;2.51564;0;1.5;0.20422;1.85572;3;1.44423;0.765367;0.91823;0.20422;2.74889;3.83343");
//## 38
dades.append("1;0.541196;1;1.2;2.07127;4.31969;3;0.431615;0.541196;0.431615;2.07127;4.31969;0.284924");
//## 39
dades.append("0;0;0;1;0.463648;0;3;1.11803;0.707107;1.11803;0.463648;2.35619;4.24874");
//## 40
dades.append("1;0.75;0;1.6;2.85667;4.71239;4;1.10522;0.538079;0.538079;1.10522;2.85667;3.75707;5.66771;0.284924");
//## 41
dades.append("0;0;1;1;1.5708;0;3;0.420394;0.654929;0.654929;1.5708;3.46334;5.96143");
//## 42
dades.append("1;0.471405;0;1.6;0.463648;2.35619;3;0.760117;0.471405;0.760117;0.463648;2.35619;4.24874");
//## 43
dades.append("1;0.61592;1;1.3;2.07127;4.31969;3;0.491209;0.61592;0.491209;2.07127;4.31969;0.284924");
//## 44
dades.append("0;0;2;1;2.17455;0;3;0.480704;0.330509;0.579751;2.17455;3.7296;5.96143");
//## 45
dades.append("0;0;2;1;1.81578;0;3;0.755903;0.819892;0.98728;1.81578;3.60524;5.90268");
//## 46
dades.append("0;0;2;1;2.35619;0;3;0.777817;0.614919;1.13385;2.35619;3.60524;6.03821");
//## 47
dades.append("0;0;2;1;1.81578;0;3;0.755903;0.502079;1.11893;1.81578;2.76109;5.32312");
//## 48
dades.append("0;0;2;1;2.03444;0;3;0.409946;0.755903;0.93482;2.03444;3.38657;6.08579");
//## 49
dades.append("0;0;2;1;2.35619;0;3;0.259272;0.73904;0.921239;2.35619;3.26595;6.18352");
//## 50
dades.append("0;0;0;1;1.44644;0;3;0.73904;0.586953;0.518545;1.44644;3.81633;5.49779");
//## 51
dades.append("0;0;2;1;2.67795;0;3;0.819892;1.29636;2.02498;2.67795;3.28349;6.19253");
//## 52
dades.append("0;0;2;1;2.35619;0;3;0.777817;1.1;1.73925;2.35619;3.14159;5.96143");
//## 53
dades.append("0;0;2;1;1.69515;0;5;1.10856;0.6875;1.1748;1.1748;0.6875;1.69515;2.2143;4.35362;5.07116;0.927295");
//## 54
dades.append("0;0;3;1;2.49809;0;4;0.6875;1.10856;0.6875;1.1748;2.49809;3.01724;3.78509;5.92441");
//## 55
dades.append("0;0;3;1;1.5708;0;4;0.869626;0.869626;1.16673;1.16673;1.5708;2.81984;3.92699;5.49779");
//## 56
dades.append("0;0;0;1;1.91063;0;4;1.5;1.5;0.649286;1.5;1.91063;1.23096;5.59135;4.37255");
//## 57 vuit elements: trapezi gran
dades.append("0;0;1;1;0.4086378;0;4;1.0897247;1.5612495;1.5612495;1.0897247;0.4086378;6.0021504;3.4226276;2.7329548");
//## 58
dades.append("0;0;0;1;2.18628;0;4;0.866025;0.866025;0.579471;0.866025;2.18628;0.955317;0.529903;4.09691");
//## 59
dades.append("0;0;0;1;1.91063;0;5;1.5;0.649286;1.11803;0.736813;1.5;1.91063;6.97502;5.17604;4.99731;4.37255");
//## 60
dades.append("0;0;1;1;2.67795;0;3;0.745356;1.37437;0.942809;2.67795;0.244979;3.92699");
//## 61
dades.append("1;1.76383;2;2;2.61799;4.37892;3;0.666667;0.881917;1.33333;2.61799;3.99866;5.75959");
//## 62
dades.append("1;1.41139;1;0.95;0.938882;3.63354;3;0.826209;1.41139;0.826209;0.938882;2.64964;5.8679");
//## 63
dades.append("1;1.41421;0;2;2.35619;3.92699;3;1.41421;1;1.43178;2.35619;3.14159;5.48544");
//## 64
dades.append("0;0;0;1;2.67795;0;4;1.11803;1.11803;0.619657;0.619657;2.67795;0.463648;5.65127;3.77351");
//## 65
dades.append("0;0;0;1;2.80812;0;4;1.32288;0.866025;1.32288;0.866025;2.80812;0.523599;5.94971;3.66519");
//## 66
dades.append("0;0;2;1;2.61799;0;5;1.1547;1.1547;1.52753;0.57735;1.1547;2.61799;0.523599;6.09306;5.75959;4.71239");
//## 67
dades.append("1;1;2;1;2.0944;5.67729;3;0.666667;0.881917104;1.201850425;2.0944;0.713724;4.43135");
//## 68
dades.append("1;0.25;0;1;1.85183;0;3;0.901388;0.25;0.901388;1.85183;0;4.43135");
//## 69
dades.append("1;0.5;0;1.2555;2.86056;1.5708;2;1.55121;1.55121;2.86056;0.281035");
//## 70
dades.append("0;0;0;1;2.58299;0;5;1.33417;0.761577;0.761577;1.33417;0.424264;2.58299;1.19029;5.0929;3.70019;3.14159");
//## 71
dades.append("0;0;3;1;2.58299;0;5;1.33417;0.761577;0.761577;1.47648;0.424264;2.58299;1.19029;5.0929;4.42093;3.14159");
//## 72
dades.append("0;0;3;1;2.58299;0;6;1.33417;0.761577;0.761577;1.47648;1.33417;0.424264;2.58299;1.19029;5.0929;4.42093;3.70019;3.14159");
//## 73
dades.append("0;0;0;1;2.89661;0;4;1.45774;1.45774;0.790569;0.790569;2.89661;0.244979;5.81954;3.60524");
//## 74
dades.append("0;0;1;1;2.67795;0;3;0.527046;0.971825;0.666667;2.67795;0.244979;3.92699");
//## 75
dades.append("0;0;1;1;2.67795;0;3;1.17851;2.17307;1.49071;2.67795;0.244979;3.92699");
//## 76
dades.append("0;0;0;1;2.35619;0;4;1;1;0.707107;1;2.35619;0.785398;0;3.92699");
//## 77
dades.append("0;0;1;1;2.89661;0;6;1.30384;1.92354;1.58114;0.316228;1.70294;1.78885;2.89661;0.165149;5.63968;4.71239;4.33188;3.92699");
//## 78
dades.append("0;0;0;1;2.35619;0;4;1;1;1;1;2.35619;0.785398;5.49779;3.92699");
/*27/04/14 Recalculades les dues peces: les dades no estaven
 * "centrades" correctament. Ha calgut refer totes les figures del tangram
 * "peces del nou" TIPUS_TANGRAM_T9T codi 28*/
//## 79
//dades.append("0;0;0;1;2.42787;0;3;0.661438;0.661438;0.433013;2.42787;0.713724;4.71239");
dades.append("0;0;0;1;1.57079;0;3;0.577350;0.577350;0.577350;1.57079;3.66519;5.75958");
//## 80
//dades.append("0;0;0;1;2.42787;0;3;0.992157;0.992157;0.649519;2.42787;0.713724;4.71239");
dades.append("0;0;0;1;1.57079;0;3;0.866025;0.8660257;0.866025;1.57079;3.66519;5.75958");
//## 81
dades.append("0;0;1;1;2.61799;0;3;0.333333;0.600925;0.440959;2.61799;0.281035;3.99866");
//## 82
dades.append("0;0;1;1;2.61799;0;3;0.666667;1.20185;0.881917;2.61799;0.281035;3.99866");
//## 83
dades.append("0;0;1;1;2.61799;0;3;0.57735;1.04083;0.763763;2.61799;0.281035;3.99866");
//## 84
dades.append("0;0;0;1;2.73295;0;4;0.544862;0.544862;0.330719;0.330719;2.73295;0.408638;5.56946;3.85532");
//## 85
dades.append("0;0;0;1;2.55359;0;4;1.27475;0.790569;1.06066;1.27475;2.55359;1.10715;0;3.7296");
//## 86
dades.append("0;0;0;1;2.35619;0;4;1.5;1.11803;1.5;1.11803;2.35619;1.24905;5.49779;3.46334");
//## 87
dades.append("0;0;0;1;2.67795;0;4;1.58114;1.58114;1.41421;0.707107;2.67795;0.463648;0;4.71239");
//## 88
dades.append("0;0;0;1;2.2143;0;4;1.17851;0.849837;0.971825;0.849837;2.2143;5.69518;4.95737;3.7296");
//## 89
dades.append("0;0;1;1;2.81984;0;3;0.702728;1.35172;0.801234;2.81984;0.165149;3.7296");
//## 90
dades.append("0;0;1;1;2.89661;0;3;1.29577;1.60247;0.702728;2.89661;0.197396;4.24874");
//## 91
dades.append("0;0;0;1;2.76109;0;3;0.846197;0.702728;0.647884;2.76109;0.463648;4.95737");
//## 92
dades.append("0;0;2;1;2.35619;0;5;1;1;1.20185;1;1;2.35619;0.785398;0.197396;5.49779;3.92699");
//## 93
dades.append("0;0;1;1;2.89661;0;3;0.647884;0.801234;0.351364;2.89661;0.197396;4.24874");
//## 94
dades.append("0;0;1;1;2.86329;0;3;1.14396;1.75682;0.888889;2.86329;0.179853;3.92699");
//## 95
dades.append("0;0;2;1;1.5708;0;3;0.942809;1.05409;1.69967;1.5708;0.463648;4.12439");
//## 96  Triangle petit tangram 12 peces
dades.append("0;0;1;1;1.5707963;0;3;0.1666666;0.2635231;0.2635231;1.5707963;5.9614348;3.4633432");
//## 97  Trapezi regular tangram 12 peces
dades.append("0;0;1;1;0.9827937;0;4;0.4506939;1.0680005;1.0680005;0.4506939;0.9827937;5.9244146;3.5003633;2.1587989");
//## 98  Pentagon tangram 5 II
dades.append("0;0;1;1;2.0344439;0;5;1.118034;1.8027756;1.118034;0.5;1.5;2.0344439;5.6951827;4.2487454;3.1415927;3.1415927");
//## 99  Trapezi rectangular petit-llarg tangram Armonigrama
dades.append("0;0;1;1;0.4636476;0;4;0.7905694;1.457738;0.7905694;0.7905694;0.4636476;6.0382066;3.6052403;2.6779451");
return dades.value(tipusPeca-1);
}
