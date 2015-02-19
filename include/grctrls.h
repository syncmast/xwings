//---------------------------------------------------------------------------
#ifndef grctrlsH
#define grctrlsH

#include "sysobj.h"
#include "xgdi.h"
//---------------------------------------------------------------------------
namespace xwings
{
//---------------------------------------------------------------------------
//  TPaintBox
//---------------------------------------------------------------------------
class TPaintBox : public TControl
{
    DECLARE_PROPERTY(TPaintBox);

private:
    void Init();
    void __fastcall Assign(const TPaintBox &obj);

protected:

public:
    TPaintBox();
    __fastcall TPaintBox(const TPaintBox &a);
    virtual ~TPaintBox();
    __fastcall const TPaintBox & operator=(const TPaintBox &a);

};
//---------------------------------------------------------------------------
//  TImage
//---------------------------------------------------------------------------
class TImage : public TControl
{
    DECLARE_PROPERTY(TImage);

private:
    void Init();
    void __fastcall Assign(const TImage &obj);
    TGraphic *m_Picture;
    bool m_Stretch;
    bool m_Center;
    bool FDrawing;
    bool m_Proportional;
    void PictureChanged(TObject *Sender);
    void set_Center(bool value);
    void set_Stretch(bool value);
    void set_Transparent(bool value);
    void set_Proportional(bool value);
    void set_Picture(TGraphic *value);

protected:
    TRect DestRect();
    virtual void Paint();
    virtual void Progress(TObject *Sender, TProgressStage Stage, byte_t PercentDone, bool RedrawNow,
                          const TRect &R, const TString &Msg);
public:
    TImage();
    __fastcall TImage(const TImage &a);
    virtual ~TImage();
    __fastcall const TImage & operator=(const TImage &a);

    bool IncrementalDisplay;

    TProgressEvent OnProgress;

    property (m, bool, Center);
    property (m, TGraphic *, Picture);
    property (m, bool, Proportional);
    property (m, bool, Stretch);
};
//---------------------------------------------------------------------------
//  TAnimate
//---------------------------------------------------------------------------
class TAnimate : public TImage
{
    DECLARE_PROPERTY(TAnimate);

private:
    TTimer m_timer;
    void Init();
    void __fastcall Assign(const TAnimate &obj);
    short get_FrameCount();
    void set_StartFrame(short value);
    void set_StopFrame(short value);
    void set_Repeats(short value);
    short m_CurrentFrame;
    short m_StopCount;
    short m_StartFrame;
    short m_StopFrame;
    short m_FrameCount;
    short m_Repeats;
    void DoChange(TObject *Sender);

protected:
    virtual void set_Active(bool value);

public:
    short Sequence;				// Кадры в сек

    TAnimate();
    __fastcall TAnimate(const TAnimate &a);
    virtual ~TAnimate();
    __fastcall const TAnimate & operator=(const TAnimate &a);
    void __fastcall Play(int fromframe, int toframe, int repeats);
    void Stop();

    TNotifyEvent OnStart;
    TNotifyEvent OnStop;

