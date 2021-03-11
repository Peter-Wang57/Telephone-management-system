#include <stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <queue>
#include <deque>
#include <map>
using namespace std;
typedef long long LL;
const int maxn = 110;

FILE *fp_user_init, *fp_ad_init, *fp_package_init, *fp_call, *fp_data, *fp_package, *fp_oversea;//���ļ��洢���� 
map<string, int> Ump, Admp;//���û�/����ԱIDӳ��Ϊ�����±� 
char Op[3][6] = {"�ƶ�", "��ͨ", "����"}; 
struct Users//�û��ṹ�� 
{
	char ID[11], Name[20], Phone_num[15], Password[20], Email[30], Gender[5];
	char User_type[10];//ѧ�� ��ʦ ���� ���� 
	int Package_ID, Pk_mon, Pk_day;//�ײ�����빺��ʱ�� 
	double Money;//�û���� 
	int Operator;// 0->�ƶ�  1->��ͨ  2->���� 
	
	int Call, Oversea; double Data;//��������ʣ����
	int C_sum, O_sum; double D_sum;//��������ʹ���� 
	
	void Package_clear(){//����û��ײ� 
		Package_ID = 0;
		Pk_mon = Pk_day = 0;
		Call = Oversea = 0;
		Data = 0.0;
	}
	
}U[10];
int User_cnt;//�û����� 
int us;//��ǰ���������û��±� 

struct Administrators//����Ա�ṹ�� 
{
	char ID[11], Name[20], Password[20], Email[30];
	int Op_belong;//����Ա������Ӫ��   0->�ƶ�  1->��ͨ  2->���� 
}Ad[3];

struct Package//�ײͽṹ�� 
{
	char Name[20], Special_offer[100];
	char Package_type[10];// �ײ�����û�����  ѧ�� ��ʦ ���� ���� 
	int Price, Call, Data, Oversea, Operator;
	
	void Print(){//����ײ���Ϣ 
		printf("�ײ����ƣ�%s\n", Name);
		printf("�ײͼ۸�%dԪ/��\n", Price);
		printf("�ײ���Ӫ�̣�%s\n", Op[Operator]);
		printf("�ײ��Ż���Ϣ��%s\n", Special_offer);
		printf("�ײ����ͣ�%s\n", Package_type);
		printf("����ͨ��ʱ����%d����\n", Call);
		printf("���ʳ�;ͨ��ʱ����%d����\n", Oversea);
		printf("��������%dG\n", Data);
		printf("****************************************\n");
	}
	
}P[20], psort[20];//Ϊ���ı��ײ�ԭ�±� ��psort[]Ϊ�ײ�����ṹ������ 
int Package_cnt;//�ײ����� 

struct Time_block//ʱ��ֶνṹ�� 
{
	int st, ed;//��ʼ/����ʱ�� 
	int Call, Oversea;
	double Data;
}Tb[20];


