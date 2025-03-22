#include "arena.h"
#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

void usage(const char *name)
{
  fprintf(stderr, "Usage: %s [OPTIONS] <input file>\n\n", name);
  fprintf(stderr, "\t-o <output file>\n\t\tif no output file is specified use stdout.\n");
  fprintf(stderr, "\t-d\n\t\tdump ast to stdout.\n");
  fprintf(stderr, "\t-c\n\t\tcompile file to asm.\n");
  fprintf(stderr, "\t-h\n\t\tshow this message.\n\n");
}

typedef struct
{
  int compile;
  int dump_ast;
  const char *outfile;
  const char *infile;
} Pyc_Options;

static Pyc_Options parse_options(int argc, char **argv)
{
  Pyc_Options options = {0};

  int option;
  while ((option = getopt(argc, argv, "cdho:")) != -1)
  {
    switch (option)
    {
    case 'o':
      options.outfile = optarg;
      break;
    case 'd':
      options.dump_ast = 1;
      break;
    case 'c':
      options.compile = 1;
      break;
    case 'h':
      usage(argv[0]);
      exit(EXIT_SUCCESS);
    default:
      usage(argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (optind >= argc)
  {
    fprintf(stderr, "Argument error: missing input file name.\n");
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  options.infile = argv[optind];

  return options;
}

const char *pyc_read_all(Pyc_Arena *arena, const char *name)
{
  FILE *file = fopen(name, "r");

  if (file == NULL)
  {
    fprintf(stderr, "failed to open file: %s\n", name);
    exit(EXIT_FAILURE);
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char *text = pyc_arena_alloc(arena, size + 1);
  fread(text, 1, size, file);
  fclose(file);

  return text;
}

int main(int argc, char **argv)
{
  Pyc_Arena arena = {0};
  pyc_arena_init(&arena, 1024 * 1024);

  Pyc_Options options = parse_options(argc, argv);
  const char *text = pyc_read_all(&arena, options.infile);

  Pyc_Ast *ast = pyc_parse(&arena, text);

  if (options.dump_ast)
    pyc_ast_dump(&arena, stdout, ast);

  pyc_arena_free(&arena);

  return 0;
}