    property (r, short, FrameCount);
    property (m, short, StartFrame);
    property (m, short, StopFrame);
    property (m, short, Repeats);
    /*
       	property StartFrame: Integer default 0;

        property Playing: Boolean;
        property Frame: Integer default 0;
        property EndFrame: Integer;

        property AutoPlay: Boolean default False;
        property Animation: string;

    property Active: Boolean;
    property AutoSize: Boolean;
    property Center: Boolean;

    property FileName: TFileName;	Связывает компонент с AVI-файлом
    property FrameHeight: Integers;	Высота в пикселях одного кадра клипа
    property FrameWidth: Integers;	Ширина кадра
    property Open: Boolean;	Содержит True, если компонент связан с AVI-клипом и готов к работе

    property Timers: Boolean;

    procedure Reset;
    procedure Seek(Frame: Smallint);
    Восстанавливает исходное состояние компонента.
    Свойство Open вновь становится True, но свойство Active = False
    Пропускает и не показывает кадр с номером Frame (нумерация кадров начинается с 1)


    property OnClose: TNotifyEvent;	Возникает при установке значения False в свойство Open. Например, когда компонент демонстрирует несколько видеоклипов подряд
    property OnOpen: TNotifyEvent;	Возникает при установке значения True в свойство Open


    */
    /*
    typedef TCommonList<HCURSOR> THCursorList;

    class TCustomCursor
    {
    private:
    	THCursorList m_List;
    	TWinControl *m_Control;
    	short m_NextFrame;
    	void DoChange(TObject *Sender);

    public:
    	short Sequence;				// Кадры в сек
    };

    { TAnimate }

      TCommonAVI = (aviNone, aviFindFolder, aviFindFile, aviFindComputer, aviCopyFiles,
        aviCopyFile, aviRecycleFile, aviEmptyRecycle, aviDeleteFile);

      TAnimate = class(TWinControl)
      private
        FActive: Boolean;
        FFileName: string;
        FCenter: Boolean;
        FCommonAVI: TCommonAVI;
        FFrameCount: Integer;
        FFrameHeight: Integer;
        FFrameWidth: Integer;
        FOpen: Boolean;
        FRepetitions: Integer;
        FTimers: Boolean;
        FStartFrame: Smallint;
        FStopFrame: Smallint;
        FStopCount: Integer;
        FOnOpen: TNotifyEvent;
        FOnClose: TNotifyEvent;
        FOnStart: TNotifyEvent;
        procedure CheckOpen;
        function InternalClose: Boolean;
        function InternalOpen: Boolean;
        function GetActualResHandle: THandle;
        procedure GetFrameInfo;
        procedure SetActive(Value: Boolean);
        procedure SetFileName(Value: string);
        procedure SetCenter(Value: Boolean);
        procedure SetCommonAVI(Value: TCommonAVI);
        procedure SetOpen(Value: Boolean);
        procedure SetRepetitions(Value: Integer);
        procedure SetTimers(Value: Boolean);
        procedure SetStartFrame(Value: Smallint);
        procedure SetStopFrame(Value: Smallint);
        procedure WMNCCalcSize(var Message: TWMNCCalcSize); message WM_NCCALCSIZE;
        procedure WMNCHitTest(var Message: TWMNCHitTest); message WM_NCHITTEST;
        procedure WMWindowPosChanged(var Message: TWMWindowPosChanged); message WM_WINDOWPOSCHANGED;
      protected
        procedure CreateWnd; override;
        procedure DestroyWnd; override;
        procedure DoOpen; virtual;
        procedure DoClose; virtual;
        procedure DoStart; virtual;
      public
        constructor Create(AOwner: TComponent); override;
        property FrameCount: Integer read FFrameCount;
        property FrameHeight: Integer read FFrameHeight;
        property FrameWidth: Integer read FFrameWidth;
        property Open: Boolean read FOpen write SetOpen;
        procedure Play(FromFrame, ToFrame: Word; Count: Integer);
        procedure Reset;
        procedure Seek(Frame: Smallint);
        procedure Stop;
      published
        property Active: Boolean read FActive write SetActive;
        property BorderWidth;
        property CommonAVI: TCommonAVI read FCommonAVI write SetCommonAVI default aviNone;
        property FileName: string read FFileName write SetFileName;
        property Repetitions: Integer read FRepetitions write SetRepetitions default 0;
        property ShowHint;
        property StartFrame: Smallint read FStartFrame write SetStartFrame default 1;
        property StopFrame: Smallint read FStopFrame write SetStopFrame default 0;
        property Timers: Boolean read FTimers write SetTimers default False;
        property OnOpen: TNotifyEvent read FOnOpen write FOnOpen;
        property OnClose: TNotifyEvent read FOnClose write FOnClose;
        property OnStart: TNotifyEvent read FOnStart write FOnStart;
      end;
    */

};
//---------------------------------------------------------------------------
} //namespace
//---------------------------------------------------------------------------
#endif
