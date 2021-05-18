#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define MAX 100000
#define CAP 50
int com[MAX+10];
int head_pos[MAX+1];
int tail_pos[MAX+1];
int max_pos[MAX+1];
bool pack_state[MAX+1];
int OP[MAX*2][3];
int ans[10];

typedef struct q_node{
    int key;
    struct q_node *nxt;
    struct q_node *fnt;
}q_node;
typedef struct queue{
    q_node *head;
    q_node *tail;
    int size;
    int *root;
    int r_size;
    int capacity;
}queue;
q_node *node_alloc(q_node *now,int key){
    q_node *new = malloc(sizeof(q_node));
    new->key = key;
    new->nxt = NULL;
    new->fnt = now;
    if(now != NULL)now->nxt = new;
    return new;
}
queue *alloc(){
    queue *q=malloc(sizeof(queue));
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    q->r_size = 0;
    q->root = calloc(CAP,sizeof(int));
    q->capacity = CAP;
    return q;
}
queue enlarge(queue *q){
    int newcap = ((q->capacity/CAP)*2)*CAP;
    q->root = realloc(q->root,sizeof(int)*newcap);
    q->capacity = newcap;
}
bool check(queue *q){
    return (q->r_size  >= q->capacity - 5)?true : false; 
}
void swap(int root[],int i,int j){
    int s = root[i];
    root[i] = root[j];
    root[j] = s;
}
int upmaxHeapify(int root[],int idx){
    int p = idx/2;
    if(p == 0)return 0;
    if(root[p-1] < root[idx-1]){
        swap(root,p-1,idx-1);
        upmaxHeapify(root,p);
    }
}
int downmaxHeapify(int root[],int p,int size){
    int cl = 2*p;
    int cr = 2*p + 1;
    int max = p;
    if(cl <= size && root[cl-1] > root[p-1])max = cl;
    if(cr <= size && root[cr-1] > root[max-1])max = cr;
    if(max != p){
        swap(root,max-1,p-1);
        downmaxHeapify(root,max,size);
    }
}
void insertheap(int root[],int *size,int data){
    root[(*size)++]=data;
    if((*size) > 1) upmaxHeapify(root,*size);
}
void extractmax(int root[],int *size){
    root[0]=root[--(*size)];
    root[(*size)]=0;
    downmaxHeapify(root,1,*size);
}
void merge(int run[],int stop[],int *size_run,int size_stop){
    for(int i = 0;i < size_stop;i++){
        insertheap(run,size_run,stop[i]);
    }
}
int checkqueue(int data){
    int head = head_pos[data];
    int tail = tail_pos[data];
    int max = max_pos[data];
    if(head > 0 || tail > 0 || max > 0){
        if(head > 0)return 1;
        else if(tail > 0)return 2;
        else if(max > 0)return 3;
    } else if(head == -1 && tail == -1 && max == -1)return 0;
    else return 4;
}

queue enqueue(queue *q,int key){
    if(q->size == 0){
        q->head = node_alloc(q->head,key);
        q->tail = q->head;
    }else{
        q->tail = node_alloc(q->tail,key);
    }
    q->size++;
}
q_node *dequeue(queue *q,int mode){
    if(mode == 1){
        q_node *old = q->head;
        q->size--;
        if(q->size == 0){
            q->head = NULL;
            q->tail = NULL;
        }else{
            q->head = q->head->nxt;
            while(pack_state[q->head->key]){
                q_node *del = q->head;
                q->head = q->head->nxt;
                free(del);
                q->size--;
            }
            q->head->fnt = NULL;
        }
        return old;
    }else if(mode == 2){
        q_node *old = q->tail;
        q->size--;
        q->tail = q->tail->fnt;
        while(pack_state[q->tail->key]){
            q_node *del = q->tail;
            q->tail = q->tail->fnt;
            free(del);
            q->size--;
        }
        q->tail->nxt = NULL;
        return old;
    }
}
q_node *delete_queue(queue *q,int key){
    q_node *ptr = q->head;
    for(int i = 0;i < q->size;i++){
        if(ptr->key == key){
            ptr->fnt->nxt = ptr->nxt;
            ptr->nxt->fnt = ptr->fnt;
            return ptr;
        }
        ptr = ptr->nxt;
    }
}
q_node *merge_queue(queue *q1,queue *q2){
    q2->tail->nxt = q1->head;
    q1->head->fnt = q2->tail;
    q2->tail = q1->tail;
}

