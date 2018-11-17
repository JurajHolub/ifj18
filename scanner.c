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
#define TRUE 1
#define FALSE 0

token_t enterToken;
token_t tokenNext;
char nextchar;//='\0';
int globalVar=1;

int get = -1;
int ret = -1;
token_t tokens[2];


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


token_t *get_token(){

    //if (get == 1)
    //    printf("tokans[0].at\"%s\", type=%d \t\ttokans[1].at\"%s\", type=%d\n",tokens[0].attribute->string, tokens[0].type,tokens[1].attribute->string, tokens[1].type);

    if ((get == -1 && ret == -1) || (get == 0 && ret == 0))
    {
        get++;
        ret++;
//    fprintf(stderr, "get_token(), get %d, ret %d\n", get, ret);
    }
    else if (get == 1 && ret == 1)
    {
        tokens[0].attribute = tokens[1].attribute;
        tokens[0].type = tokens[1].type;
//    fprintf(stderr, "get_token(), get %d, ret %d\n", get, ret);
    }
    else if (get == 1 && ret == 0)
    {
        ret++;
//    fprintf(stderr, "get_token(), get %d, ret %d\n", get, ret);
    //    printf("get_token %s\n", tokens[1].attribute->string); 

        return &tokens[1];
    }
    else if (get == 0 && ret == -1)
    {
        ret++;
//    fprintf(stderr, "get_token(), get %d, ret %d\n", get, ret);
      //  printf("get_token %s\n", tokens[0].attribute->string); 
	    return &tokens[0];
    }
    else if (get == 1 && ret == -1)
    {
        ret++;

//    fprintf(stderr, "get_token(), get %d, ret %d\n", get, ret);
     //   printf("get_token %s\n", tokens[0].attribute->string); 
	    return &tokens[0];
    }

    //token_t *&tokens[get]=malloc(sizeof(token_t));
    char *str = NULL;
    string_t string=string_create(str);
    tokens[get].attribute=string;


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
        tokens[get].type=NOT_EQUAL;
	    return &tokens[get];

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

        charAppend(&tokens[get],nextchar);
        

        /*
        testString[charCounter]=nextchar;
        charCounter++;*/

    }
    else if (nextchar>='0' && nextchar <= '9'){
        state=INT_state;
        charAppend(&tokens[get],nextchar);
        /*
        testString[charCounter]=nextchar;
        charCounter++;*/
    }
    else if(controlOperators(nextchar,&tokens[get],'\0')==TRUE){
        nextchar='\0';
        return &tokens[get];
    }
   
    /**/

    while((c=getchar())!=EOF){

        if(state==SIGN_state){
            if( c=='=' ){

                if(controlOperators(nextchar,&tokens[get],c)==TRUE){
                    nextchar='\0';
                    return &tokens[get];
                }

            }
            else if((c>='a' && c <= 'z')  || c=='_'){
                //treba už vracať token a pamätať si prvú vec čo je tuna
                if(controlOperators(nextchar,&tokens[get],'\0')==TRUE){
                    nextchar=c;
                    return &tokens[get];
                }
            }
            else if (c>='0' && c <= '9'){
                if(controlOperators(nextchar,&tokens[get],'\0')==TRUE){
                    nextchar=c;
                    return &tokens[get];
                }
            }
        }
        else if ( (c!=' '|| c!='\t') && state!=START_state && controlSigns(c)==TRUE){
                nextchar=c;
        }


        if ( c=='#' || commentSwitch==1){

            if (c=='\n'){
                commentSwitch=0;
                //&tokens[get].type=LINE_END;
                //printf("get_token %s\n", tokens[get].attribute->string); 
                 return &tokens[get];
                //TODO problem with '\n' and the comments
            }
            else 
                commentSwitch=1;

        }            
        else if (state==STRING_state){
            
            if(c=='"'){
                tokens[get].type=STRING;
            }

            charAppend(&tokens[get],c);
            /*
            testString[charCounter]=c;
            charCounter++;*/
        }
        else if (state==START_state){
            
            
            if ( (c>='a' && c <= 'z') || c=='_'){

                state=ID_state;
                charAppend(&tokens[get],c);

                /*testString[charCounter]=c;
                charCounter++;*/
            }
            else if(c>='0' && c <= '9'){

                state=INT_state;
                charAppend(&tokens[get],c);
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
            else if(controlOperators(c,&tokens[get],'\0')==TRUE && (c!=' '|| c!='\t') ){
                return &tokens[get];
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
                tokens[get].type=FUN;
                printf("get_token %s\n", tokens[get].attribute->string); 
                return &tokens[get];
            }
            else if(c=='='){
                testString[charCounter-1]='\0';
                tokenNext.type=NOT_EQUAL;
                tokens[get].type=VAR;
                return &tokens[get];


            }
            else{
                //error
            }
        }
        else if(state==ID_state){
            if((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') || (c>='0' && c <= '9') || c=='_'){
                state=ID_state;

                charAppend(&tokens[get],c);

                /*testString[charCounter]=c;
                charCounter++;*/

            }
            else if(c=='@'){


                if(controlConstants(testString,&tokens[get])==TRUE);
                state=CONSTANT_state;
                charAppend(&tokens[get],c);

                /*testString[charCounter]=c;
                charCounter++;*/

            }
            else if(c=='?' || c== '!'){
                state=FUNCTION_state;

                charAppend(&tokens[get],c);
                /*testString[charCounter]=c;
                charCounter++;*/
            }
            else if(controlSigns(c)==TRUE){

                

                if(controlKeyWords(tokens[get].attribute->string, &tokens[get]) == TRUE){
                    nextchar=c;
                    return &tokens[get];
                }      
                if(controlWords(tokens[get].attribute->string,&tokens[get])==TRUE){
                    nextchar=c;
                    return &tokens[get];
                } 

            }
        }
        else if(state==INT_state){
            if (c>='0' && c <= '9'){

                state=INT_state;

                charAppend(&tokens[get],c);
                /*testString[charCounter]=c;
                charCounter++;*/

            }
            else if( c== '.'){
                state=DOUBLE_state;
                charAppend(&tokens[get],c);
                /*testString[charCounter]=c;
                charCounter++;*/
            }
            else if(controlSigns(c)==TRUE){
                
                if(controlInt(tokens[get].attribute->string, &tokens[get]) == TRUE) 
                    return &tokens[get];

            }
        }
        else if(state==DOUBLE_state){
            
            if (c>='0' && c <= '9'){
                charAppend(&tokens[get],c);
                /*testString[charCounter]=c;
                charCounter++;*/
            
            }
            else if(c=='e'|| c=='E'){
                //probably some error
                //TODO chyták
                state=EXPONENT_state;
                charAppend(&tokens[get],c);
                /*testString[charCounter]=c;
                charCounter++;*/
            }
            else if (controlSigns(c) == TRUE){
                
                if( controlDouble(tokens[get].attribute->string, &tokens[get]) == TRUE) 
                    return &tokens[get];
            }
            else{
                //error
            }

        }
        else if (state==EXPONENT_state){

            if(c>='0' && c <= '9'){

                state=EXPONENT_state;
                charAppend(&tokens[get],c);
                /*testString[charCounter]=c;
                charCounter++;*/
            }
            else if(c=='+' || c == '-'){
                state=EXPONENT_sign_state;
                charAppend(&tokens[get],c);
                /*
                testString[charCounter]=c;
                charCounter++;*/
            }
            else if (controlSigns(c)==TRUE) {
                tokens[get].type=FLOAT;
                return &tokens[get];
            }
            else{
                //error
            }
        }
        else if(state==EXPONENT_sign_state){
            if(c>='0' && c <= '9'){

                state=EXPONENT_sign_state;
                charAppend(&tokens[get],c);
                /*testString[charCounter]=c;
                charCounter++;*/
            
            }
            else if (controlSigns(c)==TRUE)  {
                //printf("%s\n", testString);
                tokens[get].type=FLOAT;
                return &tokens[get];
            }
            else{
                //error
            }

        }
        else if(state ==CONSTANT_state){
            if((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') || (c>='0' && c <= '9') || c=='-' || c=='+'){
                state=CONSTANT_state;
                charAppend(&tokens[get],c);
                /*
                testString[charCounter]=c;
                charCounter++;*/

            }
            else if (controlSigns(c)==TRUE){
                // TODO control of constant
                return &tokens[get];
            }
            else {
                //error
            }
        }
        

        
        
        
    }

     tokens[get].type = EOF;
     tokens[get].attribute = string_create(NULL);
	 return &tokens[get];

}

void ret_token(token_t* token)
{
    //if (get == 1)
    //    printf("tokans[0].at\"%s\", type=%d \t\ttokans[1].at\"%s\", type=%d\n",tokens[0].attribute->string, tokens[0].type,tokens[1].attribute->string, tokens[1].type);
    if (ret == -1)
        fprintf(stderr, "Return token older than 2 before.\n");
    if (tokens[ret].type != token->type || !string_is_equal(tokens[ret].attribute ,token->attribute))
        fprintf(stderr, "Return not last token.\n");

    //printf("ret_token %s\n", token->attribute->string);
    ret--;
//    fprintf(stderr, "ret_token(), get %d, ret %d\n", get, ret);
}



