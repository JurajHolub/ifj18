/**
 * @file scanner.c
 * @brief Declaration of scanner interface for parser.
 * @date November 2018
 * @author Samuel Krempsky
 */

#include "scanner.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define TRUE 1
#define FALSE 0

//#define DEBUG

#ifdef DEBUG

static char *lexemes[] = {"not", "-", "+", "*", "/", "!=", "<", "int", "str", "nil", "(", ")", "eol", "==", ">",
                          "<=", ">=", "float", "def", "do", "else", "end", "if", "then", "while", "=", "FUN", ",",
                          "id", "EOF"};

#define PRINT_TOKENS \
    setbuf(stdout, 0);\
    setbuf(stderr, 0);\
    fprintf(stderr,"--------------------\n");\
    if (tokens[0] != NULL)\
    {\
        setbuf(stdout, 0);\
        setbuf(stderr, 0);\
        fprintf(stderr, "Lexeme: %s\n", tokens[0]->type == EOF ? "EOF" : lexemes[tokens[0]->type]);\
        if (tokens[0]->attribute != NULL || tokens[0]->type != EOF)\
        {\
            setbuf(stdout, 0);\
            setbuf(stderr, 0);\
            fprintf(stderr,"Lexeme attribute: %s\n", tokens[0]->attribute->string);\
        }\
    }\
    else\
    {\
        setbuf(stdout, 0);\
        setbuf(stderr, 0);\
        fprintf(stderr,"Lexeme: NULL\n");\
        setbuf(stdout, 0);\
        setbuf(stderr, 0);\
        fprintf(stderr,"Lexeme attribute: NULL\n");\
    }\
    setbuf(stderr, 0);\
    setbuf(stdout, 0);\
    fprintf(stderr,"--------------------\n");\
    setbuf(stderr, 0);\
    setbuf(stdout, 0);\
    fprintf(stderr, "Lexeme: %s\n", tokens[1]->type == EOF ? "EOF" : lexemes[tokens[1]->type]);\
    if (tokens[1]->attribute != NULL  || tokens[1]->type != EOF)\
    {\
        setbuf(stdout, 0);\
        setbuf(stderr, 0);\
        fprintf(stderr,"Lexeme attribute: %s\n", tokens[1]->attribute->string);\
    }\
    setbuf(stdout, 0);\
    setbuf(stderr, 0);\
    fprintf(stderr,"--------------------\n\n");\
    //--///////////////////////////////////////////////////////////////////////////////////////////
    //--/////////////////////////////////////TESTING OUTPUT END////////////////////////////////////

#else

#define PRINT_TOKENS

#endif


token_t enterToken;
token_t tokenNext;
char nextchar;//='\0';
int globalVar=1;

token_t *tokens[2] = {NULL, NULL};
int returned_token = 0;
int get= 1;


int globalEOF=FALSE;
int globalEOL=FALSE;


int controlKeyWords(char *testString, token_t *token){


    char keyWords[9][10]={
        "def","do","else","end","if","not","nil","then","while"
    };


    for (int i = 0; i < 9; i++)
    {
        if      (strcmp(testString,keyWords[i]) == 0 && i ==0 ){
            token->type=DEF; 
            return TRUE;
        }
        else if (strcmp(testString,keyWords[i]) == 0 && i ==1 ){
            token->type=DO; 
            return TRUE;
        }
        else if (strcmp(testString,keyWords[i]) == 0 && i ==2 ){
            token->type=ELSE; 
            return TRUE;
        }
        else if (strcmp(testString,keyWords[i]) == 0 && i ==3 ){
            token->type=END; 
            return TRUE;
        }
        else if (strcmp(testString,keyWords[i]) == 0 && i ==4 ){
            token->type=IF; 
            return TRUE;
        }
        else if (strcmp(testString,keyWords[i]) == 0 && i ==5 ){
            token->type=NOT; 
            return TRUE;
        }
        else if (strcmp(testString,keyWords[i]) == 0 && i ==6 ){
            token->type=NIL; 
            return TRUE;
        }
        else if (strcmp(testString,keyWords[i]) == 0 && i ==7 ){
            token->type=THEN; 
            return TRUE;
        }
        else if (strcmp(testString,keyWords[i]) == 0 && i ==8 ){
            token->type=WHILE; 
            return TRUE;
        }
    }

    return FALSE;
    
}

