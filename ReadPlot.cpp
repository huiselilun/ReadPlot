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

		const int MAXTN = 501, MAXWN = 21;  // ���洢1000��ʱ�䲽��20�ھ�������
		double D[MAXTN];    // �洢Time�������ֵ
		string WELLNAME[MAXWN];   // �洢���������ʮ���ַ�
		double OILRATE[MAXWN][MAXTN],WATERRATE[MAXWN][MAXTN]; // ��ˮ����
		double GASRATE[MAXWN][MAXTN],GOR[MAXWN][MAXTN];   // �����������ͱ�
		double WOR[MAXWN][MAXTN];	// ��ˮ��
		double WCUT[MAXWN][MAXTN];	//��ˮ��
		double FBHP[MAXWN][MAXTN];    //����ѹ��
		bool FirstRead = true;
		
		for (int i = 1;i < MAXTN;i++)
			D[i] = 0.0;
		for (int i = 1;i < MAXWN;i++)
			WELLNAME[i] = "";

		if(infile.fail())//�ļ���ʧ��:����0  
		{  
		    return 0;  
		}  
		else//�ļ�����  
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
						infile.seekg(pos-pos2,ios::cur);
						infile >> c1 >> c2 >> DTIME;
						getline(infile,tmp,'\n');
						infile >> IWELL >> WELL >> a >> c3 >> c4 >> c5 >> c6
							>> ORA>>WR>>GR>>G>>WO>>WC>>FB;

						if (IT == 1 && FirstRead)// ����ǵ�һ��ʱ�䲽����
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
						else if (D[IT] == DTIME)		// �Ƿ���ͬ��ʱ�䲽��ͬ�ľ�����
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
						else    // ��һ��ʱ�䲽��һ�ھ�����
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

			// �ļ����
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
		    return 0;  
		} 
}