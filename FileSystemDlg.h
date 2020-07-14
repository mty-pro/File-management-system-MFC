
// FileSystemDlg.h: 头文件
//

#pragma once


// CFileSystemDlg 对话框
class CFileSystemDlg : public CDialogEx
{
// 构造
public:
	CFileSystemDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILESYSTEM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CTreeCtrl m_tree;
	CComboBox m_combo;
protected:
	HTREEITEM m_hRoot;
	CImageList m_ImageList;
	CString dir_path;
	int f_cnt;
	int t_cnt;
	int d_cnt;
public:
	void GetLogicalDrive(HTREEITEM hParent);
	void GetDriveDir(HTREEITEM hParent);
	CString GetFullPath(HTREEITEM hCurrent);
	void AddSubDir(HTREEITEM hParent);
	afx_msg void OnItemexpandedTree(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnOpen();
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCopy();
	afx_msg void OnDelete();
	afx_msg void OnNewfile();
	afx_msg void OnTxt();
	// 更新ListCtrl
	void Refresh(CString str);
	afx_msg void OnDocx();
	afx_msg void OnRefresh();
	afx_msg void OnPaste();
	bool DeleteFolder(LPCTSTR pstrFolder);
	bool IsDirectory(LPCTSTR pstrPath);
	afx_msg void OnClickedBack();
	afx_msg void OnClickedEnter();
	afx_msg void OnSelchangeDirpath();
	void CopyToClipboard(CString dirPath);
	void PasteToFile(CString dirPath);
};
