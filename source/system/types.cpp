//---------------------------------------------------------------------------
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <math.h>
#include <values.h>
#include <stdarg.h>
#include "vxutil.h"
#include "types.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
static const char *__trim_ch = "\b\t ";
static const wchar_t *__ltrim_ch = L"\b\t ";
//---------------------------------------------------------------------------
TMessage :: TMessage()
{
    memset(this, 0, sizeof(TMessage));
}
//---------------------------------------------------------------------------
//  TString
//---------------------------------------------------------------------------
__fastcall TString :: TString(const wchar_t *src)
{
    char *st;
    wcs2mbs(&st, src);
    *this = st;
    delete [] st;
}
//---------------------------------------------------------------------------
__fastcall TString :: TString(const TWString &src)
{
    char *st;
    wcs2mbs(&st, src.c_str());
    *this = st;
    delete [] st;
}
//---------------------------------------------------------------------------
__fastcall TString const & TString :: operator=(const wchar_t *rvalue)
{
    *this = TString(rvalue);
    return *this;
}
//---------------------------------------------------------------------------
TString __fastcall TString :: convert_case(bool ucase) const
{
    wchar_t *wt;
    mbs2wcs(&wt, c_str());
    wstring wtmp = wt;
    delete [] wt;
    transform( wtmp.begin(), wtmp.end(), wtmp.begin(), ucase ? ::towupper : ::towlower );
    char *st;
    wcs2mbs(&st, wtmp.c_str());
    string tmp = st;
    delete [] st;
    return tmp;
}
//---------------------------------------------------------------------------
TString & __fastcall TString :: printf(const char* format, ...)
{
    char *p;
    int r;
    va_list ap;

    va_start(ap, format);
    r = vasprintf(&p, format, ap);
    va_end(ap);
    if(r == -1)
        *this = "";
    else
        *this = p;
    free(p);
    return *this;
}
//---------------------------------------------------------------------------
int __fastcall TString :: ToInt(int defaultValue, int base) const
{
    const char *str;
    char *endptr;
    long val;
    bool bad = false;

    errno = 0;
    str = c_str();
    val = strtol(str, &endptr, base);

    /* Check for various possible errors */

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
            || (errno != 0 && val == 0))
    {
        bad = true;
    }
    else if (endptr == str)
    {
        bad = true;
    }
    else if (*endptr != '\0')        /* Not necessarily an error... */
    {
        bad = true;
    }
    return bad ? defaultValue : val;
}
//---------------------------------------------------------------------------
double TString :: ToDouble() const
{
    const char *str;
    char *endptr;
    double val;
    bool bad = false;

    errno = 0;
    str = c_str();
    val = strtod(str, &endptr);

    /* Check for various possible errors */

    if ((errno == ERANGE && (val == HUGE_VAL || val == -HUGE_VAL))
            || (errno != 0 && val == 0))
    {
        bad = true;
    }
    else if (endptr == str)
    {
        bad = true;
    }
    else if (*endptr != '\0')        /* Not necessarily an error... */
    {
        bad = true;
    }
    return bad ? HUGE_VAL : val;
}
//---------------------------------------------------------------------------
TString & TString :: TrimLeft()
{
    size_type on = string :: find_first_not_of(__trim_ch);
    if(on != string :: npos)
        string :: erase(0, on);
    return *this;
}
//---------------------------------------------------------------------------
TString & TString :: TrimRight()
{
    size_type on = string :: find_last_not_of(__trim_ch);
    if(on != string :: npos)
        string :: erase(on + 1);
    return *this;
}
//---------------------------------------------------------------------------
//  TWString
//---------------------------------------------------------------------------
__fastcall TWString :: TWString(const char *src)
{
    wchar_t *wt;
    mbs2wcs(&wt, src);
    *this = wt;
    delete [] wt;
}
//---------------------------------------------------------------------------
__fastcall TWString :: TWString(const TString &src)
{
    wchar_t *wt;
    mbs2wcs(&wt, src.c_str());
    *this = wt;
    delete [] wt;
}
//---------------------------------------------------------------------------
__fastcall TWString const & TWString :: operator=(const char *rvalue)
{
    *this = TWString(rvalue);
    return *this;
}
//---------------------------------------------------------------------------
TWString __fastcall TWString :: convert_case(bool ucase) const
{
    wstring wtmp = *this;
    transform( wtmp.begin(), wtmp.end(), wtmp.begin(), ucase ? ::towupper : ::towlower );
    return wtmp;
}
//---------------------------------------------------------------------------
TWString & __fastcall TWString :: printf(const wchar_t* format, ...)
{
    wchar_t *p, *t;
    int size = 256;
    int r;
    va_list ap;

    size *= sizeof(wchar_t);
    p = (wchar_t  *)malloc(size);
    while (1)
    {
        va_start(ap, format);
        r = vswprintf(p, size, format, ap);
        va_end(ap);
        if (r > 0 && r < size)
            break;
        /* Иначе попытаемся еще раз с удвоенным количеством
           символов */
        size *= 2;
        t =  (wchar_t  *)realloc(p, size);
        if(t == NULL)
        {
            free(p);
            *this = L"";
            return *this;
        }
        p = t;
    }
    *this = p;
    free(p);
    return *this;
}
//---------------------------------------------------------------------------
int __fastcall TWString :: ToInt(int defaultValue, int base) const
{
    const wchar_t *str;
    wchar_t *endptr;
    long val;
    bool bad = false;

    errno = 0;
    str = c_str();
    val = wcstol(str, &endptr, base);

    /* Check for various possible errors */

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
            || (errno != 0 && val == 0))
    {
        bad = true;
    }
    else if (endptr == str)
    {
        bad = true;
    }
    else if (*endptr != L'\0')        /* Not necessarily an error... */
    {
        bad = true;
    }
    return bad ? defaultValue : val;
}
//---------------------------------------------------------------------------
double TWString :: ToDouble() const
{
    const wchar_t *str;
    wchar_t *endptr;
    double val;
    bool bad = false;

    errno = 0;
    str = c_str();
    val = wcstod(str, &endptr);

    /* Check for various possible errors */

    if ((errno == ERANGE && (val == HUGE_VAL || val == -HUGE_VAL))
            || (errno != 0 && val == 0))
    {
        bad = true;
    }
    else if (endptr == str)
    {
        bad = true;
    }
    else if (*endptr != L'\0')        /* Not necessarily an error... */
    {
        bad = true;
    }
    return bad ? HUGE_VAL : val;
}
//---------------------------------------------------------------------------
TWString & TWString :: TrimLeft()
{
    size_type on = wstring :: find_first_not_of(__ltrim_ch);
    if(on != wstring :: npos)
        wstring :: erase(0, on);
    return *this;
}
//---------------------------------------------------------------------------
TWString & TWString :: TrimRight()
{
    size_type on = wstring :: find_last_not_of(__ltrim_ch);
    if(on != wstring :: npos)
        wstring :: erase(on + 1);
    return *this;
}
/*
//---------------------------------------------------------------------------
//  List
//---------------------------------------------------------------------------
TList :: TList()
{
}
//---------------------------------------------------------------------------
TList :: ~TList()
{
   Clear();
}
//---------------------------------------------------------------------------
void * TList :: get_Items(const int index)
{
   if(index >= 0 && index < Count)
   {
      int i = 0;
      for(TList::iterator from = begin();
            from != end();
            ++from, i++)
      {
         if(i == index)
         {
            return *from;
         }
      }
   }
   return NULL;
}
//---------------------------------------------------------------------------
void TList :: set_Items(const int index, void *item)
{
   if(index >= 0 && index < Count)
   {
      int i = 0;
      for(TList::iterator from = begin();
            from != end();
            ++from, i++)
      {
         if(i == index)
         {
            *from = item;
            return;
         }
      }
   }
}
//---------------------------------------------------------------------------
void TList :: Exchange(int index1, int index2)
{
  void *item1, *item2;

  if (index1 < 0 || index1 >= Count ||
      index2 < 0 || index2 >= Count)
    return;
  item1 = Items[index1];
  item2 = Items[index2];
  Items[index1] = item2;
  Items[index2] = item1;
}
//---------------------------------------------------------------------------
void TList :: Move(int newindex, int curindex)
{
  void *item;

  if (curindex != newindex)
  {
    if (newindex < 0 || newindex >= Count)
      return;
    item = Items[curindex];
    Items[curindex] = NULL;
    Delete(curindex);
    Insert(newindex, NULL);
    Items[newindex] = item;
  }
}
//---------------------------------------------------------------------------
void TList :: Delete(int index)
{
   int i = 0;
   for(TList::iterator from = begin();
            from != end();
            ++from, i++)
   {
      if(i == index)
      {
         erase(from);
         return;
      }
   }
}
//---------------------------------------------------------------------------
void TList :: Insert(int index, void *item)
{
   int i = 0;
   for(TList::iterator from = begin();
            from != end();
            ++from, i++)
   {
      if(i == index)
      {
         insert(from, item);
         return;
      }
   }
}
//---------------------------------------------------------------------------
int TList :: IndexOf(void *item)
{
   int i = 0;
   for(TList::iterator from = begin();
            from != end();
            ++from, i++)
   {
      if(*from == item)
         return i;
   }

   return -1;
}
*/
//---------------------------------------------------------------------------
// StringList
//---------------------------------------------------------------------------
/*
TStrings :: TStrings(): Delimiter(','),
   						QuoteChar('"'),
   						CommentChar('#')
{
}
//---------------------------------------------------------------------------
TStrings :: TStrings(char **strings, int nstrings) : Delimiter(','),
   						QuoteChar('"'),
   						CommentChar('#')
{
   Add(strings, nstrings);
}
//---------------------------------------------------------------------------
const TString & TStrings :: get_Strings(const int index)
{
   if(index >= 0 && index < Count)
   {
      int i = 0;
      for(TStrings::iterator from = begin();
            from != end();
            ++from, i++)
      {
         if(i == index)
         {
            return *from;
         }
      }
   }
   return *end();
}
//---------------------------------------------------------------------------
void TStrings :: set_Strings(const int index, const TString &s)
{
   if(index >= 0 && index < Count)
   {
      int i = 0;
      for(TStrings::iterator from = begin();
            from != end();
            ++from, i++)
      {
         if(i == index)
         {
            *from = s;
            return;
         }
      }
   }
}
//---------------------------------------------------------------------------
void TStrings :: Assign(TStrings &strings)
{
   Clear();
   QuoteChar = strings.QuoteChar;
   Delimiter = strings.Delimiter;
   CommentChar = strings.CommentChar;
   Add(strings);
}
//---------------------------------------------------------------------------
void TStrings :: Add(char **strings, int nstrings)
{
   for(register int i = 0; i < nstrings; i++)
      push_back(strings[i]);
}
* */
//---------------------------------------------------------------------------
/*
void TStrings :: Insert(int index, const TString &str)
{
   int i = 0;
   for(TStrings::iterator from = begin();
            from != end();
            ++from, i++)
   {
      if(i == index)
      {
         insert(from, str);
         return;
      }
   }
}
*/
//---------------------------------------------------------------------------
/*
int TStrings :: IndexOf(const TString &str)
{
   int i = 0;
   for(TStrings::iterator from = begin();
            from != end();
            ++from, ++i)
   {
      if(*from == str)
         return i;
   }

   return -1;
}
* */
//---------------------------------------------------------------------------
/*
void TStrings :: Exchange(int index1, int index2)
{
  TString item1, item2;

  if (index1 < 0 || index1 >= Count ||
      index2 < 0 || index2 >= Count)
    return;
  item1 = Strings[index1];
  item2 = Strings[index2];
  Strings[index1] = item2;
  Strings[index2] = item1;
}
*/
//---------------------------------------------------------------------------
/*
void TStrings :: Move(int newindex, int curindex)
{
  TString item;

  if (curindex != newindex)
  {
    if (newindex < 0 || newindex >= Count)
      return;
    item = Strings[curindex];
    Delete(curindex);
    Insert(newindex, "");
    Strings[newindex] = item;
  }
}
* */
//---------------------------------------------------------------------------
/*
void TStrings :: Delete(int index)
{
   int i = 0;
   for(TStrings::iterator from = begin();
            from != end();
            ++from, i++)
   {
      if(i == index)
      {
         erase(from);
         return;
      }
   }
}
* */
//---------------------------------------------------------------------------
/*
TString TStrings :: get_Text()
{
  TString result;

  for(TStrings::iterator from = begin();
      from != end();
      ++from)
  {
      result = result + *from + "\n";
  }
  return result.erase(result.length()-1);
}
//---------------------------------------------------------------------------
void TStrings :: set_Text(TString value)
{
   TString arg("\n\r");
   TString::size_type pos1, pos2;

   clear();
   pos1 = pos2 = 0;
   do{
      pos2 = value.find_first_of(arg, pos1);
      if(pos2 != TString::npos)
      {
         push_back(value.substr(pos1, pos2 - pos1));
         pos1 = pos2 + 1;
      }
      else
      {
         pos2 = value.size();
         push_back(value.substr(pos1, pos2 - pos1));
         break;
      }
   }while(1);
}
//---------------------------------------------------------------------------
TString TStrings :: get_DelimitedText()
{
   string lb, result, ld;
   lb += Delimiter;
   ld += QuoteChar;
   for(TStrings::iterator from = begin();
       from != end();
       ++from)
   {
      result = result + ld + *from + ld + lb;
   }
   return result.erase(result.length()-1);
}
//---------------------------------------------------------------------------
void TStrings :: set_DelimitedText(TString value)
{
   TString arg;
   TString::size_type pos1, pos2;

   clear();
   pos1 = pos2 = 0;
   do{
      pos2 = value.find_first_of(Delimiter, pos1);
      if(pos2 != TString::npos)
      {
         push_back(value.substr(pos1, pos2 - pos1));
         pos1 = pos2 + 1;
      }
      else
      {
         push_back(value.substr(pos1));
         break;
      }
   }while(1);
}
//---------------------------------------------------------------------------
int TStrings :: IndexOfName(const TString &name)
{
	TString tmp1, tmp2;
	int i = 0;
	bool b;

	for(TStrings :: iterator from = begin();
      	from != end();
      	++from, ++i)
  	{
    	b = ExtractNameValue(*from, tmp1, tmp2);
    	if(b)
    		continue;
    	if(tmp1 == name)
        	return i;
  	}
	return -1;
}
//---------------------------------------------------------------------------
TString TStrings :: get_Names(const int index)
{
	TString tmp1, tmp2;
	ExtractNameValue(get_Strings(index), tmp1, tmp2);
   	return tmp1;
}
//---------------------------------------------------------------------------
TString TStrings :: get_Values(const TString name)
{
	TString tmp1, tmp2;
	int i = 0;
	bool b;

	for(TStrings :: iterator from = begin();
      	from != end();
      	++from, ++i)
  	{
    	b = ExtractNameValue(*from, tmp1, tmp2);
    	if(b)
    		continue;
    	if(tmp1 == name)
        	return tmp2;
  	}
	return *end();
}
//---------------------------------------------------------------------------
void TStrings :: set_Values(const TString name, TString value)
{
	int i = IndexOfName(name);
  	if(!value.empty())
  	{
    	if(i < 0)
      		TCommonList<TString>::Add("");
      	TString s(name + "=" + value);
    	set_Strings(i, s);
  	}
  	else
  	{
    	if(i >= 0)
      		Delete(i);
  	}
}
//---------------------------------------------------------------------------
bool TStrings :: ExtractNameValue(const TString &str, TString &name, TString &value)
{
	TString tmp;
	string::size_type poscmnt, poseq;

    poseq = str.find('=');
    if(poseq == string::npos)
    		return true;
    poscmnt = str.find(CommentChar);
    if(poscmnt != string::npos && poscmnt < poseq)
    		return true;
    name = str.substr(0, poseq);
    name.Trim();
    poseq += 1;
    if(poscmnt == string::npos)
    	value = str.substr(poseq);
    else
    	value = str.substr(poseq, poscmnt - poseq);
    value.Trim();
    return false;
}*/
//---------------------------------------------------------------------------

