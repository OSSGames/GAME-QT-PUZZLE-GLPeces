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
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>
#include <math.h>
#include <QColor>


static const QString CONTEXTE_DiagramWindow="DiagramWindow";

/*
CONSTANTS DELS TIPUS DE TANGRAM I PECES
***************************************
darrer tangram 33
darrera peca    96
*/

//Tangram de Brugner
    // const int TIPUS_TANGRAM_TB=14;
     //  const int TB_TRIANGLE_GRAN=28;
    //   const int TB_TRIANGLE_MITJA=29;
    //   const int TB_TRIANGLE_PETIT=30;
    //   const QString NOM_ARXIU_TRANGRAM_B="tb.fig";
    //   const QString ARBRE_FIGURES_TANGRAM_B="Fig_TB";
    //   const QString TB_ICONE="peces_b.png";

//Tangram de 4 peces I
    // const int TIPUS_TANGRAM_T4=19;
    //   const int T4_TRAPEZI_GRAN=56;
     //es igual al T7_TRIANGLE_PETIT
     //  const int T4_TRIANGLE=57;
    //   const int T4_TRAPEZI_PETIT=58;
   //    const int T4_PECA_GRAN=59;
     //  const QString NOM_ARXIU_TRANGRAM_4="t4.fig";
     //  const QString ARBRE_FIGURES_TANGRAM_4="Fig_T4";
     //  const QString T4_ICONE="peces4.png";

//Tangram de 4 peces II
    // const int TIPUS_TANGRAM_T4II=30;
    //   const int T4II_TRAPEZI_1=85;
    //   const int T4II_TRAPEZI_2=86;
    //   const QString NOM_ARXIU_TRANGRAM_4II="t4II.fig";
    //   const QString ARBRE_FIGURES_TANGRAM_4II="Fig_T4II";
    //   const QString T4II_ICONE="peces4II.png";



//Tangram 5
   // const int TIPUS_TANGRAM_T5=2;
  //     const int T5_TRIANGLE_PETIT=2;
  //     const int T5_TRIANGLE_MITJA=4;
  //     const int T5_TRIANGLE_GRAN=6;
  //     const int T5_TRAPEZI=7;
     //  const QString NOM_ARXIU_TRANGRAM_5="t5.fig";
     //  const QString ARBRE_FIGURES_TANGRAM_5="Fig_T5";
    // const QString T5_ICONE="peces5.png";

//Tangram de Sam Lloyds
  //  const int TIPUS_TANGRAM_TSL=27;
  //     const int TSL_TRIANGLE_PETIT=74;
   //    const int TSL_TRIANGLE_GRAN=75;
  //     const int TSL_TRAPEZI=76;
  //     const int TSL_HEXAGON=77;
  //     const int TSL_QUADRAT=78;
     //  const QString NOM_ARXIU_TRANGRAM_SL="tsl.fig";
     //  const QString ARBRE_FIGURES_TANGRAM_SL="Fig_TSL";
    // const QString TSL_ICONE="pecesSL.png";

//Tangram 5 I Regulus
   //  const int TIPUS_TANGRAM_TREGULUS=25;
   //    const QString NOM_ARXIU_TRANGRAM_REGULUS="treg.fig";
   //    const QString ARBRE_FIGURES_TANGRAM_REGULUS="Fig_REG";
   //    const QString TREG_ICONE="pecesREG.png";


//Tangram 6 I
   //  const int TIPUS_TANGRAM_T6=3;
  //     const int T6_TRIANGLE_PETIT=10;
  //     const int T6_TRIANGLE_MITJA=3;
  //     const int T6_TRIANGLE_GRAN=11;
  //     const int T6_QUADRAT=8;
  //     const int T6_TRAPEZI=9;
   //    const QString NOM_ARXIU_TRANGRAM_6="t6.fig";
   //    const QString ARBRE_FIGURES_TANGRAM_6="Fig_T6";
   //    const QString T6_ICONE="peces6.png";

//Tangram 6 II
   //  const int TIPUS_TANGRAM_T6II=23;
   //    const int T6II_DOS_TRIANGLES=70;
  //     const int T6II_TRIANGLE_TRAPEZI=71;
  //     const int T6II_QUADRAT_TRIANGLE=72;
    //   const QString NOM_ARXIU_TRANGRAM_6II="t6II.fig";
    //   const QString ARBRE_FIGURES_TANGRAM_6II="Fig_T6II";
    //   const QString T6II_ICONE="peces6II.png";

