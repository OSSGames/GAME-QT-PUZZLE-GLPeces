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
#include <QtGui>
#include <QIcon>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>

#include "conjuntpeces.h"
#include "constants.h"
#include "configuracio.h"
#include "funcions.h"
#include "comprovafigures.h"

ComprovaFigures::ComprovaFigures(Configuracio *configJoc, QWidget *parent)
        : QDialog(parent)
{

    configuracioJoc=configJoc;
    charPathSeparator=QDir().separator();

  //  setMinimumSize (QSize(1100,700));
    setMinimumSize (QSize(1100,750));
  //  setMinimumSize (QSize(1200,785));
    setMaximumSize(QSize(1100,750));


    labelFiguraPrograma= new QLabel("primer",this);
    labelFiguraPrograma->setGeometry(0,0,350,40);
   // labelFiguraPrograma->setGeometry(0,0,300,40);
    labelFiguraPrograma->move(QPoint(0,0));
    labelFiguraPrograma->setVisible(false);

    labelFiguresGenerades= new QLabel("segon",this);
    labelFiguresGenerades->setGeometry(0,0,300,40);
   // labelFiguresGenerades->setGeometry(0,0,350,40);
    labelFiguresGenerades->move(QPoint(850,0));
  //  labelFiguresGenerades->move(QPoint(900,0));
    labelFiguresGenerades->setVisible(false);


    //arbre de figures del programa
        arbreFiguresTreeWidget= new QTreeWidget(this);
        arbreFiguresTreeWidget->setGeometry(0,0,250,700);
       // arbreFiguresTreeWidget->setGeometry(0,0,300,730);
        arbreFiguresTreeWidget->move(QPoint(0,50));
        arbreFiguresTreeWidget->expandAll();
        connect(arbreFiguresTreeWidget,SIGNAL(itemSelectionChanged()),
                this,SLOT(seleccioFiguraJoc()));

   //arbre de figures noves
        arbreNovesFiguresTreeWidget= new QTreeWidget(this);
        arbreNovesFiguresTreeWidget->setGeometry(0,0,250,700);
        arbreNovesFiguresTreeWidget->move(QPoint(850,50));
      //  arbreNovesFiguresTreeWidget->setGeometry(0,0,300,730);
      //  arbreNovesFiguresTreeWidget->move(QPoint(900,50));
        arbreNovesFiguresTreeWidget->expandAll();
        connect(arbreNovesFiguresTreeWidget,SIGNAL(itemSelectionChanged()),
                this,SLOT(seleccioFiguraNova()));        
        connect(arbreNovesFiguresTreeWidget,SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )),
                this,SLOT(seleccionFiguraItemDoubleClicked (QTreeWidgetItem * )));

   //frame dels botons
        botonsFrame=new QFrame(this);
        botonsFrame->setGeometry(0,0,600,850);
        botonsFrame->move(QPoint(250,0));
       // botonsFrame->move(QPoint(300,0));

        sceneFiguresPrograma=new QGraphicsScene(0, 0, 180, 180);
        viewFiguresPrograma = new QGraphicsView(sceneFiguresPrograma,botonsFrame);
        viewFiguresPrograma->move(QPoint(5,55));
        viewFiguresPrograma->setRenderHints(QPainter::Antialiasing
                                 | QPainter::TextAntialiasing);


        sceneFiguresNoves=new QGraphicsScene(0, 0, 180, 180);
        viewFiguresNoves = new QGraphicsView(sceneFiguresNoves,botonsFrame);
        viewFiguresNoves->move(QPoint(415,53));
        viewFiguresNoves->setRenderHints(QPainter::Antialiasing
                                 | QPainter::TextAntialiasing);

        sceneComparacio=new QGraphicsScene(0, 0, 590, 510);
       // sceneComparacio=new QGraphicsScene(0, 0, 590, 590);
        viewComparacio = new QGraphicsView(sceneComparacio,botonsFrame);
        viewComparacio->move(QPoint(5,235));;
       // viewComparacio->move(QPoint(5,188));
        viewComparacio->setRenderHints(QPainter::Antialiasing
                                 | QPainter::TextAntialiasing);

        //Cal situar els botons entre 190-400
        //i 0-140
        /*01/05/14 S'ha baixat tot 50 unitats per deixar més espai
        al missatge del temps aproximat */
        obreFiguresGenerades = new QPushButton(
                QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"obrecon.png"),"", botonsFrame);
        obreFiguresGenerades->setToolTip(configuracioJoc->tradueixCadena("figures.generades.ToolTip.Carrega"));
        connect(obreFiguresGenerades,SIGNAL(clicked()),this,SLOT(obreFiguresGeneradesSlot()));
        obreFiguresGenerades->setGeometry(0,0,40,40);
        obreFiguresGenerades->move(360,60);

        comparaFiguresPrograma = new QPushButton(
                QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"filefind.png"),"", botonsFrame);
        comparaFiguresPrograma->setToolTip(configuracioJoc->tradueixCadena("compara.figures.programa"));
        connect(comparaFiguresPrograma,SIGNAL(clicked()),this,SLOT(comparaFiguresProgramaSlot()));
        comparaFiguresPrograma->setGeometry(0,0,40,40);
        comparaFiguresPrograma->move(200,60);

        comparaFiguresGenerades = new QPushButton(
                QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"filefind.png"),"", botonsFrame);
        comparaFiguresGenerades->setToolTip(configuracioJoc->tradueixCadena("compara.figures.generades"));
        connect(comparaFiguresGenerades,SIGNAL(clicked()),this,SLOT(comparaFiguresGeneradesSlot()));
        comparaFiguresGenerades->setGeometry(0,0,40,40);
        comparaFiguresGenerades->move(320,60);



        figuraSeguentButton = new QPushButton(
                QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"edit-redo.png"),"", botonsFrame);
        figuraSeguentButton->setToolTip(configuracioJoc->tradueixCadena("seguent.figura"));
        connect(figuraSeguentButton,SIGNAL(clicked()),this,SLOT(figuraSeguentSlot()));
        figuraSeguentButton->setGeometry(0,0,40,40);
        figuraSeguentButton->move(360,147);
        figuraSeguentButton->setEnabled(false);




        figuraAnteriorButton = new QPushButton(
                QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"edit-undo.png"),"", botonsFrame);
        figuraAnteriorButton->setToolTip(configuracioJoc->tradueixCadena("anterior.figura"));
        connect(figuraAnteriorButton,SIGNAL(clicked()),this,SLOT(figuraAnteriorSlot()));
        figuraAnteriorButton->setGeometry(0,0,40,40);
        figuraAnteriorButton->move(320,147);
        figuraAnteriorButton->setEnabled(false);

        desaLlistaComparacio=new QPushButton(
                QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"save.png"),"", botonsFrame);
        desaLlistaComparacio->setToolTip(configuracioJoc->tradueixCadena("desa.canvis.comprovacio"));
        connect(desaLlistaComparacio,SIGNAL(clicked()),this,SLOT(desaLlistaComparacioSlot()));
        desaLlistaComparacio->setGeometry(0,0,40,40);
        desaLlistaComparacio->move(QPoint(270,147));
        desaLlistaComparacio->setEnabled(false);

        figuresSemblantsList=new QStringList();
        indexLlistaFiguraSemblant=0;

        eliminaFigura = new QPushButton(
                QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"cross.png"),"", botonsFrame);
        eliminaFigura->setToolTip(configuracioJoc->tradueixCadena("marca.figura.eliminar"));
        connect(eliminaFigura,SIGNAL(clicked()),this,SLOT(eliminaFiguraSlot()));
        eliminaFigura->setGeometry(0,0,40,40);        
        eliminaFigura->move(320,105);

        canviaNumeracio = new QPushButton(
                QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"noms.png"),"", botonsFrame);
        canviaNumeracio->setToolTip(configuracioJoc->tradueixCadena("renomena.figures.generades"));
        connect(canviaNumeracio,SIGNAL(clicked()),this,SLOT(canviaNumeracioFiguresGeneradesSlot()));
        canviaNumeracio->setGeometry(0,0,40,40);
        canviaNumeracio->move(360,105);


        labelTemps= new QLabel("",botonsFrame);
        labelTemps->setGeometry(0,0,350,40);
        labelTemps->move(QPoint(200,0));
        labelTemps->setVisible(true);

        tanca = new QPushButton(
                QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"surt.png"),"", botonsFrame);
        tanca->setToolTip(configuracioJoc->tradueixCadena("Tanca"));
        connect(tanca,SIGNAL(clicked()),this,SLOT(close()));
        tanca->setGeometry(0,0,40,40);
        tanca->move(360,191);
        tanca->setDefault(true);

        desaCanvis = new QPushButton(
                QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"save.png"),"", botonsFrame);
        desaCanvis->setToolTip(configuracioJoc->tradueixCadena("desa.canvis.figures.generades"));
        connect(desaCanvis,SIGNAL(clicked()),this,SLOT(desaCanvisSlot()));
        desaCanvis->setGeometry(0,0,40,40);
        desaCanvis->move(320,191);


  tradueixGUI();

  //carregam els arbres
    //carregaArbreFigures( arbreFiguresTreeWidget,directoriLinux()+
      //                          charPathSeparator+"figures"+charPathSeparator+configuracioJoc->arxiuDadesFigures);
    carregaArbreFigures( arbreFiguresTreeWidget,configuracioJoc->directoriArxiuDadesFigures);
    //Carregam l'arxiu de dades predeterminat si existeix
    carregaArbreFigures( arbreNovesFiguresTreeWidget,
                         nomArxiuFiguresGeneradesPredeterminat(),false);

    generaConjunts(configuracioJoc->tipusTangram);

    /*Per controlar les dades de generació automàtica de figures*/
    /*Cal activar codi a figuraSeguentSlot(), figuraAnteriorSlot() i
    activaBotonsMovimentLlistaComparacioFigures() */
