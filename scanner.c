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

//Global vars
token_t enterToken;
token_t tokenNext;
char nextchar;//='\0';
int globalVar=1;

token_t *tokens[2] = {NULL, NULL};
int returned_token = 0;
int get= 1;


int globalEOF=FALSE;
int globalEOL=FALSE;

int beginVar=TRUE;

/**
 * @brief Function controling wheter a string is a keyword
 */

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

/**
 * @brief control wheter it is correctly passed word 
 */


int controlWords(char *testString, token_t *token){

    if(testString[0]=='_' || (testString[0] >= 'a' && testString[0] <= 'z') ){
        token->type=VAR;
    }

    return TRUE;
}


/**
 * @brief Control wheter string is a INT
 */

int controlInt(char *testString,token_t *token){

    char *emptyString='\0';
    //emptyString[0]='\0';
    
    int i=-1;

    i=strtol(testString,&emptyString,10);
    //printf("%d\n",i );

    if (emptyString[0]!='\0'){
        return FALSE;
    }
    else if(emptyString[0]=='\0' && i==0){
        return FALSE;
    }

    token->type=INTEGER;
    return TRUE;

}

/**
 * @brief Control wheter string is a float/double
 */

int controlDouble(char *testString,token_t *token){

    for (int i = 0; i < strlen(testString); i++){

        if( (testString[i]>='0' && testString[i]<='9') || testString[i]=='e' 
            || testString[i]=='E' || testString[i]=='+' || testString[i]=='-' || testString[i]=='*' || testString[i]=='.');
        else
            return FALSE;
        
    }

    
    if(testString[0]=='\0'){
        return FALSE;
    }

    token->type=FLOAT;
    return TRUE;

}



/**
 * @brief Control and assing type of operators to the token
 */

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

/**
 * @brief controls the chars that are not allowed in the IFJ18
 */
int controlIncorrectChars(char c){
    if(c>128 || c<-1){
        return FALSE;
    }

    char operators[]={';','}','{',':','[',']','^','_','`','@','.','|','$','%','&','?','~','\\'};

    for (int i = 0; i < strlen(operators); i++){
        if( c==operators[i]){
            return TRUE;
        }
    }



    return FALSE;
}


/**
 * @brief controls the deliminators
 */

int controlSigns(char c){

    char operators[]={' ','\n','\t','*','/','+','-','<','=','>','(',')',',','"','#'};

    for (int i = 0; i < strlen(operators); i++){
        if( c==operators[i]){
            return TRUE;
        }
    }

    return FALSE;

}

/**
 * @brief Appends single char to a string
 */
void charAppend(token_t *token,char c){

    
    char append_char[5]={0};
    append_char[0]=c;

    string_append_ch(token->attribute,append_char);
}

