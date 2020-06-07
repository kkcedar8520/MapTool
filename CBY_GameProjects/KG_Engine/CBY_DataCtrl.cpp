#include "CBY_DataCtrl.h"

namespace CBY
{
	bool CBY_DataCtrl::Save(const TCHAR* pszSave)
	{
		m_data.dwStateNum = m_data.StateList.size();
		FILE* fp;
		fp = _tfopen(pszSave, _T("wt"));
		_ftprintf(fp, _T("%d %d %d %d %d %d\n"), m_data.m_CharType, m_data.SknNameList.size(),
			m_data.MtrList.size(), m_data.dwStateNum, m_data.ColBoxList.size(), m_data.ObjList.size());

		for (DWORD dw = 0; dw < m_data.SknNameList.size(); dw++)
		{
			_ftprintf(fp, _T("%s\n"), m_data.SknNameList[dw].c_str());
		}

		if (m_data.m_CharType == CHAR_FRAMETYPE)
		{
			for (DWORD dw = 0; dw < m_data.MtrList.size(); dw++)
			{
				_ftprintf(fp, _T("%s\n"), m_data.MtrList[dw].MtrName.c_str());
			}

			for (DWORD dwState = 0; dwState < m_data.StateList.size(); dwState++)
			{
				_ftprintf(fp, _T("%d %d\n"),
					m_data.StateList[dwState].m_iStartFrame, m_data.StateList[dwState].m_iEndFrame);
			}
		}
		else
		{
			for (DWORD dw = 0; dw < m_data.MtrList.size(); dw++)
			{
				if (m_data.MtrList[dw].State == -2)
				{
					continue;
				}
				_ftprintf(fp, _T("%s %d\n"), m_data.MtrList[dw].MtrName.c_str(), m_data.MtrList[dw].State);

			}
		}

		for (DWORD dw = 0; dw < m_data.ColBoxList.size(); dw++)
		{
			int index = m_data.ColBoxList[dw].GetBoneIndex();
			D3DXVECTOR3 pos = m_data.ColBoxList[dw].GetPos();
			D3DXVECTOR3 size = m_data.ColBoxList[dw].GetSize();
			_ftprintf(fp, _T("%d %10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n"), index
				, pos.x, pos.y, pos.z,
				size.x, size.y, size.z);
		}

		for (DWORD dw = 0; dw < m_data.ObjList.size(); dw++)
		{
			_ftprintf(fp, _T("%d %d %d %d %d %d\n"), m_data.ObjList[dw].objdata.m_CharType,
				m_data.ObjList[dw].objdata.SknNameList.size(),
				m_data.ObjList[dw].objdata.MtrList.size(),
				m_data.ObjList[dw].objdata.dwStateNum,
				m_data.ObjList[dw].objdata.ColBoxList.size(),
				m_data.ObjList[dw].objdata.ObjList.size());

			for (DWORD ob = 0; ob < m_data.ObjList[dw].objdata.SknNameList.size(); ob++)
			{
				_ftprintf(fp, _T("%s\n"), m_data.ObjList[dw].objdata.SknNameList[ob].c_str());
			}

			if (m_data.ObjList[dw].objdata.m_CharType == CHAR_FRAMETYPE)
			{
				for (DWORD ob = 0; ob < m_data.ObjList[dw].objdata.MtrList.size(); ob++)
				{
					_ftprintf(fp, _T("%s\n"), m_data.ObjList[dw].objdata.MtrList[ob].MtrName.c_str());
				}

				for (DWORD dwState = 0; dwState < m_data.ObjList[dw].objdata.StateList.size(); dwState++)
				{
					_ftprintf(fp, _T("%d %d\n"),
						m_data.ObjList[dw].objdata.StateList[dwState].m_iStartFrame,
						m_data.ObjList[dw].objdata.StateList[dwState].m_iEndFrame);
				}
			}
			else
			{
				for (DWORD ob = 0; ob < m_data.ObjList[dw].objdata.MtrList.size(); ob++)
				{
					if (m_data.ObjList[dw].objdata.MtrList[ob].State == -2)
					{
						continue;
					}
					_ftprintf(fp, _T("%s %d\n"), m_data.ObjList[dw].objdata.MtrList[ob].MtrName.c_str(),
						m_data.ObjList[dw].objdata.MtrList[ob].State);

				}
			}

			_ftprintf(fp, _T("%d %d %d\n"), m_data.ObjList[dw].Socket, m_data.ObjList[dw].ObjSocket, m_data.ObjList[dw].FireSocket);
		}

		fclose(fp);
		return true;
	}

