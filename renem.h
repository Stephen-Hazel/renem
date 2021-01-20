#ifndef RENEM_H
#define RENEM_H

#include <QApplication>
#include <QMainWindow>
#include "ui_renem.h"
#include "../stv/ui.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RenEm; }
QT_END_NAMESPACE

class RenEm: public QMainWindow {
   Q_OBJECT
private:
   Ui::RenEm *ui;

public:
   RenEm (QWidget *par = nullptr)
   : QMainWindow (par), ui (new Ui::RenEm)
   {  ui->setupUi (this);  }

  ~RenEm ()  {delete ui;}

   void Open (), Shut ();
public slots:
   void Dir ();
   void Go ();
};
#endif // RENEM_H
