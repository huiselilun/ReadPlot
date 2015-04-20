#include <iostream>  
#include <fstream>  
#include <string>  
#include <cctype>
#include <iomanip>
#include "discpp.h"

using namespace std;  

int main()
{
		ifstream infile;
		infile.open("plot.out",ios::in);
		string tmp;
		int pos = 0,pos2 = 0,IT = 1;

		const int MAXTN = 501, MAXWN = 21;  // 最大存储1000个时间步，20口井的数据
		double D[MAXTN];    // 存储Time后的所有值
		string WELLNAME[MAXWN];   // 存储井名，最大十个字符
		double OILRATE[MAXWN][MAXTN],WATERRATE[MAXWN][MAXTN]; // 油水产量
		double GASRATE[MAXWN][MAXTN],GOR[MAXWN][MAXTN];   // 气产量，气油比
		double WOR[MAXWN][MAXTN];	// 油水比
		double WCUT[MAXWN][MAXTN];	//含水率
		double FBHP[MAXWN][MAXTN];    //井底压力
		bool FirstRead = true;
		
		for (int i = 1;i < MAXTN;i++)
			D[i] = 0.0;
		for (int i = 1;i < MAXWN;i++)
			WELLNAME[i] = "";

		if(infile.fail())//文件打开失败:返回0  
		{  
		    return 0;  
		}  
		else//文件存在  
		{  
			streampos pos = infile.tellg();
		    while(getline(infile,tmp,'\n') && !infile.eof() )  
		    {
				streampos pos2 = infile.tellg();
		          if(tmp.find("Time") != string::npos)
				  {
						string c1,c2,c3,c4,c5,c6;
						int	a;

						double DTIME,ORA,WR,GR,G,WO,WC,FB;
						int IWELL;
						string WELL;
						
						//cout << tmp << "\n";
						infile.seekg(pos-pos2,ios::cur);	//读取指针移动到字符Time前
						infile >> c1 >> c2 >> DTIME;
						getline(infile,tmp,'\n');	//将读取指针移动到本行末尾
						infile >> IWELL >> WELL >> a >> c3 >> c4 >> c5 >> c6
							>> ORA>>WR>>GR>>G>>WO>>WC>>FB;

						if (IT == 1 && FirstRead)// 如果是第一个时间步数据
		    				{
							D[IT] = DTIME;
		    					WELLNAME[IWELL] = WELL;
		    					OILRATE[IWELL][IT] = ORA;
		    					WATERRATE[IWELL][IT] = WR;
		    					GASRATE[IWELL][IT] = GR;
		    					GOR[IWELL][IT] = G;
		    					WOR[IWELL][IT] = WO;
		    					WCUT[IWELL][IT] = WC;
		    					FBHP[IWELL][IT] = FB;
							FirstRead = false;
							}
						else if (D[IT] == DTIME)		// 是否相同的时间步不同的井数据
		    				{
		    					WELLNAME[IWELL] = WELL;
		    					OILRATE[IWELL][IT] = ORA;
		    					WATERRATE[IWELL][IT] = WR;
		    					GASRATE[IWELL][IT] = GR;
		    					GOR[IWELL][IT] = G;
		    					WOR[IWELL][IT] = WO;
		    					WCUT[IWELL][IT] = WC;
		    					FBHP[IWELL][IT] = FB;
							}
						else    // 下一个时间步第一口井数据
		    				{	
							IT = IT + 1;
		    					D[IT] = DTIME;
		    					WELLNAME[IWELL] = WELL;
		    					OILRATE[IWELL][IT] = ORA;
		    					WATERRATE[IWELL][IT] = WR;
		    					GASRATE[IWELL][IT] = GR;
		    					GOR[IWELL][IT] = G;
		    					WOR[IWELL][IT] = WO;
		    					WCUT[IWELL][IT] = WC;
		    					FBHP[IWELL][IT] = FB;
							}
				  }
				  pos = pos2;
		    }  
		    infile.close();  

			// 文件输出
			ofstream outfile;
			outfile.open ("plot.dat",ios::trunc);

				int MXWN = 0,MXTN = 0;
				for (int i = 1; i <= MAXTN;i++)
					{
						if (D[i] == 0.0)
							break;
						else
							MXTN++;
				}
				
				for (int i = 1;i <= MAXWN;i++)
					{
						if (WELLNAME[i] == "")
							break;
						else
							MXWN++;
				}
					
				for (int I = 1;I <= MXWN;I++)
				{
					outfile << WELLNAME[I] << "\n";
					outfile << setw(6) << "Time" << setw(15) << "OILRATE"<<setw(15)<<"WATERRATE"<<setw(15)<<"GASRATE"<<setw(15)<<"GOR"
						<<setw(15)<<"WOR"<<setw(15)<<"WCUT"<<setw(15)<<"FBHP\n";
					for (int J = 1;J <= MXTN;J++)
						{
							outfile << setw(6) << D[J] << setw(15) <<OILRATE[I][J]<< setw(15) <<WATERRATE[I][J]<< setw(15) <<GASRATE[I][J]
								<< setw(15) <<GOR[I][J]<< setw(15) <<WOR[I][J]<< setw(15) <<WCUT[I][J]<< setw(15) <<FBHP[I][J] << "\n";
					}
				}
				
				// plot curve with dislin
				double *x = new double[MXTN];
				double *y = new double[MXWN];
				int ic;

				for (int i = 0;i < MXTN; i++)
					x[i] = D[i+1];
				for (int i = 0;i < MXTN; i++)
					y[i] = OILRATE[1][i+1];

				Dislin g;

				g.metafl ("cons");
				g.scrmod ("revers");
				g.disini ();
				g.pagera ();
				g.complx ();
				g.axspos (450, 1800);
				g.axslen (2200, 1200);

				g.name   ("Time", "x");
				g.name   ("OilRate", "y");

				g.labdig (-1, "x");
				g.ticks  (9, "x");
				g.ticks  (10, "y");

				g.titlin ("OilRate of Well 1", 1);

				ic=g.intrgb (0.95,0.95,0.95);
				g.axsbgd (ic);

				//g.graf   (0.0, 360.0, 0.0, 90.0, -1.0, 1.0, -1.0, 0.5);
				g.setrgb (0.7, 0.7, 0.7);
				g.grid   (1, 1);

				g.color  ("fore");
				g.height (50);
				g.title  ();

				g.color  ("red");
				g.curve  (x, y, MXTN);
				g.disfin ();

				for (int i = 0;i< MXTN ;i++)
					cout << x[i] << "   "<<y[i]<< "\n";
				cin >> ic;

				delete []x;
				delete []y;

		    return 0;  
		} 
}
