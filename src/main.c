#include <stdio.h>
#include <stdlib.h>
#include "exam_session_conflicts.h"

FILE *open_input_file(const char *filename);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <sessions_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *session_file = open_input_file(argv[1]);
    if (session_file == NULL) {
        return EXIT_FAILURE;
    }

    ExamSession session = {NULL, NULL, 0};
    if (read_session_file(session_file, &session) != 0) {
        fclose(session_file);
        return EXIT_FAILURE;
    }

    print_conflicts(&session);

    destroy_session(&session);
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
