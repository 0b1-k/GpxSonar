#include "stdafx.h"
#include "GpxSonar.h"

#include "GpxSonarDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CGpxSonarDoc, CDocument)

BEGIN_MESSAGE_MAP(CGpxSonarDoc, CDocument)
	//{{AFX_MSG_MAP(CGpxSonarDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CGpxSonarDoc::CGpxSonarDoc()
{
}

CGpxSonarDoc::~CGpxSonarDoc()
{
}

void CGpxSonarDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

#ifdef _DEBUG
void CGpxSonarDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGpxSonarDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

BOOL CGpxSonarDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	
	return TRUE;
}

