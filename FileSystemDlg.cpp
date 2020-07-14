
// FileSystemDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "FileSystem.h"
#include "FileSystemDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileSystemDlg 对话框



CFileSystemDlg::CFileSystemDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILESYSTEM_DIALOG, pParent)
{
	m_hRoot = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	dir_path = _T("");
	f_cnt = 2;
	t_cnt = 2;
	d_cnt = 2;
}

void CFileSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_DIRPATH, m_combo);
}

BEGIN_MESSAGE_MAP(CFileSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE, &CFileSystemDlg::OnItemexpandedTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CFileSystemDlg::OnSelchangedTree)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CFileSystemDlg::OnDblclkList)
	ON_COMMAND(ID_Open, &CFileSystemDlg::OnOpen)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, &CFileSystemDlg::OnRclickList)
	ON_COMMAND(ID_Copy, &CFileSystemDlg::OnCopy)
	ON_COMMAND(ID_Delete, &CFileSystemDlg::OnDelete)
//	ON_WM_CONTEXTMENU()
//ON_COMMAND(ID_Paste, &CFileSystemDlg::OnPaste)
ON_COMMAND(ID_NEWFILE, &CFileSystemDlg::OnNewfile)
ON_COMMAND(ID_TXT, &CFileSystemDlg::OnTxt)
ON_COMMAND(ID_DOCX, &CFileSystemDlg::OnDocx)
ON_COMMAND(ID_REFRESH, &CFileSystemDlg::OnRefresh)
ON_COMMAND(ID_PASTE, &CFileSystemDlg::OnPaste)
ON_BN_CLICKED(IDC_BACK, &CFileSystemDlg::OnClickedBack)
ON_BN_CLICKED(IDC_ENTER, &CFileSystemDlg::OnClickedEnter)
ON_CBN_SELCHANGE(IDC_DIRPATH, &CFileSystemDlg::OnSelchangeDirpath)
END_MESSAGE_MAP()


// CFileSystemDlg 消息处理程序

BOOL CFileSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ImageList.Create(32, 32, ILC_COLOR32, 10, 30);     //创建图像序列CImageList对象 
	m_tree.SetImageList(&m_ImageList, LVSIL_NORMAL);  //为树形控件设置图像序列  
	HICON hIcon = theApp.LoadIcon(IDI_ICON1);        //图标句柄
	m_ImageList.Add(hIcon);                          //图标添加到图像序列
	m_tree.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS);
	m_hRoot = m_tree.InsertItem(_T("我的电脑"));         //插入根节点
	GetLogicalDrive(m_hRoot);                      //自定义函数 获取驱动
	GetDriveDir(m_hRoot);                           //自定义函数 获取驱动子项
	m_tree.Expand(m_hRoot, TVE_EXPAND);              //展开或折叠子项列表 TVE_EXPAND展开列表
	//m_list.InsertColumn(0, "序号", LVCFMT_LEFT, 40);
	m_list.InsertColumn(0, _T("文件名"), LVCFMT_LEFT, 300);
	m_list.InsertColumn(1, _T("类型"), LVCFMT_LEFT, 100);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	m_combo.SetMinVisibleItems(6);//设置显示的行数为5
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFileSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//函数功能:展开事件函数
void CFileSystemDlg::OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	TVITEM item = pNMTreeView->itemNew;                  //发送\接受关于树形视图项目信息
	if (item.hItem == m_hRoot)
		return;
	HTREEITEM hChild = m_tree.GetChildItem(item.hItem);  //获取指定位置中的子项
	while (hChild)
	{
		AddSubDir(hChild);                               //添加子目录
		hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);   //获取树形控件TVGN_NEXT下兄弟项
	}
	*pResult = 0;
}