/*
    indexLlistaDadesPeces=0;
    dadesGeneracioFigures_carregaDades();
    dadesGeneracioFigures_colocaPeces();*/
}


//Carrega les figures als arbres
void ComprovaFigures::carregaArbreFigures(QTreeWidget *arbre, QString nomArxiu,
                                          bool traduc){

QApplication::setOverrideCursor(Qt::WaitCursor);

//netejam
arbre->clear();
arbre->setColumnCount(7);
arbre->setColumnHidden(3,true);
arbre->setColumnHidden(4,true);
//columna per marcar les figures a eliminar
arbre->setColumnHidden(5,true);
//Longitud de la silueta de la figura
arbre->setColumnHidden(6,true);
//Nombre de vèrtex de la silueta de la figura
arbre->setColumnHidden(7,true);
//posam la capçalera de l'arbre
QTreeWidgetItem *item=new QTreeWidgetItem();
// 13/02/11 Canviat per eliminar icones de resources.qrc
item->setIcon(0,QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+configuracioJoc->nomIcone));
item->setText(0,configuracioJoc->tradueixCadena(configuracioJoc->nomJocArbreFigures));
arbre->setHeaderItem(item);

llegeixArxiuFigures(arbre,nomArxiu,traduc);

QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void ComprovaFigures::llegeixArxiuFigures(QTreeWidget *arbre, QString nomArxiu, bool traduc){
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

     int numFig, intPosDarrerPunt;
     while (!linia0.isNull()) {
         linia1=entrada.readLine();
         linia2=entrada.readLine();


         //Només es carreguen les figures del nivell
         //principiant (fins que no s'aconsegueixi generar
         //figures d'altre nivells
         //if (  linia2.mid(0,1).toInt()<=NIVELL_PRINCIPIANT ) {
            QTreeWidgetItem *item=new QTreeWidgetItem();
            intPosDarrerPunt=linia0.lastIndexOf(".");
            if (intPosDarrerPunt<=0){intPosDarrerPunt=linia0.length();}
            numFig=controlArxiu->value(linia0.mid(0,intPosDarrerPunt),"1").toInt();
          QString cadena;
          //Només cal fer la traducció dels noms de figura, quan es carrega
          //un arxiu de figures generadas no es fa la traducció
          //per permetre localitzar-les millor a l'arxiu
          if(traduc){
            //posam espais al principi per a què les cadenes tenguin
            //la mateixa longitud i quedin ben ordenades
            cadena=QString::number(numFig);
            while (cadena.length()<4){cadena=" "+cadena;}
            cadena=configuracioJoc->tradueixCadena(linia0.mid(0,intPosDarrerPunt))+" "+cadena;
             }//if de la traducció
          else {cadena=linia0;}
            //Nom de la figura
            item->setText(0,cadena);
            numFig++;
            controlArxiu->setValue(linia0.mid(0,intPosDarrerPunt),numFig);
            //Posició de les peces
            item->setText(1,linia1);
            //Nivell i dificultat
            item->setText(2,linia2);
            //controla si  la figura ja s'ha resolt
            item->setText(3,"0");
            item->setIcon(0,iconeFigura(linia2.mid(2,1).toInt()));
            //controla si cal eliminar la figura
            item->setText(4,"0");
            //Longitud i nombre de vèrtex a zero
            item->setText(5,"0");
            item->setText(6,"0");

           arrel->addChild(item);
         //}//if del nivell de les figures
         linia0 = entrada.readLine();
     }
controlArxiu->clear();
 }//Final del If de comprovació del nom de l'arxiu
arbre->addTopLevelItem(arrel);
arbre->sortItems(0,Qt::AscendingOrder);
//arbreFiguresTreeWidget->sortByColumn(0,Qt::AscendingOrder);
QString capcaleraArbre;
/*if (nomArxiu.contains(".fig")){
   capcaleraArbre=configuracioJoc->tradueixCadena("figures.programa");
}
else capcaleraArbre=configuracioJoc->tradueixCadena("figures.noves");*/
if (traduc){
   capcaleraArbre=configuracioJoc->tradueixCadena("figures.programa");
}
else capcaleraArbre=configuracioJoc->tradueixCadena("figures.noves");
arrel->setText(0,capcaleraArbre+" ("+QString::number(arrel->childCount()) +")");
arbre->expandAll();

}

/*
  Proces de comparació de les figures generades
  amb les figures del programa
  */
void ComprovaFigures::comparaFiguresProgramaSlot(){
    int totalFigures=arbreFiguresTreeWidget->topLevelItem(0)->childCount()*
                     arbreNovesFiguresTreeWidget->topLevelItem(0)->childCount();

    progresCercaFigura=new QProgressDialog(
              configuracioJoc->tradueixCadena("Cercant.figura")+"...",
              configuracioJoc->tradueixCadena("Atura.cerca"), 0,totalFigures,this,0);
    progresCercaFigura->setWindowModality(Qt::WindowModal);
    progresCercaFigura->setVisible(true);

figuresSemblantsList->clear();
activaLabelsFiguresSemblants();
activaConjuntsFigures();
tempsComparacioFigures.start();
   for(int i=0;i<=arbreNovesFiguresTreeWidget->topLevelItem(0)->childCount()-1;i++){
       conjuntComparacioOriginal->colocaPeces(
               arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->text(1));
       conjuntComparacioOriginal->conjuntVisible(true);
       /* Si no hi ha dades de la longitud i nombre de vèrtex de la figura
        * es posen*/
       if(arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->text(5)=="0"){
          arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->setText(5,
                                                 QString("%1").arg(conjuntComparacioOriginal->longitudSilueta()));
          arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->setText(6,
                                                 QString("%1").arg(nombreVertexQPainterPath(0)));
       }
       cercaFigura(i,0,arbreFiguresTreeWidget,0);
       if (progresCercaFigura->wasCanceled()){
           break;}
   }

progresCercaFigura->close();
activaBotonsMovimentLlistaComparacioFigures();
if (figuresSemblantsList->size()>0){
    desaDadesCercaFigures();}
}

/*
  Proces de comparació de les figures generades
  entre elles.
  */
void ComprovaFigures::comparaFiguresGeneradesSlot(){
   // generaFiguresMassiuAtzar_ComprovaCompactacio();
   // return;

    long int totalFigures=sumatori(arbreNovesFiguresTreeWidget->topLevelItem(0)->childCount());    
    //missatgeEstimacioTemps(totalFigures);
    progresCercaFigura=new QProgressDialog(
              configuracioJoc->tradueixCadena("Cercant.figura")+"...",
              configuracioJoc->tradueixCadena("Atura.cerca"), 0,totalFigures,this,0);
    progresCercaFigura->setWindowModality(Qt::WindowModal);
    progresCercaFigura->setVisible(true);

    figuresSemblantsList->clear();
    activaLabelsFiguresSemblants();
    activaConjuntsFigures();

arbreNovesFiguresTreeWidget->selectedItems().clear();
arbreFiguresTreeWidget->selectedItems().clear();

    tempsComparacioFigures.start();
    for(int i=0;i<=arbreNovesFiguresTreeWidget->topLevelItem(0)->childCount()-1;i++){
        conjuntComparacioOriginal->colocaPeces(
                arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->text(1));
        conjuntComparacioOriginal->conjuntVisible(true);
        /* Si no hi ha dades de la longitud i nombre de vèrtex de la figura
         * es posen*/
        if(arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->text(5)=="0"){
           arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->setText(5,
                                                 QString("%1").arg(conjuntComparacioOriginal->longitudSilueta()));
           arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->setText(6,
                                                 QString("%1").arg(nombreVertexQPainterPath(0)));
         /*  arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->setText(0,
                                                 QString("%1 %2").arg(conjuntComparacioOriginal->longitudSilueta())
                                                                  .arg(nombreVertexQPainterPath(0)));*/
        /* qCritical("%s",qPrintable(QString("%1 %2 %3")
                                   .arg(arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->text(0))
                                   .arg(conjuntComparacioOriginal->longitudSilueta())
                                   .arg(nombreVertexQPainterPath(0))));*/
        }

        cercaFigura(i,1,arbreNovesFiguresTreeWidget,i+1);

        if (progresCercaFigura->wasCanceled()){
            break;}
    }

progresCercaFigura->close();
activaBotonsMovimentLlistaComparacioFigures();
if (figuresSemblantsList->size()>0){
    desaDadesCercaFigures();}
}


