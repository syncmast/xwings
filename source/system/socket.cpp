//---------------------------------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "socket.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//	TSocket
//---------------------------------------------------------------------------
TSocket :: TSocket()
{
    m_Handle = 0;
    m_owner = NULL;
    m_Connected = false;
    m_Kind = skClient;
    m_local = NULL;
    m_remote = NULL;
    m_local_len = 0;
    m_remote_len = 0;
    m_Backlog = 50;
    m_Domain = pfUnspec;
    m_Type = stStream;
    m_Protocol = 0;
    ConnectTimeout = 0;
}
//---------------------------------------------------------------------------
TSocket :: TSocket(TSocket *owner, int hsock, void *sa, int sa_len)
{
    m_Handle = hsock;
    m_owner = owner;
    m_Connected = true;
    m_Kind = skClient;
//	m_Domain = TSocketDomain(((struct sockaddr *)sa)->sa_family);
    m_local = NULL;
    m_remote = NULL;
    m_local_len = 0;
    m_remote_len = 0;
    SetAddress(akRemote, sa, sa_len);
    m_Backlog = 0;
    ConnectTimeout = 0;

    int opt;
    socklen_t optlen;
    optlen = sizeof(opt);
    getsockopt(m_Handle, SOL_SOCKET, SO_DOMAIN, &opt, &optlen);
    m_Domain = (TSocketDomain)opt;

//	optlen = sizeof(opt);
    getsockopt(m_Handle, SOL_SOCKET, SO_TYPE, &opt, &optlen);
    m_Type = (TSocketType)opt;

//	optlen = sizeof(opt);
    getsockopt(m_Handle, SOL_SOCKET, SO_PROTOCOL, &opt, &optlen);
    m_Protocol = opt;
}
//---------------------------------------------------------------------------
TSocket :: ~TSocket()
{
    if(m_Handle)
        Close();
    delete m_local;
    delete m_remote;
}
//---------------------------------------------------------------------------
void TSocket :: set_Connected(bool value)
{
    if(value == m_Connected)
        return;
    if(value)
        Connect();
    else
        Disconnect();
}
//---------------------------------------------------------------------------
void TSocket :: set_Kind(TSocketKind value)
{
    if(value != m_Kind)
    {
        if(m_Connected)
            Close();
        m_Kind = value;
        if(m_Kind != skClient)
        {
            m_Backlog = 50;
            Reuseaddr = true;
        }
    };
}
//---------------------------------------------------------------------------
void TSocket :: set_Domain(TSocketDomain value)
{
    if(value != m_Domain)
    {
        if(m_Connected)
            Close();
        m_Domain = value;
    };
}
//---------------------------------------------------------------------------
void TSocket :: set_Protocol(int value)
{
    if(value != m_Protocol)
    {
        if(m_Connected)
            Close();
        m_Protocol = value;
    };
}
//---------------------------------------------------------------------------
void TSocket :: set_Type(TSocketType value)
{
    if(value != m_Type)
    {
        if(m_Connected)
            Close();
        m_Type = value;
    };
}
//---------------------------------------------------------------------------
void TSocket :: set_Backlog(int value)
{
    if(value > SOMAXCONN)
        value = SOMAXCONN;
    if(m_Kind != skClient && value != m_Backlog)
    {
        if(m_Connected)
            Close();
        m_Backlog = value;
    };
}
//---------------------------------------------------------------------------
int TSocket :: get_Linger()
{
    struct linger lg;
    socklen_t lgsize = sizeof(lg);

    getsockopt(m_Handle, SOL_SOCKET, SO_LINGER, &lg, &lgsize);
    return (lg.l_onoff == 0) ? LINGER_OFF : lg.l_linger;
}
//---------------------------------------------------------------------------
void TSocket :: set_Linger(int value)
{
    struct linger lg;

    switch(value)
    {
    case LINGER_OFF:
        lg.l_onoff = 0;
        lg.l_linger = 0;
        break;
    case 0:
        lg.l_onoff = 1;
        lg.l_linger = 0;
        break;
    default:
        if(value < 0)
            value = -value;
        lg.l_onoff = 1;
        lg.l_linger = value;
    }
    setsockopt(m_Handle, SOL_SOCKET, SO_LINGER, &lg, sizeof(lg));
}
//---------------------------------------------------------------------------
int TSocket :: get_RecvTimeout()
{
    struct timeval tv;
    socklen_t optlen = sizeof(tv);
    getsockopt(m_Handle, SOL_SOCKET, SO_RCVTIMEO, &tv, &optlen);
    return tv.tv_sec + tv.tv_usec / 1000;
}
//---------------------------------------------------------------------------
void TSocket :: set_RecvTimeout(int value)
{
    struct timeval tv;
    tv.tv_sec = value / 1000;
    tv.tv_usec = (value % 1000) * 1000;
    setsockopt(m_Handle, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}
//---------------------------------------------------------------------------
int TSocket :: get_SendTimeout()
{
    struct timeval tv;
    socklen_t optlen = sizeof(tv);
    getsockopt(m_Handle, SOL_SOCKET, SO_SNDTIMEO, &tv, &optlen);
    return tv.tv_sec + tv.tv_usec / 1000;
}
//---------------------------------------------------------------------------
void TSocket :: set_SendTimeout(int value)
{
    struct timeval tv;
    tv.tv_sec = value / 1000;
    tv.tv_usec = (value % 1000) * 1000;
    setsockopt(m_Handle, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
}
//---------------------------------------------------------------------------
void TSocket :: Open(TSocketDomain domain, TSocketType type, int protocol)
{
    if(m_Handle == 0)
        m_Handle = socket(domain, type, protocol);
    if(m_Kind != skClient)
        Reuseaddr = true;
}
//---------------------------------------------------------------------------
void TSocket :: Close()
{
    if(m_Handle == 0)
        return;
    if(m_Kind != skClient)
    {
        TSocket *sok;

        for(TList::iterator from = m_Connections.begin();
                from != m_Connections.end();
                ++from)
        {
            sok = (TSocket *)(*from);
            delete sok;
        }
        m_Connections.Clear();
    }
    close(m_Handle);
    m_Handle = 0;
    if(m_owner)
    {
        m_owner->Close(this);
    }
}
//---------------------------------------------------------------------------
void TSocket :: Close(TSocket *sock)
{
    if(m_Handle == 0)
        return;
    if(m_Kind != skClient)
    {
        m_Connections.Delete(sock);
        delete sock;
    }
}
//---------------------------------------------------------------------------
void TSocket :: Close(int no)
{
    if(m_Handle == 0)
        return;
    if(m_Kind != skClient)
    {
        delete Connections[no];
        m_Connections.Delete(no);
    }
}
//---------------------------------------------------------------------------
void TSocket :: Accept()
{
    if(m_Handle == 0 || m_Kind == skClient || m_Type == stDgram)
        return;

    if(m_remote == NULL)
        SetAddress(akRemote, NULL, m_local_len);

    int clisok;
    printf("Accept begin\n");
    clisok = accept(m_Handle, m_remote, &m_remote_len);
    if(clisok == -1)
        return;
    printf("Accept end\n");

    TSocket *s = new TSocket(this, clisok, m_remote, m_remote_len);
    m_Connections.Add(s);

    if(m_Kind == skServerProcess)
    {
        int ret = fork();
        if(ret == 0)
        {
            close(m_Handle);
            if(!OnAccept.empty())
                OnAccept(this, s);
            _exit(EXIT_SUCCESS);
        }
        close(clisok);
    }
    else
    {
        if(!OnAccept.empty())
            OnAccept(this, s);
    }

    char buff[INET_ADDRSTRLEN];
    struct sockaddr_in *sa = (struct sockaddr_in *)m_remote;
    printf("ACCEPT: Connection from %s, port %d\n",
           inet_ntop(AF_INET, &sa->sin_addr, buff, INET_ADDRSTRLEN),
           ntohs(sa->sin_port));

}
//---------------------------------------------------------------------------
static int __socket_ready(int fd, int msec)
{
    fd_set rset;
    struct timeval tv;

    FD_ZERO(&rset);
    FD_SET(fd, &rset);

    tv.tv_sec = msec / 1000;
    tv.tv_usec = (msec % 1000) * 1000;

    return select(fd + 1, &rset, NULL, NULL, &tv);
}

void TSocket :: Connect()
{
    int ret = 0;

    if(m_Handle == 0)
        return;
    m_Connected = false;
    if(m_local)
        ret = bind(m_Handle, m_local, m_local_len);

    if(m_Kind != skClient)
    {
        if(m_Type == stStream || m_Type == stSeqPacket)
            ret = listen(m_Handle, m_Backlog);
    }
    else
    {
        if(m_Type != stDgram)
            if(__socket_ready(m_Handle, ConnectTimeout) <= 0)
                return;
        ret = connect(m_Handle, m_remote, m_remote_len);
        if(ret)
        {
            Close();
            Open();
            if(m_local)
                bind(m_Handle, m_local, m_local_len);
            m_Connected = false;
        }
    }
    m_Connected = true;
    if(!OnConnect.empty())
        OnConnect(this);
}
//---------------------------------------------------------------------------
void TSocket :: Disconnect(int how)
{
    if(m_Handle == 0)
        return;
    shutdown(m_Handle, how);
    m_Connected = false;
    if(!OnDisconnect.empty())
        OnDisconnect(this);
}
//---------------------------------------------------------------------------
void * TSocket :: GetAddress(TAddressKind akind)
{
    void *ret;

    if(akind == akLocal)
    {
        if(m_Connected)
        {
            if(m_local == NULL)
            {
                m_local_len = m_remote_len;
                m_local = (struct sockaddr *)new byte_t[m_local_len];
                memset(m_local, 0, m_local_len);
            }
            getsockname(m_Handle, m_local, &m_local_len);
        }
        ret = m_local;
    }
    else
    {
        if(m_Connected)
        {
            if(m_remote == NULL)
            {
                m_remote_len = m_local_len;
                m_remote = (struct sockaddr *)new byte_t[m_remote_len];
                memset(m_remote, 0, m_remote_len);
            }
            getpeername(m_Handle, m_remote, &m_remote_len);
        }
        ret = m_remote;
    }
    return ret;
}
//---------------------------------------------------------------------------
void * TSocket :: SetAddress(TAddressKind akind, void *sa, int sa_len)
{
    byte_t *b = NULL;
    if(sa_len)
    {
        b = new byte_t[sa_len];
        memset(b, 0, sa_len);
        if(sa)
            memcpy(b, sa, sa_len);
    }
    if(akind == akLocal)
    {
        delete m_local;
        m_local = (struct sockaddr *)b;
        m_local_len = sa_len;
    }
    else
    {
        delete m_remote;
        m_remote = (struct sockaddr *)b;
        m_remote_len = sa_len;
    }
    return b;
}
//---------------------------------------------------------------------------
int TSocket :: GetOptions(const int level, const int optname, void *opt)
{
    socklen_t optlen;

    if(m_Handle)
        getsockopt(m_Handle, level, optname, opt, &optlen);
    return optlen;
}
//---------------------------------------------------------------------------
void TSocket :: SetOptions(const int level, const int optname, const void *value, int len)
{
    if(m_Handle)
        setsockopt(m_Handle, level, optname, value, len);
}
//---------------------------------------------------------------------------

ssize_t readn(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread = 0;
    char *ptr;

    ptr = (char *)vptr;
    nleft = n;
    while(nleft > 0)
    {
        printf("READN1 n=%lu nread=%ld\n", n, nread);
        nread = read(fd, ptr, nleft);
        printf("READN2 n=%lu nread=%ld\n",n, nread);
        if(nread < 0)
        {
            if(errno == EINTR)
                nread = 0;
            else
                return -1;
        }
        else if(nread == 0)
            break;
        nleft -= nread;
        ptr += nread;
    }
    return n - nleft;
}

void TSocket :: Read(void *buf, size_t len)
{
    int flags = MSG_WAITALL;
    ssize_t ret;

    if(m_Type == stDgram && !(m_Kind == skClient && m_Connected) )
    {
        if(m_remote == NULL)
            SetAddress(akRemote, NULL, m_local_len);
        ret = recvfrom(m_Handle, buf, len, flags, m_remote, &m_remote_len);
    }
    else
        ret = recv(m_Handle, buf, len, flags);
//	ssize_t ret = readn(m_Handle, buf, len);
}
//---------------------------------------------------------------------------
ssize_t writen(int fd, const void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = (const char *)vptr;
    nleft = n;
    while(nleft > 0)
    {
        nwritten = write(fd, ptr, nleft);
        if(nwritten <= 0)
        {
            if(errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

void TSocket :: Write(const void *buf, size_t len)
{
    int flags = 0;
    ssize_t ret;

    if(m_Type == stDgram && !(m_Kind == skClient && m_Connected) )
        ret = sendto(m_Handle, buf, len, flags, m_remote, m_remote_len);
    else
        ret = send(m_Handle, buf, len, flags);
    perror("ERRSRV");
//	ssize_t ret = writen(m_Handle, buf, len);
}
/*

#define MAXLINE 4096

	static ssize_t my_read(int fd, char *ptr)
	{
		static int read_cnt = 0;
		static char *read_ptr;
		static char read_buf[MAXLINE];

		if(read_cnt <= 0)
		{
		again:
			read_cnt = read(fd, read_buf, sizeof(read_buf));
			if(read_cnt < 0)
			{
				if(errno == EINTR)
					goto again;
				return -1;
			}
			else if(read_cnt == 0)
				return 0;
			read_ptr = read_buf;
		}
		read_cnt --;
		*ptr = *read_ptr++;

		return 1;
	}

	ssize_t readln(int fd, void *vptr, size_t maxlen)
	{
		size_t n, rc;
		char *ptr, c;

		ptr = vptr;
		for(n = 1; n < maxlen; n++)
		{
		again:
			rc = my_read(fd, &c);
			if(rc == 1)
			{
				*ptr++ = c;
				if(c == '\n')
					break;
			}
			else if(rc == 0)
			{
				if(n == 1)
					return 0;
				else
					break;
			}
			else
			{
				if(errno == EINTR)
					goto again;
				return -1;
			}
		}
		*ptr = '\0';
		return n;
	}
*/
ssize_t read_fd(int fd, void *ptr, size_t nbytes, int *recvfd)
{
    struct msghdr msg;
    struct iovec iov[1];
    ssize_t n;
//		int newfd;
    union
    {
        struct cmsghdr cm;
        char control[CMSG_SPACE(sizeof(int))];
    } control_un;
    struct cmsghdr *cmptr;

    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);
    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    if( (n = recvmsg(fd, &msg, 0)) <= 0 )
        return n;
    if( (cmptr = CMSG_FIRSTHDR(&msg)) != NULL &&
            cmptr->cmsg_len == CMSG_LEN(sizeof(int)) )
    {
        if(cmptr->cmsg_level != SOL_SOCKET)
            return -1; //err_quit("control level != SOL_SOCKET");
        if(cmptr->cmsg_type != SCM_RIGHTS)
            return -1; //err_quit("control type != SCM_RIGHTS");
//			*recvfd = *( (int *)CMSG_DATA(cmptr) );
        int *ptmp = (int *)CMSG_DATA(cmptr);
        *recvfd = *ptmp;
    }
    else
        *recvfd = -1;
    return n;
}

ssize_t write_fd(int fd, void *ptr, size_t nbytes, int sendfd)
{
    struct msghdr msg;
    struct iovec iov[1];
    union
    {
        struct cmsghdr cm;
        char control[CMSG_SPACE(sizeof(int))];
    } control_un;
    struct cmsghdr *cmptr;

    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.control);
    cmptr = CMSG_FIRSTHDR(&msg);
    cmptr->cmsg_len = CMSG_LEN(sizeof(int));
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;

    int *ptmp = (int *)CMSG_DATA(cmptr);
    *ptmp = sendfd;
//		*( (int *)CMSG_DATA(cmptr) ) = sendfd;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;

    iov[0].iov_base = ptr;
    iov[0].iov_len = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;

    return sendmsg(fd, &msg, 0);
}


// struct msghdr msg = {0};
// struct cmsghdr *cmsg;
// int myfds[NUM_FD]; /* Contains the file descriptors to pass. */
// char buf[CMSG_SPACE(sizeof myfds)];  /* ancillary data buffer */
// int *fdptr;

// msg.msg_control = buf;
// msg.msg_controllen = sizeof buf;
// cmsg = CMSG_FIRSTHDR(&msg);
// cmsg->cmsg_level = SOL_SOCKET;
// cmsg->cmsg_type = SCM_RIGHTS;
// cmsg->cmsg_len = CMSG_LEN(sizeof(int) * NUM_FD);
/* Initialize the payload: */
// fdptr = (int *)CMSG_DATA(cmsg);
// memcpy(fdptr, myfds, NUM_FD * sizeof(int));
/* Sum of the length of all control messages in the buffer: */
// msg.msg_controllen = cmsg->cmsg_len;


//---------------------------------------------------------------------------
//	TLocalSocket
//---------------------------------------------------------------------------
TLocalSocket :: TLocalSocket()
{
    m_Domain = pfLocal;
    m_Type = stStream;
}
//---------------------------------------------------------------------------
TLocalSocket :: ~TLocalSocket()   //???????????????
{
    struct sockaddr_un *sa;

    if(Kind != skClient)
    {
        sa = (struct sockaddr_un*)GetAddress(akLocal);
        printf("^^^^^^^^^^^^ %s\n", sa->sun_path);
        unlink(sa->sun_path);
        perror("^^^^^^^");
    }
}
//---------------------------------------------------------------------------
TString TLocalSocket :: get_Address()
{
    TString ret;
    struct sockaddr_un *sa;

    sa = (struct sockaddr_un *)GetAddress((Kind == skServer)? akLocal : akRemote);
    ret = sa ? sa->sun_path : "";
    return ret;
}
//---------------------------------------------------------------------------
void TLocalSocket :: set_Address(TString value)
{
    if(get_Address() == value)
        return;
    struct sockaddr_un sa;
    memset(&sa, 0, sizeof(struct sockaddr_un));
    sa.sun_family = AF_LOCAL;
    strncpy(sa.sun_path, value.c_str(), sizeof(sa.sun_path) - 1);
    if(Kind != skClient)
    {
        unlink(value.c_str());
        SetAddress(akLocal, &sa, SUN_LEN(&sa));
    }
    else
    {
        SetAddress(akRemote, &sa, SUN_LEN(&sa));
        strncpy(sa.sun_path, tmpnam(NULL), sizeof(sa.sun_path) - 1);
        SetAddress(akLocal, &sa, SUN_LEN(&sa));
    }
}
//---------------------------------------------------------------------------
struct ucred TLocalSocket :: get_PeerCred()
{
    struct ucred ret;
    socklen_t optlen = sizeof(ret);

    getsockopt(m_Handle, SOL_SOCKET, SO_PEERCRED, &ret, &optlen);
    return ret;
}
//---------------------------------------------------------------------------
//	TInetSocket
//---------------------------------------------------------------------------
TInetSocket :: TInetSocket()
{
    m_Domain = pfInet;
    m_Type = stStream;
}
//---------------------------------------------------------------------------
int TInetSocket :: get_LocalPort()
{
    int ret;
    struct sockaddr_in *sa;
    sa = (struct sockaddr_in *)GetAddress(akLocal);
    ret = sa ? ntohs(sa->sin_port) : 0;
    return ret;
}
//---------------------------------------------------------------------------
void TInetSocket :: set_LocalPort(int value)
{
    if(Connected || get_LocalPort() == value)
        return;
    struct sockaddr_in *sa;
    sa = (struct sockaddr_in *)GetAddress(akLocal);
    if(!sa)
        sa = (struct sockaddr_in *)SetAddress(akLocal, NULL, sizeof(struct sockaddr_in));
    if(sa)
    {
        sa->sin_family = AF_INET;
        sa->sin_port = htons(value);
    }
}
//---------------------------------------------------------------------------
int TInetSocket :: get_RemotePort()
{
    int ret;
    struct sockaddr_in *sa;
    sa = (struct sockaddr_in *)GetAddress(akRemote);
    ret = sa ? ntohs(sa->sin_port) : 0;
    return ret;
}
//---------------------------------------------------------------------------
void TInetSocket :: set_RemotePort(int value)
{
    if(Connected || get_RemotePort() == value)
        return;
    struct sockaddr_in *sa;
    sa = (struct sockaddr_in *)GetAddress(akRemote);
    if(!sa)
        sa = (struct sockaddr_in *)SetAddress(akRemote, NULL, sizeof(struct sockaddr_in));
    if(sa)
    {
        sa->sin_family = AF_INET;
        sa->sin_port = htons(value);
    }
}
//---------------------------------------------------------------------------
TString TInetSocket :: get_DeviceName()
{
    char str[IFNAMSIZ + 1];
    socklen_t optlen = sizeof(str);

    getsockopt(m_Handle, SOL_SOCKET, SO_BINDTODEVICE, &str, &optlen);
    return str;
}
//---------------------------------------------------------------------------
void TInetSocket :: set_DeviceName(TString value)
{
    if(get_DeviceName() == value)
        return;
    char str[IFNAMSIZ + 1];
    str[value.copy(str, IFNAMSIZ)] = '\0';
    setsockopt(m_Handle, SOL_SOCKET, SO_BINDTODEVICE, str, sizeof(str));
}
//---------------------------------------------------------------------------
TString TInetSocket :: get_LocalAddress()
{
    struct sockaddr_in *sa;
    const char *p;
    char str[INET_ADDRSTRLEN];
    memset(str, 0, sizeof(str));

    sa = (struct sockaddr_in *)GetAddress(akLocal);
    if(sa)
        p = inet_ntop(AF_INET, &sa->sin_addr, str, INET_ADDRSTRLEN);
    return str;
}
//---------------------------------------------------------------------------
void TInetSocket :: set_LocalAddress(TString value)
{
    if(get_LocalAddress() == value)
        return;
    struct sockaddr_in *psa;
    psa = (struct sockaddr_in *)GetAddress(akLocal);
    if(psa)
        inet_pton(AF_INET, value.c_str(), &psa->sin_addr.s_addr);
    else
    {
        struct sockaddr_in sa;
        memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        inet_pton(AF_INET, value.c_str(), &sa.sin_addr.s_addr);
        SetAddress(akLocal, &sa, sizeof(sa));
    }
}
//---------------------------------------------------------------------------
TString TInetSocket :: get_RemoteAddress()
{
    struct sockaddr_in *sa;
    const char *p;
    char str[INET_ADDRSTRLEN];
    memset(str, 0, sizeof(str));

    sa = (struct sockaddr_in *)GetAddress(akRemote);
    if(sa)
        p = inet_ntop(AF_INET, &sa->sin_addr, str, INET_ADDRSTRLEN);
    return str;
}
//---------------------------------------------------------------------------
void TInetSocket :: set_RemoteAddress(TString value)
{
    if(get_RemoteAddress() == value)
        return;
    struct sockaddr_in *psa;
    psa = (struct sockaddr_in *)GetAddress(akRemote);
    if(psa)
        inet_pton(AF_INET, value.c_str(), &psa->sin_addr.s_addr);
    else
    {
        struct sockaddr_in sa;
        memset(&sa, 0, sizeof(sa));
        sa.sin_family = AF_INET;
        inet_pton(AF_INET, value.c_str(), &sa.sin_addr.s_addr);
        SetAddress(akRemote, &sa, sizeof(sa));
    }
}
//---------------------------------------------------------------------------
TString TInetSocket :: get_LocalHost()
{
    struct sockaddr *sa;
    char buf[NI_MAXHOST];
    buf[0] = '\0';

    sa = (struct sockaddr *)GetAddress(akLocal);
    if(sa)
        getnameinfo(sa, m_local_len, buf, NI_MAXHOST, NULL, 0, 0);
    return buf;
}
//---------------------------------------------------------------------------
void TInetSocket :: set_LocalHost(TString value)
{
    if(get_LocalHost() == value)
        return;

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = m_Domain;
    hints.ai_socktype = m_Type;
    hints.ai_protocol = m_Protocol;

    TString svc = get_LocalService();

    int err = getaddrinfo(value.c_str(), svc.IsEmpty() ? NULL : svc.c_str(),
                          &hints, &res);

    SetAddress(akLocal, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
}
//---------------------------------------------------------------------------
TString TInetSocket :: get_RemoteHost()
{
    struct sockaddr *sa;
    char buf[NI_MAXHOST];
    buf[0] = '\0';

    sa = (struct sockaddr *)GetAddress(akRemote);
    if(sa)
        getnameinfo(sa, m_remote_len, buf, NI_MAXHOST, NULL, 0, 0);
    return buf;
}
//---------------------------------------------------------------------------
void TInetSocket :: set_RemoteHost(TString value)
{
    if(get_RemoteHost() == value)
        return;

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = m_Domain;
    hints.ai_socktype = m_Type;
    hints.ai_protocol = m_Protocol;

    TString svc = get_RemoteService();

    int err = getaddrinfo(value.c_str(), svc.IsEmpty() ? NULL : svc.c_str(),
                          &hints, &res);

    SetAddress(akRemote, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
}
//---------------------------------------------------------------------------
TString TInetSocket :: get_LocalService()
{
    struct sockaddr *sa;
    char buf[NI_MAXSERV];
    buf[0] = '\0';

    sa = (struct sockaddr *)GetAddress(akLocal);
    if(sa)
        getnameinfo(sa, m_local_len, NULL, 0, buf, NI_MAXSERV, 0);
    return buf;
}
//---------------------------------------------------------------------------
void TInetSocket :: set_LocalService(TString value)
{
    if(get_LocalService() == value)
        return;
    struct servent *sptr;
    const char *s;
    switch(m_Type)
    {
    case stStream:
        s = "tcp";
        break;
    case stDgram:
        s = "udp";
        break;
    default:
        s = NULL;
    }
    sptr = getservbyname(value.c_str(), s);
    if(sptr)
        set_LocalPort(ntohs(sptr->s_port));
}
//---------------------------------------------------------------------------
TString TInetSocket :: get_RemoteService()
{
    struct sockaddr *sa;
    char buf[NI_MAXSERV];
    buf[0] = '\0';

    sa = (struct sockaddr *)GetAddress(akRemote);
    if(sa)
        getnameinfo(sa, m_remote_len, NULL, 0, buf, NI_MAXSERV, 0);
    return buf;
}
//---------------------------------------------------------------------------
void TInetSocket :: set_RemoteService(TString value)
{
    if(get_RemoteService() == value)
        return;
    struct servent *sptr;
    const char *s;
    switch(m_Type)
    {
    case stStream:
        s = "tcp";
        break;
    case stDgram:
        s = "udp";
        break;
    default:
        s = NULL;
    }
    sptr = getservbyname(value.c_str(), s);
    if(sptr)
        set_RemotePort(ntohs(sptr->s_port));
}
//---------------------------------------------------------------------------
//	TTCPSocket
//---------------------------------------------------------------------------
TTCPSocket :: TTCPSocket()
{
    m_Type = stStream;
}
//---------------------------------------------------------------------------
//	TUDPSocket
//---------------------------------------------------------------------------
TUDPSocket :: TUDPSocket()
{
    m_Type = stDgram;
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
