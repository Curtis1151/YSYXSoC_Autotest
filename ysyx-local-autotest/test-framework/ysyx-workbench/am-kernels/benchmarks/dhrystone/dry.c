/****************** "DHRYSTONE" Benchmark Program ***************************/
#define Version "C, Version 2.2"
/*  File:       dhry_1.c (part 2 of 3)
 *  Author:     Reinhold P. Weicker
 *              Siemens Nixdorf, Paderborn/Germany
 *              weicker@specbench.org
 *  Date:       May 25, 1988
 *  Modified:	Steven Pemberton, CWI, Amsterdam; Steven.Pemberton@cwi.nl
 *  Date:       October, 1993; March 1995
 *              Included both files into one source, that gets compiled
 *              in two passes. Made program auto-compiling, and auto-running,
 *              and generally made it much easier to use.
 *
 *              Original Version (in Ada) published in
 *              "Communications of the ACM" vol. 27., no. 10 (Oct. 1984),
 *              pp. 1013 - 1030, together with the statistics
 *              on which the distribution of statements etc. is based.
 *
 *              In this C version, the following C library functions are used:
 *              - strcpy, strcmp (inside the measurement loop)
 *              - printf, scanf (outside the measurement loop)
 *              In addition, Berkeley UNIX system calls "times ()" or "time ()"
 *              are used for execution time measurement. For measurements
 *              on other systems, these calls have to be changed.
 *
 *  Collection of Results:
 *              Reinhold Weicker (address see above) and
 *
 *              Rick Richardson
 *              PC Research. Inc.
 *              94 Apple Orchard Drive
 *              Tinton Falls, NJ 07724
 *                      Phone:  (201) 389-8963 (9-17 EST)
 *                      Usenet: ...!uunet!pcrat!rick
 *
 *      Please send results to Rick Richardson and/or Reinhold Weicker.
 *      Complete information should be given on hardware and software used.
 *      Hardware information includes: Machine type, CPU, type and size
 *      of caches; for microprocessors: clock frequency, memory speed
 *      (number of wait states).
 *      Software information includes: Compiler (and runtime library)
 *      manufacturer and version, compilation switches, OS version.
 *      The Operating System version may give an indication about the compiler;
 *      Dhrystone itself performs no OS calls in the measurement loop.
 *
 *      The complete output generated by the program should be mailed
 *      such that at least some checks for correctness can be made.
 *
 ***************************************************************************
 *
 * Defines:     The following "Defines" are possible:
 *      -DREG          (default: Not defined)
 *              As an approximation to what an average C programmer
 *              might do, causes the "register" storage class to be applied
 *              - for local variables, if they are used (dynamically)
 *                five or more times
 *              - for parameters if they are used (dynamically)
 *                six or more times
 *              Note that an optimal "register" strategy is
 *              compiler-dependent, and that "register" declarations
 *              do not necessarily lead to faster execution.
 *      -DNOSTRUCTASSIGN        (default: Not defined)
 *              Define if the C compiler does not support
 *              assignment of structures.
 *      -DNOENUMS               (default: Not defined)
 *              Define if the C compiler does not support
 *              enumeration types.
 *      -DTIMES                 (default)
 *      -DTIME
 *              The "times" function of UNIX (returning process times)
 *              or the "time" function (returning wallclock time)
 *              is used for measurement.
 *              For single user machines, "time ()" is adequate. For
 *              multi-user machines where you cannot get single-user
 *              access, use the "times ()" function. If you have
 *              neither, use a stopwatch in the dead of night.
 *              "printf"s are provided marking the points "Start Timer"
 *              and "Stop Timer". DO NOT use the UNIX "time(1)"
 *              command, as this will measure the total time to
 *              run this program, which will (erroneously) include
 *              the time to allocate storage (malloc) and to perform
 *              the initialization.
 *      -DHZ=nnn
 *              In Berkeley UNIX, the function "times" returns process
 *              time in 1/HZ seconds, with HZ = 60 for most systems.
 *              CHECK YOUR SYSTEM DESCRIPTION BEFORE YOU JUST APPLY
 *              A VALUE.
 *
 ***************************************************************************
 *
 *  History:	Version C/2.1 was made for two reasons:
 *
 *	1) There was an obvious need for a common C version of
 *      Dhrystone, since C is at present the most popular system
 *      programming language for the class of processors
 *      (microcomputers, minicomputers) where Dhrystone is used most.
 *      There should be, as far as possible, only one C version of
 *      Dhrystone such that results can be compared without
 *      restrictions. In the past, the C versions distributed
 *      by Rick Richardson (Version 1.1) and by Reinhold Weicker
 *      had small (though not significant) differences.
 *
 *      2) As far as it is possible without changes to the Dhrystone
 *      statistics, optimizing compilers should be prevented from
 *      removing significant statements.
 *
 *      This C version has been developed in cooperation with
 *      Rick Richardson (Tinton Falls, NJ), it incorporates many
 *      ideas from the "Version 1.1" distributed previously by
 *      him over the UNIX network Usenet.
 *      I also thank Chaim Benedelac (National Semiconductor),
 *      David Ditzel (SUN), Earl Killian and John Mashey (MIPS),
 *      Alan Smith and Rafael Saavedra-Barrera (UC at Berkeley)
 *      for their help with comments on earlier versions of the
 *      benchmark.
 *
 *  Changes:    In the initialization part, this version follows mostly
 *      Rick Richardson's version distributed via Usenet, not the
 *      version distributed earlier via floppy disk by Reinhold Weicker.
 *      As a concession to older compilers, names have been made
 *      unique within the first 8 characters.
 *      Inside the measurement loop, this version follows the
 *      version previously distributed by Reinhold Weicker.
 *
 *      At several places in the benchmark, code has been added,
 *      but within the measurement loop only in branches that
 *      are not executed. The intention is that optimizing compilers
 *      should be prevented from moving code out of the measurement
 *      loop, or from removing code altogether. Since the statements
 *      that are executed within the measurement loop have NOT been
 *      changed, the numbers defining the "Dhrystone distribution"
 *      (distribution of statements, operand types and locality)
 *      still hold. Except for sophisticated optimizing compilers,
 *      execution times for this version should be the same as
 *      for previous versions.
 *
 *      Since it has proven difficult to subtract the time for the
 *      measurement loop overhead in a correct way, the loop check
 *      has been made a part of the benchmark. This does have
 *      an impact - though a very minor one - on the distribution
 *      statistics which have been updated for this version.
 *
 *      All changes within the measurement loop are described
 *      and discussed in the companion paper "Rationale for
 *      Dhrystone version 2".
 *
 *      Because of the self-imposed limitation that the order and
 *      distribution of the executed statements should not be
 *      changed, there are still cases where optimizing compilers
 *      may not generate code for some statements. To a certain
 *      degree, this is unavoidable for small synthetic benchmarks.
 *      Users of the benchmark are advised to check code listings
 *      whether code is generated for all statements of Dhrystone.
 *
 *      Version 2.1 is identical to version 2.0 distributed via
 *      the UNIX network Usenet in March 1988 except that it corrects
 *      some minor deficiencies that were found by users of version 2.0.
 *      The only change within the measurement loop is that a
 *      non-executed "else" part was added to the "if" statement in
 *      Func_3, and a non-executed "else" part removed from Proc_3.
 *
 * Version C/2.2, Steven Pemberton, October 1993
 *	Functionally, identical to version 2.2; the changes are in
 *	how you compile and use it:
 *	- Everything is in one file now, but compiled in 2 passes
 *	- Compile (and run) by running the file through the shell: 'sh dhry.c"
 *	- Uses the system definition of HZ if one can be found
 *	- HZ must be defined, otherwise it won't compile (no defaults here)
 *	- The (uninteresting) output is printed to stderr (dhry2 > /dev/null)
 *	- The number of loops is passed as a parameter, rather than read
 *	  (dhry2 500000)
 *	- If the number of loops is insufficient to get a good result,
 *	  it repeats it with loops*10 until it is enough (rather than just
 *	  stopping)
 *	- Output says which sort of clock it is using, and the HZ value
 *	- You can use -DREG instead of the -DREG=register of previous versions
 *	- Some stylistic cleanups.
 *
 ***************************************************************************
 *
 *  Compilation model and measurement (IMPORTANT):
 *
 *  The following "ground rules" apply for measurements:
 *  - Separate compilation
 *  - No procedure merging
 *  - Otherwise, compiler optimizations are allowed but should be indicated
 *  - Default results are those without register declarations
 *  See the companion paper "Rationale for Dhrystone Version 2" for a more
 *  detailed discussion of these ground rules.
 *
 *  For 16-Bit processors (e.g. 80186, 80286), times for all compilation
 *  models ("small", "medium", "large" etc.) should be given if possible,
 *  together with a definition of these models for the compiler system used.
 *
 **************************************************************************
 *
 *  Dhrystone (C version) statistics:
 *
 *  [Comment from the first distribution, updated for version 2.
 *   Note that because of language differences, the numbers are slightly
 *   different from the Ada version.]
 *
 *  The following program contains statements of a high level programming
 *  language (here: C) in a distribution considered representative:
 *
 *    assignments                  52 (51.0 %)
 *    control statements           33 (32.4 %)
 *    procedure, function calls    17 (16.7 %)
 *
 *  103 statements are dynamically executed. The program is balanced with
 *  respect to the three aspects:
 *
 *    - statement type
 *    - operand type
 *    - operand locality
 *         operand global, local, parameter, or constant.
 *
 *  The combination of these three aspects is balanced only approximately.
 *
 *  1. Statement Type:
 *  -----------------             number
 *
 *     V1 = V2                     9
 *       (incl. V1 = F(..)
 *     V = Constant               12
 *     Assignment,                 7
 *       with array element
 *     Assignment,                 6
 *       with record component
 *                                --
 *                                34       34
 *
 *     X = Y +|-|"&&"|"|" Z        5
 *     X = Y +|-|"==" Constant     6
 *     X = X +|- 1                 3
 *     X = Y *|/ Z                 2
 *     X = Expression,             1
 *           two operators
 *     X = Expression,             1
 *           three operators
 *                                --
 *                                18       18
 *
 *     if ....                    14
 *       with "else"      7
 *       without "else"   7
 *           executed        3
 *           not executed    4
 *     for ...                     7  |  counted every time
 *     while ...                   4  |  the loop condition
 *     do ... while                1  |  is evaluated
 *     switch ...                  1
 *     break                       1
 *     declaration with            1
 *       initialization
 *                                --
 *                                34       34
 *
 *     P (...)  procedure call    11
 *       user procedure      10
 *       library procedure    1
 *     X = F (...)
 *             function  call      6
 *       user function        5
 *       library function     1
 *                                --
 *                                17       17
 *                                        ---
 *                                        103
 *
 *    The average number of parameters in procedure or function calls
 *    is 1.82 (not counting the function values aX *
 *
 *  2. Operators
 *  ------------
 *                          number    approximate
 *                                    percentage
 *
 *    Arithmetic             32          50.8
 *
 *       +                     21          33.3
 *       -                      7          11.1
 *       *                      3           4.8
 *       / (int div)            1           1.6
 *
 *    Comparison             27           42.8
 *
 *       ==                     9           14.3
 *       /=                     4            6.3
 *       >                      1            1.6
 *       <                      3            4.8
 *       >=                     1            1.6
 *       <=                     9           14.3
 *
 *    Logic                   4            6.3
 *
 *       && (AND-THEN)          1            1.6
 *       |  (OR)                1            1.6
 *       !  (NOT)               2            3.2
 *
 *                           --          -----
 *                           63          100.1
 *
 *
 *  3. Operand Type (counted once per operand reference):
 *  ---------------
 *                          number    approximate
 *                                    percentage
 *
 *     Integer               175        72.3 %
 *     Character              45        18.6 %
 *     Pointer                12         5.0 %
 *     String30                6         2.5 %
 *     Array                   2         0.8 %
 *     Record                  2         0.8 %
 *                           ---       -------
 *                           242       100.0 %
 *
 *  When there is an access path leading to the final operand (e.g. a record
 *  component), only the final data type on the access path is counted.
 *
 *
 *  4. Operand Locality:
 *  -------------------
 *                                number    approximate
 *                                          percentage
 *
 *     local variable              114        47.1 %
 *     global variable              22         9.1 %
 *     parameter                    45        18.6 %
 *        value                        23         9.5 %
 *        reference                    22         9.1 %
 *     function result               6         2.5 %
 *     constant                     55        22.7 %
 *                                 ---       -------
 *                                 242       100.0 %
 *
 *  The program does not compute anything meaningful, but it is syntactically
 *  and semantically correct. All variables have a value assigned to them
 *  before they are used as a source operand.
 *
 *  There has been no explicit effort to account for the effects of a
 *  cache, or to balance the use of long or short displacements for code or
 *  data.
 *
 ***************************************************************************
 */