void ComprovaFigures::activaBotonsMovimentLlistaComparacioFigures(){
    if (figuresSemblantsList->size()>0){        
        indexLlistaFiguraSemblant=0;
        figuresSemblantsList->sort();
        figuraSeguentButton->setEnabled(true);
        figuraAnteriorButton->setEnabled(true);
        desaLlistaComparacio->setEnabled(true);
        mostraFiguraSemblant(0);
    }
/*
    if(llistaDadesPeces.size()>0){
        indexLlistaDadesPeces=0;
        figuraSeguentButton->setEnabled(true);
        figuraAnteriorButton->setEnabled(true);
    }*/
    //això per eliminar les peces de l'scene
    //i mostrar un missatge
    else {
        figuraSeguentButton->setEnabled(false);
        figuraAnteriorButton->setEnabled(false);
        desaLlistaComparacio->setEnabled(false);
        mostraFiguraSemblant(-1);}
}

/*
  Proces de recerca de figures
  arbreComparacio=0 figures del programa
  arbreComparacio=1 figures generades

  26/09/15 Afegit el procediment comparaQPainterPath()
  que millora la recerca (a rotacions de les figures).
  */
void ComprovaFigures::cercaFigura(int indexOriginal,int arbreComparacio,
                                  QTreeWidget *arbreCerca, int itemInici){
    this->setFocus(Qt::OtherFocusReason);
    if (arbreCerca->topLevelItem(0)==0){ return;}

   //Figura que es busca (de les figures generades)
       conjuntComparacioOriginal->centraFiguraScene();
       conjuntComparacioOriginal->colorPecesConjunt(Qt::green);
       //Figura amb la qual es compara
       //(pot ésser del programa o de les generades)
       conjuntComparacioComparada->conjuntDeMostra();
//int contador=0;
       for(int i=itemInici;i<=arbreCerca->topLevelItem(0)->childCount()-1;i++){
         progresCercaFigura->setValue(progresCercaFigura->value()+1);
         progresCercaFigura->setLabelText(configuracioJoc->tradueixCadena("Cercant.figura")+
                                     QString(".. %1/%2").arg(progresCercaFigura->value()+1)
                                    .arg(progresCercaFigura->maximum()));

        if(arbreCerca->topLevelItem(0)->child(i)->text(5)=="0"){
            colocaFiguraComparada(arbreCerca->topLevelItem(0)->child(i)->text(1));
            arbreCerca->topLevelItem(0)->child(i)->setText(5,
                                        QString("%1").arg(conjuntComparacioComparada->longitudSilueta()));
            arbreCerca->topLevelItem(0)->child(i)->setText(6,
                                        QString("%1").arg(nombreVertexQPainterPath(1)));
         }
         //Comprovam si es pot fer una comparació inmediata de longituds
          // i vèrtex de les figures
         int diferenciaVertex=0;
         double diferenciaLongitudSiluetes=0.0;
         if(arbreCerca->topLevelItem(0)->child(i)->text(5) != "0"){
           if (arbreComparacio==0){
             diferenciaVertex=abs(arbreCerca->topLevelItem(0)->child(i)->text(6).toInt()-
                             arbreNovesFiguresTreeWidget->topLevelItem(0)->child(indexOriginal)->text(6).toInt());
             diferenciaLongitudSiluetes=comparacioLongitudSiluetesPercentatge(
                         arbreNovesFiguresTreeWidget->topLevelItem(0)->child(indexOriginal)->text(5).toFloat(),
                         arbreCerca->topLevelItem(0)->child(i)->text(5).toFloat());
           }
           else{
             diferenciaVertex=abs(arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->text(6).toInt()-
                             arbreNovesFiguresTreeWidget->topLevelItem(0)->child(indexOriginal)->text(6).toInt());
             diferenciaLongitudSiluetes=comparacioLongitudSiluetesPercentatge(
                         arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->text(5).toFloat(),
                         arbreNovesFiguresTreeWidget->topLevelItem(0)->child(indexOriginal)->text(5).toFloat());
           }
          }
         /* Si les longituds i el nombre de vèrtex de les dues figures coincedeixen
          * es comprova si són iguals*/
         /*bool continuar=true;
         if(( diferenciaVertex<0  || diferenciaLongitudSiluetes<0) && continuar){
            if (comparacioSiluetes()){
            colocaFiguraComparada(arbreCerca->topLevelItem(0)->child(i)->text(1));
            afegeixFiguraSemblant(indexOriginal,i,arbreComparacio,100.5);
            continuar=false;
            }
           }*/
         //if (continuar){
             if(diferenciaVertex<3 && diferenciaLongitudSiluetes<1.0
                     //|| !comparaRectanglesFigures() )
                     ){
             colocaFiguraComparada(arbreCerca->topLevelItem(0)->child(i)->text(1));
             double comparacio=comparaQPainterPath(conjuntComparacioOriginal->siluetaFiguraEscala(1.005),
                                      conjuntComparacioComparada->siluetaFiguraEscala(1.005),
                                      QRectF(conjuntComparacioOriginal->rectangleFigura()).center(),
                                      QRectF(conjuntComparacioComparada->rectangleFigura()).center());
//qDebug("comparacio %f",comparacio);
            if(comparacio>PERCENTATGE_COINCIDENCIA_BUSCAR_FIGURA){                
                afegeixFiguraSemblant(indexOriginal,i,arbreComparacio,comparacio);
                }
              }
            /*else if (diferenciaVertex<10 || diferenciaLongitudSiluetes<3.0) {
               colocaFiguraComparada(arbreCerca->topLevelItem(0)->child(i)->text(1));
               cercaFiguraAntic(indexOriginal,i,arbreComparacio);
                 }*/
            //}

     if (progresCercaFigura->wasCanceled()){
         activaBotonsMovimentLlistaComparacioFigures();
         break;}

     //Actualitzam la previsió del temps només cada 10000 comprovacions
     QString maximString=QString("%1").arg(progresCercaFigura->maximum());
     if( (progresCercaFigura->value() % int(pow(10,maximString.length()/2)) ==0 ) &&
         (progresCercaFigura->value()>1) &&
         (progresCercaFigura->value()*100/progresCercaFigura->maximum()>5 ||
          progresCercaFigura->value()>500000)
             ){
         missatgeEstimacioTemps(progresCercaFigura->value()+1,progresCercaFigura->maximum(),
                                tempsComparacioFigures.elapsed());

      }
  }//for
   sceneComparacio->update();
   qApp->processEvents();
}

/* Compara els rectangles de les figures
 * per cercar semblances
 */
bool ComprovaFigures::comparaRectanglesFigures(){
/*qDebug("diferencia altura %d",abs(conjuntComparacioComparada->rectangleFigura().height()-
                                  conjuntComparacioOriginal->rectangleFigura().height()));

qDebug("diferencia amplada %d",abs(conjuntComparacioComparada->rectangleFigura().width()-
                                   conjuntComparacioOriginal->rectangleFigura().width()));*/
  return (abs(conjuntComparacioComparada->rectangleFigura().height()-
              conjuntComparacioOriginal->rectangleFigura().height())<0.5
          && abs(conjuntComparacioComparada->rectangleFigura().width()-
                 conjuntComparacioOriginal->rectangleFigura().width())<0.5);
}

/* Antic procediment. Ara es fa servir en els casos que la longitud de les dues
 * siluetes sigui semblant i el nombre de vèrtex no sigui igual però la diferència
 * sigui petita
 */
