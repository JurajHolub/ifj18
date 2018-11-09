//#include "scanner.h"
#include "test.h"
#include <stdio.h>

// Some simulation of scanner for testing purposes of parser.  
/* //Jakub's code for testing purposes
token_t t;
int cnt = -1;

struct tmp_t {
    char* attribute;
    int type;
};

struct tmp_t tokens[] = {
        {.type = VAR, .attribute = "id"},//id
        {.type = ASSIG, .attribute = ""},//+
        {.type = VAR, .attribute = "id"},//id
        {.type = EQUAL, .attribute = ""},//-
        {.type = VAR, .attribute = "id"},//id
        {.type = EOL, .attribute = ""}
};

// return always constant test aritmetic expression
token_t* get_token()
{
    if (cnt != -1)
        string_free(t.attribute);
    cnt++;
    t.type = tokens[cnt].type;
    t.attribute = string_create(tokens[cnt].attribute);
    //printf("get_token(%s)\n", t.attribute->string);
    return &t;
//    cnt++;
//
//    //printf("get_token(%s)\n", tokens[cnt].key);
//    return &tokens[cnt];
}

token_t tokenNext;
token_t tokens[10];
char nextchar='\0';

/*

typedef struct token_s {
    int type;   ///< Type of parsed symbol. Always value of enum type "data_type_e".
   
     * Value of parsed symbol.
     * NULL if type is one of *,/,-,+,(,) ...
     * String representation of parsed value if type is integer, float or string.
     * Name of variable if type is VAR (variable).
     
    string_t attribute; 
} token_t;

*/

