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
#include <math.h>

#include <QtWidgets/QGraphicsScene>
#include "conjuntpeces.h"
#include "constants.h"
#include "funcions.h"


ConjuntPeces::ConjuntPeces()
{
   // arrayPeces.resize(0);
    escala=ESCALA_GENERAL;
    //cream el tangram clàssic
    creaConjunt(1,55.0);
}


ConjuntPeces::ConjuntPeces(int tipusTangram)
{
   // arrayPeces.resize(0);
    escala=ESCALA_GENERAL;
    creaConjunt(tipusTangram,55.0);
}


ConjuntPeces::ConjuntPeces(int tipusTangram,double pescala){
   // arrayPeces.resize(0);
    escala= pescala;
    creaConjunt(tipusTangram,escala);
}

/*
Col·loca les peces a l'atzar
*/
void ConjuntPeces::colocaPecesAtzar(){
 for(int i=0;i<=arrayPeces.size()-1;i++){
       arrayPeces[i]->colocaPecaAtzar();
       arrayPeces[i]->setSelected(false);
       }
}

/*
Col·loca les peces segons les dades passades
*/
void ConjuntPeces::colocaPeces(QString dades){
  if (dades.isNull() || dades.isEmpty()){
      return;
  }
  else {
  QStringList list1 = dades.split(" ");
  int tPeca,simPeca;
  float pX,pY,angle;
  float pescala=list1.at(0).toFloat()/escala;

    for(int i=0;i<=arrayPeces.size()-1;i++){
      tPeca=list1.at(1+i*5).toInt();
      pX=list1.at(2+i*5).toFloat()/pescala;
      pY=list1.at(3+i*5).toFloat()/pescala;
      angle=list1.at(4+i*5).toFloat();
      simPeca=list1.at(5+i*5).toInt();

     //arrayPeces[i]->creaPeca(tPeca);
     // 25/02/11 Canvis en la creació de peces
    // arrayPeces[i]->creaPeca(tPeca,dadesPeces.value(tPeca-1));
     arrayPeces[i]->creaPeca(tPeca);
     arrayPeces[i]->setPos(pX,pY);
     if (simPeca==1){
        arrayPeces[i]->simetriaPeca();
        }
     arrayPeces[i]->setAngleRotacio(angle);
     arrayPeces[i]->actualitzaPeca();
    }
   centraFiguraScene();
  }
}

void ConjuntPeces::colocaPeca(QString dades, int indexPeca,QPointF punt){
if (dades.isNull() || dades.isEmpty()){
      return;
  }
  else {
  QStringList list1 = dades.split(" ");
  int tPeca,simPeca;
  float pX,pY,angle;
      tPeca=list1.at(0).toInt();
      pX=punt.x()+list1.at(1).toFloat();
      pY=punt.y()+list1.at(2).toFloat();
      angle=list1.at(3).toFloat();
      simPeca=list1.at(4).toInt();
     // 25/02/11 Canvis en la creació de peces
    // arrayPeces[indexPeca]->creaPeca(tPeca,dadesPeces.value(tPeca-1));
     arrayPeces[indexPeca]->creaPeca(tPeca);
     arrayPeces[indexPeca]->setPos(pX,pY);
     if (simPeca==1){
        arrayPeces[indexPeca]->simetriaPeca();
        }
     arrayPeces[indexPeca]->setAngleRotacio(angle);
     arrayPeces[indexPeca]->actualitzaPeca();
  }
}
/*
  Recol·loca el conjunt de peces fent coincidir el seu
  vertexSuperiorEsquerra amb el punt passat per paràmetre
  */
void ConjuntPeces::recolocaPeces(QPointF punt){
    QPointF vertexSupEsq=vertexSuperiorEsquerra();
    for(int i=0;i<=arrayPeces.size()-1;i++){
      arrayPeces[i]->setPos(arrayPeces[i]->pos()-(vertexSupEsq-punt));
    }
}


void ConjuntPeces::simetriaVertical(){
//Cal assegurar-se que la figura està centrada
centraFiguraScene();
float valor=arrayPeces[0]->scene()->height()/2 ;
for(int i=0;i<=arrayPeces.size()-1;i++){
    arrayPeces[i]->setPos(2*valor-arrayPeces[i]->pos().x(),
                                      arrayPeces[i]->pos().y());
    arrayPeces[i]->simetriaPeca();
    arrayPeces[i]->aferraPeca2();
  }
centraFiguraScene();
}

