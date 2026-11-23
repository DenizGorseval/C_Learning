//
// Created by Deniz Görseval on 22/11/2026.
//


#include "exam_session_conflicts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int read_session_file(FILE *session_file,  Exam *exam_sessions) {
    char buffer[256];
    if (fscanf(session_file,"%255s", &buffer) != 1) {
        fprintf(stderr, "Input file error \n");
        return 1;
    }
    char *timestamp = malloc(strlen(buffer)+1 * sizeof(char));

    int ExamCount;
    if (fscanf(session_file, " %d", &ExamCount) != 1) {
        fprintf(stderr, "Input file error \n");
        return 1;
    }

    for (int i = 0; i < ExamCount; i++) {

    }


    return 0;
}