/* Compiler and system dependent definitions: */

/* variables for time measurement: */

#include <am.h>
#include <klib.h>
#include <klib-macros.h>

static uint32_t uptime_ms() { return io_read(AM_TIMER_UPTIME).us / 1000; }
#define Start_Timer() Begin_Time = uptime_ms()
#define Stop_Timer()  End_Time   = uptime_ms()

#define NUMBER_OF_RUNS		50 /* Default number of runs */
#define PASS2

#ifdef  NOSTRUCTASSIGN
#define structassign(d, s)      memcpy(&(d), &(s), sizeof(d))
#else
#define structassign(d, s)      d = s
#endif

#ifdef  NOENUM
#define Ident_1 0
#define Ident_2 1
#define Ident_3 2
#define Ident_4 3
#define Ident_5 4
  typedef int   Enumeration;
#else
  typedef       enum    {Ident_1, Ident_2, Ident_3, Ident_4, Ident_5}
                Enumeration;
#endif
        /* for boolean and enumeration types in Ada, Pascal */

/* General definitions: */


#define Null 0
                /* Value of a Null pointer */

typedef int     One_Thirty;
typedef int     One_Fifty;
typedef char    Capital_Letter;
typedef int     Boolean;
typedef char    Str_30 [31];
typedef int     Arr_1_Dim [50];
typedef int     Arr_2_Dim [50] [50];

