#include "cdt.h"

#ifndef	_CRD_H
#define	_CRD_H

typedef int CD;


/* WARNING: Order of su's is assumed */
#define suClub 0
#define suDiamond 1
#define suHeart 2
#define suSpade 3
#define suMax 4
#define suFirst suClub

#define raAce 0
#define raDeuce 1
#define raTres 2
#define raFour 3
#define raFive 4
#define raSix 5
#define raSeven 6
#define raEight 7
#define raNine 8
#define raTen 9
#define raJack 10
#define raQueen 11
#define raKing 12
#define raMax 13
#define raNil 15
#define raFirst raAce

typedef int RA;
typedef int SU;

#define cdNil 0x3c


#define cIDFACEDOWN (IDFACEDOWNLAST-IDFACEDOWNFIRST+1)

#define SuFromCd(cd) ((cd)&0x03)
#define RaFromCd(cd) ((cd)>>2)
#define Cd(ra, su) (((ra)<<2)|(su))


// Entry points in CARDS.DLL:

typedef BOOL (WINAPI * LPCDTINIT)(int FAR *pdxCard, int FAR *pdyCard);
/*
 * Parameters:
 *	pdxCard, pdyCard
 *			Far pointers to ints where card size will be placed
 *
 * Returns:
 *	True when successful, False when it can't find one of the standard
 *	bitmaps.
 */

typedef BOOL (WINAPI * LPCDTDRAWEXT)(HDC hdc, int x, int y, int dx, int dy, 
				int cd, int mode, DWORD rgbBgnd);
/*
 * Parameters:
 *	hdc  	HDC to window to draw cards on
 *	x, y	Where you'd like them
 *	dx,dy card extents
 *	cd		Card to be drawn
 *	mode	Way you want it drawn
 *
 * Returns:
 *	True if card successfully drawn, False otherwise
 */

typedef BOOL (WINAPI * LPCDTDRAW)(HDC hdc, int x, int y, int cd, int mode, DWORD rgbBgnd);
/*
 * Parameters:
 *	hdc		HDC to window to draw cards on
 *	x, y	Where you'd like them
 *	cd		Card to be drawn
 *	mode	Way you want it drawn
 *
 * Returns:
 *	True if card successfully drawn, False otherwise
 */

typedef void (WINAPI * LPCDTTERM)();
/*
 * Free up space if it's time to do so.
 *
 * Parameters:
 *	none
 *
 * Returns:
 *	nothing
 */


class CCards {

private:
    HMODULE		hDLL;
    LPCDTINIT		cdtInit;
    LPCDTDRAWEXT	cdtDrawExt;
    LPCDTDRAW		cdtDraw;
    LPCDTTERM		cdtTerm;
    CSize		sizBitmap;

public:
    CCards () {
	hDLL = LoadLibrary ("CARDS.DLL");

	if (hDLL < (HMODULE)HINSTANCE_ERROR) {
	    MessageBox (0, "Could not find CARDS.DLL.", "Error", MB_OK);
	    sizBitmap = CSize(0,0);
	    return;
	}

	cdtInit		= (LPCDTINIT) GetProcAddress (hDLL, "cdtInit");
	cdtDrawExt	= (LPCDTDRAWEXT) GetProcAddress (hDLL, "cdtDrawExt");
	cdtDraw		= (LPCDTDRAW) GetProcAddress (hDLL, "cdtDraw");
	cdtTerm		= (LPCDTTERM) GetProcAddress (hDLL, "cdtTerm");

	cdtInit ((PINT) &sizBitmap.cx, (PINT) &sizBitmap.cy);
    }

    ~CCards () {
        cdtTerm ();
	FreeLibrary (hDLL);
    }

    inline BOOL Draw (
	HDC hdc, 
	int x, int y, 
	int cd, int mode, DWORD rgbBgnd
    ) {
	return cdtDraw (hdc, x, y, cd, mode, rgbBgnd);
    }

    inline BOOL DrawExt (
	HDC hdc, 
	int x, int y, 
	int dx, int dy, 
	int cd, int mode, DWORD rgbBgnd
    ) {
	return cdtDrawExt (hdc, x, y, dx, dy, cd, mode, rgbBgnd);
    }

    CSize& BitmapSize () { return sizBitmap; }
};

#endif