	bool CBY_DataCtrl::Load(const TCHAR* pszLoad)
	{
		FILE* fp_src;
		fp_src = _tfopen(pszLoad, _T("rt"));
		if (fp_src == NULL) return false;


		TCHAR  pBuffer[256];
		TCHAR  pString[256];

		DWORD sknsize, mtrsize, boxsize, objsize;
		_fgetts(pBuffer, 256, fp_src);
		_stscanf_s(pBuffer, _T("%d%d%d%d%d%d"), &m_data.m_CharType, &sknsize, &mtrsize, &m_data.dwStateNum, &boxsize, &objsize);
		m_data.SknNameList.resize(sknsize);
		m_data.MtrList.resize(mtrsize);														//맨 윗부분 타입과 스킨, mtr갯수 받는곳
		m_data.ColBoxList.resize(boxsize);
		m_data.ObjList.resize(objsize);

		for (DWORD dw = 0; dw < sknsize; dw++)
		{
			_fgetts(pBuffer, 256, fp_src);
			_stscanf_s(pBuffer, _T("%s"), pString, _countof(pString));
			m_data.SknNameList[dw] = pString;
		}																					//스킨 갯수만큼 받아서 읽기


		if (m_data.m_CharType == CHAR_FRAMETYPE)
		{
			for (DWORD dw = 0; dw < mtrsize; dw++)
			{
				_fgetts(pBuffer, 256, fp_src);
				_stscanf_s(pBuffer, _T("%s"), pString, _countof(pString));
				m_data.MtrList[dw].MtrName = pString;
			}

			m_data.StateList.resize(m_data.dwStateNum);

			for (DWORD dwState = 0; dwState < m_data.dwStateNum; dwState++)
			{
				_fgetts(pBuffer, 256, fp_src);
				_stscanf_s(pBuffer, _T("%d%d"),
					&m_data.StateList[dwState].m_iStartFrame, &m_data.StateList[dwState].m_iEndFrame);
			}
		}
		else
		{
			for (DWORD dw = 0; dw < mtrsize; dw++)
			{
				_fgetts(pBuffer, 256, fp_src);
				_stscanf_s(pBuffer, _T("%s%d"), pString, _countof(pString), &m_data.MtrList[dw].State);
				m_data.MtrList[dw].MtrName = pString;
			}
		}

		for (DWORD dw = 0; dw < m_data.ColBoxList.size(); dw++)
		{
			int index;
			D3DXVECTOR3 pos;
			D3DXVECTOR3 size;
			_fgetts(pBuffer, 256, fp_src);
			_stscanf_s(pBuffer, _T("%d%f%f%f%f%f%f"), &index
				, &pos.x, &pos.y, &pos.z,
				&size.x, &size.y, &size.z);
			D3DXMATRIX mat;
			D3DXMatrixIdentity(&mat);
			m_data.ColBoxList[dw].CreateBox(index, pos, size.x, size.y, size.z, mat);
		}

		for (DWORD dw = 0; dw < objsize; dw++)
		{
			_fgetts(pBuffer, 256, fp_src);
			_stscanf_s(pBuffer, _T("%d%d%d%d%d%d"), &m_data.ObjList[dw].objdata.m_CharType,
				&sknsize, &mtrsize, &m_data.ObjList[dw].objdata.dwStateNum, &boxsize, &objsize);
			m_data.ObjList[dw].objdata.SknNameList.resize(sknsize);
			m_data.ObjList[dw].objdata.MtrList.resize(mtrsize);														//맨 윗부분 타입과 스킨, mtr갯수 받는곳
			m_data.ObjList[dw].objdata.ColBoxList.resize(boxsize);
			m_data.ObjList[dw].objdata.ObjList.resize(objsize);

			for (DWORD ob = 0; ob < sknsize; ob++)
			{
				_fgetts(pBuffer, 256, fp_src);
				_stscanf_s(pBuffer, _T("%s"), pString, _countof(pString));
				m_data.ObjList[dw].objdata.SknNameList[ob] = pString;
			}

			if (m_data.ObjList[dw].objdata.m_CharType == CHAR_FRAMETYPE)
			{
				for (DWORD ob = 0; ob < mtrsize; ob++)
				{
					_fgetts(pBuffer, 256, fp_src);
					_stscanf_s(pBuffer, _T("%s"), pString, _countof(pString));
					m_data.MtrList[ob].MtrName = pString;
				}

				m_data.ObjList[dw].objdata.StateList.resize(m_data.ObjList[dw].objdata.dwStateNum);

				for (DWORD dwState = 0; dwState < m_data.ObjList[dw].objdata.dwStateNum; dwState++)
				{
					_fgetts(pBuffer, 256, fp_src);
					_stscanf_s(pBuffer, _T("%d%d"),
						&m_data.ObjList[dw].objdata.StateList[dwState].m_iStartFrame, &m_data.ObjList[dw].objdata.StateList[dwState].m_iEndFrame);
				}
			}
			else
			{
				for (DWORD ob = 0; ob < mtrsize; ob++)
				{
					_fgetts(pBuffer, 256, fp_src);
					_stscanf_s(pBuffer, _T("%s%d"), pString, _countof(pString), &m_data.ObjList[dw].objdata.MtrList[ob].State);
					m_data.ObjList[dw].objdata.MtrList[ob].MtrName = pString;
				}
			}
			_fgetts(pBuffer, 256, fp_src);
			_stscanf_s(pBuffer, _T("%d%d%d"), &m_data.ObjList[dw].Socket, &m_data.ObjList[dw].ObjSocket, &m_data.ObjList[dw].FireSocket);

			_fgetts(pBuffer, 256, fp_src);
			_stscanf_s(pBuffer, _T("%f%f%f"), &m_data.ObjList[dw].vpos.x, &m_data.ObjList[dw].vpos.y, &m_data.ObjList[dw].vpos.z);
		}

		fclose(fp_src);
		return true;
	}

	TCHAR* CBY_DataCtrl::SaveFileDlg(TCHAR* szExt, TCHAR* szTitle)
	{
		OPENFILENAME    ofn;
		TCHAR            szFile[256], szFileTitle[256];
		static TCHAR     *szFilter;

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		_tcscpy_s(szFile, _T("*."));
		_tcscat_s(szFile, szExt);
		_tcscat_s(szFile, _T("\0"));

		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = GetActiveWindow();
		ofn.lpstrFilter = szFilter;
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter = 0L;
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFileTitle = szFileTitle;
		ofn.nMaxFileTitle = sizeof(szFileTitle);
		ofn.lpstrInitialDir = NULL;
		ofn.lpstrTitle = szTitle;
		ofn.Flags = 0L;
		ofn.nFileOffset = 0;
		ofn.nFileExtension = 0;
		ofn.lpstrDefExt = szExt;

		if (!GetSaveFileName(&ofn)) //GetOpenFileName
		{
			return NULL;
		}

		return szFile;
	}

	CBY_DataCtrl::CBY_DataCtrl()
	{
	}


	CBY_DataCtrl::~CBY_DataCtrl()
	{
	}
}
