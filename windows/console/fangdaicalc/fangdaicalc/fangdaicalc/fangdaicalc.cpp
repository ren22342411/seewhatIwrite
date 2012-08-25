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
	cout << "* 等额本息：" << endl;

	cout << "    商业代款总额：          " ;
	cin >> sd_dkze;

	cout << "    公积金代款总额：        " ;
	cin >> gjj_dkze;
	
	int years = 15;

	cout << "    代款年限：              " ;
	cin >> years;

	double sd_my_hke = 0;

	if ( 0 != sd_dkze)
	{
		double sd_jzlv = 0.0705;
		double sd_sxf = 0.1;

		cout << "    商业代款年基准利率：    " ;
		cin >> sd_jzlv;

		cout << "    商业代款利率浮动：      " ;
		cin >> sd_sxf;

		sd_my_hke = yhke(years, sd_dkze, sd_jzlv, sd_sxf);
	}

	double gjj_my_hke = 0;

	if ( 0 != gjj_dkze)
	{
		double gjj_jzlv = 0.049;
		double gjj_sxf = 0.1;

		cout << "    公积金代款年基准利率：  " ;
		cin >> gjj_jzlv;

		cout << "    公积金代款利率浮动：    " ;
		cin >> gjj_sxf;

		gjj_my_hke = yhke(years, gjj_dkze, gjj_jzlv, gjj_sxf);
	}


	cout << endl ;
	cout << "* 计算结果：" << endl;
	cout << "    贷款总额：              " << sd_dkze + gjj_dkze << endl;
	cout << "    商代每月还款额：        " << sd_my_hke << endl;
	cout << "    公积金每月还款额：      " << gjj_my_hke << endl;
	cout << "    每月还款总额：          " << sd_my_hke + gjj_my_hke << endl;
	cout << "    还款总额：              " << ( sd_my_hke + gjj_my_hke )*years*12 << endl;
	cout << "    还款利息：              " << ( sd_my_hke + gjj_my_hke )*years*12 - sd_dkze - gjj_dkze << endl;

	return 0;
}

double yhke(double years, double dke, double nlv, double sf)
{
	double mlv = nlv*(1+sf)/12;

	double calc_block = pow((1+mlv),years*12);

	double my_hke = dke*mlv*calc_block/(calc_block-1);

	return my_hke;
}

