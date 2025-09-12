// FRAME.CPP - Window handlers for SPIDER.
// T. N. Roberts, 08-Oct-92.
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlcrack.h>

#include "resource.h"

#include "crd.h"
#include "frame.h"
#include "spider.h"
#include "dialogs.h"

CAppModule _Module;


////////////////////////////////////////////////////////////
//
// CSpiderApp
//
////////////////////////////////////////////////////////////


int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT )
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    CMainWindow wndMain;

    if(wndMain.CreateEx() == NULL)
    {
        ATLTRACE(_T("Main window creation failed!\n"));
        return 0;
    }

    wndMain.ShowWindow(nCmdShow);

    int nRet = theLoop.Run();

    _Module.RemoveMessageLoop();
    return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{

    INITCOMMONCONTROLSEX iccx;
    iccx.dwSize = sizeof(iccx);
    iccx.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
    BOOL bRet = ::InitCommonControlsEx(&iccx);
    bRet;
    ATLASSERT(bRet);

    HRESULT hRes = _Module.Init(NULL, hInstance);
    hRes;
    ATLASSERT(SUCCEEDED(hRes));

    int nRet = Run(lpstrCmdLine, nCmdShow);

    _Module.Term();
    return nRet;
}





////////////////////////////////////////////////////////////
//
// CMainWindow implementation
//
////////////////////////////////////////////////////////////



#define YMAP(y)    (68 + (y) * bmSize.cy / 4)


CMainWindow::CMainWindow ()
{
    // Cards are 71 x 96.  Set a default gap of 5.

    CRect rectRequest (0, 0, (71 + 5) * 10 + 14, 700);

    srand ((WORD) (GetTickCount () & 0xffff));
    nRows = 5;
    nColumns = 10;
    bWinner = 0;

#if 0
    Create (NULL,
	"Spider",
	WS_OVERLAPPEDWINDOW,
	rectRequest,
	NULL,
	"MainMenu");
#endif
}


