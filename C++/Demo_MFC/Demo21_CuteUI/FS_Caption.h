#pragma once


// FS_Caption

class FS_Caption : public CStatic
{
	DECLARE_DYNAMIC(FS_Caption)

public:
	FS_Caption();
	virtual ~FS_Caption();
	void Init(int x, int y, int W, int H);

protected:
	HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	DECLARE_MESSAGE_MAP()
};


