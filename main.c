 #include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

typedef struct Account{
    int idx;
    int sn;
    int mention;
    char id[20];
    char date[30];
};

typedef struct Word{
    int idx;
    wchar_t word[200];
    char date[1000];
    int sn;
};

int compare (const void *a , const void *b);
int compareWd(const void *a, const void *b);
int RBS(const void *a, int nBegin, int nEnd, int nTarget);

int show_interface();
void readfile(const void *a, const void *b, const void **c, int* usernum, int* friendnum, int* wordnum);
void statistics(const void *a, const void *b, const void **c, int *usernum, int *friendnum, int *wordnum, int *graphwh);
void mostword(const void *a, int wordnum);
void mostuser(const void *a, int usernum);
void deleteword(wchar_t *findword, const void *a, const void *b, int *usernum, int *wordnum);
void deleteuser(wchar_t *findword, const void *a, const void *b, const void **c, int *usernum, int *friendnum, int *wordnum, int *graphwh);
void dijikstra(const void *a, const void **b, int* usernum, int* graphwh, int startnode);

int user[100];

int main()
{
    _wsetlocale(LC_ALL, L"korean");

    struct Account *account;
    account = (struct Account *)malloc(5000 * sizeof(struct Account));
    account[0].sn = 0;

    struct Word *word;
    word = (struct Word *)malloc(20000 * sizeof(struct Word));
    word[0].word[0] = 0;

    user[0] = 0;

    int i, n, m, quit = 0;

    short **graph;
    graph = (short **)malloc(5000 * sizeof(short));
    for(n=0;n<5000;n++) {
        graph[n] = (short *)malloc(5000 * sizeof(short));
    }


    int usernum, friendnum, wordnum, graphwh, w=0;
    wchar_t findword[100];
    char findid[20];
    int* list = (int*)malloc(5000 * sizeof(int));
    int* visited = (int*)malloc(5000 * sizeof(int));


    while(!quit) {
        switch(show_interface()){
            case 0:
                readfile(account, word, graph, &usernum, &friendnum, &wordnum);
                graphwh = usernum;
                break;
            case 1:
                if(account[0].sn == 0 || word[0].word[0] == 0) {
                    printf("WARNING: Please Read data files\n\n");
                    break;
                }
                statistics(account, word, graph, &usernum, &friendnum, &wordnum, &graphwh);
                break;
            case 2:
                if(account[0].sn == 0 || word[0].word[0] == 0) {
                    printf("WARNING: Please Read data files\n\n");
                    break;
                }
                mostword(word, wordnum);
                break;
            case 3:
                if(account[0].sn == 0 || word[0].word[0] == 0) {
                    printf("WARNING: Please Read data files\n\n");
                    break;
                }
                mostuser(account, usernum);
                break;
            case 4:
                if(account[0].sn == 0 || word[0].word[0] == 0) {
                    printf("WARNING: Please Read data files\n\n");
                    break;
                }
                printf("Enter Word: ");
                scanf("%ls", &findword);
                finduser(findword, account, word, &usernum, &wordnum);
                break;
            case 5:
                if(account[0].sn == 0 || word[0].word[0] == 0) {
                    printf("WARNING: Please Read data files\n\n");
                    break;
                }
                if(user[0] == 0){
                    printf("WARNING: No users have been searched\n\n");
                    break;
                }
                findfriend(account, graph, &usernum);
                break;
            case 6:
                if(account[0].sn == 0 || word[0].word[0] == 0) {
                    printf("WARNING: Please Read data files\n\n");
                    break;
                }
                printf("Enter Word: ");
                scanf("%ls", &findword);
                deleteword(findword, account, word, &usernum, &wordnum);
                break;
            case 7:
                if(account[0].sn == 0 || word[0].word[0] == 0) {
                    printf("WARNING: Please Read data files\n\n");
                    break;
                }
                printf("Enter Word: ");
                scanf("%ls", &findword);
                deleteuser(findword, account, word, graph, &usernum, &friendnum, &wordnum, &graphwh);
                break;
            case 8:
 /*               for(i=0; i<graphwh; ++i){
                    list[i] = '\0';
                    visited[i] = 0;
                }
                SCC(graph, 0, &graphwh, list, visited, &w);*/
                printf("=========================RESULT========================\n");
                printf("Not fully constructed...\n\n");
                printf("=======================================================\n");
                break;
            case 9:
                if(account[0].sn == 0 || word[0].word[0] == 0) {
                    printf("WARNING: Please Read data files\n\n");
                    break;
                }
                printf("Start id: ");
                scanf("%s", &findid);
                m = -1;
                for(n=0;n<usernum;n++) {
                    if(!strcmp(account[n].id, findid)){
                        m = account[n].idx;
                        break;
                    }
                }
                if(m==-1){
                    printf("=========================RESULT========================\n");
                    printf("WARNING: non-existing ID\n");
                    printf("=======================================================\n\n");
                } else {
                    dijikstra(account, graph, &usernum, &graphwh, m);
                    printf("%d^^", n);
                }
                break;
            case 99: quit = 1;
            default: break;
        }
    }

    return 0;

}

