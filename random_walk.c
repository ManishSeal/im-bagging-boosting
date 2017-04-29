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
	for(i = 0;i<V;i++)
	{
		graph[i] = NULL;// (GraphNode*)malloc(sizeof(GraphNode));
		//graph[i]->nodeNo = i;
		//graph[i]->parent = -1;
		nodeMark[i] = 'w';
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

int main()
{
	ReadInput_Edgelist_undirected();
	printf("Graph reading done\n");
	int r = (int)(rand()%V);
	printf("Start = %d\n",r);
	startWalk(r);
	printf("ncdWalk = %d\n",ncdWalk);
	printNodeCount();	
	return 0;	
}