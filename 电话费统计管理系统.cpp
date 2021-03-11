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

FILE *fp_user_init, *fp_ad_init, *fp_package_init, *fp_call, *fp_data, *fp_package, *fp_oversea;//用文件存储数据 
map<string, int> Ump, Admp;//将用户/管理员ID映射为整形下标 
char Op[3][6] = {"移动", "联通", "电信"}; 
struct Users//用户结构体 
{
	char ID[11], Name[20], Phone_num[15], Password[20], Email[30], Gender[5];
	char User_type[10];//学生 教师 商人 其他 
	int Package_ID, Pk_mon, Pk_day;//套餐序号与购买时间 
	double Money;//用户余额 
	int Operator;// 0->移动  1->联通  2->电信 
	
	int Call, Oversea; double Data;//话费流量剩余量
	int C_sum, O_sum; double D_sum;//话费流量使用量 
	
	void Package_clear(){//清空用户套餐 
		Package_ID = 0;
		Pk_mon = Pk_day = 0;
		Call = Oversea = 0;
		Data = 0.0;
	}
	
}U[10];
int User_cnt;//用户数量 
int us;//当前所操作的用户下标 

struct Administrators//管理员结构体 
{
	char ID[11], Name[20], Password[20], Email[30];
	int Op_belong;//管理员所属运营商   0->移动  1->联通  2->电信 
}Ad[3];

struct Package//套餐结构体 
{
	char Name[20], Special_offer[100];
	char Package_type[10];// 套餐针对用户类型  学生 教师 商人 其他 
	int Price, Call, Data, Oversea, Operator;
	
	void Print(){//输出套餐信息 
		printf("套餐名称：%s\n", Name);
		printf("套餐价格：%d元/月\n", Price);
		printf("套餐运营商：%s\n", Op[Operator]);
		printf("套餐优惠信息：%s\n", Special_offer);
		printf("套餐类型：%s\n", Package_type);
		printf("国内通话时长：%d分钟\n", Call);
		printf("国际长途通话时长：%d分钟\n", Oversea);
		printf("总流量：%dG\n", Data);
		printf("****************************************\n");
	}
	
}P[20], psort[20];//为不改变套餐原下标 设psort[]为套餐排序结构体数组 
int Package_cnt;//套餐数量 

struct Time_block//时间分段结构体 
{
	int st, ed;//开始/结束时间 
	int Call, Oversea;
	double Data;
}Tb[20];