int compare (const void *a , const void *b)
{
    struct Account* ptra = (struct Account*)a;
    struct Account* ptrb = (struct Account*)b;

    if ((int*)ptra->sn > (int*)ptrb->sn) {
        return 1;
    } else if((int*)ptra->sn <(int*)ptrb->sn) {
        return -1;
    } else {
        return 0;
    };
}
int compareWd(const void *a, const void *b)
{
    struct Word* ptra = (struct Word*)a;
    struct Word* ptrb = (struct Word*)b;

    return wcscmp((wchar_t**)ptra->word , (wchar_t**)ptrb->word );
}

int RBS(const void *a, int nBegin, int nEnd, int nTarget)
{
    int nMid = 0, result;

    if( nBegin > nEnd ) {
        return -1;
    }

    nMid = (nBegin+nEnd) / 2;

    struct Account* ptra = (struct Account*)(a + sizeof(struct Account) * nMid);

    if ((int*)ptra->sn > nTarget) {
        return RBS( a, nBegin, nMid-1, nTarget );
    } else if((int*)ptra->sn < nTarget) {
        return RBS( a, nMid+1, nEnd, nTarget );
    } else {
        return nMid;
    }
}

int show_interface(){
    int a;
    printf(" 0. Read data files\n");
    printf(" 1. display statistics\n");
    printf(" 2. Top 5 most tweeted words\n");
    printf(" 3. Top 5 most tweeted users\n");
    printf(" 4. Find users who tweeted a word\n");
    printf(" 5. Find all people who are friends of the above users\n");
    printf(" 6. Delete all mentions of a word\n");
    printf(" 7. Delete all users who mentioned a word\n");
    printf(" 8. Find strongly connected components\n");
    printf(" 9. Find shortest path from a given user\n");
    printf(" 99. Quit\n");
    printf("Select Menu: ");
    scanf("%d", &a);
    return a;
}

