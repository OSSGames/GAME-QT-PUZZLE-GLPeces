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
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include "desafigura.h"
#include "funcions.h"

DesaFigura::DesaFigura( Configuracio *configuracioJoc,int dificultat, QWidget *parent)
        : QDialog(parent)
{
     setupUi(this);
     configJoc=configuracioJoc;

     connect(pbDirectori, SIGNAL(clicked()), this, SLOT(seleccionaDirectoriSlot()));
     QStringList items;
     items << configJoc->tradueixCadena("dificultat.baixa")
           << configJoc->tradueixCadena("dificultat.mitjana")
           << configJoc->tradueixCadena("dificultat.alta");
    // nivellDificultat=1;
     cmbDificultad->addItems(items);
     cmbDificultad->setCurrentIndex(dificultat);
    // qDebug("nivellDificultat %d",dificultat);
     tradueixGUI();
//seleccionam el directori per defecte per les figures
directoriFigures();

leNomFigura->setFocus(Qt::OtherFocusReason);
}

void DesaFigura::seleccionaDirectoriSlot(){
    QFileDialog::Options opcionsDialeg = QFileDialog::DontResolveSymlinks |
                                         QFileDialog::ShowDirsOnly;
    directoriFigures();
    directoriSeleccionat = QFileDialog::getExistingDirectory(this,
                                 configJoc->tradueixCadena("selecciona.directori"),
                                 directoriSeleccionat,
                                 opcionsDialeg);
}

void DesaFigura::accept(){    
    if (directoriSeleccionat.isEmpty()){
       //seleccionam el directori per defecte per les figures
       directoriFigures();
    }
    QDir dirArxiu(directoriSeleccionat);
    QFile arxiu(dirArxiu.filePath(configJoc->arxiuDadesFigures));
    if (!arxiu.open(QIODevice::Append)){
        QMessageBox::information(this, configJoc->tradueixCadena("desa.dades.figura"),
                                 configJoc->tradueixCadena("nova.figura.error")+"\n"+
                                 configJoc->tradueixCadena("arxiuNoCreat").arg(arxiu.fileName()) );
    }
    else {
    //desam les dades de la figura
    QTextStream entrada(&arxiu);
    //nom de la figura
    if(leNomFigura->text().isEmpty()){leNomFigura->setText("Figura.001");}
    entrada<<(QString("%1\n").arg(leNomFigura->text()));
    entrada<<(QString("%1\n").arg(dadesFigura));
    QString figuraComposta;
    if (chkFiguraCompacte->isChecked()){
        figuraComposta="1";}
    else figuraComposta="0";
    entrada<<(QString("%1 %2 %3\n").arg(configJoc->nivellJoc).arg(cmbDificultad->currentIndex()+1).arg(figuraComposta));
    arxiu.close();
    QMessageBox::information(this, configJoc->tradueixCadena("desa.dades.figura"),
                                 configJoc->tradueixCadena("nova.figura.desada")+"\n"+
                                 arxiu.fileName() );
    }
    //tancam el formulari
    close();
}

/*
void DesaFigura::reject(){
   close();
   // destroy();
}*/

void DesaFigura::directoriFigures(){
   directoriSeleccionat =directoriFiguresPersonals();
}

void DesaFigura::tradueixGUI(){
    setWindowTitle(configJoc->tradueixCadena("desa.dades.figura"));
    lbNomFigura->setText(configJoc->tradueixCadena("label.nom.figura"));
    lbDificultad->setText(configJoc->tradueixCadena("Dificultat_de_la_figura"));
    pbDirectori->setToolTip(configJoc->tradueixCadena("selecciona.directori"));
    chkFiguraCompacte->setText(configJoc->tradueixCadena("figura.compacte"));
}
