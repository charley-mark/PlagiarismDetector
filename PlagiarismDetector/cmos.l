%{
//*****************************************************************************************
//  @author : Mark Tetteh
//
//
// File: cmos.l
// Purpose: To scan through C code files and tokenize significant elements for plagiarism
// detection. This lexer aims to capture keywords, operators, numbers, and specific patterns
// while ignoring comments and whitespace.
//*****************************************************************************************

#include <stdio.h>
#include <iostream>
#include <fstream>

#define TK_KEYWORD 1
#define TK_OPERATOR 2
#define TK_NUMBER 3
#define TK_IDENTIFIER 4
#define TK_STRING 5
#define TK_CHARACTER 6
#define TK_COMMENT 7
#define TK_PREPROCESSOR 8
#define TK_PUNCTUATIONS 9
#define TK_CONTROL_STRUCTURE 10
#define TK_LOOPS 11
#define TK_CURLYBRACES 12
#define TK_UNMATCHED 255

int linecount = 1;


void Addline(const char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            linecount++;
        }
    }
}

int line_num() {
    return linecount;
}

%}

%%

[ \t]+                                                                                           { /* Ignore whitespace */ }

\n                                                                                               { Addline(yytext); }

"//"[^"\n"]*                                                                                     { Addline(yytext);  }

"/*"([^*]|"*"+[^*/])*"*"+"/"                                                                     { Addline(yytext); }

"#"[^"\n"]*                                                                                      { return TK_PREPROCESSOR; }

"int"|"char"|"float"|"double"|"struct"|"return"|"void"|"long"|"unsigned"                         { return TK_KEYWORD; }

"+"|"-"|"*"|"/"|"="|"=="|"!="|">"|"<"|">="|"<="|"&&"|"||"|"!"|"++"|"--"|"&"|"|"|"^"|"~"|"<<"|">>"|"+="|"-="|"="|"/="|"%="|"&="|"|="|"^="|"<<="|">>=" { return TK_OPERATOR; }

[()\[\],;]                                                                                       { return TK_PUNCTUATIONS; }

\{|\}                                                                                            { return TK_CURLYBRACES; }

[0-9]+("."[0-9]+)?([eE][-+]?[0-9]+)?                                                             { return TK_NUMBER; }

0[xX][0-9a-fA-F]+                                                                                { return TK_NUMBER; }

if\s*\(.*?\)|else\s*\(.*?\)|switch\s*\(.*?\)|case\s*.*?:|break\s*;|continue\s*;|return\s*;       { return TK_CONTROL_STRUCTURE; }

for\s*\(.*?\)|while\s*\(.*?\)|do\s*\{.*?\}\s*while\s*\(.*?\)                                     {return TK_LOOPS;}

\"(\\.|[^\\"])*\"                                                                                { return TK_STRING; }

\'(\\.|[^\\'])*\'                                                                                { return TK_CHARACTER; }

[a-zA-Z_][a-zA-Z0-9_]*                                                                           { return TK_IDENTIFIER; }

.                                                                                                { return TK_UNMATCHED; }

%%

int yywrap() {
     return 1;
 }



int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename1> [<filename2> ...]\n", argv[0]);
        return 1;
    }

    for (int file_index = 1; file_index < argc; ++file_index) {
        FILE *input_file = fopen(argv[file_index], "r");
        if (!input_file) {
            perror("Failed to open input file");
            continue;
        }

        yyin = input_file;

        char *filename = basename(strdup(argv[file_index]));

        printf("%s ", filename);

        int token = 0;
        while ((token = yylex()) != 0) {
            printf("%d ", token);
        }

        printf("\n"); 

        fclose(input_file);
    }

    return 0;
}