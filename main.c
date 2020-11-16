#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum Node_type
{
    BRANCH1,
    BRANCH1_SYM,
    BRANCH2,
} Node_type;

typedef struct Node
{
    Node_type type;
    Node *branch[2];
    char symbol;
} Node;

Node *node_alloc(Node_type type)
{
    Node *result = calloc(sizeof(Node), 1);
    result->type = type;

    return (result);
}

void dfa(const char *pattern)
{
    char c = *pattern++;

    Node *prev = 0;

    while (c != 0)
    {
        if (c == '.')
        {
            Node *n0 = node_alloc(BRANCH2);
            Node *n1 = node_alloc(BRANCH1);
            Node *n2 = node_alloc(BRANCH1);

            n0->branch[0] = n1;
            n0->branch[1] = n2;

            n1->branch[0] = n2;

            if (prev)
            {
                prev->branch[0] = n0;
            }
            prev = n2;
        }
        else
        {
            Node *n0 = node_alloc(BRANCH1_SYM);
            Node *n1 = node_alloc(BRANCH1);

            n0->symbol = c;
            n0->branch[0] = n1;

            if (prev)
            {
                prev->branch[0] = n0;
            }
            prev = n1;
        }

        c = *pattern++;
    }
}

int main(void)
{
    char *pattern = "a.b.c";

    Node *start = node_alloc(BRANCH1);
    Node *end = node_alloc(BRANCH1);

    dfa(pattern);

    //dfa_simulate(start, end, "abzc");

    return (0);
}
