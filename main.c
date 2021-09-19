#include "stdio.h"
#include "stdlib.h"

#define DEFAULT_NUM_CELLS 30000
#define DEFAULT_CELL_MAX 255
#define MAX_PROGRAM_LENGTH 1000

struct machine
{
  int* cells;
  int num_cells;
  int cell_max;
  int index;
};

struct machine* create_machine(int num_cells, int cell_max)
{
  struct machine* machine = malloc(sizeof(struct machine));
  machine->cells = malloc(num_cells*sizeof(int));
  memset(machine->cells, 0, num_cells*sizeof(int));
  machine->num_cells = num_cells;
  machine->cell_max = cell_max;
  machine->index = 0;
  return machine;
}

void destroy_machine(struct machine* m)
{
  free(m->cells);
  free(m);
}

void run_program(struct machine* m, const char* program)
{
  for (int i = 0; i < strlen(program); i++)
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
        fflush(stdout);
        break;
      default:
        break;
    }
  }
}

// Interpreter for the Brainf**k programming language.
// Can potentially be passed two command line arguments:
// 1) The number of cells (default is 30000)
// 2) The maximum value of a cell (default 1 byte, so 255)
int main(int argc, char** argv)
{
  int num_cells = DEFAULT_NUM_CELLS;
  int cell_max = DEFAULT_CELL_MAX;
  if (argc > 1)
  {
    num_cells = atoi(argv[1]);
    if (argc > 2)
    {
      cell_max = atoi(argv[2]);
    }
  }

  struct machine* m;
  char program[MAX_PROGRAM_LENGTH];
  while (fgets(program, MAX_PROGRAM_LENGTH, stdin))
  {
    m = create_machine(num_cells, cell_max);
    run_program(m, program);
    destroy_machine(m);
  }

  return 0;
}
