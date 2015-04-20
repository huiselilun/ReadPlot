#include <iostream>  
#include <fstream>  
#include <string>  
#include <cctype>
#include <iomanip>

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
				outfile.close();

				//out for MatLab
				ofstream outM;
				outM.open ("MatLabplot.dat",ios::trunc);

				outM << MXTN << "\n";
				outM << MXWN << "\n";
				for (int i = 1;i <= MXWN; i++)
					outM << WELLNAME[i] << "\n";
				outM.close();

				ofstream outM2;
				outM2.open("MatLabPlot2.dat",ios::trunc);
				for (int I = 1;I <= MXWN;I++)		//按照井顺序，然后按照时间顺序输出值
				{
					for (int J = 1;J <= MXTN;J++)
						{
							outM2 << D[J] << setw(15) <<OILRATE[I][J]<< setw(15) <<WATERRATE[I][J]<< setw(15) <<GASRATE[I][J]
								<< setw(15) <<GOR[I][J]<< setw(15) <<WOR[I][J]<< setw(15) <<WCUT[I][J]<< setw(15) <<FBHP[I][J] << "\n";
					}
				}
				outM2.close();

				
		    return 0;  
		} 
}
