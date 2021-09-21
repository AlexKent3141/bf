#include "stdio.h"
#include "stdlib.h"
#include "argparse.h"

#define DEFAULT_NUM_CELLS 30000
#define DEFAULT_CELL_MAX 256
#define MAX_PROGRAM_LENGTH 100000

struct machine
{
  int* cells;
  int num_cells;
  int cell_max;
  int index;
};

struct machine* create_machine(int num_cells, int cell_max)
{
  struct machine* m = malloc(sizeof(struct machine));
  m->cells = malloc(num_cells*sizeof(int));
  memset(m->cells, 0, num_cells*sizeof(int));
  m->num_cells = num_cells;
  m->cell_max = cell_max;
  m->index = 0;
  return m;
}

void reset_machine(struct machine* m)
{
  memset(m->cells, 0, m->num_cells*sizeof(int));
  m->index = 0;
}

void destroy_machine(struct machine* m)
{
  free(m->cells);
  free(m);
}

// Show the pointer location and the cell values either side.
void print_machine_state(struct machine* m, int range)
{
  // How many characters do we need per cell?
  int max_val = max(m->cell_max, m->num_cells);
  int num_chars = 0;
  while (max_val)
  {
    ++num_chars;
    max_val /= 10;
  }

  printf("v: ... ");

  for (int i = 0; i < 2*range + 1; i++)
  {
    int index = m->index + (i - range);
    if (index < 0) index += m->num_cells;
    if (index >= m->num_cells) index -= m->num_cells;
    printf("%*d ", num_chars, m->cells[index]);
  }

  printf("...\n");

  printf("p: ... ");

  for (int i = 0; i < 2*range + 1; i++)
  {
    int index = m->index + (i - range);
    if (index < 0) index += m->num_cells;
    if (index >= m->num_cells) index -= m->num_cells;
    printf("%*d ", num_chars, index);
  }

  printf("...\n");
  printf("%*s\n", (int)(7 + 0.5*(num_chars + 1)*(2*range + 1)), "^");
}

int run_program(
  struct machine* m,
  const char* program,
  int program_start_index)
{
  int i;
  for (i = program_start_index; i < strlen(program); i++)
  {
    char c = program[i];
    switch(c)
    {
      case '+':
        ++m->cells[m->index];
        if (m->cells[m->index] >= m->cell_max)
        {
          m->cells[m->index] = 0;
        }
        break;
      case '-':
        --m->cells[m->index];
        if (m->cells[m->index] < 0)
        {
          m->cells[m->index] = m->cell_max - 1;
        }
        break;
      case '>':
        ++m->index;
        if (m->index >= m->num_cells)
        {
          m->index = 0;
        }
        break;
      case '<':
        --m->index;
        if (m->index < 0)
        {
          m->index = m->num_cells - 1;
        }
        break;
      case '.':
        printf("%d\n", m->cells[m->index]);
        break;
      case ',':
        m->cells[m->index] = fgetc(stdin);
        break;
      case '[':
        // Start of a loop.
        // Execute this in a separate call to run program.
        if (m->cells[m->index] > 0)
        {
          i = run_program(m, program, i+1);
        }
        else
        {
          // Skip to end of loop.
          while (program[i++] != ']');
        }
        break;
      case ']':
        // If the current value is zero the loop ends.
        // Otherwise go back to the start.
        if (m->cells[m->index] == 0) return i;
        i = program_start_index - 1;
        break;
      default:
        break;
    }
  }

  return i;
}

// Interpreter for the Brainfuck programming language.
int main(int argc, char** argv)
{
  setbuf(stdout, NULL);

  int num_cells = DEFAULT_NUM_CELLS;
  int cell_max = DEFAULT_CELL_MAX;

  struct argparse_option options[] =
  {
    OPT_HELP(),
    OPT_INTEGER('n', "num_cells", &num_cells, "number of cells"),
    OPT_INTEGER('m', "cell_max", &cell_max, "maximum value of a cell"),
    OPT_END()
  };

  struct argparse argparse;
  argparse_init(&argparse, options, 0, 0);
  argparse_describe(
    &argparse,
    "Interpreter for the Brainfuck programming language.",
    0);
  argc = argparse_parse(&argparse, argc, argv);

  struct machine* m = create_machine(num_cells, cell_max);
  char line[MAX_PROGRAM_LENGTH];
  printf("#: ");
  while (fgets(line, MAX_PROGRAM_LENGTH, stdin))
  {
    switch (line[0])
    {
      case 'q': goto cleanup;
      case 'r': reset_machine(m); break;
      case 'p': print_machine_state(m, 3); break;
      default: run_program(m, line, 0);
    }

    printf("#: ");
  }

cleanup:
  destroy_machine(m);

  return 0;
}
