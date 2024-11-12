#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles
// Uma pessoa tem dois pais e dois alélos
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
}
person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
    // Seed random number generator
    // Semente de geração randomica
    srand(time(0));

    // Create a new family with three generations
    // cria uma nova familia de três gerações
    person *p = create_family(GENERATIONS);

    // Print family tree of blood types
    //printa o tipo sanguineo da familia
    print_family(p, 0);

    // Free memory
    //libera memória
    free_family(p);
}

// Create a new individual with `generations`
//cria um novo indivíduo com 'generations'
person *create_family(int generations)
{
    // TODO: Allocate memory for new person
    // TODO: Aloca memória para uma nova pessoa
    person *p = malloc(sizeof(person));

    // If there are still generations left to create
    // Se ainda restam gerações para criar
    if (generations > 1)
    {
        // Create two new parents for current person by recursively calling create_family
        // Cria dois novos pais para a pessoa atual chamando recursivamente create_family
        person *parent0 = create_family(generations - 1);
        person *parent1 = create_family(generations - 1);

        // TODO: Set parent pointers for current person
        // TODO: Define ponteiros para a pessoa atual
        p->parents[0] = parent0;
        p->parents[1] = parent1;

        // TODO: Randomly assign current person's alleles based on the alleles of their parents
        // TODO: Atribua aleatoriamente os alelos da pessoa atual com base nos alelos de seus pais
        p->alleles[0] = p->parents[0]->alleles[rand()%2];
        p->alleles[1] = p->parents[1]->alleles[rand()%2];
    }

    // If there are no generations left to create
    // Se não sobrarem gerações para criar

    else
    {
        // TODO: Set parent pointers to NULL
        // TODO: Definir ponteiros para NULL
        p->parents[0] = NULL;
        p->parents[1] = NULL;

        // TODO: Randomly assign alleles
        // TODO: atribuir alelos aleatoriamente
        p->alleles[0] = random_allele();
        p->alleles[1] = random_allele();
    }

    // TODO: Return newly created person
    // TODO: Retornar a pessoa recém-criada
    return p;
}

// Free `p` and all ancestors of `p`.
//`p` livre para todos os ancestrais de `p`.
void free_family(person *p)
{
    // TODO: Handle base case
    // TODO: Lidar com caixa base
    if (p == NULL)
    {
        return;
    }

    // TODO: Free parents recursively
    // TODO: Pais livres recursivamente
    free_family(p->parents[0]);
    free_family(p->parents[1]);

    // TODO: Free child
    // TODO: criança livre
    free(p);
}

// Print each family member and their alleles.
// Imprima cada membro da família e seus alelos.
void print_family(person *p, int generation)
{
    // Handle base case
    // Lidar com a base
    if (p == NULL)
    {
        return;
    }

    // Print indentation
    // Printa a indentação
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // Print person
    // printa pessoa
    if (generation == 0)
    {
        printf("Child (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else if (generation == 1)
    {
        printf("Parent (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }
    else
    {
        for (int i = 0; i < generation - 2; i++)
        {
            printf("Great-");
        }
        printf("Grandparent (Generation %i): blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    }

    // Print parents of current generation
    // Printa os pais da geração atual
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// Randomly chooses a blood type allele.
// De forma aleatória escolhe o tipo sanguineo
char random_allele()
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}
