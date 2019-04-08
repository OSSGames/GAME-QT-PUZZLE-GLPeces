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
#ifndef CREACONCURS_H
#define CREACONCURS_H

#include <QtWidgets/QDialog>
#include <QtGui>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGraphicsView>

#include "conjuntpeces.h"
#include "constants.h"
#include "configuracio.h"



class CreaConcurs : public QDialog
{
    Q_OBJECT

public:
    CreaConcurs(Configuracio *configJoc, QWidget *parent = 0);

    QGraphicsScene *sceneMostra;
    QGraphicsView *viewMostra;

    QFrame *botonsFrame;
    QPushButton *afegeixFigura ;
    QPushButton *eliminaFigura;
    QPushButton *eliminaTotesFigures;
    QPushButton *concursAtzar;
    QPushButton *obreConcurs ;
    QPushButton *desaConcurs ;
    QPushButton *tanca ;

    QTreeWidget *arbreFiguresTreeWidget;
    QTreeWidget *arbreConcursTreeWidget;

    ConjuntPeces *conjuntMostra;

    Configuracio *configuracioJoc;

    QString charPathSeparator;


private slots:
    // void closeEvent(QCloseEvent *event);

    /*
     Gestiona la selecció d'una figura a l'arbre
     (conectat amb arbreFiguresTreeWidget::itemSelectionChanged ()
    */
    void seleccioFiguraJoc();
    void seleccioFiguraConcurs();
    void seleccioFigura(QTreeWidget *arbre);

    //botons
    void afegeixFiguraSlot() ;
    void eliminaFiguraSlot();
    void eliminaTotesFiguresSlot();
    void concursAtzarSlot();
    void obreConcursSlot() ;
    void desaConcursSlot() ;
    void tancaSlot() ;

private:

    //Carrega les figures als arbres
    void carregaArbreFigures(QTreeWidget *arbre, QString nomArxiu);
    void llegeixArxiuFigures(QTreeWidget *arbre, QString nomArxiu);


    void creaTangram(int tipusTangram);

    void actualitzaTextItemArrel();

    void tradueixGUI();
};

#endif // CREACONCURS_H
