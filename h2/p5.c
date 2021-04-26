#ifndef _PROB45_GENERATOR
#define _PROB45_GENERATOR
#include<stdio.h>
#include<string.h>
typedef unsigned long long uint64_t;
static uint64_t _rnd_rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}
uint64_t _rnd_s[4];
void _rnd_seed(uint64_t _s[4]){_rnd_s[0]=_s[0];_rnd_s[1]=_s[1];_rnd_s[2]=_s[2];_rnd_s[3]=_s[3];}
uint64_t _rnd_next(){
    const uint64_t result=_rnd_rotl(_rnd_s[0]+_rnd_s[3],23)+_rnd_s[0];
    const uint64_t t=_rnd_s[1]<<17;
    _rnd_s[2]^=_rnd_s[0]; _rnd_s[3]^=_rnd_s[1]; _rnd_s[1]^=_rnd_s[2]; _rnd_s[0]^=_rnd_s[3]; _rnd_s[2]^=t;
    _rnd_s[3] = _rnd_rotl(_rnd_s[3], 45);
    return result;
}
int _rnd_randint(int l,int r){
    return (_rnd_next()&(r-l))+l;    
}
void _rnd_genarray(long n,long *a,long l,long r,unsigned char *v,long flush){
    if(v){
        if(flush) memset(v+l,0,(r-l+1)*sizeof(unsigned char));
        for(int i=0;i<n;i++){
            do{
                a[i]=_rnd_randint(l,r);
            }while(v[a[i]]);
            v[a[i]]=1;
        }
    }
    else for(int i=0;i<n;i++) a[i]=_rnd_randint(l,r);
}
long _generator_n,_generator_t,_generator_task,_generator_v;
void _generator_init(){
    scanf("%ld%ld%ld%ld",&_generator_task,&_generator_t,&_generator_n,&_generator_v);
    scanf("%llu%llu%llu%llu",_rnd_s,_rnd_s+1,_rnd_s+2,_rnd_s+3);
}
int _generator_gett(){
    return _generator_t;
}
void _generator_getdata(long* rn,long** rp,long **rq,long **rr){
    static long p[10000005],q[10000005],r[10000005];
    static unsigned char _vis[16777216],*vis=_vis+8388608;
    long n=_generator_n;
    if(_generator_task==2||_generator_task==3) _rnd_genarray(n,p,-_generator_v,_generator_v-1,vis,1);
    else  _rnd_genarray(n,p,-_generator_v,_generator_v-1,NULL,0);
    if(_generator_task==2){
        _rnd_genarray(n,q,-_generator_v,_generator_v-1,vis,1);
        _rnd_genarray(n,r,-_generator_v,_generator_v-1,vis,0);
    }else{
        _rnd_genarray(n,q,-_generator_v,_generator_v-1,NULL,0);
        _rnd_genarray(n,r,-_generator_v,_generator_v-1,NULL,0);
    }
    *rn=n; *rp=p; *rq=q; *rr=r;
}
struct{
    void (*init)();
    int (*getT)();
    void (*getData)(long*,long**,long**,long**);
}generator={_generator_init,_generator_gett,_generator_getdata};
#endif

long n,*p,*q,*r;
int BIT[3000000];
long MAX;
void swap(int i,int j){
    long P=p[i];
    long Q=q[i];
    long R=r[i];
    p[i]=p[j];
    q[i]=q[j];
    r[i]=r[j];
    p[j]=P;
    q[j]=Q;
    r[j]=R;
}
long quicksort(int l,int r){
    int pivot = p[l];
    int i=l+1;
    int j=r;
    if(l>=r)return 0;
    while(1){
        while(i<=r){
            if(p[i] > pivot)break;
            i++;
        }
        while(j>l){
            if(p[j]< pivot)break;
            j--;
        }
        if(i>j)break;
        swap(i,j);
    }
    swap(l,j);
    quicksort(l,j-1);
    quicksort(j+1,r);
}
void move(){
    long min=q[0];
    long max=r[0];
    long a=0;
    for(int i=0;i<n;i++){
        if(min > q[i]) min=q[i];
        if(max < r[i]) max=r[i];
    }
    if(min <= 0){
        a=0-min+1;
        for(int i=0;i<n;i++){
            q[i]=q[i]+a;
            r[i]=r[i]+a;
        }
    }
    MAX = max+a;
}

long lowbit(long x){
    return x&-x;
}

long add(long x){
    while(x<=MAX){
        BIT[x]++;
        x+=lowbit(x);
    }
}

long sum(long x){
    int res=0;
    while(x>=1){
        res+=BIT[x];
        x-=lowbit(x);
    }
    return res;
}

int main() {
    generator.init();
    int t = generator.getT();
    long count=0;
    long same=0;
    while (t--) {
        generator.getData(&n, &p, &q, &r);
        quicksort(0,n-1);
        for(int i=0;i<n;i++){
            if(q[i]>r[i]){
                long swap=q[i];
                q[i]=r[i];
                r[i]=swap;
            }
        }
        move();
        for(int i=0;i<n;i++){
            if(p[i]!=p[i+1]){ //p不相同，算當前三角形的左頂點到N個三角形中最大右頂點的和。[q[i] MAX]
                count+=(i-sum(q[i]-1));
                add(r[i]);
            }
            else if(p[i]==p[i+1]){//p相同，把每個p相同的三角形，都先用左頂點算小於等於的右頂點，再加進去BIT，避免有多算得情形。
                int j=i;
                while(j<n){
                    if(p[j]!=p[j+1])break;
                    j++;
                    same++;
                }
                for(int k=i;k<=j;k++){
                    count+=(i-sum(q[k]-1));
                }
                for(int k=i;k<=j;k++){
                    add(r[k]);
                }
                count+=(((same+1)*(same))/2);//若有連續i個相同的p，就算Ci取2的情況有幾種再加進去。
                same=0;
                i=j;
            }
        }
        printf("%ld\n",count);
        count=0;
        if(t!=0)memset(BIT,0,sizeof(int)*3000000);
        //memset(p,0,sizeof(long)*300000);
        //memset(q,0,sizeof(long)*300000);
        //memset(r,0,sizeof(long)*300000);
    }
}
//[1 MAX]的前綴和 減去 [1 當前三角形的左頂點的值-1]的前綴和 為當前三角形左下頂點小於等於前面三角形右下頂點的個數
//MAX為n個三角形的右下頂點中的最大值。