void ConjuntPeces::simetriaHoritzontal(){
//Cal assegurar-se que la figura està centrada
centraFiguraScene();
float valor=arrayPeces[0]->scene()->height()/2 ;
for(int i=0;i<=arrayPeces.size()-1;i++){
    arrayPeces[i]->setPos(arrayPeces[i]->pos().x(),
                                      2*valor-arrayPeces[i]->pos().y());
    arrayPeces[i]->rotaPeca(PI);
    arrayPeces[i]->simetriaPeca();
    arrayPeces[i]->aferraPeca2();
  }
centraFiguraScene();
}


/**
* Retorna una cadena amb les coordenades de les peces
* per tal de desar-les a un arxiu de text
*/
QString ConjuntPeces::coordenadesFigura(){
  QPointF verSuperiorEsquerra;
  verSuperiorEsquerra=vertexSuperiorEsquerra();
  QString dadesFigura=QString("%1 ").arg(escala);
  for(int i=0;i<=arrayPeces.size()-1;i++){
    dadesFigura=dadesFigura+
        arrayPeces[i]->cadenaPropietatsPeca(verSuperiorEsquerra);
    }
 return dadesFigura;
}

/**
* Retorna les coordenades (en forma de Point), del vèrtex
* superior-esquerra de les Peces passades
* per paràmetre
*/
QPointF ConjuntPeces::vertexSuperiorEsquerra(){
  QPointF verSuperiorEsquerra(10000.0,10000.0);
  QVector<QPointF> pSolucio;
  for(int i=0;i<=arrayPeces.size()-1;i++){
     arrayPeces[i]->puntsSolucio(pSolucio,1.0);
     for(int j=0;j<=pSolucio.size()-1;j++){
        verSuperiorEsquerra.setX(qMin(pSolucio[j].x(),
                                   verSuperiorEsquerra.rx()));
        verSuperiorEsquerra.setY(qMin(pSolucio[j].y(),
                                   verSuperiorEsquerra.ry()));
     }
  }
      return verSuperiorEsquerra;
 }


QPointF ConjuntPeces::vertexInferiorDreta(){
  QPointF verInferiorDreta(0.0,0.0);
  QVector<QPointF> pSolucio;
  for(int i=0;i<=arrayPeces.size()-1;i++){
     arrayPeces[i]->puntsSolucio(pSolucio,1.0);
     for(int j=0;j<=pSolucio.size()-1;j++){
        verInferiorDreta.setX(qMax(pSolucio[j].x(),
                                   verInferiorDreta.rx()));
        verInferiorDreta.setY(qMax(pSolucio[j].y(),
                                   verInferiorDreta.ry()));
     }
  }
 return verInferiorDreta;
 }

 /*
* Centra la figura en el panell
*/
void ConjuntPeces::centraFiguraScene(){
    QPointF centreScene=arrayPeces[0]->scene()->sceneRect().center();
    QPointF centreFigura=QRectF(vertexSuperiorEsquerra(),vertexInferiorDreta()).center();
    QPointF vectorPosicio=centreScene-centreFigura;
    for(int i=0;i<=arrayPeces.size()-1;i++){
        arrayPeces[i]->setPos(arrayPeces[i]->scenePos()+vectorPosicio);
    }
}

QRectF ConjuntPeces::rectangleFigura(){
 // QRectF rectFigura=QRectF(vertexSuperiorEsquerra(),vertexInferiorDreta());
  return QRectF(vertexSuperiorEsquerra(),vertexInferiorDreta());
}


/*
Color de les peces del conjunt
*/
void ConjuntPeces::colorPecesConjunt(QColor color){
  for(int i=0;i<=arrayPeces.size()-1;i++){
       arrayPeces[i]->setColorPeca(color);
       arrayPeces[i]->setColorLimits(color);
       }
}

