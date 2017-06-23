/*
* Program: Iterative calculations (2. project in IZP)
* Written by David Bolvansky (xbolva00)
* November 2015
* Note: Implemented binary search of iterations
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <limits.h>

/*
* Error codes
*/
enum error_codes {OK, INPUT_ERROR, RANGE_ERROR, EPS_ERROR, NUMBER_FORMAT_ERROR};

/*
* Program options
*/
enum options {UNKNOWN_OPTION, FIND_LOGARITHM, SEARCH_ITERATIONS};

/*
* Logarithms
*/
enum logarithms {UNKNOWN_ALGORITHM, USE_TAYLOR, USE_CFRAC};


/*
* Parameters structure
*/
struct parameters {    
       double number;
       double iterations_number;
       double min;
       double max;
       double eps;
       int option;
       int error_code;
};


/*
* Fuction prototypes
*/
double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
void search_iterations(double min, double max, double eps);
double get_diff_value(double first_number, double second_number);
double get_abs_value(double number);
int is_equal(double first_number, double second_number, double eps);
void find_left_right_values(double number, double eps, unsigned int *left_value, unsigned int *right_value, int use_algorithm);
unsigned int binary_search_iterations(double number, double eps, int use_algorithm );
void find_logarithm(double number, double iterations_number);
int init_core(struct parameters params);
struct parameters process_parameters(int argc, char *argv[]);



/**
* Calculation of the logarithm through the development of chain fraction
* @x: The given number
* @n: Number of the given iterations
* Returns the logaritm of a number
*/
double cfrac_log(double x, unsigned int n) {
    /* 
    * If the number is -INF, NAN or it is smaller than zero, return NAN
    */
    if (isnan(x) || x < 0) {
        return NAN;
    }
    
    /*
    * Handle infinity number
    */
    if (isinf(x)) {
        return INFINITY;
    }
    
    /* 
    * If the number is -INF, NAN or it is smaller than zero, return NAN
    */
    if (x == -INFINITY || isnan(x) || x < 0) {
        return NAN;
    }
    
    /*
    * Handle zero number as the iterations_number
    */
    if (n == 0) {
        return 0.0;
    }
    
    double result = 0.0;
    /*
    * Decrease number of iterations, since 2*z / (1 - fraction) is
    * the first iteration. Number of iterations = Number of divisions
    */
    n--;
    if (x > 0.0) {
        double z = (x - 1)/(x + 1);
        double fraction = 0.0; 
        double z2 = z * z;
        double numerator = z2;
    
        for (double i = n; i >= 1; --i) { 
            unsigned int first_number = 2 * i + 1;
            numerator = i * i * z2;
            fraction = numerator / (first_number - fraction); 
        }

        /*
        * The first iteration
        */
        result = 2*z / (1 - fraction);
        return result;
    } else {
        /*
        * number = 0
        */
        return -INFINITY;
    }
    return result;  
}

/**
* Calculation of the logarithm through the Taylor series
* @x: The given number
* @n: Number of the given iterations
* Returns the logaritm of a number
*/
double taylor_log(double x, unsigned int n) {
    /* 
    * If the number is -INF, NAN or it is smaller than zero, return NAN
    */
    if (isnan(x) || x < 0) {
        return NAN;
    }

    /*
    * Handle infinity number
    */
    
    if (isinf(x)) {
        return INFINITY;
    }
 
    /* 
    * If the number is -INF, NAN or it is smaller than zero, return NAN
    */
    if (x == -INFINITY || isnan(x) || x < 0) {
        return NAN;
    }
    
    /*
    * Handle zero number as the iterations_number
    */
    if (n == 0) {
        return 0.0;
    }

    double result = 0.0;
    unsigned int i = 1;
    double numerator = 1.0;
    double denumerator = 1.0;
    if (x > 0.0 && x < 1.0) {
        x = 1.0 - x;
        while (i <= n) {
              numerator = numerator * x;
              result-= numerator / denumerator;
              denumerator++;
              i++;
        }
        return result;

    }
    else if (x > 1.0) {
        while (i <= n) {
              numerator = numerator * ((x - 1) / x);
              result+= numerator / denumerator;
              denumerator++;
              i++;
        }
        return result;

    } else if (x > 0.0) {
        /*
        * number = 1.0
        */
        return 0.0;
    } else {
        /*
        * number = 0.0
        */
        return -INFINITY;
    }
    return result;
}

