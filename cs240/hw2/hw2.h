#ifndef HW2_H
#define HW2_H

/* Constant Definitions */

#define MAX_PRODUCT_LEN    (40)  /* max product name length                */
#define MAX_RECORDS     (10000)  /* max no of records that can be read     */
#define STORES             (10)  /* number of stores to process            */

/* Error Codes */

#define OK                 (0)   /* No errors, everything as should be     */
#define RECORD_ERROR      (-3)   /* unexpected fields/chars in record      */
#define FILE_READ_ERROR   (-4)   /* can't open file for read               */
#define FILE_WRITE_ERROR  (-5)   /* can't open file for write              */
#define OUT_OF_BOUNDS     (-6)   /* the entry not within expected range    */
#define NO_SUCH_PRODUCT   (-7)   /* the product does not exist in tables   */

#define access(x,y) (printf("\n\n\n\n\nDo not use access().\n"), abort(), 1)

#include <stdlib.h>

/* Function Prototypes */

int input_tables(char *);
float unit_price(char *);
float net_worth(int );
float max_net_product(int );
int max_prod_availability(char *);
int max_store_availability(int );
int output_tables(char *, int , int );

extern int g_product_count;
extern char g_product_name[MAX_RECORDS][MAX_PRODUCT_LEN];
extern float g_unit_price[MAX_RECORDS];
extern int g_availability[MAX_RECORDS][STORES];

#if (_UNISTD_H == 1)
#error "Take out the #include <unistd.h>.  You don't need it."
#endif

#endif // HW2_H
