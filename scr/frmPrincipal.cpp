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
//#include <QtGui>
//#include <QModelIndex>
//#include <QImage>


/* 11/05/14 La revisió de versió no funciona
 * amb sourceforge.net
#include <QNetworkRequest>
#include <QNetworkReply>*/


#include <QtWidgets/QProgressDialog>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include "frmPrincipal.h"

#include "peca.h"
#include "conjuntpeces.h"
#include "constants.h"
#include "configuracio.h"
#include "desafigura.h"
#include "creaconcurs.h"
#include "funcions.h"
#include "comprovafigures.h"


class QSettings;

//Numero de figures
//int numeroFigures=0;

double distanciaAferraPeca=50;

/* Nom del darrer arxiu de traducció de Qt
 * carregat*/
QString translatorFileName;

formulariPrincipal::formulariPrincipal()
{


charPathSeparator=QDir().separator();

estatDelJoc=Inactiu;
//Activam el rellotge que es fa servir
//per els càlculs aleatoris
rellotgeAtzar.start();

setMinimumSize (QSize(900,620));
//28/02/11
//setMaximumSize(QSize(900,620));


//cream la configuració del programa
configuracioJoc= new Configuracio();
seleccioIdiomaQt(configuracioJoc->strLocale());
//Dimensionam segons les dades de configuració
/* 20/02/12
   Millorat

setGeometry(0,0,configuracioJoc->configArxiu->value(
                          QString("General/amplada")).toInt(),
                  configuracioJoc->configArxiu->value(
                          QString("General/alt")).toInt());*/

restoreGeometry(configuracioJoc->configArxiu->value(
        QString("General/geometry")).toByteArray());

figuresResoltesList=new QStringList();
carregaFiguresResoltes();

figuresSemblantsList=new QStringList();

    sceneFrame=new QFrame(this);
    sceneFrame->setGeometry(0,60,900,550);
    sceneFrame->setMinimumSize(900,550);

    //Panell de la mostra de les figures
    sceneMostra=new QGraphicsScene(0, 0, 140, 140);

    viewMostra = new QGraphicsView(sceneMostra,sceneFrame);
    viewMostra->setRenderHints(QPainter::Antialiasing
                             | QPainter::TextAntialiasing);
    viewMostra->move(QPoint(150,0));

    //Panell del joc
    sceneJoc = new QGraphicsScene(0, 0, 600, 530);
    connect(sceneJoc,SIGNAL(selectionChanged()),
            this,SLOT(canvisSceneJoc()));
    viewJoc = new QGraphicsView(sceneJoc,sceneFrame);
    viewJoc->setRenderHints(QPainter::Antialiasing
                           | QPainter::TextAntialiasing);
    viewJoc->move(QPoint(300,0));
    //No es  mostraran les barres de desplaçament
    viewJoc->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewJoc->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    viewJoc->setMinimumSize(600,530);


    contadorsFrame=new QFrame(sceneFrame);
    contadorsFrame->setGeometry(0,0,140,140);
    contadorsFrame->move(QPoint(0,0));

    rellotgeLabel = new Rellotge(contadorsFrame);
    rellotgeLabel->setGeometry(0,0,140,30);
    rellotgeLabel->setAlignment(Qt::AlignHCenter);

    numFigures= new QLabel("Figures",contadorsFrame);
    numFigures->setGeometry(0,35,140,30);
    numFigures->setAlignment(Qt::AlignHCenter);

    punts= new QLabel("Punts",contadorsFrame);
    punts->setGeometry(0,70,140,30);
    punts->setAlignment(Qt::AlignHCenter);



    //arbre de figures
    arbreFiguresTreeWidget= new QTreeWidget(sceneFrame);
    arbreFiguresTreeWidget->setGeometry(0,0,295,390);
    arbreFiguresTreeWidget->setMinimumSize(QSize(295,390));
    arbreFiguresTreeWidget->move(QPoint(0,145));
    arbreFiguresTreeWidget->expandAll();
    connect(arbreFiguresTreeWidget,SIGNAL(itemSelectionChanged()),
           this,SLOT(seleccioFigura()));
    connect(arbreFiguresTreeWidget,SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )),
            this,SLOT(seleccionFiguraItemDoubleClicked (QTreeWidgetItem * )));


    creaAcccions();
    creaMenus();
    creaBarraTasques();


    setWindowTitle("Peces (" + configuracioJoc->tradueixCadena("Jocs.tangram")+")");
    setWindowIcon(QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"capitols.png"));

    creaTangram(configuracioJoc->tipusTangram);
    canviaNivellJoc(configuracioJoc->nivellJoc);

    switch (configuracioJoc->nivellJoc){
        case NIVELL_INFANTIL:
         nivellInfantilAction->setChecked(true);
         break;
        case NIVELL_PRINCIPIANT:
         nivellPrincipiantAction->setChecked(true);
         break;
        case NIVELL_INICIAT:
         nivellIniciatAction->setChecked(true);
         break;
        case NIVELL_EXPERT:
         nivellExpertAction->setChecked(true);
         break;
        default:
         nivellIniciatAction->setChecked(true);
    }
 //Això és necessari degut a que canviaNivellJoc()
 //col·loca les peces a l'atzar
 figuraInicialTangram(configuracioJoc->tipusTangram);

 //Numero de figures
 //Cal seguir aquesta etiqueta
 // 05/01/2016 Hi havia un error en el recompte
 /*nivellJocActual=-1;
 canviaNivellJoc(NIVELL_INICIAT);
 tangramActual=-1;
 for(int i=1;i<=40;i++){
     if(i==1){
      numeroFigures=0;
     }
     qDebug("Tangram: %d",i);
     creaTangram(i);
     qDebug("Numero acc de figures: %d",numeroFigures);
     //qDebug("Numero de figures: %d",numeroFigures);
 }
 qDebug("Numero total de figures: %d",numeroFigures);
*/

}

/*
  Actualitza l'estat activat/desactivat dels menús
  segons l'estat del joc
  */
void formulariPrincipal::actualitzaMenus(){
    creaFiguraMenu->setEnabled(estatDelJoc==Inactiu);


    comprovaMostraAction->setEnabled(estatDelJoc==Inactiu);
    esborraMostraAction->setEnabled(estatDelJoc==Inactiu);
    desaFiguraAction->setEnabled(estatDelJoc==Inactiu);
    cercaFiguraAction->setEnabled(estatDelJoc==Inactiu &&
                                arbreFiguresTreeWidget->topLevelItem(0)!=0 );
    cercaNomFigura->setEnabled(estatDelJoc != JocIniciat &&
                               arbreFiguresTreeWidget->topLevelItem(0)!=0 );

    proposaFigura->setEnabled(estatDelJoc != JocIniciat);
    mostraSolucioAction->setEnabled(estatDelJoc==JocIniciat );
    colocaPecaAction->setEnabled(estatDelJoc==JocIniciat && configuracioJoc->nivellJoc != NIVELL_EXPERT
                                 && configuracioJoc->nivellJoc != NIVELL_INFANTIL);

    //Opcions de generació automàtica de noves figures
    controlaActivacioMenusFiguresAtzar();
    if (estatDelJoc==JocIniciat && configuracioJoc->nivellJoc != NIVELL_EXPERT
                                 && configuracioJoc->nivellJoc != NIVELL_INFANTIL){
       colocaPecaAction->setEnabled(true);
    }
    else {
        colocaPecaAction->setEnabled(false);
        colocaPecaAction->setChecked(false);
        configuracioJoc->colocaPeca=false;
    }
    carregaFiguresArxiuAction->setEnabled(estatDelJoc !=JocIniciat);
    carregaFiguresProgramaAction->setEnabled(estatDelJoc!=JocIniciat);

    elimnaRecordAction->setEnabled(estatDelJoc!=JocIniciat);
    eliminaTotsRecordsAction->setEnabled(estatDelJoc!=JocIniciat);
    /* 11/05/14 Afegits tres nous controls */
    generaFigures->setEnabled(estatDelJoc!=JocIniciat);
    concursMenu->setEnabled(estatDelJoc!=JocIniciat);
    idiomaMenu->setEnabled(estatDelJoc!=JocIniciat);
}

 void formulariPrincipal::canviaEstatDelJoc(EstatsDelJoc estat){
    switch (estatDelJoc){
        case Inactiu:
            switch (estat){
               case FiguraCarregada:
                    estatDelJoc=estat;
               break;
               case FiguraResolta:
                    estatDelJoc=estat;
                    conjuntFigura->colorPecesConjunt(COLOR_FIG_RESOLTA);
                    conjuntMostra->colorPecesConjunt(COLOR_FIG_RESOLTA);
                    break;
               case Inactiu:
               case JocIniciat:
               case MostraSolucio:
               break;
           }
        break;//Fi Inactiu

        case FiguraCarregada:
            switch (estat){
                case Inactiu:
                estatDelJoc=estat;
                conjuntFigura->conjuntVisible(false);
                conjuntMostra->conjuntVisible(false);
                break;
                case JocIniciat:
                estatDelJoc=estat;
                //posar en marxa el rellotge
                rellotgeLabel->iniciaRellotge();
                /* 4.0.2 21/12/10
                 Aquesta absurda repetició soluciona el problema de la col·locació
                 de les peces en el nivell infantil. Abans, en el primer intent
                 de col·locar la primera peça, no es col·locava.
                   */
                conjuntJoc->arrayPeces[0]->setSelected(true);
                conjuntJoc->arrayPeces[0]->setSelected(false);
                break;
                case FiguraResolta:
                    estatDelJoc=estat;
                    conjuntFigura->colorPecesConjunt(COLOR_FIG_RESOLTA);
                    conjuntMostra->colorPecesConjunt(COLOR_FIG_RESOLTA);
                break;
                case FiguraCarregada:
                case MostraSolucio:
                break;
            }
        break;  // FiguraCarregada

        case JocIniciat:
            switch (estat){
                case FiguraResolta:
                       estatDelJoc=estat;
                        //cal aturar el rellotge
                       rellotgeLabel->aturaRellotge();
                       if (!configuracioJoc->colocaPeca){
                       actualitzaPuntuacio(1,configuracioJoc->nivellJoc*conjuntJoc->arrayPeces.size());
                        }
                       desaContadors();
                       conjuntFigura->colorPecesConjunt(COLOR_FIG_RESOLTA);
                       conjuntMostra->colorPecesConjunt(COLOR_FIG_RESOLTA);
                       /*
                        Generació de noves figures a l'atzar
                        Si es juga amb una figura generada a l'atzar, no hi ha
                        items seleccionats a l'arbre de figures
                         */
                       if(arbreFiguresTreeWidget->selectedItems().count()>0){
                           if(arbreFiguresTreeWidget->selectedItems().first()->childCount()==0){
                       arbreFiguresTreeWidget->selectedItems().first()->setIcon(0,iconeFigura(4));
                       arbreFiguresTreeWidget->selectedItems().first()->setText(3,"1");
                   }}
     /*
       v. 4.0.2 20/12/10
       S'afageix la col·locació de les peces. Així es conserva la solució
       obtinguda per l'usuari
       */
                       /* v. 4.1 28/01/11
                          Afegit el % de coincidència de la solució
                          */
                       /*
                         Generació de noves figures a l'atzar
                         No és possible desar les dades de la figura
                         solucionada en aquest cas
                         */
                       if(arbreFiguresTreeWidget->selectedItems().count()>0){
                       if( arbreFiguresTreeWidget->selectedItems().first()->childCount()==0 ){
                       figuresResoltesList->append(
                                QString("%1;").arg(configuracioJoc->tipusTangram)+
                                QString("%1;").arg(configuracioJoc->nivellJoc)+
                                QString("%1;").arg(arbreFiguresTreeWidget->selectedItems().first()->text(0))+
                                QString("%1;").arg(conjuntJoc->coordenadesFigura())+
                                QString("%1").arg(figuraSolucionada() ));
                       }
                       /* 25/12/11
                       Eliminat. Les figures generades es desen inmediatament
                       al arxiu, encara que no es resolguin
                       else {
                           //aprofitam per desar la figura generada
                           qsrand(rellotgeAtzar.elapsed());
                           desaFiguraAtzar("Auto-s.",QString("%1").arg(qrand()));
                       }*/
                   }


                    //v. 4.0.2 20/12/10
                    //Es desen les figures resoltes inmediatament.
                    desaFiguresResoltes();
                    break;
                case FiguraCarregada:
                case Inactiu:
                       estatDelJoc=estat;
                        //cal aturar el rellotge
                       rellotgeLabel->aturaRellotge();
                       desaContadors();
                    break;
                case MostraSolucio:
                       estatDelJoc=estat;
                        //cal aturar el rellotge
                       rellotgeLabel->aturaRellotge();
                       actualitzaPuntuacio(1,-configuracioJoc->nivellJoc*conjuntJoc->arrayPeces.size());
                       desaContadors();
                       conjuntFigura->conjuntSolucio();
                       /*Aquesta condició inicial és necessaria per evitar un error
                         si s'està jugant amb una figura generada pel programa
                       */
                       if( !arbreFiguresTreeWidget->selectedItems().isEmpty()){
                       if(arbreFiguresTreeWidget->selectedItems().first()->childCount()==0){
                       arbreFiguresTreeWidget->selectedItems().first()->setIcon(0,iconeFigura(4));
                       arbreFiguresTreeWidget->selectedItems().first()->setText(3,"1");
                       figuresResoltesList->append(
                                QString("%1;").arg(configuracioJoc->tipusTangram)+
                                QString("%1;").arg(configuracioJoc->nivellJoc)+
                                QString("%1").arg(arbreFiguresTreeWidget->selectedItems().first()->text(0)));}
                    }
                    break;
                    case JocIniciat:
                    break;
            }
        break; //JocIniciat

        case FiguraResolta:
                switch (estat){
                    case Inactiu:
                       estatDelJoc=estat;
                       conjuntFigura->conjuntVisible(false);
                       conjuntMostra->conjuntVisible(false);
                    break;
                    case FiguraCarregada:
                       estatDelJoc=estat;
                    break;
                    case FiguraResolta:
                        estatDelJoc=estat;
                        //primer cal eliminar les dades de la figura anterior
                        //QString nomFigura=QString("%1;").arg(arbreFiguresTreeWidget->selectedItems().first()->text(0));
                        figuresResoltesList->removeAt(buscaFiguraSolucionadaEnLlista(
                                arbreFiguresTreeWidget->selectedItems().first()->text(0)));
                        figuresResoltesList->append(
                                QString("%1;").arg(configuracioJoc->tipusTangram)+
                                QString("%1;").arg(configuracioJoc->nivellJoc)+
                                QString("%1;").arg(arbreFiguresTreeWidget->selectedItems().first()->text(0))+
                                QString("%1;").arg(conjuntJoc->coordenadesFigura())+
                                QString("%1").arg(figuraSolucionada() ));
                    //v. 4.0.2 20/12/10
                    //Es desen les figures resoltes inmediatament.
                    desaFiguresResoltes();
                        conjuntFigura->colorPecesConjunt(COLOR_FIG_RESOLTA);
                        conjuntMostra->colorPecesConjunt(COLOR_FIG_RESOLTA);
                    break;
                    case JocIniciat:
                    case MostraSolucio:
                    break;
                }
        break; //  FiguraResolta


        case MostraSolucio:
           switch (estat){
                case Inactiu:
                    estatDelJoc=estat;
                    conjuntFigura->conjuntVisible(false);
                    conjuntMostra->conjuntVisible(false);
                break;
                case FiguraResolta:
                    estatDelJoc=estat;
                    conjuntFigura->colorPecesConjunt(COLOR_FIG_RESOLTA);
                    conjuntMostra->colorPecesConjunt(COLOR_FIG_RESOLTA);
                break;
                case FiguraCarregada:
                   estatDelJoc=estat;
                break;
                case JocIniciat:
                case MostraSolucio:
                break;
           }
        break;  //  MostraSolucio
    }
 // actualitzar l'estat de les QActions
 actualitzaMenus();
 }
/*
  Gestiona els canvis al panell de joc
*/
void formulariPrincipal::canvisSceneJoc(){
   switch (estatDelJoc){
        case Inactiu:
        break;
        case FiguraCarregada:
            canviaEstatDelJoc(JocIniciat);
        break;
        case JocIniciat:
            //si estem al nivell infantil, controlam si cal
            //col·locar la peça directament
            if (configuracioJoc->nivellJoc==NIVELL_INFANTIL){
                    for(int i=0;i<=conjuntJoc->arrayPeces.size()-1;i++){
                        if(conjuntJoc->arrayPeces.value(i)->isSelected() &&
                          sceneJoc->selectedItems().indexOf(conjuntJoc->arrayPeces.value(i))>-1){
                          pecaSeleccionadaNivellInfantil=i;
                          break;
                        }
                    }

                if (pecaSeleccionadaNivellInfantil>=0){
                    if(colocaPecaNivellInfantil(pecaSeleccionadaNivellInfantil)){
                              colocaPeca(pecaSeleccionadaNivellInfantil);
                              pecaSeleccionadaNivellInfantil=-1;
                          }
                    }
                }

            if (configuracioJoc->colocaPeca){
               for(int i=0;i<=conjuntJoc->arrayPeces.size()-1;i++){
                   if(conjuntJoc->arrayPeces.value(i)->isSelected() &&
                      sceneJoc->selectedItems().indexOf(conjuntJoc->arrayPeces.value(i))>-1){
                      actualitzaPuntuacio(0,-configuracioJoc->nivellJoc);
                      colocaPeca(i);
                      break;
                      }
                   }
               }
            //cal revisa si la figura està resolta
            //Aquest if evita que es faci la comprovació dues vegades
            //(Quan es selecciona i quan es deselecciona una peça)
            if(sceneJoc->selectedItems().size()==0){
            float percentatgeSolucio;
            percentatgeSolucio=figuraSolucionada();
            if ( percentatgeSolucio>=(float)(PERCENTATGE_COINCIDENCIA_SOLUCIO)) {
                        canviaEstatDelJoc(FiguraResolta);
                    }
             }
        break;
        case FiguraResolta:
        /* 4.1  28/01/11
           Si l'usuari aconsegueix un percentatge major a la solució,
           llavors es controla
           */
           if(sceneJoc->selectedItems().size()==0){
            float percentatgeSolucio;
            percentatgeSolucio=figuraSolucionada();
            if(percentatgeSolucio>=(float)(PERCENTATGE_COINCIDENCIA_SOLUCIO)){
            if (percentatgeSolucio>percentatgeSolucioAnterior(
                    arbreFiguresTreeWidget->selectedItems().first()->text(0))) {
                    QMessageBox missatgeBox;
                    missatgeBox.setText(configuracioJoc->tradueixCadena("Desa.Solucio.pregunta"));
                    missatgeBox.setIcon(QMessageBox::Question);
                    missatgeBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    missatgeBox.setDefaultButton(QMessageBox::No);
                    if(missatgeBox.exec()==QMessageBox::Yes){
                        canviaEstatDelJoc(FiguraResolta);
                    }
                }
             }
        }
        break;
        case MostraSolucio:
        break;
    }
}

/* 4.1 28/01/11
   Quan l'usuari juga a "millorar" una figura ja resolta,
   aquesta funció retorna el % de la solució anterior de la figura
   */
float formulariPrincipal::percentatgeSolucioAnterior(QString nomFigura){
    if (figuraJaResolta(nomFigura)){
    QString dadesFigura=figuresResoltesList->value(buscaFiguraSolucionadaEnLlista(nomFigura));
    QStringList dadesFiguraList=dadesFigura.split(";");
    if(dadesFiguraList.count()>4){
       QString percentatgeString=dadesFiguraList.value(4);
    return percentatgeString.toLong();
    }
    else return 0;
  }
else return 0;
}

/*
  Compara les siluetes dels conjunts per saber
  si poden ésser semblants
  */
bool formulariPrincipal::comparacioLongitudSiluetes(){
 // return true;
    return ( fabs(
            conjuntFigura->longitudSilueta()-
            conjuntJoc->longitudSilueta()) ) <=10;
}

/*
  Les dues funcions següents estan repetides a comprovaciofigures.

bool formulariPrincipal::comprovaCentresFigures(ConjuntPeces *conjunt1,
                                                ConjuntPeces *conjunt2){
    //bool centresCorrectes=false;
    int numeroCentresCorrectes=0;
    int descomptarCentre=0;
    for(int k=0;k<conjunt1->arrayPeces.size();k++){
    QPointF centrePeca=conjunt1->arrayPeces[k]->pos()+
            conjunt1->arrayPeces[k]->centreRectanglePeca();
    //Si el centre de la peça està dins de la peça, el comprovam
    if (conjunt1->arrayPeces[k]->shape().contains(
            conjunt1->arrayPeces[k]->centreRectanglePeca())){
     for(int i=0;i<conjunt2->arrayPeces.size();i++){
         //qDebug("centre comprovat");
             if( conjunt2->arrayPeces[i]->shape().contains(//centrePeca)
                     centrePeca-conjunt2->arrayPeces[i]->pos())
                 ){
                 numeroCentresCorrectes=numeroCentresCorrectes+1;
                 //centresCorrectes=true;
                 break;
             }
         }
       }
    else descomptarCentre=descomptarCentre+1;
 }
 return numeroCentresCorrectes==conjunt1->arrayPeces.size()-descomptarCentre;
}
*/

/*
  Comprova si una figura està ja resolta
  */
float formulariPrincipal::figuraSolucionada(){
    //Fem la comprovació dels centres de les peces
    if (
       (!comprovaCentresFigures(conjuntFigura,conjuntJoc)) ||
       (!comprovaCentresFigures(conjuntJoc,conjuntFigura)) ){
       // return 0.0;
      }

    if(comparacioSiluetes()){
        //missatgeStatusBar(QString("100 %"));
        return 100.0;
    }

    int positiu,totals;
    positiu=0;
    totals=0;
    QVector<QPointF> pSolucio;

    //Si el nivell és expert, primer cal "resituar"
    //el conjuntFigura (nomès si la figura no és "composta")
    if (configuracioJoc->nivellJoc==NIVELL_EXPERT &&
       !conjuntFigura->arrayPeces[0]->pecaVisible){
        conjuntFigura->recolocaPeces(conjuntJoc->vertexSuperiorEsquerra());
    }

//Carregam els vèrtex de la figura
//Obsolet 10/10/11
    //QPointF vertSupEsq=conjuntJoc->vertexSuperiorEsquerra();
for(int i=0;i<=conjuntFigura->arrayPeces.size()-1;i++){
   if (configuracioJoc->nivellJoc==NIVELL_EXPERT){
    /* 4.0.2 afegit aquest if per millorar una mica la correcció amb
       el nivell expert*/
       conjuntFigura->arrayPeces[i]->puntsSolucio(pSolucio,0.85);}
   else  conjuntFigura->arrayPeces[i]->puntsSolucio(pSolucio,0.9);
   }

/* Això estaria molt be, però cal trobar un triangle que sigui
   el "MCD" de totes les peces (...que no sembla possible)
for(int j=0;j<=pSolucio.size()-1;j++){
    for(int k=0;k<=pJoc.size()-1;k++){
     QPointF punt=pSolucio.value(j)-pJoc.value(k);
        if(sqrt(pow(punt.x(),2)+pow(punt.y(),2))<=DISTANCIA_PUNTS_SOLUCIO){
          positiu1=positiu1+1;
          break;
        }
    }
}
*/
//ara fem la comprovació
totals=pSolucio.size();
if (totals<1){return 0.0;}
for(int j=0;j<=pSolucio.size()-1;j++){
    for(int k=0;k<=conjuntJoc->arrayPeces.size()-1;k++){
        if(conjuntJoc->arrayPeces[k]->shape().contains(
                pSolucio.value(j)-conjuntJoc->arrayPeces[k]->pos())){
            positiu=positiu+1;
            //aquest break evita que un mateix punt es comptabilitzi
            //més d'una vegada si hi ha peces superposades
            break;
        }
    }
}

bool interseccioSiluetes=siluetesIntersecten();
float coincidenciaPercentual=((float)((float)(positiu*100)/(float)totals));
qreal comparacioLongitud=comparacioLongitudSiluetesPercentatge(
        conjuntJoc->longitudSilueta(),
        conjuntFigura->longitudSilueta());

if( ( coincidenciaPercentual >(float)(99.9)) &&
    (comparacioLongitud<=(float)(0.05)) &&
    (interseccioSiluetes) ){
    //missatgeStatusBar(QString("%1 %").arg(coincidenciaPercentual));
    return coincidenciaPercentual; //92.0;
    }

if( (coincidenciaPercentual >(float)(95.0)) &&
    (comparacioLongitud>(float)(8.0)) &&
    (interseccioSiluetes) ){
    //missatgeStatusBar(QString("%1 %").arg(coincidenciaPercentual));
    return 99.0;
    }

if ( (coincidenciaPercentual >(float)(97.9)) &&
      interseccioSiluetes ){
    //missatgeStatusBar(QString("%1 %").arg(coincidenciaPercentual));
    return 99.5;
}
/* 4.0.2
   Afegit aquest avís per facilitar la resolució de figures
   */
//missatgeStatusBar(QString("%1 %").arg(coincidenciaPercentual));
return coincidenciaPercentual;
}

