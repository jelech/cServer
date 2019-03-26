/*
 * 输入：两个文件名
 * 功能：比较文件内容是否一致；
 *       若完全相同或者文件末有多余的空行，则判断为AC；
 *       若每行中间或末尾的空格数量不符合，但是忽略空格差异后能AC，则判断为PE；
 *       若有非空格字符无法匹配，则判断为WA；
 * 限制：文件内容最大行数为2e6行
 *
 */


#include <iostream>
#include <cstdio>
#include <fstream>
using namespace std;

int compare(string str1,string str2){
    if (str1==str2) return 0;
    int x=0,y=0;
    while (x<str1.length() && y<str2.length()){
        if (str1[x]==' ') x++;
        else if (str2[y]==' ') y++;
        else if (str1[x]!=str2[y]) return 1;
        else x++,y++;
    }
    while (x<str1.length() && str1[x]==' ') x++;
    while (y<str2.length() && str2[y]==' ') y++;
    if (x==str1.length() && y==str2.length()) return 1;
    return 2;
}

bool Is_empty(string s){
    if (s.length()==0) return 1;
    int x=0;
    while (x<s.length() && s[x]==' ') x++;
    if (x==s.length()) return 1;
    return 0;
}

string check(const string& wj1, const string& wj2){
    ifstream op;
    string str1[20005],str2[20005];
    op.open(wj1);
    int cnt1=0;
    while (!op.eof()){
        char ch=op.get();
        if (ch=='\n') cnt1++;
        else str1[cnt1]+=ch;
    }
    op.close();

    op.open(wj2);
    int cnt2=0;
    while (!op.eof()){
        char ch=op.get();
        if (ch=='\n') cnt2++;
        else str2[cnt2]+=ch;
    }
    op.close();
    int res=0;
    for (int i=0;i<min(cnt1,cnt2);i++){
        if (res==2) break;
        res=compare(str1[i],str2[i]);
    }

    if (cnt1>cnt2){
        for (int i=cnt2;i<cnt1;i++){
            if (!Is_empty(str1[i])){
                res=2;
                break;
            }
        }
    }
    if (cnt2>cnt1){
        for (int i=cnt1;i<cnt2;i++){
            if (!Is_empty(str2[i])){
                res=2;
                break;
            }
        }
    }

    if (res==0) return "AC";
    if (res==1) return "PE";
    if (res==2) return "WA";
}

// 输入两个path
int main(int argc, char const *argv[])
{
    if(argc < 3){
        perror("check mode error");
        return -1;
    }
    string a = argv[1];
    string b = argv[2];
    cout << check(a, b);
}
