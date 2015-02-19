//---------------------------------------------------------------------------
#ifndef socketH
#define socketH

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include "tobj.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//	TSocket
//---------------------------------------------------------------------------
enum TSocketDomain
{
    pfUnspec = PF_UNSPEC,		// Unspecified.
    pfLocal = PF_LOCAL,			// Local to host (pipes and file-domain)
    pfUnix = PF_UNIX,			// POSIX name for PF_LOCAL.
    pfFile = PF_FILE,			// Another non-standard name for PF_LOCAL
    pfInet = PF_INET,			// IP protocol family.
    pfAX25 = PF_AX25,			// Amateur radio AX.25 protocol
    pfIPX = PF_IPX,				// IPX - Novell protocols
    pfAppleTalk = PF_APPLETALK,	// Appletalk	ddp(7)
    pfNetRom = PF_NETROM,		// Amateur radio NetROM
    pfBridge = PF_BRIDGE,		// Multiprotocol bridge
    pfAtmPvc = PF_ATMPVC,		// ATM PVCs
    pfX25 = PF_X25,				// Reserved for X.25 project
    pfInet6 = PF_INET6,			// IPv6 Internet protocols	ipv6(7)
    pfRose = PF_ROSE,			// Amateur Radio X.25 PLP
    pfDecNet = PF_DECnet,		// Reserved for DECnet project
    pfNetbeui = PF_NETBEUI,		// Reserved for 802.2LLC project
    pfSecurity = PF_SECURITY,	// Security callback pseudo AF
    pfKey = PF_KEY,				// PF_KEY key management API
    pfNetLink = PF_NETLINK,		// Kernel user interface device 	netlink(7)
    pfRoute = PF_ROUTE,			// Alias to emulate 4.4BSD.
    pfPacket = PF_PACKET,		// Low level packet interface 	packet(7)
    pfAsh = PF_ASH,				// Ash
    pfEcoNet = PF_ECONET,		// Acorn Econet
    pfAtmSvc = PF_ATMSVC,		// ATM SVCs
    pfSna = PF_SNA,				// Linux SNA Project
    pfIrda = PF_IRDA,			// IRDA sockets
    pfPPPoX = PF_PPPOX,			// PPPoX sockets.
    pfWanPipe =	PF_WANPIPE,		// Wanpipe API sockets
    pfBluetooth = PF_BLUETOOTH,	// Bluetooth sockets
    pfIucv = PF_IUCV,			// IUCV sockets
    pfRxRPC = PF_RXRPC,			// RxRPC sockets
    pfMax = PF_MAX
};

enum TSocketType
{
    stStream = SOCK_STREAM,
    stDgram = SOCK_DGRAM,
    stSeqPacket = SOCK_SEQPACKET,
    stRaw = SOCK_RAW,
    stRdm = SOCK_RDM,
    stPacket = SOCK_PACKET
};

#define LINGER_OFF -1

class TSocket;
typedef delegate< void (TSocket *srv, TSocket *cli) > TAcceptEvent;

enum TSocketKind { skClient, skServer, skServerProcess };
enum TAddressKind { akLocal, akRemote };

