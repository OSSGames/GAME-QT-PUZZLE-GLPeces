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
#ifndef CONFIGURACIO_H
#define CONFIGURACIO_H

#include <QtGui>
#include <QObject>
#include "constants.h"
 class QSettings;

class Configuracio : public QObject
{
public:
    /*
      Constructor per defecte
    */
    Configuracio();

    /*
      Constructor amb el tipus de tangram i nivell
    */
    Configuracio(int tipusTangram,int nivellJoc);
    /**
     * Canvia el tipus de tangram segons el paràmetre passat
     * @param tipus
     */
   void canviaTipusTangram(int tipus);
    /**
    * Canvia el nivell de joc al parametre passat
    * @param nivell
    */
    void canviaNivellJoc(int nivell);
    /*
      Actualitza la propietat angleGirPeca
    */
    void actualitzaAngleGir();

    void carregaPuntuacio();
    void desaPuntuacio(QString pTemps);


    /**
         * Tipus de tangram que es juga
         * Per defecte el tangram classic
         */
        int tipusTangram;
        /**
         * Nivell del joc
         * Per defecte principiant
         */
        int nivellJoc;
        /**
         * Angle de rotació. Depen del nivell
         * Per defecte 1 graus
         */
        double angleGir;
        /**
         * Determinan si s'ha de mostrar la silueta de la figura en joc
         */
        bool mostraSilueta;
        /*
          Determina si està activada l'opció
          Col·loca peça
        */
        bool colocaPeca;

        /*
          Controla si cal desar la puntuació
          (nomès es desarà amb les figures del programa,
          no amb les figures personals o els concursos
        */
        bool desaRecords;
         /**
         * Cadena amb el nom del joc actual.
         * Per defecte, "figures del tangram xinès"
         */
        QString nomJocArbreFigures;

        /**
         * Cadena amb el nom del nivell del joc actual
         */
        QString nomNivellArbreFigures;

        /*
          Cadena amb el nom de l'icone
        */
        QString nomIcone;

        /**
         * Nom del paquet de dades de les figures actuals
         */
        QString arxiuDadesFigures;

        QString directoriArxiuDadesFigures;

        /* 07/02/12 v. 5.0
          Retorna el camí complet de l'arxiu de figures.
          */
        QString ArxiuDadesFigures();

        /*
         Gestiona la conservació de la configuració a un arxiu
        */
        QSettings *configArxiu;

        /*
        Gestiona la traducció
        */
        QSettings *arxiuTraduccio;
        /*
         Tradueix la cadena passada
         */
        QString tradueixCadena(QString cadena);

        /*
          Retorna l'arxiu de traducció adequat
         */
        QString arxiuLocale();
        /*
          Retorna les lletres del locale
        */
        QString strLocale();

        void canviaArxiuLocale(QString strLocale);

        QString charPathSeparator;


    int comptadorFigures;
    int comptadorPunts;
    QString temps;

    //Llista amb les dades per construir
    //les peces. Les dades es carreguen de l'arxiu
    //figures/peces.dat. L'índex+1 a la
    //llista és el codi de la peça
    QStringList dadesPeces;


  private:
    /*
    Mostra la pàgina web de "registre" de l'aplicació
    */
      bool registraAplicacio();


};

#endif // CONFIGURACIO_H