/*
  Compara directament les siluetes per comprovar
  si són idèntiques
  */
bool formulariPrincipal::comparacioSiluetes(){
return conjuntFigura->siluetaFigura()==
       conjuntJoc->siluetaFigura();
}


bool formulariPrincipal::siluetesIntersecten(){
 return conjuntJoc->siluetaFigura()
       .intersects(conjuntFigura->siluetaFigura());
}

/* 4.1 Aquesta funció no es fa servir

bool formulariPrincipal::figuraSolucionadaNivellInfantil(){
QString coordenadesFigura;
coordenadesFigura=conjuntFigura->coordenadesFigura();
  if (coordenadesFigura==conjuntJoc->coordenadesFigura()){
     return true;
  }
  else {
    QList<int> index;
    int indexPeca;
    QPointF vertexFigura=conjuntFigura->vertexSuperiorEsquerra();
    for(int i=0;i<=conjuntJoc->arrayPeces.size()-1;i++){
       indexPeca=coordenadesFigura.indexOf(conjuntJoc->arrayPeces[i]->cadenaPropietatsPeca(
               vertexFigura));
       if(indexPeca>-1 && index.indexOf(indexPeca==-1)){
          index.append(indexPeca);
       }
    }
return index.size()==conjuntJoc->arrayPeces.size();
  }
}
*/

/* 4.1 28/01/11
    Busca en la llista de figures resoltes el nom passat per paràmetre
    i retorna el índex
       */
int formulariPrincipal::buscaFiguraSolucionadaEnLlista(QString nom){
    QString nom1=QString("%1;%2;%3").arg(configuracioJoc->tipusTangram).arg(configuracioJoc->nivellJoc).
                      arg(nom);
    QRegExp rx(nom1+"*");
    rx.setPatternSyntax(QRegExp::Wildcard);
    return figuresResoltesList->indexOf(rx);
}


/* 4.0.2
  Elimina la solució de la figura obtinguda per l'usuari
  */
void formulariPrincipal::seleccionFiguraItemDoubleClicked(QTreeWidgetItem * item){
    //si la figura ha estat resolta
   if (item->text(3)=="1" ){
        /* 4.1 28/01/11
           Introduida la nova funció buscaFiguraSolucionadaEnLlista()
           */
          int indexFigura= buscaFiguraSolucionadaEnLlista(
                  arbreFiguresTreeWidget->selectedItems().first()->text(0));
          QStringList dades=figuresResoltesList->value(indexFigura).split(";");
          //Cal evitar que es demani si es vol eliminar les dades de la solució....quan no n'hi ha!
          if (dades.size()>3){
          QMessageBox missatgeBox;
        missatgeBox.setText(configuracioJoc->tradueixCadena("Elimina.solucio.pregunta"));
        missatgeBox.setIcon(QMessageBox::Question);
        missatgeBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        missatgeBox.setDefaultButton(QMessageBox::No);
        if(missatgeBox.exec()==QMessageBox::Yes){
          //Eliminam la figura resolta
          figuresResoltesList->removeAt(indexFigura);
          //I ara l'afegim sense les dades de la solució
          figuresResoltesList->append(
                                QString("%1;").arg(configuracioJoc->tipusTangram)+
                                QString("%1;").arg(configuracioJoc->nivellJoc)+
                                QString("%1").arg(arbreFiguresTreeWidget->selectedItems().first()->text(0)));
          desaFiguresResoltes();
          conjuntJoc->colocaPecesAtzar();
         }
        }
  }
}

/*
Gestiona la selecció d'una figura a l'arbre
(conectat amb arbreFiguresTreeWidget::itemSelectionChanged ()
*/
void formulariPrincipal::seleccioFigura(){

if (!arbreFiguresTreeWidget->selectedItems().isEmpty()){
 missatgeStatusBar("");
 QTreeWidgetItem *item;
 item=arbreFiguresTreeWidget->selectedItems().first();
 //S'ha clicat el nus arrel
 if (item->childCount()>0){
    canviaEstatDelJoc(Inactiu);
    conjuntMostra->conjuntVisible(false);
    conjuntFigura->conjuntVisible(false);
    creaFiguraMenu->setEnabled(true);
     }
 else if (!item->text(1).isEmpty()){
    canviaEstatDelJoc(FiguraCarregada);
    conjuntMostra->colocaPeces(item->text(1));
 //qCritical("%s", qPrintable(item->text(1)));
    conjuntMostra->conjuntVisible(true);
    conjuntMostra->conjuntDeMostra();
    conjuntFigura->colocaPeces(item->text(1));
    conjuntFigura->conjuntDeMostra(); //&& item->text(4)=="0"
   if(configuracioJoc->nivellJoc==NIVELL_EXPERT ){
    conjuntFigura->conjuntVisible(false);
    }
   conjuntJoc->colocaPecesAtzar();
   //La figura ja ha estat resolta per l'usuari
    if (item->text(3)=="1"){
     canviaEstatDelJoc(FiguraResolta);
      //Carregam la solució de l'usuari
     conjuntJoc->colocaPeces(figuraJaResoltaPosicioPeces(item->text(0)));
     //restablim l'angle de gir de les peces
     angleGirConjuntPecesJoc();
     }
   // conjuntJoc->colocaPecesAtzar();
   //Això és necessari degut a que, a vegades, queda algun
   //"rastre" de la figura anterior
   sceneMostra->update();
   sceneJoc->update();
  // creaArxiuImatge(item->text(0));
  }
else {
    canviaEstatDelJoc(Inactiu);
    conjuntMostra->conjuntVisible(false);
    conjuntFigura->conjuntVisible(false);
    conjuntJoc->colocaPecesAtzar();
   }
 }


}
/*
  Crea un arxiu amb la imatge actual del conjuntMostra
  */
void formulariPrincipal::creaArxiuImatge(QString nomArxiu){
if (!nomArxiu.isEmpty()){
    QRectF rectFigura =conjuntMostra->rectangleFigura();
    QImage *image1 = new QImage(rectFigura.width(),
                                rectFigura.height(),
                                QImage::Format_ARGB32_Premultiplied);
    QPainter *painter= new QPainter(image1);
    conjuntMostra->conjuntSolucio();
    sceneMostra->update();
    QRectF target;
    sceneMostra->render(painter,target,rectFigura);
    painter->end();
    QString  nomArxiuImatge;
    nomArxiuImatge=QDir::homePath ()+QDir().separator()+"tangram"+QDir().separator()+nomArxiu+".png";
    QFile arxiu(nomArxiuImatge);
    arxiu.remove(nomArxiuImatge);
    if(!image1->save(nomArxiuImatge)){
        qDebug("no s'ha desat");
    }
  }
}

void formulariPrincipal::carregaArbreFigures(QString nomArxiu){
QApplication::setOverrideCursor(Qt::WaitCursor);

carregaContadors();

canviaEstatDelJoc(Inactiu);
//netejam
arbreFiguresTreeWidget->clear();
arbreFiguresTreeWidget->setColumnCount(4);
arbreFiguresTreeWidget->setColumnHidden(1,true);
arbreFiguresTreeWidget->setColumnHidden(2,true);
arbreFiguresTreeWidget->setColumnHidden(3,true);
arbreFiguresTreeWidget->setColumnHidden(4,true);
//1: visible la mostra de la figura al nivell 4
//arbreFiguresTreeWidget->setColumnHidden(5,true);

//posam la capçalera de l'arbre
QTreeWidgetItem *item=new QTreeWidgetItem();
//13/02/11 Canviat per eliminar icones del resources.qrc
item->setIcon(0,QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+configuracioJoc->nomIcone));


item->setText(0,configuracioJoc->tradueixCadena(configuracioJoc->nomJocArbreFigures));
arbreFiguresTreeWidget->setHeaderItem(item);
llegeixArxiuFigures(nomArxiu);
//actualitzaMenus();
QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void formulariPrincipal::llegeixArxiuFigures( QString nomArxiu){
  //obrim l'arxiu de figures
  QFile arxiuFigures(nomArxiu);
    if (!arxiuFigures.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::information(this, "Peces (" + configuracioJoc->tradueixCadena("Jocs.tangram")+")",
                                configuracioJoc->tradueixCadena("ArxiuNoTrobat").arg(nomArxiu) );
         return ;
     }

  QSettings *controlArxiu;
  controlArxiu=new QSettings(QSettings::IniFormat, QSettings::UserScope,"De Marchi", "NomFigures");
  controlArxiu->clear();

     QTextStream entrada(&arxiuFigures);
     QString linia0 = entrada.readLine();
     QString linia1,linia2;
     bool figuraComposta;

     QTreeWidgetItem *arrel=new QTreeWidgetItem();
     int numFig, intPosDarrerPunt;
     while (!linia0.isNull()) {
         linia1=entrada.readLine();
         linia2=entrada.readLine();
            //Controla si cal NO carregar la figura al nivell expert
            //(a les figures "compostes")
            if (linia2.length()==5){
               if (linia2.mid(4,1)=="1"){
                   figuraComposta=true;}
               else figuraComposta=false;
           }
            else figuraComposta=false;

         //El nivell iniciat també carrega les figures del nivell expert
         //(a partir de la versió 4.0)
         if ( ( (linia2.mid(0,1).toInt()<=configuracioJoc->nivellJoc) ||
              configuracioJoc->nivellJoc== NIVELL_INICIAT ) &&
              //les figures "compostes" no son visibles al nivell expert
              !(figuraComposta && configuracioJoc->nivellJoc==NIVELL_EXPERT)  ){
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
            cadena=configuracioJoc->tradueixCadena(linia0.mid(0,intPosDarrerPunt))+" "+cadena;
             }
            else cadena=linia0;

            //Això per botar-se la traducció
            //cadena=linia0;
            item->setText(0,cadena);
            numFig++;
            controlArxiu->setValue(linia0.mid(0,intPosDarrerPunt),numFig);
            item->setText(1,linia1);
            item->setText(2,linia2);
            //controla si  la figura ja s'ha resolt
            item->setText(3,"0");
            item->setIcon(0,iconeFigura(linia2.mid(2,1).toInt()));
            //miram si la figura ja ha estat resolta
            if (figuraJaResolta(cadena)){
               item->setIcon(0,iconeFigura(4));
               item->setText(3,"1");
            }
           arrel->addChild(item);
         }
         linia0 = entrada.readLine();
     }
arbreFiguresTreeWidget->addTopLevelItem(arrel);
arbreFiguresTreeWidget->sortItems(0,Qt::AscendingOrder);
//arbreFiguresTreeWidget->sortByColumn(0,Qt::AscendingOrder);
arrel->setText(0,configuracioJoc->tradueixCadena(
              /* 08/06/14 Eliminat .toAscii-> deixa d'existir
               * a Qt 5.0*/
                                  configuracioJoc->nomNivellArbreFigures) //.toAscii()

                                  +" ("+QString::number(arrel->childCount()) +")");

arbreFiguresTreeWidget->expandAll();
controlArxiu->clear();

//Numero de figures
/*numeroFigures =numeroFigures+arrel->childCount();
qDebug("Numero de figures: %d",arrel->childCount());
*/
}

/*
  Slots d'eliminació de records
*/
void formulariPrincipal::eliminaRecordSlot(){
    QMessageBox missatgeBox;
        missatgeBox.setText(configuracioJoc->tradueixCadena("Elimina.record.pregunta"));
        missatgeBox.setIcon(QMessageBox::Question);
        missatgeBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        missatgeBox.setDefaultButton(QMessageBox::No);
        if(missatgeBox.exec()==QMessageBox::Yes){
          canviaEstatDelJoc(Inactiu);
          rellotgeLabel->estableixTemps("00:00:00");
          configuracioJoc->temps=rellotgeLabel->retornaTemps(1);
          actualitzaPuntuacio(-configuracioJoc->comptadorFigures,
                        -configuracioJoc->comptadorPunts);
          eliminaFiguresResoltes(1);
          desaFiguresResoltes();
          carregaArbreFigures(configuracioJoc->directoriArxiuDadesFigures);
        }
    }

void formulariPrincipal::eliminaTotsRecordsSlot(){
QMessageBox missatgeBox;
missatgeBox.setText(configuracioJoc->tradueixCadena("Elimina.tots.records.pregunta"));
missatgeBox.setIcon(QMessageBox::Question);
missatgeBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
missatgeBox.setDefaultButton(QMessageBox::No);
if(missatgeBox.exec()==QMessageBox::Yes){
    canviaEstatDelJoc(Inactiu);
    rellotgeLabel->estableixTemps("00:00:00");
    configuracioJoc->temps=rellotgeLabel->retornaTemps(1);
    actualitzaPuntuacio(-configuracioJoc->comptadorFigures,
                        -configuracioJoc->comptadorPunts);
    int registre=configuracioJoc->configArxiu->value("joc/registre",0).toInt();
    QString locale =configuracioJoc->configArxiu->value("joc/idioma",QLocale::system().name().mid(0,2)).toString();
    configuracioJoc->configArxiu->clear();
    configuracioJoc->configArxiu->setValue("joc/tipusTangram", configuracioJoc->tipusTangram);
    configuracioJoc->configArxiu->setValue("joc/nivellJoc", configuracioJoc->nivellJoc);
    configuracioJoc->configArxiu->setValue("joc/registre", registre);
    configuracioJoc->configArxiu->setValue("joc/idioma", locale);
    eliminaFiguresResoltes(0);
    desaFiguresResoltes();
    carregaArbreFigures(configuracioJoc->directoriArxiuDadesFigures);
  }
}


void formulariPrincipal::carregaFiguresProgramaSlot(){
//configuracioJoc->carregaPuntuacio();
configuracioJoc->directoriArxiuDadesFigures=configuracioJoc->ArxiuDadesFigures();
/*11/05/14 això no estava actualitzat pel nou directori "fig"
       directoriLinux()+charPathSeparator+"figures"+charPathSeparator+configuracioJoc->arxiuDadesFigures;
       */
//configuracioJoc->directoriArxiuDadesFigures=QCoreApplication::applicationDirPath ()+
  //                   charPathSeparator+"figures"+charPathSeparator+configuracioJoc->arxiuDadesFigures;
carregaArbreFigures(configuracioJoc->directoriArxiuDadesFigures);
configuracioJoc->desaRecords=true;
//qCritical("%s", qPrintable(configuracioJoc->directoriArxiuDadesFigures));
 actualitzaMenus();
}

void formulariPrincipal::carregaFiguresArxiuSlot(){
    configuracioJoc->directoriArxiuDadesFigures = QFileDialog::getOpenFileName(this,
                                 configuracioJoc->tradueixCadena("carrega.arxiu.figures"),
                                 directoriFiguresPersonals(),
                                 filtreArxiusFiguresGeneradesAtzar(configuracioJoc));
                                // configuracioJoc->arxiuDadesFigures);

    if(configuracioJoc->directoriArxiuDadesFigures.isEmpty()){
       carregaFiguresProgramaSlot();
       }
    else{
   carregaArbreFigures(configuracioJoc->directoriArxiuDadesFigures);
   configuracioJoc->desaRecords=false;
   reiniciaContadors();
   conjuntFigura->conjuntVisible(false);
   conjuntMostra->conjuntVisible(false);
   }
 actualitzaMenus();
}

void formulariPrincipal::creaTangram(int tipusTangram){
    if (tangramActual !=tipusTangram){
        canviaEstatDelJoc(Inactiu);
        configuracioJoc->canviaTipusTangram(tipusTangram);
        tangramActual=configuracioJoc->tipusTangram;

        nomFiguraPerCercar.clear();
        indexFiguraPerCercar=0;
        //eliminam les peces actuals
        sceneJoc->clear();
        sceneMostra->clear();


        conjuntJoc= new ConjuntPeces(tipusTangram,ESCALA_GENERAL);
        carregaColorsPersonalitzatsPeces();
        conjuntMostra=new ConjuntPeces(tipusTangram,ESCALA_MOSTRA);
        conjuntMostra->conjuntDeMostra();
        conjuntMostra->conjuntVisible(false);
        conjuntFigura=new ConjuntPeces(tipusTangram,ESCALA_GENERAL);
        conjuntFigura->conjuntDeMostra();
        conjuntFigura->conjuntVisible(false);

       //ara afegim les peces a l'scena
        for(int i=0;i<=conjuntJoc->arrayPeces.size()-1;i++){
            conjuntJoc->arrayPeces[i]->setZValue(0);
            conjuntJoc->arrayPeces[i]->estableixAngleGirPeca(configuracioJoc->angleGir);
            conjuntFigura->arrayPeces[i]->estableixAngleGirPeca(configuracioJoc->angleGir);
            sceneJoc->addItem(conjuntJoc->arrayPeces[i]);
            sceneJoc->addItem(conjuntFigura->arrayPeces[i]);
            sceneMostra->addItem(conjuntMostra->arrayPeces[i]);
            }
        //Cal posar això aqui degut a que cal tenir les dades
        //de l'scene!!
        // conjuntJoc->colocaPecesAtzar();
        //20/02/11 Inicialment, les peces es col·loquen
        //formant la figura inicial
        figuraInicialTangram(tipusTangram);
        //Carregam l'arbre de figures
        carregaArbreFigures(configuracioJoc->directoriArxiuDadesFigures);
        //Opcions de generació automàtica de noves figures
        controlaActivacioMenusFiguresAtzar();
        //qDebug("tipus tangram %d", tipusTangram);
        }
}//final formulariPrincipal::creaTangram(int tipusTangram)

void formulariPrincipal::carregaColorsPersonalitzatsPeces(){
 for(int i=0;i<conjuntJoc->arrayPeces.size();i++){
    QString color=configuracioJoc->configArxiu->value(QString("%1/%2")
                              .arg(configuracioJoc->tipusTangram)
                              .arg(i),"").toString();
    if ( !color.isEmpty()){
        conjuntJoc->arrayPeces[i]->setColorPeca(QColor(color));
        conjuntJoc->arrayPeces[i]->setColorLimits(QColor(color));
      }
    }
}

/* 20/02/11
  Col·loca les peces en la figura
  "inicial" del tangram*/