double ComprovaFigures::cercaFiguraAntic(int indexOriginal,int i,int arbreComparacio){
   //qDebug("indexOriginal %d i %d",indexOriginal,i);

   double resultat=0.0;

   sceneComparacio->update();
   qApp->processEvents();
    if (comparaRectanglesFigures()){
       resultat=figuraSolucionada();
       //qDebug("resultat 1 %f",resultat);
        if ( resultat>=PERCENTATGE_COINCIDENCIA_BUSCAR_FIGURA){
            afegeixFiguraSemblant(indexOriginal,i,arbreComparacio,resultat);
         }
       }
    else {
        conjuntComparacioComparada->simetriaHoritzontal();
        //això per evitar que quedin "rastres"
        sceneComparacio->update();
        qApp->processEvents();
        if (comparaRectanglesFigures()){
        resultat=figuraSolucionada();
        //qDebug("resultat 2 %f",resultat);
        if ( resultat>=PERCENTATGE_COINCIDENCIA_BUSCAR_FIGURA){
            afegeixFiguraSemblant(indexOriginal,i,arbreComparacio,resultat);
            }
        }
        else {
                conjuntComparacioComparada->simetriaVertical();
                //això per evitar que quedin "rastres"
                sceneComparacio->update();
                qApp->processEvents();
                if (comparaRectanglesFigures()){
                resultat=figuraSolucionada();
                //qDebug("resultat 3 %f",resultat);
                    if (resultat>=PERCENTATGE_COINCIDENCIA_BUSCAR_FIGURA){
                    afegeixFiguraSemblant(indexOriginal,i,arbreComparacio,resultat);
                   }
                }
                else {
                    conjuntComparacioComparada->simetriaHoritzontal();
                    //això per evitar que quedin "rastres"
                    sceneComparacio->update();
                    qApp->processEvents();
                    if (comparaRectanglesFigures()){
                    resultat=figuraSolucionada();
                    //qDebug("resultat 4 %f",resultat);
                    if (resultat>=PERCENTATGE_COINCIDENCIA_BUSCAR_FIGURA){
                       afegeixFiguraSemblant(indexOriginal,i,arbreComparacio,resultat);
                     }
                    }
                // qDebug("finals");
                }
             }
      }
 qDebug("resultat %f", resultat);
 return resultat;
}

float ComprovaFigures::figuraSolucionada(){
   //return 0.0;
   //Fem la comprovació dels centres de les peces
    if (
      (!comprovaCentresFigures(conjuntComparacioOriginal,conjuntComparacioComparada)) ||
      (!comprovaCentresFigures(conjuntComparacioComparada,conjuntComparacioOriginal)) ){
        return 0.0;
      }

    if(comparacioSiluetes()){
        return 100.0;
    }


    int positiu,totals;
    positiu=0;
    totals=0;
    QVector<QPointF> pSolucio;

for(int i=0;i<=conjuntComparacioOriginal->arrayPeces.size()-1;i++){
   conjuntComparacioComparada->arrayPeces[i]->puntsSolucio(pSolucio,0.9);
   }

//ara fem la comprovació
totals=pSolucio.size();
if (totals<1){return 0.0;}
//bool bpositiu=false;
for(int j=0;j<=pSolucio.size()-1;j++){
    //bpositiu=false;
    for(int k=0;k<=conjuntComparacioOriginal->arrayPeces.size()-1;k++){
        //if(conjuntComparacioOriginal->arrayPeces[k]->shapeEscala(1.01).contains(
        if(conjuntComparacioOriginal->arrayPeces[k]->shape().contains(
                pSolucio.value(j)-conjuntComparacioOriginal->arrayPeces[k]->pos())){
            positiu=positiu+1;
            //aquest break evita que un mateix punt es comptabilitzi
            //més d'una vegada si hi ha peces superposades
            break;
        }
    }
}

bool interseccioSiluetes=siluetesIntersecten();
float coincidenciaPercentual=((float)((float)(positiu*100)/(float)totals));
float comparacioLongitud=comparacioLongitudSiluetesPercentatge(
        conjuntComparacioComparada->longitudSilueta(),
        conjuntComparacioOriginal->longitudSilueta());

if( ( coincidenciaPercentual >(float)(99.9)) &&
    (comparacioLongitud<=(float)(0.5)) &&
    (interseccioSiluetes) ){
    return 99.0;
    }


if ( (coincidenciaPercentual >(float)(97.9)) &&
      interseccioSiluetes ){
    return 99.5;
}

if( (coincidenciaPercentual >(float)(95.0)) &&
    (comparacioLongitud < (float)(8.0)) &&
    (interseccioSiluetes) ){
    return 97.0;
    }

return coincidenciaPercentual;
}

/*
  Gestió de la llista de figures semblants actual
  */

//Afegeix a la llista de figures semblants (creació de figures)
//les dades d'una figura
void ComprovaFigures::afegeixFiguraSemblant(int indexOriginal,int indexComparada,
                                            int arbreComparacio, float percentatge){

int figuraSemblant;
if(arbreComparacio==0){
  figuraSemblant=indexOriginal;
}
else figuraSemblant=indexComparada;

    QString Missatge;
    //figura amb la qual es compara
    //(que pot ésser del programa o be de les generades
    Missatge.append(QString("%1;").arg(indexOriginal));
    //figura a comparar (que sempre serà de les generades)
    Missatge.append(QString("%1;").arg(indexComparada));
    Missatge.append(QString("%1;").arg(arbreComparacio));
    Missatge.append(QString("%1;").arg(percentatge));
    //05/12/15 Aquest if és incomprensible!
    //if(percentatge != (float)(99.45)){
   //  Missatge.append(QString("%1;").arg(percentatge));
     //}
    //else Missatge.append("--");
    //if(figuresSemblantsList->indexOf(Missatge)<0){
    //figuresSemblantsList->append(Missatge);}
    labelFiguresGenerades->setVisible(true);
    labelFiguresGenerades->setText(
            configuracioJoc->tradueixCadena("semblances.trobades")
            .arg(figuresSemblantsList->count()));
    //Si la semblança és alta, marcam l'item per eliminar
    //El percentatge no és "igual" per a tots els jocs
    //Alguns, amb el 98% ja són idèntiques
    /*26/04/14 Aqui hi havia un error: enlloc de indexOriginal
     * posava indexComparada
     */
    /* 09/12/15
     * S'ha canviat la figura que es marca com a semblant: ara es marca
     * la que és comparada
     */
    if ( (percentatge>=98.0) &&
        (arbreNovesFiguresTreeWidget->topLevelItem(0)->child(figuraSemblant)->text(4) !="2") ){
        arbreNovesFiguresTreeWidget->topLevelItem(0)->child(figuraSemblant)->setText(4,"1");
        arbreNovesFiguresTreeWidget->topLevelItem(0)->child(figuraSemblant)->setTextColor(0,Qt::blue);
        if(figuresSemblantsList->indexOf(Missatge)<0){
        figuresSemblantsList->append(Missatge);}
    }
    if(percentatge>=99.0 ){
      arbreNovesFiguresTreeWidget->topLevelItem(0)->child(figuraSemblant)->setText(4,"2");
      arbreNovesFiguresTreeWidget->topLevelItem(0)->child(figuraSemblant)->setTextColor(0,Qt::red);
      if(figuresSemblantsList->indexOf(Missatge)<0){
      figuresSemblantsList->append(Missatge);}
    }

    labelFiguresGenerades->setVisible(true);
    labelFiguresGenerades->setText(
            configuracioJoc->tradueixCadena("semblances.trobades")
            .arg(figuresSemblantsList->count()));
}


/*
  Compara les siluetes dels conjunts per saber
  si poden ésser semblants
  */
bool ComprovaFigures::comparacioLongitudSiluetes(){
 /*qCritical("Comparacio siluetes: %s",
         qPrintable(QString("%1").arg(conjuntComparacioOriginal->longitudSilueta()-
                                      conjuntComparacioComparada->longitudSilueta())));*/
 //  return true;
   return ( fabs(
          conjuntComparacioOriginal->longitudSilueta()-
          conjuntComparacioComparada->longitudSilueta()) ) < 300;
}


bool ComprovaFigures::comparacioSiluetes(){
 return conjuntComparacioOriginal->siluetaFigura()==
           conjuntComparacioComparada->siluetaFigura();
}

bool ComprovaFigures::siluetesIntersecten(){
 return conjuntComparacioOriginal->siluetaFigura()
       .intersects(conjuntComparacioComparada->siluetaFigura());
 /*return conjuntMostraPrograma->siluetaFigura()==
           conjuntMostraNovesFigures->siluetaFigura();*/
}





/* Retorna el nombre dels vèrtex d'una figura formada per un
 * conjunt de peces. O pel conjunt original i 1 pel conjunt comparada
 */
