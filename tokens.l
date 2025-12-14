%{
/*
 */

#include "parse.tab.h"
# define YYLMAX 200
#include <stdlib.h>
#include <string.h>
extern int yylineno;
int yylook();
int yyback(int*,int);
int yywrap();
extern YYSTYPE yylval;
static int scanTable = 0;

static void skipComments();
unsigned long stringToNumber(const char *s);
%}
%a 2000
%e 1500
%k 1000
%n 500
%p 4000

%Start Snormal Stable

Space       [\r\t\b ]
Neline      [\n]
Alpha       [a-zA-Z]
AlphaU      [a-zA-Z_]
AlphaNum    [a-zA-Z0-9]
AlphaNumU   [a-zA-Z0-9_]
AlphaNumC   [a-zA-Z0-9_\-\[\]\{\}\"\/\&\*]
CtrStr      \\[^"]
Digit       [0-9]
DigitU      [0-9_]
Number      {Digit}{DigitU}*
Decimal     ({Number})?'[dD]{Number}
Octal       ({Number})?'[oO][0-7xXzZ?][0-7xXzZ?_]*
Hexdecimal  ({Number})?'[hH][0-9a-fA-FxXzZ?][0-9a-fA-FxXzZ?_]*
Binary      ({Number})?'[bB][01xXzZ?][01xXzZ?_]*

%%
				if(scanTable)
					BEGIN Stable;
				else
					BEGIN Snormal;

<Snormal,Stable>{Space}+     { continue; }
<Snormal,Stable>{Neline}     { yylineno++; continue; }


<Snormal,Stable>"/*"         { skipComments(); continue; }
<Snormal,Stable>"//"[^\n]*\n { yylineno++; continue; }

<Snormal>\"[^"]*\"          { 
				yylval.id = (char *)yytext;
				return _STRING; 
			    }

<Snormal>buf                { return BUF; }
<Snormal>module             { return MODULE; }
<Snormal>endmodule          { return ENDMODULE; }
<Snormal>input              { return INPUT; }
<Snormal>nand               { return NAND; }
<Snormal>or                 { return OR; }
<Snormal>nor                { return NOR; }
<Snormal>not                { return NOT; }
<Snormal>output             { return OUTPUT; }
<Snormal>xnor               { return XNOR; }
<Snormal>xor                { return XOR; }
<Snormal>dff                { return DFF; }
<Snormal>wire               { return WIRE; }
<Snormal>and                { return AND; }

<Snormal>{AlphaU}{AlphaNumU}* { 
                printf(" %s  ", yytext);
		yylval.id = (char *)yytext; // yytext -> need to copy to declared space
		return _NAME; 
                              }
<Snormal>{CtrStr}{AlphaNumC}* {
		yylval.id = (char *)yytext;
		return _NAME;
}
<Snormal>{Number}*\.{Number}+ { return _NUMBER; }
<Snormal>{Number}+\.{Number}* { return _NUMBER; }
<Snormal>{Number}       { yylval.ln=stringToNumber(yytext); return _NUMBER; }
<Snormal>{Decimal}		{ yylval.ln = stringToNumber(yytext); return _BASENUMBER; }
<Snormal>{Octal}		{ yylval.ln = stringToNumber(yytext); return _BASENUMBER; }
<Snormal>{Hexdecimal}		{ yylval.ln = stringToNumber(yytext); return _BASENUMBER; }
<Snormal>{Binary}		{ yylval.ln = stringToNumber(yytext); return _BASENUMBER; }
<Snormal,Stable>.		{return yytext[0];}

%%


int yywrap(){
	return 1;
}

static void skipComments(){
    int done, level = 0;
    char c;

    for (done=0; !done; yyleng = (yyleng > YYLMAX-2) ? YYLMAX-2 : yyleng) {
        yytext[yyleng++] = c;

        c = input() ; 

        if( c == '\n' ) yylineno++ ;

        while( c == '*') {
            yytext[yyleng++] = c;
            c = input();  
            if( c == '\n' ) yylineno++ ;
            if( c == '/') { done = (level-- == 0); }
            if( c == 0) { fprintf(stderr,"line %d: incomplete comment ",yylineno );}
	} 
	if (c == '/') {
	    yytext[yyleng++] = c;
            c = input(); 

            if( c == '\n' ) yylineno++ ;
	    if( c == '*') { level++; } 
            if( c == 0) { fprintf(stderr,"line %d: incomplete comment ",yylineno );}

        } else if (c == 0) {
	    fprintf(stderr,"line %d: incomplete comment ",yylineno );
	}
  }
}