void formulariPrincipal::figuraInicialTangram(int tipusTangram){
//return;
    //NOU TANGRAM 2
    QStringList dadesFiguraInicial;
    dadesFiguraInicial.append("55.0 4 26 78 3.9269908169872414 0 5 137 58 -0.7853981633974483 0 2 40 142 2.356194490192345 0 4 78 26 5.497787143782138 0 2 105 77 7.0685834705770345 0 1 79 116 -0.7853981633974483 0 3 130 129 4.71238898038469 0");
    //tangram clàssic                                       codi 1
    dadesFiguraInicial.append("55.0 6 28 83 -1.5707963267948966 0 6 83 28 0.0 0 4 128 74 4.71238898038469 0 7 82 138 -0.7853981633974483 1 2 147 147 4.71238898038469 0");
    //Tangram 5                                                 codi 2
    dadesFiguraInicial.append("58 10 18.2276 18.2277 1.5708 0 3 136.707 54.6829 4.71239 0 3 54.683 136.707 4.71239 0 11 91.1382 36.4553 1.5708 0 8 123.037 123.037 3.14159 0 9 27.3415 82.0244 -0.785398 0");
    //Tangram 6 I                                           codi 3
    dadesFiguraInicial.append("55.0 8 39 118 1.5707963267948966 1 12 117 79 -1.5707963267948966 1 3 130 131 0.0 1 3 26 53 1.5707963267948966 1 1 78 40 2.356194490192345 1 2 39 14 0.7853981633974483 1 2 117 14 0.7853981633974483 1");
    //Tangram de Fletcher (Pitàgores)                      codi 4
    dadesFiguraInicial.append("58 3 27.3416 27.3415 1.5708 0 3 136.707 27.3415 3.14159 0 13 20.5061 102.531 5.49779 0 14 123.037 82.0244 -8.63938 1 15 143.543 164.049 1.5708 0 15 20.5061 164.049 -1.5708 1 16 82.0244 143.543 3.14159 0");
    //Tangram del xinès també Tangram Pitagòric    codi 5
    dadesFiguraInicial.append("58 1 82.0244 41.0122 -0.785398 0 2 123.037 13.6708 5.49779 0 3 136.707 136.707 4.71239 0 3 27.3415 27.3415 1.5708 0 5 61.5183 143.543 -0.785398 1 13 20.5061 102.531 5.49779 0 17 123.037 82.0244 -0.785398 0");
    //Tangram de la saviesa Chie-No-Ita                   codi 6
    dadesFiguraInicial.append("58 1 29 145 2.18557e-08 0 2 19.3333 19.3334 1.5708 0 2 19.3333 96.6667 -5.01216e-09 0 3 77.3334 58 3.92699 0 3 38.6667 58 0.785398 0 3 116 19.3334 5.49779 0 13 87 116 -1.5708 1 14 145 87 -10.9956 1");
    //Tangram 8                                         codi 7
    dadesFiguraInicial.append("58 18 123.037 47.3568 -1.0472 0 18 82.0244 165.749 -1.19209e-07 0 18 41.0122 189.427 3.14159 0 19 191.39 189.427 3.14159 0 19 95.6951 118.392 3.14159 1 19 218.732 189.427 3.14159 1 19 68.3536 118.392 3.14159 0 20 143.543 106.553 4.18879 1 20 143.543 177.588 2.0944 0");
    //Tangram 9                                codi 8
    dadesFiguraInicial.append("58 17 29.0001 116.095 1.5708 0 21 91.2183 86.5679 -1.5708 0 21 124.964 144.568 1.5708 0 23 83.782 188.804 3.66519 0 22 124.964 202.568 -1.5708 0 22 92.2183 29.5679 1.5708 0 23 134.639 43.4999 0.523599 0 2 177.691 67.5861 0 0 1 188.237 116.095 0 0 2 178.746 164.604 1.5708 0 1 3 0");
   // dadesFiguraInicial.append("55.0 17 28 111 1.5707963267948966 0 21 87 83 -1.5707963267948966 0 21 119 138 1.5707963267948966 0 23 79 179 1.5707963267948961 1 22 119 193 -1.5707963267948966 0 22 87 28 1.5707963267948966 0 23 127 42 4.71238898038469 1 2 169 65 5.551115123125783E-17 0 1 179 111 0.0 0 2 170 157 1.5707963267948966 0");
    //Tangram 10                              codi 9
    dadesFiguraInicial.append("58 23 150.688 58.8657 2.61799 0 23 100.459 29.8657 2.61799 0 23 50.2295 58.8657 -0.523599 0 22 184.508 29.2886 -0.523599 0 22 83.5492 87.5771 1.5708 0 22 16.9098 29.5771 0.523599 0 22 184.508 87.8657 3.66519 0 24 33.4863 97.5323 -1.5708 0 24 133.945 97.5324 -1.5708 0 25 50.7293 10.2438 0 1 25 151.188 10.2438 0 0");
    //Tangram 11                        codi 10
    dadesFiguraInicial.append("58 1 147.644 98.4295 0.785398 0 2 98.4294 32.8098 2.35619 0 2 98.4291 65.6197 5.49779 0 2 32.8098 98.4294 0.785401 0 3 164.049 164.049 -3.33285e-06 1 13 73.8221 123.037 -2.35619 1 15 24.6075 49.2149 1.57079 1 26 180.454 16.405 3.14159 0 26 16.4052 180.454 1.57079 1 26 65.6196 16.405 3.14159 1 27 36.9114 159.948 0.785398 1 27 159.947 36.9111 3.92699 0");
    //Tangram rus                         codi 11
    dadesFiguraInicial.append("55.0 2 39 143 2.356194490192345 0 2 78 130 5.497787143782138 0 2 117 143 2.356194490192345 0 2 143 117 7.0685834705770345 0 2 78 104 2.356194490192345 0 2 13 117 3.9269908169872414 0 1 39 78 3.9269908169872414 0 1 78 39 3.9269908169872414 0 2 143 39 7.0685834705770345 0 2 117 13 5.497787143782138 0 2 13 39 3.9269908169872414 0 2 39 13 5.497787143782138 0 1 117 78 3.9269908169872414 0");
    //Tangram 13                               codi 12
    dadesFiguraInicial.append("58 1 29 29 3.14159 1 1 145 203 -1.5708 1 2 77.3333 19.3334 3.14159 1 2 19.3333 96.6667 1.5708 1 2 193.333 212.667 1.5708 1 2 135.333 135.333 3.14159 1 3 212.667 174 2.35619 1 3 96.6667 58 2.35619 1 4 77.3334 154.667 -1.5708 1 4 38.6667 193.333 1.5708 1 4 193.333 77.3334 5.01216e-09 1 4 154.667 38.6667 3.14159 1 5 58 87 3.14159 1 5 174 145 0 0");
    //Tangram 14                             codi 13
    dadesFiguraInicial.append("55.0 28 186 74 3.141592653589784 1 30 122 52 4.031710572106918 1 29 76 89 -0.680678408277775 1");
    //Tangram de Brügner                                         codi 14
    dadesFiguraInicial.append("58 31 58 100.509 1.04719 0 32 203.669 100.459 1.04719 0 21 116.03 33.5312 4.18879 0 34 87.3111 184.046 -1.0472 0 35 145.629 150.711 0 0 36 167.409 62.7866 5.23598 0 1 3 0");
    //Tangram Hexagrama                                 codi 15
    dadesFiguraInicial.append("58 37 45.9878 157.012 3.92699 0 37 128.012 157.371 2.35619 1 38 74.9878 203.358 0.785398 0 38 99.0122 203.358 1.48578e-08 0 39 58 116.358 -1.5708 1 39 116 116.358 1.5708 0 40 117.759 56.5993 -2.35619 0 40 56.2409 56.2409 2.35619 0 41 87 75.3463 3.14159 0");
    //Tangram de l'ou                          codi 16
    dadesFiguraInicial.append("58 42 41.8244 30.6586 5.49779 0 42 14.483 58 3.92699 0 42 150.92 57.1878 0.785398 0 43 137.519 24.9959 3.92699 0 43 110.178 24.9959 3.14159 0 13 103.343 119.518 5.49779 0 1 82.8366 58 3.92699 0 2 28.1537 99.0122 0.785398 0 5 62.3305 119.518 3.92699 1");
    //Tangram del cor                             codi 17
    dadesFiguraInicial.append("58 44 174.352 107.124 -1.7308e-08 1 46 174.352 64.9424 1.5708 1 55 142.715 144.033 3.14159 0 45 147.988 21.7061 4.05327e-08 1 47 105.806 28.0333 1.5708 0 54 95.2606 41.7424 3.14159 1 48 42.5333 11.1606 7.04857e-08 0 51 10.897 74.4333 4.71239 1 52 31.9879 95.5242 4.71239 1 45 42.5333 170.397 3.14159 1 48 53.0788 118.724 1.5708 1 53 86.8242 128.215 -3.94854e-08 1 49 116.352 117.67 -1.5708 0 50 132.17 86.0333 3.94854e-08 1");
    ///Tangram Stomachion o Luculus Archimedius                              codi 18
    dadesFiguraInicial.append("58 56 87 140.024 -4.74932e-07 0 2 19.3335 202.715 -5.41454e-07 0 58 49.5062 172.543 2.35619 1 59 29.0001 82.0244 3.14159 1");
    //Tangram de 4 peces I                                     codi 19
    dadesFiguraInicial.append("58 60 96.6668 77.3333 -1.5708 0 60 135.333 77.351 1.5708 1 60 96.6666 154.667 4.71239 1 60 135.333 154.684 1.5708 0 61 212.666 82.5311 -1.57079 0 61 212.666 149.504 4.71239 1 62 135.333 43.8647 4.71239 0 62 135.333 188.171 -1.5708 1 63 57.9999 57.9999 1.5708 1 63 58 174 1.5708 0");
    //Tangram circular I                   codi 20
    dadesFiguraInicial.append("58 21 58.0003 66.9729 -1.0472 0 21 116 33.4866 2.0944 0 24 164.333 150.688 2.0944 0 24 154.667 100.459 -1.0472 0 64 116 171.918 2.43431e-07 0 65 188.5 75.3446 1.0472 1 66 58.0001 133.946 3.14159 1");
    //Tangram 7 I                                    codi 21
    dadesFiguraInicial.append("58 22 58.0002 84.049 3.14159 0 64 58.0001 121.689 8.29453e-07 1 65 116 100.459 1.0472 0 67 106.333 33.4863 2.0944 0 67 20.4523 83.7161 3.14159 1 67 154.667 83.7159 3.14159 0 68 50.75 37.6721 1.0472 0 69 87.5595 175.642 3.14159 1");
    //Tangram circular de 8 peces CII             codi 22
   // dadesFiguraInicial.append("58 22 58.0002 84.049 3.14159 0 64 58.0001 121.689 8.29453e-07 1 65 116 100.459 1.0472 0 67 106.333 33.4863 2.0944 0 67 20.4523 83.7161 3.14159 1 67 154.667 83.7159 3.14159 0 68 50.75 37.6721 1.0472 0 69 87.5595 175.642 3.14159 1");
    dadesFiguraInicial.append("58 1 41.0122 82.0244 0.785398 0 5 102.531 20.5061 2.35619 1 3 27.3415 27.3415 1.5708 0 70 147.644 123.037 8.17162e-08 0 71 123.037 65.6195 4.71239 1 72 41.0122 147.644 1.5708 0");
    //Tangram 6 II                                         codi 23
    dadesFiguraInicial.append("58 1 82.0244 41.0127 2.35619 0 5 61.5183 102.53 3.92699 0 3 136.707 27.342 3.14159 0 2 13.6708 123.037 3.92699 0 70 16.4051 41.0123 3.14159 0 13 143.543 102.53 0.785398 1 73 82.0244 143.543 -3.17868e-09 0");
    //Tangram 7 II Revathi                               codi 24
    dadesFiguraInicial.append("58 3 136.707 27.3415 3.14159 0 3 27.3415 136.707 -1.83826e-07 0 13 102.531 143.543 3.92699 0 16 41.0122 61.5184 -5.41454e-07 0 12 123.037 82.0244 1.5708 0");
    //Tangram 5 I Regulus                                    codi 25
    dadesFiguraInicial.append("58 3 27.3418 27.3416 1.5708 0 3 136.707 136.707 4.71239 0 13 61.5186 143.543 2.35619 1 2 123.037 13.6709 5.49779 0 12 41.0125 82.0245 1.5708 1 5 102.53 61.5184 0.785398 1 5 143.543 61.5184 2.35619 0");
    //Tangram 7 Heptex                                    codi 26
    dadesFiguraInicial.append("58 78 92.1242 92.5396 1.09956 0 74 128.48 12.6447 -0.471237 0 75 122.275 153.54 5.91034e-07 1 76 165.208 55.3015 1.09956 0 77 18.3412 74.0608 1.5708 0");
    //Tangram de Sam Lloyds                                codi 27
    /*Canviat degut al recalcul de les peces 79 i 80 29/04/14*/
    //dadesFiguraInicial.append("58 79 141.375 56.5082 2.0944 0 79 119.625 131.852 -2.0944 0 80 179.437 28.2541 -1.0472 0 80 65.25 56.5082 3.14159 0 81 210.25 138.131 3.66519 1 82 29 100.459 1.5708 1 83 239.25 87.9016 2.0944 1 84 168.562 103.598 1.0472 0 84 92.4375 103.598 -1.0472 0");
    dadesFiguraInicial.append("58 79 130.5 50.2301 3.14159 0 79 130.501 125.574 3.14159 0 80 65.2502 37.6726 4.18879 0 80 195.75 37.6725 4.18879 0 81 210.25 138.132 3.66519 1 82 29.0011 100.459 1.5708 1 83 239.251 87.9024 2.0944 1 84 92.4379 103.599 -1.0472 0 84 168.563 103.599 1.0472 0");
    //Tangram "los nueve tangram"              codi 28
     dadesFiguraInicial.append("58 74 122.968 92.1677 -2.04203 0 74 128.478 12.645 -0.471239 0 74 56.3216 172.154 2.67035 0 74 12.6449 56.3216 -2.04204 0 74 172.154 128.478 1.09956 0 76 19.3154 129.638 -2.04203 0 76 129.638 165.484 2.67035 0 76 92.3997 92.3996 1.09956 0 76 165.484 55.1617 1.09956 0 76 55.1616 19.3154 -0.471236 0");
    //Tangram Egipci                       codi 29
    dadesFiguraInicial.append("58 3 136.707 27.3415 3.14159 0 85 61.5183 41.0122 3.14159 1 85 123.037 102.53 -1.5708 0 86 61.5183 102.53 -4.76837e-07 0");
    //Tangram de 4 peces II                                    codi 30
    dadesFiguraInicial.append("58 3 156.601 191.4 2.35619 0 74 130.501 165.3 5.49779 1 87 78.3006 182.7 2.35619 1 88 60.9 8.70023 2.35619 1 89 139.2 11.6002 3.14159 1 90 174 58.0002 2.35619 0 90 139.2 46.4003 -0.785398 0 91 87.0002 63.8003 0.785398 1 91 17.4002 40.6002 3.92699 1 91 17.4008 168.201 -0.785398 0 92 52.2003 104.4 0.785398 1 93 17.4008 133.401 3.92699 1 94 156.6 133.4 0.785398 0 95 191.4 121.8 2.35619 1");
    //Tangram Stomachion rectangular                                          codi 31
    dadesFiguraInicial.append("58 54 64.4789 119.625 -1.07875e-06 1 45 21.9455 53.1671 -1.5708 0 46 160.179 31.9001 3.14159 0 47 138.912 153.845 1.5708 0 48 138.912 106.334 -1.67933e-07 0 46 128.279 175.451 3.14159 1 76 48.1893 143.212 -2.38419e-07 0 90 86.3487 21.4178 3.14159 1 53 128.279 71.7754 1.5708 0 45 170.812 138.234 1.5708 0 48 53.8456 85.067 3.14159 0");
    //Tangram Stomach (modificació de Stomachion)                                                   codi 32
    dadesFiguraInicial.append("58 15 43.5 159.5 2.35619 1 15 130.5 14.5 -0.785398 1 15 159.5 130.5 0.785398 1 15 14.5 43.5 -2.35619 1 3 58 19.3334 5.49779 0 3 154.667 58 2.35619 1 3 19.3333 116 -0.785398 1 3 87 67.6667 2.35619 0 3 116 154.667 2.35619 0 3 87 106.333 5.49779 0");
    //Tangram Elzzup (10 peces)                  codi 33
    dadesFiguraInicial.append("58 31 174.577 100.459 1.0472 0 32 160.077 226.033 4.18879 1 34 29.2886 284.466 1.0472 0 35 87.5772 251.147 3.14159 0 36 174.577 276.261 3.14159 0 21 116.577 167.431 1.0472 0 33 268.827 213.475 1.0472 0 57 225.327 238.59 1.0472 1");
    //Tangram dels vuit elements (8 peces)         codi 34
    dadesFiguraInicial.append("58 1 130.5 43.5003 5.96046e-08 0 2 29.0002 116 1.5708 0 2 29.0001 58.0002 -3.27826e-07 0 2 58.0001 29.0002 3.14159 0 3 203 87.0003 3.92699 0 13 87.0001 130.5 3.27826e-07 0 15 282.75 152.25 3.92699 0 26 217.5 159.5 2.35619 0 96 261 72.5 -3.57628e-07 0 96 275.5 87 1.5708 0 97 261 32.625 -5.96046e-08 0 97 315.375 87 1.5708 0");
    //Tangram de 12 peces         codi 35
    dadesFiguraInicial.append("58 3 154.667 116 0.785398 0 1 29.0001 145 1.5708 0 16 130.5 43.5 3.92699 0 4 38.6669 38.6667 1.5708 0 98 87.0003 116 3.14159 1");
    //Tangram 5 II         codi 36
    dadesFiguraInicial.append("58 3 150.378 27.3415 1.5708 0 3 177.719 136.707 4.71239 0 13 102.531 61.5185 2.35619 0 14 164.049 82.0245 5.49779 0 15 41.0125 61.5187 3.14159 0 15 41.0122 102.53 1.49012e-07 1 99 41.0125 20.5067 -5.36442e-07 0 99 41.0122 143.543 3.14159 1");
    //Tangram Armonigrama         codi 37
    dadesFiguraInicial.append("58 3 226.2 125.668 2.35619 0 3 150.8 100.534 5.49779 0 13 75.4 37.7008 -3.27826e-07 1 1 37.7002 113.101 1.5708 0 2 276.467 25.1338 3.14159 0 2 276.466 100.534 -1.5708 1 2 100.533 125.667 -8.9407e-08 0 12 188.5 37.7003 3.92699 0");
    //Tangram rectangular         codi 38
    dadesFiguraInicial.append("58 2 41.0122 13.6709 5.49779 0 2 82.0245 27.3417 2.35619 0 2 123.037 13.6707 5.49779 0 2 13.6708 41.0125 3.92699 0 2 82.0246 109.366 2.35619 0 2 54.6831 82.0246 3.92699 0 2 136.707 82.0245 3.92699 0 2 27.3416 82.0246 0.785398 0 2 41.0125 150.378 2.35619 0 2 82.0246 54.6831 5.49779 0 2 13.6709 123.037 3.92699 0 2 109.366 82.0246 0.785398 0 2 123.037 150.378 2.35619 0 2 150.378 41.0122 0.785398 0 2 150.378 123.037 0.785398 0 2 82.0246 136.707 5.49779 0");
    //Tangram de 16 peces (només triangles) derivat del tangram clàssic codi 39
    dadesFiguraInicial.append("58 31 58 100.476 -2.0944 0 21 116.03 167.459 1.0472 0 21 116.02 33.4933 4.18879 0 23 145.05 100.466 -2.0944 0 23 174.06 50.2294 -2.0944 0 22 203.464 117.328 2.0944 0 22 203.459 83.487 1.0472 0 22 174.144 134.206 -1.0472 0 22 174.311 168.066 2.0944 0");
    //Tangram de 9 peces derivat del tangram nou (afegit 01/09/15)  codi 40
    conjuntJoc->colocaPeces(dadesFiguraInicial.value(tipusTangram-1));
    angleGirConjuntPecesJoc();
}


/*
  Slots de nivell de Joc
*/
void formulariPrincipal::nivellInfantilSlot(){
    canviaNivellJoc(NIVELL_INFANTIL);
}
void formulariPrincipal::nivellPrincipiantSlot(){
    canviaNivellJoc(NIVELL_PRINCIPIANT);
}
void formulariPrincipal::nivellIniciatSlot(){
    canviaNivellJoc(NIVELL_INICIAT);
}
void formulariPrincipal::nivellExpertSlot(){
    canviaNivellJoc(NIVELL_EXPERT);
}

//Fa els canvis de nivell de joc
void formulariPrincipal::canviaNivellJoc(int pnivellJoc){
 //només es mouran les peces si el canvi de nivell és
 //cap avall
 /*
   19/11/11
   Amb la nova funció de xoc de les peces
   (que s'elimina en el nivell infantil degut
   a que dificulta molt el joc)
   és millor fer que cada vegada s'executi
   colocaPecesAtzar
   (Linies comentades amb //)
   */
//bool pecesAtzar=true;
missatgeStatusBar("");
 if (nivellJocActual !=pnivellJoc){
    // if (nivellJocActual>pnivellJoc){pecesAtzar=true;}
   canviaEstatDelJoc(Inactiu);
   nivellJocActual=pnivellJoc;
   pecaSeleccionadaNivellInfantil=-1;
   configuracioJoc->canviaNivellJoc(pnivellJoc);
   nomFiguraPerCercar.clear();
   indexFiguraPerCercar=0;
   conjuntMostra->conjuntVisible(false);
   conjuntFigura->conjuntVisible(false);

   /* 4.0.2 21/12/10 Nou procediment */
   angleGirConjuntPecesJoc();
   ////if (pecesAtzar) {conjuntJoc->colocaPecesAtzar();}
   conjuntJoc->colocaPecesAtzar();
   carregaArbreFigures(configuracioJoc->directoriArxiuDadesFigures);
   //02/07/15 El menú principal no es gestionava correctament
   //quan en canviar el nivell de joc.
   controlaActivacioMenusFiguresAtzar();
  }
}

/*
  Estableix l'angle de gir de les peces del conjunt conjuntJoc
  Assegurant que és l'adequat al nivell de joc actual
  */
void formulariPrincipal::angleGirConjuntPecesJoc(){
   for(int i=0;i<=conjuntJoc->arrayPeces.size()-1;i++){
       conjuntJoc->arrayPeces[i]->estableixAngleGirPeca(configuracioJoc->angleGir);
       conjuntJoc->arrayPeces[i]->nivellJoc=
                  configuracioJoc->nivellJoc;
       /* 13/09/15
        * Afegit per la generació a l'atzar de noves figures
        */
       conjuntFigura->arrayPeces[i]->estableixAngleGirPeca(configuracioJoc->angleGir);
       conjuntFigura->arrayPeces[i]->nivellJoc=
                  configuracioJoc->nivellJoc;
       }
}

void formulariPrincipal::actualitzaPuntuacio(int numFig,int afegeixPunts){
configuracioJoc->comptadorFigures=configuracioJoc->comptadorFigures+numFig;
configuracioJoc->comptadorPunts=configuracioJoc->comptadorPunts+afegeixPunts;
numFigures->setText(configuracioJoc->tradueixCadena("Figures")+
                       QString(" %1").arg(configuracioJoc->comptadorFigures ));
punts->setText(configuracioJoc->tradueixCadena("Punts") +
                       QString(" %1").arg(configuracioJoc->comptadorPunts ));
desaContadors();
}

void formulariPrincipal::carregaContadors(){
   //Actualitzam els comptadors amb la puntuació prèvia      
   configuracioJoc->carregaPuntuacio();
   rellotgeLabel->estableixTemps(configuracioJoc->temps);
   numFigures->setText(configuracioJoc->tradueixCadena("Figures")+
                       QString(" %1").arg(configuracioJoc->comptadorFigures ));
   punts->setText(configuracioJoc->tradueixCadena("Punts") +
                  QString(" %1").arg(configuracioJoc->comptadorPunts ));
}

void formulariPrincipal::desaContadors(){
  configuracioJoc->desaPuntuacio(rellotgeLabel->retornaTemps(1));
}

void formulariPrincipal::reiniciaContadors(){
    actualitzaPuntuacio(-configuracioJoc->comptadorFigures,
                        -configuracioJoc->comptadorPunts);
    rellotgeLabel->estableixTemps("00:00:00");
}

 void formulariPrincipal::closeEvent(QCloseEvent *event){
   desaContadors();
   desaFiguresResoltes();
   /* 20/02/12
      Millorat
   configuracioJoc->configArxiu->setValue(
           QString("General/amplada"),this->width());
   configuracioJoc->configArxiu->setValue(
           QString("General/alt"),this->height());
   */

   configuracioJoc->configArxiu->setValue(
           QString("General/geometry"),saveGeometry());
   event->accept();
 }

void formulariPrincipal::carregaFiguresResoltes(){
QString nomArxiu=nomArxiuFiguresResoltes();
QDir dirArxiu;
 if (dirArxiu.exists(nomArxiu)){
     QFile arxiu(nomArxiu);
     if (arxiu.open(QFile::ReadOnly | QFile::Text)){
        QTextStream lectura(&arxiu);
        QString linia0 = lectura.readLine();
        while (!linia0.isNull()) {
            figuresResoltesList->append(linia0);
            linia0 = lectura.readLine();
     }
     }
 }
figuresResoltesList->sort();
}

void formulariPrincipal::desaFiguresResoltes(){
QString nomArxiu=nomArxiuFiguresResoltes();
QDir dirArxiu;
 if (dirArxiu.exists(nomArxiu)){
    dirArxiu.remove(nomArxiu);
}

if (figuresResoltesList->size()<1){
    return;
}
     QFile arxiu(nomArxiu);
     if (arxiu.open(QFile::WriteOnly | QFile::Text)){
     QTextStream entrada(&arxiu);
        for(int i=0;i<=figuresResoltesList->size()-1;i++){
            entrada<<(QString("%1\n").arg(figuresResoltesList->value(i)));
     }
  arxiu.close();
   }
 }

void formulariPrincipal::eliminaFiguresResoltes(int tipus){
    switch(tipus){
        //elimina tot
        case 0:
          figuresResoltesList->clear();
        break;
        //nomès elimina les corresponents al tipusTangram i nivell actual
        case 1:
            QString nom=QString("%1;%2;").arg(configuracioJoc->tipusTangram).arg(configuracioJoc->nivellJoc);
            QRegExp reg(nom+"*",Qt::CaseSensitive,QRegExp::Wildcard);
            while (figuresResoltesList->indexOf(reg)>-1){
                figuresResoltesList->removeAt(figuresResoltesList->indexOf(reg));
            }
        break;
    }
}

//Comprova si el nom de la figura passada per paràmetre
//està a la llista de figures resoltes
bool formulariPrincipal::figuraJaResolta(QString nomFigura){
 QString nom=QString("%1;%2;%3").arg(configuracioJoc->tipusTangram).arg(configuracioJoc->nivellJoc).arg(nomFigura);
 //return (figuresResoltesList->indexOf(nom)>-1);
 /*4.0.2
 Canviat degut a que s'ha afegit les dades de la posició de les peces
 Ara no es pot fer servir el caràcter ? en el nom de les figures
 */
 QRegExp rx(nom+"*");
 rx.setPatternSyntax(QRegExp::Wildcard);
 return (figuresResoltesList->indexOf(rx)>-1);
}

//Comprova si el nom de la figura passada per paràmetre
//ja està resolta i, en cas positiu, retorna
//la cadena amb la posició de les peces
QString formulariPrincipal::figuraJaResoltaPosicioPeces(QString nomFigura){
    if (figuraJaResolta(nomFigura)){
    QString nom=QString("%1;%2;%3").arg(configuracioJoc->tipusTangram).arg(configuracioJoc->nivellJoc).arg(nomFigura);
    QRegExp rx(nom+";*");
    rx.setPatternSyntax(QRegExp::Wildcard);
    QString dadesFigura=figuresResoltesList->value(figuresResoltesList->indexOf(rx));
    QStringList dadesFiguraList=dadesFigura.split(";");
    return dadesFiguraList.value(3);
    }
 return QString("");
}

QString formulariPrincipal::nomArxiuFiguresResoltes(){
    QString directoriHomeConfig = QFileInfo(
            configuracioJoc->configArxiu->fileName()).absolutePath();
    QString nomArxiu=directoriHomeConfig+
                     charPathSeparator+"figres.fig";
return nomArxiu;
}

/*
  Ajuda per resoldre les figures
*/
void formulariPrincipal::mostraSolucioSlot(){
 canviaEstatDelJoc(MostraSolucio);
}

void formulariPrincipal::colocaPecaSlot(){
    configuracioJoc->colocaPeca=!configuracioJoc->colocaPeca;
    colocaPecaAction->setChecked(configuracioJoc->colocaPeca);
}


void formulariPrincipal::colocaPeca(int indexPecaAColocar){
 for(int i=0;i<=conjuntFigura->arrayPeces.size()-1;i++){
     if (conjuntJoc->arrayPeces[indexPecaAColocar]->tipusPeca==
         conjuntFigura->arrayPeces[i]->tipusPeca){
         if(!pecaJaColocada(i)){
             colocaPeca(indexPecaAColocar,i);
             break;
         }
     }
  }
}

/*
  En el nivell infantil, si es col·loca el centre de la peça dins
  de la silueta, llavors el programa col·loca automàticament
  la peça a la seva posició
  */
bool formulariPrincipal::colocaPecaNivellInfantil(int indexPeca){
QPointF centrePeca=conjuntJoc->arrayPeces[indexPeca]->pos()+
                   conjuntJoc->arrayPeces[indexPeca]->centrePeca();
bool resultat= false;
for(int j=0;j<=conjuntFigura->arrayPeces.size()-1;j++){
   if(conjuntFigura->arrayPeces[j]->shape().contains(centrePeca-
                                                     conjuntFigura->arrayPeces[j]->pos())){
       resultat= true;
    }
}
return resultat;
}
/*
  Comprovam si en el conjuntJoc ja hi ha una peça que te les
  mateixes propietats que la peça que volem col·locar
  */
bool formulariPrincipal::pecaJaColocada(int indexPecaAColocar){
bool resultat=false;
 QVector<QPointF> pSolucio;
    conjuntFigura->arrayPeces[indexPecaAColocar]->puntsSolucio(pSolucio,0.9);

int coincidencies=0;
   for(int i=0;i<=conjuntJoc->arrayPeces.size()-1;i++){
      for(int j=0;j<=pSolucio.size()-1;j++){
      if(conjuntJoc->arrayPeces[i]->shape().contains(
         pSolucio.value(j)-conjuntJoc->arrayPeces[i]->pos())){
            coincidencies=coincidencies+1;
        }
     }
     if (coincidencies>=pSolucio.size()-1){
       resultat=true;
       break;
     }
     else coincidencies=0;
   }
return resultat;
}