void readfile(const void *a, const void *b, const void **c, int* usernum, int *friendnum, int* wordnum){

    struct Account* account = (struct Account*)a;
    struct Word* word = (struct Word*)b;
    short **graph = (short**)c;

    FILE *fp;

    //user insert
    fp = fopen("user.txt", "rt");
    int i;
    char tmp[200];
    wchar_t wtmp[200];

    i = 0;
    while(!feof(fp)) {
        fscanf(fp, "%d\n", &account[i].sn);
        fgets(tmp, 100, fp);
        tmp[strlen(tmp)-1] = '\0';
        strcpy(account[i].date, tmp);
        fgets(tmp, 100, fp);
        tmp[strlen(tmp)-1] = '\0';
        strcpy(account[i].id, tmp);
        fscanf(fp, "\n", &tmp);
        account[i].idx = i;
        account[i].mention = 0;

        ++i;
    }
    fclose(fp);
    *usernum = i;

    qsort(account, i, sizeof(struct Account), compare);

    int n, m;
    for(n=0;n<*usernum;n++){
        for(m=0;m<*usernum;m++){
            graph[n][m] = 0;
        }
    }


    //create friend graph
    fp = fopen("friend.txt", "rt");
    int findedsn1, findedsn2, idx;

    i = 0;
    while(!feof(fp)) {
        fscanf(fp, "%d\n", &idx);
        findedsn1 = account[RBS(account, 0, *usernum-1, idx)].idx;
        fscanf(fp, "%d\n", &idx);
        findedsn2 = account[RBS(account, 0, *usernum-1, idx)].idx;
        if(findedsn1 != -1 && findedsn2 != -1) {
            if(graph[findedsn1][findedsn2] == 1 ){
                i--;
            }
            graph[findedsn1][findedsn2] = 1;
        }
        fscanf(fp, "\n", &idx);

        ++i;
    }
    fclose(fp);
    *friendnum = i;

   //insert word
    fp = fopen("word.txt", "rt");

    i = 0;
    int k;
    while(!feof(fp)) {
        k=0;
        fscanf(fp, "%d\n", &word[i].sn);
        account[RBS(account, 0, *usernum-1, word[i].sn)].mention++;
        fgets(tmp, 100, fp);
        strcpy(word[i].date,tmp);
        fgetws(wtmp, 100, fp);
        for(k=0;k<100;k++) {
            if(wtmp[k]=='\n'){
                wtmp[k]='\0';
                break;
            }
        }
        wcscpy(word[i].word,wtmp);
        fscanf(fp, "\n", &tmp);
        word[i].idx = i;

        ++i;
    }
    fclose(fp);

    *wordnum = i;

    qsort(word, *wordnum, sizeof(word[0]), compareWd);

    printf("=========================RESULT========================\n");
    printf("Completed\n\n");
    printf("Total users: %d\n", *usernum);
    printf("Total friendship records: %d\n", *friendnum);
    printf("Total tweets: %d\n", *wordnum);
    printf("=======================================================\n\n");
}

void statistics(const void *a, const void *b, const void **c, int *usernum, int *friendnum, int *wordnum, int *graphwh){
    double avg_friends, avg_twit;
    int min_friends, max_friends, min_twit, max_twit;
    struct Account* account = (struct Account*)a;
    struct Word* word = (struct Word*)b;
    short **graph = (short**)c;

    avg_friends = (double)*friendnum / (double)*usernum;
    avg_twit = (double)*wordnum / (double)*usernum;

    int i,j, sum=0, min=*graphwh, max=0, tmp=0;

    for(i=0; i<*graphwh; ++i){
        tmp = 0;
        for(j=0; j<*graphwh; ++j){
            if(graph[i][j] == 1) {
                tmp += 1;
            }
        }
        if(tmp < min) {
            min = tmp;
        }
        if(tmp > max) {
            max = tmp;
        }
    }
    min_friends = min;
    max_friends = max;
    min = *graphwh;
    max = 0;
    for(i=0; i<*graphwh; ++i){
        if(account[i].mention < min) {
            min = account[i].mention;
        }
        if(account[i].mention > max) {
            max = account[i].mention;
        }
    }
    min_twit = min;
    max_twit = max;
    printf("=========================RESULT========================\n");
    printf("Average number of friends per user: %.2lf\n", avg_friends);
    printf("Minimum number of friends in 1 user: %d\n", min_friends);
    printf("Maximum number of friends in 1 user: %d\n", max_friends);
    printf("Average number of tweets per user: %.2lf\n", avg_twit);
    printf("Minimum number of tweets in 1 user: %d\n", min_twit);
    printf("Maximum number of tweets in 1 user: %d\n", max_twit);
    printf("=======================================================\n\n");
}

