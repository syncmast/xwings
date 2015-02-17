//---------------------------------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "sysobj.h"
//---------------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
static void timer_routine(sigval_t sv);
//---------------------------------------------------------------------------
//  TTimer
//---------------------------------------------------------------------------
static void timer_routine(sigval_t sv)
{
    void *tmp = sv.sival_ptr;
    TTimer *timer = (TTimer *)tmp;
    if(!timer->OnTimer.empty())
        (timer->OnTimer)(timer);
}
//---------------------------------------------------------------------------
TTimer :: TTimer()
{
    Init();
}
//---------------------------------------------------------------------------
__fastcall TTimer :: TTimer(const TTimer &a) : TObject(a)
{
    Init();
    Assign(a);
}
//---------------------------------------------------------------------------
TTimer :: ~TTimer()
{
    timer_delete(m_timerid);
}
//---------------------------------------------------------------------------
__fastcall const TTimer & TTimer :: operator=(const TTimer &a)
{
    if(this == &a)
        return *this;
    TObject :: operator=(a);
    Assign(a);
    return *this;
}
//---------------------------------------------------------------------------
void TTimer :: Init()
{
    m_Enabled = false;
    m_Interval = 1000;
    m_timerid = 0;

    struct sigevent se;
    memset(&se, 0, sizeof(se));

    se.sigev_notify = SIGEV_THREAD;
    se.sigev_notify_function = timer_routine;
    se.sigev_value.sival_ptr = this;

    timer_create(CLOCK_REALTIME, &se, &m_timerid);
}
//---------------------------------------------------------------------------
__fastcall void TTimer :: Assign(const TTimer &obj)
{
    m_Enabled = obj.m_Enabled;
    m_Interval = obj.m_Interval;
    OnTimer = obj.OnTimer;
    if(m_Enabled)
        set_timer(m_Interval);
    else
        set_timer(0);
}
//---------------------------------------------------------------------------
void TTimer :: set_timer(uint_t interval)
{
    struct itimerspec tv;
    memset(&tv, 0, sizeof(tv));
    unsigned int sec, nsec;
    sec = interval / 1000;
    nsec = (interval % 1000) * 1000 * 1000;
    tv.it_interval.tv_sec = sec;
    tv.it_interval.tv_nsec = nsec;
    tv.it_value.tv_sec = sec;
    tv.it_value.tv_nsec = nsec;
    timer_settime(m_timerid, 0, &tv, NULL);
}
//---------------------------------------------------------------------------
void TTimer :: set_Enabled(bool v)
{
    if(m_Enabled != v)
    {
        if( v )
            set_timer(m_Interval);
        else
            set_timer(0);
        m_Enabled = v;
    }
}
//---------------------------------------------------------------------------
void TTimer :: set_Interval(uint_t interval)
{
    if(m_Interval != interval)
    {
        m_Interval = interval;
        if( m_Enabled )
            set_timer(m_Interval);
    }
}
//---------------------------------------------------------------------------
//	TMutex
//---------------------------------------------------------------------------
TMutex :: TMutex()
{
    pthread_mutex_init(&m_id, NULL);
}
//---------------------------------------------------------------------------
TMutex :: ~TMutex()
{
    pthread_mutex_destroy(&m_id);
}
//---------------------------------------------------------------------------
void TMutex :: Lock()
{
    pthread_mutex_lock(&m_id);
}
//---------------------------------------------------------------------------
void TMutex :: Unlock()
{
    pthread_mutex_unlock(&m_id);
}
//---------------------------------------------------------------------------
bool TMutex :: Trylock()
{
    return bool(pthread_mutex_trylock(&m_id));
}
//---------------------------------------------------------------------------
//	TThread
//---------------------------------------------------------------------------
static void *thread_entry_point(void *pthis)
{
    TThread *pt = (TThread*)pthis;
    pt->Execute();
    return pt->Data;
}
//---------------------------------------------------------------------------
TThread :: TThread()
{
    m_thread_id = 0;
    pthread_attr_init(&m_thread_attrib);
    Data = NULL;
}
//---------------------------------------------------------------------------
TThread :: ~TThread()
{
    if(m_thread_id != 0)
        pthread_detach(m_thread_id);
    pthread_attr_destroy(&m_thread_attrib);
}
//---------------------------------------------------------------------------
bool TThread :: get_Detached()
{
    int detachstate;
    pthread_attr_getdetachstate(&m_thread_attrib, &detachstate);
    return detachstate == PTHREAD_CREATE_DETACHED;
}
//---------------------------------------------------------------------------
void TThread :: set_Detached(bool value)
{
    if(get_Detached() != value)
    {
        int detachstate = value ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE;
        pthread_attr_setdetachstate(&m_thread_attrib, detachstate);
    }
}
//---------------------------------------------------------------------------
size_t TThread :: get_GuardSize()
{
    size_t ss;
    pthread_attr_getguardsize(&m_thread_attrib, &ss);
    return ss;
}
//---------------------------------------------------------------------------
void TThread :: set_GuardSize(size_t value)
{
    if(get_GuardSize() != value)
    {
        pthread_attr_setguardsize(&m_thread_attrib, value);
    }
}
//---------------------------------------------------------------------------
bool TThread :: get_InheritSched()
{
    int inherit;
    pthread_attr_getinheritsched(&m_thread_attrib, &inherit);
    return inherit == PTHREAD_INHERIT_SCHED;
}
//---------------------------------------------------------------------------
void TThread :: set_InheritSched(bool value)
{
    if(get_InheritSched() != value)
    {
        int inherit = value ? PTHREAD_INHERIT_SCHED : PTHREAD_EXPLICIT_SCHED;
        pthread_attr_setinheritsched(&m_thread_attrib, inherit);
    }
}
//---------------------------------------------------------------------------
TSchedPolicy TThread :: get_Policy()
{
    int policy;
    pthread_attr_getschedpolicy(&m_thread_attrib, &policy);
    return TSchedPolicy(policy);
}
//---------------------------------------------------------------------------
void TThread :: set_Policy(TSchedPolicy value)
{
    if(get_Policy() != value)
    {
        pthread_attr_setschedpolicy(&m_thread_attrib, value);
    }
}
//---------------------------------------------------------------------------
int TThread :: get_Priority()
{
    sched_param param;
    pthread_attr_getschedparam(&m_thread_attrib, &param);
    return param.sched_priority;
}
//---------------------------------------------------------------------------
void TThread :: set_Priority(int value)
{
    sched_param param;

    if(get_Priority() != value)
    {
        param.sched_priority = value;
        pthread_attr_setschedparam(&m_thread_attrib, &param);
    }
}
//---------------------------------------------------------------------------
bool TThread :: get_Scope()
{
    int scope;
    pthread_attr_getscope(&m_thread_attrib, &scope);
    return scope == PTHREAD_CREATE_DETACHED;
}
//---------------------------------------------------------------------------
void TThread :: set_Scope(bool value)
{
    if(get_Scope() != value)
    {
        int scope = value ? PTHREAD_SCOPE_SYSTEM : PTHREAD_SCOPE_PROCESS;
        pthread_attr_setscope(&m_thread_attrib, scope);
    }
}
//---------------------------------------------------------------------------
size_t TThread :: get_StackSize()
{
    size_t ss;
    pthread_attr_getstacksize(&m_thread_attrib, &ss);
    return ss;
}
//---------------------------------------------------------------------------
void TThread :: set_StackSize(size_t value)
{
    if(value > 0 && value < PTHREAD_STACK_MIN)
        value += PTHREAD_STACK_MIN;
    if(get_StackSize() != value)
    {
        pthread_attr_setstacksize(&m_thread_attrib, value);
    }
}
//---------------------------------------------------------------------------
void *TThread :: get_StackAddress()
{
    void *ss;
    size_t td;
    pthread_attr_getstack(&m_thread_attrib, &ss, &td);
    return ss;
}
//---------------------------------------------------------------------------
void TThread :: set_StackAddress(void *value)
{
    if(value && get_StackAddress() != value)
    {
        pthread_attr_setstack(&m_thread_attrib, value, get_StackSize());
    }
}
//---------------------------------------------------------------------------
int TThread :: Start()
{
    int code = pthread_create(&m_thread_id, &m_thread_attrib, &thread_entry_point, this);
    return code;
}
//---------------------------------------------------------------------------
void TThread :: Stop()
{
    pthread_cancel(m_thread_id);
    m_thread_id = 0;
}
//---------------------------------------------------------------------------
void TThread :: Execute()
{
    if(OnExecute)
        OnExecute(this);
}
//---------------------------------------------------------------------------
void *TThread :: WaitFor()
{
    void *status;

    pthread_join(m_thread_id, &status);
    m_thread_id = 0;
    return status;
}
//---------------------------------------------------------------------------
//	TProcess
//---------------------------------------------------------------------------
TProcess :: TProcess() : m_pid(0)
{
}
//---------------------------------------------------------------------------
TProcess :: ~TProcess()
{
    Stop();
}
//---------------------------------------------------------------------------
int TProcess :: Start()
{
    if(m_pid)
        WaitFor();
    int ret = fork();
    if(ret == 0)
    {
        Execute();
        _exit(EXIT_SUCCESS);
    }
    else
        m_pid = ret;
    return ret;
}
//---------------------------------------------------------------------------
void TProcess :: Stop()
{
    if(m_pid)
        kill(m_pid, SIGTERM);
    m_pid = 0;
}
//---------------------------------------------------------------------------
void TProcess :: Resume()
{
    if(m_pid)
        kill(m_pid, SIGCONT);
}
//---------------------------------------------------------------------------
void TProcess :: Pause()
{
    if(m_pid)
        kill(m_pid, SIGSTOP);
}
//---------------------------------------------------------------------------
void TProcess :: Execute()
{
    if(!OnExecute.empty())
        (OnExecute)(this);
}
//---------------------------------------------------------------------------
int TProcess :: WaitFor()
{
    int status;

    waitpid(m_pid, &status, 0);
    m_pid = 0;
    return WEXITSTATUS(status);
    /*
    /proc/[pid]/stat
        Status information about the process. This is used by ps(1). It is defined in /usr/src/linux/fs/proc/array.c.

           state %c
                One character from the string "RSDZTW" where R is running,
                S is sleeping in an interruptible wait, D is waiting in uninterruptible disk sleep,
                Z is zombie, T is traced or stopped (on a signal), and W is paging.
    */	/*
 * Ждем завершения дочернего процесса
 * и обрабатываем код статуса завершения
 */
//   if((pid = waitpid(pid, &status, 0)) && WIFEXITED(status)) {
//       printf("Дочерний процесс с PID = %d завершил выполнение\n", pid);
//       printf("Код статуса завершения равен %d\n", WEXITSTATUS(status));
//   }

}
//---------------------------------------------------------------------------
//	TSignal
//---------------------------------------------------------------------------
class TSignalRegister
{
    typedef map<int, TSignal *> TSignalMap;

private:
    static TSignalMap &get_signal_map()
    {
        static TSignalMap __signal_map;
        return __signal_map;
    }

public:
    static void __register_signal(int signum, TSignal *signal)
    {
        if(signal == NULL)
            return;
        for( TSignalMap::iterator it = get_signal_map().begin();
                it != get_signal_map().end();
                ++it )
        {
            if(it->first == signum || it->second == signal)
                return;
        }
        get_signal_map()[signum] = signal;
    }