void formulariPrincipal::colocaPeca(int indexPecaAColocar,int pecaFigura){

conjuntJoc->arrayPeces[indexPecaAColocar]->reiniciaAnglesVertex();

 if(!conjuntJoc->arrayPeces[indexPecaAColocar]->simetriaBool==
   conjuntFigura->arrayPeces[pecaFigura]->simetriaBool){
    conjuntJoc->arrayPeces[indexPecaAColocar]->simetriaPeca();
   }
 conjuntJoc->arrayPeces[indexPecaAColocar]->setPos(
      conjuntFigura->arrayPeces[pecaFigura]->pos());
  conjuntJoc->arrayPeces[indexPecaAColocar]->setAngleRotacio(
      conjuntFigura->arrayPeces[pecaFigura]->anglesVertex[0]-
      conjuntFigura->arrayPeces[pecaFigura]->angleInicial);

  //Aferra peça
  conjuntJoc->arrayPeces[indexPecaAColocar]->setPos(
              conjuntJoc->arrayPeces[indexPecaAColocar]->pos()-
  conjuntJoc->arrayPeces[indexPecaAColocar]->aferraPeca());
//pausa(5);

conjuntJoc->arrayPeces[indexPecaAColocar]->setSelected(false);
}

/*
Generació de noves figures
*/
void formulariPrincipal::comprovaMostraSlot(){
    conjuntFigura->colocaPeces(conjuntJoc->coordenadesFigura());
    //colocam el conjunt al vertex sup.esq per a què es vegi millor
    //(enlloc de centrat);
    conjuntFigura->recolocaPeces(QPointF(50,50));
    conjuntFigura->conjuntVisible(true);
    conjuntFigura->conjuntDeMostra();
    conjuntMostra->colocaPeces(conjuntJoc->coordenadesFigura());
    conjuntMostra->conjuntVisible(true);
    conjuntMostra->conjuntDeMostra();
    sceneJoc->update();
}

void formulariPrincipal::esborraMostraSlot(){
    conjuntFigura->conjuntVisible(false);
    conjuntMostra->conjuntVisible(false);
    missatgeStatusBar("");
}
void formulariPrincipal::desaFiguraSlot(){
   //  int nivellDificultat=calculaNivellDificultatFigura()-1;
     DesaFigura desaFiguraFormulari(configuracioJoc,calculaNivellDificultatFigura()-1,this);
     //passam les dades de la figura al formulari
     desaFiguraFormulari.dadesFigura.append(conjuntJoc->coordenadesFigura());
     desaFiguraFormulari.exec();
}
void formulariPrincipal::simetriaVerticalSlot(){
    conjuntJoc->simetriaVertical();
}
void formulariPrincipal::simetriaHoritzontalSlot(){
    conjuntJoc->simetriaHoritzontal();
}

void formulariPrincipal::cercaFiguraSlot(){
 this->setFocus(Qt::OtherFocusReason);
 if (arbreFiguresTreeWidget->topLevelItem(0)==0){ return;}

 /*10/10/11
   En fet Crtl-F o be amb l'opció de menú, sovint
   no sortia el dialeg de progres. S'ha afegit
   conjuntJoc->arrayPeces[0]->setSelected(false)
   i qApp->processEvents();
   Encara passa...la cosa no està
   gens clara. Un error de QProgressDialog?
   13/11/11
   Probablement, és imprescindible posar
   progres.setVisible(true);
   */
//  conjuntJoc->arrayPeces[0]->setSelected(false);
  qApp->processEvents();
  QProgressDialog progres(
            configuracioJoc->tradueixCadena("Cercant.figura")+"...",
            configuracioJoc->tradueixCadena("Atura.cerca"), 0,
            arbreFiguresTreeWidget->topLevelItem(0)->childCount(), this,0);

   progres.setWindowModality(Qt::WindowModal);
   progres.setVisible(true);
   qApp->processEvents();

missatgeStatusBar("");
qApp->processEvents();
float resultat;
figuresSemblantsList->clear();
    conjuntJoc->centraFiguraScene();
    conjuntFigura->conjuntDeMostra();
    conjuntFigura->conjuntVisible(true);
    qApp->processEvents();
    for(int i=0;i<=arbreFiguresTreeWidget->topLevelItem(0)->childCount()-1;i++){
      //el formulari genera un missatge d'error
      //a la consola (comunicat a Qt)
      progres.setValue(i+1);
      qApp->processEvents();
      conjuntFigura->colocaPeces(arbreFiguresTreeWidget->topLevelItem(0)->child(i)->text(1));
      //això per evitar que quedin "rastres"
      sceneJoc->update();
      qApp->processEvents();
 //Comprovació prèvia per fer més ràpida la recerca
 //if( comparacioLongitudSiluetes()){
 if(comparacioLongitudSiluetesPercentatge(
              conjuntJoc->longitudSilueta(),
              conjuntFigura->longitudSilueta())< 40.0){
       double comparacio=comparaQPainterPath(conjuntJoc->siluetaFiguraEscala(1.005),
                                conjuntFigura->siluetaFiguraEscala(1.005),
                                QRectF(conjuntJoc->rectangleFigura()).center(),
                                QRectF(conjuntFigura->rectangleFigura()).center() );
      //qDebug("comparacio %f",comparacio);
      if ( comparacio>=PERCENTATGE_COINCIDENCIA_BUSCAR_FIGURA){
            afegeixFiguraSemblant(comparacio,i);
       }
      else {
      resultat=figuraSolucionada();
      if (resultat>=PERCENTATGE_COINCIDENCIA_BUSCAR_FIGURA){
          afegeixFiguraSemblant(resultat,i);
      }
      else {
          conjuntFigura->simetriaHoritzontal();
          //això per evitar que quedin "rastres"
          sceneJoc->update();
          qApp->processEvents();
          resultat=figuraSolucionada();
          //qDebug("resultat 1 %f",resultat);
          if (resultat>=PERCENTATGE_COINCIDENCIA_BUSCAR_FIGURA){
              afegeixFiguraSemblant(resultat,i);
          }
          else {
              conjuntFigura->simetriaVertical();
             //això per evitar que quedin "rastres"
             sceneJoc->update();
             qApp->processEvents();
              resultat=figuraSolucionada();
              //qDebug("resultat 2 %f",resultat);
              if (resultat>=PERCENTATGE_COINCIDENCIA_BUSCAR_FIGURA){
                  afegeixFiguraSemblant(resultat,i);
              }
              else {
                  conjuntFigura->simetriaHoritzontal();
                 //això per evitar que quedin "rastres"
                 sceneJoc->update();
                 qApp->processEvents();
                  resultat=figuraSolucionada();
                  //qDebug("resultat 3 %f",resultat);
                  if (resultat>=PERCENTATGE_COINCIDENCIA_BUSCAR_FIGURA){
                    //Aquesta linea fa que algunes vegades, la cerca s'aturi abans d'acabar
                    //  if (arbreFiguresTreeWidget->headerItem()->childCount()<1){ return;}
                      afegeixFiguraSemblant(resultat,i);
                  }
              }
          }
      }
    }
  }//if de comprovació de la longitud de les siluetes
  if (progres.wasCanceled())  break;
  }//for

sceneJoc->update();
qApp->processEvents();
if (figuresSemblantsList->size()>0){
    indexLlistaFiguraSemblant=0;
    figuresSemblantsList->sort();
    figuraSeguentAction->setEnabled(true);
    figuraAnteriorAction->setEnabled(true);
    mostraFiguraSemblant(0);
QMessageBox::information(this, configuracioJoc->tradueixCadena("Cerca.figura"),
                    configuracioJoc->tradueixCadena("msg.FiguraTrobada").arg(figuresSemblantsList->size()));
}
else {conjuntFigura->conjuntVisible(false);
QMessageBox::information(this, configuracioJoc->tradueixCadena("Cerca.figura"),
                    configuracioJoc->tradueixCadena("msg.FiguraNoTrobada"));
}

}


//Afegeix a la llista de figures semblants (creació de figures)
//les dades d'una figura
void formulariPrincipal::afegeixFiguraSemblant(float percentatge,int index){
    QString Missatge;
    Missatge.append(QString("%1;").arg(100-percentatge));
    Missatge.append(QString(arbreFiguresTreeWidget->topLevelItem(0)->child(index)->text(0)));
    Missatge.append(QString(" "));
    Missatge.append(QString("( %1 % ) ;").arg(percentatge));
    Missatge.append(arbreFiguresTreeWidget->topLevelItem(0)->child(index)->text(1));
    figuresSemblantsList->append(Missatge);
}

void formulariPrincipal::figuraSeguentSlot(){
  if (indexLlistaFiguraSemblant<figuresSemblantsList->size()-1){
      indexLlistaFiguraSemblant=indexLlistaFiguraSemblant+1;
      mostraFiguraSemblant(indexLlistaFiguraSemblant);      
      //això per evitar que quedin "rastres"
      sceneJoc->update();
  }
}
void formulariPrincipal::figuraAnteriorSlot(){
  if (indexLlistaFiguraSemblant>0){
      indexLlistaFiguraSemblant=indexLlistaFiguraSemblant-1;
      mostraFiguraSemblant(indexLlistaFiguraSemblant);
      //això per evitar que quedin "rastres"
      sceneJoc->update();
  }
}

void formulariPrincipal::mostraFiguraSemblant(int index){
if (index>-1 && index<=figuresSemblantsList->size()-1) {
  QStringList list1 = figuresSemblantsList->value(index).split(";");
  conjuntFigura->colocaPeces(list1.value(2));
  //colocam el conjunt al vertex sup.esq per a què es vegi millor
  //(enlloc de centrat);
  conjuntFigura->recolocaPeces(QPointF(10,10));
  conjuntFigura->conjuntDeMostra();
  //conjuntFigura->colorPecesConjunt(Qt::black);
  conjuntFigura->conjuntVisible(true);
  missatgeStatusBar(list1.value(1));
 }
 else missatgeStatusBar("");
}

/*
 Gestió de concurs
*/
void formulariPrincipal::creaConcursSlot(){
CreaConcurs concurs(configuracioJoc,this);
concurs.exec();
}

void formulariPrincipal::carregaConcursSlot(){

QString directoriSeleccionat = QFileDialog::getOpenFileName(this,
                                 configuracioJoc->tradueixCadena("Carrega.concurs"),
                                 directoriFiguresPersonals(),
                                 filtreArxiusConcurs(configuracioJoc) );
    if(!directoriSeleccionat.isEmpty()){
        carregaArbreFigures(directoriSeleccionat);
        canviaEstatDelJoc(Inactiu);        
        configuracioJoc->desaRecords=false;
        reiniciaContadors();
    }
}


void formulariPrincipal::tradueixGUI(){
   setWindowTitle("Peces (" + configuracioJoc->tradueixCadena("Jocs.tangram")+")");

   surtAction->setText(configuracioJoc->tradueixCadena("&Surt")) ;

   elimnaRecordAction->setText(configuracioJoc->tradueixCadena("Elimina.record"));
   eliminaTotsRecordsAction->setText(configuracioJoc->tradueixCadena("Elimina.tots.records"));

    carregaFiguresProgramaAction->setText(configuracioJoc->tradueixCadena("carrega.figures.paquet"));
    carregaFiguresArxiuAction->setText(configuracioJoc->tradueixCadena("carrega.dades.figures.arxiu"));

   mostraSolucioAction->setText(configuracioJoc->tradueixCadena("Mostra.solucio"));
   colocaPecaAction->setText(configuracioJoc->tradueixCadena("Coloca.peca"));
   // 13/02/11
   tradueixNomsJocsTangram();

    nivellInfantilAction->setText(configuracioJoc->tradueixCadena(NOM_NIVELL_INFANTIL));
    nivellPrincipiantAction->setText(configuracioJoc->tradueixCadena(NOM_NIVELL_PRINCIPIANT));
    nivellIniciatAction->setText(configuracioJoc->tradueixCadena(NOM_NIVELL_INICIAT));
    nivellExpertAction->setText(configuracioJoc->tradueixCadena(NOM_NIVELL_EXPERT));

    creaFiguraMenu->setTitle(configuracioJoc->tradueixCadena("crea.nova.figura"));
    comprovaMostraAction->setText(configuracioJoc->tradueixCadena("comprova.mostra"));
    esborraMostraAction->setText(configuracioJoc->tradueixCadena("esborra.mostra"));
    desaFiguraAction->setText(configuracioJoc->tradueixCadena("desa.dades.figura"));
    simetriaVerticalAction->setText(configuracioJoc->tradueixCadena("Simetria.vertical"));
    simetriaHoritzontalAction->setText(configuracioJoc->tradueixCadena("Simetria.horitzontal"));
    cercaFiguraAction->setText(configuracioJoc->tradueixCadena("Cerca.figura"));    
    figuraSeguentAction->setText(configuracioJoc->tradueixCadena("seguent.figura"));
    figuraAnteriorAction->setText(configuracioJoc->tradueixCadena("anterior.figura"));
    pausaAction->setText(configuracioJoc->tradueixCadena("Pausa"));

    aplicacioMenu->setTitle(configuracioJoc->tradueixCadena("Programa"));

    jocMenu->setTitle(configuracioJoc->tradueixCadena("Joc"));
    jocsTangramMenu->setTitle(configuracioJoc->tradueixCadena("Jocs.tangram"));

    proposaFigura->setText(configuracioJoc->tradueixCadena("Proposa.figura"));


    cercaNomFigura->setText(configuracioJoc->tradueixCadena("cerca.nom.figura"));
    continuaCercaNomFigura->setText(configuracioJoc->tradueixCadena("continua.cerca.nom.figura"));

    //Concurs
    concursMenu->setTitle(configuracioJoc->tradueixCadena("Concurs"));
    creaConcursAction->setText(configuracioJoc->tradueixCadena("Crea.concurs"));
    carregaConcursAction->setText(configuracioJoc->tradueixCadena("Carrega.concurs"));

    idiomaMenu->setTitle(configuracioJoc->tradueixCadena("idioma"));
    /*
  Canviat el sistema de gestió del idioma
  v. 4.0.2
    */

    /*
      Generació de figures
      */
    generaFigures->setTitle(configuracioJoc->tradueixCadena("genera.figures"));
    generaFiguraAtzarAction->setText(configuracioJoc->tradueixCadena("genera.figura"));
    generaFiguresMassiuAtzarAction->setText(configuracioJoc->tradueixCadena("genera.figures"));
    generaFiguresAtzarAutomaticAction->setText(configuracioJoc->tradueixCadena("genera.figures.atzar"));

    comprovaFiguresAtzarAction->setText(configuracioJoc->tradueixCadena("comprova.figures"));

ajudaMenu->setTitle(configuracioJoc->tradueixCadena("Ajuda"));
    ajuda->setText(configuracioJoc->tradueixCadena("Ajuda"));
    webPrograma->setText(configuracioJoc->tradueixCadena("Web.programa"));
  //  comprovaVersio->setText(configuracioJoc->tradueixCadena("comprova.versio"));
    creditsPrograma->setText(configuracioJoc->tradueixCadena("Credits"));
    colaboraPrograma->setText(configuracioJoc->tradueixCadena("Colabora"));
    agraiment->setText(configuracioJoc->tradueixCadena("agraiments"));


    simetriaPecaAction->setText(configuracioJoc->tradueixCadena("simetria.peca"));

    actualitzaPuntuacio(0,0);
    carregaArbreFigures(configuracioJoc->directoriArxiuDadesFigures);
}


void formulariPrincipal::creaAcccions()
{
    surtAction = new QAction(configuracioJoc->tradueixCadena("&Surt"), this);
    surtAction->setShortcut(tr("Ctrl+S"));
    surtAction->setIcon(QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"surt.png"));
    connect(surtAction, SIGNAL(triggered()), this, SLOT(close()));

    elimnaRecordAction= new QAction(configuracioJoc->tradueixCadena("Elimina.record"), this);
    connect(elimnaRecordAction, SIGNAL(triggered()), this, SLOT(eliminaRecordSlot()));

    eliminaTotsRecordsAction= new QAction(configuracioJoc->tradueixCadena("Elimina.tots.records"), this);
    connect(eliminaTotsRecordsAction, SIGNAL(triggered()), this, SLOT(eliminaTotsRecordsSlot()));

    carregaFiguresProgramaAction= new QAction(configuracioJoc->tradueixCadena("carrega.figures.paquet"), this);
    connect(carregaFiguresProgramaAction, SIGNAL(triggered()), this, SLOT(carregaFiguresProgramaSlot()));

    carregaFiguresArxiuAction= new QAction(configuracioJoc->tradueixCadena("carrega.dades.figures.arxiu"), this);
    connect(carregaFiguresArxiuAction, SIGNAL(triggered()), this, SLOT(carregaFiguresArxiuSlot()));

    mostraSolucioAction= new QAction(configuracioJoc->tradueixCadena("Mostra.solucio"), this);
    mostraSolucioAction->setIcon(QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"solucio.png"));
    connect(mostraSolucioAction, SIGNAL(triggered()), this, SLOT(mostraSolucioSlot()));


    colocaPecaAction= new QAction(configuracioJoc->tradueixCadena("Coloca.peca"), this);
    colocaPecaAction->setCheckable(true);
    connect(colocaPecaAction, SIGNAL(triggered()), this, SLOT(colocaPecaSlot()));

    proposaFigura= new QAction(configuracioJoc->tradueixCadena("Proposa.figura"), this);
    proposaFigura->setShortcut(tr("Ctrl+P"));
    connect(proposaFigura, SIGNAL(triggered()), this, SLOT(proposaFiguraSlot()));


    cercaNomFigura= new QAction(configuracioJoc->tradueixCadena("cerca.nom.figura"), this);
    cercaNomFigura->setShortcut(tr("Ctrl+N"));
    connect(cercaNomFigura, SIGNAL(triggered()), this, SLOT(cercaNomFiguraSlot()));

    continuaCercaNomFigura= new QAction(configuracioJoc->tradueixCadena("continua.cerca.nom.figura"), this);
    continuaCercaNomFigura->setShortcut(tr("F3"));
    connect(continuaCercaNomFigura, SIGNAL(triggered()), this, SLOT(continuaCercaNomFiguraSlot()));
    continuaCercaNomFigura->setEnabled(false);

    pausaAction= new QAction(configuracioJoc->tradueixCadena("Pausa"), this);
    pausaAction->setShortcut(tr("Ctrl+A"));
    pausaAction->setIcon(QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"atura.png"));
    connect(pausaAction, SIGNAL(triggered()), this, SLOT(pausaSlot()));

    /*
      Accions que crean els tangrams
      13/02/11
      Ara es crean en temps d'execució
    */

    /*
      Nivell del Joc
    */
    nivellJocGroup = new QActionGroup(this);

    nivellInfantilAction = new QAction(configuracioJoc->tradueixCadena(NOM_NIVELL_INFANTIL), this);
    nivellInfantilAction->setCheckable(true);
    nivellInfantilAction->setActionGroup(nivellJocGroup);
    nivellInfantilAction->setIcon(QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"nivellInfantil.png"));
    nivellInfantilAction->setShortcut(tr("Alt+1"));
    connect(nivellInfantilAction, SIGNAL(triggered()), this, SLOT(nivellInfantilSlot()));

    nivellPrincipiantAction = new QAction(configuracioJoc->tradueixCadena(NOM_NIVELL_PRINCIPIANT), this);
    nivellPrincipiantAction->setCheckable(true);
    nivellPrincipiantAction->setActionGroup(nivellJocGroup);
    nivellPrincipiantAction->setIcon(QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"nivellPrincipiant.png"));
    nivellPrincipiantAction->setShortcut(tr("Alt+2"));
    connect(nivellPrincipiantAction, SIGNAL(triggered()), this, SLOT(nivellPrincipiantSlot()));

    nivellIniciatAction = new QAction(configuracioJoc->tradueixCadena(NOM_NIVELL_INICIAT), this);
    nivellIniciatAction->setCheckable(true);
    nivellIniciatAction->setActionGroup(nivellJocGroup);
    nivellIniciatAction->setIcon(QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"nivellIniciat.png"));
    nivellIniciatAction->setShortcut(tr("Alt+3"));
    connect(nivellIniciatAction, SIGNAL(triggered()), this, SLOT(nivellIniciatSlot()));

    nivellExpertAction = new QAction(configuracioJoc->tradueixCadena(NOM_NIVELL_EXPERT), this);
    nivellExpertAction->setCheckable(true);
    nivellExpertAction->setActionGroup(nivellJocGroup);
    nivellExpertAction->setIcon(QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"nivellExpert.png"));
    nivellExpertAction->setShortcut(tr("Alt+4"));
    connect(nivellExpertAction, SIGNAL(triggered()), this, SLOT(nivellExpertSlot()));

    nivellInfantilAction->setChecked(true);

    comprovaMostraAction = new QAction(configuracioJoc->tradueixCadena("comprova.mostra"), this);
    comprovaMostraAction->setShortcut(Qt::CTRL + Qt::Key_M);
    connect(comprovaMostraAction, SIGNAL(triggered()), this, SLOT(comprovaMostraSlot()));

    esborraMostraAction = new QAction(configuracioJoc->tradueixCadena("esborra.mostra"), this);
    esborraMostraAction->setShortcut(Qt::CTRL + Qt::Key_E);
    connect(esborraMostraAction, SIGNAL(triggered()), this, SLOT(esborraMostraSlot()));

    desaFiguraAction = new QAction(configuracioJoc->tradueixCadena("desa.dades.figura"), this);
    desaFiguraAction->setShortcut(Qt::CTRL + Qt::Key_D);
    connect(desaFiguraAction, SIGNAL(triggered()), this, SLOT(desaFiguraSlot()));

    simetriaVerticalAction = new QAction(configuracioJoc->tradueixCadena("Simetria.vertical"), this);
    simetriaVerticalAction->setShortcut(Qt::CTRL + Qt::Key_V);
    connect(simetriaVerticalAction, SIGNAL(triggered()), this, SLOT(simetriaVerticalSlot()));

    simetriaHoritzontalAction = new QAction(configuracioJoc->tradueixCadena("Simetria.horitzontal"), this);
    simetriaHoritzontalAction->setShortcut(Qt::CTRL + Qt::Key_H);
    connect(simetriaHoritzontalAction, SIGNAL(triggered()), this, SLOT(simetriaHoritzontalSlot()));

    cercaFiguraAction = new QAction(configuracioJoc->tradueixCadena("Cerca.figura"), this);
    cercaFiguraAction->setShortcut(Qt::CTRL + Qt::Key_F);
    connect(cercaFiguraAction, SIGNAL(triggered()), this, SLOT(cercaFiguraSlot()));


    figuraSeguentAction = new QAction(configuracioJoc->tradueixCadena("seguent.figura"), this);
    figuraSeguentAction->setShortcut(QKeySequence::MoveToNextPage);
    figuraSeguentAction->setEnabled(false);
    connect(figuraSeguentAction, SIGNAL(triggered()), this, SLOT(figuraSeguentSlot()));

    figuraAnteriorAction = new QAction(configuracioJoc->tradueixCadena("anterior.figura"), this);
    figuraAnteriorAction->setShortcut(QKeySequence::MoveToPreviousPage);
    figuraAnteriorAction->setEnabled(false);
    connect(figuraAnteriorAction, SIGNAL(triggered()), this, SLOT(figuraAnteriorSlot()));

    /* Creació de concurs */
    creaConcursAction = new QAction(configuracioJoc->tradueixCadena("Crea.concurs"), this);
    connect(creaConcursAction, SIGNAL(triggered()), this, SLOT(creaConcursSlot()));

    carregaConcursAction = new QAction(configuracioJoc->tradueixCadena("Carrega.concurs"), this);
    connect(carregaConcursAction, SIGNAL(triggered()), this, SLOT(carregaConcursSlot()));

//Idiomes
    /*
  Canviat el sistema de gestió del idioma
  v. 4.0.2
  Les accions es crean en temps d'execució
     */

    /*
      Generació a l'atzar de noves figures
      */
    generaFiguraAtzarAction= new QAction(configuracioJoc->tradueixCadena("genera.figura"), this);
    generaFiguraAtzarAction->setShortcut(Qt::CTRL + Qt::Key_R);
    connect(generaFiguraAtzarAction, SIGNAL(triggered()), this, SLOT(generaFiguraAtzarSlot()));

    generaFiguresMassiuAtzarAction= new QAction(configuracioJoc->tradueixCadena("genera.figures"), this);
    generaFiguresMassiuAtzarAction->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(generaFiguresMassiuAtzarAction, SIGNAL(triggered()), this, SLOT(generaFiguresMassiuAtzarSlot()));

    generaFiguresAtzarAutomaticAction= new QAction(configuracioJoc->tradueixCadena("genera.figures.atzar"), this);
   // generaFiguresAtzarAutomaticAction->setShortcut(Qt::CTRL + Qt::Key_Q);
    connect(generaFiguresAtzarAutomaticAction, SIGNAL(triggered()), this, SLOT(generaFiguresAtzarSlot()));

    /* comprovació de les noves figures generades a l'atzar */
    comprovaFiguresAtzarAction = new QAction(configuracioJoc->tradueixCadena("comprova.figures"), this);
    connect(comprovaFiguresAtzarAction, SIGNAL(triggered()), this, SLOT(comprovaNovesFiguresAtzarSlot()));


    ajuda= new QAction(configuracioJoc->tradueixCadena("Ajuda"), this);
    connect(ajuda, SIGNAL(triggered()), this, SLOT(ajudaSlot()));

    webPrograma= new QAction(configuracioJoc->tradueixCadena("Web.programa"), this);
    connect(webPrograma, SIGNAL(triggered()), this, SLOT(webProgramaSlot()));

  /*  comprovaVersio=new QAction(configuracioJoc->tradueixCadena("comprova.versio"), this);
    connect(comprovaVersio, SIGNAL(triggered()), this, SLOT(comprovaVersioSlot()));*/

    creditsPrograma= new QAction(configuracioJoc->tradueixCadena("Credits"), this);
    connect(creditsPrograma, SIGNAL(triggered()), this, SLOT(creditsProgramaSlot()));

    colaboraPrograma= new QAction(configuracioJoc->tradueixCadena("Colabora"), this);
    connect(colaboraPrograma, SIGNAL(triggered()), this, SLOT(colaboraProgramaSlot()));


    agraiment= new QAction(configuracioJoc->tradueixCadena("Agraiments"), this);
    connect(agraiment, SIGNAL(triggered()), this, SLOT(agraimentSlot()));

    simetriaPecaAction= new QAction(configuracioJoc->tradueixCadena("simetria.peca"), this);
    simetriaPecaAction->setIcon(QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"simetria.png"));
    connect(simetriaPecaAction, SIGNAL(triggered()), this, SLOT(simetriaPecaSlot()));
}

