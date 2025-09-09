#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX 100
#define hist_size 100
#define WIN_W 600
#define WIN_H 800

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

int isOperator(const char *c) {
    return (strcmp(c,"+") == 0 || strcmp(c,"-") == 0 || strcmp(c,"*") == 0 || strcmp(c,"/") == 0);
}

int isFuction(const char *c) {
    return (strcmp(c,"sin") == 0 || strcmp(c,"cos") == 0 || strcmp(c,"sqrt") == 0 || strcmp(c,"log") == 0 || strcmp(c,"CtoF") == 0 || 
                strcmp(c,"FtoC") == 0 || strcmp(c,"DEG") == 0 || strcmp(c,"RAD") == 0 || strcmp(c,"BIN") == 0 || strcmp(c,"HEX") == 0);
}

int precedente_char(char op){
    if(op == '+' || op == '-')
        return 1;
    if(op == '*' || op == '/')
        return 2;
    return 0;
}

int precedente(const char *op){

    if(strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 1;

    if(strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
        return 2;
    
    if(isFuction(op))
        return 3;

    return 0;
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
                strcpy(postfix[(*postfixSize)++], popChar(&s));
            }
            if(!isEmptyChar(&s) && strcmp(peekChar(&s), "(") == 0){
                popChar(&s);
            }
        }

        else if(infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/'){
            char current_op[2] = {infix[i], '\0'};
            
            while(!isEmptyChar(&s) && strcmp(peekChar(&s), "(") != 0 && 
                  precedente(peekChar(&s)) >= precedente(current_op)){
                strcpy(postfix[(*postfixSize)++], popChar(&s));
            }
            pushChar(&s, current_op);
        }

        i++;
    }

    while(!isEmptyChar(&s)){
        strcpy(postfix[(*postfixSize)++], popChar(&s));
    }
}

void printBinary(int n){

    if(n==0){
        printf("0");
        return;
    }

    char buff[64];
    int i=0;

    while(n>0){

        buff[i++] = (n%2) + '0';
        n=n/2;
    }

    for(int j=i-1;j>=0;j--){
        printf("%c", buff[j]);
    }

}

void printHex(int n){
    printf("%X", n);
}

double evaluatePostfix(char postfix[][MAX], int size, int *special_print){

    DoubleStack s;
    initDoubleStack(&s);
    *special_print = 0;

    for(int i=0;i<size;i++){

        if(isOperator(postfix[i])){

            double b = popDouble(&s);
            double a = popDouble(&s);

            switch(postfix[i][0]){

                case '+' : pushDouble(&s, a + b); break;
                case '-' : pushDouble(&s, a - b); break;
                case '*' : pushDouble(&s, a * b); break;
                case '/' : pushDouble(&s, a / b); break;
            }
        }else if(isFuction(postfix[i])){
            double a = popDouble(&s);

            if(strcmp(postfix[i],"sin") == 0)
                pushDouble(&s,sin(a));
            else if(strcmp(postfix[i],"cos") == 0)
                pushDouble(&s,cos(a));
            else if(strcmp(postfix[i],"sqrt") == 0)
                pushDouble(&s,sqrt(a));
            else if(strcmp(postfix[i],"log") == 0)
                pushDouble(&s,log(a));
            else if(strcmp(postfix[i],"CtoF") == 0)
                pushDouble(&s,a* 9.0/5.0 + 32);
            else if(strcmp(postfix[i],"FtoC") == 0)
                pushDouble(&s,(a-32) * 5.0/9.0);
            else if(strcmp(postfix[i],"DEG") == 0)
                pushDouble(&s, a * M_PI / 180.0);
            else if(strcmp(postfix[i],"RAD") == 0)
                pushDouble(&s, a * 180.0 / M_PI);
            else if(strcmp(postfix[i],"BIN") == 0){
                printf("= ");
                printBinary((int)a);
                printf("\n");
                *special_print = 1;
            }
            else if(strcmp(postfix[i],"HEX") == 0){
                printf("= ");
                printHex((int)a);
                printf("\n");
                *special_print = 1;
            }
        }else{
            pushDouble(&s, atof(postfix[i]));
        }
    }

    if(isEmptyDouble(&s))
        return 0;
    else
        return popDouble(&s);
}

typedef struct 
{
    SDL_Rect r;
    char label[32];
}Button;

void drawRect(SDL_Renderer *ren, SDL_Rect *r){
    SDL_RenderFillRect(ren, r);
}

SDL_Texture *renderText(SDL_Renderer *ren, TTF_Font *font, const char *text){

    SDL_Color color = {255,255,255,255};
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font,text,color);
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren,surf);
    SDL_FreeSurface(surf);
    return tex;
}

int pointInRect(int x, int y, SDL_Rect *r){

    return (x>= r->x && x <= r->x + r->w && y >= r->y && y <= r->y + r->h);
}

#define BTN_ROWS 6
#define BTN_COLS 4

void setUPButtons(Button btns[], int *btnCount){

    int margin = 8;
    int btnW = (WIN_W - (margin * (BTN_COLS + 1))) / BTN_COLS;
    int btnH = 56;

    int startY = 240;

    const char *labels[BTN_ROWS * BTN_COLS] = {

        "7","8","9","/",
        "4","5","6","*",
        "3","2","1","-",
        ".","0","=","+",
        "sin","cos","sqrt","log"
    };

    int idx = 0;
    for(int r=0;r<BTN_ROWS;r++){
        for(int c=0;c<BTN_COLS;c++){
            Button *b = &btns[idx];
            b->r.x = margin + c * (btnW + margin);
            b->r.y = startY + c * (btnH + margin);
            b->r.w = btnW;
            b->r.h = btnH;
            strncpy(b->label,labels[idx],sizeof(b->label)-1);
            b->label[sizeof(b->label)-1] = '\0';
            idx++; 
        }
    }

    *btnCount = idx;
}

void appendExpr(char *expr, const char *token){

    if(strlen(expr) + strlen(token) + 1 >= MAX) 
        return;

    strcat(expr,token);
}

void delLast(char *expr){

    int n = strlen(expr);
    if(n>0)
        expr[n-1] = '\0';
}

int main(){

    return 0;
}