int ComprovaFigures::nombreVertexQPainterPath(int conjunt){
    QPainterPath conjuntPainterPath;
    QTransform transformOrig;
    transformOrig.rotate(0,Qt::XAxis);
    QList<QPointF> llistaPoligon;
        if(conjunt==0){
              conjuntPainterPath=conjuntComparacioOriginal->siluetaFiguraEscala(1.005);

              llistaPoligon=vertexQPainterPath(conjuntPainterPath.toFillPolygon(transformOrig).toList(),
                                               QPointF(conjuntComparacioOriginal->rectangleFigura().center()));
          }
        else  {
            conjuntPainterPath=conjuntComparacioComparada->siluetaFiguraEscala(1.005);
            llistaPoligon=vertexQPainterPath(conjuntPainterPath.toFillPolygon(transformOrig).toList(),
                                             QPointF(conjuntComparacioComparada->rectangleFigura().center()));
        }
    return llistaPoligon.count();
}


/* Calcula l'angle format per tres punts, per tal de detectar canvis de direcció a una
 * silueta de figura.
 * 11/11/15
 * No es fa servir *  

bool ComprovaFigures::angleEntrePunts(QPointF PoligonOriginal_P1, QPointF PoligonOriginal_P2, QPointF PoligonOriginal_P3,
                                      QPointF PoligonComparada_P1, QPointF PoligonComparada_P2, QPointF PoligonComparada_P3){

 QPointF vectorOriginal1,vectorOriginal2, vectorComparada1,vectorComparada2;
 vectorOriginal1=PoligonOriginal_P2-PoligonOriginal_P1;// (PoligonOriginal_P2.x()-PoligonOriginal_P1.x(),PoligonOriginal_P2.y()-PoligonOriginal_P1.y());
 vectorOriginal2=PoligonOriginal_P3-PoligonOriginal_P2;//(PoligonOriginal_P3.x()-PoligonOriginal_P2.x(),PoligonOriginal_P3.y()-PoligonOriginal_P2.y());

 vectorComparada1=PoligonComparada_P2-PoligonComparada_P1;//(PoligonComparada_P2.x()-PoligonComparada_P1.x(),PoligonComparada_P2.y()-PoligonComparada_P1.y());
 vectorComparada2=PoligonComparada_P3-PoligonComparada_P2;//(PoligonComparada_P3.x()-PoligonComparada_P2.x(),PoligonComparada_P3.y()-PoligonComparada_P2.y());
 double cosinusOriginal, cosinusComparada;
 cosinusOriginal=(vectorOriginal1.x()*vectorOriginal2.x()+vectorOriginal1.y()*vectorOriginal2.y())/
                  (sqrt(pow(vectorOriginal1.x(),2)+pow(vectorOriginal1.y(),2))*
                   sqrt(pow(vectorOriginal2.x(),2)+pow(vectorOriginal2.y(),2)));

 cosinusComparada=(vectorComparada1.x()*vectorComparada2.x()+vectorComparada1.y()*vectorComparada2.y())/
         (sqrt(pow(vectorComparada1.x(),2)+pow(vectorComparada1.y(),2))*
          sqrt(pow(vectorComparada2.x(),2)+pow(vectorComparada2.y(),2)));
 return (cosinusOriginal != cosinusComparada);
}*/

/*11/11/15 No es fa servir
QList<QPointF> ComprovaFigures::inverteixLlistaPunts(QList<QPointF> llista){
 QList<QPointF> llistaInvertida;

 for(int p=0;p<llista.count();p++){
    llistaInvertida.append(llista.value(llista.count()-p-1));
 }
 return llistaInvertida;
}*/

/* Procés comú en la comparació de figures
 */
void ComprovaFigures::colocaFiguraComparada(QString dades){
    conjuntComparacioComparada->colocaPeces(dades);
    conjuntComparacioComparada->conjuntVisible(true);
    //això per evitar que quedin "rastres"
    sceneComparacio->update();
    qApp->processEvents();
}

void ComprovaFigures::figuraSeguentSlot(){
  if (indexLlistaFiguraSemblant<figuresSemblantsList->size()-1){
      indexLlistaFiguraSemblant=indexLlistaFiguraSemblant+1;}
      mostraFiguraSemblant(indexLlistaFiguraSemblant);
      //això per evitar que quedin "rastres"
      sceneComparacio->update();
  //}
/*
  if(indexLlistaDadesPeces<(llistaDadesPeces.size()-1)/3){
  indexLlistaDadesPeces=indexLlistaDadesPeces+1;
  dadesGeneracioFigures_colocaPeces();
   }*/
}

void ComprovaFigures::figuraAnteriorSlot(){
  if (indexLlistaFiguraSemblant>0){
      indexLlistaFiguraSemblant=indexLlistaFiguraSemblant-1;
  }
      mostraFiguraSemblant(indexLlistaFiguraSemblant);
      //això per evitar que quedin "rastres"
      sceneComparacio->update();
   //}
/*
  if(indexLlistaDadesPeces>0){
       indexLlistaDadesPeces=indexLlistaDadesPeces-1;
       dadesGeneracioFigures_colocaPeces();
   }*/
}

/*
  Marca la figura generada seleccionada per eliminar-la
  */
void ComprovaFigures::eliminaFiguraSlot(){ 
    if(arbreNovesFiguresTreeWidget->selectedItems().count()>0){
        QTreeWidgetItem *item;
        item=arbreNovesFiguresTreeWidget->selectedItems().first();
        if(item->childCount()==0){
            if(item->text(4)=="0"){
                item->setText(4,"1");
                item->setTextColor(0,Qt::red);
            }
            else{
                item->setText(4,"0");
                item->setTextColor(0,Qt::black);
            }
        }
    }

}

/*
  Desa els canvis realitzats en els figures generades
  */
void ComprovaFigures::desaCanvisSlot(){
    QString nomArxiuComplet = QFileDialog::getSaveFileName(this,
                       configuracioJoc->tradueixCadena("desa.canvis.figures.generades"),
                       directoriFiguresGeneradesAtzar(),
                       filtreArxiusFiguresGeneradesAtzar(configuracioJoc,1));
/* 20/02/12
   Això és necessari degut a què a Windows, nomArxiuComplet te / enlloc
   de \ i no funciona la resta de codi
  */
  nomArxiuComplet.replace(QString("/"),QString("%1").arg(QDir().separator())) ;

    if (!nomArxiuComplet.isEmpty()){
        QString nomArxiu=nomArxiuComplet.mid(
                nomArxiuComplet.lastIndexOf(QDir().separator())+1,
                nomArxiuComplet.length()-
                nomArxiuComplet.lastIndexOf(QDir().separator())+1);

        QString directoriComplet=nomArxiuComplet.mid(
                0,nomArxiuComplet.lastIndexOf(QDir().separator())+1);


       //Si el nom de l'arxiu NO va precedit pel nom del tipus de tangram
       //l'afagim automàticament
       QString prefixeNom=configuracioJoc->arxiuDadesFigures.mid(0,
                          configuracioJoc->arxiuDadesFigures.length()-4);//+"-";
       //25/12/11 Eliminat el "_" per permetre posar el nom sense res més
       /*25/04/14 el prefixe va seguit de - en tots els cassos
        * degut al canvi en el sistema de filtre dels noms d'arxius*/
       if(nomArxiu.left(prefixeNom.length()) != prefixeNom){
          nomArxiu= configuracioJoc->arxiuDadesFigures.mid(0,
                           configuracioJoc->arxiuDadesFigures.length()-4)+"-"+nomArxiu;
          }
       //I ara l'extensió
       if(nomArxiu.right(4) != ".fig"){
           nomArxiu=nomArxiu+".fig";
       }
          nomArxiu=directoriComplet+nomArxiu;

          QFile arxiu(nomArxiu);
          if (!arxiu.open(QIODevice::WriteOnly)){
              QMessageBox::information(this, configuracioJoc->tradueixCadena("desa.canvis.figures.generades"),
                                     configuracioJoc->tradueixCadena("concurs.error")+"\n"+
                                     configuracioJoc->tradueixCadena("arxiuNoCreat").arg(arxiu.fileName()) );
             }
          else {
             QTextStream entrada(&arxiu);
             for(int j=0;j<=arbreNovesFiguresTreeWidget->topLevelItem(0)->childCount()-1;j++){
                QTreeWidgetItem *item;
                item=arbreNovesFiguresTreeWidget->topLevelItem(0)->child(j);
                if(item->text(4)=="0"){
                entrada<<(QString("%1\n").arg(item->text(0)));
                entrada<<(QString("%1\n").arg(item->text(1)));
                entrada<<(QString("%1\n").arg(item->text(2)));
                }
             }
          arxiu.close();
          }
        }
}

/*
  Canvia la numeració de les figures generades per tal de fer-la
  "uniforme"
  */