void mostword(const void *a, int wordnum) {
    struct Word* word = (struct Word*)a;

    int now=1, j=0;
    int max1=0, max2=0, max3=0, max4=0, max5=0;
    wchar_t prv[100] = L"";
    wchar_t maxstr1[100] = L"";
    wchar_t maxstr2[100] = L"";
    wchar_t maxstr3[100] = L"";
    wchar_t maxstr4[100] = L"";
    wchar_t maxstr5[100] = L"";

    wcscpy(prv, word[0].word);

    while(j<=wordnum) {
        if(!wcscmp(prv, word[j].word)){
            now++;
        } else {
            if(now>=max1) {
                max5 = max4;
                wcscpy(maxstr5, maxstr4);
                max4 = max3;
                wcscpy(maxstr4, maxstr3);
                max3 = max2;
                wcscpy(maxstr3, maxstr2);
                max2 = max1;
                wcscpy(maxstr2, maxstr1);
                max1 = now;
                wcscpy(maxstr1, prv);
            } else if(now>=max2) {
                max5 = max4;
                wcscpy(maxstr5, maxstr4);
                max4 = max3;
                wcscpy(maxstr4, maxstr3);
                max3 = max2;
                wcscpy(maxstr3, maxstr2);
                max2 = now;
                wcscpy(maxstr2, prv);
            } else if(now>=max3) {
                max5 = max4;
                wcscpy(maxstr5, maxstr4);
                max4 = max3;
                wcscpy(maxstr4, maxstr3);
                max3 = now;
                wcscpy(maxstr3, prv);
            } else if(now>=max4) {
                max5 = max4;
                wcscpy(maxstr5, maxstr4);
                max4 = now;
                wcscpy(maxstr4, prv);
            } else if(now>=max5) {
                max5 = now;
                wcscpy(maxstr5, prv);
            }
            now = 1;
        }
        wcscpy(prv, word[j].word);
        j++;
    }
    printf("=========================RESULT========================\n");
    printf("1. %ls (%d)\n2. %ls (%d)\n3. %ls (%d)\n4. %ls (%d)\n5. %ls (%d)\n", maxstr1,max1,maxstr2,max2,maxstr3,max3,maxstr4,max4,maxstr5,max5);
    printf("=======================================================\n\n");

}

void mostuser(const void *a, int usernum) {
    struct Account* account = (struct Account*)a;

    int max1 = 0, maxidx1 = 0;
    int max2 = 0, maxidx2 = 0;
    int max3 = 0, maxidx3 = 0;
    int max4 = 0, maxidx4 = 0;
    int max5 = 0, maxidx5 = 0;

    int j=0;
    while(j<usernum) {
        if(account[j].mention >= max1) {
            max5 = max4;
            maxidx5 = maxidx4;
            max4 = max3;
            maxidx4 = maxidx3;
            max3 = max2;
            maxidx3 = maxidx2;
            max2 = max1;
            maxidx2 = maxidx1;
            max1 = account[j].mention;
            maxidx1 = j;
        } else if(account[j].mention >= max2) {
            max5 = max4;
            maxidx5 = maxidx4;
            max4 = max3;
            maxidx4 = maxidx3;
            max3 = max2;
            maxidx3 = maxidx2;
            max2 = account[j].mention;
            maxidx2 = j;
        } else if(account[j].mention >= max3) {
            max5 = max4;
            maxidx5 = maxidx4;
            max4 = max3;
            maxidx4 = maxidx3;
            max3 = account[j].mention;
            maxidx3 = j;
        } else if(account[j].mention >= max4) {
            max5 = max4;
            maxidx5 = maxidx4;
            max4 = account[j].mention;
            maxidx4 = j;
        } else if(account[j].mention >= max5) {
            max5 = account[j].mention;
            maxidx5 = j;
        }
        j++;
    }
    user[0] = account[maxidx1].sn;
    user[1] = account[maxidx2].sn;
    user[2] = account[maxidx3].sn;
    user[3] = account[maxidx4].sn;
    user[4] = account[maxidx5].sn;
    user[5] = 0;
    printf("=========================RESULT========================\n");
    printf("1. %s (%d)\n2. %s (%d)\n3. %s (%d)\n4. %s (%d)\n5. %s (%d)\n", account[maxidx1].id,max1,account[maxidx2].id,max2,account[maxidx3].id,max3,account[maxidx4].id,max4,account[maxidx5].id,max5);
    printf("=======================================================\n\n");
}

void finduser(wchar_t *findword, const void *a, const void *b, int *usernum, int *wordnum){
    struct Account* account = (struct Account*)a;
    struct Word* word = (struct Word*)b;

    printf("=========================RESULT========================\n");
    int j=0;
    int i=0;
    while(j<=*wordnum) {
        if(!wcscmp(findword, word[j].word)){
            printf(" - %s\n", account[RBS(account, 0, *usernum-1, word[j].sn)].id);
            user[i++] = word[j].sn;
        }
        j++;
    }
    user[i] = 0;
    if(user[0] == 0) {
        printf("No Result!\n");
    }
    printf("=======================================================\n\n");
}

