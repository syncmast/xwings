//---------------------------------------------------------------------------
#ifndef sysobjH
#define sysobjH

#include <pthread.h>
#include <signal.h>
#include "tobj.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//	TTimer
//---------------------------------------------------------------------------
class TTimer : public TObject
{
    DECLARE_PROPERTY(TTimer);

private:
    timer_t m_timerid;
    bool m_Enabled;
    uint_t m_Interval;
    void Init();
    __fastcall void Assign(const TTimer &obj);
    void set_Enabled(bool v);
    void set_Interval(uint_t interval);
    void set_timer(uint_t interval);

protected:

public:
    TTimer();
    __fastcall TTimer(const TTimer &a);
    virtual ~TTimer();
    __fastcall const TTimer & operator=(const TTimer &a);

    TNotifyEvent OnTimer;

    property (m, bool, Enabled);
    property (m, uint_t, Interval);
};
//---------------------------------------------------------------------------
//	TMutex
//---------------------------------------------------------------------------
class TMutex : public TObject
{
private:
    pthread_mutex_t m_id;

protected:

public:
    __fastcall TMutex();
    ~TMutex();
    void Lock();
    void Unlock();
    bool Trylock();
};
//---------------------------------------------------------------------------
//	TThread
//---------------------------------------------------------------------------
enum TSchedPolicy { spFifo = SCHED_FIFO, spRR = SCHED_RR, spOther = SCHED_OTHER };

class TThread : public TObject
{
    DECLARE_PROPERTY(TThread);

private:
    pthread_t m_thread_id;
    pthread_attr_t m_thread_attrib;

    bool get_Detached();
    void set_Detached(bool value);
    size_t get_GuardSize();
    void set_GuardSize(size_t value);
    bool get_InheritSched();
    void set_InheritSched(bool value);
    TSchedPolicy get_Policy();
    void set_Policy(TSchedPolicy value);
    int get_Priority();
    void set_Priority(int value);
    bool get_Scope();
    void set_Scope(bool value);
    size_t get_StackSize();
    void set_StackSize(size_t value);
    void *get_StackAddress();
    void set_StackAddress(void *value);

protected:

public:
    void *Data;
    __fastcall TThread();
    ~TThread();
    virtual void Execute();
    int Start();
    void Stop();
    void *WaitFor();

    TNotifyEvent OnExecute;

    property (rw, bool, Detached);
    property (rw, size_t, GuardSize);
    property (rw, bool, InheritSched);
    property (rw, TSchedPolicy, Policy);
    property (rw, int, Priority);
    property (rw, bool, Scope);
    property (rw, size_t, StackSize);
    property (rw, void *, StackAddress);
};
//---------------------------------------------------------------------------
//	TProcess
//---------------------------------------------------------------------------
class TProcess : public TObject
{
    DECLARE_PROPERTY(TProcess);

private:
    pid_t m_pid;

protected:

public:
    void *Data;
    TProcess();
    ~TProcess();
    virtual void Execute();
    int Start();
    void Stop();
    void Pause();
    void Resume();
    int WaitFor();

    TNotifyEvent OnExecute;
};
//---------------------------------------------------------------------------
//	TSignalSet
//---------------------------------------------------------------------------
class TSignalSet : public TObject
{
    DECLARE_PROPERTY(TSignalSet);

private:
    sigset_t m_Handle;

public:
    inline TSignalSet()
    {
        sigemptyset(&m_Handle);
    };
    inline void Add(int signum)
    {
        sigaddset(&m_Handle, signum);
    };
    inline void Block()
    {
        sigprocmask(SIG_SETMASK, &m_Handle, NULL);
        sigprocmask(SIG_BLOCK, &m_Handle, NULL);
    };
    inline void Clear()
    {
        sigemptyset(&m_Handle);
    };
    inline bool Contains(int signum)
    {
        return (bool)sigismember(&m_Handle, signum);
    };
    inline void Delete(int signum)
    {
        sigdelset(&m_Handle, signum);
    };
    inline void Fill()
    {
        sigfillset(&m_Handle);
    };
    inline void GetCurrentSet()
    {
        sigprocmask(SIG_BLOCK, NULL, &m_Handle);
    };
    inline void GetPendingSet()
    {
        sigpending(&m_Handle);
    };
    inline void Unblock()
    {
        sigprocmask(SIG_UNBLOCK, &m_Handle, NULL);
    };
    inline void Wait()
    {
        sigsuspend(&m_Handle);
    };

    property (mr, sigset_t, Handle);
};
//---------------------------------------------------------------------------
//	TSignal
//---------------------------------------------------------------------------
class TSignal;
typedef delegate< void (TSignal *signal, siginfo_t *siginfo, void *context) > TSignalEvent;
enum TSignalHandler { shUndefined, shDefault, shIgnored, shCustom };

class TSignal : public TObject
{
    DECLARE_PROPERTY(TSignal);

private:
    int m_Signal;
    bool m_ResetHandler;
    bool m_Restart;
    TSignalSet m_Mask;
    struct sigaction m_oldaction;
    TSignal& operator=(const TSignal&);
    TSignal(const TSignal&);
    void set_Signal(int value);
    TSignalHandler get_Handler();
    void set_Handler(TSignalHandler value);
    TString get_Name();
    void set_ResetHandler(bool value);
    void set_Restart(bool value);

protected:

public:
    TSignal();
    __fastcall TSignal(int signum);
    ~TSignal();
    void Raise(int pid = 0, void *data = NULL);

    TSignalEvent	OnSignal;

    property (m, bool, ResetHandler);
    property (m, bool, Restart);
    property (m, int, Signal);
    property (rw, TSignalHandler, Handler);
    property (r, TString, Name);
    property (mr, TSignalSet, Mask);
};
//---------------------------------------------------------------------------
}  // namespace
//---------------------------------------------------------------------------
#endif
