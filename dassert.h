/* For simple unit testing
 * Compatible with make output for better error handling in IDE/test editors (like VIM quickfix)
 * If use DWARN, exit with WARNCOUNT from main()
 
..
   #include <dassert.h>
   
   static size_t WARNCOUNT = 0;
   
   int main()
   {
       ..
       DWARN(1 == 2, "Alwais fail");
       ..
       return WARNCOUNT;
   }
 */

#define DASSERT(x, msg) if (!(x)) { fprintf(stderr, "%s:%d: (%s) %s : %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, #x, msg); abort(); }
#define DWARN(x, msg) if (!(x)) { fprintf(stderr, "%s:%d: (%s) %s : %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, #x, msg); WARNCOUNT++; }
#define DMSG(msg) fprintf(stderr, "%s:%d: (%s) : %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, msg); }
