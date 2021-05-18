#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct DisjointSet{
    // TODO: Determine fields to use by your method
    int parent;
    const char *s;
} disjoint_set;

disjoint_set ds[1 << 24];
bool set[1 << 24] = {};

int c2i(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    else if ('a' <= c && c <= 'z') return c - 'a' + 10;
    else if ('A' <= c && c <= 'Z') return c - 'A' + 36;
    return -1;
}

int hash(const char* s) {
    int ret = 0;
    for (int i = 0; i < 4; ++i)
        ret = (ret << 6) | c2i(s[i]);
    return ret;
}

void makeset(const char* s){
    int i = hash(s);
    // TODO: Initialize a set
    ds[i].parent = i;
    ds[i].s = malloc(sizeof(char)*4);
    strcpy(ds[i].s,s);
}

inline void static init(const char* s) {
    int i = hash(s);
    if (!set[i]) {
        makeset(s);
        set[i] = 1;
    }
}

int find_set(const char* s) {
    init(s);
    int i = hash(s);
    // TODO: Implement your find algorithm here
    if(ds[i].parent != i){
        ds[i].parent = find_set(ds[ds[i].parent].s);
    }
    return ds[i].parent;/* something */
}

void link(const char *ra, const char *rb) {
    int a = find_set(ra), b = find_set(rb);
    // TODO: Implement your union algorithm here
    ds[b].parent = a;
}

bool same_set(const char *a, const char *b) {
    return (find_set(a) == find_set(b));
}

bool stringcompare(const char *a, const char *b) {
    // implement your string compare
    int A = strlen(a);
    int B = strlen(b);
    int max = A;
    char *a_now = malloc(sizeof(char)*(4));
    char *b_now = malloc(sizeof(char)*4);
    if(A < B)max = B;
    for(int i = 0;i < B;i+=4){
        for(int j = 0;j < 4;j++){
            a_now[j] = a[j + i];
            b_now[j] = b[j + i];
        }
        int ha = find_set(a_now);
        int hb = find_set(b_now);
        if(ha != hb){
            return false;
        }
    }
    return true;
}

int main() {
    int n;
    scanf("%d", &n);
    char cmd[16], a[512], b[512];
    bool *ans=malloc(sizeof(bool));
    int time = 0;
    for (int i = 0; i < n; ++i) {
        scanf("%s %s %s", cmd, a, b);
        if (!strcmp(cmd, "union")) {
            init(a);
            init(b);
            link(a, b);
        } else {
            bool same = stringcompare(a, b);
            ans = realloc(ans,sizeof(bool)*(time+1));
            ans[time++]=same;
        }
    }
    for(int i = 0;i<time;i++){
        if(ans[i])printf("True\n");
        else printf("False\n");
    }
}