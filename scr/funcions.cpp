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
#include "funcions.h"

#include <QtGui>

#include <QtWidgets/QColorDialog>

#include "configuracio.h"
#include "constants.h"
#include "conjuntpeces.h"


QString directoriFiguresPersonals(){
    QDir dirArxiu(QDir::homePath ());
    if (!dirArxiu.exists(DIR_ARXIUS_FIGURES_PERSONALS)){
         dirArxiu.mkdir(DIR_ARXIUS_FIGURES_PERSONALS);
      }
 return QDir::homePath ()+QDir().separator()+DIR_ARXIUS_FIGURES_PERSONALS;
}

QString directoriFiguresGeneradesAtzar(){
    QDir dirArxiu(directoriFiguresPersonals());
    if (!dirArxiu.exists("auto")){
         dirArxiu.mkdir("auto");
      }
return QDir::homePath ()+QDir().separator()+DIR_ARXIUS_FIGURES_PERSONALS
        +QDir().separator()+"auto";
}

QString filtreArxiusConcurs(Configuracio *confJoc){
return confJoc->tradueixCadena("Concurs")+" (*."+
                   confJoc->arxiuDadesFigures.mid(0,
                           confJoc->arxiuDadesFigures.lastIndexOf("."))+
                           QString("%1)").arg(confJoc->nivellJoc) ;
}

QString extensioArxiuConcurs(Configuracio *confJoc){
 return "."+confJoc->arxiuDadesFigures.mid(0,
                           confJoc->arxiuDadesFigures.lastIndexOf("."))+
                           QString("%1").arg(confJoc->nivellJoc);
}

/*
  Noms dels arxius de les figures generades a l'atzar
  pel formulari de revisió i per la càrrega

  El nom comencarà amb el nom de l'arxiu de figures del tangram
  seguit de - i el que vulgui l'usuari i l'extensió .fig
  */
QString filtreArxiusFiguresGeneradesAtzar(Configuracio *confJoc, int tipus){
QString tipusFig;
if(tipus==0){
   tipusFig="arxiu.figures";
}
else tipusFig="figures.noves";


/*25/04/14 El filtre no funcionava bé amb els noms d'arxius que compartien
 * part del nom. Ara es filtren els arxius amb el nom exacte corresponent
 * al tangram i amb el sufix -*.fig*/
/* 16/10/15 Juntats els dos tipus de filtre per evitar haver de seleccionar-los
 */
  return  "("+confJoc->arxiuDadesFigures+" "+
       confJoc->arxiuDadesFigures.mid(0,
       confJoc->arxiuDadesFigures.lastIndexOf("."))+"-*.fig)";
 /* return  "("+confJoc->arxiuDadesFigures+");;("+
       confJoc->arxiuDadesFigures.mid(0,
       confJoc->arxiuDadesFigures.lastIndexOf("."))+"-*.fig)";*/

}

QString extensioArxiuFiguresGeneradesAtzar(Configuracio *confJoc){
return "."+confJoc->arxiuDadesFigures.mid(0,
                              confJoc->arxiuDadesFigures.lastIndexOf("."))+
                              QString("%1").arg(confJoc->nivellJoc);
}

/*
retorna la suma de tots els nombres entre
nombre i 1
*/
int sumatori(int nombre){
    /*int sumatori=0;
    for(int i=1;i<nombre;i++){
        sumatori=sumatori+i;
    }*/
    return ((nombre*(nombre-1))/2);
}

/*
Retorna el icone corresponent al nivel de dificultat
de la figura
*/
QIcon iconeFigura(int tipus){
switch(tipus){
     case 1:
       return QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"facil.png");
     //  return QIcon(":/images/facil.png");
       break;
     case 2:
       return QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"mitja.png");
      // return QIcon(":/images/mitja.png");
       break;
     case 3:
       return QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"dificil.png");
    //   return QIcon(":/images/dificil.png");
       break;
     case 4:
       return QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"ok.png");
     //  return QIcon(":/images/ok.png");
       break;
     default:
       return QIcon(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"facil.png");
      // return QIcon(":/images/facil.png");
   }
}

/*
  Directoris linux
  21/04/11
  Modificat per permetre la coexistència de l'aplicació instal·lada
  i el codi font
 */
