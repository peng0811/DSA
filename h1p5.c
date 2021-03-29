#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct node{
    int num;
    struct node *nxt;
    struct node *fnt;
}node;

typedef struct stack
{
    node *top;
    node *head;
}stack;

node *alloc(int data,node *next){
    node *s=malloc(sizeof(node));
    s->num=data;
    s->nxt=next;
    s->fnt=NULL;
    return s;
}

bool Isempty(stack *s){
    return (s->top == NULL);
}


void push(stack *s,node *l){
    if(Isempty(s)){
        s->top=l;
        s->head=l;
    }
    else{
        (s->top)->fnt=l;
        (s->top)=l;
    }
}

void pop(stack *s){
    node *old=(s->top);
    (s->top)=(s->top)->nxt;
    if((s->top)==NULL)s->head=NULL;
    else if((s->top)->nxt == old){
        s->top->nxt = s->top->fnt;
        s->top->fnt = NULL;
    }
    else if((s->top)->nxt != old){
        s->top->fnt = NULL;
    }
    memset(old,0,sizeof(node));
    free(old);
}
    
int migrate(stack *s1,stack *s2){
    if(s1->head != s1->top){ 
        s1->top->fnt = s1->top->nxt;
        s1->top->nxt = s2->top;
        if(!Isempty(s2))s2->top->fnt = s1->top;
        else if(Isempty(s2)) s2->head = s1->top;
        s1->head->nxt = s1->head->fnt;
        s1->head->fnt = NULL;
        s2->top = s1->head;
        s1->head = NULL;
        s1->top = NULL;
    }
    else{
        s1->top->nxt = s2->top;
        if(!Isempty(s2))s2->top->fnt = s1->top;
        else if(Isempty(s2))s2->head = s1->top;
        s2->top = s1->top;
        s1->top = NULL;
        s1->head = NULL;
    }
}


int main(){
    char s[100];
    int i;
    int stack_row[2];// 0 num of stack, 1 num of operation
    int input[5]={0};
    stack **S=0;
    scanf("%d %d",&stack_row[0],&stack_row[1]);
    S = malloc(sizeof(stack *)*(stack_row[0]+2));
    for(i=0;i<stack_row[0];i++){
        S[i] = malloc(sizeof(stack));
        S[i]->top=NULL;
        S[i]->head=NULL;
    }
    for(i=0;i<stack_row[1];i++){
        scanf("%s %d %d",s,&input[1],&input[2]);
        int L=input[1];
        int N=input[2];
        switch(s[0]){
            case 'e' :push(S[L],alloc(N,S[L]->top));
                        break;
            case 'l' :if(!Isempty(S[L]))pop(S[L]);
                        break;
            case 'm' :if(!Isempty(S[L]))migrate(S[L],S[N]);
                        break;
        }
        
    }
    for(int j=0;j<stack_row[0];j++){
        node *last=NULL;
        while(S[j]->head != NULL){
            printf("%d ",S[j]->head->num);
            if(S[j]->top == S[j]->head)break;
            else if(S[j]->head->nxt == last){
                last = S[j]->head;
                S[j]->head = S[j]->head->fnt;
            }
            else if(S[j]->head->nxt != last){
                last = S[j]->head;
                S[j]->head = S[j]->head->nxt;
            }
        }
        printf("\n");
    }
    free(S);
    return 0;
}