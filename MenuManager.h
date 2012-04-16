#include "MenuEntry.h"
#include "MenuLCD.h"

class MenuManager
{
  public:
  MenuManager(MenuLCD* pMenuLCD);
  bool addMenuRoot( MenuEntry * p_menuEntry);
  MenuEntry * getMenuRoot();
  void DrawMenu();
  void MenuUp();
  void MenuDown();
  void MenuSelect();
  void MenuBack();
  void addChild( MenuEntry * p_menuEntry );    
  void addSibling( MenuEntry * p_menuEntry );  
  void SelectRoot();
  
  private:
  MenuEntry* m_pRootMenuEntry;
  MenuEntry* m_pCurrentMenuEntry;
  MenuLCD* m_pMenuLCD;
};



