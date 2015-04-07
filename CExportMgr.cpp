void CGpxSonarView::OnToolsExportExportoptions() 
{
	CExportPreferences Dlg;

	Dlg.m_ExportsNamesAsGpxName = (BOOL) m_ExportLocationMgr.m_ExportsNamesAsGpxName;
	Dlg.m_ExportMaxLimit = m_ExportLocationMgr.m_ExportMaxLimit;

	Dlg.DoModal();

	m_ExportLocationMgr.m_ExportsNamesAsGpxName = (bool) Dlg.m_ExportsNamesAsGpxName;
	m_ExportLocationMgr.m_ExportMaxLimit = Dlg.m_ExportMaxLimit;
}

void CGpxSonarView::ShowExportMsg(bool Limited, long Count, String& File)
{
	CString Msg;

	if (Limited)
	{
		Msg.Format(_T("Successfully wrote %ld out of %ld waypoints to %s file due to the export limit you have set or to a GPS limitation."), Count - 1, m_GpxParser.CacheCount(), File.c_str());
	}
	else
	{
		Msg.Format(_T("Successfully wrote %ld waypoints to %s"), Count, File.c_str());
	}

	MessageBox(Msg, _T("GPS Export Complete"), MB_OK | MB_ICONINFORMATION);
}

//
// BEGIN EXPORT Functions
//
void CGpxSonarView::OnExport2meridian() 
{
	CommonMagellanExports(MAGELLAN_EXPORT);
}

void CGpxSonarView::OnToolsExportWaypointsToexploristsdfile() 
{
	CommonMagellanExports(MAGELLAN_EXPLORIST_EXPORT);
}

void CGpxSonarView::CommonMagellanExports(TCHAR* pExportType)
{
	itGC it;

	long	Count = 0;
	bool	Limited = false;

	CMeridianWriter	MW;

	BeginWaitCursor();

	String	File;
	
	File = m_ExportLocationMgr.GetFilename(pExportType);

	MW.Open(File.c_str());

	CGeoCache* pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		if (Count >= 500 || Count >= m_ExportLocationMgr.m_ExportMaxLimit)
		{
			Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			MW.ProcessCache(*pCache);

			Count++;
		}

		pCache = m_GpxParser.Next(it);
	}

	MW.Close();

	EndWaitCursor();

	ShowExportMsg(Limited, Count, File);
}

void CGpxSonarView::OnMenuFileToolsExportExporttolocfile() 
{
	itGC it;

	long	Count = 0;
	bool	Limited = false;

	CLocFileWriter	LFW;

	BeginWaitCursor();

	String	File;
	
	File = m_ExportLocationMgr.GetFilename(LOC_EXPORT);

	LFW.Open(File.c_str());

	CGeoCache* pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		if (Count >= m_ExportLocationMgr.m_ExportMaxLimit)
		{
			Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			LFW.ProcessCache(*pCache);

			Count++;
		}

		pCache = m_GpxParser.Next(it);
	}

	LFW.Close();

	EndWaitCursor();

	ShowExportMsg(Limited, Count, File);
}

void CGpxSonarView::OnToolsExportFieldnotesTogsak() 
{
	String	File;
	
	File = m_ExportLocationMgr.GetFilename(GSAK_EXPORT);

	int Rc = m_NotesMgr.ExportToGSAK((TCHAR*) File.c_str());

	if (Rc < 0)
	{
		MessageBox(_T("The notes could not be exported to GSAK."), _T("Export Failed"), MB_OK | MB_ICONERROR);
	}
	else
	{
		CString Msg;

		Msg.Format(_T("%d notes successfully written to %s"), Rc, File.c_str());

		MessageBox(Msg, _T("GSAK Export Successful"), MB_OK | MB_ICONINFORMATION);
	}
}

void CGpxSonarView::OnToolsExportWaypointsTopharosostia6() 
{
	itGC	it;
	long	Count = 0;
	bool	Limited = false;

	CPharosOstia6Writer	PO;

	BeginWaitCursor();

	String	File;
	
	File = m_ExportLocationMgr.GetFilename(PHAROS_EXPORT);

	PO.Open(File.c_str());

	CGeoCache* pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		if (Count >= m_ExportLocationMgr.m_ExportMaxLimit)
		{
			Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			PO.ProcessCache(*pCache);
			Count++;
		}

		pCache = m_GpxParser.Next(it);
	}

	PO.Close();

	EndWaitCursor();

	ShowExportMsg(Limited, Count, File);
}

