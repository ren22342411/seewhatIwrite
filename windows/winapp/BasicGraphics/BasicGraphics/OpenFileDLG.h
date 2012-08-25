#ifndef OFDLG
#define OFDLG
#include "COMMDLG.H"
static OPENFILENAME ofn4load;
static OPENFILENAME ofn4save;

BOOL LoadFileDLG (HWND, PTSTR, PTSTR);
BOOL SaveFileDLG(HWND, PTSTR, PTSTR);

#endif