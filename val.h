#ifndef MUHLISP_VAL_H_
#define MUHLISP_VAL_H_

enum muhlisp_val_type {
    MUHLISP_VAL_ERR,
    MUHLISP_VAL_NUMBER,
    MUHLISP_VAL_SYM,
    MUHLISP_VAL_SEXPR,
};

typedef enum muhlisp_val_type muhlisp_val_type_t;

typedef struct {
    muhlisp_val_type_t type;
    void* pval;
} muhlisp_val_t;

typedef struct {
    int count;
    muhlisp_val_t** pvals;
} muhlisp_sexpr_list_t;

muhlisp_val_t muhlisp_val_errorf(char* fmt, ...);
muhlisp_val_t muhlisp_val_double(double val);
muhlisp_val_t muhlisp_val_sexpr(int count);
muhlisp_val_t muhlisp_val_symbol(const char* symbol);

muhlisp_val_t* muhlisp_val_ptr(muhlisp_val_t val);

char* muhlisp_val_str(muhlisp_val_t* val);
void muhlisp_val_free(muhlisp_val_t* val);

#endif