typedef struct record
    {
    struct record *Ptr_Comp;
    Enumeration    Discr;
    union {
          struct {
                  Enumeration Enum_Comp;
                  int         Int_Comp;
                  char        Str_Comp [31];
                  } var_1;
          struct {
                  Enumeration E_Comp_2;
                  char        Str_2_Comp [31];
                  } var_2;
          struct {
                  char        Ch_1_Comp;
                  char        Ch_2_Comp;
                  } var_3;
          } variant;
      } Rec_Type, *Rec_Pointer;

/* Global Variables: */

Rec_Pointer     Ptr_Glob,
                Next_Ptr_Glob;
int             Int_Glob;
Boolean         Bool_Glob;
char            Ch_1_Glob,
                Ch_2_Glob;
int             Arr_1_Glob [50];
int             Arr_2_Glob [50] [50];

Enumeration     Func_1 ();
  /* forward declaration necessary since Enumeration may not simply be int */

#ifndef REG
        Boolean Reg = false;
#define REG
        /* REG becomes defined as empty */
        /* i.e. no register variables   */
#else
        Boolean Reg = true;
#undef REG
#define REG register
#endif

Boolean		Done;

long            Begin_Time,
                End_Time,
                User_Time;
float           Microseconds,
                Dhrystones_Per_Second;

