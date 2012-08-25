#include <iostream>
#include <fstream>
using namespace std;
int s2l(int score)
{
	int i=0;
	while(true)
	{
		i++;
		if(score<30*i*i) break;
	}
	return i;
}
int main()
{
	//char outI[4],outScore[16],outNmb[16],outMaxerasingtimes[4],outLevel[4];
	ofstream sql("sql.txt");
	if(!sql)
		cerr << "can not open file" <<endl;
	for(int i=1;i<=300;i++)
	{
		int score,nmb,maxerasingtimes,level,year,month,day;
		score=rand()%10000;
		nmb=score/3+rand()%20;
		maxerasingtimes=rand()%15;
		level=s2l(score);
		year=1990+rand()%20;
		month=1+rand()%12;
		day=1+rand()%28;
		//_itoa_s(i,outI,sizeof(char)*4);
		//('1','481','235','4','5',to_date(20020101182112,'yyyymmddhh24miss') );
		sql<<"insert into RECORD values\r";
		sql<<"(\'"<<i<<"\',\'"<<score<<"\',\'"<<nmb<<"\',\'"<<maxerasingtimes<<"\',\'"<<level<<"\',to_date("<<year;
		if(month<10) sql<<0<<month;
		else sql<<month; 
		if(day<10) sql<<0<<day;
		else sql<<day; 
		sql<<",\'yyyymmdd\') )\;\r";
		
	}
	sql.close();
	return 0;
}