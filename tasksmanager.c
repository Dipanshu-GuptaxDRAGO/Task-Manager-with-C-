#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// struct def
struct task{
    char task_name[256];
    char start_time_stamp[256];
    char end_time_stamp[256];
    char task_desc[256];
};
// func def
void getstr(char str[]) {
    char ch;
    int i = 0;
    while (i < 256 - 1 && scanf("%c", &ch) == 1 && ch != '\n') {
        if (i == 0 && ch == '\n') {
            continue; 
        }
        str[i] = ch;
        i++;
    }
    str[i] = '\0';
}

//loading of data to working memory
int load(int index,struct task tasks[]){
    FILE *file = fopen("tasks.txt", "r");
    if (file == NULL) {
        perror("Error opening file\n");
        return 0;
    }
    char *lines[200];
    char buffer[256];
    int line_count = 0;

    while (line_count < 200 && fgets(buffer, 256, file) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        lines[line_count] = strdup(buffer);
        line_count++;
    }
    for(int i=0;i<line_count;i++){
        switch (i%4){
            case 0:
                strcpy(tasks[index].task_name,lines[i]);            
                break;
            case 1:
                strcpy(tasks[index].start_time_stamp,lines[i]);
                break;
            case 2:
                strcpy(tasks[index].end_time_stamp,lines[i]);
                break;
            case 3:
                strcpy(tasks[index].task_desc,lines[i]);
                index++;
                break;
        }
    }
    printf("%d loaded\n",index);
    fclose(file);
    return index;
}
//sorting
void sort(int index,struct task tasks[]){
    for (int i=0;i<index-1;i++){
        for(int j=0;j<index-1;j++){
            struct task temp_storage;
            if (strcmp(tasks[j].start_time_stamp,tasks[j+1].start_time_stamp)>0){
                printf("got!\n");
                temp_storage=tasks[j];
                tasks[j]=tasks[j+1];
                tasks[j+1]=temp_storage;
            }
        }
    }
}
//saving
void save(int index,struct task tasks[]){
    FILE *file = fopen("tasks.txt", "w");
    if (file == NULL) {
        perror("Error opening file\n");
    }
    int cnt=0;
    while (cnt<index) {
        for (int i=0;i<4;i++){
            switch (i){
                case 0:
                    fputs(strcat(tasks[cnt].task_name,"\n"),file);
                    break;
                case 1:
                    fputs(strcat(tasks[cnt].start_time_stamp,"\n"),file);
                    break;
                case 2:
                    fputs(strcat(tasks[cnt].end_time_stamp,"\n"),file);
                    break;
                case 3:
                    fputs(strcat(tasks[cnt].task_desc,"\n"),file);
                    break;
            }
        }
        cnt++;
    }

    fclose(file);
}
//viewing
void view(int index,struct task tasks[]){
    for (int i=0; i<index;i++){
        printf("________________________________________________________________________________\n");
        printf("Task: %d",i+1);
        printf("\n--------------------------------------------------------------------------------\n");
        printf("%s\n",tasks[i].task_name);
        printf("%s                                          %s\n",tasks[i].start_time_stamp,tasks[i].end_time_stamp);
        printf("\n%s\n",tasks[i].task_desc);
    }
    printf("________________________________________________________________________________\n");
}
//addition of new task
int add(int index,struct task tasks[]){
    printf("Task_Name(*No ' ' allowed): ");
    scanf("%s",tasks[index].task_name);
    while (getchar() != '\n');
    printf("Starting_Time:(yyyy-mm-dd hh-mm-ss) ");
    getstr(tasks[index].start_time_stamp);
    int egs=0;
    while (egs==0){
        printf("Ending_Time:(yyyy-mm-dd hh-mm-ss) ");
        getstr(tasks[index].end_time_stamp);
        if(strcmp(tasks[index].end_time_stamp,tasks[index].start_time_stamp)>0){
            egs=1;
        }else{
            printf("Error: Ending_Time < Starting_Time\n");
        }
    }
    printf("Task_Description(Max 256 entities): ");
    getstr(tasks[index].task_desc);
    index++;
    printf("Added Successfully!!\n");
    return index;
}
//marking the task completed and deleting
int mark(int index,struct task tasks[]){
    printf("To mark a task complete and to delete it type the name of it correctly");
    printf("Task_Name: ");
    char *name;
    scanf("%s",name);
    int indx=-1;
    for (int i=0;i<index;i++){
        if(strcmp(name,tasks[i].task_name)==0){
            indx=i;
            for (int i=indx;i<index-1;i++){
                tasks[i]=tasks[i+1];
            }
            index--;
            return index;
        }
    }
    if (indx==-1){
        printf("Task Not Found!!\n");
        return index;
    }else{
        printf("Marked Completed!!\n");
    }
}
//main func
int main(){
    int index=0;
    struct task tasks[50];
    index= load(index,tasks);
    sort(index,tasks);
    //Intro_Dialougr_Box
    printf("Task Manager 1.0\n");
    int loop=1;
    //Loop
    while(loop==1){
        printf("1) View Uncompleted Tasks | 2) Add Task | 3) Mark Tasks Completed(Delete)\n");
        printf("0) Exit\n");
        int ch;
        printf(": ");
        scanf("%d",&ch);
        switch (ch){
            case 0:
                loop=0;
                break;
            case 1:
                view(index,tasks);
                break;
            case 2:
                index=add(index,tasks);
                sort(index,tasks);
                break;
            case 3:
                index=mark(index,tasks);
                break;
            default:
            printf("Invalid Choice!!\n");
                break;
        }
    }
    //Saving and exiting
    save(index,tasks);
    printf("Sucessfully exited!");

    return 0;
}