// 函数功能:选中事件显示图标
void CFileSystemDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_list.DeleteAllItems();
	TVITEM item = pNMTreeView->itemNew;
	if (item.hItem == m_hRoot) {
		return;
	}
	CString str = GetFullPath(item.hItem);
	if (str.Right(1) != "\\") {
		str += "\\";
	}
	//m_combo.AddString(str);
	m_combo.InsertString( 0, str);
	m_combo.SetCurSel(0);
	str += "*.*";

	CFileFind file;
	BOOL bContinue = file.FindFile(str);
	while (bContinue) {
		bContinue = file.FindNextFileW();
		if (/*file.IsDirectory() && */!file.IsDots()) {
			SHFILEINFO info = { 0 };
			CString temp = str;
			int index = temp.Find(_T("*.*"));
			temp.Delete(index, 3);
			SHGetFileInfo(temp + file.GetFileName(), 0, &info, sizeof(&info), SHGFI_DISPLAYNAME | SHGFI_ICON);
			int i = m_ImageList.Add(info.hIcon);
			m_list.SetImageList(&m_ImageList, LVSIL_SMALL);
			m_list.InsertItem(i, info.szDisplayName, i);


		}
	}
	*pResult = 0;
}


void CFileSystemDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListCtrl = (NM_LISTVIEW*)pNMHDR; //该变量是个结构体指针，该结构体中存放着双击的行号(iItem),以及列号(iSubItem),当未选中时返回-1
	int nItem = pNMListCtrl->iItem;//行号 
	//pNMLV->iSubItem;//列号 
	if (nItem >= 0 && nItem < m_list.GetItemCount())//判断双击内容是否存在
	{
		CString strTemp;
		m_combo.GetWindowText(strTemp);
		if (strTemp.Right(1) != "\\") {
			strTemp += "\\";
		}
		strTemp += m_list.GetItemText(nItem, 0);//选中行的第1项，索引为0
		//ShellExecute(NULL, TEXT("OPEN"), strTemp, NULL, NULL, SW_SHOWNORMAL);
		Refresh(strTemp);
	
	}
	*pResult = 0;
}







void CFileSystemDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem == -1) {			//没有选中某一项的时候
		POINT point;
		GetCursorPos(&point);
		CMenu menu;
		menu.LoadMenu(IDR_NEWFILE_EDIT); //读取资源
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
		return;
	}
	POINT pt;
	GetCursorPos(&pt);
	int x = m_list.GetSelectionMark();

	CMenu menu;
	menu.LoadMenu(IDR_POPUP_EDIT);
	CMenu* pop = menu.GetSubMenu(0);
	pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

	
	*pResult = 0;
}

void CFileSystemDlg::GetLogicalDrive(HTREEITEM hParent)
{
	// TODO: 在此处添加实现代码.
	//获取系统分区驱动器字符串信息
	TCHAR szBuf[100];
	memset(szBuf, 0, 100);

	DWORD len = GetLogicalDriveStrings(sizeof(szBuf) / sizeof(TCHAR), szBuf);

	for (TCHAR* s = szBuf; *s; s += _tcslen(s) + 1)
	{
		CString k = s;
		m_tree.InsertItem(k, hParent);
	}


}

void CFileSystemDlg::GetDriveDir(HTREEITEM hParent)
{
	// TODO: 在此处添加实现代码.
	HTREEITEM hChild = m_tree.GetChildItem(hParent);   //获取指定位置中的子项
	while (hChild)
	{
		CString strText = m_tree.GetItemText(hChild);  //检索列表中项目文字
		if (strText.Right(1) != "\\")                   //从右边1开始获取从右向左nCount个字符
			strText += _T("\\");
		strText += "*.*";
		//将当前目录下文件枚举并InsertItem树状显示
		CFileFind file;                                       //定义本地文件查找
		BOOL bContinue = file.FindFile(strText);              //查找包含字符串的文件
		while (bContinue)
		{
			bContinue = file.FindNextFile();                  //查找下一个文件
			if (file.IsDirectory() && !file.IsDots())          //找到文件为内容且不为点"."
				m_tree.InsertItem(file.GetFileName(), hChild); //添加盘符路径下树状文件夹
		}
		GetDriveDir(hChild);                                  //递归调用
		hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);        //获取树形控件TVGN_NEXT下兄弟项
	}
}


//函数功能:获取树项目全根路径
CString CFileSystemDlg::GetFullPath(HTREEITEM hCurrent)
{
	// TODO: 在此处添加实现代码.
	CString strTemp;
	CString strReturn = _T("");
	while (hCurrent != m_hRoot)
	{
		strTemp = m_tree.GetItemText(hCurrent);    //检索列表中项目文字
		if (strTemp.Right(1) != "\\")
			strTemp += "\\";
		strReturn = strTemp + strReturn;
		hCurrent = m_tree.GetParentItem(hCurrent); //返回父项目句柄
	}
	return strReturn;
}

