//---------------------------------------------------------------------------
#ifndef dmnmgrH
#define dmnmgrH

#include "extctrls.h"
//---------------------------------------------------------------------------
class TService : public TObject 
{
	
};

class TDaemonApplication : public TObject 
{
	USE_PROPERTIES(TDaemonApplication);
		
public:
	void __fastcall Initialize();
	void __fastcall Run();
	
	TSignalEvent	OnSignal;	
};

extern TDaemonApplication Application;
//---------------------------------------------------------------------------
#endif