/**
 * @brief used in the HEX escape string sequance
 */

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


    
    //This condition is here because I will always have to return the first token as EOL token
    if(beginVar==TRUE ){

        beginVar=FALSE;
        enterToken.type=EOL;
        tokens[1]->type = EOL;
        return tokens[1];
    }


    char hexConvert[]="00";
    char testString[64]={0};
    char hexReturn[4]={0};
    char c;
    int charCounter=0;


    int state=START_state;
    int state2=N_state;

    //This condition is used to give EOL token as a before EOF token 

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




    //used for case of '!=' operator, because it needs to decide wheter it is a function or operator

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

    
    //Control of nextchar aka next character that needs to be remembered
       
    if( nextchar=='=' || nextchar =='!' || nextchar=='<' || nextchar == '>' ){

        state=SIGN_state;
    }
    else if( (nextchar>='a' && nextchar <= 'z')  || nextchar=='_'){
        state=ID_state;

        charAppend(tokens[1],nextchar);
        

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
        
    }
    else if(nextchar=='#'){
        state=COMMENT_state;
        nextchar='\0';
    }
    else if(controlOperators(nextchar,tokens[1],'\0')==TRUE){
        
        nextchar='\0';
        PRINT_TOKENS
        return tokens[1];
    }
    else if(controlIncorrectChars(nextchar)==TRUE){

        tokens[1]->type=ERROR;
        PRINT_TOKENS
        return tokens[1];
    
    }
   
    


    while( (c=getchar()) ){



        

        //Control of EOL before EOF
        //special case for EOF
        if(c==EOF){
            

            if(state2==COMMENT_BEGIN_state){
                tokens[1]->type = EOF;
                return tokens[1];
            }


            if(state==STRING_state){
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            } 
            else if ( state==COMMENT_BEGIN_state || state== COMMENT_END_state){
                globalEOL=TRUE;
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }
            else if (state==INT_state && controlInt(tokens[1]->attribute->string, tokens[1]) == TRUE){
                
                globalEOL=TRUE;
                PRINT_TOKENS
                return tokens[1];
            
            }
            else if(state==DOUBLE_PROB_state){

                globalEOL=TRUE;
                PRINT_TOKENS
                tokens[1]->type=INTEGER;
                return tokens[1];
            
            }
            else if(state==SIGN_state){
                
                //it is a bad state but parser should detect it
                globalEOL=TRUE;
                if(controlOperators(nextchar,tokens[1],'\0') == TRUE){
                    PRINT_TOKENS
                    return tokens[1]; 
                }
                
            
            }
            else if((state==EXPONENT_sign_state || state==DOUBLE_state || state== EXPONENT_state) && controlDouble(tokens[1]->attribute->string, tokens[1]) == TRUE){
            
                globalEOL=TRUE;
                PRINT_TOKENS
                return tokens[1];
            
            }
            else if((state==FUNCTION_state || state==ID_state) && controlKeyWords(tokens[1]->attribute->string, tokens[1]) == TRUE){
                
                globalEOL=TRUE;
                PRINT_TOKENS
                return tokens[1];
            
            }  
            else if((state==FUNCTION_state || state==ID_state) && controlWords(tokens[1]->attribute->string,tokens[1])==TRUE){
                
                globalEOL=TRUE;
                PRINT_TOKENS
                return tokens[1];
            
            }
            else if(state==START_state){
                globalEOL=TRUE;
                tokens[1]->type=EOL;
                PRINT_TOKENS
                return tokens[1];
            }
            else if(state==COMMENT_state){
                globalEOL=TRUE;
                tokens[1]->type=EOL;
                PRINT_TOKENS
                return tokens[1];
            }
            else if( state==DOUBLE_DOT_state || state==EXPONENT_INTER_state || state==EXPONENT_INTER_sign_state){
                globalEOL=TRUE;
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }
            

    
            

            tokens[1]->type = EOF;
            return tokens[1];
            
        }



        //control of '==','<=','!=','>=' comparators
        if(state==SIGN_state){

            if( c=='=' ){
                
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
            else if(c=='(' || c== ')' || c=='+' || c=='-' || c== '*' || c == '/' ){
                if(controlOperators(nextchar,tokens[1],'\0')==TRUE){
                    nextchar=c;
                    PRINT_TOKENS
                    return tokens[1];
                }
            }
            else if(c=='<' || c=='!' || c=='>'){
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }
            else if(controlIncorrectChars(c)==TRUE){
                PRINT_TOKENS
                tokens[1]->type=ERROR;
                return tokens[1];
            }
        }
        else if ( (c!=' '|| c!='\t') && state!=START_state && controlSigns(c)==TRUE){
                nextchar=c;
        }



        if ( state==COMMENT_state){

            
            if (c=='\n'){
                
                PRINT_TOKENS
                tokens[1]->type=EOL;
                return tokens[1];

            }


        }            
        else if (state==STRING_state){

            //ERROR
            if(c>255 || c<-2){
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }
            

            if(state2==ESCAPE_state){
                if(c=='t'){
                    //c='\t';

                    string_append_ch(tokens[1]->attribute,"\\009");
                    
                    state2=N_state;
                    //vrátitť sa a vynulovať stav
                    //it returns and "NULL" the state 
                }
                else if (c=='"'){
                    string_append_ch(tokens[1]->attribute,"\\034");
                    state2=N_state;
                }
                else if (c=='n'){
                
                    

                    string_append_ch(tokens[1]->attribute,"\\010");
                    state2=N_state;
                
                }
                else if (c=='s'){
                    
                    
                    string_append_ch(tokens[1]->attribute,"\\032");
                    state2=N_state;
                
                }
                else if (c==92){
                    string_append_ch(tokens[1]->attribute,"\\092");
                    
                    state2=N_state;
                
                }
                else if (c=='x'){

                    state2=HEX_ESCAPE_state;
                    
                
                }
                else {
                    tokens[1]->type=ERROR;
                    PRINT_TOKENS
                    return tokens[1];
                }

            }
            //
            else if(state2==HEX_ESCAPE_state){

                if( (c>='0' && c <= '9') || (c>='A' && c <= 'F') || (c>='a' && c <= 'f')  ){//TODO check wheter it should be capital or noncapital letters //it has been DONE
                    hexConvert[0]=c;
                    state2=HEX_ESCAPE2_state;
                }
                else{
                    //ERROR
                    tokens[1]->type=ERROR;
                    PRINT_TOKENS
                    return tokens[1];
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
                else if(c=='\\'){

                    hexConvert[1]=hexConvert[0];
                    hexConvert[0]='0';

                   
                    hexConvertFoo(hexConvert,hexReturn);
                    string_append_ch(tokens[1]->attribute,hexReturn);

                    state2=ESCAPE_state;

                    nextchar='\0';
                    
                }
                else if(c=='"') {
                    
                    hexConvert[1]=hexConvert[0];
                    hexConvert[0]='0';

                    
                    hexConvertFoo(hexConvert,hexReturn);
                    string_append_ch(tokens[1]->attribute,hexReturn);

                    state2=N_state;

                    nextchar='\0';
                    tokens[1]->type=STRING;
                    return tokens[1];
                }
                else{
                    
                    hexConvert[1]=hexConvert[0];
                    hexConvert[0]='0';

                    
                    hexConvertFoo(hexConvert,hexReturn);
                    string_append_ch(tokens[1]->attribute,hexReturn);

                    state2=N_state;

                }

            }
            ////92=='\'
            else if ( c== 92){

                
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
            else if(c =='\n' || c==EOF || c>255){
                
                tokens[1]->type=ERROR;
                return tokens[1];
            }
            else if(c<32){
                tokens[1]->type=ERROR;
                return tokens[1];
            }
            
            else{
                
                charAppend(tokens[1],c);
            }

            
            
        }
        else if (state==START_state){
            //this is the start state
            
            if ( (c>='a' && c <= 'z') || c=='_'){

                state=ID_state;
                charAppend(tokens[1],c);

                
            }
            else if(c=='#'){
                
                state=COMMENT_state;
            
            }
            else if(c=='0'){

                state=DOUBLE_PROB_state;
                charAppend(tokens[1],c);
            
            }
            else if(c>='1' && c <= '9'){

                state=INT_state;
                charAppend(tokens[1],c);
                
                
            }
            else if(c=='=' && state2==COMMENT_BEGIN_state){
                
                state2=N_state;
                state=COMMENT_BEGIN_state;

            }
            else if(c>='A' && c <= 'Z'){
                //ERROR
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
                state2=NOT;
            }
            else if ( controlIncorrectChars(c)==TRUE ){

                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            
            }
                

 

            


        }
        else if(state==COMMENT_BEGIN_state){
            
            testString[charCounter]=c;
            charCounter++;

            if(charCounter==6){

                //watch out for the whitespaces
                char beginTest1[]="begin ";
                char beginTest2[]="begin\t";
                char beginTest3[]="begin\n";


                if(strcmp(testString,beginTest3) == 0){


                    for (int i = 0; i < 64; i++)testString[i]='\0';
                    charCounter=0;

                    state=COMMENT_END_ENTER_state;
                }
                else if(strcmp(testString,beginTest1) == 0 || strcmp(testString,beginTest2) == 0){


                    //We "erase" the array and NULL the cnt
                    for (int i = 0; i < 64; i++)testString[i]='\0';
                    charCounter=0;

                    state=COMMENT_END_state;
                }
                else{
                    tokens[1]->type=ERROR;
                    PRINT_TOKENS
                    return tokens[1];
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
                char endTestEOF[]="end";
                if(strcmp(testString,endTestEOF) == 0 ){
                    state2=COMMENT_END_ENTER2_tillEnd_state;
                }
            }

            if(charCounter==4){

                char endTest1[]="end ";
                char endTest2[]="end\t";
                char endTest3[]="end\n";
                
                if(strcmp(testString,endTest1) == 0 || strcmp(testString,endTest2) == 0 ){
                    state=COMMENT_END_ENTER2_tillEnd_state;
                    state2=NOT;
                }
                else if(strcmp(testString,endTest3) == 0){
                    state2=NOT;
                    tokens[1]->type=EOL;
                    PRINT_TOKENS
                    return tokens[1];
                }
                else{

                    state=COMMENT_END_state;
                    //not good
                    //We "erase" the array and NULL the cnt
                    for (int i = 0; i < 64; ++i)
                        testString[i]='\0';
                    
                    charCounter=0;
                
                }

            }
        }
        else if(state==COMMENT_END_ENTER2_tillEnd_state){
            if(c=='\n'){
                state=START_state;
            }
        }
        else if(state==FUNCTION_state){
            if(controlSigns(c)==TRUE && c!='='){
                tokens[1]->type=FUN;
                PRINT_TOKENS
                return tokens[1];
            }
            else if(c=='='){


                


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
                //ERROR
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }
        }
        else if(state==ID_state){
            if((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') || (c>='0' && c <= '9') || c=='_'){
                state=ID_state;

                charAppend(tokens[1],c);


            }
            
            else if(c=='?' || c== '!'){
                state=FUNCTION_state;

                charAppend(tokens[1],c);
                
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
            else if(controlIncorrectChars(c)==TRUE){

                //constants are not supported in a source file

                

                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];

            }


        }
        else if(state==INT_state){
            if (c>='0' && c <= '9'){

                state=INT_state;

                charAppend(tokens[1],c);
                

            }
            else if( c== '.'){
                state=DOUBLE_DOT_state;
                charAppend(tokens[1],c);
                
            }
            else if(c=='e' || c=='E'){

                state=EXPONENT_INTER_state;
                charAppend(tokens[1],c);
            
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
            else if(controlIncorrectChars(c)==TRUE){

                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];

            }
        }
        else if(state==DOUBLE_DOT_state){//basically this state process the '0'
            if( c>='0' && c <= '9' ){
                state=DOUBLE_state;
                charAppend(tokens[1],c);
            }
            else{
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }

        }
        else if(state==DOUBLE_PROB_state){
            
            if (c=='.') {
                state=DOUBLE_DOT_state;
                charAppend(tokens[1],c);
            }

            else if (controlSigns(c)==TRUE){
                

                PRINT_TOKENS
                tokens[1]->type=INTEGER;
                return tokens[1];

            }
            else if(c=='e' || c=='E'){
                state=EXPONENT_INTER_state;
                charAppend(tokens[1],c);
            }
            else if ((c>='1' && c <= '9') || (c>='A' && c <= 'Z') || (c>='0' && c <= '9') || c=='_'){
                //ERROR
            
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            
            }
            else if(controlIncorrectChars(c)==TRUE){
                //ERROR
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];

            }

        }
        else if(state==DOUBLE_state){
            
            if (c>='0' && c <= '9'){
                charAppend(tokens[1],c);
                
            
            }
            else if(c=='e'|| c=='E'){
                //probably some error
                //TODO chyták //DONE
                state=EXPONENT_INTER_state;
                charAppend(tokens[1],c);
                
            }
            else if (controlSigns(c) == TRUE){
                
                if( controlDouble(tokens[1]->attribute->string, tokens[1]) == TRUE) {
                    PRINT_TOKENS
                    return tokens[1];
                }
            }
            else if ((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') ||  c=='_'){
                //ERROR

                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];

            }
            else if(controlIncorrectChars(c)==TRUE){

                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];

            }

        }
        else if(state==EXPONENT_INTER_state){
            
            if (c>='0' && c <= '9'){
                charAppend(tokens[1],c);
                
                state=EXPONENT_state;
            
            }
            else if(c=='-' || c == '+'){
                charAppend(tokens[1],c);
                state=EXPONENT_INTER_sign_state;
            }
            else{
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }
        }
        else if (state==EXPONENT_INTER_sign_state){
            if (c>='0' && c <= '9'){
                charAppend(tokens[1],c);
                
                state=EXPONENT_sign_state;
            
            }
            else {
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }
        }
        else if (state==EXPONENT_state){

            if(c>='0' && c <= '9'){

                state=EXPONENT_state;
                charAppend(tokens[1],c);
                
            }
            
            else if (controlSigns(c)==TRUE) {

                tokens[1]->type=FLOAT;
                PRINT_TOKENS
                return tokens[1];
            }
            else if ((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') ||  c=='_' || c==';' || c=='@'){
                //ERROR

                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];

            }
            else if (controlIncorrectChars(c)==TRUE){

                //ERROR
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];

            }

        }
        else if(state==EXPONENT_sign_state){
            if(c>='0' && c <= '9'){

                state=EXPONENT_sign_state;
                charAppend(tokens[1],c);
                
            
            }
            else if (controlSigns(c)==TRUE)  {
                
                tokens[1]->type=FLOAT;
                PRINT_TOKENS
                return tokens[1];
            }
            else if ((c>='a' && c <= 'z') || (c>='A' && c <= 'Z') ||  c=='_' || c==';' || c=='@'){
                //ERROR
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }
            else if (controlIncorrectChars(c)==TRUE){
                tokens[1]->type=ERROR;
                PRINT_TOKENS
                return tokens[1];
            }

        }
        
        else {
            //DEAD "branch" did some bugs so I defined errors in different functions
            
        }
        
        
    }


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

//dummy function to write out all the tokens from source file
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
