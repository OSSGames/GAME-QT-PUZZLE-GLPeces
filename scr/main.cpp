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
#include <QtWidgets/QApplication>
#include <QtWidgets/QSplashScreen>

#include "frmPrincipal.h"
#include "funcions.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
        app.setApplicationName("Peces (Tangram)");
        app.setApplicationVersion("5.2");
        app.setOrganizationName("I. De Marchi");

        //Això pels accents a les cadenes
        // 17/01/16 Innecessari per a Qt5?
       /* QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForTr(linuxCodec);
        QTextCodec::setCodecForCStrings(linuxCodec);
        QTextCodec::setCodecForLocale(linuxCodec);*/

     QPixmap pixmap(directoriLinux()+QDir().separator()+"images"+QDir().separator()+"present-1.png");

     QSplashScreen splash(pixmap);
     splash.show();
    // app.processEvents();
     //Sense aquesta línia,la imatge no s'actualitza
     splash.showMessage("");

    //Això per a que surtin traduïdes les cadenes dels formularis predefinits Qt
    //...si tenen instal·lats els arxius .qm
    // QString translatorFileName = QLatin1String("qt_");
    // translatorFileName += QLocale::system().name();

//qCritical("%s", qPrintable(QLocale::system().name()));
//qCritical("%s", qPrintable(QLibraryInfo::location(QLibraryInfo::TranslationsPath)));

   /*  QTranslator *translator = new QTranslator(&app);
    if (translator->load(translatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath))){
     app.installTranslator(translator);}*/
    //15/02/15 A windows cal posar
    /*if (translator->load(translatorFileName, QApplication::applicationDirPath()))
     app.installTranslator(translator);*/
    formulariPrincipal *frmPrincipal=new formulariPrincipal();
    frmPrincipal->show();


    splash.finish(frmPrincipal);
    return app.exec();
}
