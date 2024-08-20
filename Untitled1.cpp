#include <stdio.h>
#include <stdlib.h>

#define tankcapacity_X 9
#define tankcapacity_Y 4
#define empty 0
#define goal 6
#define MaxLength 100
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))


typedef struct {
	int x, y;
} State;

// min, max
//int max(int a, int b) {
//	return (a>b)?a:b;
//}
//int min(int a, int b) {
//	return (a>b)?a:b;
//}

// KHoi tao trang thai X, Y
void makeNullState(State *state) {
	state->x=0;
	state->y=0;
}

// In trang thai
void print_State(State state) {
	printf("\n	X:%d --- Y:%d", state.x, state.y);
}

// Kiem tra trang thai muc tieu
int goalcheck(State state) {
	return (state.x==goal || state.y==goal);
}

// Lam day nuoc binh X
int pourWaterFullX(State cur_state, State *result) {
	if(cur_state.x < tankcapacity_X) {
		result->x = tankcapacity_X;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

// Lam day nuoc binh Y
int pourWaterFullY(State cur_state, State *result) {
	if(cur_state.y < tankcapacity_Y) {
		result->y = tankcapacity_Y;
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

// Lam rong nuoc binh X
int pourWaterEmptyX(State cur_state, State *result) {
	if(cur_state.x > 0) {
		result->x = empty;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}

// Lam rong nuoc binh Y
int pourWaterEmptyY(State cur_state, State *result) {
	if(cur_state.y > 0) {
		result->y = empty;
		result->x = cur_state.x;
		return 1;
	}
	return 0;
}

// Chuyen nuoc tu X sang Y
int pourWaterXY(State cur_state, State *result) {
	if(cur_state.x>0 && cur_state.y<tankcapacity_Y) {
		result->x = max(cur_state.x - (tankcapacity_Y-cur_state.y), empty);
		result->y = min(cur_state.x+cur_state.y,  tankcapacity_Y);
		return 1;
	}
	return 0;
}

// Chuyen nuoc tu Y sang X
int pourWaterYX(State cur_state, State *result) {
	if(cur_state.y>0 && cur_state.x<tankcapacity_X) {
		result->y = max(cur_state.y - (tankcapacity_X-cur_state.x), empty);
		result->x = min(cur_state.x+cur_state.y,  tankcapacity_X);
		return 1;
	}
	return 0;
}

int call_operator(State cur_state, State *result, int option) {
	switch(option) {
		case 1: return pourWaterFullX(cur_state, result);
		case 2: return pourWaterFullY(cur_state, result);
		case 3: return pourWaterEmptyX(cur_state, result);
		case 4: return pourWaterEmptyY(cur_state, result);
		case 5: return pourWaterXY(cur_state, result);
		case 6: return pourWaterYX(cur_state, result);
		default: printf("Error calls operators");
			return 0;
	}
}

// Array name state
const char* action[] = {"First State", "pour Water Full X", "pour Water Full Y", "pour Water Empty X", "pour Water Empty Y", "pour Water X to Y", "pour Water Y to X"};

//*-------------------------*
// Install Search Tree
typedef struct Node{
	State state;
	struct Node* Parent;
	int no_function;
}Node;

//*-------------------------*
// Install Stack
typedef struct {
	Node* Elements[MaxLength];
	int Top_idx;
}Stack;



// Init
void makeNull_Stack(Stack *stack) {
	stack->Top_idx = MaxLength;
}

//check empty
int empty_Stack(Stack stack) {
	return stack.Top_idx == MaxLength;
}

// check full
int full_Stack(Stack stack) {
	return stack.Top_idx == 0;
}

// return element in stack
Node* top(Stack stack) {
	if(!empty_Stack(stack)) {
		return stack.Elements[stack.Top_idx];
	}
	return NULL;
}

// delete
void pop(Stack *stack) {
	if(!empty_Stack(*stack)) {
		stack->Top_idx+=1;
	} else {
		printf("Error! Stack is empty");
	}
}

int compareStates(State S1, State S2) {
    return (S1.x == S2.x && S1.y == S2.y);
}

int find_State(State state, Stack openStack) {
	while(!empty_Stack(openStack)) {
		if(compareStates(top(openStack)->state, state));
			return 1;
		pop(&openStack);
	}
	return 0;
}

void push(Node* x, Stack *stack) {
	if(full_Stack(*stack)) {
		printf("Error! Stack is full");
	} else {
//		stack->Top_idx -=1;
//		stack->Elements[stack->Top_id]=x;
		stack->Elements[--stack->Top_idx] = x;
	}
}
// DFS
Node* DFS_Algorithm(State state) {
	// declare open/close stack
	Stack Open_DFS, Close_DFS;
	makeNull_Stack(&Open_DFS);
	makeNull_Stack(&Close_DFS);
	
	// create parent's status node
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->Parent = NULL;
	root->no_function = 0;
	push(root, &Open_DFS);
	
	while(!empty_Stack(Open_DFS)) {
		// Get 1 top
		Node* node = top(Open_DFS);
		pop(&Open_DFS);
		push(node, &Close_DFS);
		
		// check target status
		if(goalcheck(node->state)) {
			return node;
		}
		
		int opt;
		// Call operators
		for(opt=1; opt<=6; opt++) {
			State newstate;
			makeNullState(&newstate);
			if(call_operator(node->state, &newstate, opt)) {
				// 
				if(find_State(newstate, Close_DFS) || find_State(newstate, Open_DFS))
					continue;
				// 
				Node* newNode = (Node*)malloc(sizeof(Node));
				newNode->state = newstate;
				newNode->Parent = node;
				newNode->no_function = opt;
				push(newNode, &Open_DFS);
				
			}
		}
	}
	return NULL;
}

// print result
void print_WayToGetGoal(Node* node) {
	Stack stackPrint;
	makeNull_Stack(&stackPrint);
	//
	while(node->Parent != NULL) {
		push(node, &stackPrint);
		node = node->Parent;
	}
	push(node, &stackPrint);
	
	// print
	int no_action = 0;
	while(!empty_Stack(stackPrint)) {
		printf("\nAction %d: %s", no_action, action[top(stackPrint)->no_function]);
		print_State(top(stackPrint)->state);
		pop(&stackPrint);
		no_action++;
	}
}

int main() {
    State cur_S = {0, 0};
    Node *p = DFS_Algorithm(cur_S);
    print_WayToGetGoal(p);
    return 0;
}