QString directoriLinux(){

 QDir dirArxiu;
 /*qCritical("%s", qUtf8Printable(QCoreApplication::applicationDirPath()+ QDir().separator()+
                                "images"));*/
  if (dirArxiu.exists(QCoreApplication::applicationDirPath()+ QDir().separator()+
                      "images")){
      return QCoreApplication::applicationDirPath();
  }
  else if (dirArxiu.exists("/usr/share/games/glpeces")){
      return "/usr/share/games/glpeces";
   }
  else return QCoreApplication::applicationDirPath();
 /*
 qCritical("Directori palicacio: %s", qPrintable(QCoreApplication::applicationDirPath()));
 QString dirInicial=QCoreApplication::applicationDirPath().replace(DIRECTORIS_LINUX_PROGRAMA,"");
 qCritical("Directori inicial: %s", qPrintable(dirInicial));
 dirInicial=dirInicial.append(DIRECTORIS_LINUX_AUXILIARS);
 qCritical("Directori inicial 2: %s", qPrintable(dirInicial));
 QDir dirArxiu;
    if (dirArxiu.exists(DIRECTORIS_LINUX)){
       return DIRECTORIS_LINUX;
    }
    else if (dirArxiu.exists(dirInicial)){
        return dirInicial;
    }
    else return QCoreApplication::applicationDirPath();
    */
}

/*
  13/02/11
  Carrega el contingut de l'arxiu passat per paràmetre
  a una llista (que retorna)
  */
