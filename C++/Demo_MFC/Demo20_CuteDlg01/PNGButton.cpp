#include "StdAfx.h"
#include "PNGButton.h"
#include "Resource.h"

BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, Gdiplus::Image *&pImg)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nID), sTR);
	if (!hRsrc) return FALSE;
	// Load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc) return FALSE;
	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy_s(pmem, len, lpRsrc, len);
	GlobalUnlock(m_hMem);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	// Load from stream
	pImg = Gdiplus::Image::FromStream(pstm);
	// Free/Release stuff
	pstm->Release();
	FreeResource(lpRsrc);
	GlobalFree(m_hMem);
	return TRUE;
}

PNGButton::PNGButton(void) { 
	this->m_is_checked=false;
	DrawBorder=false;
	m_is_mouse_over	=	false; 
	m_is_tracked = false;
} 
PNGButton::~PNGButton(void) { }
IMPLEMENT_DYNCREATE(PNGButton, CWnd)  
BEGIN_MESSAGE_MAP(PNGButton, CWnd)  
	//{{AFX_MSG_MAP(MySelfWnd)  
//	ON_WM_LBUTTONDOWN()

	//}}AFX_MSG_MAP  
	ON_WM_PAINT()
	ON_WM_MOUSEHOVER()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEMOVE()  
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()  


//void PNGButton::OnLButtonDown(UINT nFlags, CPoint point)   
//{   
//	CWnd::OnLButtonDown(nFlags, point);  
//}  
BOOL PNGButton::Create( UINT x,UINT y, CWnd* pParentWnd, UINT nID, UINT PngID,Gdiplus::Image* hoverBg, CCreateContext* pContext)   
{  
	// TODO: Add your specialized code here and/or call the base class  
	// 重新注册窗口类,  
	LPCTSTR lpszClassName=AfxRegisterWndClass( CS_HREDRAW|CS_VREDRAW ,  AfxGetApp()->LoadStandardCursor(IDC_ARROW), 
		(HBRUSH)GetStockObject(TRANSPARENT), NULL) ;   
	ImageFromIDResource(PngID,L"PNG",this->bg);
	this->hoverBg=hoverBg;
	
	mWidth=bg->GetWidth();
	mHeight=bg->GetHeight();

	BOOL OK=CWnd::Create(NULL,NULL,WS_CHILDWINDOW|WS_VISIBLE, CRect(x,y,x+mWidth,mHeight+y),pParentWnd, nID, pContext);; 
	//|BS_OWNERDRAW
	ModifyStyleEx(0, WS_EX_TRANSPARENT);// WS_EX_LAYERED||WS_EX_TRANSPARENT  
	//SetLayeredWindowAttributes(TRANSPARENT, (byte)(255 * 1), LWA_COLORKEY); //LWA_ALPHA LWA_COLORKEY
	
	return OK;
}   
BOOL PNGButton::Create(UINT x,UINT y, CWnd* pParentWnd, UINT nID, UINT PngID,UINT PngID_hOver, CCreateContext* pContext){
	LPCTSTR lpszClassName=AfxRegisterWndClass( CS_HREDRAW|CS_VREDRAW ,  AfxGetApp()->LoadStandardCursor(IDC_ARROW), (HBRUSH)GetStockObject(TRANSPARENT), NULL) ;   
	ImageFromIDResource(PngID,L"PNG",this->bg);
	ImageFromIDResource(PngID_hOver,L"PNG",this->hoverBg); 
	mWidth=bg->GetWidth();
	mHeight=bg->GetHeight(); 
	BOOL OK=CWnd::Create(NULL,NULL,WS_CHILDWINDOW|WS_VISIBLE, CRect(x,y,x+mWidth,mHeight+y),pParentWnd, nID, pContext);; 
	//|BS_OWNERDRAW
	ModifyStyleEx(0, WS_EX_TRANSPARENT);// WS_EX_LAYERED||WS_EX_TRANSPARENT  
	//SetLayeredWindowAttributes(TRANSPARENT, (byte)(255 * 1), LWA_COLORKEY); //LWA_ALPHA LWA_COLORKEY
	
	return OK;
}


void PNGButton::OnPaint()
{  
	CPaintDC dc(this);    
	Gdiplus::Graphics g(dc.m_hDC);   
	
	Gdiplus::Bitmap bmp(this->mWidth,this->mHeight);
	Gdiplus::Graphics* gBuf= Gdiplus::Graphics::FromImage(&bmp);
	gBuf->DrawImage(this->bg,0,0); 
	//画鼠标指向时的亮色背景 或被选中时
	if(DrawBorder|m_is_checked){
		gBuf->DrawImage(hoverBg, 0, 0, 0, 0, mWidth, mHeight, Gdiplus::UnitPixel);
		//gBuf->DrawImage(this->hoverBg,0,0);
		TRACE(L"DrawBorder\r\n");   
	} 
	delete gBuf;
	g.DrawImage(&bmp,0,0);
	g.ReleaseHDC(dc.m_hDC);
	 
}
void PNGButton::PaintParent()
{  
	CRect   rect; 
	GetWindowRect(&rect); 
	GetParent()-> ScreenToClient(&rect); 
	GetParent()-> InvalidateRect(&rect);
}

void PNGButton::OnMouseHover(UINT nFlags, CPoint point)
{    
	DrawBorder=true;
	PaintParent(); 
	TRACE(L"OnMouseHover\r\n");
	//CWnd::OnMouseHover(nFlags, point);
}


void PNGButton::OnMouseLeave()
{    
	m_is_mouse_over	=   false; 
	m_is_tracked	=   false;  
	DrawBorder=false;
	PaintParent(); 
	//TRACE(L"OnMouseLeave\r\n");
	CWnd::OnMouseLeave();
}


void PNGButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_is_mouse_over   =   true;  
	//TRACE(L"OnMouseMove\r\n");
	if(!m_is_tracked) 
	{ 
		TRACKMOUSEEVENT   tme; 
		tme.cbSize		=   sizeof(TRACKMOUSEEVENT); 
		tme.dwFlags		=   TME_LEAVE|TME_HOVER; //
		tme.hwndTrack   =   GetSafeHwnd(); 
		tme.dwHoverTime	=   80; 
		_TrackMouseEvent(&tme);  
		m_is_tracked   =   true;    
		//OnMouseHover(nFlags,point);
	}   
	CWnd::OnMouseMove(nFlags, point);
}

void PNGButton::Check(bool check){
	if(this->m_is_checked!=check){
		this->m_is_checked=check;
		PaintParent();
		//TRACE(L"DrawSelected\r\n");
	} /*else{  TRACE(L"Check(%d,%d)\r\n",GetDlgCtrlID(),check); }*/
}

 



void PNGButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	TRACE(L"Click!\n"); 
	GetParent()->SendMessage(WM_COMMAND,GetDlgCtrlID()|0,(LONG)GetSafeHwnd());
	CWnd::OnLButtonUp(nFlags, point);
}