/* end of variables for time measurement */

static char memory[1024];
static char *free_mem = &memory[0];

static char* myalloc(size_t size) {
  while ((unsigned long)free_mem % 4 != 0) free_mem ++;
  char *ret = free_mem;
  free_mem += size;
  return ret;
}

void Proc_6 (Enumeration, Enumeration*);
void Proc_3 (Rec_Pointer*);
void Proc_7 (One_Fifty a, One_Fifty b, One_Fifty* c);
Boolean Func_2 (Str_30, Str_30);
void Proc_8(Arr_1_Dim, Arr_2_Dim, int, int);
Boolean Func_3 (Enumeration);

void Proc_1 (Ptr_Val_Par)
/******************/

REG Rec_Pointer Ptr_Val_Par;
    /* executed once */
{
  REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;
                                        /* == Ptr_Glob_Next */
  /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
  /* corresponds to "rename" in Ada, "with" in Pascal           */

  structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);
  Ptr_Val_Par->variant.var_1.Int_Comp = 5;
  Next_Record->variant.var_1.Int_Comp
        = Ptr_Val_Par->variant.var_1.Int_Comp;
  Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
  Proc_3 (&Next_Record->Ptr_Comp);
    /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp
                        == Ptr_Glob->Ptr_Comp */
  if (Next_Record->Discr == Ident_1)
    /* then, executed */
  {
    Next_Record->variant.var_1.Int_Comp = 6;
    Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp,
           &Next_Record->variant.var_1.Enum_Comp);
    Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
    Proc_7 (Next_Record->variant.var_1.Int_Comp, 10,
           &Next_Record->variant.var_1.Int_Comp);
  }
  else /* not executed */
    structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
} /* Proc_1 */


