#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct Node
{
	int nodeNo;
	int degree;
	int *adjNodes;
	float *adjNodesWeight;
	int parent;
}GraphNode;

typedef struct block
{
    int n;
    int key;//key = freq
	struct block *left, *right;
}block;

int isHeap ;
int heapSize ;

block *tree;
char **codeList;
int *codeLength;

GraphNode **graph;
int V,E,K;
int ncdWalk;//num nodes covered during walk
char *nodeMark;//to mark whether nod has been already covered or not
int *count;
int counter;

int *queue = NULL;
int front,rear;


void freeUpQ()
{
	if(queue)
		free(queue);
}

void initQ()
{
	if(queue == NULL)
		queue = calloc(V,sizeof(int));
	rear = -1;
	front = 0;
}

int isEmptyQ()
{
	if(rear == front-1)
		return 1;
	else return 0;
}

void enqueue(int n)
{
	queue[++rear] = n;
}

int dequeue()
{
	if(isEmptyQ())
	{
		initQ();
		printf("Underflow");
		return -99;
	}
	else
	{
		front++;
		return queue[front-1];
	}
}

void SkipToEndOfLine(FILE *fpInp)
{ while (getc(fpInp) != '\n');
}

void ReadInput_Edgelist_undirected()//creates graph from an edgelist for undirected graph #Added
{
	int i,x,y,w;
	FILE *fpInp;

	/*if ((fpInp = fopen("adj_with_community_rehashed_dolphins_lcc.txt", "r")) == NULL)
		{ fprintf(stderr, "Cannot open adj_with_community_rehashed_dolphins_lcc.txt\n"); exit(1); }*/

	if ((fpInp = fopen("graph.inp", "r")) == NULL)
		{ fprintf(stderr, "graph.inp\n"); exit(1); }

	fscanf(fpInp,"%d %d",&V,&E);//SkipToEndOfLine(fpInp);
	printf("got V and E\n");
	nodeMark = (char*)calloc(V,sizeof(char));
	graph = (GraphNode**)calloc(V,sizeof(GraphNode*));
	count = (int*)calloc(V,sizeof(int));
	tree = (block*)calloc(V,sizeof(block));
	codeList = (char**)calloc(V,sizeof(char*));
	codeLength = (int*)calloc(V,sizeof(int));
	for(i = 0;i<V;i++)
	{
		graph[i] = NULL;// (GraphNode*)malloc(sizeof(GraphNode));
		//graph[i]->nodeNo = i;
		//graph[i]->parent = -1;
		nodeMark[i] = 'w';
		//tree[i] = malloc(sizeof(block));
		tree[i].n = i;
		tree[i].left = NULL;
		tree[i].right = NULL;
		codeLength[i] = 0;
		//codeList[i] = (char*)calloc(1,sizeof(char));
	}
	for(i = 0;i<E;i++)
	{
		fscanf(fpInp,"%d %d",&x,&y);
		w = 1;
		if(graph[x] == NULL)
		{
			graph[x] = (GraphNode*)malloc(sizeof(GraphNode));
			graph[x]->nodeNo = x;
			graph[x]->degree = 0;
			graph[x]->adjNodes = (int *)calloc(1,sizeof(int));
			graph[x]->adjNodesWeight = (float *)calloc(1,sizeof(float));
			graph[x]->parent = -1;
		}
		graph[x]->degree += 1;
		graph[x]->adjNodes = realloc(graph[x]->adjNodes,graph[x]->degree*sizeof(int));
		graph[x]->adjNodesWeight = realloc(graph[x]->adjNodesWeight,graph[x]->degree*sizeof(float));
		graph[x]->adjNodes[graph[x]->degree - 1] = y;
		graph[x]->adjNodesWeight[graph[x]->degree - 1] = w;
		if(graph[y] == NULL)
		{
			graph[y] = (GraphNode*)malloc(sizeof(GraphNode));
			graph[y]->nodeNo = y;
			graph[y]->degree = 0;
			graph[y]->adjNodes = (int *)calloc(1,sizeof(int));
			graph[y]->adjNodesWeight = (float *)calloc(1,sizeof(float));
			graph[y]->parent = -1;
		}
		graph[y]->degree += 1;
		graph[y]->adjNodes = realloc(graph[y]->adjNodes,graph[y]->degree*sizeof(int));
		graph[y]->adjNodesWeight = realloc(graph[y]->adjNodesWeight,graph[y]->degree*sizeof(float));
		graph[y]->adjNodes[graph[y]->degree - 1] = x;
		graph[y]->adjNodesWeight[graph[y]->degree - 1] = w;

		SkipToEndOfLine(fpInp);
	}

	fclose(fpInp);

}

void printBlock(block *a)
{
    printf("&a = %u,a->n = %d, a->key = %d,a->left = %u,a->right = %u\n",a,a->n,a->key,a->left,a->right);
}


