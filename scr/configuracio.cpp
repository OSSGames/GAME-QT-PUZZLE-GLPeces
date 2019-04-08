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
#include <QDesktopServices>

#include "configuracio.h"
#include "funcions.h"
 class QSettings;

Configuracio::Configuracio(){
charPathSeparator=QDir().separator();

configArxiu=new QSettings(QSettings::IniFormat, QSettings::UserScope,"De Marchi", "Peces");

arxiuTraduccio=new QSettings(arxiuLocale(),QSettings::IniFormat);

//carregam les dades de les peces
/* 07/02/12
//Obsolet de fa molt de temps
dadesPeces=llegeixArxiuALlista(
     directoriLinux()+QDir().separator()+"locales"+QDir().separator()+"peces.dat");*/

//Cal que sigui 0
  tipusTangram=0;
  nivellJoc=0;
  colocaPeca=false;
  desaRecords=true;

  canviaTipusTangram(configArxiu->value("joc/tipusTangram",1).toInt());//TIPUS_TANGRAM_T7_CLASSIC).toInt());
  canviaNivellJoc(configArxiu->value("joc/nivellJoc",NIVELL_INICIAT).toInt());


  //Comprovam si s'ha registrat l'aplicació
  if (configArxiu->value("joc/registre",0).toInt()==0){
     if(registraAplicacio()){
       configArxiu->setValue("joc/registre",1);
     };
  }
}

/*
Tradueix la cadena passada
*/
QString Configuracio::tradueixCadena(QString cadena){
//configArxiu->setValue("cadenes/"+cadena,1);
 //tot aquest follón per a què surtin els accents!!!
 QByteArray strArray = arxiuTraduccio->value(cadena,cadena).toString().toLatin1();
 //per cercar cadenes no traduïdes

 /*
 if (arxiuTraduccio->value(cadena,"").toString().toLatin1()==""){
     qCritical("Cadena no traduïda: %s", qPrintable(cadena));
 }
*/
 const char *strCadena = strArray.data();
 return QString::fromUtf8(strCadena);
}

/*
Retorna l'arxiu de traducció adequat
*/
QString Configuracio::arxiuLocale(){
  QString locale = strLocale();
  QDir dirArxiu;
  QString nomArxiu=directoriLinux(); //QCoreApplication::applicationDirPath();
 nomArxiu.append(charPathSeparator);
 nomArxiu.append("locales");
 nomArxiu.append(charPathSeparator);
 nomArxiu.append("locale_");
 nomArxiu.append(locale);
 nomArxiu.append(".lo");


 if (dirArxiu.exists(nomArxiu)){     
     configArxiu->setValue("joc/idioma",locale);
     return nomArxiu;
 }
 else {
     configArxiu->setValue("joc/idioma","en");
     return directoriLinux()+
                     charPathSeparator+"locales"+charPathSeparator+"locale_en.lo";
 }
}

/*
Retorna les lletres del locale
*/
QString Configuracio::strLocale(){
    return configArxiu->value("joc/idioma",QLocale::system().name().mid(0,2)).toString();
}

void Configuracio::canviaArxiuLocale(QString strLocale){
QDir dirArxiu;
 QString nomArxiu=directoriLinux(); //QCoreApplication::applicationDirPath();
 nomArxiu.append(charPathSeparator);
 nomArxiu.append("locales");
 nomArxiu.append(charPathSeparator);
 nomArxiu.append("locale_");
 nomArxiu.append(strLocale);
 nomArxiu.append(".lo");


 if (dirArxiu.exists(nomArxiu)){
     arxiuTraduccio=new QSettings(nomArxiu,QSettings::IniFormat);
     configArxiu->setValue("joc/idioma",strLocale);
 }
 else {
     arxiuTraduccio=new QSettings(directoriLinux()+
                     charPathSeparator+"locales"+charPathSeparator+"locale_en.lo",QSettings::IniFormat);     
     configArxiu->setValue("joc/idioma","en");
 }
}


void Configuracio::carregaPuntuacio(){
  //Carregam la puntuació anterior
  temps=configArxiu->value(QString("%1-%2/temps").arg(tipusTangram).arg(nivellJoc),"00:00:00").toString();
  comptadorFigures=configArxiu->value(QString("%1-%2/numFig").arg(tipusTangram).arg(nivellJoc),0).toInt();
  comptadorPunts=configArxiu->value(QString("%1-%2/punts").arg(tipusTangram).arg(nivellJoc),0).toInt();
}

