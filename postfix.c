;/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include <stdio.h> // 표준입출력 라이브러리 사용하기 위한 라이브러리 불러오기
#include <stdlib.h> // 동적 메모리 할당 및 해제 함수를 사용하기 위한 라이브러리 불러오기
#include<string.h> // strncat, strncpy 함수를 사용을 위한 라이브러리 불러오기

#define MAX_STACK_SIZE 10 // 스택의 크기 정의
#define MAX_EXPRESSION_SIZE 20 // 입력될 수식의 최대 길이 정의

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */


void postfixpush(char x); // postfix 스택에 data를 푸시하는 함수를 선언
char postfixPop(); // postfix 스택에서 항목을 팝하는 함수를 선언
void evalPush(int x); // evaluation 스택에 항목을 푸시하는 함수를 선언
int evalPop(); // evaluation 스택에서 항목을 팝하는 함수를 선언
void getInfix(); // 중위 표기법 수식을 입력받는 함수를 선언
precedence getToken(char symbol); // symbol에 해당되는 precedence 값을 반환하는 함수 정의
precedence getPriority(char x); // x의 우선순위 값을 반환하는 함수를 선언
void charCat(char* c); // 문자열에 문자를 추가하는 함수를 선언
void toPostfix(); // 중위 표기법을 후위 표기법으로 변환하는 함수를 선언
void debug(); // 프로그램 디버깅을 위한 함수를 선언
void reset(); // 스택 및 수식 초기화를 위한 함수를 선언
void evaluation(); // 후위 표기법을 계산하는 함수를 선언

int main()
{
	char command; // 명령어를 저장하는 용도의 문자형 변수 선언
	printf("[----- [Goh Geon Young]  [2020017027] -----]\n");
	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); // 명령어를 입력받아 command에 저장

		switch(command) {
		case 'i': case 'I': // insert일 떄
			getInfix(); // 중위 표기식 입력
			break;
		case 'p': case 'P': // postfix일 떄
			toPostfix(); // 중위 표기식을 후위 표기식으로 변환
			break; 
		case 'e': case 'E': // eval일 떄
			evaluation(); // 후위 표기식을 계산
			break;
		case 'd': case 'D': // debug일 떄
			debug(); // 디버깅 데이터들을 출력
			break;
		case 'r': case 'R': // reset일 떄
			reset(); // 스택 초기화
			break;
		case 'q': case 'Q': // quit일 떄
			break; // 프로그램 종료를 위한 break
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q'); // q 또는 Q가 입력이 들어올 때까지 while문 반복

	return 1; // 1 반환 및 프로그램 종료
}

void postfixPush(char x) // postfixStack에 data를 푸시하는 함수 정의
{
    postfixStack[++postfixStackTop] = x; // postfixStackTop의 값 1증가 후 postfixStackTop의 인덱스에 해당하는 스택 공간에 문자형 변수 x를 저장
}

char postfixPop() // postfixStack에서 항목을 팝하는 함수 정의
{
    char x;
    if(postfixStackTop == -1) // postfixStackTop이 -1이면 팝하지 않고 함수 종료
        return '\0';
    else {
		// postfixStackTop이 -1이 아니면
    	x = postfixStack[postfixStackTop--]; //postfixStackTop의 인덱스에 해당하는 스택 공간의 값을 x에 저장 한 후 postfixStackTop의 값 1감소
    }
    return x; // x 값 반환
}

void evalPush(int x) // evalStack에 항목을 푸시하는 함수를 선언
{
    evalStack[++evalStackTop] = x; // evalStackTop의 값 1증가 후 evalStackTop의 인덱스에 해당하는 스택 공간에 문자형 변수 x를 저장
}

int evalPop() // evalStack에서 항목을 푸시하는 함수를 선언
{
    if(evalStackTop == -1) // evalStackTop이 -1이면 팝하지 않고 함수 종료
        return -1;
    else
		// evalStackTop이 -1이 아니면
        return evalStack[evalStackTop--]; //evalStackTop의 인덱스에 해당하는 스택 공간의 값을 반환 및 그 후 evalStackTop의 값 1감소
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol) // symbol에 해당되는 precedence 값을 반환하는 함수 정의
{
	switch(symbol) { // symbol에 해당되는 precedence 값 mapping 후 반환
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x) // x의 우선순위 값을 반환하는 함수를 선언
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0') // postfixExp가 비어있다면
		strncpy(postfixExp, c, 1); // postfixExp에 c의 첫 번째 문자를 복사
	else
		strncat(postfixExp, c, 1); // postfixExp에 c의 첫 번째 문자를 추가
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
	while(*exp != '\0')
	{
		x = *exp;
		precedence token = getToken(x);
		if (token==rparen){ // token이 ')' 이면
			while (postfixStack[postfixStackTop]!= '('){ // '('가 나올 때 까지 postfixExp에 붙이기
				char temp = postfixPop();
				charCat(&temp);
			}
			postfixPop(); // '('을 스택에서 제거 (괄호는 최종 결과 값에 없어야 함)
		} else if (token == operand){ // token이 operand이면
			charCat(&x); // postfixExp에 붙이기
		}else{
			if (token != lparen){
				while(postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) >= token){ 
					// postfixStackTop이 -1 이 아니고(stack이 비어있지 않으면), token이랑 postfixStack의 top에 있는 값이랑 top에 있는 값이 더 클 경우
					// top에 있는 연산자의 우선순위가 입력 연산자의 우선순위와 같거나 높을 때까지 연산자를 pop을 한다.
					char temp = postfixPop(); // top에 있는 연산자가 더 우선순위가 높으면 pop
					charCat(&temp); // pop된 값 postfixExp에 붙이기
				}
			}
			postfixPush(x); // postfixStack에 연산자 x를 push
		}
		exp++; // exp 하나 증가 (다음 값(연산))

	}

	/* 필요한 로직 완성 */
    // 스택에 남아있는 연산자들을 모두 postfixExp에 추가
    while (postfixStackTop != -1) {
        char c = postfixPop();
        charCat(&c);
    }
}
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp); // 입력된 중위 표기식을 출력
	printf("postExp =  %s\n", postfixExp); // 변환된 후위 표기식을 출력
	printf("eval result = %d\n", evalResult); // 계산 결과값 출력

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]); // postfixStack의 각각의 원소를 출력

	printf("\n");

}

void reset() // 스택 및 수식 초기화를 위한 함수 정의
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
	/* postfixExp, evalStack을 이용한 계산 */
	precedence token;
	int op1, op2;
	int *exp = postfixExp;
	while (*exp){
		token = getToken(*exp);
		if (token == operand){ // token이 oprend이면
			evalPush(*exp - '0'); // 문자를 숫자로 변환 후 그 값 evalStack에 push
		}else{
			// token이 oprend가 아니면 -> 계산
			// evalStack에 2번의 pop을 통해 2개의 값 각각 op1,op2에 저장
			op1 = evalPop();
			op2 = evalPop();
			switch(token){
				case plus:
					evalPush(op1 + op2); // 두 개의 값 더한 후 evalStack에 push
					break;
				case minus:
					evalPush(op1 - op2); // 두 개의 값 뺀 후 evalStack에 push
					break;
				case times:
					evalPush(op1 * op2); // 두 개의 값 곱한 후 evalStack에 push
					break;
				case divide:
					evalPush(op1 / op2); // 두 개의 값 나눈 후 evalStack에 push
					break;
			}
		}
		exp ++; // exp 하나 증가 (다음 값)
	}
	
	evalResult = evalPop(); // 최종 계산된 결과값 evalResult에 저장
}

