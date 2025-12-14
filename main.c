#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "circuit.h"

extern	int	yyparse();
extern	FILE	*yyin;
extern	FILE	*output;

void parse_file(char *vhdl_file) {
  yyin = fopen(vhdl_file, "r");
  if (yyin == 0) {
      fprintf(stderr, "Could not open vhdl input file (%s)\n", vhdl_file);
      exit(1);
    }

  init_circuit();
  
  printf("Parsing file: %s\n", vhdl_file);
  yyparse();
  resolve_dff_connections();
  connect_gates_to_output_wires();
  printf("\n");
  
  fclose(yyin);
  
  printf("Total gates after parsing: %d\n", circuit.gate_count);
  printf("Inputs: %d, Outputs: %d, DFFs: %d\n\n", 
         circuit.input_count, circuit.output_count, circuit.dff_count);
  
  assign_levels();

 
  insert_buffers();
  assign_levels();
  print_circuit();
  free_circuit();
}

char *program_path;

int is_executable (dir, name)
     char *dir, *name;
{
  struct stat buffer;
  char p[2048];

  strcpy (p, dir);
  strcat (p, "/");
  strcat (p, name);
  if (stat (p, &buffer) == -1)
    return 0;
  if (buffer.st_mode & 1)
    return 1;
  return 0;
}


char *get_program_path (name)
     char *name;
{
  char *result, *path;

  if (index (name, '/'))
    {
      result = (char *) malloc (strlen (name) + 1);
      strcpy (result, name);
      return result;
    }

  path = (char *) getenv ("PATH");
  if (!path){
    printf ("getenv failed to get value of PATH");
    exit(0);
  }

  while (*path) {
      char *scan;

      scan = path;
      while (*scan && (*scan != ':')) scan++;
      if (scan == path) {
	     path++;
	     continue;
	  }
      *scan = '\0';
      if (is_executable (path, name)) {
	     result = (char *) malloc (strlen (path) + strlen (name) + 2);
	     strcpy (result, path);
	     strcat (result, "/");
	     strcat (result, name);
	     *scan = ':';
	     return result;
	  }
      *scan = ':';
      path = scan + 1;
  }
  printf ("could not locate executable file for '%s'", name);
  exit(0);
}


int main (argc, argv)
     int argc;
     char **argv;
{
  char *op, *argv0;
  int files;

  argv0 = argv[0];
  program_path = get_program_path (argv0);

  argv++;
  files = 0;
  while (*argv) {
      op = *argv++;
      if (*op != '-') {
	     parse_file (op);
	     files++;
	     continue;
	  }
      op++;
   }
}