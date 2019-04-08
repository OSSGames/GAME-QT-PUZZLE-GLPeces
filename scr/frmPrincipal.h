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
#ifndef FORMULARIPRINCIPAL_H
#define FORMULARIPRINCIPAL_H

/* 11/05/14 La revisió de versió no funciona
 * amb sourceforge.net
#include <QNetworkAccessManager>*/
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QGraphicsView>
#include "constants.h"
#include "rellotge.h"
#include "desafigura.h"

class Peca;
class ConjuntPeces;
class Configuracio;
class Rellotge;


class formulariPrincipal : public QMainWindow{

    Q_OBJECT

public:
    formulariPrincipal();
    QGraphicsScene *sceneJoc;
    QGraphicsScene *sceneMostra;
    QFrame *sceneFrame;
    QFrame *contadorsFrame;

    QTreeWidget *arbreFiguresTreeWidget;

    ConjuntPeces *conjuntJoc;
    ConjuntPeces *conjuntFigura;
    ConjuntPeces *conjuntMostra;
   // Peca *pecaSeleccionada;
    Configuracio *configuracioJoc;
    /*
    Controla l'estat del joc
    */
    EstatsDelJoc estatDelJoc;
    /*
      El rellotge que controla el temps
    */
    Rellotge *rellotgeLabel;
    QLabel *numFigures;
    QLabel *punts;
    //Llista de les figures ja resoltes
    QStringList *figuresResoltesList;

    //LLista amb les dades de les figures semblants
    //(funció de cerca de figures semblants en la
    //creació de figures)
    QStringList *figuresSemblantsList;

    QStringList nomsDelsJocsPerTraduccio;


    /* 26/10/11
       Comprovació d'actualitzacions
       11/05/14 no funciona amb sourceforge.net

    QFile output;
    QNetworkReply *currentDownload;
    QNetworkAccessManager manager;*/

    /*
      Generació automàtica de noves figures
      */
    int nivellFiguresAutomatiques;
    //Rellotge per fer de llavor en els
    //càlculs de nombres aleatoris
    QTime rellotgeAtzar;


private slots:


     void closeEvent(QCloseEvent *event);
    /*
     Gestiona la selecció d'una figura a l'arbre
     (conectat amb arbreFiguresTreeWidget::itemSelectionChanged ()
    */
    void seleccioFigura();

   /*  4.0.2
       Elimina les dades de la solució obtinguda per l'usuari
       */
    /*
      16/12/11
      Eliminat el paràmetre column que no es fa servir
      */
    //void seleccionFiguraItemDoubleClicked(QTreeWidgetItem * item, int column);
    void seleccionFiguraItemDoubleClicked(QTreeWidgetItem * item);

    /* 4.1 28/01/11
       Retorna el % de la solució anterior de la figura
      */
     float percentatgeSolucioAnterior(QString nomFigura);
    /* 4.1 28/01/11
    Busca en la llista de figures resoltes el nom passat per paràmetre
    i retorna el índex
       */
    int buscaFiguraSolucionadaEnLlista(QString nom);

    void creaArxiuImatge(QString nomArxiu);

    void canvisSceneJoc();

    //Deixa el programa en pausa
    void pausaSlot();

 /*
  Slots d'eliminació de records
*/
    void eliminaRecordSlot();
    void eliminaTotsRecordsSlot();

    void carregaFiguresProgramaSlot();
    void carregaFiguresArxiuSlot();
/*
  Slots de creació dels tangrams
  13/02/11
  Ara es crean en temps d'execució
   */
    void creaTangram(int tipusTangram);

    void carregaColorsPersonalitzatsPeces();

