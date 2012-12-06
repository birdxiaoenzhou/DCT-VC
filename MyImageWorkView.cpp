// MyImageWorkView.cpp : implementation of the CMyImageWorkView class
// Download by http://www.codefans.net

#include "stdafx.h"
#include "MyImageWork.h"

#include "MyImageWorkDoc.h"
#include "MyImageWorkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyImageWorkView

IMPLEMENT_DYNCREATE(CMyImageWorkView, CView)

BEGIN_MESSAGE_MAP(CMyImageWorkView, CView)
	//{{AFX_MSG_MAP(CMyImageWorkView)
	ON_COMMAND(ID_NORMAL, OnNormal)
	ON_COMMAND(ID_STRETCH, OnStretch)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_EMBED, OnEmbed)
	ON_COMMAND(ID_PICK, OnPick)
	ON_UPDATE_COMMAND_UI(ID_PICK, OnUpdatePick)
	ON_UPDATE_COMMAND_UI(ID_EMBED, OnUpdateEmbed)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyImageWorkView construction/destruction

CMyImageWorkView::CMyImageWorkView()
{
	// TODO: add construction code here
	ImageStyle = 0;
    have_open_a_file = false;

	embed = false;
	pick = false;
	show_contrast = false;
}

CMyImageWorkView::~CMyImageWorkView()
{
}

BOOL CMyImageWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	WNDCLASSEX wndcls;
	wndcls.cbSize=sizeof(WNDCLASSEX);
	HINSTANCE hinst=AfxGetInstanceHandle();
	if(CView::PreCreateWindow(cs)&&cs.lpszClass!=NULL)
	{
		HBRUSH hbkbrush=CreateSolidBrush(RGB(0,0,0));
		if(!GetClassInfoEx(hinst,cs.lpszClass,&wndcls))
			return FALSE;
		UnregisterClass(cs.lpszClass,hinst);
		wndcls.hbrBackground=hbkbrush;
		RegisterClassEx(&wndcls);  
		return TRUE;
	}
	else
		return FALSE;


	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyImageWorkView drawing

void CMyImageWorkView::OnDraw(CDC* pDC)
{
	CMyImageWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    
	RECT Rect;
	GetClientRect( &Rect );

	if (!show_contrast)
		m_Dib.Draw( pDC, 0, 0, Rect.right, Rect.bottom, ImageStyle );
	else
	{
		m_Dib.DrawContrast(pDC, Rect.right, Rect.bottom);
		pDC->SetBkColor((0,0,0));
		pDC->SetTextColor((255,255,255));
		pDC->TextOut(0, Rect.bottom-30, "��ͼΪԭʼͼ����ͼΪǶ����������Ϣ��ͼ");
	}

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMyImageWorkView printing

BOOL CMyImageWorkView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyImageWorkView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyImageWorkView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyImageWorkView diagnostics

#ifdef _DEBUG
void CMyImageWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CMyImageWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyImageWorkDoc* CMyImageWorkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyImageWorkDoc)));
	return (CMyImageWorkDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyImageWorkView message handlers

void CMyImageWorkView::OnNormal() 
{
	// TODO: Add your command handler code here
	ImageStyle = 0;

	CMenu *pMnu = AfxGetMainWnd()->GetMenu( );
	pMnu->CheckMenuItem(ID_STRETCH,MF_UNCHECKED);
	pMnu->CheckMenuItem(ID_NORMAL,MF_CHECKED);
	
	InvalidateRect( NULL, TRUE );
	UpdateWindow();
}

void CMyImageWorkView::OnStretch() 
{
	// TODO: Add your command handler code here
	ImageStyle = 1;
	
	CMenu *pMnu = AfxGetMainWnd()->GetMenu( );
	pMnu->CheckMenuItem(ID_STRETCH,MF_CHECKED);
	pMnu->CheckMenuItem(ID_NORMAL,MF_UNCHECKED);
	
	InvalidateRect( NULL, TRUE );
	UpdateWindow();
}

void CMyImageWorkView::OnFileOpen() 
{
	show_contrast = false;   //�ǶԱ���ʾ
	// TODO: Add your command handler code here
	static char szFilter[] = "BMP Files(*.BMP)|*.BMP||";

	CFileDialog FileDlg( TRUE, NULL, NULL,
		OFN_HIDEREADONLY, szFilter );

	if( FileDlg.DoModal() == IDOK &&
		m_Dib.Load( FileDlg.GetPathName() ) )
	{
		InvalidateRect( NULL, TRUE );
		UpdateWindow();
		have_open_a_file=true;
		if (m_Dib.tag == 0)//��ͼ��������Ϣ
		{
			embed = true;
			pick = false;  //�˵���������
		}//
		else
		{
			embed = false;
			pick = true;  //�˵���������
		}//
	}
}

void CMyImageWorkView::OnEmbed() 
{
	// TODO: Add your command handler code here
	//�����򿪶Ի����û�ѡ��Ƕ����ļ�

	static char szFilter[] = "All Files(*.*)|*.*||";

	CFileDialog FileDlg( TRUE, NULL, NULL,
		OFN_HIDEREADONLY, szFilter );

	if( FileDlg.DoModal() == IDOK &&
		m_Dib.LoadEmbFile( FileDlg.GetPathName() ) )
	{ 
		if (m_Dib.bitmap_size / m_Dib.embfile_size < 8)
		{
			AfxMessageBox("�ļ�̫���޷�Ƕ�룡");
		}
		else
		{
			show_contrast = true;
			m_Dib.BackUpDib(); //ͼƬԭʼ���ݱ��ݹ��Ա���ʾ
			m_Dib.Embed();   //Ƕ��
			
			Invalidate(true);//ˢ������ʾ����������Ϣ���ͼ��
			
			static char szFilter[] = "BMP Files(*.BMP)|*.BMP||";
			CFileDialog FileDlg( FALSE, "bmp", NULL,                //����
				OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter );
			
			if( FileDlg.DoModal() == IDOK )
			{
				m_Dib.Save( FileDlg.GetPathName() );
			}
			embed = false;
		}
		
	}
	
}

void CMyImageWorkView::OnPick() 
{
	// TODO: Add your command handler code here
	

	//��ȡ��ϣ���������Ի����û����б���
	static char szFilter[] = "All Files(*.*)|*.*||";
	CFileDialog FileDlg( FALSE, "", NULL,                //����
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter );
				
	if (FileDlg.DoModal() == IDOK)
	{	
		m_Dib.Pick();
		m_Dib.SavePicked(FileDlg.GetPathName());
		pick = false;
	}
				
}

void CMyImageWorkView::OnUpdatePick(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(pick);
	
}

void CMyImageWorkView::OnUpdateEmbed(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(embed);
}
