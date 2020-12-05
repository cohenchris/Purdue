#ifndef MISC_H
#define MISC_H

char *get_content_type(char *filename);

typedef struct {
  char *display_name;
  char *url;
} file_link;

char *generate_dir_listing(char *dir_name, int num_files, file_link *files);


#endif // MISC_H
