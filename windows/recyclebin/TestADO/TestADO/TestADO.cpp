// TestADO.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "TestADO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
		::CoInitialize(0);
		try
		{
			_ConnectionPtr pConn = NULL;
			pConn.CreateInstance(__uuidof(Connection));//创建连接对象
			pConn->ConnectionString = "Provider=MSDAORA.1;Password=tongji;User ID=system;Data Source=xe;Persist Security Info=True";//设置连接字符串
			pConn ->ConnectionTimeout = 5;//设置连接的超时时间，可选
			pConn ->CursorLocation = adUseServer;//设置光标的位置，可选
			pConn ->Open("","","",-1);
			if ( NULL == pConn )
			{
				AfxMessageBox(L"连接失败");
			} 
			else
			{
				//AfxMessageBox(L"连接成功");
				_variant_t vt;
				//得到一个记录集
				_RecordsetPtr rs = pConn->Execute("select * from \"TestOracle\"",&vt,adCmdText);
				//判定记录集是否到达末尾
				int i = 0;
				while (!rs->ENDOFRS)
				{
					//_variant_t vA = rs->Fields->GetItem("A")->Value;
					_variant_t vA = rs->Fields->GetItem(long(0))->Value;
					//_variant_t vB = rs->Fields->GetItem("B")->Value;
					_variant_t vB = rs->Fields->GetItem(long(1))->Value;
					//_variant_t vC = rs->Fields->GetItem("C")->Value;
					_variant_t vC = rs->Fields->GetItem(long(2))->Value;

					char szA[10];
					char szB[10];
					double nC;

					if (vA.vt == NULL)
					{
						strcpy(szA,"NULL");
					}
					else
					{
						WideCharToMultiByte(CP_ACP,0,vA.bstrVal,-1,szA,sizeof(szA),0,0);
					}
		
					if (vB.vt == NULL)
					{
						strcpy(szB,"NULL");
					}
					else
					{
						WideCharToMultiByte(CP_ACP,0,vB.bstrVal,-1,szB,sizeof(szB),0,0);
					}

					nC = vC.dblVal;

					cout << szA <<"\t"<< szB <<"\t"<< nC <<endl;

					rs->MoveNext();
				}

				//关闭并释放记录集
				rs->Close();
				rs.Release();

				_RecordsetPtr rsUpdate(__uuidof(Recordset));
				rsUpdate->Open("select * from \"TestOracle\"",_variant_t((IDispatch*)pConn),adOpenStatic,adLockOptimistic,adCmdText);
				//增加记录,允许为空的字段可以不赋值
				rsUpdate->LockType=adLockPessimistic;
				rsUpdate->CursorType=adOpenDynamic;
				rsUpdate->CursorLocation=adUseServer;
				rsUpdate->AddNew();
				//rsUpdate->Fields->GetItem("A")->Value = _bstr_t("Ren");
				//rsUpdate->Fields->GetItem("C")->Value = double(789);
				rsUpdate->Update();

				rsUpdate->Close();
				rsUpdate->Release();
			}
			pConn->Close();//关闭数据库连接
			pConn.Release();//释放数据库连接对象（注意是.不是->）
		}
		catch (_com_error& e) {
			AfxMessageBox(e.Description());
		}
		::CoUninitialize();
	}

	return nRetCode;
}