void findfriend(const void *a, const void **b, int* usernum){

    struct Account* account = (struct Account*)a;
    short **graph = (short**)b;

    printf("=========================RESULT========================\n");
    int i, j=0;
    while(user[j]) {
        printf(" - Friends of %s:\n", account[RBS(account, 0, *usernum-1, user[j])].id);
        for(i=0;i<*usernum;i++) {
            if(graph[RBS(account, 0, *usernum-1, user[j])][i] == 1) {
               printf("     %s\n", account[i].id);
            }
        }
        printf("\n");
        j++;
    }
    printf("=======================================================\n\n");
}

void deleteword(wchar_t *findword, const void *a, const void *b, int *usernum, int *wordnum){
    struct Account* account = (struct Account*)a;
    struct Word* word = (struct Word*)b;

    int j=0, wordnumpre=*wordnum;
    while(j<*wordnum) {
        if(!wcscmp(findword, word[j].word)){
            account[RBS(account, 0, *usernum-1, word[j].sn)].mention -= 1;
            word[j].idx = word[*wordnum-1].idx;
            wcscpy(word[j].word, word[*wordnum-1].word);
            strcpy(word[j].date, word[*wordnum-1].date);
            word[j].sn = word[*wordnum-1].sn;
            *wordnum-=1;
            j--;
        }
        j++;
    }

    qsort(word, *wordnum, sizeof(word[0]), compareWd);

    printf("=========================RESULT========================\n");
    printf("Completed\n\n");
    printf("Deleted %d words\n", wordnumpre-*wordnum);
    printf("=======================================================\n\n");
}

void deleteuser(wchar_t *findword, const void *a, const void *b, const void **c, int *usernum, int *friendnum, int *wordnum, int *graphwh){

    struct Account* account = (struct Account*)a;
    struct Word* word = (struct Word*)b;
    short **graph = (short**)c;

    int usernumpre = *usernum;
    int friendnumpre = *friendnum;
    int wordnumpre = *wordnum;

    int i=0, j=0, k=0, l=0, idxsn, idx;
    while(j<*wordnum) {
        if(!wcscmp(findword, word[j].word)){
            i = RBS(account, 0, *usernum-1, word[j].sn);
            if(i!=-1) {
                idxsn = word[j].sn;
                idx = account[i].idx;
                account[i].idx = account[*usernum-1].idx;
                account[i].sn = account[*usernum-1].sn;
                account[i].mention = account[*usernum-1].mention;
                strcpy(account[i].id, account[*usernum-1].id);
                strcpy(account[i].date, account[*usernum-1].date);
                *usernum-=1;
                for(k=0; k<*graphwh; k++) {
                    if(graph[idx][k] == 1) {
                        *friendnum-=1;
                    }
                    graph[idx][k] = 0;
                }
                for(k=0; k<*graphwh; k++) {
                    if(graph[k][idx] == 1) {
                        *friendnum-=1;
                    }
                    graph[k][idx] = 0;
                }
                l=0;
                while(l<*wordnum) {
                    if(idxsn == word[l].sn){
                        word[l].word[0] = '\0';
                    }
                    l++;
                }
                qsort(account, *usernum, sizeof(struct Account), compare);
            }
        }
        j++;
    }

    j=0;
    while(j<*wordnum) {
        if(word[j].word[0]=='\0'){
            word[j].idx = word[*wordnum-1].idx;
            wcscpy(word[j].word, word[*wordnum-1].word);
            strcpy(word[j].date, word[*wordnum-1].date);
            word[j].sn = word[*wordnum-1].sn;
            *wordnum-=1;
            j--;
        }
        j++;
    }

    qsort(word, *wordnum, sizeof(word[0]), compareWd);

    printf("=========================RESULT========================\n");
    printf("Completed\n\n");
    printf("Deleted %d users\n", usernumpre-*usernum);
    printf("=======================================================\n\n");
}