//retorna una llista amb les dades dels
//tangram ordenats pel número de peces
QStringList formulariPrincipal::dadesTangram(){
    /*Tangrams ordenats pel número de peces
    Codi del tangram, codi del nom
    nom del icone
    NOU TANGRAM 1
    darrer codi 39*/
    QStringList dadesArxiu;
    dadesArxiu.append("14;Fig_TB;peces_b.png");
    dadesArxiu.append("19;Fig_T4;peces4.png");
    dadesArxiu.append("30;Fig_T4II;peces4II.png");
    dadesArxiu.append("2;Fig_T5;peces5.png");
    dadesArxiu.append("36;Fig_T5II;peces5II.png");
    dadesArxiu.append("27;Fig_TSL;pecesSL.png");
    dadesArxiu.append("25;Fig_REG;pecesREG.png");
    dadesArxiu.append("3;Fig_T6;peces6.png");
    dadesArxiu.append("23;Fig_T6II;peces6II.png");
    dadesArxiu.append("15;Fig_TH;peces_h.png");
    dadesArxiu.append("1;Fig_T7;pecesset.png");
    dadesArxiu.append("4;Fig_TF;peces_f.png");
    dadesArxiu.append("21;Fig_T7I;peces7I.png");
    dadesArxiu.append("24;Fig_T7II;peces7II.png");
    dadesArxiu.append("26;Fig_THEP;pecesHEP.png");
    dadesArxiu.append("34;Fig_T8E;peces8E.png");
    dadesArxiu.append("5;Fig_TX;peces_x.png");
    dadesArxiu.append("6;Fig_TS;peces_s.png");
    dadesArxiu.append("7;Fig_T8;peces8.png");
    dadesArxiu.append("22;Fig_TCII;pecesCII.png");
    dadesArxiu.append("37;Fig_TA;pecesA.png");
    dadesArxiu.append("38;Fig_TREC;pecesrec.png");//Tangram rectangular
    dadesArxiu.append("8;Fig_T9;peces9.png");
    dadesArxiu.append("28;Fig_T9T;peces9t.png");
    dadesArxiu.append("16;Fig_TOU;peces_ou.png");
    dadesArxiu.append("17;Fig_COR;peces_c.png");
    dadesArxiu.append("40;Fig_HEX;pecesHex.png");
    dadesArxiu.append("9;Fig_T10;peces10.png");
    dadesArxiu.append("29;Fig_TE;pecese.png");
    dadesArxiu.append("33;Fig_EL;pecesel.png");
    dadesArxiu.append("20;Fig_TCIRI;pecesCI.png");
    dadesArxiu.append("10;Fig_T11;peces11.png");
    dadesArxiu.append("32;Fig_STIII;pecesSTOIII.png");
    dadesArxiu.append("11;Fig_TR;peces_r.png");
    dadesArxiu.append("35;Fig_T12;peces12.png"); //Tangram de 12 peces
    dadesArxiu.append("12;Fig_T13;peces13.png");
    dadesArxiu.append("13;Fig_T14;peces14.png");
    dadesArxiu.append("18;Fig_ST;peces_sto.png");
    dadesArxiu.append("31;Fig_STII;pecesSTOII.png");
    dadesArxiu.append("39;Fig_T16;peces16.png");
    return dadesArxiu;
}

/* 13/02/11
  Crea les accions, menus i botons
  dels tipus de tangram
  */
void formulariPrincipal::creaTipusTangramMenu(){
    jocsTangramActionGroup = new QActionGroup(this);
    connect(jocsTangramActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(creaTangramAccio(QAction *)));

   //QString nomArxiu=directoriLinux()+QDir().separator()+"dades"+QDir().separator()+"dades_menus_tan.dat";
   //QStringList dadesArxiu=llegeixArxiuALlista(nomArxiu);

//Carregam les dades dels tangram
QStringList dadesArxiu=dadesTangram();

   nomsDelsJocsPerTraduccio.clear();
   if (dadesArxiu.count()>0){
       //Començam a llegir les dades de la llista de menus que cal crear
      for (int i = 0; i < dadesArxiu.count(); ++i) {
         QStringList dadesMenu= dadesArxiu.value(i).split(";");
         //Ara cream l'acció
         QAction *action = new QAction(configuracioJoc->tradueixCadena(dadesMenu.value(1)), this);
        //La llista amb els codis dels noms dels jocs de tangram
        //per fer servir en la traduccio del GUI
        nomsDelsJocsPerTraduccio.append(dadesMenu.value(1));
        action->setData(dadesMenu.value(0));
        action->setIcon(QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+dadesMenu.value(2)));
        jocsTangramMenu->addAction(action);
        jocsTangramActionGroup->addAction(action);
      }
   }
}

/* 13/02/11
   Crea el tipus de tangram segons les dades
   de l'acció
  */
void formulariPrincipal::creaTangramAccio(QAction *accio){
   creaTangram(accio->data().toInt());
}

/* 13/02/11
   Tradueix els noms dels jocs de tangram en les
   actions
   */
void formulariPrincipal::tradueixNomsJocsTangram(){
   for(int i=0;i<jocsTangramActionGroup->actions().count();i++){
     jocsTangramActionGroup->actions().value(i)->setText(
             configuracioJoc->tradueixCadena(nomsDelsJocsPerTraduccio.value(i)));
   }
}

/*
  13/02/11
  Carrega el contingut de l'arxiu passat per paràmetre
  a una llista (que retorna)
  */
QStringList formulariPrincipal::llegeixArxiuALlista(QString arxiu){
    //obrim l'arxiu de jocs
    QFile arxiuData(arxiu);
    QStringList  dadesArxiu;
    if (arxiuData.open(QIODevice::ReadOnly | QIODevice::Text)){
     QTextStream entrada(&arxiuData);
     QString linia0 = entrada.readLine();
     while (!linia0.isNull()) {
         if ( ! (linia0.mid(0,1)=="#") ){
            dadesArxiu.append(linia0);
         }
       linia0 = entrada.readLine();
     } //final del while de la lectura de l'arxiu
 }
 return dadesArxiu;
}


//Crea el menú de selecció d'idioma de l'aplicació
void formulariPrincipal::creaIdiomaMenu(){
    idiomaActionGroup = new QActionGroup(this);
    connect(idiomaActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(seleccioIdioma(QAction * )));

    QDir dir(directoriLinux()+QDir().separator()+"locales");
    QStringList arxiusIdioma = dir.entryList(QStringList("locale_*.lo"));

for (int i = 0; i < arxiusIdioma.size(); ++i) {
    QString locale = arxiusIdioma[i];
    locale.remove(0, locale.indexOf('_') + 1);
    locale.truncate(locale.lastIndexOf('.'));


     QString nomArxiu=directoriLinux()+QDir().separator()+"locales"+QDir().separator()+arxiusIdioma[i];
     QSettings *arxiuLocale=new QSettings(nomArxiu,QSettings::IniFormat);
     QString language=arxiuLocale->value("Language","Language").toString().toLatin1();

    QAction *action = new QAction(tr("&%1 %2").arg(i + 1).arg(language), this);
    action->setCheckable(true);
    action->setData(locale);
    idiomaMenu->addAction(action);
    idiomaActionGroup->addAction(action);
   if (configuracioJoc->strLocale().contains((locale) )){
        action->setChecked(true);
        //a debian, sense això es carregan les cadenes
        //originals enlloc de les traduïdes
         configuracioJoc->canviaArxiuLocale(locale);
        //canviaIdioma(action);
       }
  }
}

void formulariPrincipal::seleccioIdioma(QAction *action){
    configuracioJoc->canviaArxiuLocale(action->data().toString());
    seleccioIdiomaQt(action->data().toString());
    tradueixGUI();

}

void formulariPrincipal::seleccioIdiomaQt(QString strLocale){
    QTranslator *translator = new QTranslator(qApp);
    QString translatorArxiu = QLatin1String("qt_");
    translatorArxiu.append(strLocale);
   /* qCritical("%s", qPrintable(QLibraryInfo::location(QLibraryInfo::TranslationsPath)));
    qCritical("%s", qPrintable(translatorArxiu));*/
   if (translator->load(translatorArxiu, QLibraryInfo::location(QLibraryInfo::TranslationsPath))){
      // qDebug("instal·lat");
    qApp->installTranslator(translator);
    translatorFileName=translatorArxiu ;
   }
   else {
     /* Això és per a què es torni a fer servir l'anglès,
      * però no funciona */
     QTranslator *translator1 = new QTranslator(qApp);
     if(translator1->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath))){
     qApp->removeTranslator(translator1);
     translatorFileName.clear();
     qApp->processEvents();
      }
     }
     //19/09/15 A windows cal posar
     /*if (translator->load(translatorFileName, QApplication::applicationDirPath()))
      app.installTranslator(translator);*/
}

/*
  Canviat el sistema de gestió del idioma
  v. 4.0.2
*/

//Ajuda
void formulariPrincipal::ajudaSlot(){
 QDir dirArxiu;
 QString directori=directoriLinux()+
                            charPathSeparator+
                            DIR_HELP+
                            charPathSeparator+
                            configuracioJoc->strLocale()+
                            charPathSeparator+
                            "index.html";

    if (dirArxiu.exists(directori)){
     //Això evita problemas amb els espais als noms dels directoris
        QDesktopServices::openUrl(QUrl("file:///"+directori, QUrl::TolerantMode));
    }
   else  QDesktopServices::openUrl(QUrl("file:///"+directoriLinux()+
                            charPathSeparator+
                            DIR_HELP+
                            charPathSeparator+
                            "en"+
                            charPathSeparator+
                            "index.html", QUrl::TolerantMode));
}

void formulariPrincipal::webProgramaSlot(){
   QDesktopServices::openUrl(ADRECA_WEB_PROGRAMA);
}

void formulariPrincipal::creditsProgramaSlot(){
    //QDate avui=QDate::currentDate () ;

    QString missatge=QString("<h2>"+configuracioJoc->tradueixCadena("credits.versio")+""
                             +QString(" v. %1 </h2><br/>").
                                                          arg(qApp->applicationVersion()));
//27/12/11  Nova funció
    missatge.append(HTML_llicencia());
  /*  missatge.append(QString("&copy;  "+QString("2000-%1").arg(avui.year())+"  I. De Marchi <br/>" ));
    missatge.append(QString(configuracioJoc->tradueixCadena("credits.llicencia")+"<br/>"));

    missatge.append(QString("<a href=\"%1\">%1</a><br/>").arg(ADRECA_WEB_PROGRAMA));
    missatge.append(QString("<a href=mailto:%1>%1</a><br/>").arg(ADRECA_CORREU));*/
//missatge.append(QString("<p>"));
    /*
      28/10/11
      Això ha estat substituït per una opció de menú que mostra
      un arxiu html
    missatge.append(QString("<p>Thanks for translations: <br/>"));
    missatge.append(QString("Français: Philippe Moutou  (v. 4.1.1) <br/>"));
    missatge.append(QString("Portuguese: Américo Monteiro  (v. 4.0.1) <br/>"));
    missatge.append(QString("Italiano :      Vincenzo Campanella  (v. 4.0.2) <br/>"));
    missatge.append(QString("Deutsch :     Erik Pfannenstein  (v. 4.0.2) <br/>"));
    missatge.append(QString("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Hendrik Knackstedt  (v. 4.0.2) <br/>"));
    missatge.append(QString("Portuguese do Brasil (Brasileiro) :     Adriano Rafael Gomes  (v. 4.0.2) <br/>"));
*/
    QMessageBox::about(this,
            configuracioJoc->tradueixCadena("credits.rotul"),missatge);
}


void formulariPrincipal::colaboraProgramaSlot(){
   QDir dirArxiu;
 QString directori=directoriLinux()+
                            charPathSeparator+
                            DIR_HELP+
                            charPathSeparator+
                            configuracioJoc->strLocale()+
                            charPathSeparator+
                            "html/colabora.html";

    if (dirArxiu.exists(directori)){
      QDesktopServices::openUrl(QUrl("file:///"+directori, QUrl::TolerantMode));
    }
   else  QDesktopServices::openUrl(QUrl("file:///"+directoriLinux()+
                            charPathSeparator+
                            DIR_HELP+
                            charPathSeparator+
                            "en"+
                            charPathSeparator+
                            "html/colabora.html", QUrl::TolerantMode));
}

/* 28/10/11
   Mostra l'arxiu html amb els agraïments
  */
void formulariPrincipal::agraimentSlot(){
    QDesktopServices::openUrl(QUrl("file:///"+directoriLinux()+
                                charPathSeparator+
                                DIR_HELP+
                                charPathSeparator+
                                "index.html", QUrl::TolerantMode));
}


/*
  Selecciona a l'atzar una de les figures de l'arbre
*/
void formulariPrincipal::proposaFiguraSlot(){
   QTreeWidgetItem *item= arbreFiguresTreeWidget->topLevelItem(0);
   if(item->childCount()>0){
   int nus=qrand() % item->childCount();
   QTreeWidgetItem *itemSel=item->child(nus);
   arbreFiguresTreeWidget->setCurrentItem(itemSel);
}
}


void formulariPrincipal::cercaNomFiguraSlot(){
 if (arbreFiguresTreeWidget->topLevelItem(0)==0){ return;}

nomFiguraPerCercar.clear();
indexFiguraPerCercar=0;
bool busca;
bool trobat=false;
nomFiguraPerCercar = QInputDialog::getText(this,
                            configuracioJoc->tradueixCadena("cerca.nom.figura"),
                            configuracioJoc->tradueixCadena("label.nom.figura"),
                            QLineEdit::Normal,
                            "", &busca);

 if (busca && !nomFiguraPerCercar.isEmpty()){
    for(int i=0;i<=arbreFiguresTreeWidget->topLevelItem(0)->childCount()-1;i++){
        indexFiguraPerCercar=i;
        if (arbreFiguresTreeWidget->topLevelItem(0)->child(i)->text(0).contains(
                nomFiguraPerCercar,Qt::CaseInsensitive)){
           continuaCercaNomFigura->setEnabled(true);
           arbreFiguresTreeWidget->setCurrentItem(arbreFiguresTreeWidget->topLevelItem(0)->child(i));
           indexFiguraPerCercar=i+1;
           trobat=true;
           break;
        }
     }

 //if(indexFiguraPerCercar>=arbreFiguresTreeWidget->topLevelItem(0)->childCount()){
    if (!trobat){
     QMessageBox::information(this, configuracioJoc->tradueixCadena("cerca.nom.figura"),
                    configuracioJoc->tradueixCadena("msg.nomFigura.noTrobada"));
  }
 }
}

void formulariPrincipal::continuaCercaNomFiguraSlot(){
    bool trobat=false;
for(int i=indexFiguraPerCercar;i<=arbreFiguresTreeWidget->topLevelItem(0)->childCount()-1;i++){
    indexFiguraPerCercar=i;
    if (arbreFiguresTreeWidget->topLevelItem(0)->child(i)->text(0).contains(
           nomFiguraPerCercar,Qt::CaseInsensitive)){
       arbreFiguresTreeWidget->setCurrentItem(arbreFiguresTreeWidget->topLevelItem(0)->child(i));
       indexFiguraPerCercar=i+1;
       trobat=true;
       break;
    }
 }
//if(indexFiguraPerCercar+1>=arbreFiguresTreeWidget->topLevelItem(0)->childCount()){
if (!trobat){
    nomFiguraPerCercar.clear();
    indexFiguraPerCercar=0;
    continuaCercaNomFigura->setEnabled(false);
    QMessageBox::information(this, configuracioJoc->tradueixCadena("cerca.nom.figura"),
                    configuracioJoc->tradueixCadena("msg.nomFigura.noTrobada"));
  }
}

void formulariPrincipal::creaMenus()
{
    aplicacioMenu = menuBar()->addMenu(configuracioJoc->tradueixCadena("Programa"));
    aplicacioMenu->addAction(elimnaRecordAction);
    aplicacioMenu->addAction(eliminaTotsRecordsAction);
    aplicacioMenu->addSeparator();

    aplicacioMenu->addAction(carregaFiguresProgramaAction);
    aplicacioMenu->addAction(carregaFiguresArxiuAction);

    aplicacioMenu->addSeparator();
    aplicacioMenu->addAction(surtAction);


    jocMenu=menuBar()->addMenu(configuracioJoc->tradueixCadena("Joc"));
    jocsTangramMenu=jocMenu->addMenu(configuracioJoc->tradueixCadena("Jocs.tangram"));
    // 13/02/11
    creaTipusTangramMenu();
    jocMenu->addSeparator();
    //menú del nivell de Joc
    jocMenu->addAction(nivellInfantilAction);
    jocMenu->addAction(nivellPrincipiantAction);
    jocMenu->addAction(nivellIniciatAction);
    jocMenu->addAction(nivellExpertAction);
     /*
      Generació de noves figures
    */
    jocMenu->addSeparator();
    creaFiguraMenu=jocMenu->addMenu(configuracioJoc->tradueixCadena("crea.nova.figura"));    
        creaFiguraMenu->addAction(comprovaMostraAction);
        creaFiguraMenu->addAction(esborraMostraAction);
        creaFiguraMenu->addAction(desaFiguraAction);
        creaFiguraMenu->addSeparator();
        creaFiguraMenu->addAction(simetriaVerticalAction);
        creaFiguraMenu->addAction(simetriaHoritzontalAction);
        creaFiguraMenu->addSeparator();
        creaFiguraMenu->addAction(cercaFiguraAction);
        creaFiguraMenu->addAction(figuraSeguentAction);
        creaFiguraMenu->addAction(figuraAnteriorAction);



    jocMenu->addSeparator();
    jocMenu->addAction(proposaFigura);

    jocMenu->addSeparator();

    jocMenu->addAction(cercaNomFigura);
    jocMenu->addAction(continuaCercaNomFigura);

    //ajuda a la resolució de figures
    jocMenu->addSeparator();
    jocMenu->addAction(colocaPecaAction);
    jocMenu->addAction(mostraSolucioAction);

    jocMenu->addSeparator();
    jocMenu->addAction(pausaAction);

    //creació de concurs
    concursMenu=menuBar()->addMenu(configuracioJoc->tradueixCadena("Concurs"));
    concursMenu->addAction(creaConcursAction);
    concursMenu->addAction(carregaConcursAction);

    //selecció d'idioma
    idiomaMenu=menuBar()->addMenu(configuracioJoc->tradueixCadena("idioma"));
    creaIdiomaMenu();


    /*
      Generació de figures
      */
    generaFigures=menuBar()->addMenu(configuracioJoc->tradueixCadena("genera.figures"));
       generaFigures->addAction(generaFiguraAtzarAction);
       generaFigures->addAction(generaFiguresMassiuAtzarAction);
       generaFigures->addAction(generaFiguresAtzarAutomaticAction);
       generaFigures->addAction(comprovaFiguresAtzarAction);

    //Ajuda
    ajudaMenu=menuBar()->addMenu(configuracioJoc->tradueixCadena("Ajuda"));
        ajudaMenu->addAction(ajuda);
        ajudaMenu->addAction(webPrograma);
      //  ajudaMenu->addAction(comprovaVersio);
        ajudaMenu->addAction(colaboraPrograma);
        ajudaMenu->addSeparator();
        ajudaMenu->addAction(creditsPrograma);
        ajudaMenu->addAction(agraiment);

}


  void formulariPrincipal::missatgeStatusBar(QString missatge)
 {
     statusBar()->showMessage(missatge);
 }

void formulariPrincipal::creaBarraTasques()
{
    tangramsToolBar = addToolBar(configuracioJoc->tradueixCadena("Tangrams"));
    tangramsToolBar->setIconSize(QSize(15,15));


    // 13/02/11 Afegim les accions dels tipus de tangram
    for(int i=0;i<jocsTangramActionGroup->actions().count();i++){
        tangramsToolBar->addAction( jocsTangramActionGroup->actions().value(i));
   }

    tangramsToolBar->addSeparator();

    //nivell de Joc
    tangramsToolBar->addAction(nivellInfantilAction);
    tangramsToolBar->addAction(nivellPrincipiantAction);
    tangramsToolBar->addAction(nivellIniciatAction);
    tangramsToolBar->addAction(nivellExpertAction);
    tangramsToolBar->setMovable(false);
    tangramsToolBar->addSeparator();
    tangramsToolBar->addAction(simetriaPecaAction);

}
void formulariPrincipal::resizeEvent ( QResizeEvent * event ){
QSize augment=this->size()-this->minimumSize();
sceneFrame->resize(QSize(sceneFrame->minimumSize()+augment));
sceneJoc->setSceneRect(0,0,viewJoc->minimumSize().width()+augment.width(),
                       viewJoc->minimumSize().height()+augment.height());
viewJoc->resize(QSize(viewJoc->minimumSize()+augment));

arbreFiguresTreeWidget->resize(
     arbreFiguresTreeWidget->minimumSize().width(),
     arbreFiguresTreeWidget->minimumSize().height()+augment.height());

//recolocam les peces per evitar que quedin
//fora de la vista
conjuntFigura->centraFiguraScene();
conjuntJoc->centraFiguraScene();
for(int i=0;i<=conjuntJoc->arrayPeces.size()-1;i++){
   while  (!conjuntJoc->arrayPeces[i]->pecaEnLimits()){
     conjuntJoc->arrayPeces[i]->setPos(qrand() % viewJoc->width()
                                   ,qrand() % viewJoc->height());
  }
  }
QMainWindow::resizeEvent(event);
}

/*
  Manté el joc en pausa
  */
void formulariPrincipal::pausaSlot(){
    //Si s'està jugant, s'atura el joc
    if(rellotgeLabel->rellotgeEnMarxa()){
        QProgressDialog pausaProgres(tr("Pausa"), tr("Continua"), 0,0 ,this);
        rellotgeLabel->aturaRellotge();
        pausaProgres.setWindowModality(Qt::WindowModal);
        pausaProgres.setVisible(true);
        qApp->processEvents();
        conjuntJoc->conjuntVisible(false);
        conjuntMostra->conjuntVisible(false);
        bool conjuntFiguraVisible=conjuntFigura->arrayPeces[0]->pecaVisible;
        conjuntFigura->conjuntVisible(false);
        bool continua=true;
        while (continua){
            if (pausaProgres.wasCanceled()){
                continua=false;
                conjuntJoc->conjuntVisible(true);
                conjuntMostra->conjuntVisible(true);
                conjuntFigura->conjuntVisible(conjuntFiguraVisible);
                rellotgeLabel->iniciaRellotge();}
            qApp->processEvents();
        }
        pausaProgres.close();
  }
}

/* 26/10/11
   Comprova si hi ha una nova versió del programa.
   Baixa l'arxiu «versio» de mallorcaweb i compara
   els codis de les versions
   11/05/14 Tot això no funciona amb sourceforge.net

void formulariPrincipal::baixaArxiuFinished(){
    output.close();
    currentDownload->deleteLater();

    QString missatge;
    if (currentDownload->error()) {
     missatge=configuracioJoc->tradueixCadena("Error.versio");
     missatge.append(":\n");
     missatge.append(currentDownload->errorString());
    }
    else{
      missatge=llegeixNovaVersio();
    }
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMessageBox::information(this, configuracioJoc->tradueixCadena("comprova.versio"),
                        missatge);

}

void formulariPrincipal::baixaArxiuReadyRead(){
    output.write(currentDownload->readAll());
}*/

