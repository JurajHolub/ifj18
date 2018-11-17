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

token_t enterToken;
token_t tokenNext;
char nextchar;//='\0';
int globalVar=1;

token_t *tokens[2] = {NULL, NULL};
int returned_token = 0;
int get= 1;


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
        else if (strcmp(testString,keyWords[i]) == 0 && i ==5 ){
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

    token->type=INTEGER;
    return TRUE;

}

int controlDouble(char *testString,token_t *token){

    token->type=FLOAT;
    return TRUE;

}




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
    else if (c=='=' && scndOperator=='!'){
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

    char operators[]={' ','\n','\t','*','/','+','-','<','=','>','(',')',','};

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

void clean(void) {
    if (tokens[1] != NULL)
    {
        if (tokens[1]->attribute == NULL)
        {
            free((void *) tokens[1]->attribute);
        }
        free((void *) tokens[1]);
    }
    if (tokens[1] != NULL)
    {
        if (tokens[1]->attribute == NULL)
        {
            free((void *) tokens[1]->attribute);
        }
        free((void *) tokens[2]);
    }
}

token_t *get_token(){

    printf("get_token\n");

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
        free((void *) tokens[0]);
    }
    tokens[0] = tokens[1];
    //allocation of new token
    tokens[1] = (token_t *) malloc(sizeof(token_t));
    if (tokens[1] == NULL)
    {
        fprintf(stderr, "Malloc fail\n");
        clean();
    }
    //data will be achieved and inserted in token

    //token_t *tokens[1]=malloc(sizeof(token_t));
    char *str = NULL;
    string_t string=string_create(str);
    tokens[1]->attribute=string;


    int commentSwitch=0;


    char testString[64]={0};
    
    char c;

    int charCounter=0;


    int state=START_state;
    int state2=N_state;



    





    //char *testString_IFJ;
    //string_t stringTest=string_create(testString_IFJ);

    if(tokenNext.type==NOT_EQUAL){

        nextchar='\0';
        tokenNext.type=NOT;
        tokens[1]->type=NOT_EQUAL;
	    return tokens[1];

    }

    if(enterToken.type==EOL){

        enterToken.type=NOT;
        state2=COMMENT_BEGIN_state;
    
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
    else if (nextchar>='0' && nextchar <= '9'){
        state=INT_state;
        charAppend(tokens[1],nextchar);
        /*
        testString[charCounter]=nextchar;
        charCounter++;*/
    }
    else if(controlOperators(nextchar,tokens[1],'\0')==TRUE){
        nextchar='\0';
        return tokens[1];
    }
   
    /**/

    while((c=getchar())!=EOF){

        if(state==SIGN_state){
            if( c=='=' ){

                if(controlOperators(nextchar,tokens[1],c)==TRUE){
                    nextchar='\0';
                    return tokens[1];
                }

            }
            else if((c>='a' && c <= 'z')  || c=='_'){
                //treba už vracať token a pamätať si prvú vec čo je tuna
                if(controlOperators(nextchar,tokens[1],'\0')==TRUE){
                    nextchar=c;
                    return tokens[1];
                }
            }
            else if (c>='0' && c <= '9'){
                if(controlOperators(nextchar,tokens[1],'\0')==TRUE){
                    nextchar=c;
                    return tokens[1];
                }
            }
        }
        else if ( (c!=' '|| c!='\t') && state!=START_state && controlSigns(c)==TRUE){
                nextchar=c;
        }


        if ( c=='#' || commentSwitch==1){

            if (c=='\n'){
                commentSwitch=0;
                //tokens[1].type=LINE_END;
                //printf("get_token %s\n", tokens[1]->attribute->string); 
                 return tokens[1];
                //TODO problem with '\n' and the comments
            }
            else 
                commentSwitch=1;

        }            
        else if (state==STRING_state){
            
            if(c=='"'){
                tokens[1]->type=STRING;
            }

            charAppend(tokens[1],c);
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
            else if(c>='0' && c <= '9'){

                state=INT_state;
                charAppend(tokens[1],c);
                /*testString[charCounter]=c;
                charCounter++;*/
                
            }
            else if(c=='=' && state2==COMMENT_BEGIN_state){
                
                state2=N_state;
                state=COMMENT_BEGIN_state;

            }
            else if(c=='=' || c =='!' || c=='<' || c == '>' ){
                nextchar=c;
                state=SIGN_state;
            }
            else if(c=='"'){
                state=STRING_state;
            }
            else if(controlOperators(c,tokens[1],'\0')==TRUE && (c!=' '|| c!='\t') ){
                return tokens[1];
            }
            else if( c==' ' || c=='\t'){
                state=START_state;
            }

 

            


        }
        else if(state==COMMENT_BEGIN_state){
            
            testString[charCounter]=c;
            charCounter++;

            if(charCounter==5){//error by one
                char beginTest[]="begin";
                if(strcmp(testString,beginTest) == 0){

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
                }

            }
        }
        else if(state==FUNCTION_state){
            if(controlSigns(c)==TRUE && c!='='){
                tokens[1]->type=FUN;
                return tokens[1];
            }
            else if(c=='='){
                testString[charCounter-1]='\0';
                tokenNext.type=NOT_EQUAL;
                tokens[1]->type=VAR;
                return tokens[1];


            }
            else{
                //error
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


                if(controlConstants(testString,tokens[1])==TRUE);
                state=CONSTANT_state;
                charAppend(tokens[1],c);

                /*testString[charCounter]=c;
                charCounter++;*/

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
                    return tokens[1];
                }      
                if(controlWords(tokens[1]->attribute->string,tokens[1])==TRUE){
                    nextchar=c;
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
                
                if(controlInt(tokens[1]->attribute->string, tokens[1]) == TRUE) 
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
                
                if( controlDouble(tokens[1]->attribute->string, tokens[1]) == TRUE) 
                    return tokens[1];
            }
            else{
                //error
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
                return tokens[1];
            }
            else{
                //error
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
                return tokens[1];
            }
            else{
                //error
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
                return tokens[1];
            }
            else {
                //error
            }
        }
        

        
        
        
    }

     tokens[1]->type = EOF;
     tokens[1]->attribute = string_create(NULL);
	 return tokens[1];

}

void ret_token(token_t* token)
{
    printf("ret_token\n");
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



