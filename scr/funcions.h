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
#ifndef FUNCIONS_H
#define FUNCIONS_H

#include <QtGui>
#include "configuracio.h"
#include "conjuntpeces.h"

QString directoriFiguresPersonals();
QString filtreArxiusConcurs(Configuracio *confJoc);
QString extensioArxiuConcurs(Configuracio *confJoc);

QString directoriFiguresGeneradesAtzar();
/*
  Noms dels arxius de les figures generades a l'atzar
  pel formulari de revisió i per la càrrega
  */
QString filtreArxiusFiguresGeneradesAtzar(Configuracio *confJoc, int tipus=0);
QString extensioArxiuFiguresGeneradesAtzar(Configuracio *confJoc);
int sumatori(int nombre);

/*
Retorna el icone corresponent al nivel de dificultat
de la figura
*/
QIcon iconeFigura(int tipus);

/*
  Directoris linux
 */
QString directoriLinux();

/*
  13/02/11
  Carrega el contingut de l'arxiu passat per paràmetre
  a una llista (que retorna)
  */
QStringList llegeixArxiuALlista(QString arxiu);

QString llegeixLiniaArxiu(QString arxiu, int linia);

/*
  Genera els directoris necessaris per la generació
  dels arxius html
  */
void directorisHTML(QString dir);

/*
  Retorna un nombre l'atzar a partir del passat
  com a parámetre
  */
int generaNombreAtzar(int llavor);

/*
  Retorna la diferència de longitud de les siluetes,
  com a percentatge de la longitud de la silueta
  amb major longitud
  */
float comparacioLongitudSiluetesPercentatge(qreal longitud1,qreal longitud2);

/*
  Comprova que els centres de les figures estiguin dins de la solució
  */
bool comprovaCentresFigures(ConjuntPeces *conjunt1,
                                             ConjuntPeces *conjunt2);

/* Retorna la distància entre els dos punts passats
 */
double distanciaEntrePunts(QPointF punt1,QPointF punt2);

/* Retorna el sentit del gir que cal donar a una peça
 * depenent de si el punt2 està a la dreta o l'esquerra
 * del punt central de la peça
 */
int sentitDelGir(QPointF centrePeca, QPointF punt);

/*
  Permet seleccionar un color per la peça
  i escriu la selecció a l'arxiu de configuració
  */
QColor personalitzaColorPeca(int indexEnElConjunt,
                             QColor colorOrig, int pTipusTangram);
void desaColorsPecaPersonalitzat(QString clau,QString color);
void eliminaPersonalitzaColorPeca(int indexEnElConjunt,
                                  int ptipusTangram);

/* fa una pausa en microsegons*/
//void pausa(int temps=2);

/* Procediments de cerca de figures semblants*/
/* Compara el nombre d'elements de cada silueta
 * i retorna si son iguals o no
 */
//double comparaQPainterPath(QPainterPath original, QPainterPath comparada, QPointF centreOriginal, QPointF centreComparada);
double comparaQPainterPath(QPainterPath original, QPainterPath comparada, QPointF centreOriginal,
                           QPointF centreComparada);
/* Localitza els vèrtex de la llista de QPainterPath
 * (localitzant els canvis de direcció en forma d'angles)
 * i els retorna en una llista de punts
 */
QList<QPointF> vertexQPainterPath(QList<QPointF> llista, QPointF centreFigura);
/* Retorna una llista de distàncies i angles elaborada a partir
 * d'una llista de vèrtex d'una figura
 */
QList<double> llistaDistanciesAngles(QList<QPointF> llistaVertex);
double angleEntreTresPunts(QPointF PoligonOriginal_P1, QPointF PoligonOriginal_P2, QPointF PoligonOriginal_P3);
bool puntEntreDosPunts(QPointF mitja,QPointF extrem1,QPointF extrem2);
//double angleVertexRespecteOrigen(QPointF vertex);
double angleVertex(QPointF final, QPointF origen);
/* Ordena els punts de la llista respecta al centre de la figura
 * i en sentit antihorari
 */
QList<QPointF> ordenaLlistaVertex(QList<QPointF> llista, QPointF centreFigura);
#endif // FUNCIONS_H