int controlConstants(char *testString,token_t *token){
     char keyWords[5][10]={
        "int","bool","float","string","nil"
    };
    for(int i = 0; i < 5; i++){
       if (strcmp(testString,keyWords[i]) == 0 && i ==0 ){
            token->type=INTEGER; 
            return TRUE;
        }
        else if(strcmp(testString,keyWords[i]) == 0 && i ==1 ){
            token->type=BOOL; 
            return TRUE;
        }
        else if(strcmp(testString,keyWords[i]) == 0 && i ==2 ){
            token->type=FLOAT; 
            return TRUE;
        }
        else if(strcmp(testString,keyWords[i]) == 0 && i ==3 ){
            token->type=STRING; 
            return TRUE;
        }
        else if(strcmp(testString,keyWords[i]) == 0 && i ==4 ){
            token->type=NIL; 
            return TRUE;
        }

    }
    return FALSE;



}

int controlWords(char *testString, token_t *token){

    if(testString[0]=='_' || (testString[0] >= 'a' && testString[0] <= 'z') ){
        token->type=VAR;
    }

    return TRUE;
}

int controlInt(char *testString,token_t *token){

    char *emptyString='\0';
    //emptyString[0]='\0';
    
    strtol(testString,&emptyString,10);


    if (emptyString[0]!='\0'){
        return FALSE;
    }

    token->type=INTEGER;
    return TRUE;

}

int controlDouble(char *testString,token_t *token){

    for (int i = 0; i < strlen(testString); i++){

        if( (testString[i]>='0' && testString[i]<='9') || testString[i]=='e' 
            || testString[i]=='E' || testString[i]=='+' || testString[i]=='-' || testString[i]=='*' || testString[i]=='.');
        else
            return FALSE;
        
    }

    
    token->type=FLOAT;
    return TRUE;

}


//nextchar, c

int controlOperators(char c, token_t *token, char scndOperator){
    if (c=='=' && scndOperator=='\0'){
        token->type=ASSIG;
        return TRUE;
    }
    else if(c=='+' && scndOperator=='\0'){
        token->type=ADD;
        return TRUE;
    }
    else if (c=='-' && scndOperator=='\0'){
        token->type=SUB;
        return TRUE;
    }
    else if (c=='/' && scndOperator=='\0'){
        token->type=DIV;
        return TRUE;
    }
    else if (c=='*' && scndOperator=='\0'){
        token->type=MUL;
        return TRUE;
    }
    else if (c=='=' && scndOperator=='\0'){
        token->type=EQUAL;
        return TRUE;
    }
    else if (c=='<' && scndOperator=='\0'){
        token->type=LESS;
        return TRUE;
    }
    else if (c=='>' && scndOperator=='\0'){
        token->type=GREATER;
        return TRUE;
    }
    else if (c=='<' && scndOperator=='='){
        token->type=LESS_EQ;
        return TRUE;
    }
    else if (c=='>' && scndOperator=='='){
        token->type=GREATER_EQ;
        return TRUE;
    }
    else if (c=='!' && scndOperator=='='){
        token->type=NOT_EQUAL;
        return TRUE;
    }
    else if (c=='=' && scndOperator=='='){
        token->type=EQUAL;
        return TRUE;
    }
    else if (c=='(' && scndOperator=='\0'){
        token->type=LEFT_B;
        return TRUE;
    }
    else if (c==')' && scndOperator=='\0'){
        token->type=RIGHT_B;
        return TRUE;
    }
    else if (c=='\n' && scndOperator=='\0'){
        enterToken.type=EOL;
        token->type=EOL;
        return TRUE;
    }
    else if (c==',' && scndOperator=='\0'){
        token->type=DELIM;
        return TRUE;
    }

    return FALSE;
}

int controlSigns(char c){

    char operators[]={' ','\n','\t','*','/','+','-','<','=','>','(',')',',','"','#'};

    for (int i = 0; i < strlen(operators); i++){
        if( c==operators[i]){
            return TRUE;
        }
    }

    return FALSE;

}


void charAppend(token_t *token,char c){

    
    char append_char[5]={0};
    append_char[0]=c;

    string_append_ch(token->attribute,append_char);
}