    static TSignal *__get_signal(int signum)
    {
        for( TSignalMap::iterator it = get_signal_map().begin();
                it != get_signal_map().end();
                ++it )
        {
            if(it->first == signum)
                return (TSignal *)(it->second);
        }
        return NULL;
    }

    static inline void __erase_signal(int signum)
    {
        get_signal_map().erase(signum);
    }
};
//---------------------------------------------------------------------------
static void __sig_handler(int signum, siginfo_t *siginfo, void *context)
{
    TSignal *signal = TSignalRegister :: __get_signal(signum);
    if(signal)
        if(signal->OnSignal)
            signal->OnSignal(signal, siginfo, context);
    return;
}
//---------------------------------------------------------------------------
TSignal :: TSignal() : m_Signal(0)
{
    m_ResetHandler = false;
    m_Restart = true;
}
//---------------------------------------------------------------------------
TSignal :: TSignal(int signum) : m_Signal(0)
{
    m_ResetHandler = false;
    m_Restart = true;
    Signal = signum;
}
//---------------------------------------------------------------------------
TSignal :: ~TSignal()
{
    Signal = 0;
}
//---------------------------------------------------------------------------
TSignalHandler TSignal :: get_Handler()
{
    TSignalHandler ret;
    if(m_Signal == 0)
        ret = shUndefined;
    else
    {
        struct sigaction act;
        sigaction(m_Signal, NULL, &act);
        if(act.sa_handler == SIG_DFL)
            ret = shDefault;
        else if(act.sa_handler == SIG_IGN)
            ret = shIgnored;
        else ret = shCustom;
    }
    return ret;
}
//---------------------------------------------------------------------------
void TSignal :: set_Handler(TSignalHandler value)
{
    switch(value)
    {
    case shUndefined:
        sigaction(m_Signal, &m_oldaction, NULL);
        TSignalRegister :: __erase_signal(m_Signal);
        break;
    case shDefault:
        signal(m_Signal, SIG_DFL);
        break;
    case shIgnored:
        signal(m_Signal, SIG_IGN);
        break;
    case shCustom:
        struct sigaction act;
        memset(&act, 0, sizeof(act));
        act.sa_sigaction = __sig_handler;
        act.sa_mask = m_Mask.Handle;
        act.sa_flags = SA_SIGINFO;
        if(m_ResetHandler)
            act.sa_flags |= SA_RESETHAND;
        if(m_Restart)
            act.sa_flags |= SA_RESTART;
        sigaction(m_Signal, &act, &m_oldaction);
        TSignalRegister :: __register_signal(m_Signal, this);
        break;
    }
}
//---------------------------------------------------------------------------
void TSignal :: set_Signal(int value)
{
    if(m_Signal == value)
        return;
    if(value < 0 || value > NSIG)
        return;
    if(value == 0)
    {
        sigaction(m_Signal, &m_oldaction, NULL);
        TSignalRegister :: __erase_signal(m_Signal);
    }
    m_Signal = value;
}
//---------------------------------------------------------------------------
void TSignal :: set_ResetHandler(bool value)
{
    if(m_ResetHandler == value)
        return;
    m_ResetHandler = value;
    if(Handler == shCustom)
    {
        Handler = shUndefined;
        Handler = shCustom;
    }
}
//---------------------------------------------------------------------------
void TSignal :: set_Restart(bool value)
{
    if(m_Restart == value)
        return;
    m_Restart = value;
    if(Handler == shCustom)
    {
        Handler = shUndefined;
        Handler = shCustom;
    }
}
//---------------------------------------------------------------------------
TString TSignal :: get_Name()
{
    if(m_Signal == 0)
        return TString("Undefined");
    else
        return TString(sys_siglist[m_Signal]);
}
//---------------------------------------------------------------------------
void TSignal :: Raise(int pid, void *data)
{
    sigval_t val;

    if(m_Signal > 0)
    {
        if(data == NULL)
        {
            if(pid)
                kill(pid, m_Signal);
            else
                raise(m_Signal);
        }
        else
        {
            val.sival_ptr = data;
            if(pid == 0)
                pid = getpid();
            sigqueue(pid, m_Signal, val);
        }
    }
}
//---------------------------------------------------------------------------
} // namespace
//---------------------------------------------------------------------------------
