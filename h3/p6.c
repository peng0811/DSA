#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
int ans[1000000][2];
int SIZE[100001];
int top[100001];
int BIT[100001];
bool q_check[100001];

typedef struct node{
    int key;
    struct node *nxt;
    struct node *fnt;
}node;
typedef struct queue
{
    node *head;
    node *tail;
} queue;

node *alloc(node *old,int key){
    node *new = malloc(sizeof(node));
    new->key = key;
    new->nxt = NULL;
    if(old != NULL)
        old->nxt = new;
    new->fnt = old;
    return new; 
}

long lowbit(long x)
{
    return x & -x;
}
long add(long x)
{
    while (x <= 100001)
    {
        BIT[x]++;
        x += lowbit(x);
    }
}
long minu(long x)
{
    while (x <= 100001)
    {
        BIT[x]--;
        x += lowbit(x);
    }
}
long sum(long x)
{
    int res = 0;
    while (x >= 1)
    {
        res += BIT[x];
        x -= lowbit(x);
    }
    return res;
}
int find(int l, int r)
{
    int m = (l + r) / 2;
    if (l >= r)
        return l;
    int left = sum(m);
    int right = 0;
    if (sum(m) > 0)
    {
        find(l, m);
    }
    else if (sum(r) - left > 0)
    {
        find(m + 1, r);
    }
}

int main()
{
    int N;
    scanf("%d", &N);
    int **adj = malloc(sizeof(int *) * (N + 1));
    bool Con = true;
    int edge = 0;
    int complete_adj = 0;
    int exist_adj = N;
    queue *q = malloc(sizeof(queue));
    q->head = NULL;
    q->tail = NULL;
    for (int i = 1; i <= N; i++)
    {
        scanf("%d", &SIZE[i]);
        if (SIZE[i] > 0)
        {
            adj[i] = malloc(sizeof(int) * (SIZE[i]));
            for (int j = 0; j < SIZE[i]; j++)
                scanf("%d", &adj[i][j]);
            add(i);
        }
        else
        {
            adj[i] = 0;
            exist_adj--;
        }
    }
    int now_v = find(1, N);
    int next_v = 0;
    int count = 0;
    if (exist_adj > 0)
    {
        while (Con)
        {
            next_v = adj[now_v][top[now_v]];
            if (adj[next_v][top[next_v]] == now_v)
            {
                ans[edge][0] = now_v;
                ans[edge][1] = next_v;
                top[now_v]++;
                top[next_v]++;
                edge++;
                count = 0;
                if (top[next_v] >= SIZE[next_v]){
                    exist_adj--;
                    minu(next_v);
                }else{
                    if(q->head == NULL){
                        q->head = alloc(q->head,next_v);
                        q->tail = q->head;
                    }else q->tail = alloc(q->tail,next_v);
                    q_check[next_v] = true;
                }
                if (top[now_v] >= SIZE[now_v])
                {
                    exist_adj--;
                    minu(now_v);
                    if (exist_adj == 0)break;
                    if (top[next_v] >= SIZE[next_v]){
                        if (q->head == NULL)now_v = find(1, N);
                        else{
                            while(1){
                                now_v = q->head->key;
                                q->head = q->head->nxt;
                                if(q->head == NULL)q->tail = NULL;
                                q_check[now_v] = false;
                                if (top[now_v] < SIZE[now_v]){
                                    break;
                                }else if (q->head == NULL){
                                    now_v = find(1, N);
                                    break;
                                }
                            }
                        }
                    }else{
                        now_v = next_v;
                        q->tail = q->tail->fnt;
                        if(q->tail != NULL){
                            q->tail->nxt = NULL;
                        }else{
                            q->head = NULL;
                        }
                    }
                }
            }else{
                if(q->head != NULL){
                    while(1){
                        now_v = q->head->key;
                        q->head = q->head->nxt;
                        if(q->head != NULL)q->head->fnt = NULL;
                        else q->tail = NULL;
                        q_check[now_v] = false;
                        if(top[now_v] < SIZE[now_v])break;
                        else if(q->head == NULL){
                            now_v = next_v;
                            count++;
                            break;
                        }
                    }
                }else{
                    now_v = next_v;
                    count++;
                }
            }
            if (count > exist_adj)Con = false;
        }
    }
    if (Con)
    {
        printf("Yes\n");
        for (int i = 0; i < edge; i++)
        {
            printf("%d %d\n", ans[i][0], ans[i][1]);
        }
    }
    else
    {
        printf("No\n");
    }
}