void dijikstra(const void *a, const void **b, int* usernum, int* graphwh, int startnode)
{
    struct Account* account = (struct Account*)a;
    short **graph = (short**)b;

    int n;

    short **cost;
    cost = (short **)malloc(5000 * sizeof(short));
    for(n=0;n<5000;n++) {
        cost[n] = (short *)malloc(5000 * sizeof(short));
    }

    short *distance;
    distance = (short *)malloc((*graphwh) * sizeof(short));

    short *pred;
    pred = (short *)malloc((*graphwh) * sizeof(short));

    short *visited;
    visited = (short *)malloc((*graphwh) * sizeof(short));

    int count, mindistance, nextnode, i,j;

    for(i=0;i < *graphwh;i++) {
        for(j=0;j < *graphwh;j++) {
            if(graph[i][j]==0) {
                cost[i][j]=32767;
            }
            else if (i == j) {
                cost[i][j]=0;
            }
            else {
                cost[i][j]=graph[i][j];
            }
        }
    }

   for(i=0;i< *graphwh;i++){
      distance[i]=cost[startnode][i];
      pred[i]=startnode;
      visited[i]=0;
   }
   distance[startnode]=0;
   visited[startnode]=1;
   count=1;
   while(count < *graphwh-1){
      mindistance=32767;
      for(i=0;i < *graphwh;i++) {
         if(distance[i] < mindistance&&!visited[i])
         {
            mindistance=distance[i];
            nextnode=i;
         }
      }
      visited[nextnode]=1;
      for(i=0;i < *graphwh;i++)
         if(!visited[i])
            if(mindistance+cost[nextnode][i] < distance[i])
            {
               distance[i]=mindistance+cost[nextnode][i];
               pred[i]=nextnode;
            }
         count++;
   }
    printf("=========================RESULT========================\n");

    char m[20], o[20];

   for(i=0;i < *graphwh;i++) {
      if(i!=startnode)
      {
          m[0] = '\0';
            for(n=0;n<usernum;n++) {
                if(account[n].idx == i){
                    strcpy(m, account[n].id);
                    break;
                }
            }
            if(m[0] != '\0') {
                if(distance[i] == 32767) {
                    printf("Distance of %s = Node non-reachable\n\n", i);
                } else {
                    printf("Distance of %s = %d\n", m, distance[i]);
                    printf("Path : %s", m);
                    j=i;
                    do {
                        j=pred[j];
                        for(n=0;n<usernum;n++) {
                            if(account[n].idx == j){
                                strcpy(o, account[n].id);
                                break;
                            }
                        }
                        printf(" <- %s", o);
                    }
                    while(j!=startnode);
                    printf("\n\n");
                }
            }
        }
    }

    printf("=======================================================\n\n");
}

/*
void SCC(const void **a, int start, int *graphwh, int* list, int* visited, int *w){
    short** graph = (short**) a;

    short **cost;
    int n, k=*w, i=0, tmp1=0, tmp2=0;
    cost = (short **)malloc(5000 * sizeof(short));
    for(n=0;n<5000;n++) {
        cost[n] = (short *)malloc(5000 * sizeof(short));
    }

    DFSvisit(graph, start, graphwh,  list, visited, w);
    for(*w = k-1; *w>= 0; *w--){
        for(i=0; i<*graphwh; ++i){
            tmp1 += graph[i][start];
            tmp2 += visited[i];
    }    tmp1=0;tmp2=0;

        DFStranspose(graph, *w, graphwh, list, visited, w);

    }

    if(tmp1 == tmp2)

    tmp1=0;tmp2=0;

}

void DFSvisit(const void **a, int start, int *graphwh, int* list, int* visited, int *w){
    short**graph = (short**) a;
    int v;
    for(v=0;v<*graphwh;v++){
        if(graph[start][v] ==1 && visited[v] == 0){
            visited[v] = 1;
            DFSvisit(graph, v, graphwh, list, visited, w);
            list[(*w)++] = v;
        }
    }

}

void DFStranspose(const void **a, int start, int *graphwh, int* list, int* visited, int *w){
    short**graph = (short**) a;
    if(graph[*w][start] ==1 && visited[*w] == 0){
        visited[*w] = 1;
        (*w)--;
        DFStranspose(graph, *w, graphwh, list, visited, w);
    }

}
*/
