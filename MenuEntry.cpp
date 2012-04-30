#include <arduino.h>
#include "MenuEntry.h"

MenuEntry::MenuEntry( char * menuText, void * userData, MENU_ACTION_CALLBACK_FUNC func)
{
  m_menuText = strdup(menuText);
  m_userData = userData;
  m_nextSibling = NULL;
  m_prevSibling = NULL;
  m_child = NULL;  
  m_parent = NULL;
  m_callback = func;
}

void MenuEntry::ExecuteCallback()
{
  if( m_callback != NULL )
  {
    m_callback(m_menuText, m_userData);
  }
}

bool MenuEntry::addChild(MenuEntry* child)
{
  child->setParent( this );
  if(m_child != NULL)
  {
    m_child->addSibling( child );
  }
  else
  {
    m_child = child;
  }
  return true;
}

bool MenuEntry::addSibling( MenuEntry* sibling)
{
  sibling->setParent( m_parent );
  if( m_nextSibling != NULL )
  {
    m_nextSibling->addSibling(sibling);
  }
  else
  {
    m_nextSibling = sibling;
    sibling->setPrevSibling( this );
  }
}

void MenuEntry::setPrevSibling( MenuEntry * pPrevSibling)
{
  m_prevSibling = pPrevSibling;
}

char * MenuEntry::getMenuText()
{
  return m_menuText;
}

MenuEntry *MenuEntry::getNextSibling()
{
  return m_nextSibling;
}
MenuEntry *MenuEntry::getPrevSibling()
{
  return m_prevSibling;
}
MenuEntry *MenuEntry::getChild()
{
  return m_child;
}
MenuEntry *MenuEntry::getParent()
{
  return m_parent;
}
void MenuEntry::setParent( MenuEntry * parent)
{
  m_parent = parent;
}