void Tb_init(){//ʱ��ֶγ�ʼ�� 
	for(int i=0; i<12; i++){
		Tb[i].st = i * 2;
		Tb[i].ed = i * 2 + 1;
		Tb[i].Call = Tb[i].Oversea = 0;
		Tb[i].Data = 0.0;
	}
}
int Check(char a[], char b[]){//�ж��Ƿ�Ϊͬһ���ַ��� 
	int len1 = strlen(a), len2 = strlen(b);
	if(len1 != len2) return 0;
	else{
		for(int i=0; i<len1; i++){
			if(a[i] != b[i]) return 0;
			else continue;
		}
	}
	return 1;
}
void User_login(){//�û���¼ 
	char id[11]; memset(id, 0, sizeof(id)); 
	us = 0;
	printf("����������ID��������#���˳���");
	while(scanf("%s", id) == 1){
		if(id[0] == '#') {
			printf("�����˳��û���¼��\n"); return;
		}else if(Ump[id] == 0) {//����Ump��������û�IDȷ���û��±� 
			printf("�Բ��𣬴��û������ڡ�\n����������ȷ��ID��������#���˳���");
		}else{
			us = Ump[id]; break;//��¼��ǰ�����û�Ϊus 
		}
	}
	printf("�������������룺");
	char pswd[20];
	while(scanf("%s", pswd) == 1){
		if(pswd[0] == '#') {
			printf("�����˳��û���¼��\n"); return;
		} 
		if(Check(pswd, U[us].Password) == 0){
			printf("�Բ��������������\n��������ȷ�����룬������#���˳���"); 
		}else{
			printf("��¼�ɹ���\n"); break; 
		}
	}
}
void Query_name(){//�ײͲ�ѯ���������ײ�����ѯ 
	char nm[20]; memset(nm, 0, sizeof(nm));
	int pk = 0;
	printf("�����Ը����ײ�����ѯ��������#�˳���\n�������ײ�����");
	while(scanf("%s", nm) == 1){
		if(nm[0] == '#') break;
		pk = 0;
		for(int i=1; i<=Package_cnt; i++){//�������е��ײ� 
			if(Check(nm, P[i].Name)){
				pk = i; break;
			}
		}
		if(!pk){
			printf("�Բ��𣬴��ײͲ����ڣ����ٴ������ײ�����������#�˳���");
		}else{
			printf("****************************************\n");
			P[pk].Print();
			printf("����Ϊ�ײ�%s�ľ������ݡ����ɼ��������ײ�����ѯ��������#�˳���", nm); 
		}
	}
}
void Package_print(int x){
	printf("������%s��Ӫ�̵������ײ���Ϣ��\n", Op[x]);
	printf("****************************************\n");
	for(int i=1; i<=Package_cnt; i++){
		if(x == P[i].Operator){
			P[i].Print();
		}
	}
}
void Query_operation(){//�ײͲ�ѯ����������Ӫ�̲�ѯ 
	char op[20];
	printf("�����Ը�����Ӫ�̲�ѯ��\n0.�ƶ��ײͣ�1.��ͨ�ײͣ�2.�����ײͣ�#.�˳���\n��������Ӫ�̴��룺");
	while(scanf("%s", &op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '0'){
			Package_print(0);
		} else if(op[0] == '1'){
			Package_print(1);
		} else if(op[0] == '2'){
			Package_print(2);
		} else {
			printf("�Բ������������ʽ����\n");
		}
		printf("�����Ը�����Ӫ�̲�ѯ��\n0.�ƶ��ײͣ�1.��ͨ�ײͣ�2.�����ײͣ�#.�˳���\n��������Ӫ�̴��룺");
	} 
}
bool cmp_price(const Package &A, const Package &B){//�۸�Ƚ������� 
	return A.Price < B.Price;//�������� 
}
void Sort_price(){
	printf("�����������ײͰ��ռ۸���������Ľ����\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i] = P[i];//��P[]�ײ���Ϣ��ֵ��psort[] 
	}
	sort(psort+1, psort+1+Package_cnt, cmp_price);
	printf("****************************************\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i].Print();
	}
}
bool cmp_data(const Package &A, const Package &B){
	return A.Data > B.Data;
}
void Sort_data(){
	printf("�����������ײͰ���������������Ľ����\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i] = P[i];
	}
	sort(psort+1, psort+1+Package_cnt, cmp_data);
	printf("****************************************\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i].Print();
	}
}
bool cmp_call(const Package &A, const Package &B){
	return A.Call > B.Call;
}
bool cmp_oversea(const Package &A, const Package &B){
	return A.Oversea > B.Oversea;
}
void Sort_call(){
	printf("�����������ײͰ��չ���ͨ��ʱ�併������Ľ����\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i] = P[i];
	}
	sort(psort+1, psort+1+Package_cnt, cmp_call);
	printf("****************************************\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i].Print();
	}
	
	printf("�����������ײͰ��չ��ʳ�;ͨ��ʱ�併������Ľ����\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i] = P[i];
	}
	sort(psort+1, psort+1+Package_cnt, cmp_oversea);
	printf("****************************************\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i].Print();
	}
} 
double Trans(char s[]){//�������ַ���ת��Ϊ�������� 
	int len = strlen(s);
	double x = 0;
	for(int i=0; i<len; i++){
		if(s[i]>='0' && s[i]<='9'){
			x *= 10.0;
			x += 1.0*(s[i] - '0');
		} 
	}
	return x;
}
int Due_tim(){//�ж��û������ײͺ��ײ��Ƿ��� 
	if(!U[us].Package_ID) return 0;
	time_t now1;
	struct tm *tm_now1;
	time(&now1);
	tm_now1 = localtime(&now1) ;
	int year1 = tm_now1->tm_year+1900;
	int mon1  = tm_now1->tm_mon+1;
	int day1  = tm_now1->tm_mday; 
	int hour1 = tm_now1->tm_hour; 
	int min1  = tm_now1->tm_min; 
	int sec1  = tm_now1->tm_sec;
	if(mon1 - U[us].Pk_mon > 1) {
		U[us].Package_clear();
		return 0;
	} else if(mon1 - U[us].Pk_mon == 1){
		if(day1 - U[us].Pk_day > 0){
			U[us].Package_clear();
			return 0;
		} else return 1;
	} else return 1;
}
void Pay_package(int i){//�ײͲ�ѯ���������ײ� 
	if(Due_tim()){
		printf("�Բ�������ǰʹ�õ��ײ�δ���ڣ�\n��ǰ�ײ���%d��%d�չ������ڹ����һ���µ��ڣ����ڵ�ǰ�ײ͵��ں��ٹ����µ��ײ͡�\n", U[us].Pk_mon, U[us].Pk_day);
	} else {
		if(U[us].Money < (double)P[i].Price){
			printf("�Բ��������˺����㣬��ǰ���%.2lf���������ֵ��20/50/100/200����������#�˳���\n", U[us].Money);
			char mon[6];
			while(scanf("%s", mon) == 1){
				if(mon[0] == '#') return;
				else {
					double Mon = Trans(mon);
					U[us].Money += Mon;
					if(U[us].Money < (double)P[i].Price){
						printf("�Բ��������˺������㣬��ǰ���%.2lf���������ֵ��20/50/100/200����������#�˳���\n", U[us].Money);
					} else {
						U[us].Money -= (double)P[i].Price;
						U[us].Package_ID = i;
						U[us].Call = P[i].Call;
						U[us].Oversea = P[i].Oversea;
						U[us].Data = 1.0*P[i].Data*1024;
						printf("���ѳɹ������ײ�%s����ǰ�û����%.2lf\n", P[i].Name, U[us].Money);
						time_t now1;//��¼�����ײ͵�ʱ�� 
						struct tm *tm_now1;
						time(&now1);
						tm_now1 = localtime(&now1) ;
						int year1 = tm_now1->tm_year+1900;
						int mon1  = tm_now1->tm_mon+1;
						int day1  = tm_now1->tm_mday; 
						int hour1 = tm_now1->tm_hour; 
						int min1  = tm_now1->tm_min; 
						int sec1  = tm_now1->tm_sec;
						U[us].Pk_mon = mon1; 
						U[us].Pk_day = day1;
						fprintf(fp_package, "%s %s %d %d-%d-%d %d:%d:%d\n", U[us].ID, P[i].Name, P[i].Operator, year1, mon1, day1, hour1, min1, sec1);
						break;
					}
				}
			}
		}
	}
}
void Query_Package(){//�ײͲ�ѯ 
	printf("�𾴵��û�%s����ӭ�����ײͲ�ѯ��\n", U[us].ID);
	char op[20]; memset(op, 0, sizeof(op));
	do{
		printf("��ѡ���ѯ��׼��\n1.�����ײ�����ѯ��\n2.������Ӫ������ѯ��\n");
		printf("3.�����ײͼ۸�����\n4.�����ײ���������\n");
		printf("5.�����ײͻ���ʱ������\n#.�˳��ײͲ�ѯ��\n���������Ĳ�����");
		scanf("%s", &op);
		if(op[0] == '#') break;
		else if(op[0]=='1'){
			Query_name();
		} else if(op[0]=='2'){
			Query_operation();
		} else if(op[0]=='3'){
			Sort_price();
		} else if(op[0]=='4'){
			Sort_data();
		} else if(op[0]=='5'){
			Sort_call();
		} else{
			printf("�Բ������������ʽ����\n");
		}
	}while(1);
	
	//�����û����ͽ����ײ��Ƽ� 
	printf("���������%s������Ϊ���Ƽ������ײͣ�\n", U[us].User_type);
	printf("****************************************\n");
	for(int i=1; i<=Package_cnt; i++){//Ϊ�û��Ƽ���������Ӫ���������û�����һ�µ��ײ� 
		if((U[us].Operator == P[i].Operator) && (Check(U[us].User_type, P[i].Package_type) == 1)){
			P[i].Print();
		}
	}
	//�����ײ� 
	printf("�����빺���ײͣ��������ײ����ƣ�������#�˳��ײ͹���\n"); 
	char nm[20];
	memset(nm, 0, sizeof(nm));
	while(scanf("%s", nm) == 1){
		bool flag = 0;
		if(nm[0] == '#') break;
		else {
			for(int i=1; i<=Package_cnt; i++){
				if((U[us].Operator == P[i].Operator) && (Check(nm, P[i].Name) == 1)){
					Pay_package(i);
					flag = 1;
					break;
				}
			}
			if(!flag){
				printf("�Բ��𣬴��ײ���������Ӫ���²����ڣ���������ȷ���ײ����ƣ�������#�˳��ײ͹���\n");
			} else break; 
		}
	}
	printf("�����˳��ײͲ�ѯ��\n");
}
int isphnm(char phnm[20]){//�ж��û�������Ƿ�����ȷ��11λ�绰����
 	int len = strlen(phnm);
 	int j = 0;
 	for(int i=0; i<len; i++){
        if(phnm[i]<=57&&phnm[i]>=48) j++;
    }
    if(len == 11 && j==len) return 1;
    else return 0;
}
void Pay_fee(double mon){//֧������ 
	if(mon <= U[us].Money){
		U[us].Money -= mon;
		printf("���ı��������Ѵ��û�����п۳�����ǰ���%.2lfԪ��\n", U[us].Money);
	} else {
		U[us].Money -= mon;
		printf("���ı��������Ѵ��û�����п۳�����ǰǷ��%.2lfԪ��\n", -1.0*U[us].Money);
		printf("����Ƿ�ѽ����󣬽�Ӱ�������û��������������ֵ��20/50/100/200����������#�˳���");
		char num[10];
		while(scanf("%s", num) == 1){
			if(num[0] == '#') break;
			else {
				double Num = Trans(num);
				U[us].Money += Num;
				printf("��ֵ�ɹ�����ǰ���%.2lfԪ��������ֵ�������20/50/100/200�����˳�������#��", U[us].Money); 
			}
		} 
	}
}
void Call(){//�û�����绰��������ͨ�� 
	char ph[20], End;
	memset(ph, 0, sizeof(ph)); 
	printf("��ӭ��ʹ�ù���ͨ������������Ҫ����ĵ绰���룬������#�˳���");
	while(scanf("%s", ph) == 1){
		if(ph[0] == '#'){
			printf("���ѽ�������ͨ����\n"); return;
		} else if(isphnm(ph)){ 
			break;
		} else {
			printf("�Բ���������ĺ��벻���ڣ���ȷ��Ϊ11λ���ֲ��������룬������#�˳���"); 
		}
	} 
	time_t Call_start, Call_end;//��¼��ʼ/����ʱ�� 
	time_t now1, now2;
	struct tm *tm_now1, *tm_now2;
	printf("����ɹ�������ͨѶ�У���������ǰͨ��������#��");
	Call_start = time(NULL);
	time(&now1);
	tm_now1 = localtime(&now1) ;
	int year1 = tm_now1->tm_year+1900;
	int mon1  = tm_now1->tm_mon+1;
	int day1  = tm_now1->tm_mday; 
	int hour1 = tm_now1->tm_hour; 
	int min1  = tm_now1->tm_min; 
	int sec1  = tm_now1->tm_sec;
	
	while(scanf("%c", &End) == 1){
		if(End == '#'){
			Call_end = time(NULL);
			printf("��ǰͨ���ѽ�����\n"); 
			break;
		}
	}
	
	time(&now2) ;
	tm_now2 = localtime(&now2) ;
	int year2 = tm_now2->tm_year+1900;
	int mon2  = tm_now2->tm_mon+1;
	int day2  = tm_now2->tm_mday; 
	int hour2 = tm_now2->tm_hour; 
	int min2  = tm_now2->tm_min; 
	int sec2  = tm_now2->tm_sec;
	
	//����ͨ��0.06Ԫ/���� 
	int Call_time = Call_end - Call_start;//ͨ��ʱ��/�� 
	int Real_time = Call_time/60;//ͨ��ʱ��/���� 
	if(Call_time%60) Real_time++;
	int Now_real = 0;//���ײ�ʱ�䲻�㣬��Ҫ����ɷѵ�ͨ��ʱ��/���� 
	U[us].C_sum += Real_time;
	if(U[us].Call >= Real_time){
		printf("���ã�����ͨ��%d���ӣ��Ѵ������ײ�ͨ��ʱ���п۳�����л����ʹ�á�\n", Real_time); 
		U[us].Call -= Real_time;
	} else {
		Now_real = Real_time - U[us].Call;
		U[us].Call = 0;
		printf("���ã�����ͨ��%d���ӣ������ײ�ʣ��ͨ��ʱ�䲻�㣬����ɷ�%.2lfԪ��\n", Real_time, Now_real*0.06);
		Pay_fee(Now_real * 0.06);
	}
	
	printf("��������%s �Է�����%s\n%d-%d-%d %d:%d:%d ~ %d-%d-%d %d:%d:%d\n����ͨ�� ͨ��ʱ��%d����\n���λ���%.2lfԪ\n", U[us].Phone_num, ph, year1,mon1,day1,hour1,min1,sec1,year2,mon2,day2,hour2,min2,sec2,Real_time,Now_real*0.06);
	//�û���  �������� �Է�����  ��ʼʱ�� ����ʱ�� ͨ��ʱ�� ͨ������
	fprintf(fp_call, "%s %s %s %d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d %d���� %.2lfԪ\n", U[us].ID, U[us].Phone_num, ph, year1,mon1,day1,hour1,min1,sec1,year2,mon2,day2,hour2,min2,sec2,Real_time,Now_real*0.06);
	
}
void Oversea(){//�û���绰�������ʳ�; 
	char ph[20], End;
	memset(ph, 0, sizeof(ph));
	printf("��ӭ��ʹ�ù��ʳ�;����������Ҫ����ĵ绰���룬������#�˳���");
	while(scanf("%s", ph) == 1){
		if(ph[0] == '#'){
			printf("���ѽ������ʳ�;��\n"); return;
		} else if(isphnm(ph)){ 
			break;
		} else {
			printf("�Բ���������ĺ��벻���ڣ���ȷ��Ϊ11λ���ֲ��������룬������#�˳���"); 
		}
	} 
	time_t Call_start, Call_end;
	time_t now1, now2;
	struct tm *tm_now1, *tm_now2;
	printf("����ɹ�������ͨѶ�У���������ǰͨ��������#��");
	Call_start = time(NULL);
	time(&now1);
	tm_now1 = localtime(&now1) ;
	int year1 = tm_now1->tm_year+1900;
	int mon1  = tm_now1->tm_mon+1;
	int day1  = tm_now1->tm_mday; 
	int hour1 = tm_now1->tm_hour; 
	int min1  = tm_now1->tm_min; 
	int sec1  = tm_now1->tm_sec;
	
	while(scanf("%c", &End) == 1){
		if(End == '#'){
			Call_end = time(NULL);
			printf("��ǰͨ���ѽ�����\n"); 
			break;
		}
	}
	
	time(&now2) ;
	tm_now2 = localtime(&now2) ;
	int year2 = tm_now2->tm_year+1900;
	int mon2  = tm_now2->tm_mon+1;
	int day2  = tm_now2->tm_mday; 
	int hour2 = tm_now2->tm_hour; 
	int min2  = tm_now2->tm_min; 
	int sec2  = tm_now2->tm_sec;
	
	//���ʳ�;1Ԫ/���� 
	int Call_time = Call_end - Call_start;//ͨ��ʱ��/�� 
	int Real_time = Call_time/60;//ͨ��ʱ��/���� 
	int Now_real = 0;//���ײ�ʱ�䲻�㣬��Ҫ��ɷѵ�ͨ��ʱ��/���� 
	if(Call_time%60) Real_time++;
	U[us].O_sum += Real_time;
	if(U[us].Oversea >= Real_time){
		printf("���ã�����ͨ��%d���ӣ��Ѵ������ײ�ͨ��ʱ���п۳�����л����ʹ�á�\n", Real_time); 
		U[us].Oversea -= Real_time;
	} else {
		Now_real = Real_time - U[us].Oversea;
		U[us].Oversea = 0;
		printf("���ã�����ͨ��%d���ӣ������ײ�ʣ��ͨ��ʱ�䲻�㣬����ɷ�%.2lfԪ��\n", Real_time, 1.0 * Now_real);
		Pay_fee(Now_real * 1);
	}
	
	printf("��������%s �Է�����%s\n%d-%d-%d %d:%d:%d ~ %d-%d-%d %d:%d:%d\n���ʳ�; ͨ��ʱ��%d����\n���λ���%.2lfԪ\n", U[us].Phone_num, ph, year1,mon1,day1,hour1,min1,sec1,year2,mon2,day2,hour2,min2,sec2,Real_time,1.0*Now_real);
	//�û���  �������� �Է�����  ��ʼʱ�� ����ʱ�� ͨ��ʱ�� ͨ������
	fprintf(fp_oversea, "%s %s %s %d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d %d���� %.2lfԪ\n", U[us].ID, U[us].Phone_num, ph, year1,mon1,day1,hour1,min1,sec1,year2,mon2,day2,hour2,min2,sec2,Real_time,Now_real*1.0);
	
}
void Data(){//�û���绰��������ʹ�� 
	char End;
	printf("��ӭ����������ʹ�ã�����ʹ��������1���˳�������#��");
	char op[20];
	memset(op, 0, sizeof(op));
	while(scanf("%s", op) == 1){
		if(op[0] == '#'){
			printf("�����˳�����ʹ�á�\n");
			return;
		}else if(op[0] != '1'){
			printf("�Բ������������ʽ��������ʹ��������1���˳�������#��");
		}else break;
	} 
	time_t Call_start, Call_end;
	time_t now1, now2;
	struct tm *tm_now1, *tm_now2;
	printf("����ʹ���ѿ�ʼ������ʹ��������#��"); 
	
	Call_start = time(NULL);
	time(&now1);
	tm_now1 = localtime(&now1) ;
	int year1 = tm_now1->tm_year+1900;
	int mon1  = tm_now1->tm_mon+1;
	int day1  = tm_now1->tm_mday; 
	int hour1 = tm_now1->tm_hour; 
	int min1  = tm_now1->tm_min; 
	int sec1  = tm_now1->tm_sec;
	
	while(scanf("%c", &End) == 1){
		if(End == '#'){
			Call_end = time(NULL);
			printf("����ʹ���ѽ�����\n");
			break;
		}
	}
	
	time(&now2) ;
	tm_now2 = localtime(&now2) ;
	int year2 = tm_now2->tm_year+1900;
	int mon2  = tm_now2->tm_mon+1;
	int day2  = tm_now2->tm_mday; 
	int hour2 = tm_now2->tm_hour; 
	int min2  = tm_now2->tm_min; 
	int sec2  = tm_now2->tm_sec;

	//10MB/min    0.3yuan/MB 
	
	int Data_time = Call_end - Call_start;//ʱ��/�� 
	int Real_time = Data_time/60;//ʱ��/���� 
	if(Data_time%60) Real_time++;
	double Data_MB = 1.0*Real_time*10;//ʹ������/MB 
	double Data_dif = 0.0, Mon_dif = 0.0;//����ɷѵ�����/���� 
	U[us].D_sum += Data_MB;
	if(Data_MB <= U[us].Data){
		U[us].Data -= Data_MB;
		printf("����ʹ������%.2lfMB���Ѵ������ײ����������п۳�����ǰ��������%.2lfMB����л����ʹ�á�\n", Data_MB, U[us].Data); 
	} else {
		Data_dif = Data_MB - U[us].Data;
		U[us].Data = 0;
		Mon_dif = Data_dif * 0.3;
		printf("����ʹ������%.2lfMB�������ײ������������㣬����ɷ�%.2lfԪ��\n", Data_MB, Mon_dif);
		Pay_fee(Mon_dif);
	}
	
	printf("�û���%s\n%d-%d-%d %d:%d:%d ~ %d-%d-%d %d:%d:%d\n����ʹ��ʱ��%d���� ���η���%.2lf\n", U[us].ID, year1,mon1,day1,hour1,min1,sec1,year2,mon2,day2,hour2,min2,sec2,Real_time,Mon_dif);
	
	fprintf(fp_data, "%s %d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d %d���� %.2lfMB %.2lfԪ\n", U[us].ID, year1,mon1,day1,hour1,min1,sec1,year2,mon2,day2,hour2,min2,sec2,Real_time, Data_MB, Mon_dif );
}
map<string, int> Ow_max;//�������û����Ƿ�Ѷ�� 
int Owe_money(){//�ж��û��Ƿ񳬹����Ƿ�Ѷ�� 
	Ow_max["ѧ��"] = 20;
	Ow_max["��ʦ"] = 10;
	Ow_max["����"] = 5;
	Ow_max["����"] = 10; 
	if(U[us].Money >= 0) return 0;
	else if(-U[us].Money <= 1.0*Ow_max[U[us].User_type]) return 0;
	else return 1;
}
void Simulate_call(){//�û���绰 
	char op[10];
	printf("�𾴵��û�%s����ӭ�������û���绰��\n", U[us].ID);
	
	while(Owe_money()){//���û�Ƿ���ҳ�������ȣ����ܽ���ͨ�� 
		printf("����ǰ�û���Ƿ��%.2lfԪ���ѳ����������Ƿ�Ѷ��%.2lfԪ���������г�ֵ��Ӱ����������ʹ�á�\n", -1.0*U[us].Money, 1.0*Ow_max[U[us].User_type]);
		printf("���ѳ�ֵ������1���˳�������#��");
		scanf("%s", op);
		if(op[0] == '#'){
			printf("�Բ�������Ƿ���޷���������ҵ�����˳��û���绰��\n");
			return;
		} else if(op[0] == '1'){
			printf("�������ֵ��������#�˳���");
			char Mon[10];
			U[us].Money += Trans(Mon);
		}
	}
	
	printf("1.����ͨ����\n2.���ʳ�;��\n3.����ʹ�ã�\n#.�˳��û���绰��\n���������Ĳ�����");
	
	while(scanf("%s", op) == 1){
		if(op[0] == '#'){
			printf("�����˳��û���绰����л����ʹ�á�\n");
			break;
		} else if(op[0] == '1'){
			while(Owe_money()){//ÿ�ζ�Ҫ��Ƿ�ѽ����ж� 
				printf("����ǰ�û���Ƿ��%.2lfԪ���ѳ����������Ƿ�Ѷ��%.2lfԪ���������г�ֵ��Ӱ����������ʹ�á�\n", -1.0*U[us].Money, 1.0*Ow_max[U[us].User_type]);
				printf("���ѳ�ֵ������1���˳�������#��");
				scanf("%s", op);
				if(op[0] == '#'){
					printf("�Բ�������Ƿ���޷���������ҵ�����˳��û���绰��\n");
					return;
				} else if(op[0] == '1'){
					printf("�������ֵ��������#�˳���");
					char Mon[10];
					U[us].Money += Trans(Mon);
				}
			}
			int tmp = Due_tim(); 
			Call();
		} else if(op[0] == '2'){
			while(Owe_money()){
				printf("����ǰ�û���Ƿ��%.2lfԪ���ѳ����������Ƿ�Ѷ��%.2lfԪ���������г�ֵ��Ӱ����������ʹ�á�\n", -1.0*U[us].Money, 1.0*Ow_max[U[us].User_type]);
				printf("���ѳ�ֵ������1���˳�������#��");
				scanf("%s", op);
				if(op[0] == '#'){
					printf("�Բ�������Ƿ���޷���������ҵ�����˳��û���绰��\n");
					return;
				} else if(op[0] == '1'){
					printf("�������ֵ��������#�˳���");
					char Mon[10];
					U[us].Money += Trans(Mon);
				}
			}
			int tmp = Due_tim();
			Oversea();
		} else if(op[0] == '3'){
			while(Owe_money()){
				printf("����ǰ�û���Ƿ��%.2lfԪ���ѳ����������Ƿ�Ѷ��%.2lfԪ���������г�ֵ��Ӱ����������ʹ�á�\n", -1.0*U[us].Money, 1.0*Ow_max[U[us].User_type]);
				printf("���ѳ�ֵ������1���˳�������#��");
				scanf("%s", op);
				if(op[0] == '#'){
					printf("�Բ�������Ƿ���޷���������ҵ�����˳��û���绰��\n");
					return;
				} else if(op[0] == '1'){
					printf("�������ֵ��������#�˳���");
					char Mon[10];
					U[us].Money += Trans(Mon);
				}
			}
			int tmp = Due_tim();
			Data();
		} else {
			printf("�Բ�������������Ч��\n");
		}
		printf("1.����ͨ����\n2.���ʳ�;��\n3.����ʹ�ã�\n#.�˳��û���绰��\n���������Ĳ�����");
	} 
	printf("�����˳��û�����绰��\n"); 
}
void Change_us_phone(){//������Ϣ�������޸ĵ绰 
	char ph[15]; 
	printf("���������ľ��ֻ����룬������#�˳������޸ģ�");
	while(scanf("%s", ph) == 1){//�жϾ��ֻ�����ȷ 
		if(ph[0] == '#') return; 
		if(Check(ph, U[us].Phone_num) == 0){
			printf("�Բ���������ľ��ֻ����벻��ȷ�����ٴ����룬������#�˳������޸ģ�"); 
		} else {
			printf("�������������ֻ����룬ע���ֻ�������Ϊ11λ���֣�");
			break;
		}
	}
	while(scanf("%s", ph) == 1){
		int len = strlen(ph);
		bool flag = 1;
		if(len == 11){
			for(int i=0; i<11; i++){
				if(ph[i]>='0' && ph[i]<='9') continue;
				else {
					flag = 0; break;
				}
			}
		} else flag = 0;
		if(!flag){
			printf("�Բ����������ֻ����벻����Ҫ�����������룺"); 
		} else {
			printf("�޸ĳɹ��������ֻ�������Ϊ��%s\n", ph);
			memcpy(U[us].Phone_num, ph, sizeof(ph));
			break;
		}
	}
}
void Change_us_pswd(){//������Ϣ�������޸����� 
	char pw[20]; memset(pw, 0, sizeof(pw));
	printf("���������ľ����룬������#�˳������޸ģ�");
	while(scanf("%s", pw) == 1){//�жϾ�������ȷ 
		if(pw[0] == '#') return; 
		if(Check(pw, U[us].Password) == 0){
			printf("�Բ���������ľ����벻��ȷ�����ٴ����룬������#�˳������޸ģ�"); 
		} else {
			printf("���������������룬ע����������Ϊ6λ��");
			break;
		}
	}
	while(scanf("%s", pw) == 1){//����������Ϊ6λ 
		int len = strlen(pw);
		int flag = 1;
		if(len < 6) flag = 0;
		if(!flag){
			printf("�Բ������������벻����Ҫ�����������룺"); 
		} else {
			printf("�޸ĳɹ�������������Ϊ��%s\n", pw);
			memcpy(U[us].Password, pw, sizeof(pw));
			break;
		}
	}
}
int is_email(char s[]){//�ж��Ƿ�Ϊ�Ϸ�����
	//�Ϸ������ʽΪ ***@***.*** ��*��ӦΪ0~9���ֻ��Сд��ĸ�� 
	int len = strlen(s);
	int j1 = 0, j2 = 0;// @ and .
	for(int i=0; i<len; i++){
		if(s[i] == '@') j1 = i;
		if(s[i] == '.') j2 = i;
	}
	if(j1 > j2) return 0;//@��.�ĺ��� 
	if(j2-j1 <= 1) return 0;//@��.���� 
	if(j1 == 0 || j2 == 0 || j1 == len-1 || j2 == len-1) return 0;//@��.����ǰ/��� 
	for(int i=0; i<len; i++){
		if(s[i]>='0' && s[i]<='9') continue;
		else if(s[i]>='a' && s[i]<='z') continue;
		else if(s[i]>='A' && s[i]<='Z') continue;
		else {
			if(i == j1 || i == j2) continue;
			else return 0;//*��Ϊ�����ַ� 
		}
	}
	return 1;
}
void Change_us_email(){//������Ϣ�������޸����� 
	char Em[30]; 
	memset(Em, 0, sizeof(Em));
	printf("���������ľ����䣬������#�˳������޸ģ�");
	while(scanf("%s", Em) == 1){
		if(Em[0] == '#') return; 
		if(Check(Em, U[us].Email) == 0){
			printf("�Բ���������ľ����䲻��ȷ�����ٴ����룬������#�˳������޸ģ�"); 
		} else {
			printf("���������������䣬ע�������ʽΪ:***@***.***��");
			break;
		}
	}
	while(scanf("%s", Em) == 1){
		if(is_email(Em) == 0){//����������***@***.*** 
			printf("�Բ������������䲻����Ҫ�����������룺"); 
		} else {
			printf("�޸ĳɹ�������������Ϊ��%s\n", Em);
			memcpy(U[us].Email, Em, sizeof(Em));
			break;
		}
	}
}
void User_information_manage(){//������Ϣ���� 
	printf("�𾴵��û�%s����ӭ�����������Ϣ����\n", U[us].ID);
	char op[20]; memset(op, 0, sizeof(op)); 
	do{
		printf("��ѡ����Ҫ���ĵ���Ϣ��\n1.�޸ĵ绰��\n2.�޸����룬\n3.�޸����䣬\n#.�˳�������Ϣ����\n���������Ĳ�����");
		scanf("%s", op);
		if(op[0] == '#') break;
		else if(op[0] == '1'){
			Change_us_phone();
		} else if(op[0] == '2'){
			Change_us_pswd();
		} else if(op[0] == '3'){
			Change_us_email();
		} else {
			printf("�Բ������������ʽ����\n");
		}
	}while(op[0] != '#');
	printf("�����˳�������Ϣ����\n");
}
void Query_us_detail(){//��ѯ�����嵥����ͨ���嵥��������ϸ 
	printf("��ӭ������ͨ���嵥��������ϸ��ѯ������Ϊ��ѯ�������ݣ�\n");
	rewind(fp_call);//�����ļ�ָ��ָ���ʼ 
	rewind(fp_oversea);
	rewind(fp_data);
	printf("����ͨ����\n");
	char id[11], ph1[15], ph2[15];
	int year1, mon1, day1, hour1, min1, sec1,
		year2, mon2, day2, hour2, min2, sec2;
	int tim;
	double Fee;
	while(fscanf(fp_call, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d����%lfԪ", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		if(Check(id, U[us].ID)){
			printf("ͨ��ʱ�䣺%d-%d-%d %d:%d:%d ~ %d-%d-%d %d:%d:%d, %d����\n", year1, mon1, day1, hour1, min1, sec1, year2, mon2, day2, hour2, min2, sec2, tim);
			printf("�������룺%s\n", ph1);
			printf("�Է����룺%s\n", ph2);	
			printf("���λ��ѣ�%.2lfԪ\n", Fee);
		}
	}printf("\n");
	printf("���ʳ�;��\n");
	while(fscanf(fp_oversea, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d����%lfԪ", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		if(Check(id, U[us].ID)){
			printf("ͨ��ʱ�䣺%d-%d-%d %d:%d:%d ~ %d-%d-%d %d:%d:%d, %d����\n", year1, mon1, day1, hour1, min1, sec1, year2, mon2, day2, hour2, min2, sec2, tim);
			printf("�������룺%s\n", ph1);
			printf("�Է����룺%s\n", ph2);	
			printf("���λ��ѣ�%.2lfԪ\n", Fee);
		}
	}printf("\n");
	double Data_MB;
	printf("������ϸ��\n");
	while(fscanf(fp_data, "%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d����%lfMB%lfԪ", id, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Data_MB, &Fee) != EOF){
		if(Check(id, U[us].ID)){
			printf("ʹ��ʱ�䣺%d-%d-%d %d:%d:%d ~ %d-%d-%d %d:%d:%d, %d����\n", year1, mon1, day1, hour1, min1, sec1, year2, mon2, day2, hour2, min2, sec2, tim);
			printf("����������%.2lfMB\n", Data_MB);
			printf("���η��ã�%.2lfԪ\n", Fee);
		}
	}printf("\n");
	printf("����Ϊ����ͨ����������ϸ��\n");
	fseek(fp_call, 0, SEEK_END);
	fseek(fp_oversea, 0, SEEK_END);
	fseek(fp_data, 0, SEEK_END);//�����ļ�ָ��ָ���ļ���� 
}//12312312312
int Calc_tim(int h1, int m1, int s1, int h2, int m2, int s2){//��������ʱ��Ĳ�/���ӣ�Ĭ��ʱ���ᳬ��24Сʱ�� 
	h1%=24; h2%=24;
	int x = 0;
	if(h1 == h2){
		x = m2 - m1;
		if(s1 < s2) x++;
		return x;
	} 
	else {
		while((h1+1)%24 != h2){
			x += 60;
			h1 = (h1+1) % 24;
		}
		x += (60-m1+m2);
		if(s1 < s2) x++;
		return x;
	}
}
void Statistic_us(){//��ѯ�����嵥����ʱ�����ͳ�� 
	printf("��ӭ������ʱ�����ͳ�Ʋ�ѯ��\n");
	rewind(fp_call);//�ļ�ָ��ָ��ͷ 
	rewind(fp_oversea);
	rewind(fp_data);
	Tb_init();
	char id[11], ph1[15], ph2[15];
	int year1, mon1, day1, hour1, min1, sec1,
		year2, mon2, day2, hour2, min2, sec2;
	int tim;
	double Fee;
	while(fscanf(fp_call, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d����%lfԪ", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		if(Check(id, U[us].ID)){
			int Bl1 = hour1/2, Bl2 = hour2/2;
			if(Bl1 == Bl2){
				Tb[Bl1].Call += tim;
			} else {
				Tb[Bl1].Call += Calc_tim(hour1, min1, sec1, Tb[Bl1].ed+1, 0, 0);
				while((Bl1+1)%12 != Bl2){
					Bl1++;
					Bl1 %= 12;
					Tb[Bl1].Call += 60;
				}
				Bl1++; Bl1%=12;
				Tb[Bl1].Call += Calc_tim(Tb[Bl1].st, 0, 0, hour2, min2, sec2);
			}
		}
	}
	
	while(fscanf(fp_oversea, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d����%lfԪ", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		if(Check(id, U[us].ID)){
			int Bl1 = hour1/2, Bl2 = hour2/2;
			if(Bl1 == Bl2){
				Tb[Bl1].Oversea += tim;
			} else {
				Tb[Bl1].Oversea += Calc_tim(hour1, min1, sec1, Tb[Bl1].ed+1, 0, 0);
				while(Bl1 != Bl2){
					Bl1++;
					Bl1 %= 12;
					Tb[Bl1].Oversea += 60;
				}
				Tb[Bl1].Oversea += Calc_tim(Tb[Bl1].st, 0, 0, hour2, min2, sec2);
			}
		}
	}
	
	double Data_MB;
	while(fscanf(fp_data, "%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d����%lfMB%lfԪ", id, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Data_MB, &Fee) != EOF){
		if(Check(id, U[us].ID)){
			int Bl1 = hour1/2, Bl2 = hour2/2;
			if(Bl1 == Bl2){
				Tb[Bl1].Data += tim;
			} else {
				Tb[Bl1].Data += Calc_tim(hour1, min1, sec1, Tb[Bl1].ed+1, 0, 0);
				while(Bl1 != Bl2){
					Bl1++;
					Bl1 %= 12;
					Tb[Bl1].Data += 60;
				}
				Tb[Bl1].Data += Calc_tim(Tb[Bl1].st, 0, 0, hour2, min2, sec2);
			}
		}
	}
	fseek(fp_call, 0, SEEK_END);
	fseek(fp_oversea, 0, SEEK_END);
	fseek(fp_data, 0, SEEK_END);//�ļ�ָ��ָ���β 
	 
	for(int i=0; i<12; i++){
		printf("%d:00 ~ %d:00\n", Tb[i].st, Tb[i].ed+1);
		printf("����ͨ����%d���ӣ� ���ʳ�;��%d���ӣ� ����ʹ�ã�%.2lfMB\n", Tb[i].Call, Tb[i].Oversea, 10.0*Tb[i].Data);
	} 
}
void Query_tel_charge(){//�����嵥��ѯ 
	printf("�𾴵��û�%s����ӭ�����뻰���嵥��ѯ��\n", U[us].ID);
	char op[20]; memset(op, 0, sizeof(op)); 
	printf("1.��ѯ�û�ͨ���嵥������ʹ����ϸ��\n2.��ѯʱ�����ͳ��ͨ����������\n#.�˳������嵥��ѯ\n���������Ĳ�����");
	while(scanf("%s", op) == 1){
		if(op[0] == '#'){
			break;
		} else if(op[0] == '1'){
			Query_us_detail();
		} else if(op[0] == '2'){
			Statistic_us();
		} else {
			printf("�Բ������������ʽ����\n");
		}
		printf("1.��ѯ�û�ͨ���嵥������ʹ����ϸ��\n2.��ѯʱ�����ͳ��ͨ����������\n#.�˳������嵥��ѯ\n���������Ĳ�����");
	} 
	printf("�����˳������嵥��ѯ��\n"); 
}
void User_operation(){//�û����� 
	User_login();//�û���¼��ȷ���±�us 
	if(us == 0) return;//��½ʧ�� 
	
	char op[20]; memset(op, 0, sizeof(op));
	//op->1 ��ѯ�ײ�  op->2 ģ���绰  
	//op->3 ������Ϣ����  op->4 ��ѯ�����嵥 
	printf("�𾴵��û�%s����ѡ���û�������\n", U[us].ID);
	printf("1.��ѯ�ײͣ�\n2.����绰��ʹ��������\n3.���������Ϣ��\n4.��ѯ�����嵥��\n#.�˳��û�������\n���������Ĳ�����"); 
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1') Query_Package();
		else if(op[0] == '2') Simulate_call();
		else if(op[0] == '3') User_information_manage();
		else if(op[0] == '4') Query_tel_charge();
		else{
			printf("�Բ������������ʽ����\n");
		}
		printf("1.��ѯ�ײͣ�\n2.����绰��ʹ��������\n3.���������Ϣ��\n4.��ѯ�����嵥�룬\n#.�˳��û�������\n���������Ĳ�����"); 
	}
	printf("�𾴵��û�%s�������˳��û�������\n", U[us].ID); 
	return;
}