class TSocket : public TObject
{
    DECLARE_PROPERTY(TSocket);

private:
    int m_Backlog;
    bool m_Connected;
    TList m_Connections;
    TSocketKind m_Kind;
    struct sockaddr *m_local;
    struct sockaddr *m_remote;
    TSocket *m_owner;
    void set_Kind(TSocketKind value);
    void set_Domain(TSocketDomain value);
    void set_Protocol(int value);
    void set_Type(TSocketType value);
    void set_Backlog(int value);
    void set_Connected(bool value);
    int get_Linger();
    void set_Linger(int value);
    inline TSocket *get_Connections(const int index)
    {
        return (TSocket *)m_Connections[index];
    };
    inline short get_ConnectionCount()
    {
        return m_Connections.Count;
    };
    inline bool get_Listening()
    {
        return bool( GetOptions(SOL_SOCKET, SO_ACCEPTCONN) );
    };
    inline bool get_Broadcast()
    {
        return bool( GetOptions(SOL_SOCKET, SO_BROADCAST) );
    };
    inline void set_Broadcast(bool value)
    {
        SetOptions(SOL_SOCKET, SO_BROADCAST, value);
    };
    inline int get_SocketError()
    {
        return GetOptions(SOL_SOCKET, SO_ERROR);
    };
    inline bool get_DontRoute()
    {
        return bool( GetOptions(SOL_SOCKET, SO_DONTROUTE) );
    };
    inline void set_DontRoute(bool value)
    {
        SetOptions(SOL_SOCKET, SO_DONTROUTE, value);
    };
    inline bool get_KeepAlive()
    {
        return bool( GetOptions(SOL_SOCKET, SO_KEEPALIVE) );
    };
    inline void set_KeepAlive(bool value)
    {
        SetOptions(SOL_SOCKET, SO_KEEPALIVE, value);
    };
    inline bool get_OOBinline()
    {
        return bool( GetOptions(SOL_SOCKET, SO_OOBINLINE) );
    };
    inline void set_OOBinline(bool value)
    {
        SetOptions(SOL_SOCKET, SO_OOBINLINE, value);
    };
    inline int get_Priority()
    {
        return GetOptions(SOL_SOCKET, SO_PRIORITY);
    };
    inline void set_Priority(int value)
    {
        SetOptions(SOL_SOCKET, SO_PRIORITY, value);
    };
    inline int get_RecvBufsize()
    {
        return GetOptions(SOL_SOCKET, SO_RCVBUF);
    };
    inline void set_RecvBufsize(int value)
    {
        SetOptions(SOL_SOCKET, SO_RCVBUF, value);
    };
    inline int get_RecvBufforce()
    {
        return GetOptions(SOL_SOCKET, SO_RCVBUFFORCE);
    };
    inline void set_RecvBufforce(int value)
    {
        SetOptions(SOL_SOCKET, SO_RCVBUFFORCE, value);
    };
    inline int get_SendBufsize()
    {
        return GetOptions(SOL_SOCKET, SO_SNDBUF);
    };
    inline void set_SendBufsize(int value)
    {
        SetOptions(SOL_SOCKET, SO_SNDBUF, value);
    };
    inline int get_SendBufforce()
    {
        return GetOptions(SOL_SOCKET, SO_SNDBUFFORCE);
    };
    inline void set_SendBufforce(int value)
    {
        SetOptions(SOL_SOCKET, SO_SNDBUFFORCE, value);
    };
    inline int get_RecvLowat()
    {
        return GetOptions(SOL_SOCKET, SO_RCVLOWAT);
    };
    inline void set_RecvLowat(int value)
    {
        SetOptions(SOL_SOCKET, SO_RCVLOWAT, value);
    };
    inline int get_SendLowat()
    {
        return GetOptions(SOL_SOCKET, SO_SNDLOWAT);
    };
    inline void set_SendLowat(int value)
    {
        SetOptions(SOL_SOCKET, SO_SNDLOWAT, value);
    };
    inline bool get_Reuseaddr()
    {
        return bool( GetOptions(SOL_SOCKET, SO_REUSEADDR) );
    };
    inline void set_Reuseaddr(bool value)
    {
        SetOptions(SOL_SOCKET, SO_REUSEADDR, value);
    };
    int get_RecvTimeout();
    void set_RecvTimeout(int value);
    int get_SendTimeout();
    void set_SendTimeout(int value);

protected:
    TSocketDomain m_Domain;
    TSocketType m_Type;
    int m_Protocol;
    int m_Handle;
    socklen_t	m_local_len;
    socklen_t	m_remote_len;
    void *GetAddress(TAddressKind akind);
    void *SetAddress(TAddressKind akind, void *sa, int sa_len);

public:
    int ConnectTimeout;