void Configuracio::desaPuntuacio(QString pTemps){
  if (pTemps=="00:00:00"){
      configArxiu->remove(QString("%1-%2").arg(tipusTangram).arg(nivellJoc));
  }
  else if (desaRecords){
  temps=pTemps;
  configArxiu->setValue(QString("%1-%2/temps").arg(tipusTangram).arg(nivellJoc),pTemps);
  configArxiu->setValue(QString("%1-%2/numFig").arg(tipusTangram).arg(nivellJoc),comptadorFigures);
  configArxiu->setValue(QString("%1-%2/punts").arg(tipusTangram).arg(nivellJoc),comptadorPunts);
  }
}


    /*
     * Canvia el tipus de tangram segons el paràmetre passat
     * @param tipusQSettings
     */
   void Configuracio::canviaTipusTangram(int tipus){
    if (tipus!=tipusTangram){
        if(tipus>40){tipusTangram=1;}
        else tipusTangram=tipus;
    configArxiu->setValue("joc/tipusTangram", tipusTangram);
    carregaPuntuacio();
    /* 23/02/11
       Simplificat el procediment
       NOU TANGRAM 5
      */
    QStringList dadesTangram;
    dadesTangram.append("t7.fig;Fig_T7;pecesset.png"); //tangram clàssic                            codi 1
    dadesTangram.append("t5.fig;Fig_T5;peces5.png"); //Tangram 5                                    codi 2
    dadesTangram.append("t6.fig;Fig_T6;peces6.png"); //Tangram 6 I                                  codi 3
    dadesTangram.append("tf.fig;Fig_TF;peces_f.png"); //Tangram de Fletcher (Pitàgores)             codi 4
    dadesTangram.append("tx.fig;Fig_TX;peces_x.png"); //Tangram del xinès també Tangram Pitagòric   codi 5
    dadesTangram.append("ts.fig;Fig_TS;peces_s.png"); //Tangram de la saviesa Chie-No-Ita           codi 6
    dadesTangram.append("t8.fig;Fig_T8;peces8.png"); //Tangram 8                                    codi 7
    dadesTangram.append("t9.fig;Fig_T9;peces9.png"); //Tangram 9                                    codi 8
    dadesTangram.append("t10.fig;Fig_T10;peces10.png"); //Tangram 10                                codi 9
    dadesTangram.append("t11.fig;Fig_T11;peces11.png"); //Tangram 11                                codi 10
    dadesTangram.append("tr.fig;Fig_TR;peces_r.png"); //Tangram rus                                 codi 11
    dadesTangram.append("t13.fig;Fig_T13;peces13.png"); //Tangram 13                                codi 12
    dadesTangram.append("t14.fig;Fig_T14;peces14.png"); //Tangram 14                                codi 13
    dadesTangram.append("tb.fig;Fig_TB;peces_b.png"); //Tangram de Brügner                          codi 14
    dadesTangram.append("th.fig;Fig_TH;peces_h.png"); //Tangram Hexagrama                           codi 15
    dadesTangram.append("tou.fig;Fig_TOU;peces_ou.png"); //Tangram de l'ou                          codi 16
    dadesTangram.append("tc.fig;Fig_COR;peces_c.png"); //Tangram del cor                             codi 17
    dadesTangram.append("tst.fig;Fig_ST;peces_sto.png");
    ///Tangram Stomachion o Luculus Archimedius                              codi 18
    dadesTangram.append("t4.fig;Fig_T4;peces4.png"); //Tangram de 4 peces I                         codi 19
    dadesTangram.append("tcI.fig;Fig_TCIRI;pecesCI.png"); //Tangram circular I                      codi 20
    dadesTangram.append("t7I.fig;Fig_T7I;peces7I.png"); //Tangram 7 I                               codi 21
    dadesTangram.append("tCII.fig;Fig_TCII;pecesCII.png"); //Tangram circular de 8 peces CII        codi 22
    dadesTangram.append("t6II.fig;Fig_T6II;peces6II.png"); //Tangram 6 II                           codi 23
    dadesTangram.append("t7II.fig;Fig_T7II;peces7II.png"); //Tangram 7 II Revathi                   codi 24
    dadesTangram.append("treg.fig;Fig_REG;pecesREG.png"); //Tangram 5 I Regulus                     codi 25
    dadesTangram.append("thep.fig;Fig_THEP;pecesHEP.png"); //Tangram 7 Heptex                       codi 26
    dadesTangram.append("tsl.fig;Fig_TSL;pecesSL.png"); //Tangram de Sam Lloyds                     codi 27
    dadesTangram.append("t9t.fig;Fig_T9T;peces9t.png"); //Tangram "los nueve tangram"               codi 28
    dadesTangram.append("te.fig;Fig_TE;pecese.png"); //Tangram Egipci                               codi 29
    dadesTangram.append("t4II.fig;Fig_T4II;peces4II.png"); //Tangram de 4 peces II                  codi 30
    dadesTangram.append("tstII.fig;Fig_STII;pecesSTOII.png");
                          //Tangram Stomachion rectangular                                          codi 31
    dadesTangram.append("tstIII.fig;Fig_STIII;pecesSTOIII.png");
    //Tangram Stomach (modificació de Stomachion)                                                   codi 32
    dadesTangram.append("el.fig;Fig_EL;pecesel.png"); //Tangram Elzzup (10 peces)                  codi 33
    dadesTangram.append("t8e.fig;Fig_T8E;peces8E.png"); //Tangram dels vuit elements               codi 34
    dadesTangram.append("t12.fig;Fig_T12;peces12.png"); //Tangram de 12 peces               codi 35
    dadesTangram.append("t5II.fig;Fig_T5II;peces5II.png"); //Tangram 5 II                   codi 36
    dadesTangram.append("ta.fig;Fig_TA;pecesA.png"); //Tangram Armonigrama            codi 37
    dadesTangram.append("trec.fig;Fig_TREC;pecesrec.png"); //Tangram rectangular         codi 38
    dadesTangram.append("t16.fig;Fig_T16;peces16.png"); //T. 16 triangles (del clàssic)  codi 39
    dadesTangram.append("thex.fig;Fig_HEX;pecesHex.png"); //tangram hexagonal (nou 01/09/15)  codi 40

    //ara assignam els valors a les variables de la configuració
    QStringList dades = dadesTangram.value(tipusTangram-1).split(";");
    arxiuDadesFigures=dades.value(0);
    nomJocArbreFigures=dades.value(1);
    nomIcone=dades.value(2);
    //cal assegurar que l'angle de gir de les peces és correcte
    actualitzaAngleGir();
    directoriArxiuDadesFigures=ArxiuDadesFigures();
    }

   }//finals Configuracio::canviaTipusTangram

