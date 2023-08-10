#ifndef __MTECH__PRETTY_TERMINAL_COLORS_H_
    #define __MTECH__PRETTY_TERMINAL_COLORS_H_

    #include <stdio.h>

    #define SET_DEFAULT "\033[0m"

    #define FG_BLACK  "\033[0;30m"
    #define FG_RED    "\033[0;31m"
    #define FG_GREEN  "\033[0;32m"
    #define FG_YELLOW "\033[0;33m"
    #define FG_BLUE   "\033[0;34m"
    #define FG_PURPLE "\033[0;35m"
    #define FG_CYAN   "\033[0;36m"
    #define FG_WHITE  "\033[0;37m"

    #define BG_BLACK  "\033[40m"
    #define BG_RED    "\033[41m"
    #define BG_GREEN  "\033[42m"
    #define BG_YELLOW "\033[43m"
    #define BG_BLUE   "\033[44m"
    #define BG_PURPLE "\033[45m"
    #define BG_CYAN   "\033[46m"
    #define BG_WHITE  "\033[47m"

    #define MLOG_error(err) fprintf(stderr, FG_RED err SET_DEFAULT "\n");
    // err = error as string (including type specifiers).
    // __VA_ARGS__ == values.
    #define MLOG_formatted_error(err, ...) fprintf(stderr, FG_RED err SET_DEFAULT "\n", __VA_ARGS__);
    #define MLOG_output_test(description, expectedOutput, input)\
        if( input == expectedOutput ) printf(description ": " FG_GREEN "[PASSED]" SET_DEFAULT "\n");\
        else printf(description ": " FG_RED "[FAILED]" SET_DEFAULT "\n");
    #define MLOG_log_array(arr, arrSize, typeFormatSpecifier)\
        for(unsigned i = 0 ; i < (unsigned ) arrSize ; ++i) {\
            printf("Index: %d | Value: " typeFormatSpecifier "\n", i, arr [i]); \
        }
    #define MLOG_log_matrix(description , arr, typeFormatSpecifier, width, height)\
        printf("Description: " description "\n"); for(unsigned i = 0; i < height; i++) {\
            printf("%d: [ ", i);\
            for(unsigned j = 0; j < width; j++) {\
                if(j != width - 1){ \
                    printf(typeFormatSpecifier ", ", arr[i][j]); \
                }\
                else {\
                    printf(typeFormatSpecifier, arr[i][j]); \
                }\
            }\
            printf(" ] \n"); \
        } 
    #define GET_ARR_SIZE(arr) sizeof( arr ) / sizeof ( arr [0] )
    #define MLOG_log_variable(label, varName, formatSpecifier)\
        printf(label " : " formatSpecifier "\n", varName);
    #define MLOG_normal(output) puts(output);

#endif

