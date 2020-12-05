/* Name, hw2.c, CS 24000, Fall 2018
 * Last updated September 6, 2018
 */

/* Add any includes here */

#include "hw2.h"

#include <stdio.h>
#include <string.h>

/* Define any global variables here */

char g_product_name[MAX_RECORDS][MAX_PRODUCT_LEN] = {'\0'};
int g_product_count = 0;
float g_unit_price[MAX_RECORDS] = {'\0'};
int g_availability[MAX_RECORDS][STORES] = {'\0'};

/* This function opens the file specified (argument 1) and reads in the records
 * filling in g_product_name[], g_unit_price[], and g_availabbility[]. Returns
 * the number of records read. Also sets g_product_count to be the number of
 * products read in from the file.
 */

int input_tables(char * filename) {

  #define CHECK_NUM_READ (12)

  FILE * f_read = NULL;
  f_read = fopen(filename, "r");
  if (f_read == NULL) {
    return FILE_READ_ERROR;
  }
  int num_read_in = 0;
  g_product_count = 0;

  /* This while loop reads in the lines of data given by the file (argument 1).
   * It sets the variable 'num_read_in' equal to the number of data points read
   * per line. It stops at each comma, and the %d and %f each correspond to a
   * different data point being collected. Each data point is stored to a
   * seperate position in one of a few arrays. The first piece of data is
   * stored as the name, the second as the price, and the rest as the
   * availability of each item at stores 0-9 (where the second argument of the
   * g_availability array is the store number.
   */

  while ((num_read_in = fscanf(f_read,
          "%[^,], $%f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n",
          g_product_name[g_product_count],
          &g_unit_price[g_product_count],
          &g_availability[g_product_count][0],
          &g_availability[g_product_count][1],
          &g_availability[g_product_count][2],
          &g_availability[g_product_count][3],
          &g_availability[g_product_count][4],
          &g_availability[g_product_count][5],
          &g_availability[g_product_count][6],
          &g_availability[g_product_count][7],
          &g_availability[g_product_count][8],
          &g_availability[g_product_count][9]))) {
    if(num_read_in != EOF) {
      g_product_count++;
    }

    /* If the num_read_in variable hits the end of the file, the read pointer
     * is closed and set to null, then broken from the while loop.
     */

    if (num_read_in == EOF) {
      fclose(f_read);
      f_read = NULL;
      break;
    }

    /* If the num_read_in variable does not equal CHECK_NUM_READ (12), that
     * means that there was an incomplete line in the records. RECORD_ERROR
     * is returned.
     */

    if(num_read_in != CHECK_NUM_READ) {
      fclose(f_read);
      f_read = NULL;
      return RECORD_ERROR;
    }
  }
  return g_product_count;
} /* input_tables() */

/* Takes the product name (argument 1) and returns its unit price. Checks the
 * product name and returns NO_SUCH_PRODUCT if the entry doesn't exist.
 */

float unit_price(char * product_name) {
  int strcmp_result;

  /* Advances through the g_product_name array to find which product, if any,
   * has the same name as product_name (argument 1). If it does, strcmp will
   * return 0, so if strcmp_result is 0, return the index i in the g_unit_price
   * array (since corresponding indices link a product's information to it).
   */

  for (int i = 0; i < g_product_count; i++) {
    strcmp_result = strcmp(product_name, g_product_name[i]);
    if (strcmp_result == 0) {
      return g_unit_price[i];
    }
  }
  return NO_SUCH_PRODUCT;
} /* unit_price() */

/* Takes the store number as the first argument and returns its net worth.
 * Net worth is determined by summing the result of multiplying each
 * product's availability with its unit price.
 */

float net_worth(int store_number) {
  if ((store_number < 0) || (store_number > 9)) {
    return OUT_OF_BOUNDS;
  }
  float sum_products = 0;

  /* For the corresponding store number (argument 1), the g_availability
   * and g_unit_price arrays are explored, and for each value in the
   * availability array, the corresponding value in the unit price
   * array is multiplied, giving the value of that certain product.
   * That's added to a value that counts the total sum, and each
   * product in the inventory is added.
   */

  for (int i = 0; i < g_product_count; i++) {
    sum_products += g_availability[i][store_number] * g_unit_price[i];
  }
  return sum_products;
} /* net_worth() */

/* Takes the store number (argument 1) and returns the maximum net product
 * value, which is determined by summing the reesult of multiplying
 * each availability by the unit price.
 */

