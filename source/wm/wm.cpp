//---------------------------------------------------------------------------
#include <stdio.h>
#include "wm.h"
#include "netwm.h"
//---------------------------------------------------------------------------
//	TDesktop
//---------------------------------------------------------------------------
TDesktop :: TDesktop()
{

}
//---------------------------------------------------------------------------
TDesktop :: ~TDesktop()
{
}
//---------------------------------------------------------------------------
__fastcall TDesktop :: TDesktop(const TDesktop &a)
{
	*this = a;
}
//---------------------------------------------------------------------------
__fastcall TDesktop & TDesktop :: operator=(const TDesktop &a)
{
	if(this == &a)
		return *this;
        // выполняет присваивание, развернутое копирование
        // или еще что-нибудь
	return *this;
}
//---------------------------------------------------------------------------
//	TWindowManager
//---------------------------------------------------------------------------
TWindowManager :: TWindowManager() : TCustomForm(HWND(0))
{

}
//---------------------------------------------------------------------------
TWindowManager :: ~TWindowManager()
{
}
//---------------------------------------------------------------------------
__fastcall TWindowManager :: TWindowManager(const TWindowManager &a)
{
	*this = a;
}
//---------------------------------------------------------------------------
__fastcall TWindowManager & TWindowManager :: operator=(const TWindowManager &a)
{
	if(this == &a)
		return *this;
        // выполняет присваивание, развернутое копирование
        // или еще что-нибудь
	return *this;
}
//---------------------------------------------------------------------------
bool __fastcall TWindowManager :: WndProc(XEvent &event)
{
	Atom m_atom;
	bool ret = false;
	TRect rect;

	switch (event.type)
   	{
		case ClientMessage:
				m_atom = XInternAtom(Application.Handle, "WM_DELETE_WINDOW", true);
				if(m_atom == (Atom)event.xclient.data.l[0])
				{
					Close();
					ret = true;
				}
				else
				{
					m_atom = XInternAtom(Application.Handle, "WM_TAKE_FOCUS", true);
					if(m_atom == (Atom)event.xclient.data.l[0])
					{
						printf("WM_TAKE_FOCUS %d\n", id);
						ret = true;
					}
					else
					{
						m_atom = XInternAtom(Application.Handle, "WM_SAVE_YOURSELF", true);
						if(m_atom == (Atom)event.xclient.data.l[0])
						{
							printf("WM_SAVE_YOURSELF %d\n", id);
							ret = true;
						}
						else
						{
							m_atom = XInternAtom(Application.Handle, "_NET_WM_PING", true);
							if(m_atom == (Atom)event.xclient.data.l[0])
							{
								SendPing(0, &event);
								ret = true;
							}
						}
					}
				}
				break;
		case MapNotify:
				if(!OnShow.empty())
   					OnShow(this);
            	break;
        case UnmapNotify:
        		if(!OnHide.empty())
   					OnHide(this);
        		break;
		case MapRequest:
				printf("MAPREQUEST FORM %x\n",(uint_t)event.xmaprequest.window);
				break;
		case ConfigureRequest:
				printf("ConfigureRequest\n");
				break;
      	case PropertyNotify:
				m_atom = event.xproperty.atom;
				printf("XPROPERTY win=%lu atom=%d name=%s\n", m_Handle, (int)m_atom, XGetAtomName(Application.Handle, m_atom));
            break;
   }
	if(!ret)
		ret = TCustomForm::WndProc(event);
   return ret;
}
//---------------------------------------------------------------------------