void Tb_init(){//时间分段初始化 
	for(int i=0; i<12; i++){
		Tb[i].st = i * 2;
		Tb[i].ed = i * 2 + 1;
		Tb[i].Call = Tb[i].Oversea = 0;
		Tb[i].Data = 0.0;
	}
}
int Check(char a[], char b[]){//判断是否为同一个字符串 
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
void User_login(){//用户登录 
	char id[11]; memset(id, 0, sizeof(id)); 
	us = 0;
	printf("请输入您的ID，或输入#键退出：");
	while(scanf("%s", id) == 1){
		if(id[0] == '#') {
			printf("您已退出用户登录。\n"); return;
		}else if(Ump[id] == 0) {//利用Ump数组根据用户ID确定用户下标 
			printf("对不起，此用户不存在。\n请您输入正确的ID，或输入#键退出：");
		}else{
			us = Ump[id]; break;//记录当前操作用户为us 
		}
	}
	printf("请输入您的密码：");
	char pswd[20];
	while(scanf("%s", pswd) == 1){
		if(pswd[0] == '#') {
			printf("您已退出用户登录。\n"); return;
		} 
		if(Check(pswd, U[us].Password) == 0){
			printf("对不起，您的密码错误。\n请输入正确的密码，或输入#键退出："); 
		}else{
			printf("登录成功！\n"); break; 
		}
	}
}
void Query_name(){//套餐查询——根据套餐名查询 
	char nm[20]; memset(nm, 0, sizeof(nm));
	int pk = 0;
	printf("您可以根据套餐名查询，或输入#退出。\n请输入套餐名：");
	while(scanf("%s", nm) == 1){
		if(nm[0] == '#') break;
		pk = 0;
		for(int i=1; i<=Package_cnt; i++){//遍历所有的套餐 
			if(Check(nm, P[i].Name)){
				pk = i; break;
			}
		}
		if(!pk){
			printf("对不起，此套餐不存在，请再次输入套餐名，或输入#退出：");
		}else{
			printf("****************************************\n");
			P[pk].Print();
			printf("以上为套餐%s的具体内容。您可继续输入套餐名查询，或输入#退出：", nm); 
		}
	}
}
void Package_print(int x){
	printf("以下是%s运营商的所有套餐信息：\n", Op[x]);
	printf("****************************************\n");
	for(int i=1; i<=Package_cnt; i++){
		if(x == P[i].Operator){
			P[i].Print();
		}
	}
}
void Query_operation(){//套餐查询——根据运营商查询 
	char op[20];
	printf("您可以根据运营商查询。\n0.移动套餐，1.联通套餐，2.电信套餐，#.退出。\n请输入运营商代码：");
	while(scanf("%s", &op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '0'){
			Package_print(0);
		} else if(op[0] == '1'){
			Package_print(1);
		} else if(op[0] == '2'){
			Package_print(2);
		} else {
			printf("对不起，您的输入格式错误。\n");
		}
		printf("您可以根据运营商查询。\n0.移动套餐，1.联通套餐，2.电信套餐，#.退出。\n请输入运营商代码：");
	} 
}
bool cmp_price(const Package &A, const Package &B){//价格比较排序函数 
	return A.Price < B.Price;//升序排序 
}
void Sort_price(){
	printf("以下是所有套餐按照价格升序排序的结果：\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i] = P[i];//将P[]套餐信息赋值给psort[] 
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
	printf("以下是所有套餐按照流量降序排序的结果：\n");
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
	printf("以下是所有套餐按照国内通话时间降序排序的结果：\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i] = P[i];
	}
	sort(psort+1, psort+1+Package_cnt, cmp_call);
	printf("****************************************\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i].Print();
	}
	
	printf("以下是所有套餐按照国际长途通话时间降序排序的结果：\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i] = P[i];
	}
	sort(psort+1, psort+1+Package_cnt, cmp_oversea);
	printf("****************************************\n");
	for(int i=1; i<=Package_cnt; i++){
		psort[i].Print();
	}
} 
double Trans(char s[]){//将数字字符串转化为整形数字 
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
int Due_tim(){//判断用户有无套餐和套餐是否到期 
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
void Pay_package(int i){//套餐查询——购买套餐 
	if(Due_tim()){
		printf("对不起，您当前使用的套餐未到期，\n当前套餐于%d月%d日购买，且于购买后一个月到期，请于当前套餐到期后再购买新的套餐。\n", U[us].Pk_mon, U[us].Pk_day);
	} else {
		if(U[us].Money < (double)P[i].Price){
			printf("对不起，您的账号余额不足，当前余额%.2lf，请输入充值金额（20/50/100/200），或输入#退出：\n", U[us].Money);
			char mon[6];
			while(scanf("%s", mon) == 1){
				if(mon[0] == '#') return;
				else {
					double Mon = Trans(mon);
					U[us].Money += Mon;
					if(U[us].Money < (double)P[i].Price){
						printf("对不起，您的账号仍余额不足，当前余额%.2lf，请输入充值金额（20/50/100/200），或输入#退出：\n", U[us].Money);
					} else {
						U[us].Money -= (double)P[i].Price;
						U[us].Package_ID = i;
						U[us].Call = P[i].Call;
						U[us].Oversea = P[i].Oversea;
						U[us].Data = 1.0*P[i].Data*1024;
						printf("您已成功购买套餐%s，当前用户余额%.2lf\n", P[i].Name, U[us].Money);
						time_t now1;//记录购买套餐的时间 
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
void Query_Package(){//套餐查询 
	printf("尊敬的用户%s，欢迎进入套餐查询。\n", U[us].ID);
	char op[20]; memset(op, 0, sizeof(op));
	do{
		printf("请选择查询标准：\n1.根据套餐名查询，\n2.根据运营商名查询，\n");
		printf("3.根据套餐价格排序，\n4.根据套餐流量排序，\n");
		printf("5.根据套餐话费时长排序，\n#.退出套餐查询。\n请输入您的操作：");
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
			printf("对不起，您的输入格式错误。\n");
		}
	}while(1);
	
	//根据用户类型进行套餐推荐 
	printf("您的身份是%s，我们为您推荐以下套餐：\n", U[us].User_type);
	printf("****************************************\n");
	for(int i=1; i<=Package_cnt; i++){//为用户推荐其所属运营商下且与用户类型一致的套餐 
		if((U[us].Operator == P[i].Operator) && (Check(U[us].User_type, P[i].Package_type) == 1)){
			P[i].Print();
		}
	}
	//购买套餐 
	printf("若您想购买套餐，请输入套餐名称，或输入#退出套餐购买。\n"); 
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
				printf("对不起，此套餐在您的运营商下不存在，请输入正确的套餐名称，或输入#退出套餐购买。\n");
			} else break; 
		}
	}
	printf("您已退出套餐查询。\n");
}
int isphnm(char phnm[20]){//判断用户拨打的是否是正确的11位电话号码
 	int len = strlen(phnm);
 	int j = 0;
 	for(int i=0; i<len; i++){
        if(phnm[i]<=57&&phnm[i]>=48) j++;
    }
    if(len == 11 && j==len) return 1;
    else return 0;
}
void Pay_fee(double mon){//支付话费 
	if(mon <= U[us].Money){
		U[us].Money -= mon;
		printf("您的本次消费已从用户余额中扣除，当前余额%.2lf元。\n", U[us].Money);
	} else {
		U[us].Money -= mon;
		printf("您的本次消费已从用户余额中扣除，当前欠费%.2lf元。\n", -1.0*U[us].Money);
		printf("若您欠费金额过大，将影响您的用户操作，请输入充值金额（20/50/100/200），或输入#退出：");
		char num[10];
		while(scanf("%s", num) == 1){
			if(num[0] == '#') break;
			else {
				double Num = Trans(num);
				U[us].Money += Num;
				printf("充值成功，当前余额%.2lf元。继续充值请输入金额（20/50/100/200），退出请输入#：", U[us].Money); 
			}
		} 
	}
}
void Call(){//用户拨打电话——国内通话 
	char ph[20], End;
	memset(ph, 0, sizeof(ph)); 
	printf("欢迎您使用国内通话，请输入您要拨打的电话号码，或输入#退出：");
	while(scanf("%s", ph) == 1){
		if(ph[0] == '#'){
			printf("您已结束国内通话。\n"); return;
		} else if(isphnm(ph)){ 
			break;
		} else {
			printf("对不起，您输入的号码不存在，请确认为11位数字并重新输入，或输入#退出："); 
		}
	} 
	time_t Call_start, Call_end;//记录开始/结束时间 
	time_t now1, now2;
	struct tm *tm_now1, *tm_now2;
	printf("拨打成功，正在通讯中，若结束当前通话请输入#：");
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
			printf("当前通话已结束。\n"); 
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
	
	//国内通话0.06元/分钟 
	int Call_time = Call_end - Call_start;//通话时间/秒 
	int Real_time = Call_time/60;//通话时间/分钟 
	if(Call_time%60) Real_time++;
	int Now_real = 0;//若套餐时间不足，需要额外缴费的通话时间/分钟 
	U[us].C_sum += Real_time;
	if(U[us].Call >= Real_time){
		printf("您好，本次通话%d分钟，已从您的套餐通话时间中扣除，感谢您的使用。\n", Real_time); 
		U[us].Call -= Real_time;
	} else {
		Now_real = Real_time - U[us].Call;
		U[us].Call = 0;
		printf("您好，本次通话%d分钟，您的套餐剩余通话时间不足，另需缴费%.2lf元。\n", Real_time, Now_real*0.06);
		Pay_fee(Now_real * 0.06);
	}
	
	printf("本机号码%s 对方号码%s\n%d-%d-%d %d:%d:%d ~ %d-%d-%d %d:%d:%d\n国内通话 通话时长%d分钟\n当次话费%.2lf元\n", U[us].Phone_num, ph, year1,mon1,day1,hour1,min1,sec1,year2,mon2,day2,hour2,min2,sec2,Real_time,Now_real*0.06);
	//用户名  本机号码 对方号码  开始时间 结束时间 通话时长 通话费用
	fprintf(fp_call, "%s %s %s %d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d %d分钟 %.2lf元\n", U[us].ID, U[us].Phone_num, ph, year1,mon1,day1,hour1,min1,sec1,year2,mon2,day2,hour2,min2,sec2,Real_time,Now_real*0.06);
	
}
void Oversea(){//用户打电话——国际长途 
	char ph[20], End;
	memset(ph, 0, sizeof(ph));
	printf("欢迎您使用国际长途，请输入您要拨打的电话号码，或输入#退出：");
	while(scanf("%s", ph) == 1){
		if(ph[0] == '#'){
			printf("您已结束国际长途。\n"); return;
		} else if(isphnm(ph)){ 
			break;
		} else {
			printf("对不起，您输入的号码不存在，请确认为11位数字并重新输入，或输入#退出："); 
		}
	} 
	time_t Call_start, Call_end;
	time_t now1, now2;
	struct tm *tm_now1, *tm_now2;
	printf("拨打成功，正在通讯中，若结束当前通话请输入#：");
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
			printf("当前通话已结束。\n"); 
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
	
	//国际长途1元/分钟 
	int Call_time = Call_end - Call_start;//通话时间/秒 
	int Real_time = Call_time/60;//通话时间/分钟 
	int Now_real = 0;//若套餐时间不足，需要另缴费的通话时间/分钟 
	if(Call_time%60) Real_time++;
	U[us].O_sum += Real_time;
	if(U[us].Oversea >= Real_time){
		printf("您好，本次通话%d分钟，已从您的套餐通话时间中扣除，感谢您的使用。\n", Real_time); 
		U[us].Oversea -= Real_time;
	} else {
		Now_real = Real_time - U[us].Oversea;
		U[us].Oversea = 0;
		printf("您好，本次通话%d分钟，您的套餐剩余通话时间不足，另需缴费%.2lf元。\n", Real_time, 1.0 * Now_real);
		Pay_fee(Now_real * 1);
	}
	
	printf("本机号码%s 对方号码%s\n%d-%d-%d %d:%d:%d ~ %d-%d-%d %d:%d:%d\n国际长途 通话时长%d分钟\n当次话费%.2lf元\n", U[us].Phone_num, ph, year1,mon1,day1,hour1,min1,sec1,year2,mon2,day2,hour2,min2,sec2,Real_time,1.0*Now_real);
	//用户名  本机号码 对方号码  开始时间 结束时间 通话时长 通话费用
	fprintf(fp_oversea, "%s %s %s %d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d %d分钟 %.2lf元\n", U[us].ID, U[us].Phone_num, ph, year1,mon1,day1,hour1,min1,sec1,year2,mon2,day2,hour2,min2,sec2,Real_time,Now_real*1.0);
	
}
void Data(){//用户打电话——流量使用 
	char End;
	printf("欢迎您进入流量使用，流量使用请输入1，退出请输入#：");
	char op[20];
	memset(op, 0, sizeof(op));
	while(scanf("%s", op) == 1){
		if(op[0] == '#'){
			printf("您已退出流量使用。\n");
			return;
		}else if(op[0] != '1'){
			printf("对不起，您的输入格式错误。流量使用请输入1，退出请输入#：");
		}else break;
	} 
	time_t Call_start, Call_end;
	time_t now1, now2;
	struct tm *tm_now1, *tm_now2;
	printf("流量使用已开始，结束使用请输入#："); 
	
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
			printf("流量使用已结束。\n");
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
	
	int Data_time = Call_end - Call_start;//时间/秒 
	int Real_time = Data_time/60;//时间/分钟 
	if(Data_time%60) Real_time++;
	double Data_MB = 1.0*Real_time*10;//使用流量/MB 
	double Data_dif = 0.0, Mon_dif = 0.0;//另需缴费的流量/费用 
	U[us].D_sum += Data_MB;
	if(Data_MB <= U[us].Data){
		U[us].Data -= Data_MB;
		printf("本次使用流量%.2lfMB，已从您的套餐流量余量中扣除，当前流量余量%.2lfMB，感谢您的使用。\n", Data_MB, U[us].Data); 
	} else {
		Data_dif = Data_MB - U[us].Data;
		U[us].Data = 0;
		Mon_dif = Data_dif * 0.3;
		printf("本次使用流量%.2lfMB，您的套餐流量余量不足，需另缴费%.2lf元。\n", Data_MB, Mon_dif);
		Pay_fee(Mon_dif);
	}
	
	printf("用户名%s\n%d-%d-%d %d:%d:%d ~ %d-%d-%d %d:%d:%d\n流量使用时长%d分钟 当次费用%.2lf\n", U[us].ID, year1,mon1,day1,hour1,min1,sec1,year2,mon2,day2,hour2,min2,sec2,Real_time,Mon_dif);
	
	fprintf(fp_data, "%s %d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d %d分钟 %.2lfMB %.2lf元\n", U[us].ID, year1,mon1,day1,hour1,min1,sec1,year2,mon2,day2,hour2,min2,sec2,Real_time, Data_MB, Mon_dif );
}
map<string, int> Ow_max;//各类型用户最大欠费额度 
int Owe_money(){//判断用户是否超过最大欠费额度 
	Ow_max["学生"] = 20;
	Ow_max["教师"] = 10;
	Ow_max["商人"] = 5;
	Ow_max["其他"] = 10; 
	if(U[us].Money >= 0) return 0;
	else if(-U[us].Money <= 1.0*Ow_max[U[us].User_type]) return 0;
	else return 1;
}
void Simulate_call(){//用户打电话 
	char op[10];
	printf("尊敬的用户%s，欢迎您进入用户打电话。\n", U[us].ID);
	
	while(Owe_money()){//若用户欠费且超过最大额度，则不能进行通话 
		printf("您当前用户已欠费%.2lf元，已超过您的最大欠费额度%.2lf元，若不进行充值将影响您的正常使用。\n", -1.0*U[us].Money, 1.0*Ow_max[U[us].User_type]);
		printf("话费充值请输入1，退出请输入#：");
		scanf("%s", op);
		if(op[0] == '#'){
			printf("对不起，您因欠费无法进行正常业务，已退出用户打电话。\n");
			return;
		} else if(op[0] == '1'){
			printf("请输入充值金额，或输入#退出：");
			char Mon[10];
			U[us].Money += Trans(Mon);
		}
	}
	
	printf("1.国内通话，\n2.国际长途，\n3.流量使用，\n#.退出用户打电话。\n请输入您的操作：");
	
	while(scanf("%s", op) == 1){
		if(op[0] == '#'){
			printf("您已退出用户打电话，感谢您的使用。\n");
			break;
		} else if(op[0] == '1'){
			while(Owe_money()){//每次都要对欠费进行判断 
				printf("您当前用户已欠费%.2lf元，已超过您的最大欠费额度%.2lf元，若不进行充值将影响您的正常使用。\n", -1.0*U[us].Money, 1.0*Ow_max[U[us].User_type]);
				printf("话费充值请输入1，退出请输入#：");
				scanf("%s", op);
				if(op[0] == '#'){
					printf("对不起，您因欠费无法进行正常业务，已退出用户打电话。\n");
					return;
				} else if(op[0] == '1'){
					printf("请输入充值金额，或输入#退出：");
					char Mon[10];
					U[us].Money += Trans(Mon);
				}
			}
			int tmp = Due_tim(); 
			Call();
		} else if(op[0] == '2'){
			while(Owe_money()){
				printf("您当前用户已欠费%.2lf元，已超过您的最大欠费额度%.2lf元，若不进行充值将影响您的正常使用。\n", -1.0*U[us].Money, 1.0*Ow_max[U[us].User_type]);
				printf("话费充值请输入1，退出请输入#：");
				scanf("%s", op);
				if(op[0] == '#'){
					printf("对不起，您因欠费无法进行正常业务，已退出用户打电话。\n");
					return;
				} else if(op[0] == '1'){
					printf("请输入充值金额，或输入#退出：");
					char Mon[10];
					U[us].Money += Trans(Mon);
				}
			}
			int tmp = Due_tim();
			Oversea();
		} else if(op[0] == '3'){
			while(Owe_money()){
				printf("您当前用户已欠费%.2lf元，已超过您的最大欠费额度%.2lf元，若不进行充值将影响您的正常使用。\n", -1.0*U[us].Money, 1.0*Ow_max[U[us].User_type]);
				printf("话费充值请输入1，退出请输入#：");
				scanf("%s", op);
				if(op[0] == '#'){
					printf("对不起，您因欠费无法进行正常业务，已退出用户打电话。\n");
					return;
				} else if(op[0] == '1'){
					printf("请输入充值金额，或输入#退出：");
					char Mon[10];
					U[us].Money += Trans(Mon);
				}
			}
			int tmp = Due_tim();
			Data();
		} else {
			printf("对不起，您的输入无效。\n");
		}
		printf("1.国内通话，\n2.国际长途，\n3.流量使用，\n#.退出用户打电话。\n请输入您的操作：");
	} 
	printf("您已退出用户拨打电话。\n"); 
}
void Change_us_phone(){//个人信息管理——修改电话 
	char ph[15]; 
	printf("请输入您的旧手机号码，或输入#退出号码修改：");
	while(scanf("%s", ph) == 1){//判断旧手机号正确 
		if(ph[0] == '#') return; 
		if(Check(ph, U[us].Phone_num) == 0){
			printf("对不起，您输入的旧手机号码不正确，请再次输入，或输入#退出号码修改："); 
		} else {
			printf("请输入您的新手机号码，注意手机号码需为11位数字：");
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
			printf("对不起，您的新手机号码不符合要求，请重新输入："); 
		} else {
			printf("修改成功，您的手机号码已为：%s\n", ph);
			memcpy(U[us].Phone_num, ph, sizeof(ph));
			break;
		}
	}
}
void Change_us_pswd(){//个人信息管理——修改密码 
	char pw[20]; memset(pw, 0, sizeof(pw));
	printf("请输入您的旧密码，或输入#退出密码修改：");
	while(scanf("%s", pw) == 1){//判断旧密码正确 
		if(pw[0] == '#') return; 
		if(Check(pw, U[us].Password) == 0){
			printf("对不起，您输入的旧密码不正确，请再次输入，或输入#退出号码修改："); 
		} else {
			printf("请输入您的新密码，注意密码最少为6位：");
			break;
		}
	}
	while(scanf("%s", pw) == 1){//新密码至少为6位 
		int len = strlen(pw);
		int flag = 1;
		if(len < 6) flag = 0;
		if(!flag){
			printf("对不起，您的新密码不符合要求，请重新输入："); 
		} else {
			printf("修改成功，您的密码已为：%s\n", pw);
			memcpy(U[us].Password, pw, sizeof(pw));
			break;
		}
	}
}
int is_email(char s[]){//判断是否为合法邮箱
	//合法邮箱格式为 ***@***.*** （*处应为0~9数字或大小写字母） 
	int len = strlen(s);
	int j1 = 0, j2 = 0;// @ and .
	for(int i=0; i<len; i++){
		if(s[i] == '@') j1 = i;
		if(s[i] == '.') j2 = i;
	}
	if(j1 > j2) return 0;//@在.的后面 
	if(j2-j1 <= 1) return 0;//@与.紧邻 
	if(j1 == 0 || j2 == 0 || j1 == len-1 || j2 == len-1) return 0;//@或.在最前/最后 
	for(int i=0; i<len; i++){
		if(s[i]>='0' && s[i]<='9') continue;
		else if(s[i]>='a' && s[i]<='z') continue;
		else if(s[i]>='A' && s[i]<='Z') continue;
		else {
			if(i == j1 || i == j2) continue;
			else return 0;//*处为特殊字符 
		}
	}
	return 1;
}
void Change_us_email(){//个人信息管理——修改邮箱 
	char Em[30]; 
	memset(Em, 0, sizeof(Em));
	printf("请输入您的旧邮箱，或输入#退出号码修改：");
	while(scanf("%s", Em) == 1){
		if(Em[0] == '#') return; 
		if(Check(Em, U[us].Email) == 0){
			printf("对不起，您输入的旧邮箱不正确，请再次输入，或输入#退出邮箱修改："); 
		} else {
			printf("请输入您的新邮箱，注意邮箱格式为:***@***.***：");
			break;
		}
	}
	while(scanf("%s", Em) == 1){
		if(is_email(Em) == 0){//新邮箱满足***@***.*** 
			printf("对不起，您的新邮箱不符合要求，请重新输入："); 
		} else {
			printf("修改成功，您的邮箱已为：%s\n", Em);
			memcpy(U[us].Email, Em, sizeof(Em));
			break;
		}
	}
}
void User_information_manage(){//个人信息管理 
	printf("尊敬的用户%s，欢迎您进入个人信息管理。\n", U[us].ID);
	char op[20]; memset(op, 0, sizeof(op)); 
	do{
		printf("请选择需要更改的信息：\n1.修改电话，\n2.修改密码，\n3.修改邮箱，\n#.退出个人信息管理。\n请输入您的操作：");
		scanf("%s", op);
		if(op[0] == '#') break;
		else if(op[0] == '1'){
			Change_us_phone();
		} else if(op[0] == '2'){
			Change_us_pswd();
		} else if(op[0] == '3'){
			Change_us_email();
		} else {
			printf("对不起，您的输入格式错误。\n");
		}
	}while(op[0] != '#');
	printf("您已退出个人信息管理。\n");
}
void Query_us_detail(){//查询话费清单——通话清单及流量明细 
	printf("欢迎您进入通话清单及流量明细查询，以下为查询具体内容：\n");
	rewind(fp_call);//操作文件指针指向最开始 
	rewind(fp_oversea);
	rewind(fp_data);
	printf("国内通话：\n");
	char id[11], ph1[15], ph2[15];
	int year1, mon1, day1, hour1, min1, sec1,
		year2, mon2, day2, hour2, min2, sec2;
	int tim;
	double Fee;
	while(fscanf(fp_call, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d分钟%lf元", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		if(Check(id, U[us].ID)){
			printf("通话时间：%d-%d-%d %d:%d:%d ~ %d-%d-%d %d:%d:%d, %d分钟\n", year1, mon1, day1, hour1, min1, sec1, year2, mon2, day2, hour2, min2, sec2, tim);
			printf("本机号码：%s\n", ph1);
			printf("对方号码：%s\n", ph2);	
			printf("当次话费：%.2lf元\n", Fee);
		}
	}printf("\n");
	printf("国际长途：\n");
	while(fscanf(fp_oversea, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d分钟%lf元", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		if(Check(id, U[us].ID)){
			printf("通话时间：%d-%d-%d %d:%d:%d ~ %d-%d-%d %d:%d:%d, %d分钟\n", year1, mon1, day1, hour1, min1, sec1, year2, mon2, day2, hour2, min2, sec2, tim);
			printf("本机号码：%s\n", ph1);
			printf("对方号码：%s\n", ph2);	
			printf("当次话费：%.2lf元\n", Fee);
		}
	}printf("\n");
	double Data_MB;
	printf("流量明细：\n");
	while(fscanf(fp_data, "%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d分钟%lfMB%lf元", id, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Data_MB, &Fee) != EOF){
		if(Check(id, U[us].ID)){
			printf("使用时间：%d-%d-%d %d:%d:%d ~ %d-%d-%d %d:%d:%d, %d分钟\n", year1, mon1, day1, hour1, min1, sec1, year2, mon2, day2, hour2, min2, sec2, tim);
			printf("当次流量：%.2lfMB\n", Data_MB);
			printf("当次费用：%.2lf元\n", Fee);
		}
	}printf("\n");
	printf("以上为您的通话和流量明细。\n");
	fseek(fp_call, 0, SEEK_END);
	fseek(fp_oversea, 0, SEEK_END);
	fseek(fp_data, 0, SEEK_END);//操作文件指针指向文件最后 
}//12312312312
int Calc_tim(int h1, int m1, int s1, int h2, int m2, int s2){//计算两个时间的差/分钟（默认时间差不会超过24小时） 
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
void Statistic_us(){//查询话费清单——时间分区统计 
	printf("欢迎您进入时间分区统计查询。\n");
	rewind(fp_call);//文件指针指向开头 
	rewind(fp_oversea);
	rewind(fp_data);
	Tb_init();
	char id[11], ph1[15], ph2[15];
	int year1, mon1, day1, hour1, min1, sec1,
		year2, mon2, day2, hour2, min2, sec2;
	int tim;
	double Fee;
	while(fscanf(fp_call, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d分钟%lf元", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
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
	
	while(fscanf(fp_oversea, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d分钟%lf元", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
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
	while(fscanf(fp_data, "%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d分钟%lfMB%lf元", id, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Data_MB, &Fee) != EOF){
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
	fseek(fp_data, 0, SEEK_END);//文件指针指向结尾 
	 
	for(int i=0; i<12; i++){
		printf("%d:00 ~ %d:00\n", Tb[i].st, Tb[i].ed+1);
		printf("国内通话：%d分钟， 国际长途：%d分钟， 流量使用：%.2lfMB\n", Tb[i].Call, Tb[i].Oversea, 10.0*Tb[i].Data);
	} 
}
void Query_tel_charge(){//话费清单查询 
	printf("尊敬的用户%s，欢迎您进入话费清单查询。\n", U[us].ID);
	char op[20]; memset(op, 0, sizeof(op)); 
	printf("1.查询用户通话清单及流量使用明细，\n2.查询时间分区统计通话及流量，\n#.退出话费清单查询\n请输入您的操作：");
	while(scanf("%s", op) == 1){
		if(op[0] == '#'){
			break;
		} else if(op[0] == '1'){
			Query_us_detail();
		} else if(op[0] == '2'){
			Statistic_us();
		} else {
			printf("对不起，您的输入格式错误。\n");
		}
		printf("1.查询用户通话清单及流量使用明细，\n2.查询时间分区统计通话及流量，\n#.退出话费清单查询\n请输入您的操作：");
	} 
	printf("您已退出话费清单查询。\n"); 
}
void User_operation(){//用户操作 
	User_login();//用户登录并确定下标us 
	if(us == 0) return;//登陆失败 
	
	char op[20]; memset(op, 0, sizeof(op));
	//op->1 查询套餐  op->2 模拟打电话  
	//op->3 个人信息管理  op->4 查询话费清单 
	printf("尊敬的用户%s，请选择用户操作。\n", U[us].ID);
	printf("1.查询套餐，\n2.拨打电话及使用流量，\n3.管理个人信息，\n4.查询话费清单，\n#.退出用户操作。\n请输入您的操作："); 
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1') Query_Package();
		else if(op[0] == '2') Simulate_call();
		else if(op[0] == '3') User_information_manage();
		else if(op[0] == '4') Query_tel_charge();
		else{
			printf("对不起，您的输入格式错误。\n");
		}
		printf("1.查询套餐，\n2.拨打电话及使用流量，\n3.管理个人信息，\n4.查询话费清单请，\n#.退出用户操作。\n请输入您的操作："); 
	}
	printf("尊敬的用户%s，您已退出用户操作。\n", U[us].ID); 
	return;
}

//=======================================================================================================================================================
int ad, opbl;//当前操作的管理员下标  管理员所属运营商 
void Ad_login(){//管理员登录 
	char id[11];
	ad = 0;
	printf("请输入您的管理员ID，或输入#键退出：");
	while(scanf("%s", id) == 1){
		if(id[0] == '#') {
			printf("您已退出管理员登录。\n"); return;
		}else if(Admp[id] == 0) {
			printf("对不起，此管理员不存在。\n请您输入正确的ID，或输入#键退出：");
		}else{
			ad = Admp[id]; break;
		}
	}
	printf("请输入您的密码：");
	char pswd[20];
	while(scanf("%s", pswd) == 1){
		if(pswd[0] == '#') {
			printf("您已退出管理员登录。\n"); return;
		} 
		if(Check(pswd, Ad[ad].Password) == 0){
			printf("对不起，您的密码错误。\n请输入正确的密码，或输入#键退出："); 
		}else{
			printf("登录成功！\n"); break; 
		}
	} 
}
void Query_ad_package(){//话费查询统计管理——用户套餐情况查询 
	printf("您已进入用户套餐情况查询。\n"); 
	rewind(fp_package);//文件指针指向开始 
	char id[15], Nm[20];
	memset(id, 0, sizeof(id));
	memset(Nm, 0, sizeof(Nm));
	int year1, mon1, day1, h1, m1, s1, now_op;
	while(fscanf(fp_package, "%s%s%d%d-%d-%d %d:%d:%d", id, Nm, &now_op, &year1, &mon1, &day1, &h1, &m1, &s1) != EOF){
		if(now_op == opbl){
			printf("用户%s于 %d-%d-%d %d:%d:%d 购买套餐%s\n", id, year1, mon1, day1, h1, m1, s1, Nm);
		}
	}
	fseek(fp_package, 0, SEEK_END);//文件指针指向结尾 
}
void Sta_call(){//话费查询统计管理——统计用户通话信息——各类型通话时长 
	printf("以下为各类型电话通话总时长：\n");
	int Call_time = 0, Oversea_time = 0;
	rewind(fp_call);//文件指针指向开始 
	rewind(fp_oversea);
	
	char id[11], ph1[15], ph2[15];
	memset(id, 0, sizeof(id));
	memset(ph1, 0, sizeof(ph1));
	memset(ph2, 0, sizeof(ph2)); 
	int year1, mon1, day1, hour1, min1, sec1,
		year2, mon2, day2, hour2, min2, sec2;
	int tim;
	double Fee;
	while(fscanf(fp_call, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d分钟%lf元", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		int now_us = Ump[id];
		if(U[now_us].Operator == opbl){//统计属于该运营商的用户的通话时间 
			Call_time += tim;
		}
	}
	
	while(fscanf(fp_oversea, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d分钟%lf元", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		int now_us = Ump[id];
		if(U[now_us].Operator == opbl){
			Oversea_time += tim;
		}
	}
	
	printf("国内通话总时长：%d分钟\n", Call_time);
	printf("国际长途总时长：%d分钟\n", Oversea_time);
	
	fseek(fp_call, 0, SEEK_END);
	fseek(fp_oversea, 0, SEEK_END);//文件指针指向末尾 
}
bool cmp_tot(const Time_block &A, const Time_block &B){//分区通话总时长排序函数 
	if(A.Data+A.Oversea == B.Data+B.Oversea){
		return A.st < B.st;
	} else return (A.Data+A.Oversea > B.Data+B.Oversea);
	
}
bool cmp_blk_call(const Time_block &A, const Time_block &B){//分区国内通话时长排序函数 
	if(A.Call == B.Call) return A.st < B.st;
	else return A.Call > B.Call;
}
bool cmp_blk_oversea(const Time_block &A, const Time_block &B){//分区国际长途时长排序函数 
	if(A.Oversea == B.Oversea) return A.st < B.st;
	else return A.Oversea > B.Oversea;
}
void Sort_blk(){//话费查询统计管理——统计用户通话信息——各时段通话业务量排序 
	printf("以下为各个时段通话业务量的排序：\n");
	
	rewind(fp_call);
	rewind(fp_oversea);//文件指针指向开始 
	
	Tb_init();
	int now_us;
	char id[11], ph1[15], ph2[15];
	int year1, mon1, day1, hour1, min1, sec1,
		year2, mon2, day2, hour2, min2, sec2;
	int tim;
	double Fee;
	while(fscanf(fp_call, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d分钟%lf元", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
		now_us = Ump[id];//用Ump确定用户下标 
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
	
	while(fscanf(fp_oversea, "%s%s%s%d-%d-%d %d:%d:%d %d-%d-%d %d:%d:%d%d分钟%lf元", id, ph1, ph2, &year1, &mon1, &day1, &hour1, &min1, &sec1, &year2, &mon2, &day2, &hour2, &min2, &sec2, &tim, &Fee) != EOF){
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
	printf("按总业务量排序结果为：\n");
	for(int i=0; i<12; i++){
		printf("%d:00 ~ %d:00   %d分钟\n", Tb[i].st, Tb[i].ed+1, Tb[i].Call+Tb[i].Oversea);
	}
	
	sort(Tb, Tb+12, cmp_blk_call);
	printf("按国内通话排序结果为：\n");
	for(int i=0; i<12; i++){
		printf("%d:00 ~ %d:00   %d分钟\n", Tb[i].st, Tb[i].ed+1, Tb[i].Call);
	}
	
	sort(Tb, Tb+12, cmp_blk_oversea);
	printf("按国际长途排序结果为：\n");
	for(int i=0; i<12; i++){
		printf("%d:00 ~ %d:00   %d分钟\n", Tb[i].st, Tb[i].ed+1, Tb[i].Oversea);
	}
	
	fseek(fp_call, 0, SEEK_END);
	fseek(fp_oversea, 0, SEEK_END);//文件指针指向末尾 
}

struct Simple_user//用来为用户排序的结构体 相当于用户结构体的简化 
{
	char ID[15];
	int Call, Oversea;
	double Data;
}Su[10];
bool cmp_su_call(const Simple_user &A, const Simple_user &B){//用户通话时长排序函数 
	return A.Call > B.Call;
}
bool cmp_su_oversea(const Simple_user &A, const Simple_user &B){//用户国际长途排序函数 
	return A.Oversea > B.Oversea;
}
bool cmp_su_data(const Simple_user &A, const Simple_user &B){//用户流量使用排序函数 
	return A.Data > B.Data;
}
void Sort_user(){//话费查询统计管理——统计用户通话信息——对用户通话时长与流量进行排序 
	printf("以下为按用户通话及流量使用的排序：\n");
	memset(Su, 0, sizeof(Su));
	int Su_cnt = 0;
	for(int i=1; i<=User_cnt; i++){
		if(U[i].Operator == opbl){//选取属于该运营商的用户 
			Su_cnt++;
			memcpy(Su[Su_cnt].ID, U[i].ID, sizeof(U[i].ID));
			Su[Su_cnt].Call = U[i].C_sum;
			Su[Su_cnt].Oversea = U[i].O_sum;
			Su[Su_cnt].Data = U[i].D_sum;
		}
	}
	sort(Su+1, Su+1+Su_cnt, cmp_su_call);
	printf("按用户国内通话排序：\n");
	for(int i=1; i<=Su_cnt; i++){
		printf("%s  %d分钟\n", Su[i].ID, Su[i].Call);
	}
	sort(Su+1, Su+1+Su_cnt, cmp_su_oversea);
	printf("按用户国际长途排序：\n");
	for(int i=1; i<=Su_cnt; i++){
		printf("%s  %d分钟\n", Su[i].ID, Su[i].Oversea);
	}
	sort(Su+1, Su+1+Su_cnt, cmp_su_data);
	printf("按用户流量使用排序：\n");
	for(int i=1; i<=Su_cnt; i++){
		printf("%s  %.2lfMB\n", Su[i].ID, Su[i].Data);
	}
}
void Statistic_detial(){//话费查询统计管理——统计用户通话信息
	printf("您已进入用户通话信息统计。\n");
	printf("1.统计各类型通话时长，\n2.对各个时段业务量排序，\n3.对用户通话及流量使用排序，\n#.退出用户通话信息统计。\n请输入您的操作：");
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
			printf("对不起，您的输入格式错误。\n");
		}
		printf("1.统计各类型通话时长，\n2.对各个时段业务量排序，\n3.对用户通话及流量使用排序，\n#.退出用户通话信息统计。\n请输入您的操作：");
	}
} 
void Query_statistic(){//话费查询统计管理
	printf("%s管理员%s，欢迎进入话费查询统计管理\n", Op[opbl], Ad[ad].ID);
	char op[20];
	printf("1.查看用户套餐情况，\n2.统计用户通话信息，\n#.退出话费查询统计管理。\n请输入您的操作：");
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1'){
			Query_ad_package();
		} else if(op[0] == '2'){
			Statistic_detial();
		} else {
			printf("对不起，您的输入格式错误。\n");
		}
		printf("1.查看用户套餐情况，\n2.统计用户通话信息，\n#.退出话费查询统计管理。\n请输入您的操作：");
	}
	printf("您已退出话费查询统计管理。\n");
}
void Query_ad_pkg(){//套餐管理——套餐查询 
	printf("您已进入套餐查询，以下为%s运营商所有套餐内容。\n", Op[opbl]);
	printf("****************************************\n");
	for(int i=1; i<=Package_cnt; i++){//遍历所有套餐 
		if(P[i].Operator == opbl){//查询属于该运营商的套餐 
			P[i].Print();
		}
	}
}
void Add_pkg(){//套餐管理——添加套餐 
	printf("您已进入套餐添加，添加套餐请输入1，退出请输入#：");
	char op[20]; memset(op, 0, sizeof(op));
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1'){
			Package_cnt++; int k = Package_cnt;
			printf("套餐名称："); scanf("%s", P[k].Name);
			printf("套餐价格(元/月)："); scanf("%d", &P[k].Price);
			printf("套餐优惠信息："); scanf("%s", P[k].Special_offer);
			printf("套餐类型："); scanf("%s", P[k].Package_type);
			printf("国内通话时长(分钟)："); scanf("%d", &P[k].Call);
			printf("国际长途通话时长(分钟)："); scanf("%d", &P[k].Oversea);
			printf("总流量(G)："); scanf("%d", &P[k].Data);
			P[k].Operator = opbl; 
			printf("套餐添加成功。\n");
		} else {
			printf("对不起，您的输入格式错误。\n");
		}
		printf("您已进入套餐添加，添加套餐请输入1，退出请输入#：");
	}
}
void Manage_package(){//套餐管理 
	printf("%s管理员%s，欢迎进入套餐管理。\n", Op[opbl], Ad[ad].ID);
	printf("1.查询套餐，\n2.添加套餐，\n#.退出套餐管理。\n请输入您的操作：");
	char op[20]; memset(op, 0, sizeof(op));
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1'){
			Query_ad_pkg();
		} else if(op[0] == '2'){
			Add_pkg();
		} else {
			printf("对不起，您的输入格式错误。\n");
		}
		printf("1.查询套餐，\n2.添加套餐，\n#.退出套餐管理。\n请输入您的操作：");
	} 
	printf("您已退出套餐管理。\n");
}
void Change_ad_pswd(){//个人信息管理——修改密码 
	char pw[20]; memset(pw, 0, sizeof(pw));
	printf("请输入您的旧密码，或输入#退出密码修改：");
	while(scanf("%s", pw) == 1){//判断旧密码是否正确 
		if(pw[0] == '#') return; 
		if(Check(pw, Ad[ad].Password) == 0){
			printf("对不起，您输入的旧密码不正确，请再次输入，或输入#退出号码修改："); 
		} else {
			printf("请输入您的新密码，注意密码最少为6位：");
			break;
		}
	}
	while(scanf("%s", pw) == 1){//新密码至少为6位 
		int len = strlen(pw);
		int flag = 1;
		if(len < 6) flag = 0;
 		if(!flag){
			printf("对不起，您的新密码不符合要求，请重新输入："); 
		} else {
			printf("修改成功，您的密码已为：%s\n", pw);
			memcpy(Ad[ad].Password, pw, sizeof(pw));
			break;
		}
	}
}
void Change_ad_email(){//个人信息管理——修改邮箱 
	char Em[30]; memset(Em, 0, sizeof(Em)); 
	printf("请输入您的旧邮箱，或输入#退出号码修改：");
	while(scanf("%s", Em) == 1){//输入正确旧密码 
		if(Em[0] == '#') return; 
		if(Check(Em, Ad[ad].Email) == 0){
			printf("对不起，您输入的旧邮箱不正确，请再次输入，或输入#退出邮箱修改："); 
		} else {
			printf("请输入您的新邮箱，注意邮箱格式为:***@***.***：");
			break;
		}
	}
	
	while(scanf("%s", Em) == 1){//输入新密码 需满足 ***@***.*** 
		if(is_email(Em) == 0){
			printf("对不起，您的新邮箱不符合要求，请重新输入："); 
		} else {
			printf("修改成功，您的邮箱已为：%s\n", Em);
			memcpy(Ad[ad].Email, Em, sizeof(Em));
			break;
		}
	}
}
void Ad_information_manage(){//管理员个人信息管理 
	printf("%s管理员%s，欢迎进入个人信息管理。\n", Op[opbl], Ad[ad].ID);
	printf("1.修改密码，\n2.修改邮箱，\n#.退出个人信息管理。\n请输入您的操作：");
	char op[20];
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1') Change_ad_pswd();
		else if(op[0] == '2') Change_ad_email();
		else printf("对不起，您的输入格式错误。\n"); 
		printf("1.修改密码，\n2.修改邮箱，\n#.退出个人信息管理。\n请输入您的操作：");
	} 
	printf("您已退出个人信息管理。\n");
}
void Ad_operation(){//管理员操作 
	Ad_login(); //管理员登录并确定管理员下标 
	if(!ad) return;
	opbl = Ad[ad].Op_belong;//管理员所属运营商 
	char op[20]; memset(op, 0, sizeof(op)); 
	//op-># Exit  op->1 话费查询统计  op->2 套餐管理 
	//op->3 个人信息管理
	printf("%s管理员%s，请选择管理员操作，\n", Op[opbl], Ad[ad].ID);
	printf("1.话费查询统计，\n2.套餐管理，\n3.个人信息管理，\n#.退出管理员操作。\n请输入您的操作：");
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;
		else if(op[0] == '1') Query_statistic();
		else if(op[0] == '2') Manage_package();
		else if(op[0] == '3') Ad_information_manage();
		else {
			printf("对不起，您的输入格式错误。\n");
		}
		printf("1.话费查询统计，\n2.套餐管理，\n3.个人信息管理，\n#.退出管理员操作。\n请输入您的操作：");
	
	} 
	return;
}
//=====================================================================

void User_init(){//用户信息初始化 
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
void Ad_init(){//管理员信息初始化 
	Admp.clear();
	memset(Ad, 0, sizeof(Ad));
	for(int i=1; i<=3; i++){
		fscanf(fp_ad_init, "%s%d%s%s%s", Ad[i].ID, &Ad[i].Op_belong, Ad[i].Name, Ad[i].Email, Ad[i].Password);
		Admp[Ad[i].ID] = i;
	}
}
void Package_init(){//套餐信息初始化 
	memset(P, 0, sizeof(P));
	fscanf(fp_package_init, "%d", &Package_cnt);
	for(int i=1; i<=Package_cnt; i++){
		fscanf(fp_package_init, "%s%d%d%s%s%d%d%d", P[i].Name, &P[i].Operator, &P[i].Price, P[i].Package_type, P[i].Special_offer, &P[i].Call, &P[i].Oversea, &P[i].Data);
//		printf("%s\n%d\n", P[i].Name, P[i].Price);
	}
}
void Init(){	
	Package_init();//套餐信息初始化 
	User_init();//用户信息初始化 
	Ad_init();//管理员信息初始化 
	Tb_init();
}
//=================================================================

int main(){
	srand((unsigned int)time(NULL));
	//打开文件 
	fp_user_init = fopen("user_init.txt", "r");
	fp_ad_init = fopen("ad_init.txt", "r");
	fp_package_init = fopen("package_init.txt", "r");
	fp_call = fopen("call_record.txt", "w+");
	fp_data = fopen("data_record.txt", "w+");
	fp_package = fopen("package_record.txt", "w+");
	fp_oversea = fopen("oversea_record.txt", "w+");
	//检查文件 
	if(fp_user_init == NULL){
		printf("很抱歉，user_init.txt打开失败，请检查文件是否存在。程序即将关闭。\n");
		exit(1);
	} 
	if(fp_ad_init == NULL){
		printf("很抱歉，ad_init.txt打开失败，请检查文件是否存在。程序即将关闭。\n");
		exit(1);
	} 
	if(fp_package_init == NULL){
		printf("很抱歉，package_init.txt打开失败，请检查文件是否存在。程序即将关闭。\n");
		exit(1);
	} 
	if(fp_call == NULL){
		printf("很抱歉，call_record.txt打开失败，请检查文件是否存在。程序即将关闭。\n");
		exit(1);
	} 
	if(fp_oversea == NULL){
		printf("很抱歉，oversea_record.txt打开失败，请检查文件是否存在。程序即将关闭。\n");
		exit(1);
	} 
	if(fp_data == NULL){
		printf("很抱歉，data_record.txt打开失败，请检查文件是否存在。程序即将关闭。\n");
		exit(1);
	} 
	if(fp_package == NULL){
		printf("很抱歉，package_record.txt打开失败，请检查文件是否存在。程序即将关闭。\n");
		exit(1);
	} 
	Init();//初始化 
	printf("===========================================欢迎您使用电话费统计管理系统！===========================================\n");
	printf("1.用户操作\n2.管理员操作\n#.退出系统\n请输入您的操作："); 
	char op[20]; //  op->1 Users  op->2 Ad
	while(scanf("%s", op) == 1){
		if(op[0] == '#') break;//退出程序 
		else if(op[0] == '1') User_operation();//用户操作 
		else if(op[0] == '2') Ad_operation();//管理员操作 
		else {
			printf("对不起，您的输入格式错误。\n");
		}
		printf("1.用户操作\n2.管理员操作\n#.退出系统\n请输入您的操作："); 
	}
	printf("===========================================电话费统计管理系统感谢您的使用！=========================================\n");
	//关闭文件 
	fclose(fp_user_init);
	fclose(fp_ad_init);
	fclose(fp_package_init);
	fclose(fp_call);
	fclose(fp_oversea);
	fclose(fp_data);
	fclose(fp_package);
	
	return 0;
}