//Tangram Hexagrama
     //Els mòduls d'aquestes peces estan *2
     //per que quedin més grans
   //  const int TIPUS_TANGRAM_TH=15;
       //és el mateix rombe que el del Peces 10
  //     const int TH_ROMBE_GRAN=31;
 //      const int TH_ROMBE_PETIT=32;
       //és igual que l'equilàter del peces 10
  //     const int TH_TRIANGLE_EQUILATER_PETIT=33;
  //     const int TH_TRIANGLE_EQUILATER_MINUSCUL=34;
  //     const int TH_HEXAGON=35;
  //     const int TH_TRAPEZI=36;
    //   const QString NOM_ARXIU_TRANGRAM_H="th.fig";
    //   const QString ARBRE_FIGURES_TANGRAM_H="Fig_TH";
    //   const QString TH_ICONE="peces_h.png";

//Tipus de tangram clàssic
   // const int TIPUS_TANGRAM_T7_CLASSIC=1;

  //   const int T7_QUADRAT=1;
  //   const int T7_TRIANGLE_PETIT=2;
  //   const int T7_TRIANGLE_MITJA=3;
  //   const int T7_TRIANGLE_GRAN=4;
 //    const int T7_TRAPEZI=5;
  //   const QString NOM_ARXIU_TRANGRAM_CLASSIC="t7.fig";
     //Nom de la clau a l'arxiu de traducció
     //del nom del tangram per l'arbre
  //   const QString ARBRE_FIGURES_TANGRAM_7="Fig_T7";
  //   const QString T7_ICONE="pecesset.png";

//Tangram de Fletcher també de Pitàgores
    // const int TIPUS_TANGRAM_TF=4;
  //     const int TF_TRAPEZI=12;
  //     const QString NOM_ARXIU_TRANGRAM_F="tf.fig";
  //     const QString ARBRE_FIGURES_TANGRAM_F="Fig_TF";
  //     const QString TF_ICONE="peces_f.png";

//Tangram 7 I
  //   const int TIPUS_TANGRAM_T7I=21;
  //     const int T7I_TRAPEZI=64;
  //     const int T7I_PARALELOGRAM=65;
  //     const int T7I_PENTAGON=66;
  //     const QString NOM_ARXIU_TRANGRAM_7I="t7I.fig";
  //     const QString ARBRE_FIGURES_TANGRAM_7I="Fig_T7I";
  //     const QString T7I_ICONE="peces7I.png";

//Tangram 7 II Revathi
  //   const int TIPUS_TANGRAM_T7II=24;
  //     const int T7II_TRAPEZI=73;
 //      const QString NOM_ARXIU_TRANGRAM_7II="t7II.fig";
 //      const QString ARBRE_FIGURES_TANGRAM_7II="Fig_T7II";
 //      const QString T7II_ICONE="peces7II.png";

//Tangram 7 Heptex
    // const int TIPUS_TANGRAM_THEP=26;
   //    const QString NOM_ARXIU_TRANGRAM_HEP="thep.fig";
   //    const QString ARBRE_FIGURES_TANGRAM_HEP="Fig_THEP";
   //    const QString THEP_ICONE="pecesHEP.png";

//Tangram del xinès també Tangram Pitagòric
   //  const int TIPUS_TANGRAM_TX=5;
 //      const int TX_TRAPEZI_MITJA=13;
 //      const int TX_TRAPEZI_GRAN=14;
 //      const int TX_TRAPEZI_PETIT=15;
 //      const int TX_PENTAGON=16;
 //      const int TX_TRIANGLE=3;
  //     const QString NOM_ARXIU_TRANGRAM_X="tx.fig";
  //     const QString ARBRE_FIGURES_TANGRAM_X="Fig_TX";
  //     const QString TX_ICONE="peces_x.png";

//Tangram de la saviesa Chie-No-Ita
   //  const int TIPUS_TANGRAM_TS=6;
 //      const int TS_TRAPEZI=17;
  //     const QString NOM_ARXIU_TRANGRAM_S="ts.fig";
  //     const QString ARBRE_FIGURES_TANGRAM_S="Fig_TS";
  //     const QString TS_ICONE="peces_s.png";

