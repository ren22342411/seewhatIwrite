// TestADO.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestADO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		::CoInitialize(0);
		try
		{
			_ConnectionPtr pConn = NULL;
			pConn.CreateInstance(__uuidof(Connection));//�������Ӷ���
			pConn->ConnectionString = "Provider=MSDAORA.1;Password=tongji;User ID=system;Data Source=xe;Persist Security Info=True";//���������ַ���
			pConn ->ConnectionTimeout = 5;//�������ӵĳ�ʱʱ�䣬��ѡ
			pConn ->CursorLocation = adUseServer;//���ù���λ�ã���ѡ
			pConn ->Open("","","",-1);
			if ( NULL == pConn )
			{
				AfxMessageBox(L"����ʧ��");
			} 
			else
			{
				//AfxMessageBox(L"���ӳɹ�");
				_variant_t vt;
				//�õ�һ����¼��
				_RecordsetPtr rs = pConn->Execute("select * from \"TestOracle\"",&vt,adCmdText);
				//�ж���¼���Ƿ񵽴�ĩβ
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

				//�رղ��ͷż�¼��
				rs->Close();
				rs.Release();

				_RecordsetPtr rsUpdate(__uuidof(Recordset));
				rsUpdate->Open("select * from \"TestOracle\"",_variant_t((IDispatch*)pConn),adOpenStatic,adLockOptimistic,adCmdText);
				//���Ӽ�¼,����Ϊ�յ��ֶο��Բ���ֵ
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
			pConn->Close();//�ر����ݿ�����
			pConn.Release();//�ͷ����ݿ����Ӷ���ע����.����->��
		}
		catch (_com_error& e) {
			AfxMessageBox(e.Description());
		}
		::CoUninitialize();
	}

	return nRetCode;
}
