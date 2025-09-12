
// FRAME.H - Header for spider window handlers.

#include "crd.h"
#include "dialogs.h"

const int kBlackCard = 0;
const int kRedCard = 1;
const int kFaceCard = 2;

const int nDeals = 5;


// CSpiderApp - The application.


#if 0

// CPreferences - the preferences dialog box.

class CPreferences : public CModalDialog
{
protected:
    CScrollBar& hsRows () {
        return *(CScrollBar*) GetDlgItem (SB_ROWS);
    }

    CScrollBar& hsColumns () {
        return *(CScrollBar*) GetDlgItem (SB_COLUMNS);
    }

    CEdit& ebRows () {
        return *(CEdit*) GetDlgItem (EB_ROWS);
    }

    CEdit& ebColumns () {
        return *(CEdit*) GetDlgItem (EB_COLUMNS);
    }

public:
    // Variables used to communicate between us and the caller.
    int nRows;
    int nColumns;

    CPreferences (CWnd* pParent) : CModalDialog ("Preferences", pParent)
    { }

    // Message handlers.

    afx_msg BOOL OnInitDialog ();
    afx_msg void OnHScroll (UINT, UINT, CScrollBar *);

    afx_msg void OnOK ();
    afx_msg void OnSetDefaults ();
    DECLARE_MESSAGE_MAP ();
};
#endif


// CMainWindow - our main window.

class CMainWindow 
    : public CFrameWindowImpl<CMainWindow>
    , public CUpdateUI<CMainWindow>
//    , public CMessageFilter
//    , public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

    CButton bnDeal;
    CButton bnReset;
    CButton bnExit;
    CButton bnRemove;
    CStatic tbDeals;
    CStatic tbSuits;
    CStatic tbError;
    CStatic tbSuit [4];

    CSize bmSize;
    CSize bmClient;

    CCards cards;

    int iColLeft [10];
    int iColWidth [10];

    unsigned int oldBgr;
    CBrush hFeltGreen;

    int nRows;
    int nColumns;

    CFont fVAG;
    CFont fSymbol;
    BOOL bFontAdded;	// true if we need to remove the VAG Rounded font
    BOOL bWinner;

public:
    BOOL bClipped;

public:
    CMainWindow ();

    ~CMainWindow () {
    }

    void SetDeals (char * s) {
        tbDeals.SetWindowText (s);
    };

    void SetSuits (char * s) {
        tbSuits.SetWindowText (s);
    };

    void SetHint (char * s) {
//	CFont* pf = tbSuits.GetFont();
	tbSuits.SetWindowText( s );
//	tbSuits.SetFont( &fSymbol, TRUE );
	tbSuits.Invalidate();
	tbSuits.UpdateWindow();
//	tbSuits.SetFont( pf );
    }

    void SetError (char * s) {
        tbError.SetWindowText (s);
    };

    void PlaceBitmap (CDC &, int, int, int, int);
    void ShutDown (void);

    void ClearColumn (CDC &, int);
    void ClearColumnTail (CDC &, int, int);
    void ColumnNumber (CDC & dc, int col, BYTE intense);

    void ShowSuit (int c) {
        tbSuit [c].EnableWindow (TRUE);
    }

    void HideSuit (int c) {
        tbSuit [c].EnableWindow (FALSE);
    }

    BEGIN_MSG_MAP_EX(CMainWindow)

        MSG_WM_CREATE(OnCreate)
        MSG_WM_CTLCOLORSTATIC(OnCtlColor)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_SIZE(OnSize)

        COMMAND_HANDLER_EX(BN_RESET, BN_CLICKED, OnReset)
        COMMAND_HANDLER_EX(BN_DEAL, BN_CLICKED, OnDeal)
        COMMAND_HANDLER_EX(BN_EXIT, BN_CLICKED, OnExit)
        COMMAND_HANDLER_EX(BN_REMOVE, BN_CLICKED, OnRemove)

        COMMAND_ID_HANDLER_EX(IDM_EXIT, OnExit)
        COMMAND_ID_HANDLER_EX(IDM_ABOUT, OnAbout)
        COMMAND_ID_HANDLER_EX(IDM_PREFERENCES, OnPreferences)

        CHAIN_MSG_MAP(CUpdateUI<CMainWindow>)
        CHAIN_MSG_MAP(CFrameWindowImpl<CMainWindow>)
    END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    int OnCreate(LPCREATESTRUCT);
    void OnPaint(CDCHandle);
    void OnLButtonDown(UINT, CPoint);
    void OnLButtonDblClk(UINT, CPoint);
    HBRUSH OnCtlColor(CDCHandle, CStatic);

    void OnAbout(UINT, int, CWindow);
    void OnExit(UINT, int, CWindow);
    void OnClose(UINT, WPARAM, LPARAM, BOOL&);
    void OnSize(UINT, CSize);

    void OnDeal(UINT, int, CWindow);
    void OnRemove(UINT, int, CWindow);
    void OnReset(UINT, int, CWindow);
    void OnPreferences(UINT, int, CWindow);
};

const DWORD FELTGREEN = 0x008000;
const DWORD WHITE = 0xffffff;
const DWORD RED = 0x0000ff;
const DWORD BLACK = 0x000000;

#define	FONTNAME	"VAG Rounded BT"
#define FONTFAMILY	"VAGRounded BT"
#define FONTFILE	"tt0756m_"
