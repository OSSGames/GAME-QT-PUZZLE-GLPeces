/*
 * Copyright (C) 2012-2016  Innocent De Marchi
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
#ifndef COMPROVAFIGURES_H
#define COMPROVAFIGURES_H



#include <QtWidgets/QDialog>
#include <QtWidgets/QProgressDialog>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QFrame>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QLabel>
#include <QtGui>

#include "conjuntpeces.h"
#include "constants.h"
#include "configuracio.h"



class ComprovaFigures : public QDialog
{
    Q_OBJECT

public:
    ComprovaFigures(Configuracio *configJoc, QWidget *parent = 0);

    //Per mostrar les figures del programa
    QGraphicsScene *sceneFiguresPrograma;
    QGraphicsView *viewFiguresPrograma;

    //Per mostrar les figures noves
    QGraphicsScene *sceneFiguresNoves;
    QGraphicsView *viewFiguresNoves;

    //Per fer la comprovació de figures
    QGraphicsScene *sceneComparacio;
    QGraphicsView *viewComparacio;

    QLabel *labelFiguraPrograma;
    QLabel *labelFiguresGenerades;
    QLabel *labelTemps;
    QFrame *botonsFrame;

    QPushButton *tanca ;
    QPushButton *obreFiguresGenerades;
    QPushButton *comparaFiguresPrograma;
    QPushButton *comparaFiguresGenerades;
    QPushButton *figuraAnteriorButton;
    QPushButton *figuraSeguentButton;
    QPushButton *eliminaFigura;
    QPushButton *desaCanvis;
    QPushButton *canviaNumeracio;

    QPushButton *desaLlistaComparacio;

    QProgressDialog *progresCercaFigura;

    QTreeWidget *arbreFiguresTreeWidget;
    QTreeWidget *arbreNovesFiguresTreeWidget;

    ConjuntPeces *conjuntMostraPrograma;
    ConjuntPeces *conjuntMostraNovesFigures;


    //Figura que es busca (de les figures generades)
    ConjuntPeces *conjuntComparacioOriginal;
    //Figura amb la qual es compara
    ConjuntPeces *conjuntComparacioComparada;

    Configuracio *configuracioJoc;

    QString charPathSeparator;
    //Rellotge per controlar el temps
    //de recerca de les figures
    QTime tempsComparacioFigures;

private slots:
    /*
     Gestiona la selecció d'una figura a l'arbre
     (conectat amb arbreFiguresTreeWidget::itemSelectionChanged ()
    */
    void seleccioFiguraJoc();
    void seleccioFiguraNova();
    /*marca per eliminar*/
    void seleccionFiguraItemDoubleClicked (QTreeWidgetItem * );
    QString seleccioFigura(QTreeWidget *arbre);

    //botons
    void obreFiguresGeneradesSlot();
    //busca les figures generades entre
    //les figures del programa
    void comparaFiguresProgramaSlot();


    void figuraSeguentSlot();
    void figuraAnteriorSlot();
    void eliminaFiguraSlot();
    void desaCanvisSlot();
    void canviaNumeracioFiguresGeneradesSlot();

    //busca les figures generades entre
    //les figures generades
    void comparaFiguresGeneradesSlot();
    void activaBotonsMovimentLlistaComparacioFigures();
    void desaDadesCercaFigures();
    void tancaSlot();
    void desaLlistaComparacioSlot();

private:



    //Carrega les figures als arbres
    void carregaArbreFigures(QTreeWidget *arbre, QString nomArxiu, bool traduc=true);
    void llegeixArxiuFigures(QTreeWidget *arbre, QString nomArxiu, bool traduc);
    QString nomArxiuFiguresGeneradesPredeterminat();


    void cercaFigura(int indexOriginal,int arbreComparacio,QTreeWidget *arbreCerca, int itemInici=0);
    float figuraSolucionada();
    /* Antic procediment. Ara es fa servir en els casos que la longitud de les dues
     * siluetes sigui semblant i el nombre de vèrtex no sigui igual però la diferència
     * sigui petita
     */
    double cercaFiguraAntic(int indexOriginal, int i, int arbreComparacio);

    /* Compara els rectangles de les figures
     * per cercar semblances
     */
    bool comparaRectanglesFigures();
    /*
      Compara les siluetes dels conjunts per saber
      si poden ésser semblants
      */
    bool comparacioLongitudSiluetes();

    /*
      Compara les siluetes directament
      */
    bool comparacioSiluetes();
    bool siluetesIntersecten();
    /* Retorna el nombre dels vèrtex d'una figura formada per un
     * conjunt de peces. O pel conjunt original i 1 pel conjunt comparada
     */
    int nombreVertexQPainterPath(int conjunt);
    /*11/11/15 No es fa servir
    bool angleEntrePunts(QPointF PoligonOriginal_P1,QPointF PoligonOriginal_P2,QPointF PoligonOriginal_P3,
                           QPointF PoligonComparada_P1,QPointF PoligonComparada_P2,QPointF PoligonComparada_P3);
    */
    /*11/11/15 No es fa servir
    QList<QPointF> inverteixLlistaPunts(QList<QPointF> llista);*/

    void afegeixFiguraSemblant(int indexOriginal,int indexComparada,
                               int arbreComparacio, float percentatge);
    /* Procés comú en la comparació de figures
     */
    void colocaFiguraComparada(QString dades);

    void activaLabelsFiguresSemblants(bool activa=false);
    void activaConjuntsFigures(bool activa=false);


    void mostraFiguraSemblant(int index);
    int indexLlistaFiguraSemblant;
    QStringList *figuresSemblantsList;
    /*
      Genera tots els conjunts de peces
      per mostrar les figures
      */
    void generaConjunts(int tipusTangram);

    void actualitzaTextItemArrel();

    void tradueixGUI();

    void missatgeEstimacioTemps(int figuresFetes, int total, int temps);


    /*Per controlar les dades de generació automàtica de figures*/
    void dadesGeneracioFigures_carregaDades();
    /*Carrega totes les dades de l'arxiu corresponent*/
    QStringList llistaDadesPeces;
    int indexLlistaDadesPeces;
    /*coloca les peces*/
    void dadesGeneracioFigures_colocaPeces();
    QStringList dadesGeneracioFigures_carregaDadesPosicionsPeces();
    QStringList dadesGeneracioFigures_carregaDadesPosicionsPeces1();
    bool generaFiguresMassiuAtzar_ComprovaCompactacio();


};

#endif // COMPROVAFIGURES_H