//=======================================================================================================================================================
int ad, opbl;//��ǰ�����Ĺ���Ա�±�  ����Ա������Ӫ�� 
void Ad_login(){//����Ա��¼ 
	char id[11];
	ad = 0;
	printf("���������Ĺ���ԱID��������#���˳���");
	while(scanf("%s", id) == 1){
		if(id[0] == '#') {
			printf("�����˳�����Ա��¼��\n"); return;
		}else if(Admp[id] == 0) {
			printf("�Բ��𣬴˹���Ա�����ڡ�\n����������ȷ��ID��������#���˳���");
		}else{
			ad = Admp[id]; break;
		}
	}
	printf("�������������룺");
	char pswd[20];
	while(scanf("%s", pswd) == 1){
		if(pswd[0] == '#') {
			printf("�����˳�����Ա��¼��\n"); return;
		} 
		if(Check(pswd, Ad[ad].Password) == 0){
			printf("�Բ��������������\n��������ȷ�����룬������#���˳���"); 
		}else{
			printf("��¼�ɹ���\n"); break; 
		}
	} 
}
void Query_ad_package(){//���Ѳ�ѯͳ�ƹ������û��ײ������ѯ 
	printf("���ѽ����û��ײ������ѯ��\n"); 
	rewind(fp_package);//�ļ�ָ��ָ��ʼ 
	char id[15], Nm[20];
	memset(id, 0, sizeof(id));
	memset(Nm, 0, sizeof(Nm));
	int year1, mon1, day1, h1, m1, s1, now_op;
	while(fscanf(fp_package, "%s%s%d%d-%d-%d %d:%d:%d", id, Nm, &now_op, &year1, &mon1, &day1, &h1, &m1, &s1) != EOF){
		if(now_op == opbl){
			printf("�û�%s�� %d-%d-%d %d:%d:%d �����ײ�%s\n", id, year1, mon1, day1, h1, m1, s1, Nm);
		}
	}
	fseek(fp_package, 0, SEEK_END);//�ļ�ָ��ָ���β 
}
void Sta_call(){//���Ѳ�ѯͳ�ƹ�����ͳ���û�ͨ����Ϣ����������ͨ��ʱ�� 
	printf("����Ϊ�����͵绰ͨ����ʱ����\n");
	int Call_time = 0, Oversea_time = 0;
	rewind(fp_call);//�ļ�ָ��ָ��ʼ 
	rewind(fp_oversea);
	
	char id[11], ph1[15], ph2[15];
	memset(id, 0, sizeof(id));
	memset(ph1, 0, sizeof(ph1));
	memset(ph2, 0, sizeof(ph2)); 
	int year1, mon1, day1, hour1, min1, sec1,
		year2, mon2, day2, hour2, min2, sec2;
	int tim;
	double Fee;
	while(fscanf(fp_call, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d����%lfԪ", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		int now_us = Ump[id];
		if(U[now_us].Operator == opbl){//ͳ�����ڸ���Ӫ�̵��û���ͨ��ʱ�� 
			Call_time += tim;
		}
	}
	
	while(fscanf(fp_oversea, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d����%lfԪ", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		int now_us = Ump[id];
		if(U[now_us].Operator == opbl){
			Oversea_time += tim;
		}
	}
	
	printf("����ͨ����ʱ����%d����\n", Call_time);
	printf("���ʳ�;��ʱ����%d����\n", Oversea_time);
	
	fseek(fp_call, 0, SEEK_END);
	fseek(fp_oversea, 0, SEEK_END);//�ļ�ָ��ָ��ĩβ 
}
bool cmp_tot(const Time_block &A, const Time_block &B){//����ͨ����ʱ�������� 
	if(A.Data+A.Oversea == B.Data+B.Oversea){
		return A.st < B.st;
	} else return (A.Data+A.Oversea > B.Data+B.Oversea);
	
}
bool cmp_blk_call(const Time_block &A, const Time_block &B){//��������ͨ��ʱ�������� 
	if(A.Call == B.Call) return A.st < B.st;
	else return A.Call > B.Call;
}
bool cmp_blk_oversea(const Time_block &A, const Time_block &B){//�������ʳ�;ʱ�������� 
	if(A.Oversea == B.Oversea) return A.st < B.st;
	else return A.Oversea > B.Oversea;
}
void Sort_blk(){//���Ѳ�ѯͳ�ƹ�����ͳ���û�ͨ����Ϣ������ʱ��ͨ��ҵ�������� 
	printf("����Ϊ����ʱ��ͨ��ҵ����������\n");
	
	rewind(fp_call);
	rewind(fp_oversea);//�ļ�ָ��ָ��ʼ 
	
	Tb_init();
	int now_us;
	char id[11], ph1[15], ph2[15];
	int year1, mon1, day1, hour1, min1, sec1,
		year2, mon2, day2, hour2, min2, sec2;
	int tim;
	double Fee;
	while(fscanf(fp_call, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d����%lfԪ", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		now_us = Ump[id];//��Umpȷ���û��±� 
		if(U[now_us].Operator == opbl){
			int Bl1 = hour1/2, Bl2 = hour2/2;
			if(Bl1 == Bl2){
				Tb[Bl1].Call += tim;
			} else {
				Tb[Bl1].Call += Calc_tim(hour1, min1, sec1, Tb[Bl1].ed+1, 0, 0);
				while((Bl1+1)%12 != Bl2){
					Bl1++;
					Bl1 %= 12;
					Tb[Bl1].Call += 60;
				}
				Bl1++; Bl1%=12;
				Tb[Bl1].Call += Calc_tim(Tb[Bl1].st, 0, 0, hour2, min2, sec2);
			}
		}
	}
	
	while(fscanf(fp_oversea, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d����%lfԪ", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		now_us = Ump[id];
		if(U[now_us].Operator == opbl){
			int Bl1 = hour1/2, Bl2 = hour2/2;
			if(Bl1 == Bl2){
				Tb[Bl1].Oversea += tim;
			} else {
				Tb[Bl1].Oversea += Calc_tim(hour1, min1, sec1, Tb[Bl1].ed+1, 0, 0);
				while(Bl1 != Bl2){
					Bl1++;
					Bl1 %= 12;
					Tb[Bl1].Oversea += 60;
				}
				Tb[Bl1].Oversea += Calc_tim(Tb[Bl1].st, 0, 0, hour2, min2, sec2);
			}
		}
	}
	
	sort(Tb, Tb+12, cmp_tot);
	printf("����ҵ����������Ϊ��\n");
	for(int i=0; i<12; i++){
		printf("%d:00 ~ %d:00   %d����\n", Tb[i].st, Tb[i].ed+1, Tb[i].Call+Tb[i].Oversea);
	}
	
	sort(Tb, Tb+12, cmp_blk_call);
	printf("������ͨ��������Ϊ��\n");
	for(int i=0; i<12; i++){
		printf("%d:00 ~ %d:00   %d����\n", Tb[i].st, Tb[i].ed+1, Tb[i].Call);
	}
	
	sort(Tb, Tb+12, cmp_blk_oversea);
	printf("�����ʳ�;������Ϊ��\n");
	for(int i=0; i<12; i++){
		printf("%d:00 ~ %d:00   %d����\n", Tb[i].st, Tb[i].ed+1, Tb[i].Oversea);
	}
	
	fseek(fp_call, 0, SEEK_END);
	fseek(fp_oversea, 0, SEEK_END);//�ļ�ָ��ָ��ĩβ 
}

struct Simple_user//����Ϊ�û�����Ľṹ�� �൱���û��ṹ��ļ� 
{
	char ID[15];
	int Call, Oversea;
	double Data;
}Su[10];
bool cmp_su_call(const Simple_user &A, const Simple_user &B){//�û�ͨ��ʱ�������� 
	return A.Call > B.Call;
}
bool cmp_su_oversea(const Simple_user &A, const Simple_user &B){//�û����ʳ�;������ 
	return A.Oversea > B.Oversea;
}
bool cmp_su_data(const Simple_user &A, const Simple_user &B){//�û�����ʹ�������� 
	return A.Data > B.Data;
}
void Sort_user(){//���Ѳ�ѯͳ�ƹ�����ͳ���û�ͨ����Ϣ�������û�ͨ��ʱ���������������� 
	printf("����Ϊ���û�ͨ��������ʹ�õ�����\n");
	memset(Su, 0, sizeof(Su));
	int Su_cnt = 0;
	for(int i=1; i<=User_cnt; i++){
		if(U[i].Operator == opbl){//ѡȡ���ڸ���Ӫ�̵��û� 
			Su_cnt++;
			memcpy(Su[Su_cnt].ID, U[i].ID, sizeof(U[i].ID));
			Su[Su_cnt].Call = U[i].C_sum;
			Su[Su_cnt].Oversea = U[i].O_sum;
			Su[Su_cnt].Data = U[i].D_sum;
		}
	}
	sort(Su+1, Su+1+Su_cnt, cmp_su_call);
	printf("���û�����ͨ������\n");
	for(int i=1; i<=Su_cnt; i++){
		printf("%s  %d����\n", Su[i].ID, Su[i].Call);
	}
	sort(Su+1, Su+1+Su_cnt, cmp_su_oversea);
	printf("���û����ʳ�;����\n");
	for(int i=1; i<=Su_cnt; i++){
		printf("%s  %d����\n", Su[i].ID, Su[i].Oversea);
	}
	sort(Su+1, Su+1+Su_cnt, cmp_su_data);
	printf("���û�����ʹ������\n");
	for(int i=1; i<=Su_cnt; i++){
		printf("%s  %.2lfMB\n", Su[i].ID, Su[i].Data);
	}
}
void Statistic_detial(){//���Ѳ�ѯͳ�ƹ�����ͳ���û�ͨ����Ϣ
	printf("���ѽ����û�ͨ����Ϣͳ�ơ�\n");
	printf("1.ͳ�Ƹ�����ͨ��ʱ����\n2.�Ը���ʱ��ҵ��������\n3.���û�ͨ��������ʹ������\n#.�˳��û�ͨ����Ϣͳ�ơ�\n���������Ĳ�����");
	char op[20]; memset(op, 0, sizeof(op)); 
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1'){
			Sta_call();
		} else if(op[0] == '2'){
			Sort_blk();
		} else if(op[0] == '3'){
			Sort_user();
		} else {
			printf("�Բ������������ʽ����\n");
		}
		printf("1.ͳ�Ƹ�����ͨ��ʱ����\n2.�Ը���ʱ��ҵ��������\n3.���û�ͨ��������ʹ������\n#.�˳��û�ͨ����Ϣͳ�ơ�\n���������Ĳ�����");
	}
} 
void Query_statistic(){//���Ѳ�ѯͳ�ƹ���
	printf("%s����Ա%s����ӭ���뻰�Ѳ�ѯͳ�ƹ���\n", Op[opbl], Ad[ad].ID);
	char op[20];
	printf("1.�鿴�û��ײ������\n2.ͳ���û�ͨ����Ϣ��\n#.�˳����Ѳ�ѯͳ�ƹ���\n���������Ĳ�����");
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1'){
			Query_ad_package();
		} else if(op[0] == '2'){
			Statistic_detial();
		} else {
			printf("�Բ������������ʽ����\n");
		}
		printf("1.�鿴�û��ײ������\n2.ͳ���û�ͨ����Ϣ��\n#.�˳����Ѳ�ѯͳ�ƹ���\n���������Ĳ�����");
	}
	printf("�����˳����Ѳ�ѯͳ�ƹ���\n");
}
void Query_ad_pkg(){//�ײ͹������ײͲ�ѯ 
	printf("���ѽ����ײͲ�ѯ������Ϊ%s��Ӫ�������ײ����ݡ�\n", Op[opbl]);
	printf("****************************************\n");
	for(int i=1; i<=Package_cnt; i++){//���������ײ� 
		if(P[i].Operator == opbl){//��ѯ���ڸ���Ӫ�̵��ײ� 
			P[i].Print();
		}
	}
}
void Add_pkg(){//�ײ͹���������ײ� 
	printf("���ѽ����ײ���ӣ�����ײ�������1���˳�������#��");
	char op[20]; memset(op, 0, sizeof(op));
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1'){
			Package_cnt++; int k = Package_cnt;
			printf("�ײ����ƣ�"); scanf("%s", P[k].Name);
			printf("�ײͼ۸�(Ԫ/��)��"); scanf("%d", &P[k].Price);
			printf("�ײ��Ż���Ϣ��"); scanf("%s", P[k].Special_offer);
			printf("�ײ����ͣ�"); scanf("%s", P[k].Package_type);
			printf("����ͨ��ʱ��(����)��"); scanf("%d", &P[k].Call);
			printf("���ʳ�;ͨ��ʱ��(����)��"); scanf("%d", &P[k].Oversea);
			printf("������(G)��"); scanf("%d", &P[k].Data);
			P[k].Operator = opbl; 
			printf("�ײ���ӳɹ���\n");
		} else {
			printf("�Բ������������ʽ����\n");
		}
		printf("���ѽ����ײ���ӣ�����ײ�������1���˳�������#��");
	}
}
void Manage_package(){//�ײ͹��� 
	printf("%s����Ա%s����ӭ�����ײ͹���\n", Op[opbl], Ad[ad].ID);
	printf("1.��ѯ�ײͣ�\n2.����ײͣ�\n#.�˳��ײ͹���\n���������Ĳ�����");
	char op[20]; memset(op, 0, sizeof(op));
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1'){
			Query_ad_pkg();
		} else if(op[0] == '2'){
			Add_pkg();
		} else {
			printf("�Բ������������ʽ����\n");
		}
		printf("1.��ѯ�ײͣ�\n2.����ײͣ�\n#.�˳��ײ͹���\n���������Ĳ�����");
	} 
	printf("�����˳��ײ͹���\n");
}
void Change_ad_pswd(){//������Ϣ�������޸����� 
	char pw[20]; memset(pw, 0, sizeof(pw));
	printf("���������ľ����룬������#�˳������޸ģ�");
	while(scanf("%s", pw) == 1){//�жϾ������Ƿ���ȷ 
		if(pw[0] == '#') return; 
		if(Check(pw, Ad[ad].Password) == 0){
			printf("�Բ���������ľ����벻��ȷ�����ٴ����룬������#�˳������޸ģ�"); 
		} else {
			printf("���������������룬ע����������Ϊ6λ��");
			break;
		}
	}
	while(scanf("%s", pw) == 1){//����������Ϊ6λ 
		int len = strlen(pw);
		int flag = 1;
		if(len < 6) flag = 0;
 		if(!flag){
			printf("�Բ������������벻����Ҫ�����������룺"); 
		} else {
			printf("�޸ĳɹ�������������Ϊ��%s\n", pw);
			memcpy(Ad[ad].Password, pw, sizeof(pw));
			break;
		}
	}
}
void Change_ad_email(){//������Ϣ�������޸����� 
	char Em[30]; memset(Em, 0, sizeof(Em)); 
	printf("���������ľ����䣬������#�˳������޸ģ�");
	while(scanf("%s", Em) == 1){//������ȷ������ 
		if(Em[0] == '#') return; 
		if(Check(Em, Ad[ad].Email) == 0){
			printf("�Բ���������ľ����䲻��ȷ�����ٴ����룬������#�˳������޸ģ�"); 
		} else {
			printf("���������������䣬ע�������ʽΪ:***@***.***��");
			break;
		}
	}
	
	while(scanf("%s", Em) == 1){//���������� ������ ***@***.*** 
		if(is_email(Em) == 0){
			printf("�Բ������������䲻����Ҫ�����������룺"); 
		} else {
			printf("�޸ĳɹ�������������Ϊ��%s\n", Em);
			memcpy(Ad[ad].Email, Em, sizeof(Em));
			break;
		}
	}
}
void Ad_information_manage(){//����Ա������Ϣ���� 
	printf("%s����Ա%s����ӭ���������Ϣ����\n", Op[opbl], Ad[ad].ID);
	printf("1.�޸����룬\n2.�޸����䣬\n#.�˳�������Ϣ����\n���������Ĳ�����");
	char op[20];
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1') Change_ad_pswd();
		else if(op[0] == '2') Change_ad_email();
		else printf("�Բ������������ʽ����\n"); 
		printf("1.�޸����룬\n2.�޸����䣬\n#.�˳�������Ϣ����\n���������Ĳ�����");
	} 
	printf("�����˳�������Ϣ����\n");
}
void Ad_operation(){//����Ա���� 
	Ad_login(); //����Ա��¼��ȷ������Ա�±� 
	if(!ad) return;
	opbl = Ad[ad].Op_belong;//����Ա������Ӫ�� 
	char op[20]; memset(op, 0, sizeof(op)); 
	//op-># Exit  op->1 ���Ѳ�ѯͳ��  op->2 �ײ͹��� 
	//op->3 ������Ϣ����
	printf("%s����Ա%s����ѡ�����Ա������\n", Op[opbl], Ad[ad].ID);
	printf("1.���Ѳ�ѯͳ�ƣ�\n2.�ײ͹���\n3.������Ϣ����\n#.�˳�����Ա������\n���������Ĳ�����");
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1') Query_statistic();
		else if(op[0] == '2') Manage_package();
		else if(op[0] == '3') Ad_information_manage();
		else {
			printf("�Բ������������ʽ����\n");
		}
		printf("1.���Ѳ�ѯͳ�ƣ�\n2.�ײ͹���\n3.������Ϣ����\n#.�˳�����Ա������\n���������Ĳ�����");
	
	} 
	return;
}
//=====================================================================