void CGpxSonarView::OnToolsExportWaypointsTomsstpushpins() 
{
	itGC	it;
	long	Count = 0;
	bool	Limited = false;

	CPocketStreetPushPinsWriter	PSPP;

	BeginWaitCursor();

	CString	File;
	
	File = m_ExportLocationMgr.GetFilename(MS_STREETTRIPS_EXPORT).c_str();

	// Count the # of waypoints to be written (required before opening the pushpin file)
	CGeoCache* pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		if (Count >= m_ExportLocationMgr.m_ExportMaxLimit)
		{
			Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			Count++;
		}

		pCache = m_GpxParser.Next(it);
	}

	if (Limited)
		Count--;

	// Set the total count of pushpins
	PSPP.SetTotalPushPins(Count);

	// Open the pushpin file and write the header
	PSPP.Open((LPCTSTR) File);

	Count = 0;
	Limited = false;

	pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		if (Count >= m_ExportLocationMgr.m_ExportMaxLimit)
		{
			Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			PSPP.ProcessCache(*pCache);
			Count++;
		}

		pCache = m_GpxParser.Next(it);
	}

	PSPP.Close();

	EndWaitCursor();

	CString	Msg;
	CString	MapFilename;

	if (Limited)
	{
		Msg.Format(_T("Successfully wrote %ld waypoints only to %s due to the export limit that you set."), Count, (LPCTSTR) File);
	}
	else
	{
		Msg.Format(_T("Successfully wrote %ld waypoints to %s"), Count, (LPCTSTR) File);
	}

	MessageBox(Msg, _T("Export Complete"), MB_OK | MB_ICONINFORMATION);

	long ExtPos = File.ReverseFind(_T('.'));

	// Check if there's a map corresponding to the pushpins
	if (ExtPos != -1)
	{
		// Extract the filename w/o the extension
		MapFilename = File.Left(ExtPos);

		// Add the 'mps' extension to the map filename
		MapFilename += _T(".mps");

		// Launch S+T with the push pins if the corresponding map exists
		if (GetFileAttributes((LPCTSTR) MapFilename) != -1)
		{
			CBrowserLauncher BL;

			BL.Go((LPCTSTR)MapFilename);
		}
	}
}

void CGpxSonarView::OnToolsExportWaypointsTomaptechmxfformat() 
{
	itGC	it;
	long	Count = 0;
	bool	Limited = false;

	CMxfFileWriter	PO;

	BeginWaitCursor();

	String	File;
	
	File = m_ExportLocationMgr.GetFilename(MAPTECH_MXF_EXPORT);

	PO.Open(File.c_str());

	CGeoCache* pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		if (Count >= m_ExportLocationMgr.m_ExportMaxLimit)
		{
			Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			PO.ProcessCache(*pCache);
			Count++;
		}

		pCache = m_GpxParser.Next(it);
	}

	PO.Close();

	EndWaitCursor();

	ShowExportMsg(Limited, Count, File);
}

void CGpxSonarView::OnToolsExportWaypointsTocsvfile() 
{
	itGC	it;
	long	Count = 0;
	bool	Limited = false;

	CCsvFileWriter	PO;

	BeginWaitCursor();

	String	File;
	
	File = m_ExportLocationMgr.GetFilename(CSV_EXPORT);

	PO.Open(File.c_str());

	CGeoCache* pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		if (Count >= m_ExportLocationMgr.m_ExportMaxLimit)
		{
			Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			PO.ProcessCache(*pCache);
			Count++;
		}

		pCache = m_GpxParser.Next(it);
	}

	PO.Close();

	EndWaitCursor();

	ShowExportMsg(Limited, Count, File);
}

void CGpxSonarView::OnToolsExportWaypointsTolowranceifinder() 
{
	itGC it;

	int		Count = 0;
	bool	Limited = false;

	CLowranceWriter	LW;

	BeginWaitCursor();

	// Count the # of waypoints that need to be written
	CGeoCache* pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		if (Count >= m_ExportLocationMgr.m_ExportMaxLimit)
		{
			Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			Count++;
		}

		pCache = m_GpxParser.Next(it);
	}
	
	// Set the total # of waypoints
	LW.SetWaypointCount(Count);

	Count = 0;
	Limited = false;
	
	// Write the waypoints out
	String	File;
	File = m_ExportLocationMgr.GetFilename(LOWRANCE_EXPORT);

	LW.Open(File.c_str());

	pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		if (Count >= m_ExportLocationMgr.m_ExportMaxLimit)
		{
			Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			LW.ProcessCache(*pCache);

			Count++;
		}

		pCache = m_GpxParser.Next(it);
	}

	LW.Close();

	EndWaitCursor();

	ShowExportMsg(Limited, Count, File);
}

void CGpxSonarView::OnToolsExportWaypointsTooziexplorerfile() 
{
	itGC	it;
	long	Count = 0;
	bool	Limited = false;

	COziWriter	OW;

	BeginWaitCursor();

	String	File;
	
	File = m_ExportLocationMgr.GetFilename(OZI_EXPORT);

	OW.Open(File.c_str());

	CGeoCache* pCache = m_GpxParser.First(it);

	while (!m_GpxParser.EndOfCacheList(it))
	{
		if (Count >= m_ExportLocationMgr.m_ExportMaxLimit)
		{
			Limited = true;
			break;
		}

		if (pCache->m_InScope)
		{
			OW.ProcessCache(*pCache);
			Count++;
		}

		pCache = m_GpxParser.Next(it);
	}

	OW.Close();

	EndWaitCursor();

	ShowExportMsg(Limited, Count, File);
}

//
// END EXPORT Functions
//