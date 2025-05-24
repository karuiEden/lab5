//
// Created by karui on 08.11.24.
//

#include "error_handle.h"

#include <stdio.h>
const char *error_messages[] = {
    "End of File", "Allocate of memory failed", "Invalid input", "Invalid enviroment variable", "Invalid index",
    "The search in the file descriptor wasn't completed correctly.", "Read from the fd wasn't completed correctly.", "Write in the fd wasn't completed correctly.",
    "Invalid fd", "Invalid pointer"
};
const char* specific_error_messages[] = {"Element did't find", "Tree is empty", "Duplicate key"};

int16_t error = OK;
void handle_error() {
    if (error <= OK){
    return;
    }
    if (error == EOF){
        fprintf(stderr , "\n%s\n", error_messages[0]);
        error = OK;
        return;
    }
    fprintf(stderr , "%s", error_messages[error]);
    if (error != ERR_ALLOC) error = OK;
}

void handle_specific_error(){
  fprintf(stderr , "\n%s\n", specific_error_messages[error+4]);
    error = OK;
}

