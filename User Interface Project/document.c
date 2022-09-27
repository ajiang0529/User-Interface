#include <stdio.h>
#include <string.h>
#include "document.h"
#include <ctype.h>

/* Andy Jiang 118243733 ajiang25 */

int init_document(Document *doc, const char *name) {
    int name_length = 0;
    if (doc == NULL || name == NULL) {
        return FAILURE;
    }
    name_length = strlen(name);
    if (name_length > MAX_STR_SIZE) {
        return FAILURE;
    }
    strcpy(doc->name, name);
    doc->number_of_paragraphs = 0;
    return SUCCESS;
}
int reset_document(Document *doc){
    int i = 0;
    Paragraph p = {0};
    if (doc == NULL) {
        return FAILURE;
    }
    for (i = 0; i < doc->number_of_paragraphs; i++) {
         doc->paragraphs[i] = p;
    }
    doc->number_of_paragraphs = 0;
    return SUCCESS;
}
int print_document(Document *doc) {
    int i = 0, j = 0, name_length = strlen(doc->name);
    if (doc == NULL) {
       return FAILURE;
    }
    printf("Document name: \"");
    for (i = 0; i < name_length; i++) {
       printf("%c", doc->name[i]);
    }
    printf("\"\n");
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
    for (i = 0; i < doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            if (doc->paragraphs[i].lines[j][0] != '\0') {
                printf("%s\n", doc->paragraphs[i].lines[j]);
            }
        }     
        if (doc->paragraphs[i].number_of_lines != 0 && 
        i != doc->number_of_paragraphs - 1) {
            printf("\n");
        }
    }
    return SUCCESS;
}
int add_paragraph_after(Document *doc, int paragraph_number) {
    int i = 0;
    Paragraph p = {0};  
    if (doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS
       || paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }
    if (doc->number_of_paragraphs == 0) {
        doc->number_of_paragraphs++;
        doc->paragraphs[paragraph_number] = p;
        return SUCCESS;
    }
    for (i = doc->number_of_paragraphs; i >= paragraph_number; i--) {
        if (i == paragraph_number) {
            doc->paragraphs[i] = p;
        } else {
            doc->paragraphs[i] = doc->paragraphs[i - 1];
        }
    }
    doc->number_of_paragraphs++;
    return SUCCESS;
}
int add_line_after(Document *doc, int paragraph_number, int line_number, 
                  const char *new_line) {
    int i = 0;
    if (doc == NULL || new_line == NULL || paragraph_number > 
    doc->number_of_paragraphs || 
    doc->paragraphs[paragraph_number - 1].number_of_lines == 
    MAX_PARAGRAPH_LINES || line_number > doc->paragraphs[paragraph_number - 1]
    .number_of_lines) {
      return FAILURE;
    }
    if (doc->paragraphs[paragraph_number - 1].number_of_lines == 0) {
      strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number]
      , new_line);
       doc->paragraphs[paragraph_number - 1].number_of_lines++; 
       return SUCCESS;
    }
    for (i = doc->paragraphs[paragraph_number - 1].number_of_lines; 
    i >= line_number; i--) {
       if (i == line_number) {
          strcpy(doc->paragraphs[paragraph_number - 1].lines[i], new_line);
       } else {
          strcpy(doc->paragraphs[paragraph_number - 1].lines[i], 
          doc->paragraphs[paragraph_number - 1].lines[i - 1]);
       }
    }
    doc->paragraphs[paragraph_number - 1].number_of_lines++; 
    return SUCCESS;
}
int get_number_lines_paragraph(Document *doc, int paragraph_number, 
int *number_of_lines){
    if (number_of_lines == NULL || doc == NULL || 
    paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }
    *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
    return SUCCESS;
}
int append_line(Document *doc, int paragraph_number, const char *new_line) {
    if (doc == NULL || new_line == NULL || paragraph_number > 
    doc->number_of_paragraphs || doc->paragraphs[paragraph_number - 1].
    number_of_lines == MAX_PARAGRAPH_LINES) {
       return FAILURE;
    }
    strcpy(doc->paragraphs[paragraph_number - 1].
    lines[doc->paragraphs[paragraph_number - 1].number_of_lines], new_line);
    doc->paragraphs[paragraph_number - 1].number_of_lines++;
    return SUCCESS;
}
int remove_line(Document *doc, int paragraph_number, int line_number) {
    int i;
    if (line_number > doc->paragraphs[paragraph_number - 1].number_of_lines 
    || doc == NULL || paragraph_number > doc->number_of_paragraphs) {
        return FAILURE;
    }
    for (i = line_number - 1; i < doc->paragraphs[paragraph_number - 1].
    number_of_lines; i++) {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[i], 
        doc->paragraphs[paragraph_number - 1].lines[i + 1]);
    }
    doc->paragraphs[paragraph_number - 1].number_of_lines--;
    return SUCCESS;
}
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], 
int data_lines) {
    int i, added_paragraphs = 0;
    if (doc == NULL || data == NULL || data_lines == 0) {
        return FAILURE;
    }
    add_paragraph_after(doc, added_paragraphs++);
    for (i = 0; i < data_lines; i++) {
        if (strcmp(data[i], "") == 0) {
            add_paragraph_after(doc, added_paragraphs++);
        } else {
            append_line(doc, added_paragraphs, data[i]);
        }
    }
    return SUCCESS;
}
void replace_in_line(Document *doc, int paragraph_number, int line_number, 
const char *target, const char *replacement) {
    int boolean = 0, line_increment = 0, input_length = 
    strlen(doc->paragraphs[paragraph_number].lines[line_number]), 
    same_first_char = 0, target_length = strlen(target), 
    replacement_length = strlen(replacement), target_increment = 0, 
    output_increment = 0, i = 0;
    char output[MAX_STR_SIZE + 1];
    while (line_increment < input_length) {
        if (doc->paragraphs[paragraph_number].lines[line_number]
        [line_increment] == target[0]) {
            boolean = 1;
            same_first_char = line_increment;
            while (boolean != 0 && same_first_char < target_length 
            + line_increment) {
                if (doc->paragraphs[paragraph_number].lines[line_number]
                [same_first_char] != target[target_increment++]) {
                    boolean = 0;
                }
                same_first_char++;
            }
	    target_increment = 0;
            if (boolean == 1) {
                for (i = 0; i < replacement_length; i++) {
                    output[output_increment++] = replacement[i];
                }
                line_increment = same_first_char;
            } else {
                output[output_increment++] = doc->paragraphs[paragraph_number]
                .lines[line_number][line_increment];
                line_increment++;
            }
	    boolean = 0;
        } else {
            output[output_increment++] = doc->paragraphs[paragraph_number]
            .lines[line_number][line_increment];
            line_increment++;
        }
    }
    output[output_increment] = '\0';
    strcpy(doc->paragraphs[paragraph_number].lines[line_number], output);
}