/**
* Get the difference of the two given numbers
* @first_number: The first number
* @second_number: The second number
* Returns the difference of two numbers
*/
double get_diff_value(double first_number, double second_number) {
    if (first_number > second_number) {
        return first_number - second_number;
    } else {
        return second_number - first_number;
    }
    return 0.0;
    
}

/** 
* Get absolute value of the given number
* @number: The given number
* Returns the absolute value of a number
*/
double get_abs_value(double number) {
    if (number > 0.0) {
        return number;
    } else {
        return -number;
    }
    return 0.0;
    
}

/** 
* Check whether the given numbers are equal
* @first_number: The first number
* @second_number: The second number
* @eps: EPS (epsilon)
* Returns 1 or 0 depending on the fact whether the numbers are equal or not
*/

int is_equal(double first_number, double second_number, double eps) {
    return get_abs_value(first_number - second_number) <= eps;
}


/**
* Search left and right values for a binary search of the number of iterations
* @number: The number for which we find the right number of iterations for.
* @eps: EPS (epsilon)
* @left_value: The left value for a binary search
* @right_value: The right value for a binary search
* @use_algorithm: Whether to use algoritm for the Taylor series or continued fractions
*/
void find_left_right_values(double number, double eps, unsigned int *left_value, unsigned int *right_value, int use_algorithm) {
    unsigned int previous_iterations_number = 1;
    unsigned int iterations_number = 1;
    double log_value = 0.0;
    double real_log = log(number);
    
    while (!is_equal(real_log, log_value,eps)) {
        previous_iterations_number = iterations_number;
        if (use_algorithm == USE_TAYLOR) {
            log_value = taylor_log(number, iterations_number);   
        }
        else if (use_algorithm == USE_CFRAC) {
            log_value = cfrac_log(number, iterations_number);
        }
        else {
            /*
            * Unknown algorithm
            */
            break;
        }
        
        /*
        * Function to estimate the number of iterations
        * y = 10*y
        * Handle overflow
        */
        if ((UINT_MAX / 10) >  iterations_number) {
            iterations_number = iterations_number * 10;
        }
        else {
            fprintf(stderr, "Unsigned int overflow\n");
            previous_iterations_number = 10;
            iterations_number = 1;
            break;
        }
    }
    
    previous_iterations_number = previous_iterations_number / 10;
    
    if (previous_iterations_number == 0) {
        previous_iterations_number = 1;
    }
    
    /* 
    * The left value will be the last number of iterations, for which
    * the condition in while loop is met
    */
    *left_value = previous_iterations_number;
    
    /* 
    * The left value will be the first number of iterations, for which
    * the condition in the while loop is NOT met
    */
    
    *right_value = iterations_number;
}

/** 
* Use binary search to get the right number of iterations for entered number, eps and algoritm
* @number: The number for which we find the right number of iterations for.
* @eps: EPS (epsilon)
* Returns the number of iterations
*/
unsigned int binary_search_iterations(double number, double eps, int use_algorithm) {
    double current_value = 0.0;
    double real_log = log(number);
    unsigned int right_value = 0;
    unsigned int left_value = 0;

    unsigned int mid_value = 1;
    unsigned int iterations_number = 1;
    double log_value_mid = 1.0;
    double log_value_previous_mid = 1.0;
    
    /*
    * Handle special cases
    */
    if (isinf(number) || isnan(number)) {
        return 0;
    }
    if (number == 1.0 || number <= 0) {
        return 0;
    }
    
    find_left_right_values(number, eps, &left_value, &right_value, use_algorithm);
    
    /*
    * Algoritm for binary search
    */
    while (get_abs_value(real_log-log_value_mid) > eps || get_abs_value(real_log-log_value_previous_mid) <= eps) {
        mid_value = (left_value + right_value) / 2;
        if (use_algorithm == USE_TAYLOR) {
            current_value = taylor_log(number, mid_value);          
        }
        else if (use_algorithm == USE_CFRAC){
            current_value = cfrac_log(number, mid_value);
        }
        else {
            /*
            * Unknown algorithm
            */
            break;
        }
        
        if (is_equal(real_log, current_value, eps)) {
            right_value = mid_value - 1;
        }
        
        else {
            left_value = mid_value + 1;
        }
        
        
        iterations_number = (left_value + right_value) / 2;
        if (iterations_number == 0) {
            iterations_number = 1;
            break;
        }
        
        if (use_algorithm == USE_TAYLOR) {
            /*
            * Get logarithm for the current iterations_number
            */
            log_value_mid = taylor_log(number, iterations_number); 
            /*
            * Get logarithm for the current iterations_number - 1
            */           
            log_value_previous_mid = taylor_log(number, iterations_number - 1);            
        }
        else if (use_algorithm == USE_CFRAC){
            /*
            * Get logarithm for the current iterations_number
            */
            log_value_mid = cfrac_log(number, iterations_number); 
            /*
            * Get logarithm for the current iterations_number - 1
            */  
            log_value_previous_mid = cfrac_log(number, iterations_number - 1);  
        }
        else {
            /*
            * Unknown algorithm
            */
            break;
        }
        
    }
    
    return iterations_number;
}


