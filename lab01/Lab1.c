/*                   CS 360 LAB Assignment #1

                     DUE: Week of 9-7-2015

OBJECTIVES: Stack Usage and YOUR myprintf() FUNCTION

An operating system supports only how to print char. All other printings are
based on printing char. In Linux, putchar(char c); prints a char.

2. typedef unsigned int u32;

   The following printu(u32 x) function prints an unsigned int (u32)x:
*/
#define BASE 10

char *table = "0123456789ABCDEF";

int rpu(u32 x)
{
  char c;
  if (x){
     c = table[x % BASE];
     rpu(x / BASE);
     putchar(c);
  }
} 

int printu(u32 x)
{
  if (x==0)
     putchar('0');
  else
     rpu(x);
  putchar(' ');
}

/*
3. WRITE YOUR OWN functions:
   int printd(int x): print an integer (which may be negative!!)
   int printo(u32 x): print x in OCTal as 0.....
   int printx(u32 x): print x in HEX.  as 0x....

4. WRITE a myprintf(char *fmt, ...) for formatted printing, e.g.

      myprintf("this is a string\n");   // print the string
      myprintf("this %c is a %d test %x\n", 'a', 1234, 100); 
   
  Your myprintf() should print items by %c, %s, %u, %d, %o, %x, 
  just like printf().*/

/********************** HELP INFO ***********************************/


int myprintf(char *fmt, ...) // SOME C compiler requires the 3 DOTs
{
int i = 0;
int items = 0;
if(fmt[0] == "\""){ // checks to make sure first character in string is a quotation mark
 i = 1;
}
else{
 return 0; // returns 0 if first symbol is not quote
}

while(fmt[i] != "\"" && fmt[i] != NULL){
 if(fmt[i] == "\%"){
  // cases for %c, %s, %u, %d, %o, %x
  // use variable "items" to identify which item to output when "%" occurs
  i++;
  if(fmt[i] == c){
//   putchar(
//###########################################THIS IS WHERE YOU NEED TO OUTPUT CHAR VARIABLE ############# #############################################################
  }
 }
 putchar(fmt[i]);
 i++;
}
/*   Assume the call is myprintf(fmt, a,b,c,d);
   Upon entry, the stack contains:

                            |-> "....%c ..%s ..%d .. %x ....\n"       
                            |
   HIGH                     |                                     LOW 
   ---------------------- --|------------------------------------------
       |  d  | c | b | a | fmt |retPC| ebp | locals
   -------------------------------------|------------------------------
                                        | 
                                     CPU.ebp
  
  
     1. Let char *cp point at the format string

     2. Let int *ip  point at the first item to be printed on stack, e.g. a


  *************** ALGORITHM ****************
   Use cp to scan the format string:
       spit out any char that's NOT %
       for each \n, spit out an extra \r

   Upon seeing a %: get next char, which must be one of 'c','s','u','d', 'o','x'
   Then call

        putchar(*ip) for 'c';
        prints(*ip)  for 's';
        printu(*ip)  for 'u';
        printd(*ip)  for 'd';
        printo(*ip)  for 'o';
        printx(*ip)  for 'x';

   Advance ip to point to the next item on stack.
*/

//5. Given: s.s file:



