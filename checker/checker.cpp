#include<iostream>
using namespace std;
#define ll long long
const int inf=1e9+7;
const ll INF=1e18;
const int maxn=1e6+100;


void solve(){
    FILE *fp1,*fp2;
    fp1=fopen("in.txt","r");
    fp2=fopen("out.txt","r");
    char ch1,ch2;
    int faliure=0,right=0,ukn=0;
    while((ch1=fgetc(fp1))!=EOF&&(ch2=fgetc(fp2))!=EOF){
        if(ch2=='-'){ukn++;continue;}
        if(ch1!=ch2)faliure++;
        right++;
    }
    cout<<"right: "<<right<<endl;
    cout<<"false: "<<faliure<<endl;
    cout<<"unknown: "<<ukn<<endl;
}

int main(){
    ios_base::sync_with_stdio(0);
    int t=1;
    //cin>>t;
    while(t--){
        solve();
    }
    system("pause");
}
