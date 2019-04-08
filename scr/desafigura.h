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
#ifndef DESAFIGURA_H
#define DESAFIGURA_H

#include <QtWidgets/QDialog>
#include "ui_frmDesaFigura.h"
#include "configuracio.h"


class DesaFigura : public QDialog , private Ui::DesaFigura
{
      Q_OBJECT

public:
    DesaFigura(Configuracio *configuracioJoc, int dificultat, QWidget *parent = 0);

    QString dadesFigura;
    QString directoriSeleccionat;
    int nivellDificultat;

private slots:
    void accept();
  //  void reject();
    void seleccionaDirectoriSlot();

private:
    Configuracio *configJoc;
    void tradueixGUI();
    void directoriFigures();

};

#endif // DESAFIGURA_H