/*
Fa les tasques necessàries pel conjunt de peces de mostra
*/
void ConjuntPeces::conjuntDeMostra(){
   //posam el color a negre
 for(int i=0;i<=arrayPeces.size()-1;i++){
       arrayPeces[i]->setColorPeca(Qt::black);
       //arrayPeces[i]->setColorPeca(Qt::white);
       arrayPeces[i]->setColorLimits(Qt::black);
       /*20/12/11
         Afegit per evitar que es mostri la solució quan es genera una
         figura després de mostrar la solució d'una altra figura generada
         */
       arrayPeces[i]->solucio=false;
       arrayPeces[i]->pecaMostra=true;
       arrayPeces[i]->setZValue(-1);
       }
}

/* Fa que les peces del conjunt estiguin
 * per sobre de les peces d'altres conjunts*/
void ConjuntPeces::conjuntSuperior(){
    for(int i=0;i<=arrayPeces.size()-1;i++){
     arrayPeces[i]->setZValue(10);
    }

}

/*
Mostra la solució
*/
void ConjuntPeces::conjuntSolucio(){
 conjuntVisible(true);
 centraFiguraScene();
 for(int i=0;i<=arrayPeces.size()-1;i++){
       arrayPeces[i]->solucio=true;
       arrayPeces[i]->actualitzaPeca();
       }
}

/*
Posa la figura (resolta) en color verd
*/
void ConjuntPeces::conjuntFiguraResolta(){
 for(int i=0;i<=arrayPeces.size()-1;i++){
       arrayPeces[i]->setColorPeca(Qt::green);
       arrayPeces[i]->setColorLimits(Qt::green);
       }
}
/*
Controla si el conjunt és visible
*/
void ConjuntPeces::conjuntVisible(bool visible){
 for(int i=0;i<=arrayPeces.size()-1;i++){
       arrayPeces[i]->pecaVisible=visible;
       arrayPeces[i]->actualitzaPeca();
       }
}