    /* 20/02/11
      Col·loca les peces en la figura
      "inicial" del tangram*/
    void figuraInicialTangram(int tipusTangram);
    // 13/02/11
    //Slot per a les accions dels tipus de tangram
    void creaTangramAccio(QAction *accio);

/*
  Slots del nivell de jocconjuntFigura
*/
void nivellInfantilSlot();
void nivellPrincipiantSlot();
void nivellIniciatSlot();
void nivellExpertSlot();
void canviaNivellJoc(int pnivellJoc);

/*
Creació de noves figures
*/
void comprovaMostraSlot();
void esborraMostraSlot();
void desaFiguraSlot();
void simetriaVerticalSlot();
void simetriaHoritzontalSlot();
void cercaFiguraSlot();

void figuraSeguentSlot();
void figuraAnteriorSlot();

/*
  Ajuda per resoldre les figures
*/
void mostraSolucioSlot();
void colocaPecaSlot();

void proposaFiguraSlot();
void cercaNomFiguraSlot();
void continuaCercaNomFiguraSlot();

/*
 Gestió de concurs
*/
void creaConcursSlot();
void carregaConcursSlot();

/*
  Selecció d'idioma
*/
void seleccioIdioma(QAction *action);
/* Actualitza l'arxiu de traducció dels formularis Qt */
void seleccioIdiomaQt(QString strLocale);
/*
  Canviat el sistema de gestió de l'idioma
  v. 4.0.2
*/


//Ajuda
void ajudaSlot();
void webProgramaSlot();
void creditsProgramaSlot();
void colaboraProgramaSlot();
void agraimentSlot();

/* 26/10/11
   Comprovació de nova versió
   11/05/14 no funciona amb
   sourceforge.net

void comprovaVersioSlot();
void baixaArxiuFinished();
void baixaArxiuReadyRead();*/

/*
  Generació automàtica de noves figures
  */

void generaFiguraAtzarSlot();
/*25/04/14 procediment obsolet
bool generaNovaFiguraAtzar(bool desaFigura=false);*/
void generaFiguresMassiuAtzarSlot(bool massiu=true, int ambDades=1);
void desaFiguraAtzar(QString nom, QString codi, bool niveljJoc=false);
bool generaFiguresMassiuAtzar_ComprovaCompactacio();
/* controla si el tangram actual te generació a l'atzar de noves figures*/
bool tangramsSenseGeneracioAutomatica();
void controlaActivacioMenusFiguresAtzar();
bool controlaArxiuDadesPosicionsPeces();
void comprovaNovesFiguresAtzarSlot();
//Generació a l'atzar de figures (sense dades de recolçament)
void generaFiguresAtzarSlot();

void dadesPosicionsPeces(int nivell,int peca1, int peca2);
QStringList carregaDadesPosicionsPeces();
QStringList seleccionaDadesPosicionsPeces(QStringList llistaDades2,
                                          int pecaAColocar2,int pecaColocada2);
QStringList seleccionaPosicionsCorrectesPeces(QStringList llistaDades3,
                                              int pecaAColocar3,int pecaColocada3);
QList<int> generaOrdrePecesGeneracioFiguraAtzar();
bool generaFiguraAtzar(QStringList dadesPosicionsPecesList, bool desaFigura=false);
bool generaFiguraAtzarCompacte(QStringList dadesPosicionsPecesList, bool desaFigura=false);
QStringList colocaPecesFiguraAtzar1(QStringList llistaDades1,int pecaAColocar1,int pecaColocada1);
bool colocaPeca(QStringList llistaDades5, int pecaAColocar5);
int calculaNivellDificultatFigura();

/* Generació automàtica de noves figures
 * sense dades de suport
 */
void generaFiguraAtzar_inici(bool desaFigura);
/* process general de generació de figures a l'atzar*/
bool generaFiguraAtzar_generaNovaFiguraAtzar();

/* Col·loca un vèrtex de la peça a un punt determinat
 */
bool generaFiguraAtzar_colocaPecaPunt(int indexPeca, int indexPecaInicial, QList<int> indexPecesJaColocades);

void generaFiguraAtzar_colocaPecaPunt2(int indexPeca, int indexPecaInicial,
                                       QList<int> indexPecesJaColocades,
                                       int sentitRotacio);

/* Va generant la silueta de la nova figura generada
 */
qreal generaFiguraAtzar_SiluetaFigura();
/* Comprova si la silueta de la figura és inferior a
 * perímetre total de les peces
 */
bool generaFiguraAtzar_longitudSiluetaFigura();

/* Realitza les tasques inicials comuns als dos processos de generació
 * de noves figures (amb dades i a l'atzar)
 */
void generaFiguraAtzar_configuracioInicialProces();
/* Fa les tasques necessàries per a què, una vegada generada una
 * nova figura, es pugi començar a jugar*/
void generaFiguraAtzar_configuracioFinalProcess();

/* Retorna una llista de nombres enters ordenats
 * a l'atzar
 */
QList<int> generaLlistaIndexosAtzar(int nombreIndexos);



/*
  Generació d'arxius HTML
  */
void HTML();
void HTML_creaArxiuImatge(QString nomArxiu);
void HTML_imatgeTangram(QString nomArxiu);
void HTML_creaArxiuHTML(int numFigures);
void HTML_creaArxiuIndex();
QString HTML_llicencia();
void arxiuLatex();

/* Funcions tàctils
 */
/*
 * acció pel nou botó per fer la simetria d'una peça
 */
void simetriaPecaSlot();

private:

