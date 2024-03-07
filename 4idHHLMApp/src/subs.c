#include <ctype.h>
#include <dbAccess.h>
#include <dbDefs.h>
#include <dbFldTypes.h>
#include <epicsExport.h>
#include <math.h>
#include <registryFunction.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <subRecord.h>

#define MAX_ROWS 1000
#define MAX_COLS 10
#define MAX_ROW_LENGTH 250

static double concave_table[MAX_ROWS][MAX_COLS];
static double convex_table[MAX_ROWS][MAX_COLS];

typedef struct ArrayShape {
    size_t rows;
    size_t columns;
} ArrayShape;

// removes whitespace from a string in place
void remove_whitespace(char *str) {
    int i, j;
    for (i = 0, j = 0; str[i] != '\0'; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}

// splits a string by commas, converts the values to doubles
// and store them in the given array
int split_csv_string(const char *instring, double *array) {
    const char *delimiter = ",";
    char *token;
    char *tmp_string = strdup(instring);
    remove_whitespace(tmp_string);
    token = strtok(tmp_string, delimiter);

    int col = 0;
    double val;
    while (token != NULL) {
        val = atof(token);
        array[col] = val;
        col += 1;
        token = strtok(NULL, delimiter);
    }

    free(tmp_string);

    return col;
}

// Reads the csv file line by line, splitting
// each line by comma, converting to double,
// and storing in the table array
ArrayShape load_table(const char *filename, double table[MAX_ROWS][MAX_COLS]) {

    ArrayShape shape = {.rows = 0, .columns = 0};
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        perror("Error opening file\n");
        return shape;
    }

    char buff[MAX_ROW_LENGTH];
    int row = 0;
    int col = 0;
    while (fgets(buff, sizeof(buff), fp) != NULL) {
        col = split_csv_string(buff, table[row]);
        row += 1;
    }

    if (!feof(fp)) {
        perror("Error reading, did not reach end of file");
    }

    fclose(fp);
    shape.rows = row;
    shape.columns = col;

    return shape;
}

// Returns the table index which should give a radius of curvature
// closest to the requested one
int find_roc(double r, ArrayShape table_shape, double table[MAX_ROWS][MAX_COLS]) {
    int closest_index = 0;
    double dist = fabs(table[0][1] - r);
    for (int i = 0; i < table_shape.rows; i++) {
        double dist_tmp = fabs(table[i][1] - r);
        if (dist_tmp < dist) {
            closest_index = i;
            dist = dist_tmp;
        }
    }
    return closest_index;
}

static ArrayShape concave_shape = {.rows = 0, .columns = 0};
static ArrayShape convex_shape = {.rows = 0, .columns = 0};
static dbAddr paddr_roc;
static const char pname_roc[] = "4idHHLM:BenderTargetRoC.VAL";
static bool INITIALIZED = false;

static long bender_lookup_init(struct subRecord *psub) {

    if (!INITIALIZED) {
        // Load lookup tables in iocBoot/ioc4idHHLM/bender_data into arrays
        concave_shape = load_table("./bender_data/concave.csv", concave_table);
        convex_shape = load_table("./bender_data/convex.csv", convex_table);
        printf("Loaded concave bender lookup table with shape (%lu,%lu)\n", concave_shape.rows,
               concave_shape.columns);
        printf("Loaded convex bender lookup table with shape (%lu,%lu)\n", convex_shape.rows,
               convex_shape.columns);

        // Connect to the RoCTarget record
        dbNameToAddr(pname_roc, &paddr_roc);

        INITIALIZED = true;
    }

    return 0;
}

static long lookup_roc(struct subRecord *psub) {
    // "psub->a" is the requested radius of curvature
    // use concave table for positive RoC and convex for negative
    // dbPutField the nearest RoC to 4idHHLM:RoCTarget.VAL
    int index = 0;
    if (psub->a >= 0.0) {
        index = find_roc(psub->a, concave_shape, concave_table);
        psub->val = concave_table[index][0];
        const double roc = concave_table[index][1];
        dbPutField(&paddr_roc, DBR_DOUBLE, &roc, 1);
    } else {
        index = find_roc(psub->a, convex_shape, convex_table);
        psub->val = convex_table[index][0];
        const double roc = convex_table[index][1];
        dbPutField(&paddr_roc, DBR_DOUBLE, &roc, 1);
    }
    return 0;
}

epicsRegisterFunction(bender_lookup_init);
epicsRegisterFunction(lookup_roc);
