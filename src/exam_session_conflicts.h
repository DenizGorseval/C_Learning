//
// Created by Deniz Görseval on 22/11/2026.
//

#ifndef EXAM_SESSION_CONFLICTS_EXAM_SESSION_CONFLICTS_H
#define EXAM_SESSION_CONFLICTS_EXAM_SESSION_CONFLICTS_H
#include <stdio.h>


typedef struct{
    char *code;
    char **student_ids;
    int StudentCount;
} Exam;

// reads_session file and puts it into a

int read_session_file(FILE *session_file, Exam *exam_sessions ) {

}


#endif //EXAM_SESSION_CONFLICTS_EXAM_SESSION_CONFLICTS_H