/**
* Search the number of iterations for the given interval defined by the minimal and
* maximal number for the given eps value
* @min: Minimal number of the chosen interval
* @max: Maximal number of the chosen interval
*/
void search_iterations(double min, double max, double eps) {    
    printf("       log(%g) = %.12g\n", min, log(min));
    printf("       log(%g) = %.12g\n", max, log(max));
    
    unsigned int iter_cfrac_min = binary_search_iterations(min, eps, USE_CFRAC);
    unsigned int iter_cfrac_max = binary_search_iterations(max, eps, USE_CFRAC);
    unsigned int iter_cfrac_final = (iter_cfrac_min > iter_cfrac_max) ? iter_cfrac_min : iter_cfrac_max;
    
    printf("continued fraction iterations = %u\n", iter_cfrac_final);
    printf("    cf_log(%g) = %.12g\n", min, cfrac_log(min, iter_cfrac_final));
    printf("    cf_log(%g) = %.12g\n", max, cfrac_log(max, iter_cfrac_final));
    
    unsigned int iter_taylor_min =  binary_search_iterations(min, eps, USE_TAYLOR);
    unsigned int iter_taylor_max = binary_search_iterations(max, eps, USE_TAYLOR);
    unsigned int iter_taylor_final = (iter_taylor_min > iter_taylor_max) ? iter_taylor_min : iter_taylor_max;
    
    printf("taylor polynomial iterations = %u\n", iter_taylor_final); 
    printf("taylor_log(%g) = %.12g\n", min, taylor_log(min, iter_taylor_final));
    printf("taylor_log(%g) = %.12g\n", max, taylor_log(max, iter_taylor_final));  
}

/**
* Find logarithms using the Taylor series or or continued fractions
* @number: The given number
* @iterations_number: Number of the given iterations
*/
void find_logarithm(double number, double iterations_number) {
      printf("       log(%g) = %.12g\n", number, log(number));
      printf("    cf_log(%g) = %.12g\n", number, cfrac_log(number, iterations_number));
      printf("taylor_log(%g) = %.12g\n", number, taylor_log(number, iterations_number));
    
}

/**
* Starts core of the program, performs the chosen options
* @params: The processed paramaters
* Returns error code, OK is success
*/
int init_core(struct parameters params) {
    if (params.option == FIND_LOGARITHM) {
        find_logarithm(params.number, params.iterations_number);
    }
    else if (params.option == SEARCH_ITERATIONS) {
        search_iterations(params.min, params.max, params.eps);
    }
    else {
        return UNKNOWN_OPTION;
    }
    return OK;
}