float max_net_product(int store_number) {
  if ((store_number < 0) || (store_number > 9)) {
    return OUT_OF_BOUNDS;
  }
  float sum_product_value = 0;
  float max_product_value = 0;

  /* In a certain store (argument 1), each product's value is calculated
   * and if it is bigger than the current max product value, it becomes
   * the new max_product_value. The value is determined by incrementing
   * through the availability and unit price arrays, where each value is
   * multiplied by the value in the other array with the corresponding
   * index.
   */

  for (int i = 0; i < g_product_count; i++) {
    sum_product_value = g_availability[i][store_number] * g_unit_price[i];
    if (sum_product_value > max_product_value) {
      max_product_value = sum_product_value;
    }
  }
  return max_product_value;
} /* max_net_product() */

/* Takes the product name (argument 1) and returns the store number
 * corresponding to the store that has the largest availability for that
 * particular product. NO_SUCH_PRODUCT should be returned if the product
 * doesn't exist.
 */

int max_prod_availability(char * product_name) {
  int max_availability = 0;
  int availability = 0;
  int strcmp_result = 0;
  int max_availability_store = 0;

  /* Advances through the g_product_name array to find which product, if any,
   * has the same name as product_name (argument 1). If it does, strcmp will
   * return 0, so if strcmp_result is 0, then use that index i to increment
   * through the availability array. If g_availability[i][j] is bigger than
   * the current max_availability, then the new max_availability becomes
   * the current availability of g_availability[i][j]. The index j is saved
   * to max_availability_store, and whichever store has the most, their store
   * number will be returned as max_availability_store.
   */

  for (int i = 0; i < g_product_count; i++) {
    strcmp_result = strcmp(product_name, g_product_name[i]);
    if(strcmp_result == 0) {
      for (int j = 0; j <= 9; j++) {
        availability = g_availability[i][j];
        if (availability > max_availability) {
          max_availability = availability;
          max_availability_store = j;
        }
      }
      return max_availability_store;
    }
  }
  return NO_SUCH_PRODUCT;
} /* max_prod_availability() */

/* Takes the store number (argument 1) and returns the index corresponding
 * to the product name that has the most availability in the given store.
 */

int max_store_availability(int store_number) {
  if ((store_number < 0) || (store_number > 9)) {
    return OUT_OF_BOUNDS;
  }

  int max_availability = 0;
  int product_index = 0;

  /* Increments through a g_availability array, where the second index
   * is the store number (argument 1). If the availability of the current
   * item is larger than the max_availability so far, the max_availability
   * is replaced by the availability of the current item, and the index i
   * is stored in product_index. product_index is then returned for the
   * item with the largest availability.
   */

  for (int i = 0; i < g_product_count; i++) {
    if (g_availability[i][store_number] > max_availability) {
      max_availability = g_availability[i][store_number];
      product_index = i;
    }
  }
  return product_index;
} /* max_store_availability() */

/* Accepts a character string (argument 1) that represents a filename to
 * open for writing and two integers representing the start and end stores
 * to be output. It will return OK if no errors occur and output to the
 * specified file the product names, unit prices, and product availabilites
 * between the start and end store.
 */

int output_tables(char * filename, int start, int end) {
  if (((start < 0) || (start > 9)) ||
      ((end < 0) || (end > 9))) {
    return OUT_OF_BOUNDS;
  }

  FILE * f_write = NULL;
  f_write = fopen(filename, "w");
  if (f_write == NULL) {
    return FILE_WRITE_ERROR;
  }
  fclose(f_write);

  FILE * f_append = NULL;
  f_append = fopen(filename, "a");
  if (f_append == NULL) {
    return FILE_WRITE_ERROR;
  }

  /* The first loop appends the product name and price to each line,
   * accessed by the g_product_name and g_unit_price arrays, respectively.
   * The nested second loop adds on the availabilities of each store, based
   * on the start (argument 2) and end (argument 3) variables. Then, after
   * all availabilites are added, a newline is created.
   */

  for (int i = 0; i < g_product_count; i++) {
    fprintf(f_append, "%s,$%.2f", g_product_name[i], g_unit_price[i]);
    for (int j = start; j <= end; j++) {
      fprintf(f_append, ",%d", g_availability[i][j]);
    }
    fprintf(f_append, "\n");
  }
  fclose(f_append);
  f_append = NULL;
  return OK;
} /* output_tables() */
