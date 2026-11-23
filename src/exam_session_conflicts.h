//
// Created by Deniz Görseval on 22/11/2026.
//

#ifndef EXAM_SESSION_CONFLICTS_EXAM_SESSION_CONFLICTS_H
#define EXAM_SESSION_CONFLICTS_EXAM_SESSION_CONFLICTS_H
#include <stdio.h>


typedef struct{
    char *code;
    char **student_ids;
    int student_count;
} Exam;

typedef struct{
    char *timestamp;
    Exam *exams;
    int exam_count;
} ExamSession;





void destroy_session(ExamSession *session);

// reads_session file and puts it into a

int read_session_file(FILE *session_file,
                        ExamSession *session);

int find_conflicts(ExamSession *session);

int exam_has_student(const Exam *exam, const char *student_id);

int seen_in_earlier_exam(const ExamSession *session, int current_exam, const char *student_id) ;

void print_conflicts(const ExamSession *session);



#endif //EXAM_SESSION_CONFLICTS_EXAM_SESSION_CONFLICTS_H