/*
Crea un conjunt segons els paràmetres passats
*/
void ConjuntPeces::creaConjunt(int ptipusTangram, double pescala){
  tipusTangram=ptipusTangram;
  QStringList dadesTangram;
  /* 14/02/11
     Llista amb les dades per a la construcció de cada joc de tangram
     -L'ordre a la llista és el codi del tangram-1
     -Nombre de peces del joc
     -Codis de cada peça
   Cal tenir present que l'ordre de creación de les peces és important
   que coincideixi amb l'ordre en què estan a l'arxiu de dades de
   les figures.
    NOU TANGRAM 3 */
  //dadesTangram.append("2,14,13");
  /*Cal tenir en compte que quan es carrega una figura, es tornen a crear totes les peces segons
    els codis de tipus de peça que hi ha a les dades de les figures*/
  dadesTangram.append("7,1,2,2,3,4,4,5"); //tangram clàssic                                       codi 1
  dadesTangram.append("5,7,2,4,6,6"); //Tangram 5                                                 codi 2
  dadesTangram.append("6,10,3,3,11,8,9"); //Tangram 6 I                                           codi 3
  dadesTangram.append("7,12,1,2,2,3,3,8"); //Tangram de Fletcher (Pitàgores)                      codi 4
  dadesTangram.append("7,3,3,13,14,15,15,16"); //Tangram del xinès també Tangram Pitagòric    codi 5
  dadesTangram.append("7,17,1,2,3,3,5,13"); //Tangram de la saviesa Chie-No-Ita                   codi 6
  dadesTangram.append("8,1,2,2,3,3,3,13,14"); //Tangram 8                                         codi 7
  dadesTangram.append("9,18,18,18,19,19,19,19,20,20"); //Tangram 9                                codi 8
  dadesTangram.append("10,21,21,22,22,23,23,1,2,2,17"); //Tangram 10                              codi 9
  dadesTangram.append("11,24,24,25,25,23,23,23,22,22,22,22"); //Tangram 11                        codi 10
  dadesTangram.append("12,1,2,2,2,3,13,15,26,26,26,27,27"); //Tangram rus                         codi 11
  dadesTangram.append("13,1,1,1,2,2,2,2,2,2,2,2,2,2"); //Tangram 13                               codi 12
  dadesTangram.append("14,1,1,2,2,2,2,3,3,4,4,4,4,5,5"); //Tangram 14                             codi 13
  dadesTangram.append("3,28,29,30"); //Tangram de Brügner                                         codi 14
  dadesTangram.append("6,31,32,34,35,36,21"); //Tangram Hexagrama                                 codi 15
  dadesTangram.append("9,37,37,38,38,39,39,40,40,41"); //Tangram de l'ou                          codi 16
  dadesTangram.append("9,42,42,42,43,43,13,1,2,5"); //Tangram del cor                             codi 17
  dadesTangram.append("14,44,45,45,46,47,48,48,49,50,51,52,53,54,55");
  ///Tangram Stomachion o Luculus Archimedius                              codi 18
  dadesTangram.append("4,2,56,58,59"); //Tangram de 4 peces I                                     codi 19
  dadesTangram.append("10,60,60,60,60,61,61,62,62,63,63"); //Tangram circular I                   codi 20
  dadesTangram.append("7,64,65,66,21,21,24,24"); //Tangram 7 I                                    codi 21
  //dadesTangram.append("8,67,67,67,68,69,65,64,22"); //Tangram circular de 8 peces CII             codi 22
  dadesTangram.append("8,22,64,65,67,67,67,68,69"); //Tangram circular de 8 peces CII             codi 22
  dadesTangram.append("6,70,71,72,1,5,3"); //Tangram 6 II                                         codi 23
  dadesTangram.append("7,73,1,5,3,2,70,13"); //Tangram 7 II Revathi                               codi 24
  dadesTangram.append("5,3,3,13,16,12"); //Tangram 5 I Regulus                                    codi 25
  dadesTangram.append("7,3,3,13,12,2,5,5"); //Tangram 7 Heptex                                    codi 26
  dadesTangram.append("5,78,74,75,76,77"); //Tangram de Sam Lloyds                                codi 27
  dadesTangram.append("9,79,79,80,80,81,82,83,84,84"); //Peces del nou "los nueve tangram"      codi 28
  dadesTangram.append("10,74,74,74,74,74,76,76,76,76,76"); //Tangram Egipci                       codi 29
  //dadesTangram.append("10,76,76,76,76,76,74,74,74,74,74"); //Tangram Egipci
  dadesTangram.append("4,3,85,85,86"); //Tangram de 4 peces II                                    codi 30
  dadesTangram.append("14,87,88,89,90,90,91,91,91,92,93,94,95,3,74");
                        //Tangram Stomachion rectangular                                          codi 31
  dadesTangram.append("11,54,45,46,47,48,46,76,90,53,45,48");
  //Tangram Stomach (modificació de Stomachion)                                                   codi 32
  dadesTangram.append("10,15,15,15,15,3,3,3,3,3,3"); //Tangram Elzzup (10 peces)                  codi 33
  dadesTangram.append("8,31,32,34,35,36,21,33,57"); //Tangram dels vuit elements (8 peces)        codi 34
  dadesTangram.append("12,1,2,2,2,3,13,15,26,96,96,97,97"); //Tangram 12 peces                    codi 35
  dadesTangram.append("5,3,1,16,4,98"); //Tangram 5 II                                            codi 36
  dadesTangram.append("8,3,3,13,14,15,15,99,99"); //Tangram Armonigrama                           codi 37
  dadesTangram.append("8,3,3,13,1,2,2,2,12"); //Tangram rectangular 8 peces                       codi 38
  dadesTangram.append("16,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2");//T. 16 peces (triangles clàssic)     codi 39
  dadesTangram.append("9,31,21,21,23,23,22,22,22,22"); //Peces hexagonal                          codi 40

//dimensionam l'array de les peces
QStringList dadesDelTangram=dadesTangram.value(ptipusTangram-1).split(",");
arrayPeces.resize(dadesDelTangram.value(0).toInt());
//escales especials
if (ptipusTangram==11){pescala=FACTOR_TR*pescala;}  //Tangram rus
if (ptipusTangram==31){pescala=0.9*pescala;}  //Tangram Stomachion rectangular
if (ptipusTangram==28){pescala=FACTOR_T9T*pescala;};//Tangram "los nueve tangram"
if (ptipusTangram==35){pescala=1.5*pescala;};//Tangram 5 II
if (ptipusTangram==38){pescala=1.3*pescala;};//Tangram rectangular

//cream i carregam les peces
for(int i=0;i<=arrayPeces.size()-1;i++){
    //especial per a dues peces de Tangram Stomach
    if(  (ptipusTangram==32) &&   //TIPUS_TANGRAM_STOMACH) &&
       (  (dadesDelTangram.value(i+1).toInt()==76 ) || (dadesDelTangram.value(i+1).toInt()==90 ) )) {
        arrayPeces.replace(i, new Peca(dadesDelTangram.value(i+1).toInt(),escalaStomach*pescala) ) ;
   }
   else
   arrayPeces.replace(i, new Peca(dadesDelTangram.value(i+1).toInt(),pescala) ) ;
}
 //emplenam la propietat conjuntDePeces
 //19/11/11
 // i del nivell del joc per defecte a iniciat
// degut a la funció pecaSuperposada
    for(int i=0;i<=arrayPeces.size()-1;i++){
        arrayPeces[i]->conjuntDePeces=this;
        arrayPeces[i]->indexEnElConjunt=i;
        arrayPeces[i]->nivellJoc=NIVELL_INICIAT;
    }
 }//final creaConjunt

