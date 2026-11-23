#include <stdio.h>
#include <stdlib.h>
#include "exam_session_conflicts.h"
#include "exam_session_conflicts.c"

FILE *open_input_file(const char *filename);

int main(int argc, char *argv[]) {
    /* TODO: complete the exam-session conflict detector. */
    if (argc != 2) {
        fprintf(stderr, "Usage %s <sessions_file> \n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *session_file = open_input_file(argv[1]);
    if (session_file == NULL) {
        return EXIT_FAILURE;
    }
    Exam exam_session = {NULL, NULL, 0};
    if (read_session_file(session_file, &exam_session) == 1) {
        fclose(session_file);
        return EXIT_FAILURE;
    }




    fclose(session_file);
    return EXIT_SUCCESS;
}

FILE *open_input_file(const char *filename) {
    FILE *input_file = fopen(filename, "r");

    if (input_file == NULL) {
        fprintf(stderr, "Cannot open input file: %s\n", filename);
    }
    return input_file;
}