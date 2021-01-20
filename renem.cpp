// RenEm.cpp - rename a buncha files at once via text editor

#include "renem.h"

TStr Top, FNm [128*1024];  ulong NFNm;
TStr      FN2 [128*1024];
File Fo;


bool DoDir (void *ptr, char dfx, char *fn)
// find any files and put em in F[]
{  (void)ptr;
   if ((dfx == 'f') && (NFNm < BITS (FNm)))  StrCp (FNm [NFNm++], fn);
   return false;
}


char *DoRec (char *buf, uword len, ulong pos, void *ptr)
// parse a record of the file
{ PStr rec;
   (void)ptr;   (void)len;
   FN2 [pos][0] = '\0';
   if (StrCm (buf, & FNm [pos][StrLn (Top)+1], 'x')) {
      Fo.Put (StrFmt (rec, "'`s' => '`s/`s'\n", FNm [pos], Top, buf));
      StrFmt (FN2 [pos], "`s/`s", Top, buf);
   }
   return NULL;
}


void RenEm::Go ()
{ File  f;
  TStr  fn, buf, ed, cmd;
  char *p;
  ulong i, b;
   StrCp (Top, UnQS (ui->ledDir->text ()));
   NFNm = 0;
   f.DoDir (Top, this, DoDir);
DBG ("`d files", NFNm);
   Sort (FNm, NFNm, sizeof (FNm [0]), StrCm2);

// save
   StrFmt (fn, "`s.txt", Top);
   if (! f.Open (fn, "w"))  {DBG ("can't write file=`s", fn);   return;}

   b = StrLn (Top) + 1;
   for (i = 0; i < NFNm; i++)
      {StrCp (buf, & FNm [i][b]);   StrAp (buf, CC("\n"));   f.Put (buf);}
   f.Shut ();
   StrCp (ed, (getenv ("VISUAL") == nullptr) ? CC("/opt/app/ned")
                                             : getenv ("VISUAL"));
   system (StrFmt (cmd, "`s '`s'", ed, fn));

   if (YNo ("rename (IN PLACE !), save, and click Yes when done editing",
            "RenEm?")) {
      StrFmt (buf, "`s_RenEm.txt", Top);
      if (! Fo.Open (buf, "w"))  {DBG ("can't write file=`s", buf);   return;}

      if ((p = f.DoText (fn, NULL, DoRec)))
         {Fo.Shut ();   DBG (p);   return;}
      Fo.Shut ();
      system (StrFmt (cmd, "`s '`s'", ed, buf));

      if (YNo ("Gonna -DO- RenEm", "Ok?"))
         for (i = 0; i < NFNm; i++)  if (FN2 [i][0])  Fo.ReNm (FNm [i], FN2[i]);
      f.Kill (buf);
   }
   f.Kill (fn);
   qApp->quit ();
}


void RenEm::Dir ()
{ TStr s;
   StrCp (s, UnQS (ui->ledDir->text ()));
   if (AskDir (this, s, "Pick a top directory"))  ui->ledDir->setText (s);
}


//______________________________________________________________________________
void RenEm::Open ()
{ QFont f ("Noto Sans Mono", 12);      // calc our window size from our font
  TStr  d;
  int   argc = qApp->arguments ().count ();
   QApplication::setFont (f);
   WinLoad (this, "StephenHazel", "RenEm");
   setWindowTitle ("RenEm");

   if (argc >= 2)  StrCp (d, CC(UnQS (qApp->arguments ().at (1))));
   else if (getcwd (d, sizeof (TStr)) == nullptr)  *d = '\0';
   connect (ui->btnDir, SIGNAL (clicked ()), this, SLOT (Dir ()));
   connect (ui->btnGo,  SIGNAL (clicked ()), this, SLOT (Go  ()));

   if (*d)  ui->ledDir->setText (d);
}

void RenEm::Shut ()   {WinSave (this, "StephenHazel", "RenEm");}

int main (int argc, char *argv [])
{ QApplication a (argc, argv);
  RenEm w;
  int r;
   w.Open ();   r = a.exec ();   w.Shut ();   return r;
}
