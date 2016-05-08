#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"
#include "rexp.h"
#include "strutils.h"


struct bstnode {
    char *item;
    int value;
    struct bstnode *left;
    struct bstnode *right;
};

struct dictionary {
    struct bstnode *root;
};
// INTEGRITY STATEMENT (modify if necessary)
// I received help from the following sources:
// None. I am the sole author of this work 

// sign this statement by removing the line below and entering your name

// Name: Aditya Maru
// login ID: a2maru

// implementation:

int index = 0;
int const_index = 0;

static struct enode *string_to_enode(const char *s)
{
    bool x = false;
    bool *negative = &x;
    int length = strlen(s);
    int number_holder = 0;
    struct enode *new_enode = malloc(sizeof(struct enode));
    while(s[index] != '\0')
    {
        if(s[index] == ')' || s[index] == '(' || s[index] == ' ')
        {
            ++index;
        }
        else if((s[index] == '+' || s[index] == '-' || s[index] == '*'|| 
                 s[index] == '/') 
                && (s[index + 1] < '0' || s[index + 1] > '9'))
        {
            new_enode->type = s[index];
            ++index;
            new_enode->left = string_to_enode(s);
            ++index;
            new_enode->right = string_to_enode(s);
            break;
        }
        else if((s[index] >= '0' && s[index] <= '9') 
                || ((s[index] == '-') &&
                    (s[index + 1] >= '0' && s[index + 1] <= '9')))
        {
            if((s[index] == '-') && 
               ((s[index + 1] >= '0' && s[index + 1] <= '9')))
            {
                ++index;
                *negative = true;
            }
            sscanf(s + index, "%d", &number_holder);
            new_enode->type = 'n';
            if(*negative)
            {
                new_enode->number = -number_holder;
            }
            else
            {
                new_enode->number = number_holder;
            }
            new_enode->left = NULL;
            new_enode->right = NULL; 
            char x = s[index];
            while((x != ' ') && (x != '\0') && (x != ')'))
            {
                ++index;
                x = s[index];
            }
            break;
        }
        else
        {
            int i = 0;
            new_enode->type = 'v';
            while(*(s + index) != ' ' && *(s + index) != '\0' && *(s + index) != ')')
            { 
                (new_enode->id)[i] = *(s + index);
                ++i;
                ++index;
            }
            new_enode->id[i] = '\0';
            new_enode->left = NULL;
            new_enode->right = NULL;
            break;
        }
    }
    if(index == length - 1)
    {
        index = 0;
    }
    else
    {
        while(s[index] == ')')
        {
            ++index;
        }
        if(index == length)
        {
            index = 0;
        }
    }
    return new_enode;
}





struct rexp *string_to_rexp(const char *s) 
{ 
    struct rexp *new_rexp = malloc(sizeof(struct rexp));
    new_rexp->root = string_to_enode(s);
    return new_rexp;
}



static int rexp_eval_helper(const struct enode *curnode, 
                            const struct dictionary *constants)
{
    if(curnode->type == 'n')
    {
        return curnode->number;
    }
    else if(curnode->type == 'v')
    {
        int x = dict_lookup(curnode->id, constants);
        return x;
    }
    else
    {
        if(curnode->type == '+')
        {
            return (rexp_eval_helper(curnode->left, constants)
                    + rexp_eval_helper(curnode->right, constants));
        }
        else if(curnode->type == '*')
        {
            return (rexp_eval_helper(curnode->left, constants) 
                    * rexp_eval_helper(curnode->right, constants));
        }
        else if(curnode->type == '/')
        {
            return (rexp_eval_helper(curnode->left, constants) 
                    / rexp_eval_helper(curnode->right, constants));
        }
        else
        {
            return (rexp_eval_helper(curnode->left, constants)
                    - rexp_eval_helper(curnode->right, constants));
        }
    }
}


int rexp_eval(const struct rexp *r, const struct dictionary *constants) 
{
    assert(r != NULL);
    struct enode *curnode = r->root;
    return rexp_eval_helper(curnode, constants);
}




static void destroy_helper(struct enode *x)
{
    if(x)
    {
        destroy_helper(x->left);
        destroy_helper(x->right);
        free(x);
    }
    else
    {
        return;
    }
}


void rexp_destroy(struct rexp *r) 
{
    if(r->root)
    {
        destroy_helper(r->root);
        free(r);
    }
    else
    {
        return;
    }
}


void add_constant(const char *s, struct dictionary *constants) 
{
    char string_1[7];
    char *name = malloc(sizeof(char) * 21);
    int value = 0;
    while(s[const_index])
    {
        if(s[const_index] == '(')
        {
            ++const_index;
        }
        else
        {
            sscanf(s + const_index, "%s", string_1);
            while(s[const_index] != ' ')
            {
                ++const_index;
            }
            ++const_index;
            sscanf(s + const_index, "%s", name);
            int length = strlen(name);
            name = realloc(name, (sizeof(char) * (length + 1)));
            while(s[const_index] != ' ')
            {
                ++const_index;
            }
            ++const_index;
            char *exp = my_strdup(s + const_index);
            struct rexp *new = string_to_rexp(exp);
            value = rexp_eval(new, constants);
            dict_add(name, value, constants);
            rexp_destroy(new);
            free(exp);
            free(name);
            index = 0;
            const_index = 0;
            break;
        }

    }

}


///////////////////////////////////////////////////////////////////////
// do NOT modify these functions: they are used for marmoset testing //
// (you may find them useful for your own testing)                   //
///////////////////////////////////////////////////////////////////////

void print_enode(const struct enode *e) {
    assert(e);
    if (e->type == 'n') {
        printf("%d", e->number);
    } else if (e->type == 'v') {
        printf("%s", e->id);
    } else {
        printf("(%c ", e->type);
        print_enode(e->left);
        printf(" ");
        print_enode(e->right);
        printf(")");
    }
}

void print_rexp(const struct rexp *r) {
    assert(r);
    print_enode(r->root);
    printf("\n");
}
