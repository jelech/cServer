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

int compare(const string& str1,const string& str2){
    cout << "." << str1.length() << "_" << endl;
    cout << "," << str2.length() << "_" << endl;

    if (str1.compare(str2) == 0) return 0;
    printf("debug%d\n",str1.compare(str2));
    // int x=0,y=0;
    // while (x<str1.length() && y<str2.length()){
    //     if (str1[x]==' ') x++;
    //     else if (str2[y]==' ') y++;
    //     else if (str1[x]!=str2[y]) return 1;
    //     else x++,y++;
    // }
    // while (x<str1.length() && str1[x]==' ') x++;
    // while (y<str2.length() && str2[y]==' ') y++;
    // if (x==str1.length() && y==str2.length()) return 1;
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
    string str1[2005],str2[2005];
    op.open(wj1);
    int cnt1=0;
    // ("debug::??\n");
    while (getline(op,str1[cnt1++])){
        
        if(str1[cnt1-1][str1[cnt1-1].length() -1] == '\r'){
            str1[cnt1 - 1].pop_back();
        }
    }
    op.close();
    op.open(wj2);
    int cnt2=0;
    while (getline(op,str2[cnt2++])){
        
        if(str2[cnt2-1][str2[cnt2-1].length() -1] == '\r'){
            str2[cnt2 - 1].pop_back();
        }
    }
    op.close();

    if(cnt1 != cnt2)
        return "WA";

    for (int i=0;i<cnt1;i++){
        // cout << str1[i] << "," <<endl;
        // cout << str2[i] << "." << endl;
        if (str1[i]!=str2[i])
            return "WA";
    }
    // if (cnt1>cnt2){
    //     for (int i=cnt2;i<cnt1;i++){
    //         if (!Is_empty(str1[i])){
    //             res=2;
    //             break;
    //         }
    //     }
    // }
    // if (cnt2>cnt1){
    //     for (int i=cnt1;i<cnt2;i++){
    //         if (!Is_empty(str2[i])){
    //             res=2;
    //             break;
    //         }
    //     }
    // }

    return "AC";
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
