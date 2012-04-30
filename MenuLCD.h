#include <LiquidCrystal.h>

#ifndef MenuLCD_H

#define MenuLCD_H 1
class MenuLCD
{
  public:
  MenuLCD(int LCDRS, int LCDE, int LCDD4, int LCDD5, int LCDD6, int LCDD7, int characters, int lines);
  bool MenuLCDSetup();
  bool MenuLCDPrint( char ** pString, int nLines, int nSelectedLine /*= 0*/ );
  int DoIntInput( int iMin, int iMax, int iSteps );
  
  private:
  LiquidCrystal* m_pLcd;
  int m_LCDRS;
  int m_LCDE;
  int m_LCDD4;
  int m_LCDD5;
  int m_LCDD6;
  int m_LCDD7;
  int m_characters;
  int m_lines;
};

#endif
