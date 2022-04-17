// Usefull Libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 100
#define MAX_END 1999

//Combined structure definition for list
typedef struct List_tag
{
    struct List_tag *next;
    struct List_tag *prev;
    char tag[MAX_LEN];//name of variable to allocate memory
    int start;
    int end;
    int isFree;//if this is 1 it is a Freelist else it is Allocated list
}List;

typedef struct heap_tag
{
    List *lptr;
}Heap;

/* -------------------------------------------------------------------------------------------------- */
// DECLARATION OF THE FUNCTIONS USED IN ASSIGNMENT
List *Allocated_listNodemaker(int start,int end,char name[]);
List *Free_listNodemaker(int start,int end);

void Allocate(Heap *hp,int size,char name[]);
void Free(Heap *hp,char name[]);

int Check_ListEmpty(Heap *hp);
int Check_ListFull(Heap *hp);
int Check_NamePresent(Heap *hp,char name[]);

void printAllocated_list(Heap *hp);
void printFree_list(Heap *hp);
void printBOTHLists(Heap *hp);
/* -------------------------------------------------------------------------------------------------- */
List *Allocated_listNodemaker(int start,int end,char name[])
{
    List *nptr;
	nptr = (List*)malloc(sizeof(List));
    if(nptr!=NULL) {
        nptr->start = start;
    	nptr->end = end;
    	strcpy(nptr->tag, name);
        nptr->isFree = 0;//this means that it is allocatedlist node
    	nptr->next = NULL;
        nptr->prev = NULL;
    }
	return nptr;
}

List *Free_listNodemaker(int start,int end)
{
    List *nptr;
    nptr = (List*)malloc(sizeof(List));
    if(nptr!=NULL) {
        nptr->start = start;
        nptr->end = end;
        nptr->isFree = 1;//it is freelist Node
        nptr->next = NULL;
        nptr->prev = NULL;
    }
    return nptr;
}

void Allocate(Heap *hp,int size,char name[])
{
    List *lptr=hp->lptr;
    if(lptr==NULL)
    {
        printf("MEMORY CANT BE ALLOCATED\n");
    }
    else
    {
        List *nptr=lptr,*ptr=NULL;
        int flag=0;

        while(nptr!=NULL && flag==0)
        {
            if(nptr->isFree && (nptr->end - nptr->start + 1)>=size)
            {
                ptr=nptr;
                flag=1;
            }
            nptr=nptr->next;
        }
        int index;

        if(ptr!=NULL)//if found place to allocate memory
            index=ptr->start + size -1;
        if(ptr==NULL || (ptr->next==NULL && (index > MAX_END)))
            printf("NO SPACE AVAILABLE!!!\n");

        else
        {
            List *aptr=Allocated_listNodemaker(ptr->start,index,name);
            if(aptr==NULL)
                printf("HEAP IS FULL\n");
            else
            {
                if(index==ptr->end)
                {
                   lptr = aptr;
                    if(ptr->prev!=NULL) {
                        ptr->prev->next = lptr;
                        lptr->prev = ptr->prev;
                    }
                    if(ptr->next!=NULL) {
                        ptr->next->prev = lptr;
                        lptr->next = ptr->next;
                    }
                    free(ptr);
                }
                else 
                {
                    ptr->start = index+1;
                    lptr = aptr;
                    if(ptr->prev!=NULL) {
                        ptr->prev->next = lptr;
                        lptr->prev = ptr->prev;
                    }
                    lptr->next = ptr;
                    ptr->prev = lptr;
                }
                printf("SPACE IS ALLOCATED SUCCESSFULLY!!\n");
                while(lptr->prev != NULL) {
                    lptr = lptr->prev;
                }
                hp->lptr = lptr;
            }

        }
         
    }
}

void Free(Heap *hp,char name[])
{
    List *lptr=hp->lptr,*aptr=NULL;

    if(Check_ListEmpty(hp))
        printf("NO SPACE IS ALLOCATED\n");
    else
    {
        int found=0;
        while(lptr!=NULL && !found)
        {
            if(!(lptr->isFree) && strcmp(lptr->tag,name)==0)
            {
                aptr=lptr;
                found=1;
            }
            lptr=lptr->next;
        }
        if(aptr==NULL)//element not found
        {
            printf("There is no element with given NAME\n");
        }
        else
        {
            List *ptr;
            aptr->isFree=1;//make it as a free Node 
            //Merging and Making apporopriate links
            if(aptr->prev!=NULL && aptr->prev->isFree) {
                ptr = aptr->prev;
                aptr->start = ptr->start;
                if(ptr->prev != NULL) ptr->prev->next = aptr;
                aptr->prev = ptr->prev;
                free(ptr);
            }
            //Merging and Making apporopriate links
            if(aptr->next!=NULL && aptr->next->isFree) {
                ptr = aptr->next;
                aptr->end = ptr->end;
                if(ptr->next != NULL) ptr->next->prev = aptr;
                aptr->next = ptr->next;
                free(ptr);
            }
            lptr = aptr;
            while(lptr->prev != NULL) {
                lptr = lptr->prev;
            }
            printf("\nSPACE IS FREED!!\n");
            hp->lptr = lptr;
        }
    }
}