/* 26/10/11
  Llegeix l'arxiu «versio» i comprova
  si hi ha una nova versió

QString formulariPrincipal::llegeixNovaVersio(){
    QStringList arxiuVersio;
    bool novaVersio=false;
    arxiuVersio=llegeixArxiuALlista(
            directoriFiguresPersonals()+charPathSeparator+QString("versio"));
    if (arxiuVersio.count()>0){
      QStringList versio=arxiuVersio.value(0).split(".");
      QStringList versioActual=qApp->applicationVersion().split(".");
      for(int i=0;i<versioActual.count();i++){
          if(versio.value(i).toInt()>versioActual.value(i).toInt()){
              novaVersio=true;
          }
          else if (versio.value(i).toInt()<versioActual.value(i).toInt()){
              break;}
      }
    }
    QString missatge;
    if(novaVersio){
      missatge.append(configuracioJoc->tradueixCadena("nova.versio"));
      missatge.append("\n");
      missatge.append(configuracioJoc->tradueixCadena("versio.actual")+
                      " "+qApp->applicationVersion());
      missatge.append("\n");
      missatge.append(configuracioJoc->tradueixCadena("versio.nova")+
                      " "+arxiuVersio.value(0));
      missatge.append("\n");
      missatge.append(configuracioJoc->tradueixCadena("visitau.web"));
      missatge.append("\n");
      missatge.append(QString("%1").arg(ADRECA_WEB_PROGRAMA));
    }
    else missatge.append(configuracioJoc->tradueixCadena("versio.actualitzada"));

    return missatge;
}

void formulariPrincipal::comprovaVersioSlot(){    
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QUrl url;
    url.setUrl("http://pecesjocdetangr.sourceforge.net/versio");
    //url.setUrl("http://mallorcaweb.net/tangrampeces/versio");
    QString nomArxiuDisc;
    nomArxiuDisc=directoriFiguresPersonals()+charPathSeparator+QString("versio");
    output.setFileName(nomArxiuDisc);
    output.open(QIODevice::WriteOnly);

    QNetworkRequest request(url);
    currentDownload= manager.get(request);

    connect(currentDownload, SIGNAL(finished()),
            SLOT(baixaArxiuFinished()));
    connect(currentDownload, SIGNAL(readyRead()),
            SLOT(baixaArxiuReadyRead()));
}*/

/*
  Generació automàtica de figures
  */
void formulariPrincipal::generaFiguraAtzarSlot(){
      conjuntJoc->colocaPecesAtzar();
      generaFiguraAtzar_configuracioInicialProces();
      generaFiguresMassiuAtzarSlot(false);      
      generaFiguraAtzar_configuracioFinalProcess();
}


/*
  Genera automàticament noves figures i les desa a un arxiu
  massiu: determina si es genera més d'una figura
  ambDades: determina si es fan servir les dades de posicions de les peces (1 per defecte)
            o bé la generació és absolutament a l'atzar (-1)
  */
void formulariPrincipal::generaFiguresMassiuAtzarSlot(bool massiu, int ambDades){

/* Per generar arxius HTML amb les figures
 */
//HTML();
//return;

conjuntFigura->conjuntVisible(false);
conjuntFigura->colocaPecesAtzar();
conjuntMostra->conjuntVisible(false);
//El codi numéric de cada figura serà assignat
//a partir d'aquest codi obtingut a l'atzar
int nombreInicialFigures=generaNombreAtzar(rellotgeAtzar.elapsed());
QStringList dadesPosicionsPecesList;


    //primer es demana quantes figures cal generar
    bool val;
    int figuresGenerades=0;
    QString llistaFiguresGenerades;
    int nombreFigures=0;
    //int contadorErrorsFigura=0;
    if(massiu){
      nombreFigures =QInputDialog::getInt(this, configuracioJoc->tradueixCadena("nombre.Figures"),
                                    configuracioJoc->tradueixCadena("Nombre.figures.generar"),
                                    0,0,1000,1, &val,0);}
    else {nombreFigures=1;
         val=true;}
    if (val && nombreFigures>0){        
        qApp->processEvents();
        if (ambDades>0){
           dadesPosicionsPecesList=carregaDadesPosicionsPeces();}
        conjuntJoc->colocaPecesAtzar();        
        qApp->processEvents();
        int nombreMaximFigures;
        /* Així, quan només es genera una figura
         * hi ha una mica de moviment*/
        if(nombreFigures==1){
           nombreMaximFigures=0;}
        else nombreMaximFigures=nombreFigures;

            QProgressDialog progres(
                 configuracioJoc->tradueixCadena("generant.figures")+"...",
                 configuracioJoc->tradueixCadena("Atura.cerca"), 0,
                 nombreMaximFigures, this);

       progres.setWindowModality(Qt::WindowModal);
       progres.setVisible(true);
       qApp->processEvents();
       int j=0;
       //incialment, es dona per fet que s'aconsegueix formar una figura
       bool figuraGenerada=false;
       //comprovam si hi ha dades per a la generació de noves figures
       bool generaFiguresAmbDades=controlaArxiuDadesPosicionsPeces();

       while (j<nombreFigures){           
         //conjuntFigura->colocaPecesAtzar();
         QString nomFigura="Auto.";
         if (generaFiguresAmbDades){
           /* Si el paràmetre ambDades=-1, llavors es generaran
            * figures completament a l'atzar (sense fer servir dades)
            */
           int nombreAtzar=ambDades*generaNombreAtzar(rellotgeAtzar.elapsed());
           if(ambDades>0){
               if( (nombreAtzar % 10)<4 ){
                   figuraGenerada=generaFiguraAtzar(dadesPosicionsPecesList,false);
               }
               else if ( (nombreAtzar % 10)>=4 ){
                   figuraGenerada=generaFiguraAtzarCompacte(dadesPosicionsPecesList,false);}
            }
           /* Hi ha dades per a la generació de figures, però s'ha
            * seleccionat l'opció de generació totalment a l'atzar.
            * Es genera la figura totalment a l'atzar però sense
            * desar les dades
            */
           else  {
               generaFiguraAtzar_inici(false);
               nomFigura="Auto-2.";
               figuraGenerada=true;
              }
           }
          /* No hi ha dades per la generació de figures
           */
           else {
             generaFiguraAtzar_inici(false);
             nomFigura="Auto-2.";
             figuraGenerada=true;
            }

       //  sceneJoc->update();
         qApp->processEvents();
         QString dadesFigura=conjuntFigura->coordenadesFigura();
         //desam la figura
         //Pero primer es controla si ja ha estat generada
         //i que no tengui peces aïllades
         if( !llistaFiguresGenerades.contains(dadesFigura) &&
             figuraGenerada  // ){
                 && generaFiguresMassiuAtzar_ComprovaCompactacio()){
             QString codi=QString("%1").arg(nombreInicialFigures+j);
             //qDebug("nombre figura: %d",nombreInicialFigures+j );
             while (codi.length()< QString("%1").arg(nombreFigures-1).length()){
               codi="0"+codi;
             }
             /*if ( !generaFiguresMassiuAtzar_ComprovaCompactacio()){
                 nomFigura="NO compacte.";
                 qDebug("Figura no compacte");
             }*/
             /* cal afegir el nivell de joc*/
             if(nomFigura=="Auto-2."){
                 desaFiguraAtzar(nomFigura,codi,true);
                 }
             else  desaFiguraAtzar(nomFigura,codi);
             llistaFiguresGenerades.append(dadesFigura);
             j++;
             if(progres.maximum()>0){
                progres.setValue(j);}
             figuresGenerades=j;
             progres.setLabelText(configuracioJoc->tradueixCadena("generant.figures")+"..."+
                                  QString("%1").arg(j));
             /* 07/09/15 El progres quedava sense tancar quan es seleccionava 1 figura
              */
             if(nombreFigures==j){
                 progres.close();
             }
             conjuntFigura->colocaPecesAtzar();
             qApp->processEvents();
         }

        /*
         if(!figuraGenerada){
             //qDebug("no s'ha pogut generar la figura!");
             conjuntFigura->colocaPecesAtzar();
             contadorErrorsFigura++;
         }*/

        qApp->processEvents();
        if(progres.wasCanceled()){            
            qApp->processEvents();
            break;}
       }//for numeroFigures

      // qDebug("Errors en montar figura %d",contadorErrorsFigura);

       if(massiu){
       QApplication::setOverrideCursor(Qt::ArrowCursor);
       conjuntFigura->conjuntVisible(false);
       conjuntMostra->conjuntVisible(false);
       //Cal informar a l'usuari de les figures generades
       //i la localització
       QString missatge;
       missatge.append(configuracioJoc->tradueixCadena("figures.generades")
                       .arg(figuresGenerades)+"\n");
       missatge.append(configuracioJoc->tradueixCadena("figures.generades.desades"));
       missatge.append(directoriFiguresPersonals()+charPathSeparator+"auto"+
                       charPathSeparator+configuracioJoc->arxiuDadesFigures);
       QMessageBox::information(this, configuracioJoc->tradueixCadena("generant.figures"),
                           missatge);
       }
   }//if val
}

/* Genera una figura amb les dades de suport.
 */
bool formulariPrincipal::generaFiguraAtzar(QStringList dadesPosicionsPecesList ,bool desaFigura){
    //QStringList dadesPosicionsPecesList=carregaDadesPosicionsPeces();
    if (dadesPosicionsPecesList.count()<1){
       // qDebug("sense dades");
        return false;
    }
    int nombre=generaNombreAtzar(rellotgeAtzar.elapsed());
    /*04/01/12
      Les peces es van col·locant segons l'ordre d'aquesta llista
      que es genera a l'atzar
      */
    QList<int> llistaIndexosPecesConjuntFigura=generaOrdrePecesGeneracioFiguraAtzar();
    /*03/07/15
     * Això no es fa servir!!!*/
    //int contadorPeces=0;

    bool figuraGenerada=true;

    /* Col·locam la primera peça més o menys enmig de la zona de joc*/
    conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura.value(
            nombre % conjuntFigura->arrayPeces.size())]->
            setPos(sceneJoc->sceneRect().center());

    for(int m=nombre+1;m<nombre+conjuntFigura->arrayPeces.size();m++){
        //qDebug("m: %d",llistaIndexosPecesConjuntFigura.value(m % conjuntFigura->arrayPeces.size()));
      /*23/07/15 No es considera el nivell*/
      //nivellFiguresAutomatiques=generaNombreAtzar(rellotgeAtzar.elapsed()) % 2;
      nivellFiguresAutomatiques=3;
      QStringList posicionsPeca;
      for(int n=nombre;n<m;n++){
          //qDebug("n: %d",llistaIndexosPecesConjuntFigura.value(n % conjuntFigura->arrayPeces.size()));
           //Per a cada una de les peces ja col·locades,
           //anam seleccionant les posibles col·locacions de la peça actual
           posicionsPeca.append(colocaPecesFiguraAtzar1(dadesPosicionsPecesList,
                         llistaIndexosPecesConjuntFigura.value(m % conjuntFigura->arrayPeces.size()),
                         llistaIndexosPecesConjuntFigura.value(n % conjuntFigura->arrayPeces.size())));
       }
       // qDebug("posicionsPeca %d", posicionsPeca.size());
       //I ara col·locam efectivament la peça a una de les posicions seleccionades
       int contadorIntents=1;
       bool pecaColocada=false;
       while (contadorIntents<posicionsPeca.size() && posicionsPeca.size()>0 && !pecaColocada){
           pecaColocada=colocaPeca(posicionsPeca,
                 llistaIndexosPecesConjuntFigura.value(m % conjuntFigura->arrayPeces.size()));
           /* 12/08/15 Comprovam que estigui pròxima a alguna peça augmentant l'escala de la peça
            * Això soluciona el problema de la col·locació de peces "separades" resultat de dades
            * sobre la col·locació de peces incorrectes o bé insuficients */
           if (pecaColocada){
           pecaColocada=conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura
                   .value(m % conjuntFigura->arrayPeces.size())]->pecaSuperposadaEscala(false,1.01);
             }
           /*Això va molt bé per veure el què passa
           conjuntJoc->conjuntVisible(false);
           conjuntFigura->conjuntVisible(true);
           //conjuntFigura->conjuntSolucio();
           qApp->processEvents();
           pausa(50);*/
           contadorIntents++;
           //qDebug("Colocada la peça %d",llistaIndexosPecesConjuntFigura.value(m % conjuntFigura->arrayPeces.size()));
           }

        /*16/05/14 si no s'ha aconseguit, cal que figuraGenerada=false. per
         * això es fa un darrer intent i s'actualitza figuraGenerada
         */
        if(!pecaColocada){
          return false;
        //qDebug("Ha entrat 1!");
        }
    }
    /*25/12/11
     Sempre desam les dades de la figura generada.
     Inicialment només es feia si es resolvia la figura*/
    if(figuraGenerada && desaFigura){
        qsrand(rellotgeAtzar.elapsed());
        desaFiguraAtzar("Auto.",QString("%1").arg(qrand()));
    }
 return figuraGenerada;
}


/*
  Genera una figura a l'atzar però donant prioritat a les posicions
  segons l'ordre de col·locació de les peces. Primer s'intenta col·locar
  la peça pròxima a la primera peça col·locada, si no és possible, llavors
  s'intenta amb la segona, etc
  26/08/15 Millorat el procediment.
  */
bool formulariPrincipal::generaFiguraAtzarCompacte(QStringList dadesPosicionsPecesList,bool desaFigura){

    if (dadesPosicionsPecesList.count()<1){
        qDebug("****NO HI HA DADES!!!****");
        return true;
    }

    int nombre=generaNombreAtzar(rellotgeAtzar.elapsed());
    /*04/01/12
      Les peces es van col·locant segons l'ordre d'aquesta llista
      que es genera a l'atzar
      */
    QList<int> llistaIndexosPecesConjuntFigura=generaOrdrePecesGeneracioFiguraAtzar();

    int contadorPeces=0;
    bool figuraGenerada=true;
    /* Col·locam la primera peça més o menys enmig de la zona de joc*/
    conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura.value(
            nombre % conjuntFigura->arrayPeces.size())]->
            setPos(sceneJoc->sceneRect().center());

    /* m és l'índex de la peça a col·locar*/
    int intentsColocacio=0;
    for(int m=nombre+1;m<nombre+conjuntFigura->arrayPeces.size();m++){
      contadorPeces++;
      /* 12/08/15 Això dels "nivells" s'ha abandonat */
      nivellFiguresAutomatiques=3;
     // qDebug("nivellFiguresAutomatiques %d compacte",nivellFiguresAutomatiques);
      QStringList posicionsPeca;      
      bool pecaColocada=false;
      /* n és l'índex de la peça ja col·locada */
      for(int n=nombre;n<m;n++){
           posicionsPeca.clear();
           /*24/07/15 Afegit aquest if per a què realment s'intenti posar cada peça
            * aprop de la primera peça col·locada*/
           if(!pecaColocada){
           //Per a cada una de les peces ja col·locades,
           //anam seleccionant les posibles col·locacions de la peça actual
           posicionsPeca.append(colocaPecesFiguraAtzar1(dadesPosicionsPecesList,
                         llistaIndexosPecesConjuntFigura.value(m % conjuntFigura->arrayPeces.size()),
                         llistaIndexosPecesConjuntFigura.value(n % conjuntFigura->arrayPeces.size())));
       //I ara col·locam efectivament la peça a una de les posicions seleccionades
       intentsColocacio=0;
       qsrand(rellotgeAtzar.elapsed());
       while(intentsColocacio<posicionsPeca.size() && posicionsPeca.size()>0 && !pecaColocada){
             pecaColocada=colocaPeca(posicionsPeca,
                        llistaIndexosPecesConjuntFigura.value(m % conjuntFigura->arrayPeces.size()));
             /* Comprovam que estigui pròxima a alguna peça augmentant l'escala de la peça*/
             if (pecaColocada){
             pecaColocada=conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura
                     .value(m % conjuntFigura->arrayPeces.size())]->pecaSuperposadaEscala(false,1.05);
             }
             /*Això va molt bé per veure el què passa
             conjuntJoc->conjuntVisible(false);
             conjuntFigura->conjuntVisible(true);
             qApp->processEvents();
             pausa(100);*/
             intentsColocacio++;
             }
           }
        }
       //qDebug("posicionsPeca %d",posicionsPeca.size());

         if(!pecaColocada){
        //Si no s'aconsegueix col·locar la peça, llavors es fa un segon intent
        //Aqui ja no s'intenta posar-la "compacte"
           posicionsPeca.clear();
           /* Innecessari
           conjuntFigura->arrayPeces[
           llistaIndexosPecesConjuntFigura.value(m % conjuntFigura->arrayPeces.size())]->reiniciaAnglesVertex();*/
         //int nivellAnterior=nivellFiguresAutomatiques;
           nivellFiguresAutomatiques=3;
           for(int b=nombre;b<m;b++){
               //Per a cada una de les peces ja col·locades,
               //anam seleccionant les posibles col·locacions de la peça actual
               posicionsPeca.append(colocaPecesFiguraAtzar1(dadesPosicionsPecesList,
                     llistaIndexosPecesConjuntFigura.value(m % conjuntFigura->arrayPeces.size()),
                     llistaIndexosPecesConjuntFigura.value(b % conjuntFigura->arrayPeces.size())));
           }
        intentsColocacio=0;
        while(intentsColocacio<posicionsPeca.size() && posicionsPeca.size()>0 && !pecaColocada){
              pecaColocada=colocaPeca(posicionsPeca,
                           llistaIndexosPecesConjuntFigura.value(m % conjuntFigura->arrayPeces.size()));
              /* Comprovam que estigui pròxima a alguna peça augmentant l'escala de la peça
               * per tal d'evitar que quedin peces lliures */
              if (pecaColocada){
              pecaColocada=conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura
                      .value(m % conjuntFigura->arrayPeces.size())]->pecaSuperposadaEscala(false,1.05);
               }
              intentsColocacio++;
            //qDebug("while generant figura");
        }
       /* Si no s'ha aconseguit col·locar la peça,
        * no es pot generar la figura, i sortim
        */
       if(!pecaColocada){
           return false;
         //qDebug("Ha entrat 2!");
          }
         }
       /* 26/08/15 aqueste dues linies s'han passat més adalt*/
       //}//if inicial pecaColocada
      //}//final segon for
     }//final del for inicial
    /*25/12/11
     Sempre desam les dades de la figura generada.
     Inicialment només es feia si es resolvia la figura*/
    if(figuraGenerada && desaFigura){
       // qsrand(rellotgeAtzar.elapsed());
        desaFiguraAtzar("Auto.",QString("%1").arg(generaNombreAtzar(rellotgeAtzar.elapsed())));
    }
   // else qDebug("NO s'ha pogut generar la figura!");
 return figuraGenerada;
}

/*
  Genera una llista ordenada a l'atzar dels indexs de les
  peces en el vector del conjunt.
  */
QList<int> formulariPrincipal::generaOrdrePecesGeneracioFiguraAtzar(){
  QList<int> llista;
  /* 09/03/14
   * S'ha canviat el sistema de generació a l'atzar per fer-ho
   * més ràpid i augmentar "l'atzar" generat.
   */
  qsrand(rellotgeAtzar.elapsed());
  while (llista.size()<conjuntFigura->arrayPeces.size()){
     // int nombre=generaNombreAtzar(rellotgeAtzar.elapsed());
      int nombre=qrand();
      if(!llista.contains(nombre % conjuntFigura->arrayPeces.size())){
          llista.append(nombre % conjuntFigura->arrayPeces.size());
      }
  }
 /* qDebug("llista.size: %d",llista.size());
  for(int m=0;m<llista.size();m++){
   qDebug("llista.value: %d",llista.value(m));
  }*/
return llista;
}

/* Carrega les dades de l'arxiu de les posicions de les peces
 * per la construcció a l'atzar de noves figures.
 * 09/08/15 Modificat per reduir el nombre de files dels arxius de dades.
 * Ara, es calculen part de les dades.
 */
QStringList formulariPrincipal::carregaDadesPosicionsPeces(){
 QApplication::setOverrideCursor(Qt::WaitCursor);
 qApp->processEvents();
  QString nomArxiuDades=configuracioJoc->arxiuDadesFigures.mid(0,
         configuracioJoc->arxiuDadesFigures.length()-4);
  nomArxiuDades.append(".aut");
  nomArxiuDades=directoriLinux()+charPathSeparator+"auto"+charPathSeparator
                +nomArxiuDades;

  QFile arxiu(nomArxiuDades);
  QStringList llista;
  if (arxiu.open(QIODevice::ReadOnly)){
      QTextStream lectura(&arxiu);
      QString linia0 = lectura.readLine();
      int contador=0;
      while (!linia0.isNull()) {

          if( !linia0.contains("##")  ){
              llista.append(linia0);
              contador++;
              /* Aqui es calculen les posicions invertint la peça de referència*/
              if ((contador % 3)==0){
                QStringList capcalera=llista.value(llista.size()-3).split(" ");
                QStringList primerPeca=llista.value(llista.size()-2).split(" ");
                QStringList segonaPeca=llista.value(llista.size()-1).split(" ");
                llista.append(QString("%1 %2 %3").arg(capcalera.value(0))
                                                 .arg(capcalera.value(2))
                                                 .arg(capcalera.value(1)));
                llista.append(QString("%1 0 0 %2 %3").arg(capcalera.value(2))
                                                 .arg(segonaPeca.value(3))
                                                 .arg(segonaPeca.value(4)));
                llista.append(QString("%1 %2 %3 %4 %5").arg(capcalera.value(1))
                                                 .arg(-segonaPeca.value(1).toFloat())
                                                 .arg(-segonaPeca.value(2).toFloat())
                                                 .arg(primerPeca.value(3))
                                                 .arg(primerPeca.value(4)));
               /* qCritical("%s", qPrintable(llista.value(llista.size()-3)));
                qCritical("%s", qPrintable(llista.value(llista.size()-2)));
                qCritical("%s", qPrintable(llista.value(llista.size()-1)));*/
              }

          }
          linia0 = lectura.readLine();
         // qCritical("%s", qPrintable(linia0));
   }
   arxiu.close();
  }


  /* Segon arxiu
  nomArxiuDades.clear();
    nomArxiuDades.append("ta-orig.aut");
    nomArxiuDades=directoriLinux()+charPathSeparator+"auto"+charPathSeparator
                  +nomArxiuDades;

    QFile arxiu2(nomArxiuDades);
    QStringList llista2;
    if (arxiu2.open(QIODevice::ReadOnly)){
        QTextStream lectura2(&arxiu2);
        QString linia0 = lectura2.readLine();
        while (!linia0.isNull()) {

            if( !linia0.contains("##")  ){
                llista2.append(linia0);
                }
            linia0 = lectura2.readLine();
           // qCritical("%s", qPrintable(linia0));
     }
     arxiu2.close();
    }

  //Comparam les dues llistes
  for(int m=0;m<llista2.size();m++){
      //for(int m=0;m<15;m++){
     if(llista2.value(m) !=llista.value(m))  {
       qCritical("%s", qPrintable(llista2.value(m)));
       qCritical("%s", qPrintable(llista.value(m)));
     }
  }*/


  /*Final segon arxiu*/

  QApplication::setOverrideCursor(Qt::ArrowCursor);
  qApp->processEvents();
  //qDebug("llista.size() %d",llista.size() );
  return llista;
}

QStringList formulariPrincipal::colocaPecesFiguraAtzar1(QStringList llistaDades1,
                                                int pecaAColocar1,int pecaColocada1){
    return seleccionaDadesPosicionsPeces(
                   llistaDades1, pecaAColocar1 ,pecaColocada1);
}


/*
  Selecciona les posicions en que es pot col·locar la peça
  */
QStringList formulariPrincipal::seleccionaDadesPosicionsPeces(QStringList llistaDades2,
                                          int pecaAColocar2,int pecaColocada2){
//qDebug("llistaDades2.size %d", llistaDades2.size());
    QStringList dades;
    for(int m=0;m<llistaDades2.size()/3;m++){
        QString dadesTipusPeces=llistaDades2.value(m*3);
        /*Això per controlar la lectura de les dades*/
        //qCritical("dadesTipusPeces '%s'", qPrintable(dadesTipusPeces));
        /* L'espai inicial és IMPORTANT per evitar que es seleccionin
           com iguals els codis (que acaben amb el mateix número)
           (p. ex. 3 i 13)
           També és important la condició de la longitud de les cadenes:
           evitant considerar com iguals " 1 1"  i " 1 16"
           (que és el que passava amb el quadrat del tangram t5II)
          */
        QString codis=QString(" %1 %2")
                      .arg(conjuntFigura->arrayPeces[pecaColocada2]->tipusPeca)
                      .arg(conjuntFigura->arrayPeces[pecaAColocar2]->tipusPeca);
       // qDebug("peces a colocar %d %d", conjuntFigura->arrayPeces[pecaColocada2]->tipusPeca,conjuntFigura->arrayPeces[pecaAColocar2]->tipusPeca);
       // qDebug("nivellFiguresAutomatiques seleccio %d",nivellFiguresAutomatiques);
        if ( dadesTipusPeces.contains(codis) &&
            (dadesTipusPeces.mid(0,1).toInt()<=nivellFiguresAutomatiques ) &&
            (dadesTipusPeces.length()==codis.length()+1)
            ){
          //Només el nivell
          dades.append(dadesTipusPeces.mid(0,1));
          dades.append(llistaDades2.value(m*3+1));
          dades.append(llistaDades2.value(m*3+2));
         // qDebug("seleccionada posicio");
        }
    }
    //qDebug("dades %d", dades.size());
    //Retorna la llista de posicions seleccionades
    /*for(int m=0;m<dades.size();m++){
      qCritical("%s", qPrintable(dades.value(m)));
    }*/
    return seleccionaPosicionsCorrectesPeces(dades,pecaAColocar2,pecaColocada2);
}