    void creaAcccions();
    void creaMenus();
    void creaIdiomaMenu();
    //Crea les accions i menus amb
    //els tipus de tangram
    void creaTipusTangramMenu();
    //retorna una llista amb les dades dels
    //tangram ordenats pel número de peces
    QStringList dadesTangram();
    //Retorna una llista
    //amb les dades d'un arxiu
    QStringList  llegeixArxiuALlista(QString arxiu);
    void creaBarraTasques();
    void actualitzaMenus();
    void missatgeStatusBar(QString missatge);

    void carregaArbreFigures(QString nomArxiu);
    void llegeixArxiuFigures( QString nomArxiu);

    void canviaEstatDelJoc(EstatsDelJoc estat);

    /*
      Comprova si una figura està ja resolta
      */
    float figuraSolucionada();
    /*
      Compara directament les siluetes per comprovar
      si són idèntiques
      */
    bool comparacioSiluetes();
    /*
      Compara les siluetes comprovant si intersecten
      els paths
      */
    bool siluetesIntersecten();
    //S'ha passat a funcions.h
    //bool comprovaCentresFigures(ConjuntPeces *conjunt1,
      //                          ConjuntPeces *conjunt2);
    bool comparacioLongitudSiluetes();
    //No es fa servir v. 4.1
    //bool figuraSolucionadaNivellInfantil();

    void carregaContadors();
    void desaContadors();
    void actualitzaPuntuacio(int numFig=0,int afegeixPunts=0);
    void reiniciaContadors();

    void carregaFiguresResoltes();
    void desaFiguresResoltes();
    void eliminaFiguresResoltes(int tipus=0);
    QString nomArxiuFiguresResoltes();

    //Comprova si el nom de la figura passada per paràmetre
    //està a la llista de figures resoltes
    bool figuraJaResolta(QString nomFigura);

    //Comprova si el nom de la figura passada per paràmetre
    //ja està resolta i, en cas positiu, retorna
    //la cadena amb la posició de les peces
    QString figuraJaResoltaPosicioPeces(QString nomFigura);



    void colocaPeca(int indexPecaAColocar);
    void colocaPeca(int indexPecaAColocar,int pecaFigura);
    bool pecaJaColocada(int indexPecaAColocar);
    bool colocaPecaNivellInfantil(int indexPeca);

    int pecaSeleccionadaNivellInfantil;

    QString nomFiguraPerCercar;
    int indexFiguraPerCercar;