//Tangram 8
   //  const int TIPUS_TANGRAM_T8=7;
 //      const QString NOM_ARXIU_TRANGRAM_8="t8.fig";
 //      const QString ARBRE_FIGURES_TANGRAM_8="Fig_T8";
 //      const QString T8_ICONE="peces8.png";

//Tangram circular de 8 peces CII
  //   const int TIPUS_TANGRAM_TCII=22;
  //     const int TCII_TRIANGLE_GRAN=67;
  //     const int TCII_TRIANGLE_ISOSCELES_ARC=68;
  //     const int TCII_ARC=69;
  //     const QString NOM_ARXIU_TRANGRAM_CII="tCII.fig";
  //     const QString ARBRE_FIGURES_TANGRAM_CII="Fig_TCII";
  //     const QString TCII_ICONE="pecesCII.png";


//Tangram 9
   //  const int TIPUS_TANGRAM_T9=8;
  //     const int T9_TRIANGLE_EQUILATER=18;
  //     const int T9_TRIANGLE_RECTANGLE=19;
  //     const int T9_ROMBE=20;
   //    const QString NOM_ARXIU_TRANGRAM_9="t9.fig";
   //    const QString ARBRE_FIGURES_TANGRAM_9="Fig_T9";
   //    const QString T9_ICONE="peces9.png";

//Tangram "los nueve tangram"
   //  const int TIPUS_TANGRAM_T9T=28;
  //     const int T9T_TRIANGLE_ISOSCELES_PETIT=79;
  //     const int T9T_TRIANGLE_ISOSCELES_GRAN=80;
  //     const int T9T_TRIANGLE_RECTANGLE_PETIT=81;
  //     const int T9T_TRIANGLE_RECTANGLE_GRAN_1=82;
  //     const int T9T_TRIANGLE_RECTANGLE_GRAN_2=83;
  //     const int T9T_TRAPEZI=84;
  //     const QString NOM_ARXIU_TRANGRAM_9T="t9t.fig";
  //     const QString ARBRE_FIGURES_TANGRAM_9T="Fig_T9T";
  //     const QString T9T_ICONE="peces9t.png";
       const double FACTOR_T9T=1.5;

//Tangram de l'ou
  //   const int TIPUS_TANGRAM_TOU=16;
 //      const int TOU_TRIANGLE_RECTANGLE_GRAN_ARC=37;
 //      const int TOU_TRIANGLE_ISOSCELES_ARC=38;
 //      const int TOU_TRIANGLE_ISOSCELES=39;
 //      const int TOU_TRAPEZI_ARC=40;
 //      const int TOU_TRIANGLE_ISOSCELES_PETIT=41;
 //      const QString NOM_ARXIU_TRANGRAM_OU="tou.fig";
 //      const QString ARBRE_FIGURES_TANGRAM_OU="Fig_TOU";
 //      const QString TOU_ICONE="peces_ou.png";

//Tangram del cor
  //   const int TIPUS_TANGRAM_COR=17;
 //      const int TCOR_TRIANGLE_ISOSCELES_ARC=42;
 //      const int TCOR_TRIANGLE_ISOSCELES_PETIT_ARC=43;
    //   const QString NOM_ARXIU_TRANGRAM_COR="tc.fig";
    //   const QString ARBRE_FIGURES_TANGRAM_COR="Fig_COR";
    //   const QString TCOR_ICONE="peces_c.png";


//Tangram 10
   //  const int TIPUS_TANGRAM_T10=9;
  //     const int T10_TRIANGLE_EQUILATER_GRAN=21;
 //      const int T10_TRIANGLE_EQUILATER_PETIT=22;
 //      const int T10_ROMBE=23;
  //     const QString NOM_ARXIU_TRANGRAM_10="t10.fig";
  //     const QString ARBRE_FIGURES_TANGRAM_10="Fig_T10";
  //     const QString T10_ICONE="peces10.png";

//Tangram Egipci
  //  const int TIPUS_TANGRAM_TE=29;
  //     const QString NOM_ARXIU_TRANGRAM_E="te.fig";
  //     const QString ARBRE_FIGURES_TANGRAM_E="Fig_TE";
  //   const QString TE_ICONE="pecese.png";

  //Tangram Elzzup (10 peces)
  //  const int TIPUS_TANGRAM_EL=33;