    __fastcall TSocket();
    __fastcall TSocket(TSocket *owner, int hsock, void *sa, int sa_len);
    virtual ~TSocket();
    void Open(TSocketDomain domain, TSocketType type, int protocol);
    virtual inline void Open()
    {
        Open(m_Domain, m_Type, m_Protocol);
    };
    virtual void Close();
    void Close(int no);
    void Close(TSocket *sock);
    void Accept();
    void Connect();
    void Disconnect(int how = SHUT_RDWR);
    int GetOptions(const int level, const int optname, void *opt);
    inline int GetOptions(const int level, const int optname)
    {
        int opt;
        GetOptions(level, optname, &opt);
        return opt;
    };
    void SetOptions(const int level, const int optname, const void *value, int len);
    inline void SetOptions(const int level, const int optname, int value)
    {
        SetOptions(level, optname, &value, sizeof(value));
    };
    void Read(void *buf, size_t len);
    void Write(const void *buf, size_t len);

    TAcceptEvent OnAccept;
    TNotifyEvent OnConnect;
    TNotifyEvent OnDisconnect;
    /*
      TServerType = (stNonBlocking, stThreadBlocking);
      TClientType = (ctNonBlocking, ctBlocking);
      TAsyncStyle = (asRead, asWrite, asOOB, asAccept, asConnect, asClose);
      TAsyncStyles = set of TAsyncStyle;
      TSocketEvent = (seLookup, seConnecting, seConnect, seDisconnect, seListen,
        seAccept, seWrite, seRead);
      TLookupState = (lsIdle, lsLookupAddress, lsLookupService);
      TErrorEvent = (eeGeneral, eeSend, eeReceive, eeConnect, eeDisconnect, eeAccept, eeLookup);

      TSocketEventEvent = procedure (Sender: TObject; Socket: TCustomWinSocket;
        SocketEvent: TSocketEvent) of object;
      TSocketErrorEvent = procedure (Sender: TObject; Socket: TCustomWinSocket;
        ErrorEvent: TErrorEvent; var ErrorCode: Integer) of object;
      TGetSocketEvent = procedure (Sender: TObject; Socket: TSocket;
        var ClientSocket: TServerClientWinSocket) of object;
      TGetThreadEvent = procedure (Sender: TObject; ClientSocket: TServerClientWinSocket;
        var SocketThread: TServerClientThread) of object;
      TSocketNotifyEvent = procedure (Sender: TObject; Socket: TCustomWinSocket) of object;

      TServerSocket = class(TCustomServerSocket)
      public
        constructor Create(AOwner: TComponent); override;
        property Socket: TServerWinSocket read FServerSocket;
      published
        property Active;
        property Port;
        property Service;
        property ServerType;
        property ThreadCacheSize default 10;
        property OnGetThread;
        property OnGetSocket;
        property OnThreadStart;
        property OnThreadEnd;
        property OnClientConnect;
        property OnClientDisconnect;
        property OnClientRead;
        property OnClientWrite;
        property OnClientError;
      end;

        TClientSocket = class(TCustomSocket)
      private
        FClientSocket: TClientWinSocket;
      protected
        procedure DoActivate(Value: Boolean); override;
        function GetClientType: TClientType;
        procedure SetClientType(Value: TClientType);
      public
        constructor Create(AOwner: TComponent); override;
        destructor Destroy; override;
        property Socket: TClientWinSocket read FClientSocket;
      published
        property Active;
        property Address;
        property ClientType: TClientType read GetClientType write SetClientType;
        property Host;
        property Port;
        property Service;
        property OnLookup;
        property OnConnecting;
        property OnRead;
        property OnWrite;
        property OnError;
      end;

        property OnLookup: TSocketNotifyEvent read FOnLookup write FOnLookup;
        property OnConnecting: TSocketNotifyEvent read FOnConnecting write FOnConnecting;
        property OnRead: TSocketNotifyEvent read FOnRead write FOnRead;
        property OnWrite: TSocketNotifyEvent read FOnWrite write FOnWrite;
        property OnError: TSocketErrorEvent read FOnError write FOnError;
     */
    property (m, int, Backlog);
    property (rw, bool, Broadcast);
    property (m, bool, Connected);
    property_i (r, TSocket *, int, Connections);
    property (r, short, ConnectionCount);
    property (m, TSocketDomain, Domain);
    property (rw, bool, DontRoute);
    property (mr, int, Handle);
    property (m, TSocketKind, Kind);
    property (rw, bool, KeepAlive);
    property (rw, int, Linger);
    property (r, bool, Listening);
    property (rw, bool, OOBinline);
    property (rw, int, Priority);
    property (m, int, Protocol);
    property (rw, int, RecvBufsize);
    property (rw, int, RecvBufforce);
    property (rw, int, RecvLowat);
    property (rw, int, RecvTimeout);
    property (rw, bool, Reuseaddr);
    property (rw, int, SendBufsize);
    property (rw, int, SendBufforce);
    property (rw, int, SendLowat);
    property (rw, int, SendTimeout);
    property (r, int, SocketError);
    property (m, TSocketType, Type);
};
//---------------------------------------------------------------------------
/*
 *
#define SO_NO_CHECK	11
#define SO_PRIORITY	12

 Security levels - as per NRL IPv6 - don't actually do anything
#define SO_SECURITY_AUTHENTICATION		22
#define SO_SECURITY_ENCRYPTION_TRANSPORT	23
#define SO_SECURITY_ENCRYPTION_NETWORK		24

 Socket filtering
#define SO_ATTACH_FILTER        26
#define SO_DETACH_FILTER        27

#define SO_PEERNAME		28
#define SO_TIMESTAMP		29
#define SCM_TIMESTAMP		SO_TIMESTAMP

#define SO_PEERSEC		31
#define SO_PASSSEC		34
#define SO_TIMESTAMPNS		35
#define SCM_TIMESTAMPNS		SO_TIMESTAMPNS


 *   TAbstractSocket = class(TComponent)
  private
    FPort: Integer;
    FAddress: string;
    FHost: string;
    FService: string;
    procedure DoEvent(Sender: TObject; Socket: TCustomWinSocket;
      SocketEvent: TSocketEvent);
    procedure DoError(Sender: TObject; Socket: TCustomWinSocket;
      ErrorEvent: TErrorEvent; var ErrorCode: Integer);
  protected
    procedure Event(Socket: TCustomWinSocket; SocketEvent: TSocketEvent);
      virtual; abstract;
    procedure Error(Socket: TCustomWinSocket; ErrorEvent: TErrorEvent;
      var ErrorCode: Integer); virtual; abstract;

    procedure InitSocket(Socket: TCustomWinSocket);

    procedure SetAddress(Value: string);
    procedure SetHost(Value: string);
    procedure SetPort(Value: Integer);
    procedure SetService(Value: string);

    property Address: string read FAddress write SetAddress;
    property Host: string read FHost write SetHost;
    property Port: Integer read FPort write SetPort;
    property Service: string read FService write SetService;

  end;
  *
  * struct sockaddr {
    sa_family_t sa_family;
    char        sa_data[14];
}

struct sockaddr_in {
    sa_family_t    sin_family;  address family: AF_INET
    in_port_t      sin_port;    port in network byte order
    struct in_addr sin_addr;    internet address
};

 Internet address.
struct in_addr {
    uint32_t       s_addr;      address in network byte order
};

// *  */
//---------------------------------------------------------------------------
//	TLocalSocket
//---------------------------------------------------------------------------
class TLocalSocket : public TSocket
{
    DECLARE_PROPERTY(TLocalSocket);

private:
    TString get_Address();
    void set_Address(TString value);
    inline bool get_PassCred()
    {
        return bool( GetOptions(SOL_SOCKET, SO_PASSCRED) );
    };
    inline void set_PassCred(bool value)
    {
        SetOptions(SOL_SOCKET, SO_PASSCRED, value);
    };
    struct ucred get_PeerCred();

protected:

public:
    __fastcall TLocalSocket();
    ~TLocalSocket();