/*
  15/11/11
  Retorna la silueta de la figura formada
  per les peces del conjunt
  (es fa servir per calcular el nivell de dificultad
  de les figures generades automàticament)
  07/10/15 Canviat arrayPeces[t]->shape() a
  arrayPeces[t]->shapeEscala(1.005)
  */
QPainterPath ConjuntPeces::siluetaFigura(){
    QPainterPath siluetaFigura;
    QPainterPath siluetaPeca;
    siluetaFigura.setFillRule(Qt::WindingFill);

    longitudTotalPeces=0.0;
   //Calculam la silueta de la figura
    for(int t=0;t<arrayPeces.size();t++){
        siluetaPeca=arrayPeces[t]->shapeEscala(1.005);
        longitudTotalPeces=longitudTotalPeces+siluetaPeca.length();
        siluetaFigura=siluetaFigura.united(siluetaPeca.translated(arrayPeces[t]->pos()));
        siluetaFigura=siluetaFigura.simplified();
    }
  return siluetaFigura.simplified();
}

QPainterPath ConjuntPeces::siluetaFiguraEscala(double escala){
    QPainterPath siluetaFigura;
    QPainterPath siluetaPeca;
    siluetaFigura.setFillRule(Qt::WindingFill);

    longitudTotalPeces=0.0;
   //Calculam la silueta de la figura
    for(int t=0;t<arrayPeces.size();t++){
        siluetaPeca=arrayPeces[t]->shapeEscala(escala);
       // siluetaPeca=arrayPeces[t]->shape();
        longitudTotalPeces=longitudTotalPeces+siluetaPeca.length();
        siluetaFigura=siluetaFigura.united(siluetaPeca.translated(arrayPeces[t]->pos()));
        siluetaFigura=siluetaFigura.simplified();
    }
  return siluetaFigura.simplified();
}

/*
  Retorna la longitud de la silueta de la figura actual
  07/10/15 canviada l'escala de 1.01 a 1.005
  */
qreal ConjuntPeces::longitudSilueta(){
 // return siluetaFigura().length();
  return siluetaFiguraEscala(1.01).length();
}

/* Retorna una llista amb els punts dels vèrtex
 * de les peces del conjunt*/
QList<QPointF> ConjuntPeces::llistaVertexConjunt(){
  QVector<QPointF> vectorVertex;
  QList<QPointF> llistaVertex;
  for(int t=0;t<arrayPeces.size();t++){
    arrayPeces[t]->puntsVertexPecaReducccio(vectorVertex,1.005);
    for(int n=0;n<vectorVertex.size();n++){
      llistaVertex.append(vectorVertex[n]);
    }
  }
  /*qDebug("llistaVertex del conjunt %d",llistaVertex.count() );
  for(int n=0;n<llistaVertex.count();n++){
    qDebug("(%f , %f)",llistaVertex.value(n).x(),llistaVertex.value(n).y()) ;
  }
  qDebug("*********");*/
return llistaVertex;
}