QStringList llegeixArxiuALlista(QString arxiu){
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

QString llegeixLiniaArxiu(QString arxiu, int linia){
    //obrim l'arxiu de jocs
    QFile arxiuData(arxiu);
    QString linia0;
    //QString  dadesArxiu;
    if (arxiuData.open(QIODevice::ReadOnly | QIODevice::Text)){
     QTextStream entrada(&arxiuData);
     linia0 = entrada.readLine();
     int numeroLinia=1;
     while ( (!linia0.isNull())
         && (numeroLinia<linia) ) {
         linia0 = entrada.readLine();
         numeroLinia++;
         }
     } //final del while de la lectura de l'arxiu
 return linia0;
}

/*
  Genera els directoris necessaris per la generació
  dels arxius html
  */
void directorisHTML(QString dir){
    QDir dirHTML(directoriFiguresPersonals()+QDir().separator()+"html");
    if (!dirHTML.exists()){
         dirHTML.mkdir(directoriFiguresPersonals()+QDir().separator()+"html");
      }
    QDir dirImatge(directoriFiguresPersonals()+QDir().separator()+"html");
    if (!dirImatge.exists("img")){
         dirImatge.mkdir(directoriFiguresPersonals()+QDir().separator()+"html"+QDir().separator()+"img");
      }
    QDir dirArxiu(directoriFiguresPersonals()+QDir().separator()+"html"+QDir().separator()+"img");
    if (!dirArxiu.exists(dir)){
         dirArxiu.mkdir(dir);
      }
    //directori pels arxius latex
    QDir dirLatex(directoriFiguresPersonals()+QDir().separator()+"latex");
    if (!dirLatex.exists()){
         dirLatex.mkdir(directoriFiguresPersonals()+QDir().separator()+"latex");
      }
}
/*
  Retorna un nombre l'atzar a partir del passat
  com a parámetre
  */
int generaNombreAtzar(int llavor){
    qsrand(llavor);
    return qrand();
}

/*
  Retorna la diferència de longitud de les siluetes,
  com a percentatge de la longitud de la silueta
  amb major longitud
  */
float comparacioLongitudSiluetesPercentatge(qreal longitud1,qreal longitud2){
    qreal minim,Maxim;
    if(longitud1<=longitud2){
       minim= longitud1;
       Maxim= longitud2;
    }
    else{
        minim= longitud2;
        Maxim= longitud1;
    }
    //qDebug("percentatge semblança %f",((Maxim-minim)*100)/minim);
    return ((Maxim-minim)*100)/minim;
}

/*
  Comprova que els centres de les figures dels dos
  conjunts de peces estiguin uns dins dels altres
  */
bool comprovaCentresFigures(ConjuntPeces *conjunt1,
                                             ConjuntPeces *conjunt2){
 int numeroCentresCorrectes=0;
 int descomptarCentre=0;
 for(int k=0;k<conjunt1->arrayPeces.size();k++){
 QPointF centrePeca=conjunt1->arrayPeces[k]->pos()+
         conjunt1->arrayPeces[k]->centreRectanglePeca();
 //Si el centre de la peça està dins de la peça, el comprovam
 if (conjunt1->arrayPeces[k]->shape().contains(
         conjunt1->arrayPeces[k]->centreRectanglePeca())){
  for(int i=0;i<conjunt2->arrayPeces.size();i++){
          if( conjunt2->arrayPeces[i]->shape().contains(
                  centrePeca-conjunt2->arrayPeces[i]->pos())
              ){
              numeroCentresCorrectes=numeroCentresCorrectes+1;
              break;
          }
      }
    }
 else descomptarCentre=descomptarCentre+1;
}
return abs(numeroCentresCorrectes-(conjunt1->arrayPeces.size()-descomptarCentre))<2;
//return numeroCentresCorrectes==(conjunt1->arrayPeces.size()-descomptarCentre);
}
/*
  Permet seleccionar un color per la peça
  i escriu la selecció a l'arxiu de configuració
  */
QColor personalitzaColorPeca(int indexEnElConjunt,
                             QColor colorOrig, int pTipusTangram){
    QColor color;
    color = QColorDialog::getColor(Qt::green);
    if ( (color.isValid()) && (color != Qt::white) ) {
      desaColorsPecaPersonalitzat(QString("%1/%2")
                                  .arg(pTipusTangram).arg(indexEnElConjunt),
                                  color.name());
      return color;
    }
    else return colorOrig;
}

void eliminaPersonalitzaColorPeca(int indexEnElConjunt,
                                  int ptipusTangram){
    QSettings *configArxiu=new QSettings(QSettings::IniFormat,
                                  QSettings::UserScope,"De Marchi", "Peces");
    configArxiu->remove(QString("%1/%2")
                        .arg(ptipusTangram)
                        .arg(indexEnElConjunt));
}

void desaColorsPecaPersonalitzat(QString clau,QString color){
QSettings *configArxiu=new QSettings(QSettings::IniFormat,
                              QSettings::UserScope,"De Marchi", "Peces");
configArxiu->setValue(clau,color);
}


/* Retorna la distància entre els dos punts passats
 */
double distanciaEntrePunts(QPointF punt1,QPointF punt2){
  //qDebug("distanciaEntrePunts %f",sqrt(pow(punt1.x()-punt2.x(),2)+pow(punt1.y()-punt2.y(),2)));
  return sqrt(pow(punt1.x()-punt2.x(),2)+pow(punt1.y()-punt2.y(),2)) ;
}


/* Retorna el sentit del gir que cal donar a una peça
 * depenent de si el punt2 està a la dreta o l'esquerra
 * del punt central de la peça
 */
int sentitDelGir(QPointF centrePeca,QPointF punt){
    if (punt.x()-centrePeca.x()>=0){
        return 1;
    }
    else return -1;
}

/* fa una pausa en milisegons
void pausa(int temps){
  QTest::qSleep(temps);
  qApp->processEvents();
}*/

/* Procediments de cerca de figures
 */

/* Compara el nombre d'elements de cada silueta
 * i retorna si son iguals o no
 */
/*double comparaQPainterPath(QPainterPath original, QPainterPath comparada, QPointF centreOriginal,
                           QPointF centreComparada){
    QTransform transformOrig;
    transformOrig.rotate(0,Qt::XAxis);
    QList<QPointF> llistaPoligonOriginal=vertexQPainterPath(original.toFillPolygon(transformOrig).toList(),
                                                            centreOriginal);
    QList<QPointF> llistaPoligonComparada=vertexQPainterPath(comparada.toFillPolygon(transformOrig).toList(),
                                                             centreComparada);

    //Si les llistes no tenen el mateix nombre d'elements, no poden ésser iguals
     // (tot i què una petita diferència pot fer que les llistes no siguin iguals!)
    if(llistaPoligonOriginal.count() != llistaPoligonComparada.count() ){
      int contador=0;
      for(int t=0;t<llistaPoligonOriginal.count();t++){
         if(comparada.contains(llistaPoligonOriginal.value(t))){
          contador++;
          //qDebug("%f %f",llistaPoligonOriginal.value(t).x(),llistaPoligonOriginal.value(t).y());
         }
      }
      qDebug("contador %d", contador);
      for(int t=0;t<llistaPoligonComparada.count();t++){
         if(original.contains(llistaPoligonComparada.value(t))){
          //qDebug("%f %f",llistaPoligonOriginal.value(t).x(),llistaPoligonOriginal.value(t).y());
          contador++;
         }
      }
      qDebug("contador %d", contador);
      qDebug("llistes %d", llistaPoligonOriginal.count()+llistaPoligonComparada.count());
      qDebug("percentatge %f",float(contador*100.0/(llistaPoligonOriginal.count()+llistaPoligonComparada.count())));
      if(contador*100/(llistaPoligonOriginal.count()+llistaPoligonComparada.count())>98.0){
        qDebug("resultat punt 1");
        return contador*100/(llistaPoligonOriginal.count()+llistaPoligonComparada.count());
      }
    }

    if(llistaPoligonOriginal.count() == llistaPoligonComparada.count() ){
    int contador=0;
    for(int t=0;t<llistaPoligonOriginal.count();t++){
        for(int m=0;m<llistaPoligonComparada.count();m++){
        if(distanciaEntrePunts(llistaPoligonOriginal.value((t) % llistaPoligonOriginal.count()),
                                   llistaPoligonComparada.value((m) % llistaPoligonComparada.count()))<5.0){
           contador++;
        }
        }
       }
      int mesVertex=0;
      if(llistaPoligonOriginal.count()>=llistaPoligonComparada.count()){
          mesVertex=llistaPoligonOriginal.count();
      }
      else mesVertex=llistaPoligonComparada.count();
      qDebug("percentatge %f ",contador*100.0/mesVertex);
      if(contador*100/mesVertex>98.0){
        qDebug("percentatge %f ",contador*100.0/mesVertex);
        qDebug("resultat punt 2");
         return contador*100.0/mesVertex;
      }

      contador=0;
      //if(llistaPoligonOriginal.count() == llistaPoligonComparada.count() ){
      for(int t=1;t<llistaPoligonOriginal.count();t++){
          double modulOrigen=distanciaEntrePunts(llistaPoligonOriginal.value(0),
                                                 llistaPoligonOriginal.value(t));
          double angleOrigen=angleVertex(llistaPoligonOriginal.value(0),
                                         llistaPoligonOriginal.value(t));

          for(int m=1;m<llistaPoligonComparada.count();m++){
              //for(int n=1;n<llistaPoligonComparada.count();n++){
              //if(m !=n){
              double modulComparada=distanciaEntrePunts(llistaPoligonComparada.value(0),
                                                     llistaPoligonComparada.value(m));
              double angleComparada=angleVertex(llistaPoligonComparada.value(0),
                                             llistaPoligonComparada.value(m));


              if( fabs(modulOrigen-modulComparada)<5 &&
                  fabs(angleOrigen-angleComparada)<2){
                  qDebug("diferencia moduls %f",fabs(modulOrigen-modulComparada) );
                  qDebug("diferencia angles %f",fabs(angleOrigen-angleComparada));
                  contador++;
                  break;
              }
              //}
            // }
           }
          }
      qDebug("contador %d", contador);
      qDebug("percentatge per moduls i angles %f ",contador*100.0/(mesVertex-1));
      if(contador*100/(mesVertex-1)>98.0){
        qDebug("percentatge per moduls i angles %f ",contador*100.0/(mesVertex-1));        
        qDebug("resultat punt 3");
         return contador*100/(mesVertex-1);
       }
      }


if(llistaPoligonOriginal.count() == llistaPoligonComparada.count() ){
       for(int t=0;t<llistaPoligonOriginal.count();t++){
          bool continuar=true;
          for(int m=0;m<llistaPoligonOriginal.count();m++){
              if (continuar){
              for(int n=0;n<llistaPoligonOriginal.count();n++){
                 if(continuar){
                 double distanciaPoligonOriginal,distanciaPoligonComparada,distancia;
                 double anglePoligonOriginal,anglePoligonComparada,angle1,angle2;
                 angle1=0.0;
                 distanciaPoligonOriginal=distanciaEntrePunts(
                                          llistaPoligonOriginal.value(n),
                                          llistaPoligonOriginal.value((n+1) % llistaPoligonOriginal.count()));
                 anglePoligonOriginal=angleEntreTresPunts(
                             llistaPoligonOriginal.value((n) % llistaPoligonOriginal.count()),
                             llistaPoligonOriginal.value((n+1) % llistaPoligonOriginal.count()),
                             llistaPoligonOriginal.value((n+2) % llistaPoligonOriginal.count()) );
                 distanciaPoligonComparada=distanciaEntrePunts(
                                          llistaPoligonComparada.value((n+t) % llistaPoligonOriginal.count()),
                                          llistaPoligonComparada.value((n+t+1) % llistaPoligonOriginal.count()));
                 anglePoligonComparada=angleEntreTresPunts(
                             llistaPoligonComparada.value((n+t) % llistaPoligonOriginal.count()),
                             llistaPoligonComparada.value((n+t+1) % llistaPoligonOriginal.count()),
                             llistaPoligonComparada.value((n+t+2) % llistaPoligonOriginal.count()));

                 distancia=distanciaPoligonOriginal-distanciaPoligonComparada;
                 angle1=anglePoligonOriginal-anglePoligonComparada;
                 angle2=180-(anglePoligonOriginal+anglePoligonComparada);

                // if ( (fabs(distancia) > 5.0) || (fabs(angle)>5.0) ){
                 if ( (fabs(distancia) > 2.0) && ( (fabs(angle1)>2.0) || (fabs(angle2)>2.0) ) ){
                     continuar=false;
                  }
                 }
               if (continuar && n==llistaPoligonOriginal.count()-1){                   
                   qDebug("resultat punt 4");
                 return 99.856;
               }
              }//for n
             }//if continuar
            // else qDebug("bota m %d",m);
          }//for m
       }//for t
}
    //}//else

 return 50.0;
}*/

/* Compara el nombre d'elements de cada silueta
 * i retorna el % de semblança
 */
double comparaQPainterPath(QPainterPath original, QPainterPath comparada, QPointF centreOriginal,
                           QPointF centreComparada){
    QTransform transformOrig;
    transformOrig.rotate(0,Qt::XAxis);
    QList<double> llistaPoligonOriginal;
    QList<double> llistaPoligonComparada;

    llistaPoligonOriginal=llistaDistanciesAngles(
                                          vertexQPainterPath(original.toFillPolygon(transformOrig).toList(),
                                                            centreOriginal));
    llistaPoligonComparada=llistaDistanciesAngles(
                                          vertexQPainterPath(comparada.toFillPolygon(transformOrig).toList(),
                                                             centreComparada));



//if(llistaPoligonOriginal.count() == llistaPoligonComparada.count() ){
//if(abs(llistaPoligonOriginal.count() - llistaPoligonComparada.count())<3 ){
    int punts=0;
    int angles=0;
    int final=0;
    float comparacioDistancia=1.5;
    float comparacioAngles=1.5;
    for(int t=0;t<llistaPoligonOriginal.count()/2;t++){
              bool continuar=false;
              for(int m=0;m<llistaPoligonOriginal.count()/2;m++){
                  punts=0;
                  angles=0;
                  /* Cercam la primera conincidència entre les dues llistes*/
                  /* Distància entre dos punts*/
                  if(fabs(llistaPoligonOriginal.value(2*t)-
                          llistaPoligonComparada.value(2*m % llistaPoligonComparada.count())) < comparacioDistancia){
                  /*Angle a un vèrtex*/
                  /*Angles  casi iguals*/
                  /*if( (fabs(llistaPoligonOriginal.value(2*t+1)+
                            llistaPoligonComparada.value(2*m+1)<comparacioAngles)) ||*/
                      /*Angles amb una diferència petita (casi iguals)*/
                  if(   (fabs(llistaPoligonOriginal.value(2*t+1)-
                            llistaPoligonComparada.value(2*m+1))<comparacioAngles) ||
                      /*Angles que casi resten 180 (són suplementaris)*/
                      (fabs(fabs(llistaPoligonOriginal.value(2*t+1)-
                                 llistaPoligonComparada.value(2*m+1))-180)<comparacioAngles) ||
                      /*Angles que casi sumen 180 (suplementaris)*/
                      fabs(fabs(llistaPoligonOriginal.value(2*t+1)+
                                llistaPoligonComparada.value(2*m+1))-180)<comparacioAngles ) {
                    continuar=true;
                    }
                  }
                  /*S'ha trobat una coincidència de la segona llista amb la primera
                   * A partir d'aquesta coincidència, es comença a comparar les dues llistes.
                   * Cada comparació positiva se suma a punts i angles*/
                  if (continuar){
                  for(int n=0;n<llistaPoligonOriginal.count()/2;n++){
                     int indexComparada=2*((n+m) % (llistaPoligonComparada.count()/2));
                     if(continuar){
                       /*  qDebug("indexComparada %d",indexComparada);
                         qDebug("distancies %f %f",llistaPoligonOriginal.value(2*n),
                                    llistaPoligonComparada.value(indexComparada));
                         qDebug("distancia %f",fabs(llistaPoligonOriginal.value(2*n)-
                                              llistaPoligonComparada.value(indexComparada)));
                         qDebug("angles %f %f",llistaPoligonOriginal.value(2*n+1),
                                llistaPoligonComparada.value(indexComparada+1));
                         qDebug("angle %f",fabs(llistaPoligonOriginal.value(2*n+1)+
                                                llistaPoligonComparada.value(indexComparada+1)));
                         qDebug("angle %f",fabs(llistaPoligonOriginal.value(2*n+1)+
                                                llistaPoligonComparada.value(indexComparada+1)-180));*/
                     /*Comparació de distàncies*/
                     if(fabs(llistaPoligonOriginal.value(2*n)-
                             llistaPoligonComparada.value(indexComparada)) < comparacioDistancia){
                       punts++;
                       /*Comparació d'angles*/
                     if( (fabs(llistaPoligonOriginal.value(2*n+1)+
                               llistaPoligonComparada.value(indexComparada+1))<comparacioAngles) ||
                         (fabs(llistaPoligonOriginal.value(2*n+1)-
                               llistaPoligonComparada.value(indexComparada+1))<comparacioAngles) ||
                         (fabs(fabs(llistaPoligonOriginal.value(2*n+1)-
                                    llistaPoligonComparada.value(indexComparada+1))-180)<comparacioAngles) ||
                         (fabs(fabs(llistaPoligonOriginal.value(2*n+1)+
                                   llistaPoligonComparada.value(indexComparada+1))-180)<comparacioAngles)
                         ) {
                       angles++;
                       }
                     }
                     /*Si la comprovació és positiva, es continua amb les comparacions
                      És important el valor 2: funciona millor que amb valors més alts!*/
                     if (abs(punts-(n+1))<2){
                     //if (punts==n+1){
                         continuar=true;
                      }
                     else continuar=false;
                     }
                   /*Es calcularà el % de coincidència a partir de la coincidència
                    * més petita*/
                   if(punts<angles){
                     final=punts;
                   }
                   else final=angles;
                   /*S'ha arribat al final sense trobar cap disparitat en les comprovacions*/
                   if (continuar && n==(llistaPoligonOriginal.count()/2)-1){
                      /* qDebug("punts %d", punts);
                       qDebug("angles %d", angles);
                       qDebug("resultat punt 4");*/
                       //qDebug("percentatge %f",final*100.0/(llistaPoligonOriginal.count()/2));
                     return final*100.0/(llistaPoligonOriginal.count()/2);
                    }
                   //}//for p
                  }//for n
                 }//if continuar
              }//for m
           }//for t
      //}//if inicial número de vèrtex

 return 50.0;
}


/* Localitza els vèrtex de la llista de QPainterPath
 * (localitzant els canvis de direcció en forma d'angles)
 * i els retorna en una llista de punts
 */
QList<QPointF> vertexQPainterPath(QList<QPointF> llista, QPointF centreFigura ){
double comparacioDistanciaAngle=1.5;

  llista=ordenaLlistaVertex(llista,centreFigura);
  /* elimina els punts que són pràcticament iguals*/
  /*Només elimina els que són contigus a la llista
   *Si dos punts són molt pròxims però no contigus a la llista
   * es mantenen
    Alguns s'eliminaran en revisar els angles*/
  QList<QPointF> llistaPuntsDepurada;
  for(int n=0;n<llista.count();n++){
     if(distanciaEntrePunts(llista.value((n) % llista.count()),
                            llista.value((n+1) % llista.count()))>comparacioDistanciaAngle){
       llistaPuntsDepurada.append(llista.value(n));
     }
  }
  /*qDebug("llista %d",llista.count());
  qDebug("llevats punts pròxims %d",llistaPuntsDepurada.count());*/

  /* depuram punts contigus a la llista que formen angles petits*/
  double angle=0.0;
  llista.clear();
  for(int m=0;m<llistaPuntsDepurada.count();m++){
     angle=angleEntreTresPunts(llistaPuntsDepurada.value((m-1) % llistaPuntsDepurada.count()),
                               llistaPuntsDepurada.value((m) % llistaPuntsDepurada.count()),
                               llistaPuntsDepurada.value((m+1) % llistaPuntsDepurada.count()) );
     if(angle>comparacioDistanciaAngle){
       llista.append(llistaPuntsDepurada.value(m));
     }
  }
 // qDebug("llevats punts angles petits %d",llista.count());

  llistaPuntsDepurada.clear();
  /*depuram punts repetits*/
  for(int n=0;n<llista.count();n++){
      bool igual=false;
      for(int m=n+1;m<llista.count();m++){
      if(distanciaEntrePunts(llista.value((n) % llista.count()),
                            llista.value((m) % llista.count()))<1.0){
       igual=true;
       break;
       }
     }
    if(!igual){
    //if(true){
      llistaPuntsDepurada.append(llista.value(n));
    }
  }
 /* qDebug("llevats punts repetits %d",llistaPuntsDepurada.count());
  for(int n=0;n<llistaPuntsDepurada.count();n++){
    qDebug("%f %f",llistaPuntsDepurada.value(n).x(),llistaPuntsDepurada.value(n).y()) ;
  }*/

 /* depuram punts que estan entre altres dos*/
  llista.clear();
  for(int m=0;m<llistaPuntsDepurada.count();m++){
     bool afegir=true;
     QPointF pmitja, panterior,pposterior;
     if(m==0){
       pmitja=llistaPuntsDepurada.value(m);
       panterior=llistaPuntsDepurada.value(llistaPuntsDepurada.count()-1);
       pposterior=llistaPuntsDepurada.value(m+1);
     }
     else if(m==llistaPuntsDepurada.count()-1){
         pmitja=llistaPuntsDepurada.value(m);
         panterior=llistaPuntsDepurada.value(m-1);
         pposterior=llistaPuntsDepurada.value(0);
     }
     else {
         pmitja=llistaPuntsDepurada.value(m);
         panterior=llistaPuntsDepurada.value(m-1);
         pposterior=llistaPuntsDepurada.value(m+1);
     }
     if(puntEntreDosPunts(pmitja,panterior,pposterior)){
       angle=angleEntreTresPunts(pmitja,panterior,pposterior);

     if(angle<comparacioDistanciaAngle || fabs(angle-180)<comparacioDistanciaAngle){
       afegir=false;
      }
     }//if punt mitja
  if(afegir && llista.indexOf(pmitja)<0){
     llista.append(pmitja);
   }
  }//form m


 //qDebug("llevats punts intermitjos %d",llista.count());

 /*qDebug("llista vertexQPainterPath %d",llista.count() );
 for(int n=0;n<llista.count();n++){
   qDebug("(%f , %f)",llista.value(n).x(),llista.value(n).y()) ;
 }
 qDebug("*********");*/
 return llista;
}

/* Retorna l'angle (en graus) format per tres punts
 * fent servir el producte escalar*/
double angleEntreTresPunts(QPointF PoligonOriginal_P1, QPointF PoligonOriginal_P2, QPointF PoligonOriginal_P3){
    QPointF vectorOriginal1,vectorOriginal2;
   /* qDebug("Punt 1 %f %f",PoligonOriginal_P1.x(),PoligonOriginal_P1.y());
    qDebug("Punt 2 %f %f",PoligonOriginal_P2.x(),PoligonOriginal_P2.y());
    qDebug("Punt 3 %f %f",PoligonOriginal_P3.x(),PoligonOriginal_P3.y());*/
    vectorOriginal1=PoligonOriginal_P2-PoligonOriginal_P1;
    vectorOriginal2=PoligonOriginal_P3-PoligonOriginal_P1;
   /* qDebug("vectorOriginal1 %f %f",vectorOriginal1.x(),vectorOriginal1.y());
    qDebug("vectorOriginal2 %f %f",vectorOriginal2.x(),vectorOriginal2.y());*/
    double cosinusOriginal;
    cosinusOriginal=(round(vectorOriginal1.x())*round(vectorOriginal2.x())+round(vectorOriginal1.y())*round(vectorOriginal2.y()))/
                     (sqrt(pow(round(vectorOriginal1.x()),2)+pow(round(vectorOriginal1.y()),2))*
                      sqrt(pow(round(vectorOriginal2.x()),2)+pow(round(vectorOriginal2.y()),2)));
    return acos(cosinusOriginal)*180.0/PI;
}

/* Determina si un punt (mitja) està entre els dos extrems
   Les coordenades y son negatives degut a que les coordenades de la pantalla
   son positives cap abaix */
bool puntEntreDosPunts(QPointF mitja,QPointF extrem1,QPointF extrem2){
 bool pmitja=false;
 if((int)extrem1.x()>=(int)mitja.x() && (int)mitja.x() >= (int)extrem2.x()){
   if((int)-extrem1.y() >= (int)-mitja.y() && (int)-mitja.y()>=(int)-extrem2.y()){
      pmitja=true;
   }
   else if((int)-extrem1.y() <= (int)-mitja.y() && (int)-mitja.y()<=(int)-extrem2.y()){
      pmitja=true;
   }
  }
 else if((int)extrem1.x()<=(int)mitja.x() && (int)mitja.x() <= (int)extrem2.x()){
     if((int)-extrem1.y() >= (int)-mitja.y() && (int)-mitja.y()>=(int)-extrem2.y()){
        pmitja=true;
     }
     else if(-extrem1.y() <= -mitja.y() && -mitja.y()<=-extrem2.y()){
        pmitja=true;
     }
    }
 return pmitja;
}

/*
double angleVertexRespecteOrigen(QPointF vertex){
    return atan(vertex.y()/vertex.x());
}*/

/*Calcula l'angle format per dos punts*/
double angleVertex(QPointF final, QPointF origen){
    QPointF vector=final-origen;
    double angleInicialGraus;

    angleInicialGraus=atan2(vector.y(),vector.x())*180/PI;

  //  qDebug("angle inicial %f",angleInicialGraus );
   /* qDebug("final %f %f",final.x(),final.y());
    qDebug("origen %f %f",origen.x(),origen.y());
    qDebug("vector %f %f",vector.x(),vector.y());
    qDebug("angle rectificat %f",angleInicialGraus );*/
    if(angleInicialGraus<0){
        angleInicialGraus=360+angleInicialGraus;
    }
    return angleInicialGraus;
}

/* Ordena els punts de la llista respecta al centre de la figura
 * i en sentit antihorari
 */
QList<QPointF> ordenaLlistaVertex(QList<QPointF> llista, QPointF centreFigura){
  /* primer calculam els angles de cada vertex*/
  QList<double> anglesVertex;
  QList<QPointF> llistaOrdenada;
  for(int n=0;n<llista.count();n++){
     anglesVertex.append(angleVertex(llista.value(n), centreFigura));
     llistaOrdenada.append(QPointF(0.0,0.0));
  }
  /*qDebug("*Inici ordenacio vertex****");
  for(int n=0;n<anglesVertex.count();n++){
    qDebug("%f",anglesVertex.value(n)) ;
  }*/
  qSort(anglesVertex.begin(),anglesVertex.end());
  /*qDebug("Llista angles ordenada");
  for(int n=0;n<anglesVertex.count();n++){
    qDebug("%f",anglesVertex.value(n)) ;
  }*/
  int contador=0;
  for(int n=0;n<anglesVertex.count();n++){
      contador=0;
      for(int m=0;m<anglesVertex.count();m++){
      if(fabs(anglesVertex.value(n)-angleVertex(llista.value(m), centreFigura))<0.0001 &&
         contador==0){
        llistaOrdenada.replace(n,llista.value(m));
        contador++;
     }
    }
   }
  /*qDebug("vertex ordenats %d",llistaOrdenada.count());
  qDebug("centre figura %f %f", centreFigura.x(),centreFigura.y());
  for(int n=0;n<llistaOrdenada.count();n++){
    qDebug("%f %f angle %f",llistaOrdenada.value(n).x(),llistaOrdenada.value(n).y(),anglesVertex.value(n)) ;
  }
  qDebug("*********");*/
  return llistaOrdenada;
}

/* Retorna una llista de distàncies i angles (en graus) elaborada a partir
 * d'una llista de vèrtex d'una figura
 */
QList<double> llistaDistanciesAngles(QList<QPointF> llistaVertex){
 QList<double> distanciesAngles;
 QPointF puntInicial, puntFinal,puntAnterior;
 for(int n=0;n<llistaVertex.count();n++){
  if(n==0){
    puntAnterior=llistaVertex.value(llistaVertex.count()-1);
    puntInicial=llistaVertex.value(n);
    puntFinal=llistaVertex.value(n+1);
  }
  else if(n==llistaVertex.count()-1){
    puntAnterior=llistaVertex.value(n-1);
    puntInicial=llistaVertex.value(n);
    puntFinal=llistaVertex.value(0);
  }
  else {
    puntAnterior=llistaVertex.value(n-1);
    puntInicial=llistaVertex.value(n);
    puntFinal=llistaVertex.value(n+1);
  }
  //distancia entre dos punts
  distanciesAngles.append(distanciaEntrePunts(puntInicial,puntFinal));
  //angle entre tres punts
  double angle=angleVertex(puntAnterior,puntInicial)-angleVertex(puntFinal,puntInicial);
  if(angle<0){
      angle=360+angle;
  }
  distanciesAngles.append(angle);
 }
 /*qDebug("llista distanciesAngles %d",llistaVertex.count());
 for(int n=0;n<distanciesAngles.count()/2;n++){
  qDebug("distancia %f angle %f",distanciesAngles.value(2*n),distanciesAngles.value(2*n+1));
 }
 qDebug("*********");*/
 return distanciesAngles;
}