//函数功能:添加子目录
void CFileSystemDlg::AddSubDir(HTREEITEM hParent)
{
	// TODO: 在此处添加实现代码.
	CString strPath = GetFullPath(hParent);     //获取全路径
	if (strPath.Right(1) != "\\")
		strPath += "\\";
	strPath += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);    //查找包含字符串的文件
	while (bContinue)
	{
		bContinue = file.FindNextFile();        //查找下一个文件
		if (file.IsDirectory() && !file.IsDots())
			m_tree.InsertItem(file.GetFileName(), hParent);
	}


}

void CFileSystemDlg::OnOpen()
{
	// TODO: 在此添加命令处理程序代码
	int nItem = m_list.GetSelectionMark();

	if (nItem >= 0 && nItem < m_list.GetItemCount())//判断双击内容是否存在
	{
		CString strTemp;
		m_combo.GetWindowText(strTemp);
		if (strTemp.Right(1) != "\\") {
			strTemp += "\\";
		}
		strTemp += m_list.GetItemText(nItem, 0);//选中行的第1项，索引为0
		Refresh(strTemp);

	}
}

void CFileSystemDlg::OnCopy()
{
	// TODO: 在此添加命令处理程序代码
	int nItem = m_list.GetSelectionMark();

	if (nItem >= 0 && nItem < m_list.GetItemCount())//判断双击内容是否存在
	{
		CString strTemp, curStr;
		m_combo.GetWindowText(strTemp);
		if (strTemp.Right(1) != "\\") {
			strTemp += "\\";
		}
		curStr = strTemp;
		strTemp += m_list.GetItemText(nItem, 0);//选中行的第1项，索引为0
		//dir_path = strTemp;
		CopyToClipboard(strTemp);
	}
}

void CFileSystemDlg::OnDelete()
{
	// TODO: 在此添加命令处理程序代码
	int nItem = m_list.GetSelectionMark();

	if (nItem >= 0 && nItem < m_list.GetItemCount())//判断双击内容是否存在
	{
		CString strTemp, curStr;
		m_combo.GetWindowText(strTemp);
		if (strTemp.Right(1) != "\\") {
			strTemp += "\\";
		}
		curStr = strTemp;
		strTemp += m_list.GetItemText(nItem, 0);//选中行的第1项，索引为0
		if (GetFileAttributes(strTemp) & FILE_ATTRIBUTE_DIRECTORY) {
			DeleteFolder(strTemp);
		}
		else {
			DeleteFile(strTemp);
		}
		Refresh(curStr);
	}
}

void CFileSystemDlg::OnRefresh()
{
	// TODO: 在此添加命令处理程序代码
	CString strTemp;
	m_combo.GetWindowText(strTemp);
	if (strTemp.Right(1) != "\\") {
		strTemp += "\\";
	}
	Refresh(strTemp);
}

void CFileSystemDlg::OnNewfile()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	m_combo.GetWindowText(str);
	if (str.Right(1) != "\\") {
		str += "\\";
	}
	str += _T("新建文件夹1");
	while (PathIsDirectory(str)) {
		str.Delete(str.GetLength() - 1, 1);
		CString chg;
		chg.Format(_T("%d"), f_cnt);
		str += chg;
		f_cnt++;
	}
	
	if (!PathIsDirectory(str))
	{
		CreateDirectory(str, NULL);//创建目录,已有的话不影响
	}

	CString strTemp;
	m_combo.GetWindowText(strTemp);
	if (strTemp.Right(1) != "\\") {
		strTemp += "\\";
	}
	Refresh(strTemp);
}


void CFileSystemDlg::OnTxt()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	m_combo.GetWindowText(str);
	if (str.Right(1) != "\\") {
		str += "\\";
	}
	str += _T("新建文本文档1.txt");
	while (PathFileExists(str)) {

		CString chg;
		chg.Format(_T("%d"), t_cnt);
		str.SetAt(str.Find(_T(".")) - 1, chg.GetAt(0));
		t_cnt++;
	}
	CFile file(str, CFile::modeCreate);
	file.Close();
	CString strTemp;
	m_combo.GetWindowText(strTemp);
	if (strTemp.Right(1) != "\\") {
		strTemp += "\\";
	}
	Refresh(strTemp);
}


