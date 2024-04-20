/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include <stdio.h> // 표준입출력 라이브러리 사용하기 위한 라이브러리 불러오기
#include <stdlib.h> // 동적 메모리 할당 및 해제 함수를 사용하기 위한 라이브러리 불러오기

#define MAX_QUEUE_SIZE 4 // 원형 큐의 크기를 4로 선언

typedef char element; // 큐의 원소를 element 데이터 타입으로 선언
typedef struct {
	element queue[MAX_QUEUE_SIZE]; // 큐의 원소들을 저장하는 배열 선언
	int front, rear; // 큐의 맨 앞(front)과 뒤(rear)의 큐의 인덱스 값을 저장하는 정수형 변수 선언
}QueueType;


QueueType *createQueue(); // 큐를 동적 할당으로 생성하는 함수를 선언
int freeQueue(QueueType *cQ); // 큐를 해제하는 함수를 선언
int isEmpty(QueueType *cQ); // 큐가 비어 있는지 확인하는 함수를 선언
int isFull(QueueType *cQ); // 큐가 가득 찼는지 확인하는 함수를 선언
void enQueue(QueueType *cQ, element item); // 큐에 요소를 추가하는 함수를 선언
void deQueue(QueueType *cQ, element* item); // 큐에서 요소를 제거하는 함수를 선언
void printQ(QueueType *cQ); // 큐를 출력하는 함수를 선언
void debugQ(QueueType *cQ); // 큐를 디버깅하는 함수를 선언
element getElement(); // 큐에 저장될 요소를 입력 받는 함수


int main(void)
{
	QueueType *cQ = createQueue(); // 원형 큐의 동적 할당 생성 및 초기화 수행
	element data; // 큐에 원소를 추가할 데이터를 저장하는 용도의 element형 변수 선언
	char command; // 명령어를 저장하는 용도의 문자형 변수 선언
	printf("[----- [Goh Geon Young]  [2020017027] -----]\n");
	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); // 명령어를 입력받아 command에 저장

		switch(command) {
		case 'i': case 'I': // insert일 떄
			data = getElement(); // 큐에 추가할 원소를 입력 받고 data에 저장
			enQueue(cQ, data); // data값을 큐에 추가
			break;
		case 'd': case 'D': // delete 떄
			deQueue(cQ, &data); // 큐의 요소를 제거 및 data에 저장
			break;
		case 'p': case 'P': // print일 떄
			printQ(cQ); // 큐에 저장된 요소들을 출력
			break;
		case 'b': case 'B': // debug일 떄
			debugQ(cQ); // 큐에 대한 디버깅된 원소들을 출력
			break;
		case 'q': case 'Q': // quit일 떄
   	        freeQueue(cQ); // 큐 초기화
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); // q 또는 Q가 입력이 들어올 때까지 while문 반복


	return 1; // 1 반환 및 프로그램 종료
}

QueueType *createQueue() // 큐를 동적 할당으로 생성하는 함수 정의
{
	QueueType *cQ; // 원형 큐에 대한 포인터 변수 선언
	cQ = (QueueType *)malloc(sizeof(QueueType)); // 원형 큐 크기와 맞는 동적 메모리 할당 및 cQ에 저장
	cQ->front = 0; // 원형 큐의 정수형 변수 front의 인덱스 값 0으로 초기화
	cQ->rear = 0; // 원형 큐의 정수형 변수 rear의 인덱스 값 0으로 초기화
	return cQ; // 초기화된 원형 큐 반환
}

int freeQueue(QueueType *cQ) // 큐를 해제하는 함수 정의
{
    if(cQ == NULL) return 1; // 원형 큐가 존재하지 않으면 함수 종료
    free(cQ); // 원형 큐가 존재하면 동적 메모리 할당 해제
    return 1; // 함수 종료
}

element getElement() // 큐에 저장될 요소를 입력 받는 함수
{
	element item; // 입력받을 데이터를 저장할 element형 변수 선언
	printf("Input element = ");
	scanf(" %c", &item); // 명령어를 입력받아 item에 저장
	return item;
}


/* complete the function */
int isEmpty(QueueType *cQ) // 큐가 비어 있는지 확인하는 함수 정의
{
	if (cQ->rear==cQ->front){ // front와 rear가 같다면 큐가 비어있다
		return 1; // 큐가 비어있음을 의미하는 1을 반환
	}else{
		return 0; // 큐가 비어있지 않음을 의미하는 0을 반환
	}
    	
}

/* complete the function */
int isFull(QueueType *cQ) // 큐가 가득 찼는지 확인하는 함수 정의
{
	int last; // rear의 한 칸 앞의 인덱스를 저장하기 위한 정수형 변수 last선언
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;
	if (last==cQ->front){ // rear의 한 칸 앞이 front랑 같으면 큐가 가득 찼다.
		return 1; // 큐가 가득 찼음을 의미하는 1을 반환
	}else{
		return 0; // 큐가 가득 찼지 않음을 의미하는 0을 반환
	}
   
}


/* complete the function */
void enQueue(QueueType *cQ, element item) // 큐에 요소를 추가하는 함수 정의
{
	if (isFull(cQ)){ // 가득 찼다면 요소를 추가하지 않고 함수 종료
		printf("Circular Q is Full\n");
		return;
	}
	// 큐가 가득차지 않았다면
	cQ->rear = (cQ->rear + 1)%MAX_QUEUE_SIZE; // 원형 큐에서 rear의 값을 한 칸 이동
	cQ->queue[cQ->rear] = item; // 원형 큐에서 이동한 rear의 위치에 item을 저장 (enqueue)
}

/* complete the function */
void deQueue(QueueType *cQ, element *item) // 큐에 요소를 제거하는 함수 정의
{
	if (isEmpty(cQ)){ // 비어있다면 요소를 제거하지 않고 함수 종료
		printf("Circular Q is Empty\n");
		return;
	}
	// 큐가 비어있지 않는다면
	cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE; // 원형 큐에서 front의 값을 한 칸 이동
	*item = cQ->queue[cQ->front]; // 원형 큐에서 이동한 front의 위치한 원소를 item 변수에 저장 (dequeue)
}


void printQ(QueueType *cQ) // 큐를 출력하는 함수 정의
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE; // 원형 큐에서 front을 한 칸 이동한 인덱스 값을 정수형 변수 first에 저장 (원형 큐의 첫번쨰 원소가 위치하는 인덱스)
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE; // 원형 큐에서 rear를 한 칸 이동한 인덱스 값을 정수형 변수 last에 저장 (원형 큐의 마지막 원소가 위치하는 인덱스)

	printf("Circular Queue : [");

	i = first; // first 값 정수형 변수 i에 저장
	while(i != last){ // 원형 큐의 첫 번째 원소부터 마지막 원소까지 반복
		printf("%3c", cQ->queue[i]); // 원형 큐의 각각의 원소들을 출력
		i = (i+1)%MAX_QUEUE_SIZE; // 원형 큐에서 i의 인덱스 값을 한 칸 이동
	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ) // 큐를 디버깅하는 함수를 선언
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++) // 원형 큐의 크기 만큼 반복
	{
		if(i == cQ->front) { // 인덱스 값이 큐의 front 인덱스 값과 같은 경우
			printf("  [%d] = front\n", i); // front의 인덱스 값을 출력
			continue; 
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]); // 인덱스 값과 해당 인덱스 위치의 큐 원소 mapping 하여 출력

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear); // 최종적으로 front와 rear 인덱스 값을 함께 출력
}