void Proc_2 (Int_Par_Ref)
/******************/
    /* executed once */
    /* *Int_Par_Ref == 1, becomes 4 */

One_Fifty   *Int_Par_Ref;
{
  One_Fifty  Int_Loc;
  Enumeration   Enum_Loc;

  Int_Loc = *Int_Par_Ref + 10;
  do /* executed once */
    if (Ch_1_Glob == 'A')
      /* then, executed */
    {
      Int_Loc -= 1;
      *Int_Par_Ref = Int_Loc - Int_Glob;
      Enum_Loc = Ident_1;
    } /* if */
  while (Enum_Loc != Ident_1); /* true */
} /* Proc_2 */


void Proc_3 (Ptr_Ref_Par)
/******************/
    /* executed once */
    /* Ptr_Ref_Par becomes Ptr_Glob */

Rec_Pointer *Ptr_Ref_Par;

{
  if (Ptr_Glob != Null)
    /* then, executed */
    *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
  Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
} /* Proc_3 */


void Proc_4 () /* without parameters */
/*******/
    /* executed once */
{
  Boolean Bool_Loc;

  Bool_Loc = Ch_1_Glob == 'A';
  Bool_Glob = Bool_Loc | Bool_Glob;
  Ch_2_Glob = 'B';
} /* Proc_4 */


void Proc_5 () /* without parameters */
/*******/
    /* executed once */
{
  Ch_1_Glob = 'A';
  Bool_Glob = false;
} /* Proc_5 */


        /* Procedure for the assignment of structures,          */
        /* if the C compiler doesn't support this feature       */
#ifdef  NOSTRUCTASSIGN
memcpy (d, s, l)
register char   *d;
register char   *s;
register int    l;
{
        while (l--) *d++ = *s++;
}
#endif


#ifndef REG
#define REG
        /* REG becomes defined as empty */
        /* i.e. no register variables   */
#else
#undef REG
#define REG register
#endif

extern  int     Int_Glob;
extern  char    Ch_1_Glob;


void Proc_6 (Enum_Val_Par, Enum_Ref_Par)
/*********************************/
    /* executed once */
    /* Enum_Val_Par == Ident_3, Enum_Ref_Par becomes Ident_2 */

Enumeration  Enum_Val_Par;
Enumeration *Enum_Ref_Par;
{
  *Enum_Ref_Par = Enum_Val_Par;
  if (! Func_3 (Enum_Val_Par))
    /* then, not executed */
    *Enum_Ref_Par = Ident_4;
  switch (Enum_Val_Par)
  {
    case Ident_1:
      *Enum_Ref_Par = Ident_1;
      break;
    case Ident_2:
      if (Int_Glob > 100)
        /* then */
      *Enum_Ref_Par = Ident_1;
      else *Enum_Ref_Par = Ident_4;
      break;
    case Ident_3: /* executed */
      *Enum_Ref_Par = Ident_2;
      break;
    case Ident_4: break;
    case Ident_5:
      *Enum_Ref_Par = Ident_3;
      break;
  } /* switch */
} /* Proc_6 */


void Proc_7 (One_Fifty Int_1_Par_Val, One_Fifty Int_2_Par_Val, One_Fifty *Int_Par_Ref)
{
  One_Fifty Int_Loc;

  Int_Loc = Int_1_Par_Val + 2;
  *Int_Par_Ref = Int_2_Par_Val + Int_Loc;
} /* Proc_7 */


