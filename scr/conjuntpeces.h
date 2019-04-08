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
#ifndef CONJUNTPECES_H
#define CONJUNTPECES_H

#include <QObject>

#include "constants.h"
#include "peca.h"

class ConjuntPeces : public QObject
{
public:
    /*
    Constructor per defecte. Crea el tangram classic
    */
    ConjuntPeces();
    ConjuntPeces(int tipusTangram);
    ConjuntPeces(int tipusTangram,double pescala);

    /*
    Crea un conjunt segons els paràmetres passats
    */
    void creaConjunt(int ptipusTangram, double pescala);

    /*
    Array amb les peces del conjunt
    */
    QVector<Peca *>  arrayPeces;

    double escala;

    int tipusTangram;


    /*
      Col·loca les peces a l'atzar
    */
    void colocaPecesAtzar();

    /*
      Col·loca les peces segons les dades passades
    */
    void colocaPeces(QString dades);

    void colocaPeca(QString dades, int indexPeca, QPointF punt);
    /*
      Color de les peces del conjunt
    */
    void colorPecesConjunt(QColor color);

    /*
    Fa les tasques necessàries pel conjunt de peces de mostra
    */
    void conjuntDeMostra();

    /* Fa que les peces del conjunt estiguin
     * per sobre de les peces d'altres conjunts*/
    void conjuntSuperior();

    /*
    Mostra la solució
    */
    void conjuntSolucio();

    /*
    Posa la figura (resolta) en color verd
    */
    void conjuntFiguraResolta();

    /*
    Controla si el conjunt és visible
    */
    void conjuntVisible(bool visible);

    /*
    * Retorna una cadena amb les coordenades de les peces
    * per tal de desar-les a un arxiu de text
    */
    QString coordenadesFigura();
    /*
    * Retorna les coordenades (en forma de Point), del vèrtex
    * superior-esquerra de les Peces passades
    * per paràmetre
    */
    QPointF vertexSuperiorEsquerra();

    QPointF vertexInferiorDreta();
    /*
    * Centra la figura en el panell
    */
   void centraFiguraScene();


   QRectF rectangleFigura();

   /*
     Recoloca les peces segons el punt passat
     (Pel nivell expert)
   */
   void recolocaPeces(QPointF punt);


    void simetriaVertical();
    void simetriaHoritzontal();

    //Llista amb les dades per a
    //construir les peces
   // QStringList dadesPeces;

    /*
      15/11/11
      Retorna la silueta de la figura formada
      per les peces del conjunt
      (es fa servir per calcular el nivell de dificultad
      de les figures generades automàticament)
      */
    QPainterPath siluetaFigura();
    QPainterPath siluetaFiguraEscala(double escala=1.0);
    qreal longitudTotalPeces;
    /*
      Retorna la longitud de la silueta de la figura actual
      */
    qreal longitudSilueta();
    QList<QPointF> llistaVertexConjunt();

private:


    /* 14/02/11
       Eliminat degut al nou sistema
    Crea les peces del conjunt
    */
   // void creaJocPeces(int ptipusTangram,double pescala);

};

#endif // CONJUNTPECES_H
