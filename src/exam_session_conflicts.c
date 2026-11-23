#include "exam_session_conflicts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void destroy_session(ExamSession *session) {
    if (session == NULL) {
        return;
    }
    if (session->exams != NULL) {
        for (int i = 0; i < session->exam_count; i++) {
            if (session->exams[i].student_ids != NULL) {
                for (int j = 0; j < session->exams[i].student_count; j++) {
                    free(session->exams[i].student_ids[j]);
                }
                free(session->exams[i].student_ids);
            }
            free(session->exams[i].code);
        }
        free(session->exams);
    }
    free(session->timestamp);

    session->timestamp = NULL;
    session->exams = NULL;
    session->exam_count = 0;
}

int read_session_file(FILE *session_file, ExamSession *session) {
    char buffer[256];
    if (fscanf(session_file, "%255s", buffer) != 1) {
        fprintf(stderr, "Input file error\n");
        return 1;
    }

    char *timestamp = malloc(strlen(buffer) + 1);
    if (timestamp == NULL) {
        fprintf(stderr, "Memory Allocation Error\n");
        return 1;
    }
    strcpy(timestamp, buffer);

    int exam_count;
    if (fscanf(session_file, " %d", &exam_count) != 1 || exam_count < 0) {
        fprintf(stderr, "Input file error\n");
        free(timestamp);
        return 1;
    }

    session->timestamp = timestamp;
    if (exam_count == 0) {
        session->exam_count = 0;
        session->exams = NULL;
        return 0;
    }

    Exam *exams = calloc(exam_count, sizeof(Exam));
    if (exams == NULL) {
        fprintf(stderr, "Memory Allocation Error\n");
        destroy_session(session);
        return 1;
    }
    session->exams = exams;
    session->exam_count = exam_count;

    for (int i = 0; i < exam_count; i++) {
        char code_buf[256];
        char path_buf[256];

        if (fscanf(session_file, "%255s %255s", code_buf, path_buf) != 2) {
            fprintf(stderr, "Input file error\n");
            destroy_session(session);
            return 1;
        }

        exams[i].code = malloc(strlen(code_buf) + 1);
        if (exams[i].code == NULL) {
            fprintf(stderr, "Memory Allocation Error\n");
            destroy_session(session);
            return 1;
        }
        strcpy(exams[i].code, code_buf);

        FILE *student_file = fopen(path_buf, "r");
        if (student_file == NULL) {
            fprintf(stderr, "Cannot open file: %s\n", path_buf);
            destroy_session(session);
            return 1;
        }

        int student_count;
        if (fscanf(student_file, "%d", &student_count) != 1 || student_count < 0) {
            fprintf(stderr, "Input file error\n");
            fclose(student_file);
            destroy_session(session);
            return 1;
        }

        exams[i].student_count = student_count;
        if (student_count == 0) {
            exams[i].student_ids = NULL;
        } else {
            exams[i].student_ids = calloc(student_count, sizeof(char *));
            if (exams[i].student_ids == NULL) {
                fprintf(stderr, "Memory Allocation Error\n");
                fclose(student_file);
                destroy_session(session);
                return 1;
            }
        }

        for (int j = 0; j < student_count; j++) {
            if (fscanf(student_file, "%255s", buffer) != 1) {
                fprintf(stderr, "Input file error\n");
                fclose(student_file);
                destroy_session(session);
                return 1;
            }

            exams[i].student_ids[j] = malloc(strlen(buffer) + 1);
            if (exams[i].student_ids[j] == NULL) {
                fprintf(stderr, "Memory Allocation Error\n");
                fclose(student_file);
                destroy_session(session);
                return 1;
            }
            strcpy(exams[i].student_ids[j], buffer);
        }

        fclose(student_file);
    }

    return 0;
}

int exam_has_student(const Exam *exam, const char *student_id) {
    for (int k = 0; k < exam->student_count; k++) {
        int cmp = strcmp(exam->student_ids[k], student_id);
        if (cmp == 0) {
            return 1;
        }
        if (cmp > 0) {
            break;
        }
    }
    return 0;
}
int seen_in_earlier_exam(const ExamSession *session, int current_exam, const char *student_id) {
    for (int prev = 0; prev < current_exam; prev++) {
        if (exam_has_student(&session->exams[prev], student_id)) {
            return 1;
        }
    }
    return 0;
}

void print_conflicts(const ExamSession *session) {
    int exam_count = session->exam_count;

    int total_conflicts = 0;
    for (int i = 0; i < exam_count; i++) {
        for (int j = 0; j < session->exams[i].student_count; j++) {
            const char *id = session->exams[i].student_ids[j];

            if (seen_in_earlier_exam(session, i, id)) {
                continue;
            }

            for (int other = i + 1; other < exam_count; other++) {
                if (exam_has_student(&session->exams[other], id)) {
                    total_conflicts++;
                    break;
                }
            }
        }
    }

    printf("CONFLICTS %d\n", total_conflicts);

    for (int i = 0; i < exam_count; i++) {
        for (int j = 0; j < session->exams[i].student_count; j++) {
            const char *id = session->exams[i].student_ids[j];

            if (seen_in_earlier_exam(session, i, id)) {
                continue;
            }

            int has_conflict = 0;
            for (int other = i + 1; other < exam_count; other++) {
                if (exam_has_student(&session->exams[other], id)) {
                    has_conflict = 1;
                    break;
                }
            }

            if (has_conflict) {

                printf("%s | %s", id, session->exams[i].code);
                for (int other = i + 1; other < exam_count; other++) {

                    if (exam_has_student(&session->exams[other], id)) {
                        printf(" %s", session->exams[other].code);
                    }
                }
                printf("\n");
            }
        }
    }
}