    property (rw, TString, Address);
    property (rw, bool, PassCred);
    property (r, struct ucred, PeerCred);
};
//---------------------------------------------------------------------------
//	TInetSocket
//---------------------------------------------------------------------------
enum TTOSValue
{
    tosLowdelay = IPTOS_LOWDELAY,
    tosThroughput = IPTOS_THROUGHPUT,
    tosReliability = IPTOS_RELIABILITY,
    tosLowcost = IPTOS_LOWCOST
};

class TInetSocket : public TSocket
{
    DECLARE_PROPERTY(TInetSocket);

private:
    int get_LocalPort();
    void set_LocalPort(int value);
    int get_RemotePort();
    void set_RemotePort(int value);
    TString get_DeviceName();
    void set_DeviceName(TString value);
    TString get_LocalAddress();
    void set_LocalAddress(TString value);
    TString get_RemoteAddress();
    void set_RemoteAddress(TString value);
    TString get_LocalHost();
    void set_LocalHost(TString value);
    TString get_RemoteHost();
    void set_RemoteHost(TString value);
    TString get_LocalService();
    void set_LocalService(TString value);
    TString get_RemoteService();
    void set_RemoteService(TString value);
    inline bool get_Debug()
    {
        return bool( GetOptions(SOL_SOCKET, SO_DEBUG) );
    };
    inline void set_Debug(bool value)
    {
        SetOptions(SOL_SOCKET, SO_DEBUG, value);
    };
    inline bool get_IncludeHeader()
    {
        return bool( GetOptions(IPPROTO_IP, IP_HDRINCL) );
    };
    inline void set_IncludeHeader(bool value)
    {
        SetOptions(IPPROTO_IP, IP_HDRINCL, value);
    };
    inline bool get_Mark()
    {
        return bool( GetOptions(SOL_SOCKET, SO_MARK) );
    };
    inline void set_Mark(bool value)
    {
        SetOptions(SOL_SOCKET, SO_MARK, value);
    };
    inline bool get_Nodelay()
    {
        return bool( GetOptions(IPPROTO_TCP, TCP_NODELAY) );
    };
    inline void set_Nodelay(bool value)
    {
        SetOptions(IPPROTO_TCP, TCP_NODELAY, value);
    };
    inline TTOSValue get_TOS()
    {
        return TTOSValue( GetOptions(IPPROTO_IP, IP_TOS) );
    };
    inline void set_TOS(TTOSValue value)
    {
        SetOptions(IPPROTO_IP, IP_TOS, value);
    };
    inline int get_TTL()
    {
        return GetOptions(IPPROTO_IP, IP_TTL);
    };
    inline void set_TTL(int value)
    {
        SetOptions(IPPROTO_IP, IP_TTL, value);
    };
    inline int get_Maxseg()
    {
        return GetOptions(IPPROTO_TCP, TCP_MAXSEG);
    };
    inline void set_Maxseg(int value)
    {
        SetOptions(IPPROTO_TCP, TCP_MAXSEG, value);
    };

protected:

public:
    __fastcall TInetSocket();

    property (rw, bool, 	Debug);
    property (rw, TString, 	DeviceName);
    property (rw, bool, 	IncludeHeader);
    property (rw, TString, 	LocalAddress);
    property (rw, TString, 	LocalHost);
    property (rw, int, 		LocalPort);
    property (rw, TString, 	LocalService);
    property (rw, bool, 	Mark);
    property (rw, int, 		Maxseg);
    property (rw, bool, 	Nodelay);
    property (rw, TString, 	RemoteAddress);
    property (rw, TString, 	RemoteHost);
    property (rw, int, 		RemotePort);
    property (rw, TString, 	RemoteService);
    property (rw, TTOSValue,TOS);
    property (rw, int, 		TTL);
};
//---------------------------------------------------------------------------
//	TTCPSocket
//---------------------------------------------------------------------------
class TTCPSocket : public TInetSocket
{
    DECLARE_PROPERTY(TTCPSocket);

private:

protected:

public:
    __fastcall TTCPSocket();

};
//---------------------------------------------------------------------------
//	TUDPSocket
//---------------------------------------------------------------------------
class TUDPSocket : public TInetSocket
{
    DECLARE_PROPERTY(TUDPSocket);

private:

protected:

public:
    __fastcall TUDPSocket();

};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