int replace_text(Document *doc, const char *target, const char *replacement) {
    int i, j;
    if (doc == NULL || target == NULL || replacement == NULL) {
        return FAILURE;
    }
    for (i = 0; i < doc->number_of_paragraphs; i++) {
       for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
           replace_in_line(doc, i, j, target, replacement);
       }
    }
    return SUCCESS;
}
void highlight_in_line(Document *doc, int paragraph_number, int line_number, 
const char *target) {
    int boolean = 0, line_increment = 0, input_length = strlen(doc->paragraphs
    [paragraph_number].lines[line_number]), same_first_char = 0,
    target_length = strlen(target), target_increment = 0, output_increment = 0, 
    i = 0, highlight_start_length = strlen(HIGHLIGHT_START_STR),
    highlight_end_length = strlen(HIGHLIGHT_END_STR);
    char output[MAX_STR_SIZE + 1];
    while (line_increment < input_length) {
        if (doc->paragraphs[paragraph_number].lines[line_number]
        [line_increment] == target[0]) {
            boolean = 1;
            same_first_char = line_increment;
            while (boolean != 0 && same_first_char < target_length + 
            line_increment) {
                if (doc->paragraphs[paragraph_number].lines[line_number]
                [same_first_char] != target[target_increment++]) {
                    boolean = 0;
                }
                same_first_char++;
            }
            target_increment = 0;
            if (boolean == 1) {
                for (i = 0; i < highlight_start_length; i++) {
                    output[output_increment++] = HIGHLIGHT_START_STR[i];
                }
                for (i = 0; i < target_length; i++) {
                    output[output_increment++] = target[i];
                }
                for (i = 0; i < highlight_end_length; i++) {
                    output[output_increment++] = HIGHLIGHT_END_STR[i];
                }
                line_increment = same_first_char;
            } else {
                output[output_increment++] =  doc->paragraphs[paragraph_number]
                .lines[line_number][line_increment];
                line_increment++;
            }
            boolean = 0;
        } else {
            output[output_increment++] = doc->paragraphs[paragraph_number]
            .lines[line_number][line_increment];
            line_increment++;
        }
    }
    output[output_increment] = '\0';
    strcpy(doc->paragraphs[paragraph_number].lines[line_number], output);
}
int highlight_text(Document *doc, const char *target) {
    int i = 0, j = 0;
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }
    for (i = 0; i < doc->number_of_paragraphs; i++) {
       for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
           highlight_in_line(doc, i, j, target);
       }
    }
    return SUCCESS;
}
void remove_in_line(Document *doc, int paragraph_number, int line_number, 
const char *target) {
    int boolean = 0, line_increment = 0, input_length = strlen(doc->paragraphs
    [paragraph_number].lines[line_number]), same_first_char = 0, 
    target_length = strlen(target), target_increment = 0, 
    output_increment = 0, i = 0; char output[MAX_STR_SIZE + 1];
    while (line_increment < input_length) {
        if (doc->paragraphs[paragraph_number].lines[line_number]
        [line_increment] == target[0]) {
            boolean = 1;
            same_first_char = line_increment;
            while (boolean != 0 && same_first_char < target_length + 
            line_increment) {
                if (doc->paragraphs[paragraph_number].lines[line_number]
                [same_first_char] != target[target_increment++]) {
                    boolean = 0;
                }
                same_first_char++;
            }
            target_increment = 0;
            if (boolean == 1) {
		line_increment += target_length;
                for (i = 0; i < target_length; i++) {
                    output[output_increment++] = doc->paragraphs
                    [paragraph_number].lines[line_number][line_increment++];
                }
            } else {
                output[output_increment++] = doc->paragraphs[paragraph_number]
                .lines[line_number][line_increment];
                line_increment++;
            }
            boolean = 0;
        } else {
            output[output_increment++] = doc->paragraphs[paragraph_number]
            .lines[line_number][line_increment];
            line_increment++;
        }
    }
    output[output_increment] = '\0';
    strcpy(doc->paragraphs[paragraph_number].lines[line_number], output);
}
int remove_text(Document *doc, const char *target) {
    int i = 0, j = 0;
    if (doc == NULL || target == NULL) {
        return FAILURE;
    }
    for (i = 0; i < doc->number_of_paragraphs; i++) {
       for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
           remove_in_line(doc, i, j, target);
       }
    }
    return SUCCESS;
}
int save_document(Document *doc, const char *filename) {
    FILE *output_stream = fopen(filename, "w");
    int i = 0, j = 0;
    if (filename == NULL || doc == NULL || output_stream == NULL) {
        return FAILURE;
    } else { 
        for (i = 0; i < doc->number_of_paragraphs; i++) {
            for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
                if (doc->paragraphs[i].lines[j][0] != '\0') {
                    fputs(doc->paragraphs[i].lines[j], output_stream);
                    fputs("\n", output_stream);
		}
            }
            if (doc->paragraphs[i].number_of_lines != 0 && i != 
	    doc->number_of_paragraphs - 1) {
                fputs("\n", output_stream);
            }
         }
         fclose(output_stream);
         return SUCCESS;
    }
}
int load_file(Document *doc, const char *filename) {
    FILE *input = fopen(filename, "r");
    char line[MAX_STR_SIZE + 1], *new_line_remove;
    int i = 0, added_paragraphs = 0, non_spaces = 0, line_len = 0;
    if (filename == NULL || doc == NULL || input == NULL) {
        return FAILURE;
    } else {
        add_paragraph_after(doc, added_paragraphs++);
        while (fgets(line, MAX_STR_SIZE + 1, input) != NULL) {  
            new_line_remove = strchr(line, '\n');
            if (new_line_remove != NULL) {
                *new_line_remove = '\0';
            }
            line_len = strlen(line);
            for (i = 0; i < line_len; i++) {        
                if (isspace(line[i]) == 0) {        
                    non_spaces++;
                }
            }
            if (non_spaces == 0) {
                add_paragraph_after(doc, added_paragraphs++);
            } else {
                append_line(doc, added_paragraphs, line);
            }
            non_spaces = 0;
        }
        fclose(input);
        return SUCCESS;
    }
}