/*
  De les posicions posibles de la peça, selecciona aquelles que realment
  es pot col·locar
  */
QStringList formulariPrincipal::seleccionaPosicionsCorrectesPeces(QStringList llistaDades3,
                                int pecaAColocar3,int pecaColocada3){
    //conjuntFigura->arrayPeces[pecaAColocar3]->colocaPecaAtzar();
    int nombre=generaNombreAtzar(rellotgeAtzar.elapsed());

    QStringList posicionsComprovades;
    for(int t=nombre;t<nombre+llistaDades3.size()/3;t++){
       QStringList dadesPecaColocada=llistaDades3.value((t % (llistaDades3.size()/3))*3+1).split(" ");
       QStringList dadesPecaAColocar=llistaDades3.value((t % (llistaDades3.size()/3))*3+2).split(" ");

       /*
         Es comparen els angles de la peça i es comprova si s'ha fet la simetria
         Afegit els normalitzaAngle
         */
       if( (fabs( fabs(conjuntFigura->arrayPeces[pecaColocada3]->normalitzaAngle(
                      (conjuntFigura->arrayPeces[pecaColocada3]->anglesVertex[0]-
                       conjuntFigura->arrayPeces[pecaColocada3]->angleInicial)))-
                       fabs( conjuntFigura->arrayPeces[pecaColocada3]->normalitzaAngle(
                               dadesPecaColocada.value(3).toFloat())) )<0.1) &&
           ( (conjuntFigura->arrayPeces[pecaColocada3]->simetriaBool &&
            dadesPecaColocada.value(4)=="1") ||
             (!conjuntFigura->arrayPeces[pecaColocada3]->simetriaBool &&
                         dadesPecaColocada.value(4)=="0")
             )
                   ) {
          // qDebug("estem dins");
           QPointF vectorMoviment=QPointF(dadesPecaAColocar.value(1).toFloat(),
                                          dadesPecaAColocar.value(2).toFloat());

           conjuntFigura->arrayPeces[pecaAColocar3]->reiniciaAnglesVertex();
           conjuntFigura->arrayPeces[pecaAColocar3]->setPos(
                   conjuntFigura->arrayPeces[pecaColocada3]->pos()+
                   vectorMoviment);
           /* Cal considerar totes les possibilitats: que calgui fer la simetria o no
            * i que ja s'hagi fet la simetria o no*/
           if ( ((dadesPecaAColocar.value(4)=="1") &&
                (!conjuntFigura->arrayPeces[pecaAColocar3]->simetriaBool) ) ||
                ((dadesPecaAColocar.value(4)=="0") &&
                 (conjuntFigura->arrayPeces[pecaAColocar3]->simetriaBool)) ){
              conjuntFigura->arrayPeces[pecaAColocar3]->simetriaPeca();
           }
          /* if(dadesPecaAColocar.value(3).toFloat()<0){
               qDebug("angle negatiu!! %d %f", conjuntFigura->arrayPeces[pecaAColocar3]->tipusPeca,
                          dadesPecaAColocar.value(3).toFloat());
               qDebug("canviat %f",conjuntFigura->arrayPeces[pecaAColocar3]->normalitzaAngle(
                          dadesPecaAColocar.value(3).toFloat()));
           }*/
           conjuntFigura->arrayPeces[pecaAColocar3]
                   ->setAngleRotacio(dadesPecaAColocar.value(3).toDouble());

/*Això va molt bé per veure el què passa
 * no funcina bé amb conjuntSolucio()!!*/
/*conjuntJoc->conjuntVisible(false);
conjuntFigura->conjuntVisible(true);
//conjuntFigura->conjuntSolucio();
qApp->processEvents();
pausa(20);*/

          // if (!conjuntFigura->arrayPeces[pecaAColocar3]->pecaSuperposada(false) &&
           if ( (!conjuntFigura->arrayPeces[pecaAColocar3]->pecaSuperposadaEscala(false,0.95)) &&
               (conjuntFigura->arrayPeces[pecaAColocar3]->pecaSuperposadaEscala(false,1.01)) &&
               (conjuntFigura->arrayPeces[pecaAColocar3]->pecaEnLimits()) ){
               posicionsComprovades.append(QString("%1 %2 %3 %4").
                                        arg(conjuntFigura->arrayPeces[pecaColocada3]->pos().x()+
                                            vectorMoviment.x()).
                                        arg(conjuntFigura->arrayPeces[pecaColocada3]->pos().y()+
                                            vectorMoviment.y()).
                                        arg(dadesPecaAColocar.value(3)).
                                        arg(dadesPecaAColocar.value(4)) );
/*qCritical("%s", qPrintable(QString("%1 %2 %3 %4").
                               arg(conjuntFigura->arrayPeces[pecaColocada3]->pos().x()+
                                   vectorMoviment.x()).
                               arg(conjuntFigura->arrayPeces[pecaColocada3]->pos().y()+
                                   vectorMoviment.y()).
                               arg(dadesPecaAColocar.value(3)).
                               arg(dadesPecaAColocar.value(4))));*/
              }
           /* 20/08/15 Això no pareix que serveixi per res
            * else {
             // conjuntFigura->arrayPeces[pecaAColocar3]->colocaPecaAtzar();
              // qDebug("peça sense colocar");
           }*/
       }
   }
//qDebug("posicionsComprovades.size() %d", posicionsComprovades.size());
return posicionsComprovades;
}


bool formulariPrincipal::colocaPeca(QStringList llistaDades5, int pecaAColocar5){
    //qDebug("Posicions de la peca %d", pecaAColocar5);
    //qDebug("Numero de posicions de la peca: %d",llistaDades5.count());
   // conjuntFigura->conjuntVisible(true);
    bool pecaColocada=true;
    //qsrand(llistaDades5.count()*conjuntJoc->arrayPeces[0]->pos().x());
    if (llistaDades5.count()>0){        
    //qsrand(rellotgeAtzar.elapsed());
    int posicioSelecionada=qrand();
    QStringList dadesPecaAColocar=
            llistaDades5.value(posicioSelecionada % llistaDades5.size()).split(" ");

    QPointF posicio(dadesPecaAColocar.value(0).toFloat(),
                            dadesPecaAColocar.value(1).toFloat());
    conjuntFigura->arrayPeces[pecaAColocar5]->reiniciaAnglesVertex();
    conjuntFigura->arrayPeces[pecaAColocar5]->setPos(posicio);

    if(dadesPecaAColocar.value(3)=="1"){
      conjuntFigura->arrayPeces[pecaAColocar5]->simetriaPeca();
    }

    conjuntFigura->arrayPeces[pecaAColocar5]->setAngleRotacio(dadesPecaAColocar.value(2).toDouble());
    //Angles negatius
    /*if(dadesPecaAColocar.value(2).toFloat()>=0){
    conjuntFigura->arrayPeces[pecaAColocar5]->setAngleRotacio(dadesPecaAColocar.value(2).toFloat());}
    else {
        qDebug("angle negatiu");
    conjuntFigura->arrayPeces[pecaAColocar5]->setAngleRotacio(
                conjuntFigura->arrayPeces[pecaAColocar5]->normalitzaAngle(
                              dadesPecaAColocar.value(2).toFloat()));}*/

    /*Aferra peça
    26/07/15 Eliminat
    conjuntFigura->arrayPeces[pecaAColocar5]->setPos(
                 conjuntFigura->arrayPeces[pecaAColocar5]->pos()-
                 conjuntFigura->arrayPeces[pecaAColocar5]->aferraPeca());*/
    //pausa(200);
    //qApp->processEvents();
    //qDebug("Col·locada la peça: %d",pecaAColocar5);
    }
    else {
        pecaColocada=false;
        //pausa(200);
        //qDebug("No s'ha pogut col·locar la peça: %d",pecaAColocar5);
    }
    return pecaColocada;
}

/* Comprova si les peces estan col·locades de forma compacte
 * (sense peces aïllades)
 */
bool formulariPrincipal::generaFiguresMassiuAtzar_ComprovaCompactacio(){
  QList<int> pecesCompactes;
  pecesCompactes.append(0);
  int numeroCompactes=0;
  while (pecesCompactes.size()> numeroCompactes) {
  for(int m=0;m<pecesCompactes.size();m++){
      for(int n=m+1;n<conjuntFigura->arrayPeces.size();n++){
      if( conjuntFigura->arrayPeces[n]->pecaSuperposadaEscala(pecesCompactes,1.02)){
          if (!pecesCompactes.contains(n)){
               pecesCompactes.append(n);
            }
        }
       }
    }
   numeroCompactes++;
  }
  /*if(pecesCompactes.size() != conjuntFigura->arrayPeces.size()){
      qDebug("Figura no compacte");
  }*/
  return (pecesCompactes.size() == conjuntFigura->arrayPeces.size());
}

/*
  Classifica el nivell de dificultat de la figura actual segons
  el nombre de vèrtex
  */
int formulariPrincipal::calculaNivellDificultatFigura(){
   QPainterPath siluetaFigura;
   siluetaFigura=conjuntFigura->siluetaFigura();
   qreal longitudTotalPeces;
   longitudTotalPeces=conjuntFigura->longitudTotalPeces;
   //qreal longitudSilueta=siluetaFigura.length();
   /*22-04-14 canviat el sistema de càlcul*/
   qreal longitudSilueta=generaFiguraAtzar_SiluetaFigura();

   qreal index=2*longitudSilueta-longitudTotalPeces;
  // qreal percentatgeLongitudSilueta=longitudSilueta*100/longitudTotalPeces;
  // qreal percentatgeLongitudSilueta=(longitudTotalPeces-longitudSilueta)*100/longitudSilueta;

   int nivell=0;
   if(index<-40.0){
       nivell=3;
   }
   else if ( (index>=-40.0) && (index<=200.0)){
       nivell=2;
   }
   else if ( (longitudSilueta>(longitudTotalPeces-longitudSilueta))){
       nivell=1;
   }
   else nivell=0;

  // qDebug("longitudTotalPeces %f longitudSilueta %f",longitudTotalPeces,longitudSilueta );
  // qDebug("index %f", index);
 //  qDebug("nivell %d", nivell);
   /*if( (2*longitudSilueta-longitudTotalPeces)<-40.0 ){
       nivell=3;
   }
   else if(longitudSilueta>(longitudTotalPeces-longitudSilueta)){
       nivell=1;
   }
   if( (2*longitudSilueta-longitudTotalPeces >= -40.0) &&
       (2*longitudSilueta-longitudTotalPeces <= 200.0) ){
       nivell=2;
   }*/
   return nivell;
}

void formulariPrincipal::dadesPosicionsPeces(int nivell,int peca1, int peca2){
    QString nomArxiuDades=configuracioJoc->arxiuDadesFigures.mid(0,
           configuracioJoc->arxiuDadesFigures.length()-4);
    nomArxiuDades.append(".aut");
    QFile arxiu(nomArxiuDades);

    if (!arxiu.open(QIODevice::Append)){
        qDebug("No s'ha obert");
    }
    else {
    //desam les dades de la figura
        QTextStream entrada(&arxiu);
        QString propietats;
        entrada<<(QString("%1 %2 %3\n")
                 .arg(nivell)
                 .arg(conjuntJoc->arrayPeces[peca1]->tipusPeca)
                 .arg(conjuntJoc->arrayPeces[peca2]->tipusPeca));
        propietats=conjuntJoc->arrayPeces[peca1]->cadenaPropietatsPeca(
                conjuntJoc->arrayPeces[peca1]->pos());
        propietats=propietats.mid(0,propietats.length()-1);
        entrada<<(QString("%1\n").arg(propietats));
        propietats=conjuntJoc->arrayPeces[peca2]->cadenaPropietatsPeca(
                conjuntJoc->arrayPeces[peca1]->pos());
        propietats=propietats.mid(0,propietats.length()-1);
        entrada<<(QString("%1\n").arg(propietats));

        /* 11/08/15 Eliminat. Ara, aquestes dades es calculen
         * quan es carreguen les dades de l'arxiu
        entrada<<(QString("%1 %2 %3\n")
                 .arg(nivell)
                 .arg(conjuntJoc->arrayPeces[peca2]->tipusPeca)
                 .arg(conjuntJoc->arrayPeces[peca1]->tipusPeca));
        propietats=conjuntJoc->arrayPeces[peca2]->cadenaPropietatsPeca(
                conjuntJoc->arrayPeces[peca2]->pos());
        propietats=propietats.mid(0,propietats.length()-1);
        entrada<<(QString("%1\n").arg(propietats));
        propietats=conjuntJoc->arrayPeces[peca1]->cadenaPropietatsPeca(
                conjuntJoc->arrayPeces[peca2]->pos());
        propietats=propietats.mid(0,propietats.length()-1);
        entrada<<(QString("%1\n").arg(propietats));*/

        arxiu.close();
    }
}

/*
  Desa les dades de la figura generada a l'atzar a un arxiu
  */
void formulariPrincipal::desaFiguraAtzar(QString nom, QString codi, bool nivellJoc){
    QDir dirArxiu(directoriFiguresPersonals()+charPathSeparator+"auto");
    if (!dirArxiu.exists()){
         dirArxiu.mkdir(directoriFiguresPersonals()+charPathSeparator+"auto");
      }
    QString nomArxiuDades=configuracioJoc->arxiuDadesFigures;


    /* A l'arxiu de dades, posam el nivell més baix possible
     * (en realitat, les figures generades de nivell >2, por ésser
     * que es pugin solucionar amb els nivells inferiors
    */
    int nivellDelJoc=1;
    if (nivellJoc){
       if (configuracioJoc->nivellJoc>2){
       nivellDelJoc=3;
       }
    }

    QFile arxiu(dirArxiu.filePath(nomArxiuDades));
    if (arxiu.open(QIODevice::Append)){
        //desam les dades de la figura
        QTextStream entrada(&arxiu);
        //nom de la figura
        //entrada<<(QString("%1%2\n").arg("Auto.").arg(codi));
        entrada<<(QString("%1%2\n").arg(nom).arg(codi));
        entrada<<(QString("%1\n").arg(conjuntFigura->coordenadesFigura()));
        entrada<<(QString("%1 %2 0\n")
              //el nivell del joc és sempre 1
              /* 19/04/14 Amb el nou sistema de generació de figures a l'atzar,
               * això ja no és així*/
                  .arg(nivellDelJoc)
                  .arg(calculaNivellDificultatFigura()));
        arxiu.close();
    } //if obertura de l'arxiu
}

/*
  Controla l'estat activat/desactivat de les opcions de menú
  de la generació de noves figures
  */
void formulariPrincipal::controlaActivacioMenusFiguresAtzar(){
    /*17/09/15 Activada la generació de figures a l'atzar.
     * Queda restringida als nivells 1 i 2 i exclosos alguns jocs
     */
    generaFiguraAtzarAction->setEnabled((estatDelJoc !=JocIniciat) &&
                                        (controlaArxiuDadesPosicionsPeces()));
    generaFiguresMassiuAtzarAction->setEnabled((estatDelJoc !=JocIniciat) &&
                                               (controlaArxiuDadesPosicionsPeces()));
    generaFiguresAtzarAutomaticAction->setEnabled((estatDelJoc !=JocIniciat) &&
                                                 (tangramsSenseGeneracioAutomatica())
                                                  && (configuracioJoc->nivellJoc<=NIVELL_PRINCIPIANT));

    comprovaFiguresAtzarAction->setEnabled((estatDelJoc !=JocIniciat) );
    /*20/07/15
     * Afegit  generaFiguresAtzarAutomaticAction
     * El menú principal estarà activat si ho està algun dels submenús
     */
    generaFigures->setEnabled(generaFiguraAtzarAction->isEnabled() ||
                              generaFiguresMassiuAtzarAction->isEnabled() ||
                              generaFiguresAtzarAutomaticAction->isEnabled() ||
                              comprovaFiguresAtzarAction->isEnabled());
}

/* controla si el tangram actual te generació a l'atzar de noves figures*/
bool formulariPrincipal::tangramsSenseGeneracioAutomatica(){

 if( configuracioJoc->tipusTangram==22 || //circular 8
     configuracioJoc->tipusTangram==16 || //de l'ou
     configuracioJoc->tipusTangram==17 || //de cor
     configuracioJoc->tipusTangram==20 || //de circular I
     configuracioJoc->tipusTangram==32 || //de Stomach
     configuracioJoc->tipusTangram==18 || //de Stomachion   Stomachion modificat
     configuracioJoc->tipusTangram==31  //Stomachion modificat
         )  {
     return false;
     }
 else return true;
}

/*
  Comprova si hi ha un arxiu de dades de posicions de les peces
  corresponent al tipus de tangram actual
  */
bool formulariPrincipal::controlaArxiuDadesPosicionsPeces(){
    QDir dirArxiu(directoriLinux()+QDir().separator()+"auto");
    if (!dirArxiu.exists()){
         return false;
      }
    QString nomArxiuDades=configuracioJoc->arxiuDadesFigures.mid(0,
           configuracioJoc->arxiuDadesFigures.length()-4);
    nomArxiuDades.append(".aut");
    QFile arxiu(dirArxiu.filePath(nomArxiuDades));
    if (!arxiu.open(QIODevice::ReadOnly)){
        return false;
    }
    else{
      arxiu.close();
      return true;
    }
  return false;
}



/*
 Comprovació de les noves figures generades a l'atzar
*/
void formulariPrincipal::comprovaNovesFiguresAtzarSlot(){
ComprovaFigures comprovaFig(configuracioJoc,this);
comprovaFig.exec();
}


/*
 * Botó per fer la simetria d'una peça
 */
void formulariPrincipal::simetriaPecaSlot(){
    /*per les dades de posicions de les peces
    codiPeca1 i codiPeca2 són els índexos a
    l'array de les peces. Cal anar amb compte amb les simetries. Si la
    peça no és asimètrica, llavors cal vigilar que no tengui feta la simetria.
    */
    if(0==1){
    int codiPeca1=6;
    int codiPeca2=5;
    int nivell=0;

    conjuntJoc->arrayPeces[codiPeca1]->setColorLimits(Qt::red);
    conjuntJoc->arrayPeces[codiPeca2]->setColorLimits(Qt::black);
    dadesPosicionsPeces(nivell,codiPeca1,codiPeca2);
    return;
    }
 if (sceneJoc->selectedItems().count()>0){
    for(int i=0;i<=conjuntJoc->arrayPeces.size()-1;i++){
        if (conjuntJoc->arrayPeces[i]->isSelected()){
        conjuntJoc->arrayPeces[i]->simetriaPeca();
        conjuntJoc->arrayPeces[i]->update();
        break;
        }
    }
 }
}

/* 20/07/15
 * Inici del procediment de generació automàtica de figures
 * sense dades de suport
 */
void formulariPrincipal::generaFiguresAtzarSlot(){
    generaFiguresMassiuAtzarSlot(true,-1);
}

/* Generació automàtica de noves figures
 * sense dades de suport
 */
void formulariPrincipal::generaFiguraAtzar_inici(bool desaFigura){

/* iniciam la generació de figures*/
/* Si no es possible de cap manera fer una figura,
 * el programa quedarà penjat*/
while ( !generaFiguraAtzar_generaNovaFiguraAtzar() ){ //&& (contador<3)){
  }

    /* Desam les dades de la figura generada*/
    conjuntFigura->centraFiguraScene();
    if(desaFigura){
    qsrand(rellotgeAtzar.elapsed());
    desaFiguraAtzar("Auto-2.",QString("%1").arg(qrand()),true);
    generaFiguraAtzar_configuracioFinalProcess();}

qApp->processEvents();
}

/* process general de generació de figures a l'atzar
   (sense dades de suport) */
bool formulariPrincipal::generaFiguraAtzar_generaNovaFiguraAtzar(){

    //Generam la llista d'ordre de col·locació de les peces
    //a la nova figura
    QList<int> llistaIndexosPecesConjuntFigura=
            generaLlistaIndexosAtzar(conjuntFigura->arrayPeces.size());

    /* Col·locam la primera peça més o menys enmig de la zona de joc */
    conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura.value(0)]->
         setPos(sceneJoc->sceneRect().center());

    //Sovint, la peça col·locada enmig de la zona de joc, queda
    //superposada a una altra peça
    while(conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura.value(0)]->pecaSuperposada(false)){
        conjuntFigura->colocaPecesAtzar();
        conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura.value(0)]->
             setPos(sceneJoc->sceneRect().center());
    }

    /* Començam a col·locar peces */
    QList<int> indexPecesJaColocades;
    indexPecesJaColocades.append(llistaIndexosPecesConjuntFigura.value(0));
    /* Per a cada una de les peces (seguint l'ordre
     * establert a l'atzar)
     */
    for(int i=1;i<llistaIndexosPecesConjuntFigura.size();i++){
         /* Així com es col·loquen les peces, pot ésser que alguna
         * hagi quedat superposada i, llavors, no es col·loca a la
         * nova figura
         */
         //while(conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura.value(i)]->pecaSuperposada(false)){
         while(conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura.value(i)]->pecaSuperposadaEscala(false,0.98)){
               conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura.value(i)]->colocaPecaAtzar();
         }
         /*Això va molt bé per veure el què passa*/
         /*Cal descomentar el pausa del procediment generaFiguraAtzar_colocaPecaPunt2
         conjuntJoc->conjuntVisible(false);
         conjuntFigura->conjuntVisible(true);
         //conjuntFigura->conjuntSolucio();
         qApp->processEvents();
         pausa(1);*/

         /* Coloca peces als vertex fent-les girar*/
         bool pecaColocada=false;
         while(!pecaColocada ){
             pecaColocada=generaFiguraAtzar_colocaPecaPunt(llistaIndexosPecesConjuntFigura.value(i),
                                                            indexPecesJaColocades.value(0),
                                                            indexPecesJaColocades);
             /* Comprovam que la peça no quedi aïllada*/
             if (pecaColocada){
             pecaColocada=conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura.value(i)]
                          ->pecaSuperposadaEscala(false,1.01);
              if (!pecaColocada){
                 conjuntFigura->arrayPeces[llistaIndexosPecesConjuntFigura.value(i)]->colocaPecaAtzar();
                }
              }
         }
         if( !indexPecesJaColocades.contains(llistaIndexosPecesConjuntFigura.value(i))){
             indexPecesJaColocades.append(llistaIndexosPecesConjuntFigura.value(i));}

         //Per evitar que el formulari quedi sense refrescar quan està molt de temps
         //generant una figura
         qApp->processEvents();
       }

 /* qDebug("longitudSilueta  %f",conjuntFigura->longitudSilueta());
  qDebug("longitudSilueta-2  %f",generaFiguraAtzar_SiluetaFigura());

  qDebug("longitudTotalPeces  %f",conjuntFigura->longitudTotalPeces);
  qDebug("FINAL  %f",conjuntFigura->longitudSilueta()*100/conjuntFigura->longitudTotalPeces);*/

  /* això és important: no es pot calcular bé el % sense actualitzar-ho!*/
 // conjuntFigura->longitudSilueta();
/*  qDebug("FINAL corregit  %f",generaFiguraAtzar_SiluetaFigura()*
                 100/conjuntFigura->longitudTotalPeces);*/

  return (generaFiguraAtzar_SiluetaFigura()*100/conjuntFigura->longitudTotalPeces<95.0);
}

/* Col·loca la peça aferrada a les peces ja col·locades, simplement movent-les
 * segons la direcció dels centres respectius. Pel moviment, es fa servir
 * un procediment semblant al del moviment amb el ratolí
 */
