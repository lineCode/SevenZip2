// Windows/Menu.h

#ifndef __WINDOWS_MENU_H
#define __WINDOWS_MENU_H

#include "Common/String.h"
#include "Windows/Defs.h"

namespace NWindows {

struct CMenuItem
{
  UString StringValue;
  UINT fMask; 
  UINT fType; 
  UINT fState; 
  UINT wID; 
  HMENU hSubMenu; 
  HBITMAP hbmpChecked; 
  HBITMAP hbmpUnchecked; 
  ULONG_PTR dwItemData; 
  // LPTSTR dwTypeData; 
  // UINT cch; 
  // HBITMAP hbmpItem;
  bool IsString() const // change it MIIM_STRING
    { return ((fMask & MIIM_TYPE) != 0 && (fType == MFT_STRING)); }
  CMenuItem(): fMask(0), fType(0), fState(0), wID(0), hSubMenu(0), hbmpChecked(0), 
    hbmpUnchecked(0), dwItemData(0) {}
};

class CMenu
{
  HMENU _menu;
public:
  CMenu(): _menu(NULL) {};
  operator HMENU() const { return _menu; }
  void Attach(HMENU menu) { _menu = menu; }
  
  HMENU Detach()
  {
    HMENU menu = _menu;
    _menu = NULL;
    return menu;
  }
  
  bool Create()
  { 
    _menu = ::CreateMenu();
    return (_menu != NULL); 
  }

  bool CreatePopup()
  { 
    _menu = ::CreatePopupMenu();
    return (_menu != NULL); 
  }
  
  bool Destroy()
  { 
    if (_menu == NULL)
      return false;
    return BOOLToBool(::DestroyMenu(Detach()));
  }
  
  int GetItemCount()
    { return GetMenuItemCount(_menu); }

  HMENU GetSubMenu(int pos)
    { return ::GetSubMenu(_menu, pos); }
  bool GetItemString(UINT idItem, UINT flag, CSysString &result)
  {
    result.Empty();
    int len = ::GetMenuString(_menu, idItem, 0, 0, flag);
    len = ::GetMenuString(_menu, idItem, result.GetBuffer(len + 2), 
        len + 1, flag);
    result.ReleaseBuffer();
    return (len != 0);
  }
  UINT GetItemID(int pos)
    { return ::GetMenuItemID(_menu, pos);   }
  UINT GetItemState(UINT id, UINT flags)
    { return ::GetMenuState(_menu, id, flags);   }
  
  bool GetItemInfo(UINT itemIndex, bool byPosition, LPMENUITEMINFO itemInfo)
    { return BOOLToBool(::GetMenuItemInfo(_menu, itemIndex, BoolToBOOL(byPosition), itemInfo)); }
  bool SetItemInfo(UINT itemIndex, bool byPosition, LPMENUITEMINFO itemInfo)
    { return BOOLToBool(::SetMenuItemInfo(_menu, itemIndex, BoolToBOOL(byPosition), itemInfo)); }

  bool AppendItem(UINT flags, UINT_PTR newItemID, LPCTSTR newItem)
    { return BOOLToBool(::AppendMenu(_menu, flags, newItemID, newItem)); }

  bool Insert(UINT position, UINT flags, UINT_PTR idNewItem, LPCTSTR newItem)
    { return BOOLToBool(::InsertMenu(_menu, position, flags, idNewItem, newItem)); }

  bool InsertItem(UINT itemIndex, bool byPosition, LPCMENUITEMINFO itemInfo)
    { return BOOLToBool(::InsertMenuItem(_menu, itemIndex, BoolToBOOL(byPosition), itemInfo)); }

  bool RemoveItem(UINT item, UINT flags)
    { return BOOLToBool(::RemoveMenu(_menu, item, flags)); }

  #ifndef _UNICODE
  bool GetItemInfo(UINT itemIndex, bool byPosition, LPMENUITEMINFOW itemInfo)
    { return BOOLToBool(::GetMenuItemInfoW(_menu, itemIndex, BoolToBOOL(byPosition), itemInfo)); }
  bool InsertItem(UINT itemIndex, bool byPosition, LPMENUITEMINFOW itemInfo)
    { return BOOLToBool(::InsertMenuItemW(_menu, itemIndex, BoolToBOOL(byPosition), itemInfo)); }
  bool SetItemInfo(UINT itemIndex, bool byPosition, LPMENUITEMINFOW itemInfo)
    { return BOOLToBool(::SetMenuItemInfoW(_menu, itemIndex, BoolToBOOL(byPosition), itemInfo)); }
  bool AppendItem(UINT flags, UINT_PTR newItemID, LPCWSTR newItem);
  #endif

  bool GetItem(UINT itemIndex, bool byPosition, CMenuItem &item);
  bool SetItem(UINT itemIndex, bool byPosition, const CMenuItem &item);
  bool InsertItem(UINT itemIndex, bool byPosition, const CMenuItem &item);

  int Track(UINT flags, int x, int y, HWND hWnd)
    { return ::TrackPopupMenuEx(_menu, flags, x, y, hWnd, NULL); }

  bool CheckRadioItem(UINT idFirst, UINT idLast, UINT idCheck, UINT flags)
    { return BOOLToBool(::CheckMenuRadioItem(_menu, idFirst, idLast, idCheck, flags)); }
  DWORD CheckItem(UINT id, UINT uCheck)
    { return ::CheckMenuItem(_menu, id, uCheck); }
};

class CMenuDestroyer
{
  CMenu *_menu;
public:
  CMenuDestroyer(CMenu &menu): _menu(&menu) {}
  CMenuDestroyer(): _menu(0) {}
  ~CMenuDestroyer() { if (_menu) _menu->Destroy(); }
  void Attach(CMenu &menu) { _menu = &menu; }
  void Disable() { _menu = 0; }
};

}

#endif
