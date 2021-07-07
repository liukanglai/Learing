#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define STACK_INIT_SIZE 20	 // 栈初始容量
#define STACK_INCREMENT 10   //栈满后，每次扩充的容量
#define EXPRESS_MAX 1024 //后缀表达式 长度不能超过1024
typedef int Status;
typedef char EleType;

typedef struct SeqStack
{
	EleType* top;//栈顶指针
	EleType* base;//栈底指针
	int stackSize;//栈容量
}SeqStack;
//初始化栈
Status InitStack(SeqStack* stack)
{
	//开辟空间
	stack->base = stack->top = (EleType*)malloc(STACK_INIT_SIZE * sizeof(EleType));
	if (!stack->base)
	{
		exit(0);
	}
	stack->stackSize = STACK_INIT_SIZE;
	return OK;
}
//压栈
Status push(SeqStack* stack, EleType e)
{
	if (stack == NULL)
	{
		return ERROR;
	}
	//压栈之前检测容量是否足够
	if (stack->top - stack->base == stack->stackSize)
	{
		//超出容量 进行扩容，使用realloc函数，会拷贝原内存内容
		stack->base = (EleType*)realloc(stack->base, stack->stackSize + STACK_INCREMENT);
		if (!stack->base)
		{
			exit(0);
		}
		stack->top = stack->base + stack->stackSize;
		stack->stackSize += STACK_INCREMENT;
	}
	*stack->top = e;
	stack->top++;
	return OK;
}
//弹栈
Status pop(SeqStack* stack, EleType *e)
{
	if (stack == NULL || e == NULL)
	{
		return ERROR;
	}
	//空栈
	if (stack->top == stack->base)
	{
		return ERROR;
	}
	*stack->top--;
	*e = *stack->top;
	return OK;
}
/*
获取栈顶元素
*/
Status GetTop(SeqStack* stack, EleType *e) {
	if (NULL == stack) {
		return ERROR;
	}
	*e = *(stack->top - 1);
	return OK;
}
/*
判断栈是否为空
*/
int IsEmptyStack(SeqStack* stack) {
	if (NULL == stack) {
		return ERROR;
	}
	if (stack->top == stack->base) {
		return TRUE;
	}
	return FALSE;
}
/*
销毁栈
*/
Status DestroyStack(SeqStack* stack)
{
	if (NULL == stack) {
		return ERROR;
	}
	//销毁栈 是释放栈在内存中占用的空间资源
	if (!stack->base)
	{
		free(stack->base);
	}
	stack->top = stack->base = NULL;
	stack->stackSize = 0;
	return OK;
}
/*
将中缀表达式转成后缀表达式
*/
char* MidExpToAfterExp(const char* midExp)
{
	//后缀表达式
	char* afterExp = (char*)malloc(sizeof(char)*EXPRESS_MAX);
	memset(afterExp, 0, EXPRESS_MAX);
	int j = 0;//afterExp下标
	int k = 0;//preExp下标

	SeqStack stack;// + - * / ( ）符号栈
	InitStack(&stack);

	char numBuf[10] = { 0 };//连续的数字不能超过10位，也就是中缀表达式中数字不能超过10位
	char ch = 0;
	int numIndex = 0;//numBuf 下标
	while (midExp[k])
	{
		ch = midExp[k++];
		//忽略中缀表达式中的空格
		if (ch == ' ')
		{
			continue;
		}
		//回车代表中缀输入完毕
		if ('\n' == ch)
		{
			break;
		}
		//1、若是数字就输出
		if (ch >= '0' && ch <= '9')
		{
			//如果输入连续的数字，不是连续的数字 循环完毕 会走 2、非数字
			while (((ch >= '0' && ch <= '9') || '.' == ch) && numIndex < 10)
			{
				numBuf[numIndex++] = ch;
				afterExp[j++] = ch;
				ch = midExp[k++];
			}
			numBuf[numIndex] = 0;
			afterExp[j++] = ' ';
		}
		//回车代表中缀输入完毕
		if ('\n' == ch)
		{
			break;
		}
		//忽略中缀表达式中的空格
		else if (' ' == ch)
		{
			continue;
		}
		//2、非数字
		else if (ch < '0' || ch > '9')
		{
			numIndex = 0;//进入这个if 数字肯定不连续了，下标重置为0
			//右括号一定弹栈
			if (')' == ch)
			{
				int flag = 1;//判断中缀表达式中括号是否匹配,如果成对出现
				while (!IsEmptyStack(&stack))
				{
					pop(&stack, &ch);
					if ('(' == ch)
					{
						flag = 0;//走到这里说明是()括号成对出现
						break;
					}
					afterExp[j++] = ch;
					afterExp[j++] = ' ';
				}
				if (flag)
				{
					printf("中缀表达式输入错误\n");
					exit(0);
				}

			}
			// + - 符号是优先级最低的，一定是先依次弹栈再压栈。
			else if ('+' == ch || '-' == ch)
			{
				EleType top;
				GetTop(&stack, &top);
				//栈空或者栈顶为左括号 直接压栈
				if (IsEmptyStack(&stack)|| '(' == top)
				{
					push(&stack, ch);
				}
				else
				{
					char cur = ch;
					while (!IsEmptyStack(&stack))
					{
						pop(&stack, &ch);
						if ('(' == ch)
						{
							//不是因为)右括号而弹栈，多弹的(左括号压回去
							push(&stack, ch);
							break;
						}
						afterExp[j++] = ch;
						afterExp[j++] = ' ';
					}

					push(&stack, cur);
				}
			}
			// * / 符号优先级只比 + -高，栈空或栈顶为(+-符号栈才直接压栈，其他情况先依次弹栈再压栈
			else if ('*' == ch || '/' == ch)
			{
				EleType top;
				GetTop(&stack, &top);
				//栈空或者栈顶为左括号同样直接压栈
				if (IsEmptyStack(&stack) || '(' == top || '-' == top || '+' == top)
				{
					push(&stack, ch);
				}
				else if ('*' == top || '/' == top)
				{
					char cur = ch;
					while (!IsEmptyStack(&stack))
					{
						pop(&stack, &ch);
						if ('(' == ch || '-' == ch || '+' == ch)
						{
							//不是因为)右括号而弹栈 * / 优先级高于栈顶 + - 就不弹栈了，多弹的压回去
							push(&stack, ch);
							break;
						}
						afterExp[j++] = ch;
						afterExp[j++] = ' ';
					}
					push(&stack, cur);
				}
			}
			else if ( '(' == ch)
			{
				push(&stack, ch);
			}
			else
			{
				printf("中缀表达式输入错误\n");
				exit(0);
			}
		}

	}
	//符号栈内容不为空 依次出栈并打印
	while (!IsEmptyStack(&stack))
	{
		pop(&stack, &ch);
		afterExp[j++] = ch;
		afterExp[j++] = ' ';
	}
	return afterExp;
}

int main(int argc, char *argv[])
{
	while (1)
	{
		printf("请输入中缀表达式(#表示退出)：");

		//中缀表达式
		char* midExp = (char*)malloc(sizeof(char)*EXPRESS_MAX) ;
		memset(midExp, 0, EXPRESS_MAX);
		fgets(midExp, 1024, stdin);//midExp 包含换行符
		if ('#' == midExp[0])
		{
			break;
		}

		//后缀表达式
		char* afterExp= MidExpToAfterExp(midExp);
		printf("对应的后缀表达式：%s\n", afterExp);
	}

	return 0;
}

