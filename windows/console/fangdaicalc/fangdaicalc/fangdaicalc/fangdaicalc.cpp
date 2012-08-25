// fangdaicalc.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <iostream>
#include <math.h>

using namespace std;

double yhke(double years, double dke, double nlv, double sf);

int _tmain(int argc, _TCHAR* argv[])
{
	double gjj_dkze = 0; //240000;
	double sd_dkze = 0; //200000;

	cout << endl ;
	cout << "* �ȶϢ��" << endl;

	cout << "    ��ҵ�����ܶ          " ;
	cin >> sd_dkze;

	cout << "    ����������ܶ        " ;
	cin >> gjj_dkze;
	
	int years = 15;

	cout << "    �������ޣ�              " ;
	cin >> years;

	double sd_my_hke = 0;

	if ( 0 != sd_dkze)
	{
		double sd_jzlv = 0.0705;
		double sd_sxf = 0.1;

		cout << "    ��ҵ�������׼���ʣ�    " ;
		cin >> sd_jzlv;

		cout << "    ��ҵ�������ʸ�����      " ;
		cin >> sd_sxf;

		sd_my_hke = yhke(years, sd_dkze, sd_jzlv, sd_sxf);
	}

	double gjj_my_hke = 0;

	if ( 0 != gjj_dkze)
	{
		double gjj_jzlv = 0.049;
		double gjj_sxf = 0.1;

		cout << "    ������������׼���ʣ�  " ;
		cin >> gjj_jzlv;

		cout << "    ������������ʸ�����    " ;
		cin >> gjj_sxf;

		gjj_my_hke = yhke(years, gjj_dkze, gjj_jzlv, gjj_sxf);
	}


	cout << endl ;
	cout << "* ��������" << endl;
	cout << "    �����ܶ              " << sd_dkze + gjj_dkze << endl;
	cout << "    �̴�ÿ�»���        " << sd_my_hke << endl;
	cout << "    ������ÿ�»���      " << gjj_my_hke << endl;
	cout << "    ÿ�»����ܶ          " << sd_my_hke + gjj_my_hke << endl;
	cout << "    �����ܶ              " << ( sd_my_hke + gjj_my_hke )*years*12 << endl;
	cout << "    ������Ϣ��              " << ( sd_my_hke + gjj_my_hke )*years*12 - sd_dkze - gjj_dkze << endl;

	return 0;
}

double yhke(double years, double dke, double nlv, double sf)
{
	double mlv = nlv*(1+sf)/12;

	double calc_block = pow((1+mlv),years*12);

	double my_hke = dke*mlv*calc_block/(calc_block-1);

	return my_hke;
}

