#include<iostream>
using namespace std;
#define HEAP_SIZE  8

class heap_object
{
public:
    bool mark_bit;//Mark 1 if Marked and Mark 0 is nt Marked
    int val;//value of heap object
    heap_object *link1;
    heap_object *link2;
    heap_object *link3;

    heap_object(int x)
    {
        val=x;
        mark_bit=false;
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
//Function Declaration
//initialize heap
void init_heap(root &root1,root& root2);

//Mark and Sweep Fnxs
void mark(heap_object *obj); 
void mark_root(root r);
void sweep(heap_object **H);
void garbage_collect(root &r1,root &r2,heap_object **H);

//display
void display_heapobj(heap_object *obj);
void display_root(root r);
void display(heap_object **H);

//testing the garbage collector
void test();

//-----------------------------------------------------------------------------------------------------
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

    HEAP[0]->link1=HEAP[1];

    HEAP[1]->link1=HEAP[2];
    HEAP[1]->link2=HEAP[3];
    HEAP[1]->link3=HEAP[4];
    
    HEAP[5]->link1=HEAP[1];
    HEAP[5]->link2=HEAP[6];

    HEAP[6]->link1=HEAP[4];
    HEAP[7]->link2=HEAP[6];
    HEAP[7]->link3=HEAP[4];

}
//-----------------------------------------------------------------------------------------------------
//Using Constant Space
void mark(heap_object *obj) 
{
    heap_object *traverse_ptr=obj;
    heap_object *prev=NULL,*curr=NULL;

    while(traverse_ptr!=NULL)
    {
        if(!traverse_ptr->mark_bit)
        {
            traverse_ptr->mark_bit=true;
        }

        if(traverse_ptr->link1!=NULL && !traverse_ptr->link1->mark_bit)
        {
            prev=curr;
            curr=traverse_ptr;
            traverse_ptr=traverse_ptr->link1;
        }
        else if(traverse_ptr->link2!=NULL && !traverse_ptr->link2->mark_bit)
        {
            prev=curr;
            curr=traverse_ptr;
            traverse_ptr=traverse_ptr->link2;
        }
        else if(traverse_ptr->link3!=NULL && !traverse_ptr->link3->mark_bit)
        {
            prev=curr;
            curr=traverse_ptr;
            traverse_ptr=traverse_ptr->link3;
        } 
        else
        {
            traverse_ptr=curr;
            curr=prev;
            prev=NULL;
        }
    }
}

void mark_root(root r)
{
    mark(r.links);
}
void sweep(heap_object **H)
{
    cout<<"CURRENT STATUS:\n";
    cout<<"SWEEP PHASE....\n\n";
    for(int i=0;i<HEAP_SIZE;i++)
    {
        if(H[i]!=NULL)
        {
            if(H[i]->mark_bit==false)
            {
                //DISCONNECT IT
                H[i]->link1=NULL;
                H[i]->link2=NULL;
                H[i]->link3=NULL;
                delete H[i];
                H[i]=NULL;
            }
        }
    }
}

void garbage_collect(root &r1,root &r2,heap_object **H)
{
    cout<<"CURRENT STATUS:\n";
    cout<<"MARKING PHASE....\n\n";
    mark_root(r1);
    mark_root(r2);
    cout<<"MARKING PHASE DONE....\n\n";
    sweep(H);
}
//-----------------------------------------------------------------------------------------------------

void display_heapobj(heap_object *obj)
{
    if(obj!=NULL)
    {
        cout<<" "<<obj->val<<" ";
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
void test()
{
    root A,B;
    init_heap(A,B);
    cout<<"-----------------------------------------\n";
    cout<<"\tHEAP MANAGEMENT SCHEME\n";
    cout<<"Mark Sweep Method(using constant space)\n";
    cout<<"Displaying heap before performing garbage collection\n";
    display(HEAP);
    cout<<"-----------------------------------------\n";
    cout<<"Displaying heap which is Connected to ROOTS\n";
    display_root(A);
    display_root(B);
    cout<<"-----------------------------------------\n";
    cout<<"PERFORMING GARBAGE COLLECTION...\n";
    garbage_collect(A,B,HEAP);
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