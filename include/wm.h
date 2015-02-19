//---------------------------------------------------------------------------
#ifndef wmH
#define wmH

#include "application.h"

//---------------------------------------------------------------------------
// TDesktop
//---------------------------------------------------------------------------
/*
class TDesktop : public TWinControl // TCustomForm
{
private:

protected:
	virtual void __fastcall Init();

public:
	__fastcall TDesktop();
   	__fastcall TDesktop(HWND win);
  	~TDesktop();
	void Paint(){};
};
*/

//---------------------------------------------------------------------------
class TDesktop : public TCustomForm
{
	DECLARE_PROPERTY( TDesktop );

private:

protected:

public:
   	__fastcall TDesktop();
   	__fastcall TDesktop(const TDesktop &a);
   	__fastcall TDesktop & operator=(const TDesktop &a);
   	 ~TDesktop();

//	bool __fastcall WndProc(XEvent &event);

};
//---------------------------------------------------------------------------
class TWindowManager : public TCustomForm
{
	DECLARE_PROPERTY( TWindowManager );

private:

protected:

public:
   	__fastcall TWindowManager();
   	__fastcall TWindowManager(const TWindowManager &a);
   	__fastcall TWindowManager & operator=(const TWindowManager &a);
   	 ~TWindowManager();

	bool __fastcall WndProc(XEvent &event);

};
//---------------------------------------------------------------------------
#endif
