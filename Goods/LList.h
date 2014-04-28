#include"List.h"
#include"Link.h"
#include<assert.h>

template <typename E> class LList: public List<E> {
private:
    Link<E>* head;
    Link<E>* tail;
    Link<E>* curr;
    int cnt;
    void init()
    {

        curr=tail=head=new Link<E>;

        cnt=0;
    }
    void removeall()
    {

        while(head!=NULL){
            curr=head;
            head=head->next;
            delete curr;
        }
    }
public:
    LList(){init();}
    ~LList(){removeall();}
    void clear(){removeall();init();}
    void insert(const E& it){
        curr->next=new Link<E>(it,curr->next);
        if(head==curr) tail=curr->next;
        cnt++;

    }
    void append(const E& it)
    {

        tail=tail->next=new Link<E>(it,NULL);
        cnt++;
    }
    E remove()
    {
        assert(curr->next!=NULL);
        E it=curr->next->element;
        Link<E>* temp=curr->next;
        if(tail==curr->next) tail=curr;
        curr->next=curr->next->next;
        delete temp;
        cnt--;
        return it;

    }
    void moveToStart()
    {

        curr=head;
    }
    void moveToEnd()
    {
        curr=tail;
    }
    void prev()
    {
        if(curr==head) return;
        Link<E>* temp=head;
        while(temp->next!=curr)
        {

            temp=temp->next;
        }
        curr=temp;
    }
    void next()
    {
        if(curr!=tail) curr=curr->next;
    }
    int length()const {return cnt;}
    int currPos()const {
        Link<E>* temp=head;
        int i;
        for(i=0;curr!=temp;i++)
        {
            temp=temp->next;
        }
        return i;
    }
    void moveToPos(int pos)
    {

        assert((pos>=0)&&(pos<=cnt));
        curr=head;
        for(int i=0;i<pos;i++)
            curr=curr->next;
    }
     E& getValue() const
    {
        assert(head->next!=NULL);
        return curr->next->element;
    }

};
