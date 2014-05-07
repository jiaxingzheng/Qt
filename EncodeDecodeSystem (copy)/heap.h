#ifndef HEAP_H
#define HEAP_H
// Heap class
template <typename E> class heap {
private:
    E* Heap;
    int maxsize;
    int n;

    void siftdown(int pos){
        while (!isLeaf(pos)) {
            int j = leftchild(pos);
            int rc = rightchild(pos);
            if((rc <n) && prior(Heap[rc],Heap[j]))
                    j=rc;
            if(prior(Heap[pos],Heap[j]))
                return;
            swap(Heap,pos,j);
            pos = j;
        }
    }

public:
    heap(E* h,int num,int max)
    {
        Heap = h;
        n = num;
        maxsize = max;
        buildHeap();
    }
    int size() const
    {
        return n;
    }
    bool isLeaf(int pos) const
    {
        return (pos >= n/2) && (pos < n);
    }
    int leftchild(int pos) const
    {
        return 2*pos + 1;
    }
    int rightchild(int pos) const
    {
        return 2*pos + 2;
    }
    int parent(int pos) const
    {
        return (pos-1)/2;
    }

    void buildHeap()
    {
        for(int i=n/2-1;i>=0;i--)
            siftdown(i);
    }

    void insert(const E& it)
    {
        int curr = n++;
        Heap[curr] = it;
        while((curr!=0) && prior(Heap[curr],Heap[parent(curr)]))
        {
            swap(Heap,curr,parent(curr));
            curr = parent(curr);
        }

    }
    E removefirst()
    {
        swap(Heap,0,--n);
        if(n!=0)
            siftdown(0);
        return Heap[n];
    }

    void swap(E* h,int p,int j)
    {
        E temp = h[p];
        h[p] = h[j];
        h[j] = temp;
    }
    bool prior(E h1,E h2)
    {
        if(h1->weight() < h2->weight())
            return true;
        return false;
    }
};
#endif // HEAP_H