void Proc_8 (Arr_1_Par_Ref, Arr_2_Par_Ref, Int_1_Par_Val, Int_2_Par_Val)
/*********************************************************************/
    /* executed once      */
    /* Int_Par_Val_1 == 3 */
    /* Int_Par_Val_2 == 7 */
Arr_1_Dim       Arr_1_Par_Ref;
Arr_2_Dim       Arr_2_Par_Ref;
int             Int_1_Par_Val;
int             Int_2_Par_Val;
{
  REG One_Fifty Int_Index;
  REG One_Fifty Int_Loc;

  Int_Loc = Int_1_Par_Val + 5;
  Arr_1_Par_Ref [Int_Loc] = Int_2_Par_Val;
  Arr_1_Par_Ref [Int_Loc+1] = Arr_1_Par_Ref [Int_Loc];
  Arr_1_Par_Ref [Int_Loc+30] = Int_Loc;
  for (Int_Index = Int_Loc; Int_Index <= Int_Loc+1; ++Int_Index)
    Arr_2_Par_Ref [Int_Loc] [Int_Index] = Int_Loc;
  Arr_2_Par_Ref [Int_Loc] [Int_Loc-1] += 1;
  Arr_2_Par_Ref [Int_Loc+20] [Int_Loc] = Arr_1_Par_Ref [Int_Loc];
  Int_Glob = 5;
} /* Proc_8 */


Enumeration Func_1 (Ch_1_Par_Val, Ch_2_Par_Val)
/*************************************************/
    /* executed three times                                         */
    /* first call:      Ch_1_Par_Val == 'H', Ch_2_Par_Val == 'R'    */
    /* second call:     Ch_1_Par_Val == 'A', Ch_2_Par_Val == 'C'    */
    /* third call:      Ch_1_Par_Val == 'B', Ch_2_Par_Val == 'C'    */

Capital_Letter   Ch_1_Par_Val;
Capital_Letter   Ch_2_Par_Val;
{
  Capital_Letter        Ch_1_Loc;
  Capital_Letter        Ch_2_Loc;

  Ch_1_Loc = Ch_1_Par_Val;
  Ch_2_Loc = Ch_1_Loc;
  if (Ch_2_Loc != Ch_2_Par_Val)
    /* then, executed */
    return (Ident_1);
  else  /* not executed */
  {
    Ch_1_Glob = Ch_1_Loc;
    return (Ident_2);
   }
} /* Func_1 */


Boolean Func_2 (Str_1_Par_Ref, Str_2_Par_Ref)
/*************************************************/
    /* executed once */
    /* Str_1_Par_Ref == "DHRYSTONE PROGRAM, 1'ST STRING" */
    /* Str_2_Par_Ref == "DHRYSTONE PROGRAM, 2'ND STRING" */

Str_30  Str_1_Par_Ref;
Str_30  Str_2_Par_Ref;
{
  REG One_Thirty        Int_Loc;
      Capital_Letter    Ch_Loc;

  Int_Loc = 2;
  while (Int_Loc <= 2) /* loop body executed once */
    if (Func_1 (Str_1_Par_Ref[Int_Loc],
                Str_2_Par_Ref[Int_Loc+1]) == Ident_1)
      /* then, executed */
    {
      Ch_Loc = 'A';
      Int_Loc += 1;
    } /* if, while */
  if (Ch_Loc >= 'W' && Ch_Loc < 'Z')
    /* then, not executed */
    Int_Loc = 7;
  if (Ch_Loc == 'R') {
    /* then, not executed */
    return (true);
  }
  else /* executed */
  {
    if (strcmp (Str_1_Par_Ref, Str_2_Par_Ref) > 0)
    {
      Int_Loc += 7;
      Int_Glob = Int_Loc;
      return (true);
    }
    else /* executed */
      return (false);
  } /* if Ch_Loc */
} /* Func_2 */


Boolean Func_3 (Enum_Par_Val)
/***************************/
    /* executed once        */
    /* Enum_Par_Val == Ident_3 */