void
CMainWindow::ShutDown ()
{
    if (bFontAdded) {
        RemoveFontResource (FONTFILE ".fot");
	::PostMessage (HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
	bFontAdded = 0;
    }

    SetClassLong (m_hWnd, GCL_HBRBACKGROUND, oldBgr);

    if (hFeltGreen.m_hObject)
	hFeltGreen.DeleteObject ();
}


int	CALLBACK
CheckBigFont (
	const ENUMLOGFONT FAR * pelf,
	const TEXTMETRIC FAR * pntm,
	ULONG iFontType,
	LPARAM lContext
) {
	*((int *)lContext) = 0;
	return 0;
}				


int
CMainWindow::OnCreate (LPCREATESTRUCT cs)
{
    hFeltGreen.CreateSolidBrush (FELTGREEN);

    oldBgr = GetClassLong (m_hWnd, GCL_HBRBACKGROUND);
    SetClassLong (
        m_hWnd,
	GCL_HBRBACKGROUND,
	(int) hFeltGreen.GetSafeHandle ()
    );

	HDC hDC = ::GetDC (0);
	bFontAdded = 1;
	EnumFontFamilies (hDC, FONTFAMILY, (FONTENUMPROC) CheckBigFont, (LPARAM) &bFontAdded);
	::ReleaseDC (0, hDC);

    if (bFontAdded) {
        AddFontResource (FONTFILE ".ttf");
		::SendMessage (HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
    }

    fVAG.CreateFont (250, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		4 + DEFAULT_PITCH + FF_DECORATIVE,
		FONTNAME
    );

    bmSize = cards.BitmapSize ();

    OnSize (0, cs->cx, cs->cy);

// For now, lets set all our buttons as 20x60.

    int cBtnX = 67;
    int cBtnY = 22;

    CRect rect (5, 5, 5 + cBtnX, 5 + cBtnY);
    rect.bottom = 5 + cBtnY + cBtnY;
    bnDeal.Create (
        "&Deal",
	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	rect,
	this,
	BN_DEAL
    );

    rect.OffsetRect (cBtnX + 3, 0);
    rect.bottom = 5 + cBtnY;
    bnExit.Create (
        "E&xit",
	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	rect,
	this,
	BN_EXIT
    );

    rect.OffsetRect (0, cBtnY + 1);
    bnReset.Create (
        "Re&set",
	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	rect,
	this,
	BN_RESET
    );

    rect.OffsetRect (cBtnX + 3, - cBtnY - 1);
    bnRemove.Create (
        "&Remove",
	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	rect,
	this,
	BN_REMOVE
    );

    int cTextX = 180;
    int cTextY = 15;

    rect.OffsetRect (cBtnX + 5, 0);
    rect.right = rect.left + cTextX;

    tbDeals.Create (
        "Click Deal to start.",
	WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX,
        rect,
	this,
	TB_DEALS
    );

    rect.OffsetRect (cTextX + 5, 0);
    tbError.Create (
        NULL,			// no error (yet)
	WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX,
        rect,
	this,
	TB_ERROR
    );

    rect.OffsetRect (- cTextX - 5, cTextY + 5);
    tbSuits.Create (
        NULL,
	WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX,
        rect,
	this,
	TB_SUITS
    );

    int cSuitX = 70;

    rect.OffsetRect (2 * cTextX + 10, - cTextY - 5);
    rect.right = rect.left + cSuitX;
    tbSuit [3].Create (
        "Spades",
	WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX,
        rect,
	this,
	TB_SUIT+3
    );

    rect.OffsetRect (0, cTextY + 5);
    tbSuit [2].Create (
        "Hearts",
	WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX,
        rect,
	this,
	TB_SUIT+2
    );

    rect.OffsetRect (cSuitX + 5, - cTextY - 5);
    tbSuit [1].Create (
        "Diamonds",
	WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX,
        rect,
	this,
	TB_SUIT+1
    );

    rect.OffsetRect (0, cTextY + 5);
    tbSuit [0].Create (
        "Clubs",
	WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOPREFIX,
        rect,
	this,
	TB_SUIT+0
    );

    fSymbol.CreateFont (120, 0,
        0, 0, 0, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0,
		4 + DEFAULT_PITCH + FF_DONTCARE,
		"Symbol"
    );

    tbSuits.SetFont( &fSymbol, TRUE );
    return 1;
}


void
CMainWindow::OnLButtonDown (UINT nFlags, CPoint point)
{
// Determine WHERE they clicked.

    char column;

    nFlags;

    for (
	column = 0;
        column < 9 && (point.x > (iColLeft [column] + iColWidth [column]));
        column++
    )
    ;

// This translates into either a HIGHLIGHT or a MOVE, depending on whether
// highlight was set or not.

    CClientDC dc (this);
    ProcessCommand (dc, '0' + column);
    DoColumnHints();
}


void
CMainWindow::OnLButtonDblClk (UINT nFlags, CPoint point)
{
// Determine WHERE they clicked.

    char column;

    nFlags;

    for (
	column = 0;
        column < 9 && (point.x > (iColLeft [column] + iColWidth [column]));
        column++
    )
    ;

    CClientDC dc (this);
    ProcessCommand (dc, 'A');		// select automove
    if (!ProcessCommand (dc, '0' + column)) {
        bWinner = 1;
	SetError (" ! ! !   Y O U   W I N   ! ! !");
	SetDeals ("Click 'Deal' to play again.");
	InvalidateRect (NULL);
    }
}


void
CMainWindow::OnDeal ()
{
    CClientDC dc (this);
    static int first = 1;

    if (first) {
        OnReset ();
	first = 0;
    }
    else if (!ProcessCommand (dc, 'D')) {
        MessageBeep (0xffffffff);
    }
}


void
CMainWindow::OnReset ()
{
    bWinner = 0;
    SpiderInit ();
    InvalidateRect (NULL);
}


void
CMainWindow::OnRemove ()
{
    CClientDC dc (this);

    if (!ProcessCommand (dc, 'R')) {
        bWinner = 1;
	SetError (" ! ! !   Y O U   W I N   ! ! !");
	SetDeals ("Click 'Deal' to play again.");
	InvalidateRect (NULL);
    }
}


void
CMainWindow::OnSize (UINT nType, CSize size)
{
    int iColumnWidth;
    int iColumnLeft;

    nType;

    bmClient = size;

    bClipped = (size.cx < 10 * bmSize.cx);
    if (bClipped) {
        // Cards in columns 0 through 8 will be clipped; 9 is full width.

	iColumnLeft = 0;
        iColumnWidth = (size.cx - bmSize.cx) / 9;
    }
    else {
        // We have room for all 10 columns -- distribute gap evenly.

	iColumnLeft = (size.cx - 10 * bmSize.cx) / 11;
	iColumnWidth = bmSize.cx + iColumnLeft;
    }

    for (int i = 0; i < 10; i++) {
        iColLeft [i] = iColumnLeft;
	iColWidth [i] = (bClipped && i < 9) ? iColumnWidth : bmSize.cx;
	iColumnLeft += iColumnWidth;
    }
}


void
CMainWindow::OnPaint (CDCHandle dc)
{
    if (bWinner) {
	CFont * oldFont = dc.SelectObject (&fVAG);

	CRect rctOpaq;

	GetClientRect (&rctOpaq);
	dc.SetBkColor (FELTGREEN);
	dc.SetTextColor (WHITE);
	dc.ExtTextOut (10, YMAP(0), ETO_OPAQUE, rctOpaq, "YOU", 3, NULL);
	dc.TextOut (10, YMAP(0)+220, "WON!", 4);

	dc.SelectObject (oldFont);
    }
    else {
	for (int i = 0; i < 10; i++)
	    DisplayColumn (dc, i);
	UpdateDeals ();
    }
}



void
CMainWindow::OnExit ()
{
    ShutDown ();
    DestroyWindow ();
}


void
CMainWindow::OnClose ()
{
    ShutDown ();
    CFrameWnd::OnClose ();
}


void
CMainWindow::OnAbout ()
{
    CModalDialog about ("AboutBox", this);
    about.DoModal ();
}


HBRUSH
CMainWindow::OnCtlColor (CDCHandle pdc, CStatic wnd)
{
    dc.SetTextColor (0xffffff);
    dc.SetBkColor (FELTGREEN);
    return (HBRUSH) hFeltGreen.GetSafeHandle ();
}


void
CMainWindow::ClearColumn (CDCHandle dc, int col)
{
    CRect rctOpaq;

    GetClientRect (&rctOpaq);
    dc.SetBkColor (FELTGREEN);
    rctOpaq.left = iColLeft [col];
    rctOpaq.right = rctOpaq.left + iColWidth [col];
    rctOpaq.top = YMAP(0);
    dc.ExtTextOut (0, 0, ETO_OPAQUE, rctOpaq, NULL, 0, NULL);
}


void
CMainWindow::ClearColumnTail (CDCHandle dc, int col, int start)
{
    CRect rctOpaq;

    GetClientRect (&rctOpaq);
    dc.SetBkColor (FELTGREEN);
    rctOpaq.left = iColLeft [col];

    if (!bClipped || col == 9) {
	rctOpaq.right = rctOpaq.left + iColWidth [col];
	rctOpaq.top = YMAP(start);
	dc.ExtTextOut (0, 0, ETO_OPAQUE, rctOpaq, NULL, 0, NULL);
    }
    else {

	// Make sure we erase any card remnants which might have extended
	// into the next column below its bottom card.

	rctOpaq.right = rctOpaq.left + bmSize.cx;
	rctOpaq.top = YMAP(ColumnLength(col+1)-1) + bmSize.cy;

	if (rctOpaq.top > YMAP(start)) {
	    dc.ExtTextOut (0, 0, ETO_OPAQUE, rctOpaq, NULL, 0, NULL);

	    rctOpaq.bottom = rctOpaq.top;
	    rctOpaq.right = rctOpaq.left + iColWidth [col];
	}

	rctOpaq.top = YMAP(start);
	dc.ExtTextOut (0, 0, ETO_OPAQUE, rctOpaq, NULL, 0, NULL);
    }
}


void
CMainWindow::ColumnNumber (CDCHandle dc, int col, BYTE intense)
{
    char txt [2];

    txt [0] = '0'+ (char) col;
    txt [1] = 0;

    dc.SetBkColor (FELTGREEN);
    dc.SetTextColor (intense ? RED : WHITE);
    dc.TextOut (iColLeft[col] + iColWidth [col] / 2, YMAP(0) - 16, txt, 1);
}


void
CMainWindow::PlaceBitmap (CDCHandle dc, int suit, int rank, int x, int y)
{
    CDC dcMem;

    int w = iColWidth [x];
    int col = x;

    x = iColLeft [x];
    y = YMAP(y);

    if (
        bClipped &&
	col < 9 &&
	ColumnLength (col+1) > 0) {

// We should be clipping away that part of the screen covered by the next
// column to the right.

#if 0
char s[80];
wsprintf (
    s,
    "Clipping for column %d: %d,%d to %d,%d, length is %d.",
    col,
    iColLeft [col+1], 0,
    iColLeft [col+1] + iColWidth [col+1], YMAP (ColumnLength (col+1) - 1) + bmSize.cy,
    ColumnLength (col+1)
);
MessageBox (s);
#endif

	dc.ExcludeClipRect (
	    iColLeft [col+1],
	    0,
	    iColLeft [col+1] + iColWidth [col+1],
	    YMAP (ColumnLength (col+1) - 1) + bmSize.cy
	);
    }

	dc.SetBkColor (WHITE);
	dc.SetTextColor (BLACK);

    if (suit == 4)
		cards.Draw (dc, x, y, IDO, DECKO, FELTGREEN);
    else
		cards.Draw (dc, x, y, Cd(rank,suit), FACEUP, FELTGREEN);

    if (bClipped)
		dc.SelectClipRgn (NULL);
}


void
CMainWindow::OnPreferences ()
{
    CPreferences box (this);

    box.nRows = nRows;
    box.nColumns = nColumns;

    box.DoModal ();

    nRows = box.nRows;
    nColumns = box.nColumns;
}


int
random (int n)
{
    return int ((DWORD (rand ()) * DWORD (n)) >> 15);
}

#if 0


// CPreferences -- the preferences dialog box.


BEGIN_MESSAGE_MAP (CPreferences, CDialog)
    ON_BN_CLICKED (BN_DEFAULTS, OnSetDefaults)
    ON_WM_HSCROLL ()
END_MESSAGE_MAP ()


BOOL
CPreferences::OnInitDialog ()
{
    char s [80];

    CDialog::OnInitDialog ();

    hsRows ().SetScrollRange (0, 9, FALSE);
    hsRows ().SetScrollPos (nRows, TRUE);
    wsprintf (s, "%d", nRows);
    ebRows ().SetWindowText (s);

    hsColumns ().SetScrollRange (5, 20, FALSE);
    hsColumns ().SetScrollPos (nColumns, TRUE);
    wsprintf (s, "%d", nColumns);
    ebColumns ().SetWindowText (s);

    return TRUE;
}


void
CPreferences::OnOK ()
{
    nRows = hsRows ().GetScrollPos ();
    nColumns = hsColumns ().GetScrollPos ();
    EndDialog (IDOK);
}


void
CPreferences::OnSetDefaults ()
{
    nRows = 5;
    nColumns = 10;

    hsRows ().SetScrollPos (nRows, TRUE);
    hsColumns ().SetScrollPos (nColumns, TRUE);
}


void
CPreferences::OnHScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CDialog::OnHScroll (nSBCode, nPos, pScrollBar);

    char s [80];

    wsprintf (s, "%d", pScrollBar->GetScrollPos ());
    ((CEdit *) GetDlgItem (pScrollBar->GetDlgCtrlID() + 1))->SetWindowText (s);
}

#endif