int main(){
    int T;
    scanf("%d",&T);
    for(int k=0;k<T;k++){
        int N = 0,O = 0,L = 0;
        scanf("%d %d %d",&N,&O,&L);
        bool Continue=true;
        int i=0;
        int nowcom=0;
        int times=0;
        char s[10]={0};
        queue **Q = calloc(L,sizeof(queue *));
        for(i = 0;i < O;i++){
            scanf("%s %d %d",s,&OP[i][1],&OP[i][2]);
            if(s[0] == 'p')OP[i][0] = 1;
            else if(s[0] == 'm')OP[i][0] = 2; 
        }
        for(i = 0;i < N;i++){
            scanf("%d",&com[i]);
        }
        times = i;
        for(i = 0;i < O;i++){
            int mode = OP[i][0];
            int num1 = OP[i][1];
            int num2 = OP[i][2];
            if(mode == 1){
                if(num1 != com[nowcom]){
                    if(Q[num2] == 0)Q[num2] = alloc();
                    queue *q = Q[num2];
                    if(q->size == 0){
                        head_pos[num1] = num2 + 1;
                        tail_pos[num1] = num2 + 1;
                        max_pos[num1] = num2 + 1;
                    }else if(q->size > 0){
                        head_pos[num1] = -1;
                        tail_pos[num1] = num2 + 1;
                        if(num1 > q->root[0]){
                            max_pos[num1] = num2 + 1; 
                            max_pos[q->root[0]] = -1;
                        }else max_pos[num1] = -1;
                        tail_pos[q->tail->key] = -1;
                    }
                    if(check(q))enlarge(q);
                    enqueue(q,num1);
                    insertheap(q->root,&q->r_size,num1);
                } else{
                    nowcom++;
                    while(nowcom < times && Continue){
                        int key = com[nowcom];
                        int m = checkqueue(key);
                        if(m == 4)break;
                        if(m == 0){
                            Continue = false;
                            break;
                        } else if(m == 1){
                            int index = head_pos[key] - 1;
                            queue *q = Q[index];
                            q_node *d = dequeue(q,1);
                            pack_state[key] = true;
                            if(q->size > 0){
                                head_pos[q->head->key] = index + 1;
                                if(q->root[0] == key){
                                    extractmax(q->root,&q->r_size);
                                    while(pack_state[q->root[0]]){
                                        extractmax(q->root,&q->r_size);
                                    }
                                    max_pos[q->root[0]] = index + 1;
                                }
                            } else q->r_size = 0;
                            nowcom++;
                            free(d);
                        } else if(m == 2){
                            int index = tail_pos[key] - 1;
                            queue *q = Q[index];
                            q_node *d = dequeue(q,2);
                            pack_state[key] = true;
                            tail_pos[q->tail->key] = index + 1;
                            if(q->root[0] == key){
                                extractmax(q->root,&q->r_size);
                                while(pack_state[q->root[0]]){
                                    extractmax(q->root,&q->r_size);
                                }
                                max_pos[q->root[0]] = index + 1;
                            }
                            nowcom++;
                            free(d);
                        } else if(m == 3){
                            int index = max_pos[key] - 1;
                            queue *q = Q[max_pos[key] - 1];
                            pack_state[key] = true;
                            extractmax(q->root,&q->r_size);
                            while(pack_state[q->root[0]]){
                                extractmax(q->root,&q->r_size);
                            }
                            max_pos[q->root[0]] = index + 1;
                            nowcom++;
                        }
                    }
                } 
            }else if(mode == 2){
                if(Q[num1] != 0){
                    if(Q[num1]->size > 0){
                        if(Q[num2] == 0){
                            Q[num2] = Q[num1];
                        }else{
                            if(Q[num2]->size == 0){
                                free(Q[num2]->root);
                                free(Q[num2]);
                                Q[num2] = Q[num1];
                            }else{
                                head_pos[Q[num1]->head->key] = -1;
                                tail_pos[Q[num1]->tail->key] = -1;
                                max_pos[Q[num1]->root[0]] = -1;
                                tail_pos[Q[num2]->tail->key] = -1;
                                if(Q[num2]->root[0] < Q[num1]->root[0])max_pos[Q[num2]->root[0]] = -1;
                                merge_queue(Q[num1],Q[num2]);
                                if(Q[num2]->r_size >= Q[num1]->r_size){
                                    while(Q[num1]->r_size + Q[num2]->r_size >= Q[num2]->capacity-5)enlarge(Q[num2]);
                                    merge(Q[num2]->root,Q[num1]->root,&Q[num2]->r_size,Q[num1]->r_size);
                                }else{
                                    while(Q[num1]->r_size + Q[num2]->r_size >= Q[num1]->capacity-5)enlarge(Q[num1]);
                                    merge(Q[num1]->root,Q[num2]->root,&Q[num1]->r_size,Q[num2]->r_size);
                                    Q[num2]->root = Q[num1]->root;
                                    Q[num2]->r_size = Q[num1]->r_size;
                                    Q[num2]->capacity = Q[num1]->capacity;
                                    Q[num1]->root = 0;
                                }
                                Q[num2]->size += Q[num1]->size;
                                free(Q[num1]->root);
                                free(Q[num1]);
                            }
                        }
                        head_pos[Q[num2]->head->key] = num2 + 1;
                        tail_pos[Q[num2]->tail->key] = num2 + 1;
                        max_pos[Q[num2]->root[0]] = num2 + 1; 
                    }
                    Q[num1] = 0;
                }
            }
            //for(int l=0;l<L;l++){
                //if(Q[l] != 0){
                    //q_node *ptr=Q[l]->head;
                    //printf("%d 行有%d個",l,Q[l]->size);
                    //if(Q[l]->size > 0){
                        //for(int now = 0;now < Q[l]->size;now++){
                            //printf("%d ",ptr->key);
                            //ptr = ptr->nxt;
                        //}
                        //printf("\n");
                        //for(int now = 0;now < Q[l]->r_size;now++){
                            //printf("%d->",Q[l]->root[now]);
                        //}
                    //}
                //}
                //printf("\n");
            //}
            //printf("\n");
            //for(int now = 0;now < nowcom;now++)printf("%d->",com[now]);
            //printf("\n----------------\n");
            //getchar();
            if(nowcom >= times){
                ans[k] = 1;
                break;
            }
            if(!Continue)break;
        }
        if(k < T-1){
            //for(int d = 0;d < O;d++){
                //OP[d][1] = 0;
                //if(d < L){
                    //if(Q[d] != 0){
                        //free(Q[d]->root);
                        //free(Q[d]);
                        //Q[d] = 0;
                    //}
                //}
            //}
            memset(head_pos,0,sizeof(int)*(N+1));
            memset(tail_pos,0,sizeof(int)*(N+1));
            memset(max_pos,0,sizeof(int)*(N+1));
            memset(pack_state,false,sizeof(bool)*(N+1));
            free(Q);
        }
    }
    for(int i=0;i<T;i++){
        if(ans[i])printf("possible\n");
        else printf("impossible\n");
    }
}