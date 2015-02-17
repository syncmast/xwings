//---------------------------------------------------------------------------
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "lnxfile.h"
//---------------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//  TFile
//---------------------------------------------------------------------------
TFile :: TFile()
{
}
//---------------------------------------------------------------------------
TFile :: TFile(const TString &pathname)
{
    m_Name = pathname;
}
//---------------------------------------------------------------------------
TFile :: ~TFile()
{
    Close();
}
//---------------------------------------------------------------------------
void TFile :: GetFileInfo(void *info)
{
    memset(info, 0, sizeof(struct stat));
    if(m_Handle)
        fstat(m_Handle, (struct stat*)info);
    else
        stat(m_Name.c_str(), (struct stat*)info);
}
//---------------------------------------------------------------------------
void TFile :: set_Name(TString value)
{
    if(value != m_Name)
    {
        Close();
        m_Name = value;
    }
}
//---------------------------------------------------------------------------
size_t TFile :: get_Size()
{
    struct stat buf;
    GetFileInfo(&buf);
    return buf.st_size;
}
//---------------------------------------------------------------------------
void TFile :: set_Size(size_t value)
{
    if(value != get_Size())
    {
        if(m_Handle)
            ftruncate(m_Handle, value);
        else
            truncate(m_Name.c_str(), value);
    }
}
//---------------------------------------------------------------------------
size_t TFile :: get_Pos()
{
    size_t ret;
    ret = lseek(m_Handle, 0, SEEK_CUR);
    return ret;
}
//---------------------------------------------------------------------------
void TFile :: set_Pos(size_t value)
{
    if(value != get_Size())
    {
        lseek(m_Handle, value, SEEK_SET);
    }
}
//---------------------------------------------------------------------------
size_t TFile :: get_SizeOnDisk()
{
    struct stat buf;
    GetFileInfo(&buf);
    return buf.st_blksize * buf.st_blocks;
}
//---------------------------------------------------------------------------
time_t TFile :: get_AccessTime()
{
    struct stat buf;
    GetFileInfo(&buf);
    return buf.st_atime;
}
//---------------------------------------------------------------------------
time_t TFile :: get_ChangeTime()
{
    struct stat buf;
    GetFileInfo(&buf);
    return buf.st_ctime;
}
//---------------------------------------------------------------------------
time_t TFile :: get_ModifyTime()
{
    struct stat buf;
    GetFileInfo(&buf);
    return buf.st_mtime;
}
//---------------------------------------------------------------------------
int TFile :: get_NLink()
{
    struct stat buf;
    GetFileInfo(&buf);
    return buf.st_nlink;
}
//---------------------------------------------------------------------------
TFileType TFile :: get_Type()
{
    TFileType ret;
    struct stat buf;
    GetFileInfo(&buf);
    if(S_ISLNK(buf.st_mode))
        ret = ftLink;
    else if(S_ISREG(buf.st_mode))
        ret = ftFile;
    else if(S_ISDIR(buf.st_mode))
        ret = ftDir;
    else if(S_ISCHR(buf.st_mode))
        ret = ftChar;
    else if(S_ISBLK(buf.st_mode))
        ret = ftBlock;
    else if(S_ISFIFO(buf.st_mode))
        ret = ftFifo;
    else if(S_ISSOCK(buf.st_mode))
        ret = ftSocket;
    return ret;
}
//---------------------------------------------------------------------------
void TFile :: Close()
{
    int ret;
    if(m_Handle)
    {
        fsync(m_Handle);
        ret = close(m_Handle);
        if(ret)
            return;
        m_Handle = 0;
    }
    m_Name.Clear();
}
//---------------------------------------------------------------------------
void TFile :: Create(TString &pathname, mode_t mode)
{
    Name = pathname;
    m_Handle = open(m_Name.c_str(), O_CREAT | O_WRONLY | O_TRUNC, mode);
}
//---------------------------------------------------------------------------
void TFile :: Open(TString &pathname, int flags)
{
    Name = pathname;
    m_Handle = open(m_Name.c_str(), flags);
}
//---------------------------------------------------------------------------
off_t TFile :: Seek(off_t offset, int whence)
{
    off_t ret;
    ret = lseek(m_Handle, offset, whence);
    return ret;
}
//---------------------------------------------------------------------------
void TFile :: Sync()
{
    fsync(m_Handle);
}
//---------------------------------------------------------------------------
void TFile :: Read(void *buf, size_t len)
{
    read(m_Handle, buf, len);
}
//---------------------------------------------------------------------------
void TFile :: Write(const void *buf, size_t len)
{
    write(m_Handle, buf, len);
}
//---------------------------------------------------------------------------
//	TDirectory
//---------------------------------------------------------------------------
TDirectory :: TDirectory()
{
    m_handle = 0;
}
//---------------------------------------------------------------------------
TDirectory :: ~TDirectory()
{
    Close();
}
//---------------------------------------------------------------------------
TString TDirectory :: get_CurrentDir()
{
    TString ret;
    char *tmp = getcwd(NULL, 0);
    ret = tmp;
    free(tmp);
    return ret;
}
//---------------------------------------------------------------------------
void TDirectory :: set_CurrentDir(TString value)
{
    chdir(value.c_str());
//	fchdir(m_Handle);
}
//---------------------------------------------------------------------------
void TDirectory :: ChangeRoot(TString &path)
{
    chroot(path.c_str());
    chdir("/");
}
//---------------------------------------------------------------------------
void TDirectory :: Close()
{
    if(m_handle)
    {
        closedir(m_handle);
        m_handle = NULL;
    }
}
//---------------------------------------------------------------------------
void TDirectory :: Create(TString &path, mode_t mode)
{
    mkdir(path.c_str(), mode);
}
//---------------------------------------------------------------------------
void TDirectory :: Delete(TString &path)
{
    rmdir(path.c_str());  // должен быть пустой каталог
}
//---------------------------------------------------------------------------
void TDirectory :: Open(TString &pathname)
{
    m_handle = opendir(pathname.c_str());
}
//---------------------------------------------------------------------------
TString TDirectory :: Next()
{
    TString ret;
    if(m_handle)
    {
//		errno = 0;
        struct dirent *d = readdir(m_handle);
        if(d)
            ret = d->d_name;
    }
    return ret;
}
//---------------------------------------------------------------------------
void TDirectory :: Rewind()
{
    rewinddir(m_handle);
}
//---------------------------------------------------------------------------
//	TFileSet
//---------------------------------------------------------------------------
TFileSet :: TFileSet()
{

}
//---------------------------------------------------------------------------
TFileSet :: ~TFileSet()
{

}
//---------------------------------------------------------------------------
}  // namespace
//---------------------------------------------------------------------------------