void CFileSystemDlg::OnDocx()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	m_combo.GetWindowText(str);
	if (str.Right(1) != "\\") {
		str += "\\";
	}
	str += _T("新建DOCX文档1.docx");
	while (PathFileExists(str)) {
		
		CString chg;
		chg.Format(_T("%d"), d_cnt);
		str.SetAt(str.Find(_T(".")) - 1, chg.GetAt(0));
		d_cnt++;
	}
	CFile file(str, CFile::modeCreate);
	file.Close();
	CString strTemp;
	m_combo.GetWindowText(strTemp);
	if (strTemp.Right(1) != "\\") {
		strTemp += "\\";
	}
	Refresh(strTemp);
}


void CFileSystemDlg::OnPaste()
{
	// TODO: 在此添加命令处理程序代码
	CString str;
	m_combo.GetWindowText(str);
	if (str.Right(1) != "\\") {
		str += "\\";
	}
	PasteToFile(str);
	CString strTemp;
	m_combo.GetWindowText(strTemp);
	if (strTemp.Right(1) != "\\") {
		strTemp += "\\";
	}
	Refresh(strTemp);
	
	
}

/*删除目录及目录中的所有内容*/
bool CFileSystemDlg::DeleteFolder(LPCTSTR pstrFolder)
{
	// TODO: 在此处添加实现代码.
	if ((NULL == pstrFolder))
	{
		return FALSE;
	}

	/*检查输入目录是否是合法目录*/
	if (!IsDirectory(pstrFolder))
	{
		return FALSE;
	}

	/*创建源目录中查找文件的通配符*/
	CString strWildcard(pstrFolder);
	if (strWildcard.Right(1) != _T('\\'))
	{
		strWildcard += _T("\\");
	}
	strWildcard += _T("*.*");

	/*打开文件查找，查看源目录中是否存在匹配的文件*/
	/*调用FindFile后，必须调用FindNextFile才能获得查找文件的信息*/
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		/*查找下一个文件*/
		bWorking = finder.FindNextFile();

		/*跳过当前目录“.”和上一级目录“..”*/
		if (finder.IsDots())
		{
			continue;
		}

		/*得到当前目录的子文件的路径*/
		CString strSubFile = finder.GetFilePath();

		/*判断当前文件是否是目录,*/
		/*如果是目录，递归调用删除目录,*/
		/*否则，直接删除文件*/
		if (finder.IsDirectory())
		{
			if (!DeleteFolder(strSubFile))
			{
				finder.Close();
				return FALSE;
			}
		}
		else
		{
			if (!DeleteFile(strSubFile))
			{
				finder.Close();
				return FALSE;
			}
		}

	} /*while (bWorking)*/

	/*关闭文件查找*/
	finder.Close();

	/*删除空目录*/
	return RemoveDirectory(pstrFolder);
	
}

/*判断一个路径是否是已存在的目录*/
bool CFileSystemDlg::IsDirectory(LPCTSTR pstrPath)
{
	// TODO: 在此处添加实现代码.
	if (NULL == pstrPath)
	{
		return FALSE;
	}

	/*去除路径末尾的反斜杠*/
	CString strPath = pstrPath;
	if (strPath.Right(1) == _T('\\'))
	{
		strPath.Delete(strPath.GetLength() - 1);
	}

	CFileFind finder;
	BOOL bRet = finder.FindFile(strPath);
	if (!bRet)
	{
		return FALSE;
	}

	/*判断该路径是否是目录*/
	finder.FindNextFile();
	bRet = finder.IsDirectory();
	finder.Close();
	return bRet;
}


void CFileSystemDlg::OnClickedBack()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	if (m_combo.GetCount() == 1) {
		return;
	}
	m_combo.GetLBText(1, strTemp);
	
	if (strTemp.Right(1) != "\\") {
		strTemp += "\\";
	}
	if (IsDirectory(strTemp)) {
		m_combo.DeleteString(0);
		m_combo.DeleteString(0);
		Refresh(strTemp);
	}
}