void startWalk(int S)
{
	srand(time(NULL));
	nodeMark[S] = 1;
	int u = S;
	printf("u = %d\n",u);
	int r,v;
	int i = 0;
	while(ncdWalk != V-1)
	{
		count[u] += 1;
		tree[u].key += 1;
		r = (int)(rand()%(graph[u]->degree));
		v = graph[u]->adjNodes[r];
		if(graph[u]->degree != 1 && v == graph[u]->parent)
		{
			r = (int)rand()%(graph[u]->degree);
			v = graph[u]->adjNodes[r];
		}
		graph[v]->parent = u;
		if(nodeMark[v] == 'w')
			ncdWalk++;
		nodeMark[v] = 'g';
		u = v;
		i++;
	}
	counter = i;

}

void printNodeCount()
{
	int i;
	printf("counter = %d\nNode Count\n",counter);
	for(i = 0;i < V;i++)
	{
		printf("%d %d\n",i,count[i]);
	}

}



//The MIN_PRIORITY_QUEUE part


int LEFT(int i)
{
    return 2*i+1;
}

int RIGHT(int i)
{
    return 2*i+2;
}

int PARENT(int i)
{
    if(i%2 == 0)
        return i/2 -1;
    else return i/2;
}

void display(block *a)
{
    int i;
    printf("\n");
    for(i = 0;i<heapSize;i++)
        printf("a[i].n= %d a[i].key = %d a[i].right = %u a[i].right = %u\n",a[i].n,a[i].key,a[i].left,a[i].right);
    printf("\n");
}

void min_heapify(block *a,int i,int n)//passing array pointer and its size
{
    int l = LEFT(i);
    int r = RIGHT(i);
    int smallest;
    if(l<n && a[l].key < a[i].key)
    {
        smallest = l;
    }
    else smallest = i;
    if(r<n && a[r].key < a[smallest].key)
    {
        smallest = r;
    }
    if(smallest != i)
    {

        //ind[a[smallest].n] = i;
        //ind[a[i].n] = smallest;
        //swap(&a[smallest], &a[i]);
        int temp = a[smallest].key;
        a[smallest].key = a[i].key;
        a[i].key = temp;

        temp = a[smallest].n;
        a[smallest].n = a[i].n;
        a[i].n = temp;

		block *temp1 ;
		temp1 = a[smallest].left;
		a[smallest].left = a[i].left;
		a[i].left = temp1;

		temp1 = a[smallest].right;
		a[smallest].right = a[i].right;
		a[i].right = temp1;

        min_heapify(a,smallest,n);
    }
}

void build_heap(block *a,int n)
{
    int i ;
    for(i=n/2;i>=0;i--)
        min_heapify(a,i,n);
    isHeap = 1;
}
/*
void heap_sort_min(block *a,int n)//descending order
{
    int i;
    //heapSize = n;
    int heap_size_temp = n;
    build_heap(a,n);
    display(a);
    for(i = 0;i<n;i++)
    {
        //swap(&a[0],&a[heap_size_temp-1]);
        int temp = a[0].key;
        a[0].key = a[heap_size_temp-1].key;
        a[heap_size_temp-1].key = temp;

        temp = a[0].n;
        a[0].n = a[heap_size_temp-1].n;
        a[heap_size_temp-1].n = temp;

        heap_size_temp--;
        min_heapify(a,0,heap_size_temp);
    }
    isHeap = 0;
}*/

void decrease_key(block *a,int index,int val)//val must be lesser than the value already at index
{
    if(a[index].key<val)
    {
        printf("index position has lower value than requested\n");
        return;
    }
    a[index].key = val;
    int par = PARENT(index);
    while(par>=0 && a[par].key>a[index].key)
    {
        //ind[a[par].n] = index;
        //ind[a[index].n] = par;

        int temp = a[par].key;
        a[par].key = a[index].key;
        a[index].key = temp;

        temp = a[par].n;
        a[par].n = a[index].n;
        a[index].n = temp;

		block *temp1 ;
		temp1 = a[par].left;
		a[par].left = a[index].left;
		a[index].left = temp1;

		temp1 = a[par].right;
		a[par].right = a[index].right;
		a[index].right = temp1;

        index = par;
        par = PARENT(index);
    }
}

block* insert_key(block *a,block *b)//int name,int key)
{

	//printf("To be inserted block :\n");
	//printBlock(b);
	//printf("\n");
    a = realloc(a,sizeof(block)*(heapSize+1));
    heapSize += 1;
    a[heapSize-1].n = b->n;
    a[heapSize-1].key = 999999999;
    a[heapSize-1].left = b->left;
    a[heapSize-1].right = b->right;
    //ind[name] = heapSize-1;
    if(isHeap == 0)
    {
        a[heapSize-1].key = b->key;
        build_heap(a,heapSize);
    }
    else decrease_key(a,heapSize-1,b->key);
    return a;
}