void User_init(){//�û���Ϣ��ʼ�� 
	Ump.clear();
	memset(U, 0, sizeof(U));
	fscanf(fp_user_init, "%d", &User_cnt);
	for(int i=1; i<=User_cnt; i++){
		fscanf(fp_user_init, "%s%s%s%s%s%s%s%d%d%lf", U[i].ID, U[i].Name, U[i].Gender, U[i].Phone_num, U[i].Password, U[i].Email, U[i].User_type, &U[i].Operator, &U[i].Package_ID, &U[i].Money);
//		printf("%s\n%s\n%d\n", U[i].ID, U[i].Name, U[i].Operator);
		Ump[U[i].ID] = i;
		int Pk = U[i].Package_ID;
		if(Pk == 0){
			U[i].Call = U[i].Oversea = U[i].Data = 0;
		} else {
			U[i].Call = P[Pk].Call;
			U[i].Oversea = P[Pk].Oversea;
			U[i].Data = P[Pk].Data * 1024;
		}
	}
}
void Ad_init(){//����Ա��Ϣ��ʼ�� 
	Admp.clear();
	memset(Ad, 0, sizeof(Ad));
	for(int i=1; i<=3; i++){
		fscanf(fp_ad_init, "%s%d%s%s%s", Ad[i].ID, &Ad[i].Op_belong, Ad[i].Name, Ad[i].Email, Ad[i].Password);
		Admp[Ad[i].ID] = i;
	}
}
void Package_init(){//�ײ���Ϣ��ʼ�� 
	memset(P, 0, sizeof(P));
	fscanf(fp_package_init, "%d", &Package_cnt);
	for(int i=1; i<=Package_cnt; i++){
		fscanf(fp_package_init, "%s%d%d%s%s%d%d%d", P[i].Name, &P[i].Operator, &P[i].Price, P[i].Package_type, P[i].Special_offer, &P[i].Call, &P[i].Oversea, &P[i].Data);
//		printf("%s\n%d\n", P[i].Name, P[i].Price);
	}
}
void Init(){	
	Package_init();//�ײ���Ϣ��ʼ�� 
	User_init();//�û���Ϣ��ʼ�� 
	Ad_init();//����Ա��Ϣ��ʼ�� 
	Tb_init();
}
//=================================================================

