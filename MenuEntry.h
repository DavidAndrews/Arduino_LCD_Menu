#include <arduino.h>

#ifndef MenuEntry_h
#define MenuEntry_h 1


typedef void (*MENU_ACTION_CALLBACK_FUNC)( char * pMenuText, void * pUserData );


class MenuEntry
{
  public:
  ~MenuEntry();
  MenuEntry( char * menuText, void * userData/*=0*/, MENU_ACTION_CALLBACK_FUNC func);
  bool addChild( MenuEntry* child);
  bool addSibling( MenuEntry* sibling);
  void setPrevSibling( MenuEntry* prevSibling);
  bool addActionCallback( MENU_ACTION_CALLBACK_FUNC pCallback);
  char* getMenuText();
  void setParent( MenuEntry* parent );
  MenuEntry *getNextSibling();
  MenuEntry *getPrevSibling();
  MenuEntry *getChild();
  MenuEntry *getParent();
  void ExecuteCallback();
  
  private:
  void* m_userData;
  char* m_menuText;
  MenuEntry* m_parent;
  MenuEntry* m_child;
  MenuEntry* m_nextSibling;
  MENU_ACTION_CALLBACK_FUNC m_callback;
  MenuEntry* m_prevSibling;
};

#endif