/**
    * Canvia el nivell de joc al parametre passat
    * @param nivell
    */
    void Configuracio::canviaNivellJoc(int nivell){
    if (nivell!=nivellJoc){
    nivellJoc=nivell;
    configArxiu->setValue("joc/nivellJoc", nivellJoc);
    carregaPuntuacio();
    switch(nivellJoc){
            case NIVELL_INFANTIL:
                angleGir=ROTACIO_INFANTIL;
                mostraSilueta=true;
                nomNivellArbreFigures=NOM_NIVELL_INFANTIL;
                break;
            case NIVELL_PRINCIPIANT:
                angleGir=ROTACIO_INFANTIL;
                mostraSilueta=true;
                nomNivellArbreFigures=NOM_NIVELL_PRINCIPIANT;
                break;
            case NIVELL_INICIAT:
                //Això s'ha canviat respecte a les versions
                //anteriors.
                //Ara en el nivell iniciat, la rotació és lliure
                angleGir=ROTACIO_EXPERT;
                mostraSilueta=true;
                nomNivellArbreFigures=NOM_NIVELL_INICIAT;
                break;
            case NIVELL_EXPERT:
                angleGir=ROTACIO_EXPERT;
                mostraSilueta=false;
                nomNivellArbreFigures=NOM_NIVELL_EXPERT;
                break;
            default:
                nivellJoc=NIVELL_PRINCIPIANT;
                angleGir=ROTACIO_PRINCIPIANT;
                mostraSilueta=true;
                nomNivellArbreFigures=NOM_NIVELL_PRINCIPIANT;
        }
      }
    actualitzaAngleGir();
    }//final de Configuracio::canviaNivellJoc

/*
 Actualitza la propietat angleGirPeca
 */
