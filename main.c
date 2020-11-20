#include <stdio.h>
#include <string.h>
#include <assert.h>
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
    struct Node *branch[2];
    char symbol;

    int backtrack_count;
} Node;

Node *node_alloc(Node_type type)
{
    Node *result = calloc(sizeof(Node), 1);
    result->type = type;
    result->backtrack_count = 0;

    return (result);
}

void dfa(const char *pattern, Node **s, Node **e)
{
    char c = *pattern++;

    Node *start = 0;
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
            n0->symbol = c;

            n1->branch[0] = n2;

            if (!start)
            {
                start = n0;
            }

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

            if (!start)
            {
                start = n0;
            }

            if (prev)
            {
                prev->branch[0] = n0;
            }
            prev = n1;
        }

        c = *pattern++;
    }

    *s = start;
    *e = prev;
}

int match_regex_recursive(Node *start, Node *end, const char *str)
{
    // TODO(max): This part is wrong, need to think about it
    if (start == end && *str == 0)
    {
        return (1);
    }
    if (start == end && *str != 0)
    {
        return (0);
    }

    if ((*str == 0) && (start->type == BRANCH1_SYM))
    {
        return (0);
    }

    if (start->type == BRANCH1)
    {
        return match_regex_recursive(start->branch[0], end, str);
    }
    else if (start->type == BRANCH1_SYM)
    {
        if (start->symbol == *str)
        {
            return match_regex_recursive(start->branch[0], end, str + 1);
        }
        return (0);
    }
    else if (start->type == BRANCH2)
    {
        if (*str)
        {
            int temp = match_regex_recursive(start->branch[0], end, str + 1);
            if (temp)
            {
                return (1);
            }
        }

        return match_regex_recursive(start->branch[1], end, str);
    }
    else
    {
        assert(0);
    }


    return (0);
}

#define assert_regex_match(str, start_end) assert( match_regex_recursive( (start_end)[0], (start_end)[1], str ) )
#define assert_regex_no_match(str, start_end) assert( !match_regex_recursive( (start_end)[0], (start_end)[1], str ) )
void test_regex_matcher(void)
{
    Node *start_end[2] = {};

    dfa("abcd", &start_end[0], &start_end[1]);
    assert_regex_match("abcd", start_end);
    assert_regex_no_match("abc", start_end);
    assert_regex_no_match("abcde", start_end);
    assert_regex_no_match("", start_end);

    dfa("a.b", &start_end[0], &start_end[1]);
    assert_regex_match("ab", start_end);
    assert_regex_match("acb", start_end);
    assert_regex_match("aKb", start_end);
    assert_regex_match("a:b", start_end);
    assert_regex_match("abb", start_end);
    assert_regex_match("aab", start_end);

    assert_regex_no_match("a", start_end);
    assert_regex_no_match("adba", start_end);


    dfa("a..", &start_end[0], &start_end[1]);
    assert_regex_match("a", start_end);
    assert_regex_match("ab", start_end);
    assert_regex_match("abc", start_end);
    assert_regex_no_match("abcd", start_end);

    dfa("..a", &start_end[0], &start_end[1]);
    assert_regex_match("a", start_end);
    assert_regex_match("ba", start_end);
    assert_regex_match("cba", start_end);
}

int main(int argc, char *argv[])
{
    test_regex_matcher();

    char regex[256] = {};

    printf("Regex: ");
    scanf("%s", regex);

    Node *g[2] = {};
    dfa(regex, &g[0], &g[1]);

    for (;;)
    {
        char text[256] = {};
        printf("Text: ");
        scanf("%s", text);

        if (match_regex_recursive(g[0], g[1], text))
        {
            printf("match\n");
        }
        else
        {
            printf("doesn't match\n");
        }
    }

    return (0);
}