int main(){
	srand((unsigned int)time(NULL));
	//���ļ� 
	fp_user_init = fopen("user_init.txt", "r");
	fp_ad_init = fopen("ad_init.txt", "r");
	fp_package_init = fopen("package_init.txt", "r");
	fp_call = fopen("call_record.txt", "w+");
	fp_data = fopen("data_record.txt", "w+");
	fp_package = fopen("package_record.txt", "w+");
	fp_oversea = fopen("oversea_record.txt", "w+");
	//����ļ� 
	if(fp_user_init == NULL){
		printf("�ܱ�Ǹ��user_init.txt��ʧ�ܣ������ļ��Ƿ���ڡ����򼴽��رա�\n");
		exit(1);
	} 
	if(fp_ad_init == NULL){
		printf("�ܱ�Ǹ��ad_init.txt��ʧ�ܣ������ļ��Ƿ���ڡ����򼴽��رա�\n");
		exit(1);
	} 
	if(fp_package_init == NULL){
		printf("�ܱ�Ǹ��package_init.txt��ʧ�ܣ������ļ��Ƿ���ڡ����򼴽��رա�\n");
		exit(1);
	} 
	if(fp_call == NULL){
		printf("�ܱ�Ǹ��call_record.txt��ʧ�ܣ������ļ��Ƿ���ڡ����򼴽��رա�\n");
		exit(1);
	} 
	if(fp_oversea == NULL){
		printf("�ܱ�Ǹ��oversea_record.txt��ʧ�ܣ������ļ��Ƿ���ڡ����򼴽��رա�\n");
		exit(1);
	} 
	if(fp_data == NULL){
		printf("�ܱ�Ǹ��data_record.txt��ʧ�ܣ������ļ��Ƿ���ڡ����򼴽��رա�\n");
		exit(1);
	} 
	if(fp_package == NULL){
		printf("�ܱ�Ǹ��package_record.txt��ʧ�ܣ������ļ��Ƿ���ڡ����򼴽��رա�\n");
		exit(1);
	} 
	Init();//��ʼ�� 
	printf("===========================================��ӭ��ʹ�õ绰��ͳ�ƹ���ϵͳ��===========================================\n");
	printf("1.�û�����\n2.����Ա����\n#.�˳�ϵͳ\n���������Ĳ�����"); 
	char op[20]; //  op->1 Users  op->2 Ad
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;//�˳����� 
		else if(op[0] == '1') User_operation();//�û����� 
		else if(op[0] == '2') Ad_operation();//����Ա���� 
		else {
			printf("�Բ������������ʽ����\n");
		}
		printf("1.�û�����\n2.����Ա����\n#.�˳�ϵͳ\n���������Ĳ�����"); 
	}
	printf("===========================================�绰��ͳ�ƹ���ϵͳ��л����ʹ�ã�=========================================\n");
	//�ر��ļ� 
	fclose(fp_user_init);
	fclose(fp_ad_init);
	fclose(fp_package_init);
	fclose(fp_call);
	fclose(fp_oversea);
	fclose(fp_data);
	fclose(fp_package);
	
	return 0;
}

