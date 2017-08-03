/*
 * =====================================================================================
 *
 *       Filename:  cmd_hier.h
 *
 *    Description:  This file defines the structure for maintaining cmd hierarchy
 *
 *        Version:  1.0
 *        Created:  Thursday 03 August 2017 02:08:10  IST
 *       Revision:  1.0
 *       Compiler:  gcc
 *
 *         Author:  Er. Abhishek Sagar, Networking Developer (AS), sachinites@gmail.com
 *        Company:  Brocade Communications(Jul 2012- Mar 2016), Current : Juniper Networks(Apr 2017 - Present)
 *
 * =====================================================================================
 */

#ifndef __CMD_HIER__
#define __CMD_HIER__

#include <string.h>

/* If you modify the below Macro, pls put as many zeroes in NULL_OPTIONS expansion as well*/
#define MAX_OPTION_SIZE 5
#define NULL_OPTIONS    {0,0,0,0,0}



#define CMD_NAME_SIZE   64

typedef void* (*cmd_callback)(void *arg);

typedef enum{
    INT,
    STRING,
    IPV4,
    FLOAT,
    IPV6,
    LEAF_MAX
} leaf_type_t;

typedef void* (*leaf_type_handler)(void *arg);

typedef struct _param_t_ param_t;

typedef struct cmd{
    char cmd_name[CMD_NAME_SIZE];
    cmd_callback callback;
    param_t *options[MAX_OPTION_SIZE];
} cmd_t;

typedef struct leaf{
    leaf_type_t leaf_type;
    char value_holder[64];
    cmd_callback callback;
    param_t *options[MAX_OPTION_SIZE];
} leaf_t;

typedef enum{
    CMD,
    LEAF
} param_type_t;

typedef union _param_t{
    cmd_t *cmd;
    leaf_t *leaf;
} _param_t;

struct _param_t_{
    param_type_t param_type;
    _param_t param;
};

void
init_libcli();

/*Command Registration*/
void 
static_register_command_after_command(cmd_t *parent, cmd_t *child);
void 
static_register_command_after_leaf(leaf_t *parent, cmd_t *child);
cmd_t *
dynamic_register_command_after_command(cmd_t *parent, const char *cmd_name, cmd_callback callback);
cmd_t *
dynamic_register_command_after_leaf(leaf_t *parent, const char *cmd_name, cmd_callback callback);

/*Leaf registration*/
void
static_register_leaf_after_command(cmd_t *parent, leaf_t *child);
void
static_register_leaf_after_leaf(leaf_t *parent, leaf_t *child);
leaf_t *
dynamic_register_leaf_after_command(cmd_t *parent, leaf_type_t leaf_type, 
                                    const char *def_leaf_value, cmd_callback callback);
leaf_t *
dynamic_register_leaf_after_leaf(leaf_t *parent, leaf_type_t leaf_type,
                                 const char *def_leaf_value, cmd_callback callback);

char*
get_str_leaf_type(leaf_type_t leaf_type);

void
dump_cmd_tree();

#define MIN(a,b)    (a < b ? a : b)

#define GET_PARAM_CMD(param)    (param->param.cmd)
#define GET_PARAM_LEAF(param)   (param->param.leaf)
#define IS_PARAM_CMD(param)     (param->param_type == CMD)
#define IS_PARAM_LEAF(param)    (param->param_type == LEAF)
#define GET_LEAF_TYPE_STR(param)    (get_str_leaf_type(GET_PARAM_LEAF(param)->leaf_type))

static inline int
is_cmd_string_match(param_t *param, const char *str){
    return (strncmp(param->param.cmd->cmd_name, 
            str, 
            MIN(strlen(param->param.cmd->cmd_name), strlen(str))));        
}

static inline param_t **
get_child_array_ptr(param_t *param){
    if(IS_PARAM_CMD(param)){
        return &param->param.cmd->options[0];
    }
    else{
        return &param->param.leaf->options[0];
    }
}


#define PRINT_TABS(n)     \
do{                       \
   unsigned short _i = 0; \
   for(; _i < n; _i++)    \
       printf("  ");      \
} while(0);


#endif