void ComprovaFigures::canviaNumeracioFiguresGeneradesSlot(){
   QString total=QString("%1").arg(arbreNovesFiguresTreeWidget->topLevelItem(0)->childCount()-1);
   QString darrerNomFigura;
   int contadorFigures=0;
   for(int j=0;j<=arbreNovesFiguresTreeWidget->topLevelItem(0)->childCount()-1;j++){
       if(darrerNomFigura != arbreNovesFiguresTreeWidget->topLevelItem(0)->child(j)->text(0).
               mid(0,arbreNovesFiguresTreeWidget->topLevelItem(0)->child(j)->text(0).lastIndexOf("."))){
         darrerNomFigura=arbreNovesFiguresTreeWidget->topLevelItem(0)->child(j)->text(0).
                 mid(0,arbreNovesFiguresTreeWidget->topLevelItem(0)->child(j)->text(0).lastIndexOf("."));
         contadorFigures=0;
       }
       //QTreeWidgetItem *item;
      // item=arbreNovesFiguresTreeWidget->topLevelItem(0)->child(j);
       //QString codiNumero=QString("%1").arg(j);
       QString codiNumero=QString("%1").arg(contadorFigures);
       while (codiNumero.length()<total.length()){
          codiNumero="0"+codiNumero;
       }
       contadorFigures++;
       arbreNovesFiguresTreeWidget->topLevelItem(0)->child(j)->setText(0,
                           QString("%1.%2").arg(darrerNomFigura).arg(codiNumero));
      // item->text(0)=QString("auto.%1").arg(codiNumero);
      /*20/04/14 mantenim la diferència de nom entre els dos sistemes de
       * generació de figures*/
      /* if(arbreNovesFiguresTreeWidget->topLevelItem(0)->child(j)->text(0).contains("Auto-2")){
           arbreNovesFiguresTreeWidget->topLevelItem(0)->child(j)->setText(0,
                               QString("Auto-2.%1").arg(codiNumero));
       }
       else if (arbreNovesFiguresTreeWidget->topLevelItem(0)->child(j)->text(0).contains("Auto.")){
               arbreNovesFiguresTreeWidget->topLevelItem(0)->child(j)->setText(0,
                           QString("Auto.%1").arg(codiNumero));
       else
       }*/
   }
}

void ComprovaFigures::mostraFiguraSemblant(int index){

    labelFiguraPrograma->setVisible(false);
    labelFiguresGenerades->setVisible(false);
    labelTemps->setVisible(false);
    arbreNovesFiguresTreeWidget->selectedItems().clear();
    arbreFiguresTreeWidget->selectedItems().clear();

if (index>-1 && index<=figuresSemblantsList->size()-1) {

  QStringList list1 = figuresSemblantsList->value(index).split(";");

  QString nomFiguresGenerades;


  //arbreNovesFiguresTreeWidget->topLevelItem(0)->child(list1.value(0).toInt())->setSelected(true);
  nomFiguresGenerades.append(
          arbreNovesFiguresTreeWidget->topLevelItem(0)->child(list1.value(0).toInt())->text(0));

  conjuntMostraPrograma->colocaPeces(
          arbreNovesFiguresTreeWidget->topLevelItem(0)->child(list1.value(0).toInt())->text(1));
  conjuntMostraPrograma->conjuntDeMostra();

  //Col·locam la comparada
  if(list1.value(2)=="0"){
      conjuntComparacioComparada->colocaPeces(
                arbreFiguresTreeWidget->topLevelItem(0)->child(list1.value(1).toInt())->text(1));
      conjuntComparacioComparada->conjuntDeMostra();

      conjuntMostraNovesFigures->colocaPeces(
                  arbreFiguresTreeWidget->topLevelItem(0)->child(list1.value(1).toInt())->text(1));
      conjuntMostraNovesFigures->conjuntDeMostra();

      labelFiguraPrograma->setText(
              arbreFiguresTreeWidget->topLevelItem(0)->child(list1.value(1).toInt())->text(0));
      labelFiguraPrograma->setVisible(true);
  }
  else {
      conjuntComparacioComparada->colocaPeces(
          arbreNovesFiguresTreeWidget->topLevelItem(0)->child(list1.value(1).toInt())->text(1));
      conjuntComparacioComparada->conjuntDeMostra();

      conjuntMostraNovesFigures->colocaPeces(
          arbreNovesFiguresTreeWidget->topLevelItem(0)->child(list1.value(1).toInt())->text(1));
      conjuntMostraNovesFigures->conjuntDeMostra();

      nomFiguresGenerades.append("\n");
      nomFiguresGenerades.append(
              arbreNovesFiguresTreeWidget->topLevelItem(0)->child(list1.value(1).toInt())->text(0));
  }

  nomFiguresGenerades.append(" ( "+list1.value(3)+" %)");
  nomFiguresGenerades.append(QString("  %1/%2").arg(index+1).arg(figuresSemblantsList->count()));
  labelFiguresGenerades->setText(nomFiguresGenerades);
  labelFiguresGenerades->setVisible(true);


  //colocam el conjunt al vertex sup.esq per a què es vegi millor
  //(enlloc de centrat);
  conjuntComparacioComparada->recolocaPeces(QPointF(5,5));
  conjuntComparacioComparada->conjuntDeMostra();
  //conjuntComparacioComparada->colorPecesConjunt(Qt::black);
  conjuntComparacioComparada->conjuntVisible(true);
  //missatgeStatusBar(list1.value(1));

  //Col·locam l'original
  conjuntComparacioOriginal->colocaPeces(
          arbreNovesFiguresTreeWidget->topLevelItem(0)->child(list1.value(0).toInt())->text(1));
 // conjuntComparacioOriginal->recolocaPeces(QPointF(200,200));
  conjuntComparacioOriginal->conjuntDeMostra();
  conjuntComparacioOriginal->conjuntFiguraResolta();
  //Això per evitar que les peces de les dues figures quedin superposades
  conjuntComparacioOriginal->conjuntSuperior();
 }
else{
   conjuntComparacioComparada->conjuntVisible(false);
   conjuntComparacioOriginal->conjuntVisible(false);
   labelFiguresGenerades->setText(
           configuracioJoc->tradueixCadena("semblances.trobades")
           .arg(0));
   labelFiguresGenerades->setVisible(true);
 }

}

/*
Gestiona la selecció d'una figura a l'arbre
(conectat amb arbreFiguresTreeWidget::itemSelectionChanged ()
*/
void ComprovaFigures::seleccioFiguraJoc(){
    QString dades=seleccioFigura(arbreFiguresTreeWidget);
    if (dades.length()>1){
        conjuntMostraPrograma->colocaPeces(dades);
        conjuntMostraPrograma->conjuntVisible(true);
        conjuntMostraPrograma->conjuntDeMostra();
    }
}

void ComprovaFigures::seleccioFiguraNova(){
    QString dades=seleccioFigura(arbreNovesFiguresTreeWidget);
    if (dades.length()>1){
        conjuntMostraNovesFigures->colocaPeces(dades);
        conjuntMostraNovesFigures->conjuntVisible(true);
        //conjuntMostraNovesFigures->conjuntSolucio();
        conjuntMostraNovesFigures->conjuntDeMostra();
    /* 01/06/14 Afegit per millorar la comprovació
     * de figures (veure millor si estan ben aferrades les peces*/
        conjuntComparacioComparada->colocaPeces(dades);
        conjuntComparacioComparada->conjuntVisible(true);
        conjuntComparacioOriginal->conjuntVisible(false);
        conjuntComparacioComparada->conjuntDeMostra();
        conjuntComparacioOriginal->conjuntDeMostra();
        /* Per veure les peces com a solució quan es seleccionen*/
       // conjuntComparacioComparada->conjuntSolucio();
    }
 }


/*marca per eliminar*/
void ComprovaFigures::seleccionFiguraItemDoubleClicked (QTreeWidgetItem * ){
 eliminaFiguraSlot();
}

/*
  Retorna la cadena de les dades
  per composar la figura
  */
QString ComprovaFigures::seleccioFigura(QTreeWidget *arbre){
QString dades;
if (!arbre->selectedItems().isEmpty()){
 QTreeWidgetItem *item;
 item=arbre->selectedItems().first();
 //S'ha clicat el nus arrel
 if (item->childCount()>0){
     dades.append("*");
 }
 else dades.append(item->text(1));
}
return dades;
}

void ComprovaFigures::obreFiguresGeneradesSlot(){
 QString directoriSeleccionat = QFileDialog::getOpenFileName(this,
                                    configuracioJoc->tradueixCadena("figures.generades.ToolTip.Carrega"),
                                    directoriFiguresGeneradesAtzar(),
                                    filtreArxiusFiguresGeneradesAtzar(configuracioJoc,1));
    if(!directoriSeleccionat.isEmpty()){
        carregaArbreFigures(arbreNovesFiguresTreeWidget,directoriSeleccionat,false);        
        figuresSemblantsList->clear();
        activaLabelsFiguresSemblants(false);
        activaConjuntsFigures();
       }
}

/*
  Retorna el nom de l'arxiu predeterminat (si existeix) de les
  figures generades
  */
