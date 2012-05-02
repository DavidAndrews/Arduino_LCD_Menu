/*
 Arduino_LCD_Menu Library
Copyright Dustin Andrews, David Andrews 2012 
Licensed under the follwing license:

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the 
following conditions are met:
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer. 
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
in the documentation and/or other materials provided with the distribution. 
The name of the author may not be used to endorse or promote products derived from this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


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

bool MenuLCD::PrintMenu( char* pString[], int nLines, int nSelectedLine = 0 )
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

bool MenuLCD::PrintLineRight( char* pString, int iRow )
{
  m_pLcd->setCursor(m_characters - strlen(pString),iRow);
  m_pLcd->print( pString );  
}

int MenuLCD::getLines()
{
  return m_lines;
}
int MenuLCD::getCharacters()
{
  return m_characters;
}

