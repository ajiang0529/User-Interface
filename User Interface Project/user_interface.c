#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <stdlib.h>
#include <ctype.h>
#include "document.h"
#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[]) {
    FILE *input;
    char line[MAX_LINE_LENGTH + 1], file_name[MAX_LINE_LENGTH + 1], command[MAX_LINE_LENGTH + 1], 
    symbol[MAX_LINE_LENGTH + 1], additional_info[MAX_LINE_LENGTH + 1], 
    paragraph_num[MAX_LINE_LENGTH + 1], line_num[MAX_LINE_LENGTH + 1], *line_param_output, 
    *new_line_remove, target_param[MAX_LINE_LENGTH + 1] = "", replacement_param[MAX_LINE_LENGTH + 1] = ""; 
    Document d1;
    int line_read = 0, command_read = 0, end_message = 0, int_paragraph_num = 0, int_line_num = 0, 
    target_start = 0, target_end = 0, replacement_start = 0,
    i = 0, j = 0, line_len = 0, boolean = 0, quote_counter = 0;
    init_document(&d1, "main_document");

    if (argc == 1) {
        input = stdin; 
    } else if (argc == 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "%s cannot be opened.", argv[1]);
            return EX_OSERR; 
        }
    } else {
        fprintf(stderr, "Usage: user_interface\n");
        fprintf(stderr, "Usage: user_interface <filename>\n");
        return EX_USAGE;
    }
    while (fgets(line, MAX_LINE_LENGTH, input) != NULL) { 
	    if (input == stdin) {
	        printf("> ");
	    }
	    new_line_remove = strchr(line, '\n');
        if (new_line_remove != NULL) {
            *new_line_remove = '\0';
        }
        line_read = sscanf(line, " %s", command);
        if (line_read != EOF) {
            if (strcmp(command, "add_paragraph_after") == 0) {
            command_read = sscanf(line, " %s %s %s", command, paragraph_num, additional_info);
                if (isdigit(paragraph_num[0]) == 0) {
                    fprintf(stdout, "Invalid Command\n");
            	} else {
                    int_paragraph_num = atoi(paragraph_num);
            	}
            	if (int_paragraph_num < 0) {
                   fprintf(stdout, "Invalid Command\n");
		   continue;
            	}
            if (command_read != 2) {
                fprintf(stdout, "Invalid Command\n");
		continue;  
            } else {
                end_message = add_paragraph_after(&d1, int_paragraph_num);
                if (end_message == FAILURE) {
                    fprintf(stdout, "add_paragraph_after failed\n");
                }
            }
        } else if (strcmp(command, "add_line_after") == 0) {
            command_read = sscanf(line, " %s %s %s %s", command, paragraph_num, line_num, symbol);
	    for (i = 0; i < strlen(line); i++) {
		if (line[i] == '*') {
		    boolean = 1;
		}
	    }
            line_param_output = memchr(line, '*', strlen(line) + 1);
	    if (boolean == 0) {
		fprintf(stdout, "Invalid Command\n");
		continue;
	    }
	    if (command_read < 4) {
		fprintf(stdout, "Invalid Command\n");
		continue;
	    }
            if (isdigit(paragraph_num[0]) == 0 || isdigit(line_num[0]) == 0) {
                fprintf(stdout, "Invalid Command\n");
		continue;
            } 
            int_paragraph_num = atoi(paragraph_num);
            int_line_num = atoi(line_num);
            if (line_param_output == NULL) {
                fprintf(stdout, "Invalid Command ala4\n");
		continue;
            }
            if (int_paragraph_num <= 0 || int_line_num < 0) {
                fprintf(stdout, "Invalid Command\n");
		continue;
            }
            end_message = add_line_after(&d1, int_paragraph_num, int_line_num, line_param_output + 1);
            if (end_message == FAILURE) {
                fprintf(stdout, "add_line_after failed\n");
            }
	    boolean = 0;
        } else if (strcmp(command, "print_document") == 0) {
            command_read = sscanf(line, " %s %s", command, additional_info);
            if (command_read > 1) {
                fprintf(stdout, "Invalid Command\n");
		continue;
            } else if (command_read == 1) {
                end_message = print_document(&d1);
                if (end_message == FAILURE) {
                    fprintf(stdout, "print_document failed\n");
                }
            }
        } else if (strcmp(command, "quit") == 0) {
            command_read = sscanf(line, " %s %s", command, additional_info);
            if (command_read > 1) {
                fprintf(stdout, "Invalid Command\n");
            } else if (command_read == 1) {
                return 0;
            }
	} else if (strcmp(command, "exit") == 0) {
            command_read = sscanf(line, " %s %s", command, additional_info);
            if (command_read > 1) {
                fprintf(stdout, "Invalid Command\n");
		continue;
            } else if (command_read == 1) {
                return 0;
            }
        } else if (strcmp(command, "append_line") == 0) {
            command_read = sscanf(line, " %s %s %s", command, paragraph_num, symbol);
	    for (i = 0; i < strlen(line); i++) {
                if (line[i] == '*') {
                    boolean = 1;
                }
            }
            line_param_output = memchr(line, '*', strlen(line) + 1);
            if (boolean == 0) {
                fprintf(stdout, "Invalid Command\n");
		continue;
            }
	    if (command_read < 3) {
		fprintf(stdout, "Invalid Command\n");
		continue;	
	    }
	    if (line_param_output == NULL) {
		fprintf(stdout, "Invalid Command\n");
		continue;	
	    }
            if (isdigit(paragraph_num[0]) == 0) {
                fprintf(stdout, "Invalid Command\n");
		continue;
            } else if (isdigit(paragraph_num[0]) != 0) {
                int_paragraph_num = atoi(paragraph_num);
            }
	    if (int_paragraph_num <= 0) {
		fprintf(stdout, "Invalid Command\n");
		continue;	
	    }
            end_message = append_line(&d1, int_paragraph_num, line_param_output + 1);
            if (end_message == FAILURE) {
                fprintf(stdout, "append_line failed\n");
            }
	    boolean = 0;
        } else if (strcmp(command, "remove_line") == 0) {
            command_read = sscanf(line, " %s %s %s %s", command, paragraph_num, line_num, additional_info);
            if (isdigit(paragraph_num[0]) == 0 || isdigit(line_num[0]) == 0) {
                fprintf(stdout, "Invalid Command\n");
		continue;
            } else if (isdigit(paragraph_num[0]) != 0 && isdigit(line_num[0]) != 0) {
                int_paragraph_num = atoi(paragraph_num);
                int_line_num = atoi(line_num);
            }
            if (int_paragraph_num <= 0 || int_line_num <= 0) {
                fprintf(stdout, "Invalid Command\n");
		continue;
            }
            if (command_read != 3) {
                fprintf(stdout, "Invalid Command\n");  
            	continue;
	    } else {
                end_message = remove_line(&d1, int_paragraph_num, int_line_num);
                if (end_message == FAILURE) {
                    fprintf(stdout, "remove_line failed\n");
                }
            }
        } else if (strcmp(command, "load_file") == 0) {
            command_read = sscanf(line, " %s %s %s", command, file_name, additional_info);
            if (command_read != 2) {
                fprintf(stdout, "Invalid Command\n");
		continue;  
            } else {
                end_message = load_file(&d1, file_name);
                if (end_message == FAILURE) {
                    fprintf(stdout, "load_file failed\n");
                }
            }
        }  else if (strcmp(command, "replace_text") == 0) {
            strcpy(replacement_param, "");
            strcpy(target_param, "");
            j = 0;
            quote_counter = 0;
            line_len = strlen(line);
            for (i = 0; i < line_len; i++) {
                if (line[i] == '"') {
		    quote_counter++;
                    target_start = i + 1;
                    break; 
                }
            }
            for (i = target_start; i < line_len; i++) {
                if (line[i] == '"') {
		    quote_counter++;
                    target_end = i + 1;
                    break;
                } else {
                    target_param[j++] = line[i];
                }
            }
            j = 0;
            for (i = target_end; i < line_len; i++) {
                if (line[i] == '"') {
		    quote_counter++;
                    replacement_start = i + 1;
                    break;
                }
            }
	    if (quote_counter == 3) {
            	for (i = replacement_start; i < line_len; i++) {
                    if (line[i] == '"') {
                    	break;
                    } else {
                    	replacement_param[j++] = line[i];
                    }
            	}
	    } 
            if (target_start == 0 && replacement_start == 0) {
                fprintf(stdout, "Invalid Command\n");
	        continue;	
            }
            if (replacement_start == 0) {
                fprintf(stdout, "Invalid Command\n");
		continue;
            }
            end_message = replace_text(&d1, target_param, replacement_param);
            if (end_message == FAILURE) {
                fprintf(stdout, "replace_text failed\n");
            }
        } else if (strcmp(command, "highlight_text") == 0) {
            strcpy(target_param, "");
            j = 0;
            line_len = strlen(line);
            for (i = 0; i < line_len; i++) {
                if (line[i] == '"') {
                    target_start = i + 1;
                    break; 
                }
            }
            for (i = target_start; i < line_len; i++) {
                if (line[i] == '"') {
                    break;
                } else {
                    target_param[j++] = line[i];
                }
            }
            if (target_start == 0) {
                fprintf(stdout, "Invalid Command\n");
                continue;
            }
            end_message = highlight_text(&d1, target_param);
            if (end_message == FAILURE) {
                fprintf(stdout, "highlight_text failed\n");
            }
        } else if (strcmp(command, "remove_text") == 0) {
            line_len = strlen(line);
            j = 0;
            strcpy(target_param, "");
            for (i = 0; i < line_len; i++) {
                if (line[i] == '"') {
                    target_start = i + 1;
                    break; 
                }
            }
            for (i = target_start; i < line_len; i++) {
                if (line[i] == '"') {
                    break;
                } else {
                    target_param[j++] = line[i];
                }
            }
            if (target_start == 0) {
                fprintf(stdout, "Invalid Command\n");
		continue;
            }
            end_message = remove_text(&d1, target_param);
            if (end_message == FAILURE) {
                fprintf(stdout, "remove_text failed\n");
            }
        } else if (strcmp(command, "save_document") == 0) {
            command_read = sscanf(line, " %s %s %s", command, file_name, additional_info);
            if (command_read != 2) {
                fprintf(stdout, "Invalid Command\n");  
		continue;
            } else {
                end_message = save_document(&d1, file_name);
                if (end_message == FAILURE) {
                    fprintf(stdout, "save_document failed\n");
                }
            }    
        } else if (strcmp(command, "reset_document") == 0) {
            command_read = sscanf(line, " %s %s", command, additional_info);
            if (command_read != 1) {
                fprintf(stdout, "Invalid Command\n");
		continue;  
            } else {
                end_message = reset_document(&d1);
                if (end_message == FAILURE) {
                    fprintf(stdout, "reset_document failed\n");
                }
            }
	    } else if (strcmp(command, "") != 0) {
            command_read = sscanf(line, " %s", command);
            if (command[0] != '#') {
                fprintf(stdout, "Invalid Command\n");
                continue;
            }
	    }
    }
    }
    return 0;
}



