/********************************************************************************
** Form generated from reading UI file 'frmDesaFigura.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMDESAFIGURA_H
#define UI_FRMDESAFIGURA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DesaFigura
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *leNomFigura;
    QLabel *lbNomFigura;
    QComboBox *cmbDificultad;
    QLabel *lbDificultad;
    QPushButton *pbDirectori;
    QCheckBox *chkFiguraCompacte;

    void setupUi(QDialog *DesaFigura)
    {
        if (DesaFigura->objectName().isEmpty())
            DesaFigura->setObjectName(QString::fromUtf8("DesaFigura"));
        DesaFigura->resize(400, 250);
        DesaFigura->setMinimumSize(QSize(400, 250));
        DesaFigura->setMaximumSize(QSize(400, 250));
        buttonBox = new QDialogButtonBox(DesaFigura);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(220, 210, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);
        leNomFigura = new QLineEdit(DesaFigura);
        leNomFigura->setObjectName(QString::fromUtf8("leNomFigura"));
        leNomFigura->setGeometry(QRect(20, 40, 321, 31));
        lbNomFigura = new QLabel(DesaFigura);
        lbNomFigura->setObjectName(QString::fromUtf8("lbNomFigura"));
        lbNomFigura->setGeometry(QRect(30, 20, 201, 20));
        cmbDificultad = new QComboBox(DesaFigura);
        cmbDificultad->setObjectName(QString::fromUtf8("cmbDificultad"));
        cmbDificultad->setGeometry(QRect(20, 110, 311, 30));
        lbDificultad = new QLabel(DesaFigura);
        lbDificultad->setObjectName(QString::fromUtf8("lbDificultad"));
        lbDificultad->setGeometry(QRect(30, 80, 231, 20));
        pbDirectori = new QPushButton(DesaFigura);
        pbDirectori->setObjectName(QString::fromUtf8("pbDirectori"));
        pbDirectori->setGeometry(QRect(20, 210, 31, 29));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/directori.png"), QSize(), QIcon::Normal, QIcon::Off);
        pbDirectori->setIcon(icon);
        pbDirectori->setAutoDefault(false);
        pbDirectori->setFlat(true);
        chkFiguraCompacte = new QCheckBox(DesaFigura);
        chkFiguraCompacte->setObjectName(QString::fromUtf8("chkFiguraCompacte"));
        chkFiguraCompacte->setGeometry(QRect(30, 160, 171, 23));
        chkFiguraCompacte->setChecked(false);

        retranslateUi(DesaFigura);
        QObject::connect(buttonBox, SIGNAL(accepted()), DesaFigura, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DesaFigura, SLOT(reject()));

        QMetaObject::connectSlotsByName(DesaFigura);
    } // setupUi

    void retranslateUi(QDialog *DesaFigura)
    {
        DesaFigura->setWindowTitle(QApplication::translate("DesaFigura", "Dialog", 0));
        lbNomFigura->setText(QApplication::translate("DesaFigura", "Nom de la nova figura:", 0));
        lbDificultad->setText(QApplication::translate("DesaFigura", "Dificultad de la figura:", 0));
        pbDirectori->setText(QString());
        chkFiguraCompacte->setText(QApplication::translate("DesaFigura", "Figura compacte", 0));
    } // retranslateUi

};

namespace Ui {
    class DesaFigura: public Ui_DesaFigura {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMDESAFIGURA_H
