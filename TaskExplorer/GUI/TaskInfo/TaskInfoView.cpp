#include "stdafx.h"
#include "TaskInfoView.h"
#include "../TaskExplorer.h"
#include "ProcessView.h"
#include "HandlesView.h"
#include "SocketsView.h"
#include "ThreadsView.h"
#include "ModulesView.h"
#include "MemoryView.h"
#include "JobView.h"
#include "TokenView.h"
#include "WindowsView.h"
#include "EnvironmentView.h"
#include "../SystemInfo/ServicesView.h"


CTaskInfoView::CTaskInfoView(bool bAsWindow, QWidget* patent)
: CTabPanel(patent)
{
	setObjectName(bAsWindow ? "TaskWindow" : "TaskPanel");

	InitializeTabs();

	int ActiveTab = theConf->GetValue(objectName() + "/Tabs_Active").toInt();
	QStringList VisibleTabs = theConf->GetStringList(objectName() + "/Tabs_Visible");
	RebuildTabs(ActiveTab, VisibleTabs);
}


CTaskInfoView::~CTaskInfoView()
{
	int ActiveTab = 0;
	QStringList VisibleTabs;
	SaveTabs(ActiveTab, VisibleTabs);
	theConf->SetValue(objectName() + "/Tabs_Active", ActiveTab);
	theConf->SetValue(objectName() + "/Tabs_Visible", VisibleTabs);
}

void CTaskInfoView::InitializeTabs()
{
	m_pProcessView = new CProcessView(this);
	AddTab(m_pProcessView, tr("General"));

	m_pHandlesView = new CHandlesView(false, this);
	AddTab(m_pHandlesView, tr("Handles"));

	m_pSocketsView = new CSocketsView(false, this);
	AddTab(m_pSocketsView, tr("Sockets"));

	m_pThreadsView = new CThreadsView(this);
	AddTab(m_pThreadsView, tr("Threads"));

	m_pModulesView = new CModulesView(this);
	AddTab(m_pModulesView, tr("Modules"));

	m_pWindowsView = new CWindowsView(this);
	AddTab(m_pWindowsView, tr("Windows"));

	m_pMemoryView = new CMemoryView(this);
	AddTab(m_pMemoryView, tr("Memory"));

	m_pTokenView = new CTokenView(this);
	AddTab(m_pTokenView, tr("Token"));

#ifdef WIN32
	m_pJobView = new CJobView(this);
	AddTab(m_pJobView, tr("Job"));

	m_pServiceView = new CServicesView(false, this);
	AddTab(m_pServiceView, tr("Service"));
#endif

	m_pEnvironmentView = new CEnvironmentView(this);
	AddTab(m_pEnvironmentView, tr("Environment"));

	connect(m_pTabs, SIGNAL(currentChanged(int)), this, SLOT(OnTab(int)));
}

void CTaskInfoView::ShowProcess(const CProcessPtr& pProcess)
{
	m_pCurProcess = pProcess;

	Refresh();
}

void CTaskInfoView::OnTab(int tabIndex)
{
	Refresh();
}

void CTaskInfoView::Refresh()
{
	if (m_pCurProcess.isNull())
		return;

	if(m_pTabs->currentWidget() == m_pProcessView)
		m_pProcessView->ShowProcess(m_pCurProcess);
	else if(m_pTabs->currentWidget() == m_pSocketsView)
		m_pSocketsView->ShowSockets(m_pCurProcess);
	else if(m_pTabs->currentWidget() == m_pHandlesView)
		m_pHandlesView->ShowHandles(m_pCurProcess);
	else if(m_pTabs->currentWidget() == m_pThreadsView)
		m_pThreadsView->ShowThreads(m_pCurProcess);
	else if(m_pTabs->currentWidget() == m_pModulesView)
		m_pModulesView->ShowModules(m_pCurProcess);
	else if(m_pTabs->currentWidget() == m_pWindowsView)
		m_pWindowsView->ShowWindows(m_pCurProcess);
	else if(m_pTabs->currentWidget() == m_pMemoryView)
		m_pMemoryView->ShowMemory(m_pCurProcess);
	else if(m_pTabs->currentWidget() == m_pTokenView)
		m_pTokenView->ShowToken(m_pCurProcess);
#ifdef WIN32
	else if(m_pTabs->currentWidget() == m_pJobView)
		m_pJobView->ShowJob(m_pCurProcess);
	else if(m_pTabs->currentWidget() == m_pServiceView)
		m_pServiceView->ShowServices(m_pCurProcess);
#endif
	else if(m_pTabs->currentWidget() == m_pEnvironmentView)
		m_pEnvironmentView->ShowEnvVariables(m_pCurProcess);
}