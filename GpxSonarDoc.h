// GpxSonarDoc.h : interface of the CGpxSonarDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GPXSONARDOC_H__A5B41CFC_93BE_468C_876B_91284CC3C305__INCLUDED_)
#define AFX_GPXSONARDOC_H__A5B41CFC_93BE_468C_876B_91284CC3C305__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CGpxSonarDoc : public CDocument
{
protected: // create from serialization only
	CGpxSonarDoc();
	DECLARE_DYNCREATE(CGpxSonarDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGpxSonarDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGpxSonarDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGpxSonarDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GPXSONARDOC_H__A5B41CFC_93BE_468C_876B_91284CC3C305__INCLUDED_)