//       const QString NOM_ARXIU_TRANGRAM_EL="el.fig";
//       const QString ARBRE_FIGURES_TANGRAM_EL="Fig_EL";
//     const QString EL_ICONE="pecesel.png";
     
//Tangram circular I
   //  const int TIPUS_TANGRAM_TCIRI=20;
  //     const int TCIRI_TRIANGLE=60;
 //      const int TCIRI_TRIANGLE_ARC=61;
  //     const int TCIRI_TRIANGLE_OBTUS_ARC=62;
 //      const int TCIRI_TRIANGLE_OBTUS_GRAN_ARC=63;
  //     const QString NOM_ARXIU_TRANGRAM_CIRI="tcI.fig";
  //     const QString ARBRE_FIGURES_TANGRAM_CIRI="Fig_TCIRI";
  //     const QString TCIRI_ICONE="pecesCI.png";

//Tangram 11
  //   const int TIPUS_TANGRAM_T11=10;
 //    const int T11_TRIANGLE_RECTANGLE_GRAN=24;
//     const int T11_TRIANGLE_OBTUS=25;
 //      const QString NOM_ARXIU_TRANGRAM_11="t11.fig";
 //      const QString ARBRE_FIGURES_TANGRAM_11="Fig_T11";
 //      const QString T11_ICONE="peces11.png";
    //   const double FACTOR_T11=1.0;

//Tangram rus
 //    const int TIPUS_TANGRAM_TR=11;
//     const int TR_TRIANGLE_MOLT_PETIT=26;
 //    const int TR_TRAPEZI=27;
 //      const QString NOM_ARXIU_TRANGRAM_R="tr.fig";
 //      const QString ARBRE_FIGURES_TANGRAM_R="Fig_TR";
 //      const QString TR_ICONE="peces_r.png";
       const double FACTOR_TR=1.2;

//Tangram 13
 //    const int TIPUS_TANGRAM_T13=12;
//       const QString NOM_ARXIU_TRANGRAM_13="t13.fig";
//       const QString ARBRE_FIGURES_TANGRAM_13="Fig_T13";
//       const QString T13_ICONE="peces13.png";

//Tangram 14
   //  const int TIPUS_TANGRAM_T14=13;
  //     const QString NOM_ARXIU_TRANGRAM_14="t14.fig";
  //     const QString ARBRE_FIGURES_TANGRAM_14="Fig_T14";
  //     const QString T14_ICONE="peces14.png";

//Tangram Stomach (modificació de Stomachion)
 //    const int TIPUS_TANGRAM_STOMACH=32;
//       const QString NOM_ARXIU_TRANGRAM_STOMACH="tstIII.fig";
//       const QString ARBRE_FIGURES_TANGRAM_STOMACH="Fig_STIII";
//       const QString TSTIII_ICONE="pecesSTOIII.png";
       //Especial adaptar la peça TSTII_TRIANGLE_4 (Stomachion II)
       //en aquest joc
       const double escalaStomach=1.175;

//Tangram Stomachion o Luculus Archimedius
  //   const int TIPUS_TANGRAM_STOMACHION=18;
 //      const int TST_TRIANGLE_RECTANGLE=44;
//       const int TST_TRIANGLE_1=45;
//       const int TST_TRIANGLE_RECTANGLE_GRAN=46;
//       const int TST_TRIANGLE_2=47;
//       const int TST_TRIANGLE_3=48;
//       const int TST_TRIANGLE_4=49;
//       const int TST_TRIANGLE_5=50;
//       const int TST_TRIANGLE_6=51;
//       const int TST_TRIANGLE_7=52;
//       const int TST_PENTAGON_8=53;
//       const int TST_QUADRILATER_9=54;
 //      const int TST_QUADRILATER_10=55;
//       const QString NOM_ARXIU_TRANGRAM_STOMACHION="tst.fig";
//       const QString ARBRE_FIGURES_TANGRAM_STOMACHION="Fig_ST";
       //Especial per les peces del tangram Stomachion
       const double escalaStomachion=1.1;
//       const QString TST_ICONE="peces_sto.png";

//Tangram Stomachion modificat
 //    const int TIPUS_TANGRAM_STOMACHION_II=31;
