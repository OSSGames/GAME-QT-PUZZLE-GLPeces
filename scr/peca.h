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
#ifndef PECA_H
#define PECA_H

#include <QtWidgets/QGraphicsItem>
#include <QVector>
//#include <QApplication>
#include <QColor>


#include "constants.h"
#include "conjuntpeces.h"

class ConjuntPeces;

class Peca : public QGraphicsItem
{
	//Declaració de macro per les traduccions
	//(Aqui no hi ha res per a traduir)
	 //Q_DECLARE_TR_FUNCTIONS(Peca)
	
public:
     /**
     * Número de vèrtex de la Peça
     */
    int numeroVertex;
    
    /**
     * Tipus de Peça
     */
    int tipusPeca;
    
    /**
     * Controla si s'ha fet la simetria de la Peça
     */
    bool simetriaBool;
    
    /*
    Determina si cal pintar tota la Peça
    o nomès la silueta (per mostrar la solució)
    */
    bool solucio; 

    /*
    Determina l'estat de moviment de la peça
    0: no passa res
    1: es mou la peça
    2: es gira la peça
    */
    int movimentPeca;
    /* 03/03/14
     * Determina si la peça es pot moure
     * en dispositius tàctils
     */
    bool movimentPecaTactil;

    /*
      Nivell del joc
      Es fa servir per eliminar l'efecte de xoc entre
      les peces al nivell infantil
      */
     int nivellJoc;


	
    Peca(); //constructor per defecte
    Peca(int tipusDePeca); //Constructor. Crea una peça segons el paràmetre
    //Coloca la peça a l'atzar
    Peca(int tipusDePeca,double pescala);
  //  Peca(int tipusDePeca,double pescala, QString dadesPeca);
    //Constructor per a les Peces que formen una figura
    Peca(int tipusPeca,float posicio_X,float posicio_Y,float angle,
            int simetria,double pescala );
    //~Peca(); //destructor

   // QStringList carregaDadesPeces();

    /*
    Crea la peça segons el parámetre passat.
    Estableix els valors de les propietats
    */    
    void creaPeca(int pintPeca);
    /*
      Retorna les dades de la peça
      */
    QString recuperaDadesPeca();
    

    /* 15/10/11
       Aquest procediment no es fa servir
    */
    void colocaPecaAtzar(int maxX,int maxY);
    void colocaPecaAtzar();
    /*
      Retorna una cadena amb les dades de la peça
      per tal de formar figures
    */
    QString cadenaPropietatsPeca(QPointF vertexSuperiorEsquerra);

    /* Rectangle on està inclosa la peça. És l'area que es redibuixa*/
    QRectF boundingRect() const;
    
    /* Silueta de la peça*/
    QPainterPath shape() ;
    QPainterPath shapeEscala(float pescala);

