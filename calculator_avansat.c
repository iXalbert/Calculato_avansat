#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 100

typedef struct {

    char data[MAX][MAX];
    int top;
}CharStack;

void initCharStack(CharStack *s){

    s->top = -1;
}

int isEmptyChar(CharStack *s){

    return s->top == -1;
}

void pushChar(CharStack *s, const char *str){

    strcpy(s->data[++(s->top)],str);
}

char* popChar(CharStack *s){

    return s->data[(s->top)--];
}

char* peekChar(CharStack *s){

    return s->data[(s->top)];
}

typedef struct{

    double data[MAX];
    int top;
}DoubleStack;

void initDoubleStack(DoubleStack *s){

    s->top = -1;
}

int isEmptyDouble(DoubleStack *s){

    return s->top == -1;
}

void pushDouble(DoubleStack *s, double val){

    s->data[++(s->top)] = val;
}

double popDouble(DoubleStack *s){

    return s->data[(s->top)--];
}

int precedente(char op){

    if(op == '+' || op == '-')
        return 1;

    if(op == '*' || op == '/')
        return 2;    

    return 0;
}

int isOperator(const char *c) {
    return (strcmp(c,"+") == 0 || strcmp(c,"-") == 0 || strcmp(c,"*") == 0 || strcmp(c,"/") == 0);
}

int isFuction(const char *c) {

    return (strcmp(c,"sin") == 0 || strcmp(c,"cos") == 0 || strcmp(c,"sqrt") == 0 || strcmp(c,"log") == 0);
}

void infixToPosifix(const char *infix, char postfix[][MAX], int *postfixSize){

    CharStack s;
    initCharStack(&s);
    *postfixSize = 0;

    int i=0;

    while(infix[i] != '\0'){

        if(isspace(infix[i])){
            i++;
            continue;
        }

        if(isdigit(infix[i]) || (infix[i] == '.' && isdigit(infix[i+1]))){

            char num[MAX];
            int j=0;
            while(isdigit(infix[i]) || infix[i] == '.'){
                num[j++] = infix[i++];
            }

            num[j] = '\0';
            strcpy(postfix[(*postfixSize)++], num);
            continue;
        }

        if(isalpha(infix[i])){

            char func[MAX];
            int j=0;

            while(isalpha(infix[i])){

                func[j++] = infix[i++]; 
            }

            func[j] = '\0';
            pushChar(&s, func);
            continue;
        }


        if(infix[i] == '('){
            char op[2] = {infix[i], '\0'};
            pushChar(&s, op);
        }

        else if(infix[i] == ')'){
            while(!isEmptyChar(&s) && strcmp(peekChar(&s), "(") != 0){

                char op[2] = {popChar(&s), '\0'};

                strcpy(postfix[(*postfixSize)++], op);
            }

            if(!isEmptyChar(&s) && strcmp(peekChar(&s), "(") == 0){
                popChar(&s);
            }
        }
        
        else if(isOperator(infix[i])){

            while(!isEmptyChar(&s) && precedente(peekChar(&s)) >= precedente(infix[i])){

                char op[2] = {popChar(&s), '\0'};
                strcpy(postfix[(*postfixSize)++], op);
            }

            char op[2] = {infix[i], '\0'};
            pushChar(&s, op);
        }

        i++;
    }

    while(!isEmptyChar(&s)){

        char op[2] = {popChar(&s), '\0'};
        strcpy(postfix[(*postfixSize)++], op);
    }
}


double evaluatePostfix(char postfix[][MAX], int size){

    DoubleStack s;
    initDoubleStack(&s);

    for(int i=0;i<size;i++){

        if(isOperator(postfix[i][0])){

            double b = popDouble(&s);
            double a = popDouble(&s);

            switch(postfix[i][0]){

                case '+' : pushDouble(&s, a + b); break;
                case '-' : pushDouble(&s, a - b); break;
                case '*' : pushDouble(&s, a * b); break;
                case '/' : pushDouble(&s, a / b); break;
            }
        }else if(isFuction(postfix[i][0])){
            double a = popDouble(&s);

            if(strcmp(postfix[i][0],"sin") == 0)
                pushDouble(&s,sin(a));
            else if(strcmp(postfix[i][0],"cos") == 0)
                pushDouble(&s,cos(a));
            else if(strcmp(postfix[i][0],"sqrt") == 0)
                pushDouble(&s,sqrt(a));
            else if(strcmp(postfix[i][0],"log") == 0)
                pushDouble(&s,log(a));
        }else{
            pushDouble(&s, atof(postfix[i]));
        }
    }

    return popDouble(&s);
}

int main(){

    char infix[MAX];
    char postfix[MAX][MAX];
    int postfixSize;

    printf("Introdu o expresie : \n");

    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = '\0';

    infixToPosifix(infix,postfix, &postfixSize);

    printf("Expresie in postfix : \n");

    for(int i=0;i<postfixSize;i++){

        printf("%s ", postfix[i]);
    }

    printf("\n");

    double rezultat = evaluatePostfix(postfix,postfixSize);

    printf("Rezultatul = %lf \n", rezultat);

    return 0;
}