int Check_ListEmpty(Heap *hp)
{
    int retval = 1;
    List *lptr = hp->lptr;
    while(lptr!=NULL && lptr->isFree) {
        lptr = lptr->next;
    }
    if(lptr!=NULL)
        retval=0;
    return retval;
}

int Check_ListFull(Heap *hp)
{
    int retval = 1;
    List *lptr = hp->lptr;
    while(lptr!=NULL && !(lptr->isFree)) {
        lptr = lptr->next;
    }
    if(lptr!=NULL)
        retval=0;
    return retval;
}

void printAllocated_list(Heap *hp)
{
    if(Check_ListEmpty(hp))
        printf("NO SPACE ALLOCATED\n");
    else
    {
        List *ptr = hp->lptr;
        printf("START\t\tEND\t\tSIZE\t\tTAG_NAME\n");
		while(ptr!=NULL) {
			if(!(ptr->isFree))
				printf("%d\t\t%d\t\t%d\t\t%s\n", ptr->start, ptr->end, ptr->end-ptr->start+1, ptr->tag);
			ptr = ptr->next;
		}
    }
}

void printFree_list(Heap *hp)
{
    if(Check_ListFull(hp))
        printf("NO FREE SPACE\n");
    else
    {
        List *ptr = hp->lptr;
        printf("START\t\tEND\t\tSIZE\n");
		while(ptr!=NULL) {
			if(ptr->isFree)
				printf("%d\t\t%d\t\t%d\n", ptr->start, ptr->end, ptr->end-ptr->start+1);
			ptr = ptr->next;
		}
    }
}

void printBOTHLists(Heap *hp)
{
    printf("\t\tCURRENT CONDITION\n");
    printf("_______________________________________________________________");
    printf("\nAllocated list is : \n");
    printAllocated_list(hp);
    printf("---------------------------------------------------------------");
    printf("\nFree list is : \n");
    printFree_list(hp);
    printf("\n");
}

int Check_NamePresent(Heap *hp,char name[])
{
    int found=0;
    List *ptr=hp->lptr;
    while(ptr && !found)
    {
        if(!(ptr->isFree) && strcmp(ptr->tag,name)==0)
        {
            found=1;
        }
        ptr = ptr->next;
    }
    return found;
}

int main()
{
    Heap personal_heap;
    Heap *pptr=&personal_heap;
    List *nptr=Free_listNodemaker(0,MAX_END);

    if(nptr==NULL)
        printf("NO SPACE IN HEAP!!\n");
    else
    {
        char target[MAX_LEN];
        char name[MAX_LEN];//name of variable to allocate memory
        pptr->lptr=nptr;
        int flag=0,n,input;
        do
        {
            printf("________________________________________________________________________________\n");
            printf("->The Total heap Space is %d\n",MAX_END + 1);
            printf("--------------------------------------------------------------------------------\n");
            printf(" 1  - Allocate Memory\n");
            printf(" 2  - Free Memory\n");
            printf(" .. - Any other key to EXIT\n");
            printf("--------------------------------------------------------------------------------\n");
            printf("Enter the CHOICE you want to Execute\n");
            scanf("%d", &input);
            switch(input)
            {
                case 1:
                        printf("\nEnter Name for allocated block\n->");
                        scanf("%s",name);
                        printf("Enter size to be allocate\n->");
                        scanf("%d",&n);
                        if(Check_NamePresent(pptr,name))
                            printf("Variable name already exists.\n");
                        else
                            Allocate(pptr,n,name);
                        printBOTHLists(pptr);
                        break;
                case 2:
                        printf("Enter the tag of the Block which you want to delete\n->");
                        scanf("%s",target);
                        Free(pptr,target);
                        printBOTHLists(pptr);
                        break;
                default:
                        printf("You have successfully exited");
                        flag=1;
            }
        }while(flag==0);
    }
    return 0;
}