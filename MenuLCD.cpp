#include <LiquidCrystal.h>
#include "MenuLCD.h"

MenuLCD::MenuLCD(int LCDRS, int LCDE, int LCDD4, int LCDD5, int LCDD6, int LCDD7, int characters, int lines)
: m_LCDRS( LCDRS ),
  m_LCDE( LCDE ),
  m_LCDD4( LCDD4 ),
  m_LCDD5( LCDD5 ),
  m_LCDD6( LCDD6 ),
  m_LCDD7( LCDD7 ),
  m_characters( characters ),
  m_lines( lines )
{
}

bool MenuLCD::MenuLCDSetup()
{
  m_pLcd = new LiquidCrystal(m_LCDRS, m_LCDE, m_LCDD4, m_LCDD5, m_LCDD6, m_LCDD7);
  m_pLcd->begin(m_characters, m_lines);
}

bool MenuLCD::MenuLCDPrint( char* pString[], int nLines, int nSelectedLine = 0 )
{
  m_pLcd->clear();
  for( int i =0; i < nLines; i++ )
  {
    if( i == nSelectedLine )
    {//this line should be surrounded by []
       m_pLcd->setCursor(0, i);
       m_pLcd->write( '[');
       m_pLcd->setCursor(1,i);
       m_pLcd->print( pString[i] );
       m_pLcd->setCursor(m_characters - 1, i);
       m_pLcd->write( ']');
    }
    else
    {
      m_pLcd->setCursor(0,i);
      m_pLcd->print( pString[i] );
    }

  }
}



