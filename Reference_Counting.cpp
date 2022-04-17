#include<iostream>
using namespace std;
#define HEAP_SIZE  8

class heap_object
{
public:
    int count;
    int val;//value of heap object
    heap_object *link1;
    heap_object *link2;
    heap_object *link3;

    heap_object(int x)
    {
        val=x;
        count=0;
        link1=NULL;
        link2=NULL;
        link3=NULL;
    }
};

class root
{
public:
    heap_object *links;
};
//-----------------------------------------------------------------------------------------------------
//Function Declarations
void init_heap(root &root1,root& root2);

void display_heapobj(heap_object *obj);
void display_root(root r);
void display(heap_object **H);

void garbage_collect(heap_object **H);
void test();

//-----------------------------------------------------------------------------------------------------
//INITIALIZATION OF HEAP GIVEN IN QUESTION
heap_object *HEAP[HEAP_SIZE];
void init_heap(root &root1,root& root2)
{
    int vertex[]={5,1,2,9,10,7,8,3};//8 vertices graph

    //Storing all the HEAP_OBJECTS in the HEAP
    for(int i=0;i<HEAP_SIZE;i++)
    {
        heap_object *temp_obj=new heap_object(vertex[i]);
        HEAP[i]=temp_obj;
    }

    //MAKING CONNECTIONS AS GIVEN IN DIAGRAM
    root1.links=HEAP[0];
    root2.links=HEAP[1];
    HEAP[0]->count+=1;
    HEAP[1]->count+=1;

    HEAP[0]->link1=HEAP[1];
    HEAP[1]->count+=1;

    HEAP[1]->link1=HEAP[2];
    HEAP[2]->count+=1;
    HEAP[1]->link2=HEAP[3];
    HEAP[3]->count+=1;
    HEAP[1]->link3=HEAP[4];
    HEAP[4]->count+=1;

    HEAP[5]->link1=HEAP[1];
    HEAP[1]->count+=1;
    HEAP[5]->link2=HEAP[6];
    HEAP[6]->count+=1;
    
    HEAP[6]->link1=HEAP[4];
    HEAP[4]->count+=1;

    HEAP[7]->link2=HEAP[6];
    HEAP[6]->count+=1;
    HEAP[7]->link3=HEAP[4];
    HEAP[4]->count+=1;
}

//-----------------------------------------------------------------------------------------------------
//DISPLAY HELPER FUNCTIONS
void display_heapobj(heap_object *obj)
{
    if(obj!=NULL)
    {
        cout<<" "<<obj->val<<" ("<<obj->count<<") ";
        if(obj->link1!=NULL || obj->link2!=NULL || obj->link3!=NULL)
        {
            cout<<"-> { ";
            display_heapobj(obj->link1);
            display_heapobj(obj->link2);
            display_heapobj(obj->link3);
            cout<<" } ";

        }
    }
}
void display_root(root r)
{
    cout<<"Root->";
    display_heapobj(r.links);
    cout<<endl;
}
void display(heap_object **H)
{
    for(int i=0;i<HEAP_SIZE;i++)
    {
        if(H[i]!=NULL)
        {
            display_heapobj(H[i]);
            cout<<endl;
        }
    }
}
//-----------------------------------------------------------------------------------------------------
int freq=0;
void garbage_collect(heap_object **H)
{
    // cout<<"COLLECTING GARBAGE "<<++freq<<"th time\n";
    int collected=0;
    for(int i=0;i<HEAP_SIZE;i++)
    {
        if(H[i]!=NULL)
        {
            if(H[i]->count==0)
            {
                if(H[i]->link1!=NULL)
                {
                    H[i]->link1->count-=1;
                    H[i]->link1=NULL;//AS PARENT IS DISCONNECTED WE DISCONNECT THE CHILD 
                }
                if(H[i]->link2!=NULL)
                {
                    H[i]->link2->count-=1;
                    H[i]->link2=NULL;
                }
                if(H[i]->link3!=NULL)
                {
                    H[i]->link3->count-=1;
                    H[i]->link3=NULL;
                }
                delete H[i];
                H[i]=NULL;
                collected=1;
            }
        }
    }

    if(collected)//IF GARBAGE IS COLLECTED CALLING ONCE AGAIN TO ENSURE NO GARBAGE IS LEFT BEHIND
        garbage_collect(H);
}

//-----------------------------------------------------------------------------------------------------
//TESTER FUNCTION TO TEST GARBAGE COLLECTION
void test()
{
    root A,B;
    init_heap(A,B);
    cout<<"-----------------------------------------\n";
    cout<<"\tHEAP MANAGEMENT SCHEME\n";
    cout<<"Refrence counting Method\n";
    cout<<"Displaying heap before performing garbage collection\n";
    display(HEAP);
    cout<<"-----------------------------------------\n";
    cout<<"Displaying heap which is Connected to ROOTS\n";
    display_root(A);
    display_root(B);
    cout<<"-----------------------------------------\n";
    cout<<"PERFORMING GARBAGE COLLECTION...\n";
    garbage_collect(HEAP);
    cout<<"Displaying heap after performing garbage collection\n";
    display(HEAP);
    cout<<"-----------------------------------------\n";
    cout<<"Displaying heap which is Connected to ROOTS\n";
    display_root(A);
    display_root(B);

}
//-----------------------------------------------------------------------------------------------------
int main()
{
    test();
    return 0;
}