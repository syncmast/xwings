//---------------------------------------------------------------------------
#ifndef lnxfileH
#define lnxfileH

#include <dirent.h>
#include "tobj.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//	TFile
//---------------------------------------------------------------------------
enum TFileType { ftLink, ftFile, ftDir, ftChar, ftBlock, ftFifo, ftSocket };

class TFile : public THandleObject
{
    DECLARE_PROPERTY(TFile);

private:
    TString m_Name;
    void set_Name(TString value);
    void GetFileInfo(void *info);
    size_t get_Size();
    void set_Size(size_t value);
    size_t get_Pos();
    void set_Pos(size_t value);
    size_t get_SizeOnDisk();
    time_t get_AccessTime();
    time_t get_ChangeTime();
    time_t get_ModifyTime();
    int get_NLink();
    TFileType get_Type();

protected:

public:
    __fastcall TFile();
    __fastcall TFile(const TString &pathname);
    ~TFile();
    void __fastcall Close();
    void __fastcall Create(TString &pathname, mode_t mode);
    void __fastcall Open(TString &pathname, int flags);
    void __fastcall Read(void *buf, size_t len);
    off_t __fastcall Seek(off_t offset, int whence);
    void __fastcall Sync();
    void __fastcall Write(const void *buf, size_t len);

    property (r, time_t, AccessTime);
    property (r, time_t, ChangeTime);
    property (r, time_t, ModifyTime);
    property (m, TString, Name);
    property (r, int, NLink);
    property (rw, size_t, Pos);
    property (rw, size_t, Size);
    property (r, size_t, SizeOnDisk);
    property (r, TFileType, Type);
};
//---------------------------------------------------------------------------
//	TDirectory
//---------------------------------------------------------------------------
class TDirectory : public TObject
{
    DECLARE_PROPERTY(TDirectory);

private:
    DIR *m_handle;
    TString get_CurrentDir();
    void set_CurrentDir(TString value);

protected:

public:
    TDirectory();
    ~TDirectory();
    void __fastcall ChangeRoot(TString &path);
    void  Close();
    void __fastcall Create(TString &path, mode_t mode);
    void __fastcall Delete(TString &path);
    void __fastcall Open(TString &path);
    TString Next();
    void Rewind();

    property (rw, TString, CurrentDir);
};
//---------------------------------------------------------------------------
//	TFileSet
//---------------------------------------------------------------------------
class TFileSet : public TObject
{
    DECLARE_PROPERTY(TFileSet);

private:

protected:

public:
    __fastcall TFileSet();
    ~TFileSet();

};
//---------------------------------------------------------------------------
}  //namespace
//---------------------------------------------------------------------------
#endif