void Configuracio::actualitzaAngleGir(){
switch(nivellJoc){
  case NIVELL_INFANTIL:
    angleGir=ROTACIO_INFANTIL;
    //algunes figures d'aquests tangrams
    //necessiten això per poder obtenir
    //totes les posicions
    //NOU TANGRAM 4
    if (tipusTangram== 9        //TIPUS_TANGRAM_T10
        || tipusTangram==10     //TIPUS_TANGRAM_T11
      //  || tipusTangram==28     //peces del nou TIPUS_TANGRAM_T9T
        || tipusTangram==15     //TIPUS_TANGRAM_TH
        || tipusTangram==34     //dels vuit elements
        || tipusTangram==21     //TIPUS_TANGRAM_T7I
        || tipusTangram==22     //TIPUS_TANGRAM_TCII
        || tipusTangram==40     //tangram hexagonal de 9 peces. Nou 01/09/15
      //  || tipusTangram==TIPUS_TANGRAM_T9
        ){angleGir=ROTACIO_INICIAT;}

    if (tipusTangram==18   //TIPUS_TANGRAM_STOMACHION
        || tipusTangram==31 //TIPUS_TANGRAM_STOMACHION_II
     //18/04/2014 canviat pel nou sistema de generació de figures
        || tipusTangram==8 //TIPUS_TANGRAM_T9
        || tipusTangram==28     //peces del nou TIPUS_TANGRAM_T9T
        || tipusTangram==32  //TIPUS_TANGRAM_STOMACH
       // || tipusTangram==TIPUS_TANGRAM_T9T
            ){
        angleGir=ROTACIO_INICIAT_2;
    }

    if (tipusTangram==27  //TIPUS_TANGRAM_TSL
        || tipusTangram==29 //TIPUS_TANGRAM_TE
        || tipusTangram==14 //TIPUS_TANGRAM_TB
        || tipusTangram==20 //TIPUS_TANGRAM_TCIRI )
        ){
        angleGir=ROTACIO_SAM_LLOYD;
    }
    break;
 case NIVELL_PRINCIPIANT:
    angleGir=ROTACIO_PRINCIPIANT;
    if (tipusTangram==9  //TIPUS_TANGRAM_T10
        || tipusTangram==10  //TIPUS_TANGRAM_T11
     //18/04/2014 canviat pel nou sistema de generació de figures
     //   || tipusTangram==TIPUS_TANGRAM_T9
     //   || tipusTangram==TIPUS_TANGRAM_T9T
        || tipusTangram==15  //TIPUS_TANGRAM_TH
        || tipusTangram==34 //dels vuit elements
        || tipusTangram==21  //TIPUS_TANGRAM_T7I
        || tipusTangram==22   //TIPUS_TANGRAM_TCII
        || tipusTangram==40     //tangram hexagonal de 9 peces. Nou 01/09/15 ){
            ){
        angleGir=ROTACIO_INICIAT;
    }
    if (tipusTangram==18  //TIPUS_TANGRAM_STOMACHION
        || tipusTangram==31  //TIPUS_TANGRAM_STOMACHION_II
        || tipusTangram==32  //TIPUS_TANGRAM_STOMACH
   //18/04/2014 canviat pel nou sistema de generació de figures
        || tipusTangram==8  //TIPUS_TANGRAM_T9
        || tipusTangram==28  //TIPUS_TANGRAM_T9T
        || tipusTangram==20  //TIPUS_TANGRAM_TCIRI
        ){
        angleGir=ROTACIO_INICIAT_2;
    }
    if (tipusTangram==27  //TIPUS_TANGRAM_TSL
        || tipusTangram==29 //TIPUS_TANGRAM_TE
        || tipusTangram==14  //TIPUS_TANGRAM_TB
        ){
        angleGir=ROTACIO_SAM_LLOYD;
    }
    break;
case NIVELL_INICIAT:
    //Això s'ha canviat respecte a les versions
    //anteriors.
    //Ara en el nivell iniciat, la rotació és lliure
    angleGir=ROTACIO_EXPERT;
    /*
                if (tipusTangram==TIPUS_TANGRAM_T10
                    || tipusTangram==TIPUS_TANGRAM_T11
                    || tipusTangram==TIPUS_TANGRAM_T9
                    || tipusTangram==TIPUS_TANGRAM_TH
                    || tipusTangram==TIPUS_TANGRAM_STOMACHION){
                  angleGir=ROTACIO_INICIAT_2;
                }
                */
    break;
case NIVELL_EXPERT:
    angleGir=ROTACIO_EXPERT;
    break;
default:
    angleGir=ROTACIO_PRINCIPIANT;
}
//qDebug("angleGir-actualitzaAngleGir %f",angleGir);
}
/*
 Mostra la pàgina web de "registre" de l'aplicació
 */
 bool Configuracio::registraAplicacio(){
   return QDesktopServices::openUrl(ADRECA_WEB_REGISTRE);
  }


 /* 07/02/12 v. 5.0
   Retorna el camí complet de l'arxiu de figures.
   Comprova si les figures s'ha de carregar del directori
   «antic» figures o del nou «fig»
   */
 QString Configuracio::ArxiuDadesFigures(){
 QFile arxiu;
 if(arxiu.exists(directoriLinux()+
                 charPathSeparator+"fig"+charPathSeparator+arxiuDadesFigures) ){
     return directoriLinux()+
             charPathSeparator+"fig"+charPathSeparator+arxiuDadesFigures;}
 else return directoriLinux()+
             charPathSeparator+"figures"+charPathSeparator+arxiuDadesFigures;

 }
