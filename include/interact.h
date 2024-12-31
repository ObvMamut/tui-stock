
#ifndef INTERACT_H
#define INTERACT_H


char** read_yml(const char* filename, int* count);
void write_yml(const char* filename, const char* new_data);
void run_node_command(const char *market);

#endif // INTERACT_H