QString ComprovaFigures::nomArxiuFiguresGeneradesPredeterminat(){
    QDir dirArxiu(directoriFiguresPersonals()+charPathSeparator+"auto");
    QString nomArxiuDades=configuracioJoc->arxiuDadesFigures;

    QFile arxiu(dirArxiu.filePath(nomArxiuDades));
    if(arxiu.exists()){
        return dirArxiu.filePath(nomArxiuDades);
    }
    else return "";
}

void ComprovaFigures::actualitzaTextItemArrel(){
arbreNovesFiguresTreeWidget->topLevelItem(0)->setText(0,
                                  configuracioJoc->tradueixCadena("figures.noves")
                                  +" ("+QString::number(arbreNovesFiguresTreeWidget->topLevelItem(0)->childCount()) +")");

arbreFiguresTreeWidget->topLevelItem(0)->setText(0,
                                configuracioJoc->tradueixCadena("figures.programa")
                                  +" ("+QString::number(arbreFiguresTreeWidget->topLevelItem(0)->childCount()) +")");
}

void ComprovaFigures::generaConjunts(int tipusTangram){
        //eliminam les peces actuals
        sceneFiguresPrograma->clear();
        sceneFiguresNoves->clear();
        sceneComparacio->clear();

        conjuntMostraPrograma=new ConjuntPeces(tipusTangram,ESCALA_MOSTRA);
        conjuntMostraPrograma->conjuntDeMostra();
        conjuntMostraPrograma->conjuntVisible(false);

        conjuntMostraNovesFigures=new ConjuntPeces(tipusTangram,ESCALA_MOSTRA);
        conjuntMostraNovesFigures->conjuntDeMostra();
        conjuntMostraNovesFigures->conjuntVisible(false);

        conjuntComparacioOriginal=new ConjuntPeces(tipusTangram,ESCALA_GENERAL);
        conjuntComparacioOriginal->conjuntDeMostra();
        conjuntComparacioOriginal->conjuntVisible(false);

        conjuntComparacioComparada=new ConjuntPeces(tipusTangram,ESCALA_GENERAL);
        conjuntComparacioComparada->conjuntDeMostra();
        conjuntComparacioComparada->conjuntVisible(false);


       //ara afegim les peces a l'scena
        for(int i=0;i<=conjuntMostraPrograma->arrayPeces.size()-1;i++){
            conjuntMostraPrograma->arrayPeces[i]->setZValue(0);
            sceneFiguresPrograma->addItem(conjuntMostraPrograma->arrayPeces[i]);

            conjuntMostraNovesFigures->arrayPeces[i]->setZValue(0);
            sceneFiguresNoves->addItem(conjuntMostraNovesFigures->arrayPeces[i]);

            conjuntComparacioOriginal->arrayPeces[i]->setZValue(0);
            sceneComparacio->addItem(conjuntComparacioOriginal->arrayPeces[i]);

            conjuntComparacioComparada->arrayPeces[i]->setZValue(0);
            sceneComparacio->addItem(conjuntComparacioComparada->arrayPeces[i]);
            }
}//final CreaConcurs::creaTangram(int tipusTangram)


void ComprovaFigures::tradueixGUI(){
setWindowTitle(configuracioJoc->tradueixCadena("comprova.figures"));
/*
  Si la traducció de totes les cadenes es posa aqui
  el programa no funciona
 */
}

void ComprovaFigures::activaLabelsFiguresSemblants(bool activa){
    labelFiguresGenerades->setVisible(activa);
    labelFiguraPrograma->setVisible(activa);
    activaBotonsMovimentLlistaComparacioFigures();
}

void ComprovaFigures::activaConjuntsFigures(bool activa){
   conjuntMostraPrograma->conjuntVisible(activa);
   conjuntMostraNovesFigures->conjuntVisible(activa);
   conjuntComparacioOriginal->conjuntVisible(activa);
   conjuntComparacioComparada->conjuntVisible(activa);
}

/*
  Mostra un missatge informant del temps estimat per
  fer la comprovació de figures semblants.
  Cada comprovació necessita uns 60 mseg.
  */
void ComprovaFigures::missatgeEstimacioTemps(int figuresFetes,int total, int temps){
 int hores,minuts,dies,segons;
 /*qDebug("%d", temps);
 qDebug("figuresFetes %d", figuresFetes);
 qDebug("total %d", total);*/
 double tempsMitja=temps/figuresFetes;
 if (tempsMitja==0){
    tempsMitja=1;
 }
 //qCritical("tempsMitja %s",qPrintable(QString("%1").arg(tempsMitja)));
 minuts=((total-figuresFetes)*tempsMitja)/(1000*60);
 segons=((total-figuresFetes)*tempsMitja)/(1000);
 //minuts=(figures*temps)/(1000*60);
 hores=minuts/60;
 dies=hores/24;
 segons=segons-minuts*60;
 minuts=minuts-hores*60; 
 hores=hores-dies*24;
 QString tempsFinal;

 if (dies>=1){
   tempsFinal=QString("%1 ").arg(dies)
              +configuracioJoc->tradueixCadena("dies");
   tempsFinal=tempsFinal+QString(" %1 h").arg(hores);
 }
 else if (hores>=1){
   tempsFinal=QString("%1 h %2'").arg(hores).arg(minuts);
 }
 else if (minuts>=1){
     tempsFinal=QString("%1' %2''").arg(minuts).arg(segons);
 }
 else tempsFinal=QString("%1''").arg(segons);

// qDebug("total %d figuresFetes %d temps %d",total,figuresFetes,temps);
/*
tempsFinal=QString("%1 ").arg(dies)
           +configuracioJoc->tradueixCadena("dies");
tempsFinal=tempsFinal+
           QString(" %1 h %2' %3''").arg(hores).arg(minuts).arg(segons);
           */
//qCritical("tempsFinal %s",qPrintable(tempsFinal) );
 tempsFinal=configuracioJoc->tradueixCadena("temps.estimat")+"\n"+tempsFinal;
 labelTemps->setText(tempsFinal);
 labelTemps->setVisible(true);
}

void ComprovaFigures::desaLlistaComparacioSlot(){
   desaDadesCercaFigures();
}

/*
  Desa les dades de la comprovació de les figures
  a un arxiu de text
  */
void ComprovaFigures::desaDadesCercaFigures(){
    QMessageBox missatgeBox;
    /* Això és per impedir que es pugui accedir al formulari de figures
     * i quedar el missatgeBox devall*/
    missatgeBox.setParent(this);
    missatgeBox.setWindowModality(Qt::WindowModal);
    missatgeBox.setText(configuracioJoc->tradueixCadena("desar.dades.comprovacio"));
    missatgeBox.setIcon(QMessageBox::Question);
    missatgeBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    missatgeBox.setDefaultButton(QMessageBox::No);
    if(missatgeBox.exec()==QMessageBox::Yes){
        /* 11/12/15 afegit el filtre*/
        QString nomArxiuComplet = QFileDialog::getSaveFileName(this,
                           configuracioJoc->tradueixCadena("desar.dades.comprovacio"),
                           directoriFiguresPersonals(),"(*.comp)");
        /*  11/12/15 Posam l'extensió de l'arxiu*/
        if(nomArxiuComplet.right(5) != ".comp"){
            nomArxiuComplet=nomArxiuComplet+".comp";
        }

        if (!nomArxiuComplet.isEmpty()){
            QFile arxiu(nomArxiuComplet);
            if (!arxiu.open(QIODevice::WriteOnly)){
                QMessageBox::information(this, configuracioJoc->tradueixCadena("desar.dades.comprovacio"),
                                       configuracioJoc->tradueixCadena("concurs.error")+"\n"+
                                       configuracioJoc->tradueixCadena("arxiuNoCreat").arg(arxiu.fileName()) );
               }
            else {
                QTextStream entrada(&arxiu);
                entrada<<(QString("%1 items\n").arg(figuresSemblantsList->count()));
                entrada<<(QString("===========\n"));
                for(int j=0;j<figuresSemblantsList->count();j++){
                    QStringList list1 = figuresSemblantsList->value(j).split(";");
                    entrada<<(QString("%1 => %2\n")
                             .arg(configuracioJoc->tradueixCadena("figures.noves"))
                             .arg(
                            arbreNovesFiguresTreeWidget->topLevelItem(0)->child(list1.value(0).toInt())->text(0)));
                    if(list1.value(2)=="0"){
                        entrada<<(QString("%1 => %2\n")
                                 .arg(configuracioJoc->tradueixCadena("figures.programa"))
                                 .arg(
                                arbreFiguresTreeWidget->topLevelItem(0)->child(list1.value(1).toInt())->text(0)));
                     }
                    else {
                        entrada<<(QString("%1 => %2\n")
                                .arg(configuracioJoc->tradueixCadena("figures.noves"))
                                .arg(
                                arbreNovesFiguresTreeWidget->topLevelItem(0)->child(list1.value(1).toInt())->text(0)));
                    }
                entrada<<(QString("%1%\n").arg(list1.value(3)));
                entrada<<(QString("*****************\n"));
                }
            }
            arxiu.close();
        }
    }
}