/*WARNING have to fix the nbits*/
/*for now it handle up to 32 bits */
unsigned long convert2bin(buf,nbits) char *buf; unsigned nbits;
{
	unsigned  long	n, w;
	int		 l;

	l = strlen(buf) - 1;
	n = 0;
	w = 1;
	for(;l>=0;l--){
		n = n + (buf[l]=='0'?0:1)*w;
		w = w<<1;
	}
	return n ;
}

unsigned long dec_convert2bin(buf,nbits) char *buf; unsigned nbits;
{
	unsigned	long	n, w;
	int		 l;

	l = strlen(buf) - 1;
	n = 0;
	w = 1;
	for(;l>=0;l--){
		n = n + (buf[l]=='0'?0:1)*w;
		w = w<<1;
	}
	return n ;
}

unsigned long hex_convert2bin(buf,nbits) char *buf; unsigned nbits;
{
	unsigned long	n, w;
	unsigned long	d;
	int		 l;

	l = strlen(buf) - 1;
	n = 0;
	w = 1;
	for(;l>=0;l--){
		switch( buf[l] ){
			case '0': d = 0; break;
			case '1': d = 1; break;
			case '2': d = 2; break;
			case '3': d = 3; break;
			case '4': d = 4; break;
			case '5': d = 5; break;
			case '6': d = 6; break;
			case '7': d = 7; break;
			case '8': d = 8; break;
			case '9': d = 9; break;
			case 'A': 
			case 'a': d = 10; break;
			case 'B':
			case 'b': d = 11; break;
			case 'C':
			case 'c': d = 12; break;
			case 'D':
			case 'd': d = 13; break;
			case 'E':
			case 'e': d = 14; break;
			case 'F':
			case 'f': d = 15; break;
			default: buf[l] = '0' ; d=0;
				printf("Wrong digit '%c' in string %s \n", buf[l], buf);
		}
		d = d << (w-1)*4;
		n = n | d;
		w = w + 1;
	}
	return n ;
}

unsigned long oct_convert2bin(buf,nbits) char *buf; unsigned nbits;
{
	unsigned	long n, w;
	unsigned long	d;
	int		 l;

	l = strlen(buf) - 1;
	n = 0;
	w = 1;
	for(;l>=0;l--){
		switch( buf[l] ){
			case '0': d = 0; break;
			case '1': d = 1; break;
			case '2': d = 2; break;
			case '3': d = 3; break;
			case '4': d = 4; break;
			case '5': d = 5; break;
			case '6': d = 6; break;
			case '7': d = 7; break;
			default: buf[l] = '0' ; d=0;
				printf("Wrong digit '%c'", buf[l]);
		}
		d = d << (w-1)*3;
		n = n | d;
		w = w + 1;
	}
	return n ;
}


/* convert Verilog <DECIMAL_NUMBER> to an int */
unsigned long int  stringToNumber(const char *s){
        static char buf[128];
        const char *cp;
        char *cp1;
	unsigned long int n;
	unsigned long int un;
	int	nbits;

	for(n=0; n < 128; n++ ) buf[n] = 0;
	nbits = -1;
	for(cp=s,cp1=buf;*cp;cp++){
		if(*cp == '_' || *cp == '+' || *cp == '-') 
				continue;
		if(*cp == '\'' ) {
			sscanf(buf,"%d",&nbits);
			cp1=buf; *cp1 = '\0';
			cp++;
			if( *cp == 'b' ){ 
				cp++; 
				for(;*cp;cp++){
					if( *cp == '_' || *cp == '+' || *cp == '-') continue;
					*cp1++ = *cp;
				}
        			*cp1++ = '\0';
				un = convert2bin(buf,nbits);
				return un;
			}
			if( *cp == 'o' ){ 
				cp++; 
				for(;*cp;cp++){
					if( *cp == '_' || *cp == '+' || *cp == '-') continue;
					*cp1++ = *cp;
				}
        			*cp1++ = '\0';
				un = dec_convert2bin(buf,nbits);
				return un;
			}
			if( *cp == 'h' ){ 
				cp++; 
				for(;*cp;cp++){
					if( *cp == '_' || *cp == '+' || *cp == '-') continue;
					*cp1++ = *cp;
				}
        			*cp1++ = '\0';
				un = hex_convert2bin(buf,nbits);
				return un;
			}
			if( *cp == 'd' ){ 
				cp++; 
				for(;*cp;cp++){
					if( *cp == '_' || *cp == '+' || *cp == '-') continue;
					*cp1++ = *cp;
				}
        			*cp1++ = '\0';
				sscanf(buf,"%lu", &n); 
				return n;
			}
		}
		*cp1++ = *cp;
	}

        *cp1 = '\0';
	sscanf(buf,"%lu",&n);
	return n;
}