void hexConvertFoo(char hexConvert[], char hexReturn[]){

    int i=strtol(hexConvert, NULL, 16);
    char helpArr[3]={0};

    
    
    if(i<10){
    
        strcpy(hexReturn, "\\00");
        sprintf(helpArr,"%d",i);
        strcat(hexReturn,helpArr);

    }
    else if (i<100){
    
        strcpy(hexReturn, "\\0");
        sprintf(helpArr,"%d",i);
        strcat(hexReturn,helpArr);
    
    }
    else if (i>=100){
    
        strcpy(hexReturn, "\\");
        sprintf(helpArr,"%d",i);
        strcat(hexReturn,helpArr);
    
    }


}

void free_scanner() {
    if (tokens[0] != NULL)
    {
        if (tokens[0]->attribute != NULL)
        {
            string_free(tokens[0]->attribute);
        }
        free(tokens[0]);
        tokens[0] = NULL;
    }
    if (tokens[1] != NULL)
    {
        if (tokens[1]->attribute != NULL)
        {
            string_free(tokens[1]->attribute);
        }
        free(tokens[1]);
        tokens[1] = NULL;
    }
}

token_t *get_token(){

    //max 2 tokens can be returned
    assert(returned_token >= 0 && returned_token <= 2);
    //if token was returned from scanner, there is no need to get new token. We return last token the function got
    //saved in global array
    if (returned_token > 0)
    {
        //every time there is a returned token, the last returned token is on position 1 in tokens array
        token_t *tmp = tokens[1];

        //if we have 2 tokens returned we have to rotate the array to get token, that will be now returned to position 0,
        //where is saved in case, that will be returned

        if (returned_token == 2)
        {
            tokens[1] = tokens[0];
            tokens[0] = tmp;
        }

        //decrement number of returned_tokens, because last returned token will be used now
        returned_token--;

        //last returned token will be given to parser
        PRINT_TOKENS
        return tmp;
    }
    //there are no returned tokens, so the oldest saved token is deleted and on its place is saved last token and
    //on the place of last token will be saved new token in next steps
    if (tokens[0] != NULL)
    {
        if (tokens[0]->attribute != NULL)
        {
            string_free(tokens[0]->attribute);
        }
        free(tokens[0]);
    }
    tokens[0] = tokens[1];
    //allocation of new token
    tokens[1] = (token_t *) malloc(sizeof(token_t));
    if (tokens[1] == NULL)
    {
        fprintf(stderr, "Malloc fail\n");
        free_scanner();
    }
    //data will be achieved and inserted in token



    //token_t *tokens[1]=malloc(sizeof(token_t));
    char *str = NULL;
    string_t string=string_create(str);
    tokens[1]->attribute=string;


    //int commentSwitch=0;

    //char escapeSeq[]="0000";

    char hexConvert[]="00";
    char testString[64]={0};
    char hexReturn[4]={0};
    char c;
    int charCounter=0;


    int state=START_state;
    int state2=N_state;
    //int errorState=N_state;



    if(globalEOL==TRUE){

        globalEOL=FALSE;
        globalEOF=TRUE;
        tokens[1]->type = EOL;
        return tokens[1];
    }
    else if (globalEOF==TRUE){
        tokens[1]->type = EOF;
        return tokens[1];
    }





    //char *testString_IFJ;
    //string_t stringTest=string_create(testString_IFJ);

    if(tokenNext.type==NOT_EQUAL){

        nextchar='\0';
        tokenNext.type=NOT;
        tokens[1]->type=NOT_EQUAL;
        PRINT_TOKENS
        return tokens[1];

    }

    if(enterToken.type==EOL){
        nextchar='\0';
        enterToken.type=NOT;
        state2=COMMENT_BEGIN_state;
    
    }
    else if(enterToken.type!=EOL){
        enterToken.type=NOT;
        state2=N_state;
    }

    
        //printf(">%c<\n",nextchar);
       
    if( nextchar=='=' || nextchar =='!' || nextchar=='<' || nextchar == '>' ){

        state=SIGN_state;
    }
    else if( (nextchar>='a' && nextchar <= 'z')  || nextchar=='_'){
        state=ID_state;

        charAppend(tokens[1],nextchar);
        

        /*
        testString[charCounter]=nextchar;
        charCounter++;*/

    }
    else if (nextchar=='"'){
        
        state=STRING_state;
        nextchar='\0';

    }
    else if (nextchar=='0'){
        
        state=DOUBLE_PROB_state;
        charAppend(tokens[1],nextchar);
        nextchar='\0';
    
    }
    else if (nextchar>='1' && nextchar <= '9'){

        state=INT_state;
        charAppend(tokens[1],nextchar);
        nextchar='\0';
        /*
        testString[charCounter]=nextchar;
        charCounter++;*/
    }
    else if(nextchar=='#'){
        state=COMMENT_state;
        nextchar='\0';
    }
    else if(controlOperators(nextchar,tokens[1],'\0')==TRUE){
        //printf("hello\n");
        nextchar='\0';
        PRINT_TOKENS
        return tokens[1];
    }
    else if(nextchar=='@'){

        tokens[1]->type=ERROR;
        PRINT_TOKENS
        return tokens[1];
    
    }
   
    /**/

    /*while((c=getchar())!=EOF){*/
    while( (c=getchar()) ){


        //Control of EOL before EOF
        if(c==EOF){
            

            if(state2==COMMENT_BEGIN_state){
                tokens[1]->type = EOF;
                return tokens[1];
            }


            if (controlInt(tokens[1]->attribute->string, tokens[1]) == TRUE){

                globalEOL=TRUE;
                PRINT_TOKENS
                return tokens[1];
            
            }
            else if(controlDouble(tokens[1]->attribute->string, tokens[1]) == TRUE){
            
                globalEOL=TRUE;
                PRINT_TOKENS
                return tokens[1];
            
            }
            else if(controlKeyWords(tokens[1]->attribute->string, tokens[1]) == TRUE){
                //nextchar=c;
                //printf("hello\n");
                globalEOL=TRUE;
                PRINT_TOKENS
                return tokens[1];
            
            }  
            else if(controlWords(tokens[1]->attribute->string,tokens[1])==TRUE){
                //nextchar=c;
                globalEOL=TRUE;
                PRINT_TOKENS
                return tokens[1];
            
            }  

    
            

            tokens[1]->type = EOF;
            return tokens[1];
        }



        //control of '==','<=','!=','>=' comparators
        if(state==SIGN_state){

            if( c=='=' ){
                //printf("%c,%c\n",nextchar,c);
                if(controlOperators(nextchar,tokens[1],c)==TRUE){
                    nextchar='\0';
                    PRINT_TOKENS
                    return tokens[1];
                }

            }
            else if(c=='"'){

                if(controlOperators(nextchar,tokens[1],'\0')==TRUE){
                    nextchar=c;
                    PRINT_TOKENS
                    return tokens[1];
                }

            }
            else if((c>='a' && c <= 'z')  || c=='_'){
                //treba už vracať token a pamätať si prvú vec čo je tuna

                if(controlOperators(nextchar,tokens[1],'\0')==TRUE){
                    nextchar=c;
                    PRINT_TOKENS
                    return tokens[1];
                }

            }
            else if (c>='0' && c <= '9'){

                if(controlOperators(nextchar,tokens[1],'\0')==TRUE){
                    nextchar=c;
                    PRINT_TOKENS
                    return tokens[1];
                }

            }
        }
        else if ( (c!=' '|| c!='\t') && state!=START_state && controlSigns(c)==TRUE){
                nextchar=c;
        }



        if ( state==COMMENT_state){

            //printf("%c\n",c );
            //printf("hello");
            if (c=='\n'){
                
                PRINT_TOKENS
                tokens[1]->type=EOL;
                return tokens[1];

            }


        }            
        else if (state==STRING_state){
            

            if(state2==ESCAPE_state){
                if(c=='t'){
                    //c='\t';

                    string_append_ch(tokens[1]->attribute,"\\009");
                    //charAppend(tokens[1],c);
                    state2=N_state;
                    //vrátitť sa a vynulovať stav
                }
                else if (c=='"'){
                    string_append_ch(tokens[1]->attribute,"\\034");
                    state2=N_state;
                }
                else if (c=='n'){
                
                    //c='\n';

                    string_append_ch(tokens[1]->attribute,"\\010");
                    state2=N_state;
                
                }
                else if (c=='s'){
                    
                    //c=' ';
                    string_append_ch(tokens[1]->attribute,"\\032");
                    state2=N_state;
                
                }
                else if (c==92){
                    string_append_ch(tokens[1]->attribute,"\\092");
                    //charAppend(tokens[1],c);
                    state2=N_state;
                
                }
                else if (c=='x'){

                    state2=HEX_ESCAPE_state;
                    //state2=N_state;
                
                }
            }
            //
            else if(state2==HEX_ESCAPE_state){

                if( (c>='0' && c <= '9') || (c>='A' && c <= 'F') || (c>='a' && c <= 'f')  ){//TODO check wheter it should be capital or noncapital letters 
                    hexConvert[0]=c;
                    state2=HEX_ESCAPE2_state;
                }
                else{
                    //ERROR
                }
                
            }
            else if(state2==HEX_ESCAPE2_state){

                if( (c>='0' && c <= '9') || (c>='A' && c <= 'F') || (c>='a' && c <= 'f') ){
                    
                    //swapping the order of the string
                    //hexConvert[0]=hexConvert[1];
                    hexConvert[1]=c;
                    
                    hexConvertFoo(hexConvert,hexReturn);
                    string_append_ch(tokens[1]->attribute,hexReturn);

                    state2=N_state;

                }
                else{
                    
                    hexConvertFoo(hexConvert,hexReturn);
                    string_append_ch(tokens[1]->attribute,hexReturn);

                    state2=N_state;

                }

            }
            //
            else if ( c== 92){//92=='\'

                //printf("%c\n",c);// 47 is the value of 
                //printf("hello\n");
                state2=ESCAPE_state;
            }
            else if( c==' '){
                string_append_ch(tokens[1]->attribute,"\\032");
            }
            else if( c=='#'){
                string_append_ch(tokens[1]->attribute,"\\035");
            }

            else if( c == '"' ){
                nextchar='\0';
                tokens[1]->type=STRING;
                return tokens[1];
            }
            else if(c =='\n' || c==EOF){
                //printf("hello\n");
                tokens[1]->type=ERROR;
                return tokens[1];
            }
            
            else{
                //printf("hello\n");
                charAppend(tokens[1],c);
            }

            
            /*
            testString[charCounter]=c;
            charCounter++;*/
        }
        else if (state==START_state){
            
            
            if ( (c>='a' && c <= 'z') || c=='_'){

                state=ID_state;
                charAppend(tokens[1],c);

                /*testString[charCounter]=c;
                charCounter++;*/
            }
            else if(c=='#'){
                //printf("hello\n");
                state=COMMENT_state;
            
            }
            else if(c=='0'){

                state=DOUBLE_PROB_state;
                charAppend(tokens[1],c);
            
            }
            else if(c>='1' && c <= '9'){

                state=INT_state;
                charAppend(tokens[1],c);
                /*testString[charCounter]=c;
                charCounter++;*/
                
            }
            else if(c=='=' && state2==COMMENT_BEGIN_state){
                
                state2=N_state;
                state=COMMENT_BEGIN_state;

            }
            else if(c>='A' && c <= 'Z'){
                //GOTO state error
                tokens[1]->type=ERROR;
                return tokens[1];
                
            }
            else if(c=='=' || c =='!' || c=='<' || c == '>' ){

                nextchar=c;
                state=SIGN_state;
            }
            else if(c=='"'){
                state=STRING_state;
            }
            else if(controlOperators(c,tokens[1],'\0')==TRUE && (c!=' '|| c!='\t') ){
                PRINT_TOKENS
                return tokens[1];
            }
            else if( c==' ' || c=='\t'){
                state=START_state;
            }
            else if ( c=='@' ){

                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            
            }
                

 

            


        }
        else if(state==COMMENT_BEGIN_state){
            
            testString[charCounter]=c;
            charCounter++;

            if(charCounter==5){//error by one
                char beginTest[]="begin";
                if(strcmp(testString,beginTest) == 0){


                    //We "erase" the array and NULL the cnt
                    for (int i = 0; i < 64; i++)testString[i]='\0';
                    charCounter=0;

                    state=COMMENT_END_state;
                }
                else{
                    //error
                }

            }
                
        }
        else if(state==COMMENT_END_state){

            if(c=='\n'){
                state=COMMENT_END_ENTER_state;
            }
        }
        else if(state==COMMENT_END_ENTER_state){
            if(c=='='){

                state=COMMENT_END_ENTER2_state;
                
            }
            else{
                state=COMMENT_END_state;
            }
        }
        else if(state==COMMENT_END_ENTER2_state){

            testString[charCounter]=c;
            charCounter++;
            

            if(charCounter==3){
                char endTest[]="end";
                
                if(strcmp(testString,endTest) == 0){
                    state=START_state;
                }
                else{

                    state=COMMENT_END_state;
                    //error
                    //We "erase" the array and NULL the cnt
                    for (int i = 0; i < 64; ++i)
                        testString[i]='\0';
                    
                    charCounter=0;
                
                }

            }
        }
        else if(state==FUNCTION_state){
            if(controlSigns(c)==TRUE && c!='='){
                tokens[1]->type=FUN;
                PRINT_TOKENS
                return tokens[1];
            }
            else if(c=='='){


                //testString[charCounter-1]='\0';
                /*
                int len=strlen(returnToken->attribute->string);
                returnToken->attribute->string[len-1]='\0';
                returnToken->type=VAR;*/


                //FIXED
                //I forgot some erasing the last char which is ? or !

                tokenNext.type=NOT_EQUAL;
                

                int len=strlen(tokens[1]->attribute->string);
                tokens[1]->attribute->string[len-1]='\0';
                tokens[1]->type=VAR;

                PRINT_TOKENS
                return tokens[1];


            }
            else{
                //error
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }
        }
        else if(state==ID_state){
            if((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') || (c>='0' && c <= '9') || c=='_'){
                state=ID_state;

                charAppend(tokens[1],c);

                /*testString[charCounter]=c;
                charCounter++;*/

            }
            else if(c=='@'){

                //constants are not supported in a source file

                /*if(controlConstants(testString,tokens[1])==TRUE);
                state=CONSTANT_state;
                charAppend(tokens[1],c);

                testString[charCounter]=c;
                charCounter++;*/

                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];

            }
            else if(c=='?' || c== '!'){
                state=FUNCTION_state;

                charAppend(tokens[1],c);
                /*testString[charCounter]=c;
                charCounter++;*/
            }
            else if(controlSigns(c)==TRUE){

                

                if(controlKeyWords(tokens[1]->attribute->string, tokens[1]) == TRUE){
                    nextchar=c;
                    PRINT_TOKENS
                    return tokens[1];
                }      
                if(controlWords(tokens[1]->attribute->string,tokens[1])==TRUE){
                    nextchar=c;
                    PRINT_TOKENS
                    return tokens[1];
                } 

            }
        }
        else if(state==INT_state){
            if (c>='0' && c <= '9'){

                state=INT_state;

                charAppend(tokens[1],c);
                /*testString[charCounter]=c;
                charCounter++;*/

            }
            else if( c== '.'){
                state=DOUBLE_state;
                charAppend(tokens[1],c);
                /*testString[charCounter]=c;
                charCounter++;*/
            }
            else if(controlSigns(c)==TRUE){
                
                if(controlInt(tokens[1]->attribute->string, tokens[1]) == TRUE) {
                    PRINT_TOKENS
                    return tokens[1];
                }

            }
            else if((c>='a' && c <= 'z')|| (c>='A' && c <= 'Z')){

                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
                
            }
        }
        else if(state==DOUBLE_PROB_state){
            if (c=='.') {
                state=DOUBLE_state;
                charAppend(tokens[1],c);
            }

            else if (controlSigns(c)==TRUE){
                
                if(controlInt(tokens[1]->attribute->string, tokens[1]) == TRUE) {
                    PRINT_TOKENS
                    return tokens[1];
                }
            
            }
            else if ((c>='0' && c <= '9') || (c>='A' && c <= 'Z') || (c>='0' && c <= '9') || c=='_'){
                //error
            
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            
            }
            else if (c=='@'){
                //error
            
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            
            }

        }
        else if(state==DOUBLE_state){
            
            if (c>='0' && c <= '9'){
                charAppend(tokens[1],c);
                /*testString[charCounter]=c;
                charCounter++;*/
            
            }
            else if(c=='e'|| c=='E'){
                //probably some error
                //TODO chyták
                state=EXPONENT_state;
                charAppend(tokens[1],c);
                /*testString[charCounter]=c;
                charCounter++;*/
            }
            else if (controlSigns(c) == TRUE){
                
                if( controlDouble(tokens[1]->attribute->string, tokens[1]) == TRUE) {
                    PRINT_TOKENS
                    return tokens[1];
                }
            }
            else if ((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') ||  c=='_'){
                //error
                /**/
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }

        }
        else if (state==EXPONENT_state){

            if(c>='0' && c <= '9'){

                state=EXPONENT_state;
                charAppend(tokens[1],c);
                /*testString[charCounter]=c;
                charCounter++;*/
            }
            else if(c=='+' || c == '-'){
                state=EXPONENT_sign_state;
                charAppend(tokens[1],c);
                /*
                testString[charCounter]=c;
                charCounter++;*/
            }
            else if (controlSigns(c)==TRUE) {

                tokens[1]->type=FLOAT;
                PRINT_TOKENS
                return tokens[1];
            }
            else if ((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') ||  c=='_' || c==';' || c=='@'){
                //error

                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];

            }
        }
        else if(state==EXPONENT_sign_state){
            if(c>='0' && c <= '9'){

                state=EXPONENT_sign_state;
                charAppend(tokens[1],c);
                /*testString[charCounter]=c;
                charCounter++;*/
            
            }
            else if (controlSigns(c)==TRUE)  {
                //printf("%s\n", testString);
                tokens[1]->type=FLOAT;
                PRINT_TOKENS
                return tokens[1];
            }
            else if ((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') ||  c=='_' || c==';' || c=='@'){
                //error
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }

        }
        else if(state ==CONSTANT_state){
            if((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') || (c>='0' && c <= '9') || c=='-' || c=='+'){
                state=CONSTANT_state;
                charAppend(tokens[1],c);
                /*
                testString[charCounter]=c;
                charCounter++;*/

            }
            else if (controlSigns(c)==TRUE){
                // TODO control of constant
                PRINT_TOKENS
                return tokens[1];
            }
            else {
                //error
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }
        }
        else {
            //error
            /*
            tokens[1]->type=ERROR;
            PRINT_TOKENS
            return tokens[1];*/
        
        }
        

        
        
        
    }




    //printf("%s\n",tokens[1]->attribute->string );


    /*
    if(controlKeyWords(tokens[1]->attribute->string, tokens[1]) == TRUE){
        //nextchar=c;
        globalEOL=TRUE;
        PRINT_TOKENS
        return tokens[1];
    }  
    else if(controlWords(tokens[1]->attribute->string,tokens[1])==TRUE){
        //nextchar=c;
        globalEOL=TRUE;
        PRINT_TOKENS
        return tokens[1];
    } 
    
    if(globalEOL==TRUE){
        globalEOL=FALSE;
        globalEOF=TRUE;
        tokens[1]->type = EOL;
        return tokens[1];

    }
    else if (globalEOF==TRUE){
        tokens[1]->type = EOF;
        return tokens[1];
    }

    */
    tokens[1]->type = EOF;
    return tokens[1];

}

void ret_token(token_t* token)
{
    //dummy check if no invalid token is returned
    assert(token != NULL);
    //if we 1 token was retrned before, we have to rotate the array to get returned token to last token position,
    //position 1
    if (returned_token == 1)
    {
        token_t *tmp = tokens[1];
        tokens[1] = tokens[0];
        tokens[0] = tmp;
    }

    //counter of returned tokens incrementation
    returned_token++;

    //assert(tokens[1] == token);
}


void tokenLexOutput(){
    token_t *token=get_token();
    if(token->type==ERROR){
        printf("\n!!!ERROR!!!\n");
    }

    printf("%d\t|>%s<|\n",token->type,token->attribute->string);
    //printf("hello\n");

    while(token->type!=EOF){

        if(token->type==ERROR){
        printf("\n!!!ERROR!!!\n");
        }
        token=get_token();
        printf("%d\t|>%s<|\n",token->type,token->attribute->string);
    }
    
}