Enumeration Enum_Par_Val;
{
  Enumeration Enum_Loc;

  Enum_Loc = Enum_Par_Val;
  if (Enum_Loc == Ident_3)
    /* then, executed */
    return (true);
  else /* not executed */
    return (false);
} /* Func_3 */


Boolean pass = true;
Boolean check(int cond) {
  if (!cond) pass = false;
  return cond;
}
int main ()
/*****/

  /* main program, corresponds to procedures        */
  /* Main and Proc_0 in the Ada version             */
{
        One_Fifty       Int_1_Loc;
  REG   One_Fifty       Int_2_Loc;
        One_Fifty       Int_3_Loc;
  REG   char            Ch_Index;
        Enumeration     Enum_Loc;
        Str_30          Str_1_Loc;
        Str_30          Str_2_Loc;
  REG   int             Run_Index;
  REG   int             Number_Of_Runs;

  ioe_init();

 Number_Of_Runs = NUMBER_OF_RUNS;

  /* Initializations */

  Next_Ptr_Glob = (Rec_Pointer) myalloc (sizeof (Rec_Type));
  Ptr_Glob = (Rec_Pointer) myalloc (sizeof (Rec_Type));

  Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
  Ptr_Glob->Discr                       = Ident_1;
  Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
  Ptr_Glob->variant.var_1.Int_Comp      = 40;
  strcpy (Ptr_Glob->variant.var_1.Str_Comp,
          "DHRYSTONE PROGRAM, SOME STRING");
  strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

  Arr_2_Glob [8][7] = 10;
        /* Was missing in published program. Without this statement,    */
        /* Arr_2_Glob [8][7] would have an undefined value.             */
        /* Warning: With 16-Bit processors and Number_Of_Runs > 32000,  */
        /* overflow may occur for this array element.                   */

  printf ("Dhrystone Benchmark, Version %s\n", Version);

  Done = false;
  while (!Done) {

    printf ("Trying %d runs through Dhrystone.\n", Number_Of_Runs);

    /***************/
    /* Start timer */
    /***************/

    Start_Timer();

    for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
    {

      Proc_5();
      Proc_4();
	/* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
      Int_1_Loc = 2;
      Int_2_Loc = 3;
      strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
      Enum_Loc = Ident_2;
      Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
	/* Bool_Glob == 1 */
      while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */
      {
	Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
	  /* Int_3_Loc == 7 */
	Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
	  /* Int_3_Loc == 7 */
	Int_1_Loc += 1;
      } /* while */
	/* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
      Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
	/* Int_Glob == 5 */
      Proc_1 (Ptr_Glob);
      for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
			       /* loop body executed twice */
      {
	if (Enum_Loc == Func_1 (Ch_Index, 'C'))
	    /* then, not executed */
	  {
	  Proc_6 (Ident_1, &Enum_Loc);
	  strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
	  Int_2_Loc = Run_Index;
	  Int_Glob = Run_Index;
	  }
      }
	/* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
      Int_2_Loc = Int_2_Loc * Int_1_Loc;
      Int_1_Loc = Int_2_Loc / Int_3_Loc;
      Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
	/* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
      Proc_2 (&Int_1_Loc);
	/* Int_1_Loc == 5 */

    } /* loop "for Run_Index" */

    /**************/
    /* Stop timer */
    /**************/

    Stop_Timer();

    User_Time = End_Time - Begin_Time;

    Done = true;
  }

  if (!check(Int_Glob == 5)) {
    printf("Int_Glob:            %d\n", Int_Glob);
    printf("        should be:   %d\n", 5);
  }
  if (!check(Bool_Glob == 1)) {
    printf("Bool_Glob:           %d\n", Bool_Glob);
    printf("        should be:   %d\n", 1);
  }
  if (!check(Ch_1_Glob == 'A')) {
    printf("Ch_1_Glob:           %c\n", Ch_1_Glob);
    printf("        should be:   %c\n", 'A');
  }
  if (!check(Ch_2_Glob == 'B')) {
    printf("Ch_2_Glob:           %c\n", Ch_2_Glob);
    printf("        should be:   %c\n", 'B');
  }
  if (!check(Arr_1_Glob[8] == 7)) {
    printf("Arr_1_Glob[8]:       %d\n", Arr_1_Glob[8]);
    printf("        should be:   %d\n", 7);
  }
  if (!check(Arr_2_Glob[8][7] == Number_Of_Runs + 10)) {
    printf("Arr_2_Glob[8][7]:    %d\n", Arr_2_Glob[8][7]);
    printf("        should be:   Number_Of_Runs + 10\n");
  }

  if (!check((int)Ptr_Glob->Discr == 0)) {
    printf("Ptr_Glob->Discr:             %d\n", Ptr_Glob->Discr);
    printf("        should be:   %d\n", 0);
  }
  if (!check(Ptr_Glob->variant.var_1.Enum_Comp == 2)) {
    printf("Ptr_Glob->Enum_Comp:         %d\n", Ptr_Glob->variant.var_1.Enum_Comp);
    printf("        should be:   %d\n", 2);
  }
  if (!check(Ptr_Glob->variant.var_1.Int_Comp == 17)) {
    printf("Ptr_Glob->Int_Comp:          %d\n", Ptr_Glob->variant.var_1.Int_Comp);
    printf("        should be:   %d\n", 17);
  }
  if (!check(strcmp(Ptr_Glob->variant.var_1.Str_Comp, "DHRYSTONE PROGRAM, SOME STRING") == 0)) {
    printf("Ptr_Glob->Str_Comp:          %s\n", Ptr_Glob->variant.var_1.Str_Comp);
    printf("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  }

  if (!check((int)Next_Ptr_Glob->Discr == 0)) {
    printf("Next_Ptr_Glob->Discr:             %d\n", Next_Ptr_Glob->Discr);
    printf("        should be:   %d\n", 0);
  }
  if (!check(Next_Ptr_Glob->variant.var_1.Enum_Comp == 1)) {
    printf("Next_Ptr_Glob->Enum_Comp:         %d\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
    printf("        should be:   %d\n", 1);
  }
  if (!check(Next_Ptr_Glob->variant.var_1.Int_Comp == 18)) {
    printf("Next_Ptr_Glob->Int_Comp:          %d\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
    printf("        should be:   %d\n", 18);
  }
  if (!check(strcmp(Next_Ptr_Glob->variant.var_1.Str_Comp, "DHRYSTONE PROGRAM, SOME STRING") == 0)) {
    printf("Next_Ptr_Glob->Str_Comp:          %s\n", Next_Ptr_Glob->variant.var_1.Str_Comp);
    printf("        should be:   DHRYSTONE PROGRAM, SOME STRING\n");
  }

  if (!check(Int_1_Loc == 5)) {
    printf("Int_1_Loc:           %d\n", Int_1_Loc);
    printf("        should be:   %d\n", 5);
  }
  if (!check(Int_2_Loc == 13)) {
    printf("Int_2_Loc:           %d\n", Int_2_Loc);
    printf("        should be:   %d\n", 13);
  }
  if (!check(Int_3_Loc == 7)) {
    printf("Int_3_Loc:           %d\n", Int_3_Loc);
    printf("        should be:   %d\n", 7);
  }
  if (!check(Enum_Loc == 1)) {
    printf("Enum_Loc:            %d\n", Enum_Loc);
    printf("        should be:   %d\n", 1);
  }

  if (!check(strcmp(Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING") == 0)) {
    printf("Str_1_Loc:           %s\n", Str_1_Loc);
    printf("        should be:   DHRYSTONE PROGRAM, 1'ST STRING\n");
  }
  if (!check(strcmp(Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING") == 0)) {
    printf("Str_2_Loc:           %s\n", Str_2_Loc);
    printf("        should be:   DHRYSTONE PROGRAM, 2'ND STRING\n");
  }

  printf ("Finished in %d ms\n", (int)User_Time);
  printf("==================================================\n");
  printf("Dhrystone %s         %d Marks\n", pass ? "PASS" : "FAIL",
      880900 / (int)User_Time * NUMBER_OF_RUNS/ 500000);
  printf("                   vs. 100000 Marks (i7-7700K @ 4.20GHz)\n");

  return (pass ? 0 : 1);
}


