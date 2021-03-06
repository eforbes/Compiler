/*
 * parsers.h
 *
 *  Created on: Sep 26, 2015
 *      Author: Evan
 */
#include "symbol_table.h"

#ifndef PARSERS_H_
#define PARSERS_H_

#ifndef NULL
#define NULL   ((void *) 0)
#endif

struct ParamCheckNode {
	SymbolTableNode *this;
	int param_total;
	int param_count;
	struct ParamCheckNode *next;
};

typedef struct ParamCheckNode ParamCheckNode;

ParamCheckNode *param_check_node_new(SymbolTableNode *this_in, int param_total_in, int param_count_in, ParamCheckNode *next_in);



void p_prog();
void p_prog_t();
void p_prog_tt();
void p_idlst();
void p_idlst_t();
void p_decls();
void p_decls_t();
int p_type();
int p_stdtype();
void p_subprogdecls();
void p_subprogdecls_t();
void p_subprogdecl();
void p_subprogdecl_t();
void p_subprogdecl_tt();
void p_subproghead();
int p_subproghead_t();
void p_args();
void p_paramlst();
void p_paramlst_t();
void p_cmpndstmt();
void p_cmpndstmt_t();
void p_optstmts();
void p_stmtlst();
void p_stmtlst_t();
void p_stmt();
void p_stmt_t();
int p_variable();
int p_variable_t();
void p_exprlst();
void p_exprlst_t();
int p_expr();
int p_expr_t();
int p_smplexpr();
int p_smplexpr_t();
int p_term();
int p_term_t();
int p_factor();
int p_factor_t();
void p_sign();

#endif /* PARSERS_H_ */
