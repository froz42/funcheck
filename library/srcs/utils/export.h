#ifndef EXPORT_FUNCTION_H
#define EXPORT_FUNCTION_H

/**
 * @brief This macro is used to export a function from a shared library.
 * 
 */
# define EXPORT __attribute__((visibility("default")))

#endif // EXPORT_FUNCTION_H