#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int times=0;
void search(int **P,int l,int r,int index){
    if(index==0){
        times++;
        if(P[index][1]!=-1){
            if(P[index][0]-1 >= l)search(P,l,P[index][0]-1,P[index][1]-1);
        }
        if(P[index][2]!=-1){
            if(P[index][0]+1 <= r)search(P,P[index][0]+1,r,P[index][2]-1);
        }
    }
    else{
        if(P[index][0] >= l && P[index][0] <= r){
            times++;
            if(P[index][1]!=-1){
                if(P[index][0]-1 >= l)search(P,l,P[index][0]-1,P[index][1]-1);
            }
            if(P[index][2]!=-1){
                if(P[index][0]+1 <= r)search(P,P[index][0]+1,r,P[index][2]-1);
            }
        }
        else{
            if(P[index][1]!=-1){
                int m=P[index][0];
                if(m > r)search(P,l,r,P[index][1]-1);
            }
            if(P[index][2]!=-1){
                int m=P[index][0];
                if(m < l)search(P,l,r,P[index][2]-1);
            }
        }
    }
}
int main(){
    int N;
    scanf("%d",&N);
    int i=0,j=0;
    int **arr=malloc(sizeof(int *)*N);
    for(i=0;i<N;i++){
        int *new=malloc(sizeof(int)*3);
        scanf("%d %d %d",&new[0],&new[1],&new[2]);
        arr[i]=new;
    }
    search(arr,1,1000000000,0);
    printf("%d\n",times);
    return 0;
}