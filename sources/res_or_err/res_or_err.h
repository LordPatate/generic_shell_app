#ifndef RES_OR_ERR_H
#define RES_OR_ERR_H

enum res_or_err_type {
    ERROR_TYPE,
    RESULT_TYPE
};

struct result_or_error {
    enum res_or_err_type ok;
    union {
        void *result;
        int error;
    };
};

#define RESULT(CONTENT) (struct result_or_error) {RESULT_TYPE, .result = (CONTENT)}
#define ERROR(ERROR_CODE) (struct result_or_error) {ERROR_TYPE, .error = (ERROR_CODE)}

#endif // !RES_OR_ERR_H
