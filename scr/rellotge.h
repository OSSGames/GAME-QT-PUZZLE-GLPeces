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
#ifndef RELLOTGE_H
#define RELLOTGE_H

#include <QtGui>
#include <QtWidgets/QLabel>


class QTimer;

class Rellotge : public QLabel
{
    Q_OBJECT

public:
    Rellotge(QWidget * parent = 0, Qt::WindowFlags f = 0 );

    void iniciaRellotge();
    void aturaRellotge();
    /*
      Si tipus=0 retorna el temps amb format
      tipus=1 retorna nomès el temps
    */
    QString retornaTemps(int tipus=0);
    void estableixTemps(QString temps);

    //Retorna l'estat del rellotge
    bool rellotgeEnMarxa();

private slots:
    void actualitzaRellotge();

private:
    QTimer *rellotgeQTimer;
    int hores,minuts,segons;

bool p_rellotgeEnMarxa;

};

#endif // RELLOTGE_H
