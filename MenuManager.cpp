#include "MenuManager.h"
#include "MenuEntry.h"

MenuManager::MenuManager(MenuLCD* pMenuLCD)
: m_pMenuLCD( pMenuLCD)
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



void MenuManager::DrawMenu()
{
  if( m_pCurrentMenuEntry->getNextSibling() == NULL )
  {
    if( m_pCurrentMenuEntry->getPrevSibling() != NULL )
    {
      char *pMenuTexts[2] = {m_pCurrentMenuEntry->getPrevSibling()->getMenuText(), m_pCurrentMenuEntry->getMenuText()};
      m_pMenuLCD->MenuLCDPrint( pMenuTexts, 2, 1 );
    }
    else
    {
      char * pText = m_pCurrentMenuEntry->getMenuText();
      m_pMenuLCD->MenuLCDPrint( &pText, 1, 0 );      
    }
  }
  else
  {
    char *pMenuTexts[2] = {m_pCurrentMenuEntry->getMenuText(), m_pCurrentMenuEntry->getNextSibling()->getMenuText()};
    m_pMenuLCD->MenuLCDPrint( pMenuTexts, 2, 0 );
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
    m_pCurrentMenuEntry = child;
    DrawMenu();
  }
  else
  {
    m_pCurrentMenuEntry->ExecuteCallback();
  }  
  DrawMenu();
}

void MenuManager::MenuBack()
{
  if( m_pCurrentMenuEntry->getParent() != NULL )
  {
    m_pCurrentMenuEntry = m_pCurrentMenuEntry->getParent();
    DrawMenu();
  }
}

void MenuManager::SelectRoot()
{
  m_pCurrentMenuEntry = m_pRootMenuEntry;
}