    void tradueixGUI();
    // 13/02/11
    //Fa la traducció dels tipus de joc
    void tradueixNomsJocsTangram();
    //Afegeix a la llista de figures semblants (creació de figures)
    //les dades d'una figura
    void afegeixFiguraSemblant(float percentatge,int index);
    int indexLlistaFiguraSemblant;
    void mostraFiguraSemblant(int index);

    /*4.0.2 21/12/10
     Estableix l'angle de gir de les peces segons
     el nivell de joc. Això ja existia com a part d'un
     altre procediment, però com què és necessari
     per la nova funció de "recuperació" de la solució de l'usuari
     s'ha separat com a funció
     */
   void angleGirConjuntPecesJoc();


   /* 26/10/11
      Comprovació de nova versió
     Llegeix l'arxiu «versio» i comprova
     si hi ha una nova versió
     11/05/14 No funciona amb sourceforge.net

   QString llegeixNovaVersio();*/

    /*
      Retorna el icone corresponent al nivel de dificultatn
      de la figura
    */
   // QIcon iconeFigura(int tipus);


    QMenu *aplicacioMenu;
    QToolBar *tangramsToolBar;


    QMenu *jocMenu;
    QMenu *jocsTangramMenu;
    QMenu *creaFiguraMenu;
    QMenu *concursMenu;
    QMenu *generaFigures;
    QMenu *idiomaMenu;    
       QActionGroup *idiomaActionGroup;
    QMenu *ajudaMenu;
    /*
      Tangram actual
    */
    int tangramActual;
    int nivellJocActual;
/*
  Accions del programa
*/
    QAction *surtAction; //tanca el programa
    QAction *elimnaRecordAction;
    QAction *eliminaTotsRecordsAction;


    QAction *carregaFiguresProgramaAction;
    QAction *carregaFiguresArxiuAction;
    /*
    Accions de creació de tangram
    13/02/11
    Ara es crean en temps d'execució
    */    
    QActionGroup *jocsTangramActionGroup;


    //Deixa el joc en pausa
    QAction *pausaAction;
    /*
      Procedimens de nivell del joc
    */
    QAction *nivellInfantilAction;
    QAction *nivellPrincipiantAction;
    QAction *nivellIniciatAction;
    QAction *nivellExpertAction;

    QActionGroup *nivellJocGroup;

    /*
      Creació de noves figures
    */
    QAction *comprovaMostraAction;
    QAction *esborraMostraAction;
    QAction *desaFiguraAction;
    QAction *simetriaVerticalAction;
    QAction *simetriaHoritzontalAction;
    QAction *cercaFiguraAction;
    QAction *figuraSeguentAction;
    QAction *figuraAnteriorAction;


    /*
      Ajudes per resoldre figures
    */
    QAction *mostraSolucioAction;
    QAction *colocaPecaAction;

    QAction *proposaFigura;


    QAction *cercaNomFigura;
    QAction *continuaCercaNomFigura;

/*
  Creació de concurs
*/
QAction *creaConcursAction;
QAction *carregaConcursAction;

    /*
      Idiomes
    */
/*
  Canviat el sistema de gestió del idioma
  v. 4.0.2
    */

//Generació automàtica de noves figures
QAction *generaFiguraAtzarAction;
QAction *generaFiguresMassiuAtzarAction;
QAction *generaFiguresAtzarAutomaticAction;
QAction *comprovaFiguresAtzarAction;

//Menu de l'ajuda
    QAction *ajuda;
    QAction *webPrograma;
  /*11/05/14 Elimnat. No funciona amb
   * sourceforge.net
    QAction *comprovaVersio; */
    QAction *creditsPrograma;
    QAction *colaboraPrograma;
    QAction *agraiment;


    /*Simetria de les peces*/
    QAction *simetriaPecaAction;

    QGraphicsView *viewJoc;
    QGraphicsView *viewMostra;

    QString charPathSeparator;

protected:
    void resizeEvent ( QResizeEvent * event );

};
#endif