void ComprovaFigures::tancaSlot() {
    close();
    }

/*Per controlar les dades de generació automàtica de figures*/
void ComprovaFigures::dadesGeneracioFigures_carregaDades(){
    /*carregam les dades de les peces*/
  llistaDadesPeces=dadesGeneracioFigures_carregaDadesPosicionsPeces();
  activaBotonsMovimentLlistaComparacioFigures();
}

/*coloca les peces segons les dades corresponents
 de la llista de dades de peces llistaDadesPeces*/
void ComprovaFigures::dadesGeneracioFigures_colocaPeces(){
    QStringList dades;
    dades.append(llistaDadesPeces.value(indexLlistaDadesPeces*3));
    dades.append(llistaDadesPeces.value(indexLlistaDadesPeces*3+1));
    dades.append(llistaDadesPeces.value(indexLlistaDadesPeces*3+2));
    qDebug("%d",indexLlistaDadesPeces);
    qCritical("%s", qPrintable(dades.value(0)));
    qCritical("%s", qPrintable(dades.value(1)));
    qCritical("%s", qPrintable(dades.value(2)));
    qDebug("**********");
    /*Ara col·locam les peces*/
    QStringList dadesPecaColocada=dades.value(1).split(" ");
    QStringList dadesPecaAColocar=dades.value(2).split(" ");
    conjuntComparacioOriginal->conjuntVisible(true);
    //conjuntComparacioOriginal->conjuntSolucio();
    //conjuntComparacioOriginal->colocaPecesAtzar();
    conjuntComparacioOriginal->arrayPeces[0]->tipusPeca=dadesPecaColocada.value(0).toInt();
    conjuntComparacioOriginal->arrayPeces[1]->tipusPeca=dadesPecaAColocar.value(0).toInt();
    /*col·locam la primera peça*/
    conjuntComparacioOriginal->arrayPeces[0]->reiniciaAnglesVertex();
    conjuntComparacioOriginal->arrayPeces[0]->setPos(200,200);
   // conjuntComparacioOriginal->arrayPeces[1]->setPos(200,50);
    if ( (dadesPecaColocada.value(4)=="1") && (!conjuntComparacioOriginal->arrayPeces[0]->simetriaBool) ){
        conjuntComparacioOriginal->arrayPeces[0]->simetriaPeca();
    }
    conjuntComparacioOriginal->arrayPeces[0]->setAngleRotacio(dadesPecaColocada.value(3).toFloat());

    /*col·locam la segona peça*/
    QPointF vectorMoviment=QPointF(dadesPecaAColocar.value(1).toFloat(),
                                   dadesPecaAColocar.value(2).toFloat());

    conjuntComparacioOriginal->arrayPeces[1]->reiniciaAnglesVertex();
   // qDebug("%f %f",vectorMoviment.x(),vectorMoviment.y());
   // qDebug("%f %f",conjuntComparacioOriginal->arrayPeces[0]->pos().x()+vectorMoviment.x(),
        //    conjuntComparacioOriginal->arrayPeces[0]->pos().y()+vectorMoviment.y());
    conjuntComparacioOriginal->arrayPeces[1]->setPos(
            conjuntComparacioOriginal->arrayPeces[0]->pos()+
            vectorMoviment);
    /* Cal considerar totes les possibilitats: que calgui fer la simetria o no
     * i que ja s'hagi fet la simetria o no*/
    if( ((dadesPecaAColocar.value(4)=="1") && (!conjuntComparacioOriginal->arrayPeces[1]->simetriaBool)) ||
        ((dadesPecaAColocar.value(4)=="0") && (conjuntComparacioOriginal->arrayPeces[1]->simetriaBool)) )
       {
       conjuntComparacioOriginal->arrayPeces[1]->simetriaPeca();
    }
    conjuntComparacioOriginal->arrayPeces[1]
            ->setAngleRotacio(dadesPecaAColocar.value(3).toFloat());

    sceneComparacio->update();
    //pausa(500);
   /* qCritical("%s", qPrintable(conjuntComparacioOriginal->arrayPeces[0]->
              cadenaPropietatsPeca(QPointF(200,200))));
    qCritical("%s", qPrintable(conjuntComparacioOriginal->arrayPeces[1]->
              cadenaPropietatsPeca(QPointF(200,200))));
    qDebug("++++++++");*/

}

/* Carrega totes les dades de colocació de peces de l'arxiu corresponent*/
QStringList ComprovaFigures::dadesGeneracioFigures_carregaDadesPosicionsPeces(){
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
      while (!linia0.isNull()) {
          if( !linia0.contains("##")){
         // qCritical("%s", qPrintable(linia0));
              llista.append(linia0);
          }
          linia0 = lectura.readLine();
   }
   arxiu.close();
  }
  return llista;
}

/* Carrega totes les dades de colocació de peces de l'arxiu corresponent*/
/* Modificat per fer la conversió de les dades de generació de figures del format
 * antic al nou (sense duplicar les posicions)*/
QStringList ComprovaFigures::dadesGeneracioFigures_carregaDadesPosicionsPeces1(){
  QString nomArxiuDades=configuracioJoc->arxiuDadesFigures.mid(0,
         configuracioJoc->arxiuDadesFigures.length()-4);
  nomArxiuDades.append(".aut");
  QFile arxiuFinal(nomArxiuDades);
  nomArxiuDades=directoriLinux()+charPathSeparator+"auto"+charPathSeparator
                +nomArxiuDades;

  QFile arxiu(nomArxiuDades);
  arxiuFinal.open(QIODevice::Append);
  QTextStream entrada(&arxiuFinal);
  QStringList llista;
  int contadorLinies=1;
  if (arxiu.open(QIODevice::ReadOnly)){
      QTextStream lectura(&arxiu);
      QString linia0 = lectura.readLine();
      while (!linia0.isNull()) {
          if( !linia0.contains("##")){
         // qCritical("%s", qPrintable(linia0));
              llista.append(linia0);//}
              if (contadorLinies<4){
                  entrada<<(linia0+"\n");
                 }
              contadorLinies++;
              }
          else {
             entrada<<(linia0+"\n");
          }
          linia0 = lectura.readLine();
          if (contadorLinies>6){
             contadorLinies=1;
          }
         // qDebug("contadorLinies %d",contadorLinies);
   }
   arxiu.close();
   arxiuFinal.close();
  }
  return llista;
}

bool ComprovaFigures::generaFiguresMassiuAtzar_ComprovaCompactacio(){
    for(int i=0;i<=arbreNovesFiguresTreeWidget->topLevelItem(0)->childCount()-1;i++){
        conjuntComparacioOriginal->colocaPeces(
                arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->text(1));
        conjuntComparacioOriginal->conjuntVisible(true);
        qApp->processEvents();


        QList<int> pecesCompactes;
        //QStringList pecesCompactes;
        pecesCompactes.append(0);
        int numeroCompactes=0;
        while (pecesCompactes.size()> numeroCompactes) {
        for(int m=0;m<pecesCompactes.size();m++){
            for(int n=m+1;n<conjuntComparacioOriginal->arrayPeces.size();n++){
                /*for(int m=1;m<conjuntComparacioOriginal->arrayPeces.size();m++){
                    for(int n=2;n<conjuntComparacioOriginal->arrayPeces.size();n++){*/
            //if (!conjuntComparacioOriginal->arrayPeces[m]->pecaSuperposadaEscala(false,1.01)) {
            //if( !conjuntComparacioOriginal->arrayPeces[n]->pecaSuperposada(pecesCompactes)){
            if( !conjuntComparacioOriginal->arrayPeces[n]->pecaSuperposadaEscala(pecesCompactes,1.02)){

                        /*if( (conjuntFigura->arrayPeces[m]->pecaSuperposadaEscala(false,0.95)) &&
                    (!conjuntFigura->arrayPeces[m]->pecaSuperposadaEscala(false,1.01)) ){*/
            //compacte=false;
            }
            else {
              if (!pecesCompactes.contains(n)){
                   pecesCompactes.append(n);
                  // qDebug("%d",n);
                }
              }
            }
          }
         numeroCompactes++;
        }
      if (pecesCompactes.size() != conjuntComparacioOriginal->arrayPeces.size()) {
         arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->setTextColor(0,Qt::red);
        // qCritical("%s", qPrintable(arbreNovesFiguresTreeWidget->topLevelItem(0)->child(i)->text(0)));
      }

    }//final for arbre de figures
   return true;
}