/**
* Process paramaters from input
* @argc: The number of parameters
* @argv: The parameters
* Returns parameters structure
*/
struct parameters process_parameters(int argc, char *argv[]) {
    struct parameters params;
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "--log") == 0) {
            double number = 0.0;
            unsigned long int iterations_number = 0;
            char *first_error = NULL, *second_error = NULL;
            char *first_parameter = argv[i+1];
            char *second_parameter = argv[i+2];

            /*
            * Check whether the given parameters are not empty (NULL)
            */
            if (first_parameter == NULL || second_parameter == NULL) {
                fprintf(stderr, "Paramater error\n");
                params.error_code = INPUT_ERROR;
                return params;
            }

            /*
            * Check overflow
            */
            errno = 0;
            number = strtod(first_parameter, &first_error);
            if (errno == ERANGE) {
                fprintf(stderr, "Number is out of range\n");
                params.error_code = RANGE_ERROR;
                return params;               
            }
            
            /*
            * Check overflow
            */
            errno = 0;
            iterations_number = strtoul(second_parameter, &second_error, 10);
            if (errno == ERANGE || iterations_number > UINT_MAX) {
                fprintf(stderr, "Number of iterations is out of range\n");
                params.error_code = RANGE_ERROR;
                return params;               
            }

            if (*first_error || *second_error) {
                fprintf(stderr, "Wrong format of the numbers\n");
                params.error_code = NUMBER_FORMAT_ERROR;
                return params;
            }
            params.number = number;
            params.iterations_number = iterations_number;
            params.option = FIND_LOGARITHM;
            params.error_code = OK;
            return params;

        }
        if (strcmp(argv[i], "--iter") == 0) {

            double min = 0.0;
            double max = 0.0;
            double eps = 0.0;
            char *first_error = NULL, *second_error = NULL, *third_error = NULL;
            char *first_parameter = argv[i+1];
            char *second_parameter = argv[i+2];
            char *third_parameter = argv[i+3];

            /*
            * Check whether the given parameters are not empty (NULL)
            */
            if (first_parameter == NULL || second_parameter == NULL || third_parameter == NULL) {
                fprintf(stderr, "Parameters error\n");
                params.error_code = INPUT_ERROR;
                return params;
            }


            /*
            * Check overflow
            */
            errno = 0;
            eps = strtod(third_parameter, &third_error);
            if (errno == ERANGE) {
                fprintf(stderr, "EPS is out of range\n");
                params.error_code = RANGE_ERROR;
                return params;               
            }

            if (eps < 1e-12) {
                fprintf(stderr, "EPS is smaller than 1e-12\n");
                params.error_code = EPS_ERROR;
                return params;
            }
            
            /*
            * Handle special cases
            */
            if (isinf(eps) || isnan(eps)) {
                fprintf(stderr, "Invalid EPS\n");
                params.error_code = EPS_ERROR;
                return params;
            }

            /*
            * Check overflow
            */
            errno = 0;
            min = strtod(first_parameter, &first_error);
            if (errno == ERANGE) {
                fprintf(stderr, "Min is out of range\n");
                params.error_code = RANGE_ERROR;
                return params;               
            }
            
            /*
            * Check overflow
            */
            errno = 0;
            max = strtod(second_parameter, &second_error);
            if (errno == ERANGE) {
                fprintf(stderr, "Max is out of range\n");
                params.error_code = RANGE_ERROR;
                return params;               
            }

            if (*first_error || *second_error || *third_error) {
                fprintf(stderr, "Wrong format of the numbers\n");
                params.error_code = RANGE_ERROR;
                return params;  
            }

            /*
            * Switch min and max if max is smaller than min
            */
            if (min > max) {
                double temp;
                temp = min;
                min = max;
                max = temp;
            }
            
            params.min = min;
            params.max = max;
            params.eps = eps;
            params.option = SEARCH_ITERATIONS;
            params.error_code = OK;
            return params;
        }
        i = i + 1;
    }
    return params;
}
 
/**
* Main function
* @argc: The number of the parameters
* @argv: The given parameters
* Returns 0 if success
*/

int main(int argc, char *argv[]) {
  /*
  * Inicialize values in params structure
  */
  
  struct parameters params;
  
  params.number = 0.0;
  params.iterations_number = 0;
  params.min = 0.0;
  params.max = 0.0;
  params.eps = 0.0;
  params.option = UNKNOWN_OPTION;
  params.error_code = OK;

  /*
  * Process parameters from input
  */
  params = process_parameters(argc, argv);
  if (params.error_code != OK) {
      fprintf(stderr, "Input error (error code: %d)\n", params.error_code);
      /*
      * Show help
      */
      printf("Program options:\n--log x n\n--iter min max eps\n");
      return params.error_code;
  }
  
  int error_code = init_core(params);
  if (error_code != OK) {
       fprintf(stderr, "Unknown option\n");         
  }
  return EXIT_SUCCESS;
}
