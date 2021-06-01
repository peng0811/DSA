#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
typedef struct data{
    int min_s;
    int min_len;
    int size_s;
    int condition[54];
    int check[54];
    int index_con[100000];    
    char s[100002];
    char p[100002];
    bool r[54];
    int prefix[100001];
    bool dash[100001];
}data;

int check_case(char s[],int index){
    if(s[index] >= 65 && s[index] <= 90){
        return s[index] - 64;
    }else if(s[index] >= 97 && s[index] <= 122){
        return s[index] - 70;
    }
}

int main(){
    int T;
    scanf("%d",&T);
    data **D = malloc(sizeof(data *)*(T));
    for(int i = 0;i < T;i++){
        int size_d,size_s;
        int condition_num = 0;
        int check_num = 0;
        int min_len = -1;
        int start = -1;
        int end = -1;
        int next_con = 0;
        int index = 0;
        int min_s = -1;
        bool stop = false;
        char c;
        D[i] = calloc(1,sizeof(data));
        scanf("%s",D[i]->s);
        scanf("%s",D[i]->p);
        size_s = strlen(D[i]->s);
        size_d = strlen(D[i]->p);
        for(int j = 0;j < size_d;j++){
            index = check_case(D[i]->p,j);
            D[i]->condition[index]++;
        }
        for(int j = 1;j <= 52;j++){
            if(D[i]->condition[j] > 0){
                condition_num++;
            }
        }
        for(int j = 0;j < size_s;j++){
            index = check_case(D[i]->s,j);
            if(D[i]->condition[index] > 0){
                D[i]->index_con[next_con++] = j;
                D[i]->check[index]++;
                if(D[i]->check[index] >= D[i]->condition[index] && !D[i]->r[index]){
                    check_num++;
                    D[i]->r[index] = true;
                }
                if(start == -1)start = j;
                if(check_num >= condition_num){
                    end = j;
                    break;
                }
            }
        }
        int k = 1;
        while(check_num >= condition_num){
            if(min_len > end - start + 1 || min_len == -1){
                min_len = end - start + 1;
                min_s = start;
            }
            index = check_case(D[i]->s,start);
            start = D[i]->index_con[k++];
            D[i]->check[index]--;
            if(D[i]->check[index] < D[i]->condition[index]){
                check_num--;
                for(int j = end + 1;j < size_s;j++){
                    int c = check_case(D[i]->s,j);
                    if(D[i]->condition[c] > 0){
                        D[i]->index_con[next_con++] = j;
                        D[i]->check[c]++;
                    }
                    if(c == index){
                        check_num++;
                        end = j;
                        break;
                    }
                }
            }
            if(D[i]->check[index] < D[i]->condition[index])break;
        }
        if(min_len != -1){
            for(int j = 0;j < size_s - (min_s + min_len);j++)
                D[i]->s[j + min_s] = D[i]->s[j + min_s + min_len];
            D[i]->size_s = size_s - min_len;  
        }else D[i]->size_s = size_s;
        int len = 0;
        int fnt = 0,bak = 0;
        int size = D[i]->size_s;
        int j = 0;
        bool ch = false;
        bool same = true;
        int ch_len = 0;
        while(D[i]->size_s > 1){
            fnt = 0 + len;
            bak = size - len -1;
            if( len >= size/2 || ch_len >= size/2)break;
            for(;j < size/2;j++){
                if(D[i]->s[bak - j] == D[i]->s[fnt]){
                    ch = true;
                    break;
                }else ch = false;
            }
            if(ch){
                ch_len = j + 1;
                if(D[i]->s[fnt + ch_len - 1] == D[i]->s[bak]){
                    for(j = 0;j < ch_len;j++){
                        if(D[i]->s[fnt + j] != D[i]->s[bak - ch_len + 1 + j]){
                            same = false;
                            break;
                        }else same = true;
                    }
                    if(same){
                        D[i]->dash[ch_len + len] = true;
                        D[i]->dash[bak - ch_len + 1] = true;
                        len += ch_len;
                        j = 0;
                        same = false;
                    }else j = ch_len;
                }else j = ch_len;
            }else break;
        }
    }
    for(int i = 0;i < T;i++){
        for(int j = 0;j < D[i]->size_s;j++){
            if(D[i]->dash[j])printf("|");
            printf("%c",D[i]->s[j]);
        }
        printf("\n");
    }
}