void CFileSystemDlg::OnClickedEnter()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	m_combo.GetWindowText(strTemp);
	if (strTemp.Right(1) != "\\") {
		strTemp += "\\";
	}
	if (IsDirectory(strTemp)) {
		Refresh(strTemp);
	}
}


void CFileSystemDlg::OnSelchangeDirpath()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTemp;
	//m_combo.GetWindowText(strTemp);
	int nIndex = m_combo.GetCurSel();
	m_combo.GetLBText(nIndex, strTemp);
	if (strTemp.Right(1) != "\\") {
		strTemp += "\\";
	}
	if (IsDirectory(strTemp)) {
		Refresh(strTemp);
	}
}


void CFileSystemDlg::CopyToClipboard(CString dirPath)
{
	// TODO: 在此处添加实现代码.
	dir_path = dirPath;
}


void CFileSystemDlg::PasteToFile(CString dirPath)
{
	// TODO: 在此处添加实现代码.
	if (GetFileAttributes(dir_path) & FILE_ATTRIBUTE_DIRECTORY) {
		dirPath += _T("新建文件夹1");
		while (PathIsDirectory(dirPath)) {
			dirPath.Delete(dirPath.GetLength() - 1, 1);
			CString chg;
			chg.Format(_T("%d"), f_cnt);
			dirPath += chg;
			f_cnt++;
		}

		if (!PathIsDirectory(dirPath))
		{
			CreateDirectory(dirPath, NULL);//创建目录,已有的话不影响
		}
		if (dirPath.Right(1) != "\\")//目录的最右边需要“\”字符
			dirPath += "\\";

		CFileFind filefind;
		CString path = dir_path;
		if (path.Right(1) != "\\")//目录的最右边需要“\”字符
			path += "\\";
		path += "*.*";
		BOOL res = filefind.FindFile(path);
		while (res)
		{
			res = filefind.FindNextFile();
			if (!filefind.IsDirectory() && !filefind.IsDots())
			{
				CopyFile(filefind.GetFilePath(), dirPath + filefind.GetFileName(), TRUE);// 如果目标已经存在，不拷贝（True）并返回False，覆盖目标（false）
			}
		}

		filefind.Close();

	}
	else {
		CString type = dir_path;
		CString filename = type.Right(type.GetLength() - type.ReverseFind('\\') - 1);

		dirPath += filename;
		while (PathFileExists(dirPath)) {
			CString chg;
			chg.Format(_T("%d"), t_cnt);
			dirPath.SetAt(dirPath.Find(_T(".")) - 1, chg.GetAt(0));
			t_cnt++;
		}
		CopyFile(dir_path, dirPath, TRUE);// 如果目标已经存在，不拷贝（True）并返回False，覆盖目标（false）
	}
}

// 更新ListCtrl
void CFileSystemDlg::Refresh(CString strTemp)
{
	// TODO: 在此处添加实现代码.
	if (GetFileAttributes(strTemp) & FILE_ATTRIBUTE_DIRECTORY) {
		//m_combo.DeleteString(0);
		m_combo.InsertString(0, strTemp);
		m_combo.SetCurSel(0);
		if (strTemp.Right(1) != "\\") {
			strTemp += "\\";
		}
		strTemp += "*.*";
		CFileFind file;
		BOOL bContinue = file.FindFile(strTemp);
		m_list.DeleteAllItems();
		while (bContinue) {
			bContinue = file.FindNextFileW();
			if (!file.IsDots()) {
				SHFILEINFO info = { 0 };
				CString temp = strTemp;
				int index = temp.Find(_T("*.*"));
				temp.Delete(index, 3);
				SHGetFileInfo(temp + file.GetFileName(), 0, &info, sizeof(&info), SHGFI_DISPLAYNAME | SHGFI_ICON);
				int i = m_ImageList.Add(info.hIcon);
				m_list.SetImageList(&m_ImageList, LVSIL_SMALL);
				m_list.InsertItem(i, info.szDisplayName, i);


			}
		}
	}
	else {
		if (PathFileExists(strTemp))
			ShellExecute(NULL, TEXT("OPEN"), strTemp, NULL, NULL, SW_SHOWNORMAL);	//调用外部程序打开文件
	}

}