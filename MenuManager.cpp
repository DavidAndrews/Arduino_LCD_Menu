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



#include "MenuManager.h"
#include "MenuEntry.h"

MenuManager::MenuManager(MenuLCD* pMenuLCD)
: m_pMenuLCD( pMenuLCD),
  m_fDoingIntInput( false )
{
}

bool MenuManager::addMenuRoot( MenuEntry * p_menuItem)
{
  m_pRootMenuEntry = p_menuItem;
  m_pCurrentMenuEntry = p_menuItem;
}

void MenuManager::addSibling( MenuEntry * p_menuItem)
{
  m_pCurrentMenuEntry->addSibling( p_menuItem );  
}

void MenuManager::addChild( MenuEntry * p_menuItem)
{
  m_pCurrentMenuEntry->addChild( p_menuItem );
}


MenuEntry * MenuManager::getMenuRoot()
{
  return m_pRootMenuEntry;
}


void MenuManager::WipeMenu( MenuLCD::Direction dir )
{
  if( dir == MenuLCD::LEFT )
  {
    for( int i = 0; i < m_pMenuLCD->getCharacters(); ++i )
    { 
      m_pMenuLCD->getLCD()->scrollDisplayLeft();
      delay(75);
    }
  }
  else
  {
    for( int i = 0; i < m_pMenuLCD->getCharacters(); ++i )
    {
      m_pMenuLCD->getLCD()->scrollDisplayRight();
      delay(75);
    }
  }
}


void MenuManager::DrawMenu()
{
  if( m_pCurrentMenuEntry->getNextSibling() == NULL )
  {
    if( m_pCurrentMenuEntry->getPrevSibling() != NULL )
    {
      char *pMenuTexts[2] = {m_pCurrentMenuEntry->getPrevSibling()->getMenuText(), m_pCurrentMenuEntry->getMenuText()};
      m_pMenuLCD->PrintMenu( pMenuTexts, 2, 1 );
    }
    else
    {
      char * pText = m_pCurrentMenuEntry->getMenuText();
      m_pMenuLCD->PrintMenu( &pText, 1, 0 );      
    }
  }
  else
  {
    char *pMenuTexts[2] = {m_pCurrentMenuEntry->getMenuText(), m_pCurrentMenuEntry->getNextSibling()->getMenuText()};
    m_pMenuLCD->PrintMenu( pMenuTexts, 2, 0 );
  }
}

void MenuManager::DoMenuAction( MENU_ACTION action )
{
  if( m_fDoingIntInput == true )
  {
    int iNewNum = m_pMenuIntHelper->getInt();
    char buff[64];
    switch (action )
    {
      case MENU_ACTION_UP:
        iNewNum = m_pMenuIntHelper->numDecrease();
        itoa( iNewNum, buff, 10 );
        DrawInputRow( buff );
        *m_pInt = iNewNum;
        break;
      case MENU_ACTION_DOWN:
        iNewNum = m_pMenuIntHelper->numIncrease();
        itoa( iNewNum, buff, 10 );
        DrawInputRow( buff );
        *m_pInt = iNewNum;
        break;
      case MENU_ACTION_SELECT:
        m_fDoingIntInput = false;
        DrawMenu();
        break;
      case MENU_ACTION_BACK:
        m_fDoingIntInput = false;
        DrawMenu();
        break;
    }    
  }
  else
  {
    m_pMenuLCD->ClearLCD();
    delay(10);
    DrawMenu();
    switch (action )
    {
      case MENU_ACTION_UP:
        this->MenuUp();
        break;
      case MENU_ACTION_DOWN:
        this->MenuDown();
        break;
      case MENU_ACTION_SELECT:
        this->MenuSelect();
        break;
      case MENU_ACTION_BACK:
        this->MenuBack();
        break;
    }
  }
}

void MenuManager::MenuUp()
{
  MenuEntry *prev = m_pCurrentMenuEntry->getPrevSibling();
  if( prev != NULL )
  {
    m_pCurrentMenuEntry = prev;
  }
  else
  {
    //Flash?
  }
  DrawMenu();
}

void MenuManager::MenuDown()
{
  MenuEntry *next = m_pCurrentMenuEntry->getNextSibling();
  if( next != NULL )
  {
    m_pCurrentMenuEntry = next;
  }
  else
  {
    //Flash?
  }
  DrawMenu();

}

void MenuManager::MenuSelect()
{
  MenuEntry *child = m_pCurrentMenuEntry->getChild();
  if( child != NULL )
  {
    WipeMenu( MenuLCD::LEFT);
    m_pCurrentMenuEntry = child;
    DrawMenu();
  }
  else
  {
    if( !m_pCurrentMenuEntry->isBackEntry() )
    {
      WipeMenu( MenuLCD::LEFT);
    }
    m_pCurrentMenuEntry->ExecuteCallback();
    if( !m_fDoingIntInput )
    {
      DrawMenu();
    }
  }  
}

void MenuManager::MenuBack()
{
  if( m_pCurrentMenuEntry->getParent() != NULL )
  {
    WipeMenu( MenuLCD::RIGHT);
    m_pCurrentMenuEntry = m_pCurrentMenuEntry->getParent();
    DrawMenu();
  }
}

void MenuManager::SelectRoot()
{
  m_pCurrentMenuEntry = m_pRootMenuEntry;
}

void MenuManager::DrawInputRow( char *pString )
{
  m_pMenuLCD->PrintLineRight( pString, m_pMenuLCD->getLines() - 1 );
}

void MenuManager::DoIntInput( int iMin, int iMax, int iStart, int iSteps, char **label, int iLabelLines, int *pInt )
{
  char buff[64];
  m_fDoingIntInput = true;
  m_pInt = pInt;
  *pInt = iStart;
  //The MenuIntHelper class will keep track of the input, but all other logic will stay here
  if( m_pMenuIntHelper != NULL )
  {
    delete m_pMenuIntHelper;
  }
  m_pMenuIntHelper = new MenuIntHelper(  iMin, iMax, iStart, iSteps );
  //print the label
  m_pMenuLCD->PrintMenu( label, iLabelLines, -1 );
  m_iIntLine = iLabelLines;  //Off by one because index is zero based
  itoa( m_pMenuIntHelper->getInt(), buff, 10 );  
  DrawInputRow( buff );
}