//       const int TSTII_QUADRILATER_1=87;
//       const int TSTII_QUADRILATER_2=88;
//       const int TSTII_TRIANGLE_RECT_1=89;
//       const int TSTII_TRIANGLE_4=90;
//       const int TSTII_TRIANGLE_5=91;
//       const int TSTII_PENTAGON=92;
//       const int TSTII_TRIANGLE_7=93;
//       const int TSTII_TRIANGLE_8=94;
//       const int TSTII_TRIANGLE_9=95;
//       const QString NOM_ARXIU_TRANGRAM_STOMACHION_II="tstII.fig";
//       const QString ARBRE_FIGURES_TANGRAM_STOMACHION_II="Fig_STII";
//       const QString TSTII_ICONE="pecesSTOII.png";


/*
CONSTANTS GENERALS
***************************************
*/
static const QColor COLOR_FIG_RESOLTA=Qt::green;

static const float PI = 3.14159265358979323846264338327950288419717;
        //escala per defecte
static const float ESCALA_GENERAL = 58.0 ;
static const float ESCALA_MOSTRA =  15.0;

static const int PERCENTATGE_COINCIDENCIA_SOLUCIO=95;
static const double DISTANCIA_PUNTS_SOLUCIO=3.0;
/*13/01/12
  Amb els canvis en la comparació de figures,
  s'ha pujat aquest valor
  (estava a 90.0)
  06/12/15
  Pujat a 97.5
  */
//static const float PERCENTATGE_COINCIDENCIA_BUSCAR_FIGURA_antic =  99.5;
static const float PERCENTATGE_COINCIDENCIA_BUSCAR_FIGURA =  90.0;

/*
  Nivells del joc
 (No pot començar per 0 ja que es fa servir per calcular
  la puntuació de cada figura)
*/
static const int NIVELL_INFANTIL=1;
static const int NIVELL_PRINCIPIANT=2;
static const int NIVELL_INICIAT=3;
static const int NIVELL_EXPERT=4;


static const QString NOM_NIVELL_INFANTIL="nivell.infantil";
static const QString NOM_NIVELL_PRINCIPIANT="nivell.principiant";
static const QString NOM_NIVELL_INICIAT="nivell.iniciat";
static const QString NOM_NIVELL_EXPERT="nivell.expert";




      //Angle de rotacio (en graus) de les Peces
     //(segons el nivell de joc)
     static const double ROTACIO_INFANTIL=45.0;
     static const double ROTACIO_PRINCIPIANT=45.0;
     static const double ROTACIO_INICIAT=15.0;
     static const double ROTACIO_INICIAT_2=5.0;
     static const double ROTACIO_EXPERT=1.0;
     static const double ROTACIO_SAM_LLOYD=3.0 ;//24.09484255211070096707;

/*
  Estats del joc
  Controla els diversos estats en què es pot trobar el joc
*/
   enum EstatsDelJoc {
        Inactiu = 0x0,
        FiguraCarregada = 0x1,
        JocIniciat = 0x2,
        FiguraResolta = 0x4,
        MostraSolucio = 0x5
     };

/**
 * Constants per les peces del tangram de Brugner
 */
static const float a=3.236068;
static const float b=2.5440393;
static const float c=2.0;
static const float d=1.5723027;
static const float e=1.236068;

/**
 * Constant pel tangram de l'ou
 */
static const float o=2.0-sqrt(2.0);

/*
  Transparència de la peça seleccionada
*/
static const qreal transparencia=0.4;

/*
  Adreça de registre de l'aplicació
*/
//const QString ADRECA_WEB_REGISTRE="http://www.mallorcaweb.net/tangrampeces/applet/provesappletpeces.html";
const QString ADRECA_WEB_REGISTRE="http://pecesjocdetangr.sourceforge.net/jpeces_reg.html";
const QString ADRECA_WEB_PROGRAMA="http://pecesjocdetangr.sourceforge.net/";
const QString ADRECA_CORREU="tangram.peces@gmail.com";

/*
  Directoris
21/04/11 Eliminat per innecessari. Només es fa servir
a funcions.directoriLinux()
const QString DIRECTORIS_LINUX="/usr/share/games/glpeces";
const QString DIRECTORIS_LINUX_PROGRAMA="/games";
const QString DIRECTORIS_LINUX_AUXILIARS="/share/games/glpeces";
*/

const QString DIR_HELP="help";

const QString DIR_ARXIUS_FIGURES_PERSONALS="glpeces";


#endif // CONSTANTS_H
