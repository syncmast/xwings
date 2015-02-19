#ifndef XSTREAM_H
#define XSTREAM_H

#include <fstream>
#include <sstream>
#include <types.h>
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
enum TOpenMode
{
    omAppend = ios :: app,
    omAtEnd  = ios :: ate,
    omBinary = ios :: binary,
    omRead   = ios :: in,
    omWrite  = ios :: out,
    omTrunc  = ios :: trunc
};

enum TSeekOrigin
{
    soBegin   = ios :: beg,
    soCurrent = ios :: cur,
    soEnd     = ios :: end
};
//---------------------------------------------------------------------------------
//  TStreamBuf
//---------------------------------------------------------------------------------
class TStreamBuf : public streambuf
{
    DECLARE_PROPERTY( TStreamBuf );

private:

protected:

public:

};
//---------------------------------------------------------------------------------
//  TStream
//---------------------------------------------------------------------------------
/* typedef iostream TCommonStream;

class TStream : public TCommonStream
{
    DECLARE_PROPERTY( TStream );

private:
    TStreamBuf m_streambuf;
    inline long  get_RPos() { return tellg(); };
    inline void  set_RPos( long value ) { seekg( value ); };
    inline long  get_WPos() { return tellp(); };
    inline void  set_WPos( long value ) { seekp( value ); };

protected:

public:
    TStream() : iostream( &m_streambuf )
    {
    }
    inline long Read( void *buffer, long count )
    {
        read( (char *)buffer, count );
        return gcount();
    }
    inline void SeekR( long offset, TSeekOrigin dir) { seekg( (streamoff)offset, (ios::seekdir)dir ); };
    inline void SeekW( long offset, TSeekOrigin dir) { seekp( (streamoff)offset, (ios::seekdir)dir ); };
    inline void Write( const void *buffer, long count ) { write( (const char *)buffer, count ); };

    property ( rw, long, RPos );
    property ( rw, long, WPos );
};
*/
class TStream
{
    DECLARE_PROPERTY( TStream );

private:
    iostream *m_stream;
    inline long get_RPos()
    {
        return m_stream->tellg();
    };
    inline void set_RPos( long value )
    {
        m_stream->seekg( value );
    };
    inline long get_WPos()
    {
        return m_stream->tellp();
    };
    inline void set_WPos( long value )
    {
        m_stream->seekp( value );
    };
    inline long get_Size()
    {
        long size, pos = m_stream->tellg();
        m_stream->seekg(0, ios::end);
        size = m_stream->tellg();
        m_stream->seekg(pos, ios::beg);
        return size;
    }

protected:

public:
    TStream( iostream *stream ) : m_stream( stream )
    {
    }
    inline long Read( void *buffer, long count )
    {
        m_stream->read( (char *)buffer, count );
        return m_stream->gcount();
    }
    inline void SeekR( long offset, TSeekOrigin dir)
    {
        m_stream->seekg( (streamoff)offset, (ios::seekdir)dir );
    };
    inline void SeekW( long offset, TSeekOrigin dir)
    {
        m_stream->seekp( (streamoff)offset, (ios::seekdir)dir );
    };
    inline void Write( const void *buffer, long count )
    {
        m_stream->write( (const char *)buffer, count );
    };

    property ( r,  long, Size );
    property ( rw, long, RPos );
    property ( rw, long, WPos );
};

//---------------------------------------------------------------------------------
//  TFileStream
//---------------------------------------------------------------------------------
class TFileStream : public fstream, virtual public TStream
{
private:

protected:

public:
    TFileStream();
    TFileStream( const char * filename, int mode = omRead | omWrite );
};
//---------------------------------------------------------------------------------
//  TStringStream
//---------------------------------------------------------------------------------
class TStringStream : public stringstream, virtual public TStream
{
    DECLARE_PROPERTY( TStringStream );

private:
    inline TString get_DataString()
    {
        return str();
    };

protected:

public:
    TStringStream( int mode = omRead | omWrite );
    TStringStream( const TString &str, int mode = omRead | omWrite );

    inline void Clear()
    {
        str().erase();
    };
    TString ReadString( long count = 0, char delim = '\0' );
    inline void WriteString( const TString &s )
    {
        *this << s.c_str();
    };