    /* Punts per controlar la solució*/
    void puntsSolucio(QVector<QPointF> &pSol,float reduccio=1.0);
    void puntsSolucio(QVector<QPointF> &pSol,QPointF vertSupEsq,float reduccio);
    /*
      Retorna (en el vector del parámetre) les coordenades
      dels vèrtex de la peça
    */
    void puntsVertexPeca(QVector<QPointF> &pVertex);
    void puntsVertexPecaReducccio(QVector<QPointF> &pVertex, double reduccio);
    /* Calcula el rectangle per dibuixar l'arc de la peça
    Retorna el rectangle
    */
    QRectF emetArcPeca( float &angleInici);

//això podria ésser privat
    float angleRadi(QVector<float> VectorCentreInici);
//això podria ésser privat	
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);
    
    // estableix color dels limits de la peça
    void setColorLimits(const QColor &color);
    //retorna el color dels limits de la peça 
    QColor getColorLimits() const;
    //estableix els colors a l'atzar
    void colorsAtzarPeca();
    
    //Estableix el color interior de la peca
    void setColorPeca(const QColor &color);
    //Retorna el color interior de la peca
    QColor getColorPeca() const;
    
    
    // Estableix l'angle de rotació de la peça
    void setAngleRotacio(double angle);
    //Retorna l'angle de rotació actual de la peça
    double getAngleRotacio () ;

    void estableixAngleGirPeca(double angle);
    double emetAngleGirPeca();

    //gira la peça l'angle passat
    bool rotaPeca(double angle, bool controlaLimitsSuperposicio=false);
    //actualitza els angles dels vertex
    //quan canvia angleRotacio;
    void actualitzaAnglesVertex(double angle);
    double normalitzaAngle(double angle); //en radians
    double normalitzaAngleGraus(double angle);
    
    void setEscala(double pescala);
    double getEscala() const;
    /*
      Simplement, fa update()
     */
    void actualitzaPeca();
    /*Reinicia els angles de la Peça
    */
    void reiniciaAnglesVertex();
    /**
     * Fa la simetria de la figura, canviant els angles
     */ 
    void simetriaPeca();
    /**
     * Canvia el valor de la propietat simetriaBool
     */
    void canviaSimetriaPeca();

    /*
    Punt del centre de la Peça
    */
    QPointF centrePeca() const;

    /*
    Determina si la peça és del joc o de mostra
    */
    bool pecaMostra;

    /*
    Determina si cal dibuixar la peça
    */
    bool pecaVisible;
    /**
     * Array amb els angles de les coordenades
     * polars dels vèrtex de la Peça
     *
     * Les successives rotacions de la Peça
     * es van acumulant en aquests angles
     */
     QVector<float> anglesVertex;
     /**
     * Angle incial del vèrtex 0 de la Peça
     * Es fa servir a l'hora de determinar l'angle de gir
     * actual de la Peça quan s'ha de passar a text
     */
    double angleInicial;


    /*
    Punt central del rectangle de la peça
    */
    QPointF centreRectanglePeca();

    /*
      Conjunt de les peces al qual pertany
      aquesta peça
    */
    ConjuntPeces *conjuntDePeces;
    /*
      Conserva el índex d'aquesta peça
      en el vector de peces del conjunt
    */
    int indexEnElConjunt;

    /**
     * Controla si la peça te un dels costats en forma d'arc
     */
    bool ambArc;

    //determina si la peça està en els límits de l'escena
    bool pecaEnLimits();


    /* 15/10/11
    determina si la peça xoca amb una altra
    14/11/11
    S'ha passat a públic pel tema de la generació
    de figures a l'atzar
    */
    bool pecaSuperposada(bool controlInfantil=true);
    /* El mateix que l'anterior, però es pot fer servir
     * el paràmetre escala per augmentar o minvar el tamany
     * de la peça*/
    bool pecaSuperposadaEscala(bool controlInfantil=true, float escala=1.0);

    /* Controla si la peça està superposada a
     * la peça passada per paràmetre
     */
    bool pecaSuperposada(QList<int> indexPeces);
    bool pecaSuperposadaEscala(QList<int> indexPeces, double escala);

    /*
    Calcula el trasllat necessari per
    "aferrar" la peça a la més propera
    14/11/11
    S'ha passat a públic pel tema de la generació
    de figures a l'atzar
    */
    QPointF aferraPeca();
    /*
      Intenta "aferrar" la peça pels costats
      */
    QPointF aferraPeca2();


    /* Generació de figures a l'atzar
     */



    /* Es una variant de aferraPeca() però treballa nomès amb
     * les peces ja col·locades per genrar una figura nova
     */
    QPointF generaFiguraAtzar_aferraPeca(QList<int> indexPecesColocades,double minDist=5.0,
                                         bool continua=true);
    QPointF generaFiguraAtzar_aferraPeca2(QList<int> indexPecesColocades);

    /* Aferra la peça a la més pròxima de les peces ja col·locades
     * Es fa una aproximació lenta
     */
    bool generaFiguraAtzar_aferraPecaLentament(QList<int> indexPecesColocades, bool continua=true);
    bool generaFiguraAtzar_separaPecaLentament(QList<int> indexPecesColocades,QPointF vectorMoviment);

    /* Mou la peça al punt passat per paràmetre seguint
     * el procediment del moviment amb el ratolí
     */
    bool generaFiguraAtzar_mouPeca(QPointF punt, QList<int> indexPecesColocades, int sentitRotacio);


    /*
        Determina l'angle de gir de la peça
        (segons el nivell del joc)
        1: gir lliure
        número: graus de gir
        */
        double angleGirPeca;

        //Angle de rotació (en radians)
        double angleRotacio;
 protected:
 	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent ( QGraphicsSceneMouseEvent *event);
 	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void controlPecaSeleccionada();
 	
 private:
     
     /**
     Determina el nombre de peces total del joc
     al qual pertany la peça.
     Es fa servir pel ZOrder
     */
     int nombrePecesJoc;

    /**
     * Número pel qual es multipliquen els mòduls de les
     * Peces per a què tenguin
     * un "tamany" aparent a la pantalla "acceptable"
     *
     */
    double escala;



    
    
    /*
     * Array amb els mòduls de les coordenades
     * polars dels vèrtex de la Peça
     */
    QVector<float> modulsVertex;

    /* Index del mòdul més gran de la peça per tal de calculat el rectangle en què es dibuixa*/
    int modulMesGran;

   	
    
    QColor colorPeca;
    QColor colorLimits;



    /**
     * Angle per situar el centre de l'arc de les peces amb curva
     */
    float angleCentreArc;
    /**
     * Angle de l'arc de la peça
     */
    float angleArc;
    
    /* Coeficient per ajustar l'arc calculat per Bezier a una circumferència
      Per a cada peça, està determinat empíricament*/
    float ajustBezier;
     /**
     * Modul del centre de l'arc per les peces amb curva
     */
    float centreArc;



    /* 
    Carrega les dades de la peça
    Nomès es crida desde creaPeca
    */
    void carregaDadesPeca(int pintPeca);
    
    /**
     * Carrega els angles inicials de la Peça
     * @param tPeca
     */
   // void anglesInicialsPeca(int tPeca );

    void anglesInicialsPeca();

    /*
    Retorna l'angle de l'arc de la peça
    */
    float angleArcPeca();

    
//innecessari    
    //QRectF outlineRect() const ;




 //Cadena amb les dades de la peca
 QString dadesDeLaPeca;
	
};//Final de la declaració de la clase

#endif // __PECA_H__
