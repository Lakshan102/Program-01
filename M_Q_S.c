#include<stdio.h>
#include<stdlib.h>

#define MAX 100

typedef struct process{
    int P_ID;
    int priority;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
}Process;

int size_q0=0,size_q1=0,size_q2=0,size_q3=0; 
int total_time=0;
int switch_time=20;
Process q0[MAX],q1[MAX],q2[MAX],q3[MAX];

void enqueue(Process *q,int *index,Process P){
    q[*index]=P;
    (*index)++;
}

Process dequeue(Process *q,int *index){
    int i;
    Process temp=q[0];
    (*index)--;
    for(i=0;i<*index;i++){
        q[i]=q[i+1];
    }
    return temp;
}
void print_info(Process *p){
    printf("| %-5d | %-12d | %-15d | %-16d | %-16d | %-14s |\n", p->P_ID, p->priority, p->burst_time, p->waiting_time, p->turnaround_time,"Success");
}

void Round_Robin(Process *q,int *index,int quantem_time,int switch_time,int *tt){
    while(switch_time !=0 && *index>0){
        if(switch_time >= quantem_time){
            Process temp=dequeue(q,index);
            if(temp.remaining_time>=quantem_time){
                switch_time=switch_time-quantem_time;
                temp.remaining_time=temp.remaining_time-quantem_time;
                if (temp.remaining_time==0){
                    *tt=*tt+quantem_time;
                    temp.turnaround_time=(*tt);
                    temp.waiting_time=*tt-temp.burst_time;
                    print_info(&temp);
                }else{
                    *tt=*tt+quantem_time;
                    enqueue(q,index,temp);
                }
            }else{
                switch_time=switch_time-temp.remaining_time;
                *tt=*tt+temp.remaining_time;
                temp.remaining_time=0;
                temp.turnaround_time=*tt;
                temp.waiting_time=*tt-temp.burst_time;
                print_info(&temp);
            }
            
        }else{
            if(q[0].remaining_time<=switch_time){
                switch_time=switch_time-q[0].remaining_time;
                *tt=*tt+q[0].remaining_time;
                q[0].turnaround_time=*tt;
                q[0].waiting_time=*tt-q[0].burst_time;
                print_info(&q[0]);
                dequeue(q,index);
            }else{
                q[0].remaining_time=q[0].remaining_time-switch_time;
                *tt=*tt+switch_time;
            }
        }
    }
}


void Shortest_Job_First(Process *q,int *index,int switch_time,int *tt){
    while(switch_time !=0 && *index>0){
        int sjf=0;
        for(int i=0;i<*index;i++){
            if(q[i].remaining_time < q[sjf].remaining_time){
                sjf=i;
            }
        }
        Process temp=q[0];
        q[0]=q[sjf];
        q[sjf]=temp;
        if(q[0].remaining_time<=switch_time){
            switch_time=switch_time-q[0].remaining_time;
            *tt=*tt+q[0].remaining_time;
            q[0].turnaround_time=*tt;
            q[0].waiting_time=*tt-q[0].burst_time;
            print_info(&q[0]);
            q[0].remaining_time=0;
            dequeue(q,index);
        }else{
            q[0].remaining_time=q[0].remaining_time-switch_time;
            *tt=*tt+switch_time;
            switch_time=0;
        }
    }
}

void First_Come_First_Serve(Process *q,int *Index,int switch_time,int *tt){
    while(switch_time !=0 && *Index>0){
        if(q[0].remaining_time<=switch_time){
            *tt=*tt+q[0].remaining_time;
            switch_time=switch_time-q[0].remaining_time;
            q[0].remaining_time=0;
            q[0].turnaround_time=*tt;
            q[0].waiting_time=*tt-q[0].burst_time;
            print_info(&q[0]);
            dequeue(q,Index);
        }else{
            q[0].remaining_time=q[0].remaining_time-switch_time;
            *tt=*tt+switch_time;
            switch_time=0;
        }
    }
}
int main(){
    int p,q_time;
    printf("\n============================================== I N P U T S ==========================================================\n\n");
    printf("Enter the no. of process you want to enter : ");
    scanf("%d",&p);
    for(int i=0;i<p;i++){
        Process new;
        new.P_ID=i+1;
        printf("Enter process no.%d Burst time : ",new.P_ID);
        scanf("%d",&new.burst_time);
        printf("Enter process no.%d priority Number(0-3) : ",new.P_ID);
        scanf("%d",&new.priority);
        new.remaining_time=new.burst_time;

        switch (new.priority)
        {
        case 0:
            enqueue(q0,&size_q0,new);
            break;
        case 1:
            enqueue(q1,&size_q1,new);
            break;
        case 2:
            enqueue(q2,&size_q2,new);
            break;
        case 3:
            enqueue(q3,&size_q3,new);
            break;
        default:
            printf("Invalid priority number!!");
            break;
        }
        printf("\n");
    }
    printf("Enter the Quantem time in the round Robbine Architecture : ");
    scanf("%d",&q_time);
    printf("Processor's switch time : 20\n");
    printf("*****All times away in seconds*****");     
    printf("\n\n===================================================== R E S U L T S =====================================================\n\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-12s | %-15s | %-16s | %-14s  | %-14s |\n", "P_ID", "Priority", "Burst Time", "Waiting Time", "Turnaround Time","Situation");
    printf("|-------|--------------|-----------------|------------------|------------------|----------------|\n");
    while(size_q0>0 || size_q1>0 || size_q2>0 || size_q3>0){
        if(size_q0>0){
            Round_Robin(q0,&size_q0,q_time,switch_time,&total_time);
        }
        if(size_q1>0){
            Shortest_Job_First(q1,&size_q1,switch_time,&total_time);
        }
        if(size_q2>0){
            Shortest_Job_First(q2,&size_q2,switch_time,&total_time);
        }
        if(size_q3>0){
            First_Come_First_Serve(q3,&size_q3,switch_time,&total_time);
        }
    }
    printf("-------------------------------------------------------------------------------------------------\n");
    printf("!!PROCESSES ARE SUCCESSFUL!!\n");
   


    return 0;
}