block extract_min(block *a)
{
    if(heapSize<=0)
    {
        block k ;
        k.n = -98;
        k.key = -9999;
        return k;
    }
    if(isHeap == 0)
    {
        build_heap(a,heapSize);
        isHeap = 1;
    }
    block min = a[0];
    a[0].n = a[heapSize-1].n;
    a[0].key = a[heapSize-1].key;
	a[0].left = a[heapSize-1].left;
	a[0].right = a[heapSize-1].right;
    heapSize = heapSize-1;
	min_heapify(a,0,heapSize);
    //ind[min.n] = -1;
    return min;
}

block minimum(block *a)
{
    if(isHeap == 0)
    {
        build_heap(a,heapSize);
        isHeap = 1;
    }
    return a[0];
}

//END of priority queue part

void traverse(block *a)
{
    if(a!= NULL)
    {
        printBlock(a);
        traverse(a->left);
        traverse(a->right);
    }

}



//Converting the weights to the codes
block* create_huffman_code(block *a)
{
	//int size = V;
	int i;
	isHeap = 0;
	heapSize = V;
	block *x,*y,*z,p,q;
	//build_heap(a,heapSize);
	//display(a);
	for(i = 0; i < V-1 ;i++)
	{

		x = malloc(sizeof(block));
		y = malloc(sizeof(block));
		z = malloc(sizeof(block));
		p = extract_min(a);
		x->n = p.n;
		x->key = p.key;
		x->right = p.right;
		x->left = p.left;
		//printf("x: \n");
		//printBlock(x);
		z->left = x;
		q = extract_min(a);
		y->n = q.n;
		y->key = q.key;
		y->right = q.right;
		y->left = q.left;
		//printf("y: \n");
		//printBlock(y);
		z->right = y;
		z->key = x->key + y->key;
		z->n = -100000 + x->n + y->n;
		//printf("z formed : \n");
		//printBlock(z);
		//printf("traversing z\n");
		//traverse(z);
		//printf("Done traversing z\n");
		//printf("\n\n");
		a = insert_key(a,z);
		//display(a);
	}
	return z;
}

void create_code_list(block *a,char *code,int cLength,int sideMark)//sideMark tells which side child is a of it's parent
													//sideMark = 0 for left child, 1 for right Child
{
    //printBlock(a);
	//printf("sideMark = %d,code till now = %s\n",sideMark,code);
	int i;
	char *codeNew;
	codeNew = code;
	if(sideMark!=-1){
		
	if (codeNew == NULL)
		codeNew = calloc(cLength+1,sizeof(char));
	else codeNew = realloc(codeNew,(cLength+1)*sizeof(char));
	if(sideMark == 1)
		codeNew[cLength] = '1';
	else codeNew[cLength] = '0';
	cLength += 1 ;
	}
	//printf("code at end = %s\n\n",code);
	if(a->right == NULL && a->left == NULL)//it is a leaf Node
	{
		/*if(sideMark == -1)//it is the start
		{
			codeList[a->n] = 0;
		}*/
		codeList[a->n] = calloc(cLength,sizeof(char));
		for(i = 0;i<cLength;i++)
			codeList[a->n][i] = codeNew[i];
		
		codeLength[a->n] = cLength;
		/*printf("a->n = %d code: ",a->n);
		for(i = 0;i<codeLength[a->n];i++)
			printf("%c",codeList[a->n][i]);
		printf(", len = %d\n",codeLength[a->n]);
		*/
	}
	else
	{
		
		if(a->right != NULL)
			create_code_list((a->right),codeNew,cLength,1);
		if(a->left != NULL)
			create_code_list((a->left),codeNew,cLength,0);
	}

}

void printCodeList(FILE *out)
{
	int i;
	int j;
	
	
	fprintf(out,"Node Code\n");
	for(i = 0;i<V;i++)
	{
		fprintf(out,"%d: ",i);
		for(j = 0;j<codeLength[i];j++)
			fprintf(out,"%c",codeList[i][j]);
		fprintf(out,"\t,len = %d\n",codeLength[i]);
	}
	
}


int main()
{
	time_t start,stop;
	start = time(NULL);
	ReadInput_Edgelist_undirected();
	printf("Graph reading done\n");
	int r = (int)(rand()%V);
	printf("Start = %d\n",r);
	startWalk(r);
	printf("ncdWalk = %d\n",ncdWalk);
	//printNodeCount();
	//printf("\n\n");
	block *b = create_huffman_code(tree);
	//printBlock(b);
	//printf("traversing b inside m after creating huffman tree\n");
	//traverse(b);
	//printf("\n\n");
	//printf("Starting to create code_list\n\n");
	create_code_list(b,(char*)NULL,0,-1);
	FILE *out = fopen("graph_embed_300000_1.out","w");
	printCodeList(out);
	stop = time(NULL);
	fprintf(out,"\n Runtime = %ld\n",(stop-start));
	
	float avg_length =0.0 ;
	int i;
	for(i = 0;i<V;i++)
		avg_length += codeLength[i];
	avg_length = (avg_length*1.0)/V;
	fprintf(out,"avg length = %f",avg_length);
	fclose(out);
	return 0;
}