    property ( r , TString, DataString );
};
//---------------------------------------------------------------------------------
//  TMemoryStream
//---------------------------------------------------------------------------------
class TMemoryStream : public stringstream, virtual public TStream
{
    DECLARE_PROPERTY( TMemoryStream );

private:
    inline void *get_Memory()
    {
        return (void *)str().c_str();
    };

protected:

public:
    TMemoryStream( int mode = omRead | omWrite );
    TMemoryStream( const void *mem, int len, int mode = omRead | omWrite );

    inline void Clear()
    {
        str().erase();
    };

    property ( r , void *, Memory );
};
/*
{ TCustomMemoryStream abstract class }

  TCustomMemoryStream = class(TStream)

    procedure SaveToStream(Stream: TStream);
    procedure SaveToFile(const FileName: string);
  end;

{ TMemoryStream }

  TMemoryStream = class(TCustomMemoryStream)
  public
    procedure LoadFromStream(Stream: TStream);
    procedure LoadFromFile(const FileName: string);

  end;
*/
//---------------------------------------------------------------------------------
/*
class TStreamBuf : public streambuf
{
    DECLARE_PROPERTY( TStreamBuf );

private:

protected:

public:


  protected:
    static const int bufferSize = 10;   // Размер буфера данных
    char buffer[bufferSize];            // Буфер данных

  public:
    // Конструктор
    // - Инициализация буфера данных
    // - на один символ меньше, чтобы при накоплении bufferSize символов
    //   вызывалась функция overflow()
    TStreamBuf()
    {
        setp (buffer, buffer+(bufferSize-1));
    }
    // Деструктор
    //  - Очистка буфера данных
    virtual ~TStreamBuf()
    {
        sync();
    }

  protected:

  virtual streambuf * setbuf ( char *s, streamsize n );

    // Вывод символов, хранящихся в буфере
    int flushBuffer () {
        int num = pptr()-pbase();
        if (write (1, buffer, num) != num) {
            return EOF;
        }
        pbump (-num);    // Соответствующий перевод указателя вывода
        return num;
    }

    // Буфер полон
    //  - Вывести c и все предшествующие символы
    virtual int_type overflow (int_type c) {
        if (c != EOF) {
            // Вставка символа в буфер
            *pptr() = c;
            pbump(1);
        }
        // Очистка буфера
        if (flushBuffer() == EOF) {
            // ERROR
            return EOF;
        }
        return c;
    }
    // Запись нескольких символов
    virtual
    std::streamsize xsputn (const char* s,
                            std::streamsize num) {
        return write(fd,s,num);
    }
    // Синхронизация данных с файлом/приемником
    //  - вывод данных из буфера
    virtual int sync () {
        if (flushBuffer() == EOF) {
            // ОШИБКА
            return -1;
        }
        return 0;
    }
};
    Конструктор инициализирует буфер вывода функцией setp():

  setp (buffer, buffer+(bufferSize-1);
    Буфер вывода настроен так, что функция overflow() вызывается, когда еще остается место для одного символа.
    Если функция overflow() вызывается с аргументом, отличным от EOF, то соответствующий символ может быть помещен в текущую позицию записи,
    поскольку указатель на нее не выходит за пределы конечного указателя. После того как аргумент overflow() будет помещен в позицию записи, буфер можно очистить.

    Именно эта задача решается функцией flushBuffer(). Функция записывает символы в стандартный канал вывода (дескриптор 1) при помощи функции write().
    Функция pbump() потокового буфера возвращает позицию записи к началу буфера.

    Функция overflow() вставляет в буфер символ, ставший причиной вызова overflow(), если он отличен от EOF.
    Затем функция pbump() смещает текущую позицию записи, чтобы она отражала новый конец блока буферизованных символов.
    Тем самым позиция записи временно смещается за конечную позицию (epptr()).

    Класс также содержит виртуальную функцию sync(), предназначенную для синхронизации текущего состояния потокового буфера с непосредственным носителем данных.
    Обычно синхронизация ограничивается простой очисткой буфера. Для небуферизованных версий переопределять эту функцию не обязательно,
    поскольку нет самого буфера, который можно было бы очистить.

    Виртуальный деструктор обеспечивает вывод данных, остающихся в буфере при его уничтожении.

    Эти функции переопределяются для большинства потоковых буферов. Если внешнее представление имеет особую структуру,
    возможно, придется переопределить дополнительные функции, например функции seekoff() и seekpos(), для управления позицией записи.
*/
//---------------------------------------------------------------------------------
}  // namespace
//---------------------------------------------------------------------------------
#endif // XSTREAM_H