int controlKeyWords(char testString[], token_t *token){


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

int controlConstants(char testString[],token_t *token){
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

int controlWords(char testString[], token_t *token){

    if(testString[0]=='_' || (testString[0] >= 'a' && testString[0] <= 'z') ){
        token->type=INDENTIFIER;
    }

    return TRUE;
}

int controlInt(char testString[],token_t *token){

    token->type=INTEGER;
    return TRUE;

}

int controlDouble(char testString[],token_t *token){

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
        token->type=LINE_END;
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

token_t get_token(){

    token_t returnToken;

    int commentSwitch=0;

    int stringSwitch=0;
    int quotationMarkCounter=0;

    char testString[64]={0};
    char c;

    int charCounter=0;


    int state=START_state;
    

    if(tokenNext.type==NOT_EQUAL){
        nextchar='\0';
        tokenNext.type=NOT;
        returnToken.type=NOT_EQUAL;
        return returnToken;

    }
    
        //printf(">%c<\n",nextchar);
       
    if( nextchar=='=' || nextchar =='!' || nextchar=='<' || nextchar == '>' ){

        state=SIGN_state;
    }
    else if( (nextchar>='a' && nextchar <= 'z')  || nextchar=='_'){
        state=ID_state;
        testString[charCounter]=nextchar;
        charCounter++;

    }
    else if (nextchar>='0' && nextchar <= '9'){
        state=INT_state;
        testString[charCounter]=nextchar;
        charCounter++;
    }
    else if(controlOperators(nextchar,&returnToken,'\0')==TRUE){
        nextchar='\0';
        return returnToken;
    }
   
    /**/

    while((c=getchar())!=EOF){

        if(state==SIGN_state){
            if( c=='=' || c =='!' || c=='<' || c == '>' ){

                if(controlOperators(nextchar,&returnToken,c)==TRUE){
                    nextchar='\0';
                    return returnToken;
                }

            }
            else if((c>='a' && c <= 'z')  || c=='_'){
                //treba už vracať token a pamätať si prvú vec čo je tuna
                if(controlOperators(nextchar,&returnToken,'\0')==TRUE){
                    nextchar=c;
                    return returnToken;
                }
            }
            else if (c>='0' && c <= '9'){
                if(controlOperators(nextchar,&returnToken,'\0')==TRUE){
                    nextchar=c;
                    return returnToken;
                }
            }
        }
        else if ( (c!=' '|| c!='\t') && state!=START_state && controlSigns(c)==TRUE){
                nextchar=c;
        }


        if ( c=='#' || commentSwitch==1){

            if (c=='\n'){
                commentSwitch=0;
                //returnToken.type=LINE_END;
                //return returnToken;
                //TODO problem with '\n' and the comments
            }
            else 
                commentSwitch=1;

        }
        else if(state==SIGN_state){
            
            

        }
        else if( c=='"' || stringSwitch==1){
            //printf("%c",c );
            if (c=='"' && quotationMarkCounter==0){

                quotationMarkCounter++;
                stringSwitch=1;
            
            }
            else if(c=='"' && quotationMarkCounter==1){
                quotationMarkCounter=0;
                stringSwitch=0;
            
            }

        }            
        
        else if (state==START_state){
            
            
            if ( (c>='a' && c <= 'z') || c=='_'){
                state=ID_state;
                testString[charCounter]=c;
                charCounter++;
            }
            else if(c>='0' && c <= '9'){

                state=INT_state;
                testString[charCounter]=c;
                charCounter++;
                
            }
            else if(controlOperators(c,&returnToken,'\0')==TRUE && (c!=' '|| c!='\t') ){
                return returnToken;
            }
            else if( c==' ' || c=='\t'){
                state=START_state;
            }

 

            


        }
        else if(state==FUNCTION_state){
            if(controlSigns(c)==TRUE && c!='='){
                returnToken.type=FUN;
                return returnToken;
            }
            else if(c=='='){
                testString[charCounter-1]='\0';
                tokenNext.type=NOT_EQUAL;
                returnToken.type=INDENTIFIER;
                return returnToken;


            }
            else{
                //error
            }
        }
        else if(state==ID_state){
            if((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') || (c>='0' && c <= '9')){
                state=ID_state;

                testString[charCounter]=c;
                charCounter++;

            }
            else if(c=='@'){


                if(controlConstants(testString,&returnToken)==TRUE);
                state=CONSTANT_state;
                testString[charCounter]=c;
                charCounter++;

            }
            else if(c=='?' || c== '!'){
                state=FUNCTION_state;
                testString[charCounter]=c;
                charCounter++;
            }
            else if(controlSigns(c)==TRUE){

                

                if(controlKeyWords(testString,&returnToken)==TRUE){
                    nextchar=c;
                    return returnToken;
                }      
                if(controlWords(testString,&returnToken)==TRUE){
                    nextchar=c;
                    return returnToken;
                } 

            }
        }
        else if(state==INT_state){
            if (c>='0' && c <= '9'){

                state=INT_state;
                testString[charCounter]=c;
                charCounter++;

            }
            else if( c== '.'){
                state=DOUBLE_state;
                testString[charCounter]=c;
                charCounter++;
            }
            else if(controlSigns(c)==TRUE){
                
                if(controlInt(testString,&returnToken)==TRUE) return returnToken;

            }
        }
        else if(state==DOUBLE_state){
            
            if (c>='0' && c <= '9'){

                testString[charCounter]=c;
                charCounter++;
            
            }
            else if(c=='e'|| c=='E'){
                //probably some error
                state=EXPONENT_state;
                testString[charCounter]=c;
                charCounter++;
            }
            else if (controlSigns(c)==TRUE){
                
                if(controlDouble(testString,&returnToken)==TRUE) return returnToken;
            }
            else{
                //error
            }

        }
        else if (state==EXPONENT_state){

            if(c>='0' && c <= '9'){

                state=EXPONENT_state;
                testString[charCounter]=c;
                charCounter++;
            }
            else if(c=='+' || c == '-'){
                state=EXPONENT_sign_state;
                testString[charCounter]=c;
                charCounter++;
            }
            else if (controlSigns(c)==TRUE) {
                returnToken.type=FLOAT;
                return returnToken;
            }
            else{
                //error
            }
        }
        else if(state==EXPONENT_sign_state){
            if(c>='0' && c <= '9'){

                state=EXPONENT_sign_state;
                testString[charCounter]=c;
                charCounter++;
            
            }
            else if (controlSigns(c)==TRUE)  {
                //printf("%s\n", testString);
                returnToken.type=FLOAT;
                return returnToken;
            }
            else{
                //error
            }

        }
        else if(state ==CONSTANT_state){
            if((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') || (c>='0' && c <= '9') || c=='-' || c=='+'){
                state=CONSTANT_state;
                testString[charCounter]=c;
                charCounter++;

            }
            else if (controlSigns(c)==TRUE){
                // TODO control of constant
                return returnToken;
            }
            else {
                //error
            }
        }
        

        
        
        
    }

    
}

//main for testing purposes
/*
int main(int argc, char const *argv[])
{
    
    
    token_t token;
    while (1){
        token=get_token();

        if(token.type<100 && token.type>-1){
            printf("%d\n",token.type);
        }
        else break;
    }
    /*token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);
    token=get_token();
    printf("%d\n",token.type);


    


    return 0;
}
/**/