bool formulariPrincipal::generaFiguraAtzar_colocaPecaPunt(int indexPeca, int indexPecaInicial,
                                         QList<int> indexPecesJaColocades){
   int contador=1;
   int sentitRotacio=pow(-1,qrand() % 2);
   QList<QPointF> posicionsProvades;
   int contadorRepeticio=0;
   conjuntFigura->arrayPeces[indexPeca]->movimentPeca=1;
   /* Els valors son empírics!*/
   while( (contador<15)  && (contadorRepeticio<20) ){

   generaFiguraAtzar_colocaPecaPunt2(indexPeca,indexPecaInicial,indexPecesJaColocades,
                                     sentitRotacio);
   //pausa();
   QPointF posicio;
   posicio.setX(sqrt(pow(conjuntFigura->arrayPeces[indexPeca]->pos().x(),2)+
                     pow(conjuntFigura->arrayPeces[indexPeca]->pos().y(),2)));
   posicio.setY(conjuntFigura->arrayPeces[indexPeca]->normalitzaAngle(
                    conjuntFigura->arrayPeces[indexPeca]->angleRotacio));

     if( !posicionsProvades.contains(posicio) ){
        posicionsProvades.append(posicio);
    }
    else{
        contadorRepeticio++;
    }
   contador++;
   }

  return true;
}

void formulariPrincipal::generaFiguraAtzar_colocaPecaPunt2(int indexPeca, int indexPecaInicial,
                                                    QList<int> indexPecesJaColocades,
                                                    int sentitRotacio){
    int contador=0;

    int contadorRepeticio=0;
    QList<QPointF> posicionsProvades;
    QPointF vectorCentres;
    int amplificador=1;
    amplificador=360/conjuntFigura->arrayPeces[indexPeca]->angleGirPeca;
    if(amplificador>50){
        amplificador=30;
    }

    /*if(configuracioJoc->nivellJoc>NIVELL_PRINCIPIANT){
       amplificador=2;
    }*/
    /* Els valors son empírics!*/
    while( (contador<amplificador*2) && (contadorRepeticio<amplificador*2) ){

      vectorCentres=conjuntFigura->arrayPeces[indexPecaInicial]->pos()+
                 conjuntFigura->arrayPeces[indexPecaInicial]->centrePeca()-
                 (conjuntFigura->arrayPeces[indexPeca]->pos()+
                 conjuntFigura->arrayPeces[indexPeca]->centrePeca());


       if(!conjuntFigura->arrayPeces[indexPeca]->generaFiguraAtzar_mouPeca(
                    conjuntFigura->arrayPeces[indexPeca]->pos()+vectorCentres*0.1,indexPecesJaColocades,
                    sentitRotacio)  ){
           /*si el valor de retorn és fals, llavors es canvia el sentit de rotació*/
           sentitRotacio=-sentitRotacio;           
         }
        /*aferra la peça només pels costats*/
        conjuntFigura->arrayPeces[indexPeca]->setPos(conjuntFigura->arrayPeces[indexPeca]->pos()-
              conjuntFigura->arrayPeces[indexPeca]->generaFiguraAtzar_aferraPeca(indexPecesJaColocades,
                                                                            distanciaAferraPeca));
        /*Descomentar per veure el moviment*/
       // pausa(10);
        /* això evita la pèrdua de temps quan la peça ja no es mou més*/
        QPointF posicio;
        posicio.setX(sqrt(pow(conjuntFigura->arrayPeces[indexPeca]->pos().x(),2)+
                         pow(conjuntFigura->arrayPeces[indexPeca]->pos().y(),2)));
        posicio.setY(conjuntFigura->arrayPeces[indexPeca]->normalitzaAngle(
                         conjuntFigura->arrayPeces[indexPeca]->angleRotacio));
        if( !posicionsProvades.contains(posicio)){
            posicionsProvades.append(posicio);
        }
        else {            
            contadorRepeticio++;
        }
        contador++;
    }

    /*això és important per no deixar la peça inmòvil*/
    conjuntFigura->arrayPeces[indexPeca]->movimentPeca=1;
    conjuntFigura->arrayPeces[indexPeca]->generaFiguraAtzar_aferraPecaLentament(indexPecesJaColocades);
    conjuntFigura->arrayPeces[indexPeca]->movimentPeca=1;

    //if(conjuntFigura->arrayPeces[indexPeca]->pecaSuperposada(indexPecesJaColocades)){
    if(conjuntFigura->arrayPeces[indexPeca]->pecaSuperposadaEscala(indexPecesJaColocades,0.99)){
       conjuntFigura->arrayPeces[indexPeca]->generaFiguraAtzar_separaPecaLentament(indexPecesJaColocades,
                                        conjuntFigura->arrayPeces[indexPecaInicial]->pos()+
                                        conjuntFigura->arrayPeces[indexPecaInicial]->centrePeca()-
                                        (conjuntFigura->arrayPeces[indexPeca]->pos()+
                                        conjuntFigura->arrayPeces[indexPeca]->centrePeca()));
    }
    conjuntFigura->arrayPeces[indexPeca]->generaFiguraAtzar_aferraPecaLentament(indexPecesJaColocades,false);

}


/* Va generant al silueta de la nova figura generada
 */
qreal formulariPrincipal::generaFiguraAtzar_SiluetaFigura(){
    conjuntFigura->longitudSilueta();
    QPainterPath siluetaFigura;
    QPainterPath siluetaPeca;
    siluetaFigura.setFillRule(Qt::WindingFill);
    for(int i=0;i<conjuntFigura->arrayPeces.size();i++){
    siluetaPeca=conjuntFigura->arrayPeces[i]->shapeEscala(1.004);
    siluetaFigura=siluetaFigura.united(siluetaPeca.translated(
                                      conjuntFigura->arrayPeces[i]->pos()));

    siluetaFigura.simplified();
    }
// qDebug("siluetaFigura.length() %f", siluetaFigura.length());
 // return siluetaFigura.simplified();
  return siluetaFigura.length();
}

/* Comprova si la silueta de la figura és inferior al
 * perímetre total de les peces
 */
bool formulariPrincipal::generaFiguraAtzar_longitudSiluetaFigura(){
  qreal longitudPerimetrePeces=0.0;
  QPainterPath siluetaFigura;
  QPainterPath siluetaPeca;
  siluetaFigura.setFillRule(Qt::WindingFill);
  for(int i=0;i<conjuntFigura->arrayPeces.size();i++){
   siluetaPeca=conjuntFigura->arrayPeces[i]->shape();
   siluetaFigura=siluetaFigura.united(siluetaPeca.translated(
                      conjuntFigura->arrayPeces[i]->pos()));
   siluetaFigura.simplified();
   longitudPerimetrePeces=longitudPerimetrePeces+siluetaPeca.length();
  }
 //qDebug("perimetres %f %f",siluetaFigura.length(),longitudPerimetrePeces);
 //sceneJoc->addPath(siluetaFigura);
 return siluetaFigura.length()<longitudPerimetrePeces;
}

/* Realitza les tasques inicials comuns als dos processos de generació
 * de noves figures (amb dades i a l'atzar)
 */
void formulariPrincipal::generaFiguraAtzar_configuracioInicialProces(){
    missatgeStatusBar("");
    conjuntJoc->colocaPecesAtzar();
    if(configuracioJoc->tipusTangram==29 //TIPUS_TANGRAM_TE
        ){
        for(int j=0;j<conjuntFigura->arrayPeces.size();j++){
         conjuntFigura->arrayPeces[j]->estableixAngleGirPeca(45.0);
        }
    }
    /* 07/09/15 Hi hauria d'haver
    else { */
    /*24/04/14 Afegit per garantir que el conjunt te aquesta propietat
     * correctament establerta a l'hora de genera noves figures a l'atzar*/
        for(int j=0;j<conjuntFigura->arrayPeces.size();j++){
         conjuntFigura->arrayPeces[j]->estableixAngleGirPeca(configuracioJoc->angleGir);
        }
    //}

    conjuntFigura->conjuntDeMostra();
    conjuntFigura->conjuntVisible(false);
    conjuntFigura->colocaPecesAtzar();
    conjuntMostra->conjuntVisible(false);
    qApp->processEvents();
}

/* Fa les tasques necessàries per a què, una vegada generada una
 * nova figura, es pugi començar a jugar*/
void formulariPrincipal::generaFiguraAtzar_configuracioFinalProcess(){
conjuntFigura->conjuntVisible(true);
conjuntFigura->centraFiguraScene();
//Ara preparam per a què es pugui jugar
//a resoldre la nova figura
conjuntMostra->colocaPeces(
        conjuntFigura->coordenadesFigura()
        );
conjuntMostra->conjuntVisible(true);
conjuntMostra->conjuntDeMostra();
conjuntFigura->conjuntDeMostra();
if(configuracioJoc->nivellJoc==NIVELL_EXPERT ){
    conjuntFigura->conjuntVisible(false);
}
missatgeStatusBar(configuracioJoc->tradueixCadena("nivell.figura.generada")
                  .arg(calculaNivellDificultatFigura()));
canviaEstatDelJoc(FiguraCarregada);
}

/* Retorna una llista de nombres enters ordenats
 * a l'atzar
 */
QList<int> formulariPrincipal::generaLlistaIndexosAtzar(int nombreIndexos){
    QList<int> llista;
    qsrand(rellotgeAtzar.elapsed());
    while (llista.size()<nombreIndexos){
        int nombre=qrand();
        if(!llista.contains(nombre % nombreIndexos)){
            llista.append(nombre % nombreIndexos);
        }
    }
  return llista;
}

/*
  Generació d'arxius html
  */
void formulariPrincipal::HTML(){

//primer posam el nivell iniciat
configuracioJoc->nivellJoc=3;
//Això per forçar el canvi de nivell de joc
//nivellJocActual=-1;
canviaNivellJoc(configuracioJoc->nivellJoc);
int total=40;
//per a cada tipus de tangram
for(int i=1;i<=total;i++){
   //seleccionam el tangram
   configuracioJoc->canviaTipusTangram(i);
   creaTangram(i);
   //generam els directoris
   directorisHTML(configuracioJoc->nomJocArbreFigures);

   //imatge inicial del tangram
   figuraInicialTangram(i);
   HTML_imatgeTangram(configuracioJoc->arxiuDadesFigures.left(
           configuracioJoc->arxiuDadesFigures.indexOf(".")));

   //generam l'arxiu HTML
   HTML_creaArxiuHTML(arbreFiguresTreeWidget->topLevelItem(0)->childCount());
   //generam l'arxiu latex
   arxiuLatex();

   QString nomArxiu;
   QProgressDialog progres(
           configuracioJoc->tradueixCadena("Generant arxius")+"..."+
           QString("%1/%2").arg(i).arg(total),
           configuracioJoc->tradueixCadena("atura.cerca"), 0,
           arbreFiguresTreeWidget->topLevelItem(0)->childCount(), this);
   progres.setWindowModality(Qt::WindowModal);
   progres.setVisible(true);

   //per a cada figura de l'arbre
   for(int j=0;j<arbreFiguresTreeWidget->topLevelItem(0)->childCount();j++){
   //for(int j=0;j<0;j++){
       progres.setValue(j+1);

       conjuntMostra->colocaPeces(arbreFiguresTreeWidget->topLevelItem(0)->child(j)->text(1));
       //sceneMostra->update(sceneMostra->sceneRect());

       nomArxiu=QString::number(j);
       while (nomArxiu.length()<4){nomArxiu="0"+nomArxiu;}
       HTML_creaArxiuImatge(nomArxiu);
       qApp->processEvents();
       if(progres.wasCanceled()){
           /*canviaNivellJoc(NIVELL_INFANTIL);
           configuracioJoc->canviaTipusTangram(jocInicial);           
           creaTangram(jocInicial);
           HTML_creaArxiuIndex();*/
           return;}
   }//for de l'arbre de figures
}//for del tipus de tangram
HTML_creaArxiuIndex();
}

void formulariPrincipal::HTML_creaArxiuImatge(QString nomArxiu){
    QRectF rectImatge=conjuntMostra->rectangleFigura().adjusted(-5,-5,5,5);
    QPixmap pixMap = QPixmap::grabWidget(viewMostra,rectImatge.x(),
                                         rectImatge.y(),
                                         rectImatge.width(),
                                         rectImatge.height());
    pixMap.createMaskFromColor(Qt::white,Qt::MaskInColor);
    QString nomArxiuImatge=directoriFiguresPersonals()+QDir().separator()+
                           QString("html")+QDir().separator()+QString("img")+QDir().separator()+
                           configuracioJoc->nomJocArbreFigures+QDir().separator()
                                 +nomArxiu+".png";
    pixMap.save(nomArxiuImatge);
    conjuntMostra->conjuntSolucio();
    QPixmap pixMap1 = QPixmap::grabWidget(viewMostra,rectImatge.x(),
                                         rectImatge.y(),
                                         rectImatge.width(),
                                         rectImatge.height());
    conjuntMostra->conjuntDeMostra();
    sceneMostra->update(sceneMostra->sceneRect());
    pixMap1.createMaskFromColor(Qt::white,Qt::MaskInColor);
    nomArxiuImatge=directoriFiguresPersonals()+QDir().separator()+
                   QString("html")+QDir().separator()+QString("img")+QDir().separator()+
                   configuracioJoc->nomJocArbreFigures+QDir().separator()
                                 +nomArxiu+"_sol.png";
    pixMap1.save(nomArxiuImatge);
}

void formulariPrincipal::HTML_imatgeTangram(QString nomArxiu){

    QRectF rectImatge=conjuntJoc->rectangleFigura().adjusted(-20,-20,20,20);

    QPixmap pixMap = QPixmap::grabWidget(viewJoc,rectImatge.x(),
                                         rectImatge.y(),
                                         rectImatge.width(),
                                         rectImatge.height());
    pixMap.createMaskFromColor(Qt::white,Qt::MaskInColor);
    QString nomArxiuImatge=directoriFiguresPersonals()+QDir().separator()+
                           QString("html")+QDir().separator()+QString("img")+QDir().separator()+
                           configuracioJoc->nomJocArbreFigures+QDir().separator()
                           +nomArxiu+".png";
    //qCritical("%s", qPrintable(nomArxiuImatge));
    pixMap.save(nomArxiuImatge);
}

void formulariPrincipal::HTML_creaArxiuHTML(int numFigures){
    int numeroFiguresPerLinea=15;
    QString nomArxiuHTML=
      configuracioJoc->arxiuDadesFigures.replace(configuracioJoc->arxiuDadesFigures.indexOf(".")+1,3,"html");

    //nomArxiuHTML=nomArxiuHTML.replace(nomArxiuHTML.indexOf(".")+1,3,"html");
    QDir dirArxiu(directoriFiguresPersonals()+charPathSeparator+"html");
    QFile arxiu(dirArxiu.filePath(nomArxiuHTML));

    QTextStream entrada(&arxiu);
    //capçalera de l'arxiu
    if (!arxiu.open(QIODevice::WriteOnly)){return;}
    QString titolTangram=configuracioJoc->tradueixCadena(configuracioJoc->nomJocArbreFigures);
    //titolTangram="Tangram"+titolTangram.mid(5);
    entrada<<(QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">"));
    entrada<<(QString("\n"));
    entrada<<(QString("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />"));
    entrada<<(QString("\n"));
    entrada<<(QString("<html><head><br>"));
    entrada<<(QString("\n"));
    entrada<<(QString("<title>%1</title>").arg(titolTangram));
    entrada<<(QString("\n"));
    entrada<<(QString("</head>"));
    entrada<<(QString("\n"));
    entrada<<(QString("<body>"));
    entrada<<(QString("\n"));
    entrada<<(QString("<div style=\"text-align: center;\"><big><big><big><big>%1</big></big></big></big><br><br></div>")
              .arg(titolTangram));
    entrada<<(QString("\n"));
    entrada<<(QString("<div style=\"text-align: center;\"><img src=\"img%1/%2.png\"><br>")
              .arg(QDir().separator()+configuracioJoc->nomJocArbreFigures)
              .arg(configuracioJoc->arxiuDadesFigures.left(
                      configuracioJoc->arxiuDadesFigures.indexOf("."))));
    entrada<<(QString("\n"));

    //per a cada figura afegim la linea a l'arxiu HTML
    QString nomArxiu;
    for(int j=0;j<numFigures;j++){
        if( (j%numeroFiguresPerLinea==0) && j==0){
            entrada<<(QString("<small><small><small>%1</small></small></small>").arg(j+1));
          //  entrada<<(QString("<small><small><small>%1-%2</small></small></small>").arg(j+1).arg(j+numeroFiguresPerLinea));
        }
        if (j !=0){
            if( (j%numeroFiguresPerLinea==0)){
                entrada<<(QString("<br>"));
                entrada<<(QString("\n"));
                entrada<<(QString("<small><small><small>%1</small></small></small>").arg(j+1));
               // entrada<<(QString("<small><small><small>%1-%2</small></small></small>").arg(j+1).arg(j+numeroFiguresPerLinea));
            }}
        nomArxiu=QString::number(j);
        while (nomArxiu.length()<4){nomArxiu="0"+nomArxiu;}
    entrada<<(QString("<img src=\"img%1/%2.png\" id=\"imagen\" onmouseover=\"this.src='img%1/%2_sol.png'\" onmouseout=\"this.src='img%1/%2.png'\">")
              .arg(QDir().separator()+configuracioJoc->nomJocArbreFigures).arg(nomArxiu));
    entrada<<(QString("\n"));

    }
    entrada<<(QString("</div>"));
    entrada<<HTML_llicencia();
    entrada<<(QString("</body></html>"));
    arxiu.close();
}


void formulariPrincipal::HTML_creaArxiuIndex(){
QApplication::setOverrideCursor(Qt::WaitCursor);

QString nomArxiuIndex("index.html");
QDir dirArxiu(directoriFiguresPersonals()+charPathSeparator+"html");
QFile arxiuIndex(dirArxiu.filePath(nomArxiuIndex));

QTextStream entradaIndex(&arxiuIndex);
if (!arxiuIndex.open(QIODevice::WriteOnly)){
    qDebug("falla!");
    return;}
//QString titolTangram=configuracioJoc->tradueixCadena(configuracioJoc->nomJocArbreFigures);
//titolTangram="Tangram"+titolTangram.mid(5);
entradaIndex<<(QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">"));
entradaIndex<<(QString("\n"));
entradaIndex<<(QString("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />"));
entradaIndex<<(QString("\n"));
entradaIndex<<(QString("<html><head><br>"));
entradaIndex<<(QString("\n"));
entradaIndex<<(QString("<title>Index Tangram</title>"));
entradaIndex<<(QString("\n"));
entradaIndex<<(QString("</head>"));
entradaIndex<<(QString("\n"));
entradaIndex<<(QString("<body>"));
entradaIndex<<(QString("\n"));
entradaIndex<<(QString("<div style=\"text-align: center;\"><big><big><big><big>%1</big></big></big></big><br><br></div>")
          .arg(QString("<h2>"+configuracioJoc->tradueixCadena("credits.versio")+""
                       +QString(" v. %1 </h2><br/>").
                                                    arg(qApp->applicationVersion()))));

entradaIndex<<(QString("\n"));
entradaIndex<<HTML_llicencia();

entradaIndex<<(QString("<div style=\"text-align: center;\">"));


//int total=39;
int numeroFiguresPerLinea=5;
QStringList dadesJocs=dadesTangram();
//per a cada tipus de tangram
for(int i=0;i<dadesJocs.count();i++){
QStringList dades=dadesJocs.value(i).split(";");
configuracioJoc->canviaTipusTangram(dades.value(0).toInt());
QString titolTangram=configuracioJoc->tradueixCadena(configuracioJoc->nomJocArbreFigures);
//titolTangram="Tangram"+titolTangram.mid(5);
if (i !=0){
    if( (i%numeroFiguresPerLinea==0)){
        entradaIndex<<(QString("<br>"));
        entradaIndex<<(QString("\n"));}
}
entradaIndex<<(QString("<a href=\"file:///%1.html\">")
             .arg(directoriFiguresPersonals()+QDir().separator()+"html"
                 +QDir().separator()+configuracioJoc->arxiuDadesFigures.left(
                         configuracioJoc->arxiuDadesFigures.indexOf("."))));
entradaIndex<<(QString("<img title=\"%1\" src=\"img%2/%3.png\"></a>")
          .arg(titolTangram)
          .arg(QDir().separator()+configuracioJoc->nomJocArbreFigures)
          .arg(configuracioJoc->arxiuDadesFigures.left(
                  configuracioJoc->arxiuDadesFigures.indexOf("."))));
}

entradaIndex<<(QString("</div>"));
entradaIndex<<(QString("</body></html>"));
arxiuIndex.close();
QApplication::setOverrideCursor(Qt::ArrowCursor);
}

QString formulariPrincipal::HTML_llicencia(){
    QString missatge;
    QDate avui=QDate::currentDate () ;
    missatge.append(QString("&copy;  "+QString("2000-%1").arg(avui.year())+"  I. De Marchi <br/>" ));
    missatge.append(QString("\n"));
    missatge.append(QString(configuracioJoc->tradueixCadena("credits.llicencia")+"<br/>"));
    missatge.append(QString("\n"));
    missatge.append(QString("<a href=\"%1\">%1</a><br/>").arg(ADRECA_WEB_PROGRAMA));
    missatge.append(QString("\n"));
    missatge.append(QString("<a href=mailto:%1>%1</a><br/>").arg(ADRECA_CORREU));
    missatge.append(QString("\n"));
return missatge;
}

/*
  Crea l'arxiu latex per les figures
  */
void formulariPrincipal::arxiuLatex(){

   QApplication::setOverrideCursor(Qt::WaitCursor);


   //QString nomArxiuIndex("index.html");
   QDir dirArxiu(directoriFiguresPersonals()+charPathSeparator+"latex");
   //QFile arxiuIndex(dirArxiu.filePath(nomArxiuIndex));

  //QFile arxiu(dirArxiu.filePath(dirArxiu.filePath(configuracioJoc->nomJocArbreFigures+".tex")));
  QString nomArxiuCanviat=configuracioJoc->tradueixCadena(configuracioJoc->nomJocArbreFigures);
  QString nomArxiuCanviat1=nomArxiuCanviat.replace(QRegExp(" "),"_");
  QFile arxiu(dirArxiu.filePath(dirArxiu.filePath(nomArxiuCanviat1+".tex")));
  //if(arxiu.exists(configuracioJoc->nomJocArbreFigures+".tex")){
    //  arxiu.remove(configuracioJoc->nomJocArbreFigures+".tex");}
    if (arxiu.open(QIODevice::WriteOnly)){
      QTextStream entrada(&arxiu);
      QString inici="\\scalebox{0.9}{\\includegraphics{img/"+
                     configuracioJoc->nomJocArbreFigures+"/";

      //capçalera
      entrada<<(QString("\\section{"+configuracioJoc->tradueixCadena(configuracioJoc->nomJocArbreFigures)+"}\n"));
     //Figura inicial del tangram
      entrada<<(QString("\\begin{figure}[H]\n"));
      entrada<<(QString("\\centering\n"));
      entrada<<(QString("\\scalebox{1.0}{\\includegraphics{figures_tex/img/tan4_I.png}}\n"));
      entrada<<(QString("\\end{figure}\n"));
     entrada<<(QString("\\newpage\n"));
     entrada<<(QString("\\subsection{Figures}\n"));
     entrada<<(QString("\\begin{figure}[H]\n"));
     entrada<<(QString("\\centering\n"));

int figuresPerPagina=60;
//Primer les figures....
    for(int i=0 ;i<=arbreFiguresTreeWidget->topLevelItem(0)->childCount()-1;i++){
        QString nomArxiu=QString::number(i);
        while (nomArxiu.length()<4){nomArxiu="0"+nomArxiu;}
        entrada<<(QString("%1%2.png}}\n").arg(inici).arg(nomArxiu));
        entrada<<(QString("\\qquad\n"));
        if ( ((i % figuresPerPagina)==0) && (i>0)){
            entrada<<(QString("\\end{figure}\n"));
            entrada<<(QString("\\newpage\n"));
            entrada<<(QString("\\begin{figure}[H]\n"));
            entrada<<(QString("\\centering\n"));
        }
    }
    entrada<<(QString("\\end{figure}\n"));
    entrada<<(QString("\\newpage\n"));

    //...i ara les solucions
     //capçalera
     entrada<<(QString("\\subsection{Solucions}\n"));
     entrada<<(QString("\\begin{figure}[H]\n"));
     entrada<<(QString("\\centering\n"));

    for(int i=0 ;i<=arbreFiguresTreeWidget->topLevelItem(0)->childCount()-1;i++){
        QString nomArxiu=QString::number(i);
        while (nomArxiu.length()<4){nomArxiu="0"+nomArxiu;}
        entrada<<(QString("%1%2_sol.png}}\n").arg(inici).arg(nomArxiu));
        entrada<<(QString("\\qquad\n"));
        if ( ((i % figuresPerPagina)==0) && (i>0)){
            entrada<<(QString("\\end{figure}\n"));
            entrada<<(QString("\\newpage\n"));
            entrada<<(QString("\\begin{figure}[H]\n"));
            entrada<<(QString("\\centering\n"));
        }
    }
    entrada<<(QString("\\end{figure}\n"));
    entrada<<(QString("\\newpage\n"));
   arxiu.close();
}
  QApplication::setOverrideCursor(Qt::ArrowCursor);
}