/*
{ TStrings }

procedure TStrings.LoadFromFile(const FileName: string);
var
  Stream: TStream;
begin
  Stream := TFileStream.Create(FileName, fmOpenRead or fmShareDenyWrite);
  try
    LoadFromStream(Stream);
  finally
    Stream.Free;
  end;
end;

procedure TStrings.LoadFromStream(Stream: TStream);
var
  Size: Integer;
  S: string;
begin

  try
    Size := Stream.Size - Stream.Position;
    SetString(S, nil, Size);
    Stream.Read(Pointer(S)^, Size);
    SetTextStr(S);
  finally

  end;
end;

procedure TStrings.SaveToFile(const FileName: string);
var
  Stream: TStream;
begin
  Stream := TFileStream.Create(FileName, fmCreate);
  try
    SaveToStream(Stream);
  finally
    Stream.Free;
  end;
end;

procedure TStrings.SaveToStream(Stream: TStream);
var
  S: string;
begin
  S := GetTextStr;
  Stream.WriteBuffer(Pointer(S)^, Length(S));
end;

*/
//---------------------------------------------------------------------------
TString __fastcall ChangeFileExt(const TString &filename, const TString &ext)
{
    uint_t pos = filename.rfind('.');
    if( pos == TString::npos )
    {
        if(ext[0] == '.')
            return filename + ext;
        else
            return filename + '.' + ext;
    }
    if(ext[0] != '.')
        pos++;

    return filename.substr(0, pos) + ext;
}
//---------------------------------------------------------------------------
TString __fastcall ExtractFileExt(const TString &filename)
{
    return filename.substr(filename.rfind('.') + 1);
}
//---------------------------------------------------------------------------
TString __fastcall ExtractFileName(const TString &filename)
{
    return filename.substr(filename.rfind('/') + 1);
}
//---------------------------------------------------------------------------
TString __fastcall ExtractFilePath(const TString &filename)
{
    return filename.substr(0, filename.rfind('/') + 1);
}
//---------------------------------------------------------------------------
static void __sigchld_handler(int signum)
{
    pid_t pid;
    int stat;
    do
    {
        pid = waitpid(-1, &stat, WNOHANG);
        printf("process terminated %d\n", pid);
    }
    while(pid > 0);
    return;
}

void SetSigChildHandler()
{
    struct sigaction act;
    sigaction(SIGCHLD, NULL, &act);
    if(act.sa_handler != __sigchld_handler)
    {
        sigset_t mask;
        sigemptyset(&mask);
        act.sa_handler = __sigchld_handler;
        act.sa_mask = mask;
        act.sa_flags = SA_RESTART;
        sigaction(SIGCHLD, &act, NULL);
    }
}
//---------------------------------------------------------------------------
TString __fastcall TimeToString(time_t time)
{
    struct tm  *ts;
    char       buf[80];

    ts = localtime(&time);
    strftime(buf, sizeof(buf), "%c", ts);

    return TString(buf);
}
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
