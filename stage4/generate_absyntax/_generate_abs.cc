
/*
 * Code to be included into the code generated by the 4th stage.
 *
 * This is part of the 4th stage that generates
 * the the absyntax tree in JSON format.
 * Used for other custom compilers
 */



static int  entry_count = 0;
static bool enable_print = true;


// #include <stdio.h>  /* required for NULL */
#include <string.h>  // strdup(), strchr()
#include <string>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include "generate_abs.hh"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "../stage4.hh"
#include "../../main.hh" // required for ERROR() and ERROR_MSG() macros.

#include "../../absyntax_utils/absyntax_utils.hh"


#define STAGE4_ERROR(symbol1, symbol2, ...)   {stage4err ("while generating syntax tree", symbol1, symbol2, __VA_ARGS__); exit(EXIT_FAILURE);}
#define STAGE4_WARNING(symbol1, symbol2, ...) {stage4warn("while generating syntax tree", symbol1, symbol2, __VA_ARGS__);}


/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/

/* Global variables containing user defined options */
static stage4out_c *_s4o = NULL;


/* Parse command line options passed from main.c !! */

int  stage4_parse_options(char *options) {return 0;}

void stage4_print_options(void) {
  printf("          (no options available when generating json syntax tree of 61131-3 code)\n"); 
}


/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/




class generate_jsontree_c: public iterator_visitor_c {

  protected:
    stage4out_c &s4o;

  private:
    /* Unlike programs that are mapped onto C++ classes, Function Blocks are mapped onto a data structure type
     * and a separate function containing the code. This function is passed a pointer to an instance of the data
     * structure. This means that the code inside the functions must insert a pointer to the data structure whenever
     * it wishes to access a Function Block variable.
     * The variable_prefix_ variable will contain the correct string which needs to be prefixed to all variable accesses.
     * This string is set with the set_variable_prefix() member function.
     */
    const char *variable_prefix_;
    
    // The root JSON object for the AST
    json root;
    // The current node in the JSON tree
    json* current_node;

    // Add a new node to the current node in the JSON tree
    void add_node(json object) {
        current_node->push_back(object);
    }
    
  public:
    generate_jsontree_c(stage4out_c *s4o_ptr, const char *variable_prefix = NULL): s4o(*s4o_ptr) {
      this->set_variable_prefix(variable_prefix);
    }
    ~generate_jsontree_c(void) {}

    void set_variable_prefix(const char *variable_prefix) {variable_prefix_ = variable_prefix;}
    const char *get_variable_prefix(void) {return variable_prefix_;}
    bool is_variable_prefix_null(void) {return variable_prefix_ == NULL;}

    // Tokens are produced by the lexical analysis phase of compilation, which is also called tokenization or lexing. 
    // Tokens are usually represented as strings or objects with properties that describe their type and value.
    void *print_token(token_c *token, const int line = 0) {
      // print_const_value(token);
      s4o.print(s4o.indent_spaces + std::to_string(line) + " token value at line: " + std::to_string(token->first_line) + " char: " + std::to_string(token->first_column) + " of file: " + (token->last_file ? token->last_file : "0")  + "\n");
      s4o.indent_right();
      s4o.print(s4o.indent_spaces); s4o.print(token->value); s4o.print("\n");
      s4o.indent_left(); 
      return NULL;
    }

    void *print_literal(symbol_c *type, symbol_c *value) {
      // print_const_value(value);
      s4o.print(s4o.indent_spaces + "literal\n");
      s4o.indent_right();
      if (NULL != type) {
        type->accept(*this);
        s4o.print(s4o.indent_spaces);
        s4o.print("#");
      } else {
        s4o.print(s4o.indent_spaces);
      }
      value->accept(*this);
      s4o.indent_left(); s4o.print("\n");
      return NULL;
    }

    // Not yet used in simple example
    void *print_binary_expression(symbol_c *symbol,
                symbol_c *l_exp,
                symbol_c *r_exp,
                const char *operation) {
      // print_const_value(symbol);
      s4o.print(s4o.indent_spaces + "binary expression at line" + " \n");
      s4o.indent_right();
      l_exp->accept(*this);
      s4o.print(operation);
      r_exp->accept(*this);
      s4o.print("\n");
      s4o.indent_left();
      return NULL;
    }

    // not yet used in simple example
    void *print_unary_expression(symbol_c *symbol,
			     symbol_c *exp,
			     const char *operation) {
      // print_const_value(symbol);
      s4o.print(s4o.indent_spaces + "unary expression at line" + std::to_string(symbol->first_line) + " char: " + std::to_string(symbol->first_column) + "\n");
      s4o.print(s4o.indent_spaces); s4o.print(operation); s4o.print("\n");
      exp->accept(*this);
      s4o.indent_left();
      return NULL;
    }

    void *print_elementary_data_types(const char* type) {
      s4o.print(s4o.indent_spaces + "elementary data type\n");
      s4o.indent_right();
      s4o.print(s4o.indent_spaces); s4o.print(type); s4o.print("\n");
      s4o.indent_left();
      return NULL;
    }
    

    /*******************************************/
    /* B 1.1 - Letters, digits and identifiers */
    /*******************************************/
    void *visit(                 identifier_c *symbol) {return print_token(symbol, __LINE__);}
    void *visit(derived_datatype_identifier_c *symbol) {return print_token(symbol, __LINE__);}
    void *visit(         poutype_identifier_c *symbol) {return print_token(symbol, __LINE__);}

    /*********************/
    /* B 1.2 - Constants */
    /*********************/
    /*********************************/
    /* B 1.2.XX - Reference Literals */
    /*********************************/
    /* defined in IEC 61131-3 v3 - Basically the 'NULL' keyword! */
    void *visit(ref_value_null_literal_c *symbol)  {s4o.print("NULL"); return NULL;}

    /******************************/
    /* B 1.2.1 - Numeric Literals */
    /******************************/
    void *visit(real_c *symbol)               {return print_token(symbol, __LINE__);}
    void *visit(neg_real_c *symbol)           {return print_unary_expression(symbol, symbol->exp, "-");}
    void *visit(integer_c *symbol)            {return print_token(symbol, __LINE__);}
    void *visit(neg_integer_c *symbol)        {return print_unary_expression(symbol, symbol->exp, "-");}
    void *visit(binary_integer_c *symbol)     {return print_token(symbol, __LINE__);}
    void *visit(octal_integer_c *symbol)      {return print_token(symbol, __LINE__);}
    void *visit(hex_integer_c *symbol)        {return print_token(symbol, __LINE__);}

    void *visit(integer_literal_c *symbol)    {return print_literal(symbol->type, symbol->value);}
    void *visit(real_literal_c *symbol)       {return print_literal(symbol->type, symbol->value);}
    void *visit(bit_string_literal_c *symbol) {return print_literal(symbol->type, symbol->value);}
    void *visit(boolean_literal_c *symbol)    {return print_literal(symbol->type, symbol->value);}

    /* helper class for boolean_literal_c */
    void *visit(boolean_true_c *symbol)       {s4o.print(/*"TRUE" */ "1"); return NULL;}
    void *visit(boolean_false_c *symbol)      {s4o.print(/*"FALSE"*/ "0"); return NULL;}

    /***********************************/
    /* B 1.3.1 - Elementary Data Types */
    /***********************************/
    void *visit(       time_type_name_c *symbol) {print_elementary_data_types("TIME");        return NULL;}
    void *visit(       bool_type_name_c *symbol) {print_elementary_data_types("BOOL");        return NULL;}
    void *visit(       sint_type_name_c *symbol) {print_elementary_data_types("SINT");        return NULL;}
    void *visit(        int_type_name_c *symbol) {print_elementary_data_types("INT");         return NULL;}
    void *visit(       dint_type_name_c *symbol) {print_elementary_data_types("DINT");        return NULL;}
    void *visit(       lint_type_name_c *symbol) {print_elementary_data_types("LINT");        return NULL;}
    void *visit(      usint_type_name_c *symbol) {print_elementary_data_types("USINT");       return NULL;}
    void *visit(       uint_type_name_c *symbol) {print_elementary_data_types("UINT");        return NULL;}
    void *visit(      udint_type_name_c *symbol) {print_elementary_data_types("UDINT");       return NULL;}
    void *visit(      ulint_type_name_c *symbol) {print_elementary_data_types("ULINT");       return NULL;}
    void *visit(       real_type_name_c *symbol) {print_elementary_data_types("REAL");        return NULL;}
    void *visit(      lreal_type_name_c *symbol) {print_elementary_data_types("LREAL");       return NULL;}
    void *visit(       date_type_name_c *symbol) {print_elementary_data_types("DATE");        return NULL;}
    void *visit(        tod_type_name_c *symbol) {print_elementary_data_types("TOD");         return NULL;}
    void *visit(         dt_type_name_c *symbol) {print_elementary_data_types("DT");          return NULL;}
    void *visit(       byte_type_name_c *symbol) {print_elementary_data_types("BYTE");        return NULL;}
    void *visit(       word_type_name_c *symbol) {print_elementary_data_types("WORD");        return NULL;}
    void *visit(      lword_type_name_c *symbol) {print_elementary_data_types("LWORD");       return NULL;}
    void *visit(      dword_type_name_c *symbol) {print_elementary_data_types("DWORD");       return NULL;}
    void *visit(     string_type_name_c *symbol) {print_elementary_data_types("STRING");      return NULL;}
    void *visit(    wstring_type_name_c *symbol) {print_elementary_data_types("WSTRING");     return NULL;}

    void *visit(       void_type_name_c *symbol) {print_elementary_data_types("VOID");        return NULL;} /* a non-standard extension! */

    void *visit(   safetime_type_name_c *symbol) {print_elementary_data_types("SAFETIME");    return NULL;}
    void *visit(   safebool_type_name_c *symbol) {print_elementary_data_types("SAFEBOOL");    return NULL;}
    void *visit(   safesint_type_name_c *symbol) {print_elementary_data_types("SAFESINT");    return NULL;}
    void *visit(    safeint_type_name_c *symbol) {print_elementary_data_types("SAFEINT");     return NULL;}
    void *visit(   safedint_type_name_c *symbol) {print_elementary_data_types("SAFEDINT");    return NULL;}
    void *visit(   safelint_type_name_c *symbol) {print_elementary_data_types("SAFELINT");    return NULL;}
    void *visit(  safeusint_type_name_c *symbol) {print_elementary_data_types("SAFEUSINT");   return NULL;}
    void *visit(   safeuint_type_name_c *symbol) {print_elementary_data_types("SAFEUINT");    return NULL;}
    void *visit(  safeudint_type_name_c *symbol) {print_elementary_data_types("SAFEUDINT");   return NULL;}
    void *visit(  safeulint_type_name_c *symbol) {print_elementary_data_types("SAFEULINT");   return NULL;}
    void *visit(   safereal_type_name_c *symbol) {print_elementary_data_types("SAFEREAL");    return NULL;}
    void *visit(  safelreal_type_name_c *symbol) {print_elementary_data_types("SAFELREAL");   return NULL;}
    void *visit(   safedate_type_name_c *symbol) {print_elementary_data_types("SAFEDATE");    return NULL;}
    void *visit(    safetod_type_name_c *symbol) {print_elementary_data_types("SAFETOD");     return NULL;}
    void *visit(     safedt_type_name_c *symbol) {print_elementary_data_types("SAFEDT");      return NULL;}
    void *visit(   safebyte_type_name_c *symbol) {print_elementary_data_types("SAFEBYTE");    return NULL;}
    void *visit(   safeword_type_name_c *symbol) {print_elementary_data_types("SAFEWORD");    return NULL;}
    void *visit(  safelword_type_name_c *symbol) {print_elementary_data_types("SAFELWORD");   return NULL;}
    void *visit(  safedword_type_name_c *symbol) {print_elementary_data_types("SAFEDWORD");   return NULL;}
    void *visit( safestring_type_name_c *symbol) {print_elementary_data_types("SAFESTRING");  return NULL;}
    void *visit(safewstring_type_name_c *symbol) {print_elementary_data_types("SAFEWSTRING"); return NULL;}

    /***********************/
    /* B 3.1 - Expressions */
    /***********************/    
    void *visit( deref_operator_c   *symbol) {return symbol->exp->accept(*this); s4o.print("^");}                      /* an extension to the IEC 61131-3 standard - based on the IEC 61131-3 v3 standard. ^      -> dereferences an address into a variable! */
    void *visit( deref_expression_c *symbol) {return  s4o.print("DREF("); symbol->exp->accept(*this); s4o.print(")");} /* an extension to the IEC 61131-3 standard - based on the IEC 61131-3 v3 standard. DREF() -> dereferences an address into a variable! */
    void *visit(   ref_expression_c *symbol) {return  s4o.print( "REF("); symbol->exp->accept(*this); s4o.print(")");} /* an extension to the IEC 61131-3 standard - based on the IEC 61131-3 v3 standard. REF()  -> returns address of the varible! */
    void *visit(    or_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " OR ");}
    void *visit(   xor_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " XOR ");}
    void *visit(   and_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " AND ");}
    void *visit(   equ_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " = ");}
    void *visit(notequ_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " <> ");}
    void *visit(    lt_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " < ");}
    void *visit(    gt_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " > ");}
    void *visit(    le_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " <= ");}
    void *visit(    ge_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " >= ");}
    void *visit(   add_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " + ");}
    void *visit(   sub_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " - ");}
    void *visit(   mul_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " * ");}
    void *visit(   div_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " / ");}
    void *visit(   mod_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " MOD ");}
    void *visit( power_expression_c *symbol) {return print_binary_expression(symbol, symbol->l_exp, symbol->r_exp, " ** ");}
    void *visit(   neg_expression_c *symbol) {return print_unary_expression (symbol, symbol->exp, "-");}
    void *visit(   not_expression_c *symbol) {return print_unary_expression (symbol, symbol->exp, "NOT ");}

    
    /*********************/
    /* B 1.4 - Variables */
    /*********************/
    void *visit(symbolic_variable_c *symbol) {return symbol->var_name->accept(*this);}
    void *visit(symbolic_constant_c *symbol) {return symbol->var_name->accept(*this);}

    /********************************************/
    /* B.1.4.1   Directly Represented Variables */
    /********************************************/
    void *visit(direct_variable_c *symbol) {return print_token(symbol, __LINE__);}

    /******************************************/
    /* B 1.4.3 - Declaration & Initialisation */
    /******************************************/
    void *visit(constant_option_c *symbol) {s4o.print("CONSTANT"); return NULL;}
    void *visit(retain_option_c *symbol) {s4o.print("RETAIN"); return NULL;}
    void *visit(non_retain_option_c *symbol) {s4o.print("NON_RETAIN"); return NULL;}

    /* VAR_INPUT [RETAIN | NON_RETAIN] input_declaration_list END_VAR */
    /* option -> the RETAIN/NON_RETAIN/<NULL> directive... */
    void *visit(input_declarations_c *symbol) {
      if (typeid(*(symbol->method)) == typeid(explicit_definition_c)) {
        s4o.print(s4o.indent_spaces); s4o.print("VAR_INPUT ");
        if (symbol->option != NULL)
          symbol->option->accept(*this);
        s4o.print("\n");
        s4o.indent_right();
        symbol->input_declaration_list->accept(*this);
        s4o.indent_left();
        s4o.print(s4o.indent_spaces); s4o.print("END_VAR\n");
      }
      return NULL;
    }

    /* helper symbol for input_declarations */
    /*| input_declaration_list input_declaration ';' */
    void *visit(input_declaration_list_c *symbol) {
      // return print_list(symbol, s4o.indent_spaces, ";\n" + s4o.indent_spaces, ";\n");
    }

    /* edge -> The F_EDGE or R_EDGE directive */
    void *visit(edge_declaration_c *symbol) {
      symbol->var1_list->accept(*this);
      s4o.print(" : BOOL ");
      symbol->edge->accept(*this);
      return NULL;
    }

    /* dummy classes only used as flags! */
    void *visit(explicit_definition_c *symbol) {return NULL;}
    void *visit(implicit_definition_c *symbol) {return NULL;}

    /* EN : BOOL := 1 */
    void *visit(en_param_declaration_c *symbol) {
      if (typeid(*(symbol->method)) == typeid(explicit_definition_c)) {
        symbol->name->accept(*this);
        s4o.print(" : ");
        symbol->type_decl->accept(*this);
      }
      return NULL;
    }

    /* ENO : BOOL */
    void *visit(eno_param_declaration_c *symbol) {
      if (typeid(*(symbol->method)) == typeid(explicit_definition_c)) {
        symbol->name->accept(*this);
        s4o.print(" : ");
        symbol->type->accept(*this);
      }
      return NULL;
    }

    void *visit(raising_edge_option_c *symbol) {
      s4o.print("R_EDGE");
      return NULL;
    }

    void *visit(falling_edge_option_c *symbol) {
      s4o.print("F_EDGE");
      return NULL;
    }


    /* var1_list is one of the following...
    *    simple_spec_init_c *
    *    subrange_spec_init_c *
    *    enumerated_spec_init_c *
    */
    void *visit(var1_init_decl_c *symbol) {
      symbol->var1_list->accept(*this);
      s4o.print(" : ");
      symbol->spec_init->accept(*this);
      return NULL;
    }


    // void *visit(var1_list_c *symbol) {return print_list(symbol, "", ", ");}

    /* | [var1_list ','] variable_name '..' */
    /* NOTE: This is an extension to the standard!!! */
    /* In order to be able to handle extensible standard functions
    * (i.e. standard functions that may have a variable number of
    * input parameters, such as AND(word#33, word#44, word#55, word#66),
    * we have extended the acceptable syntax to allow var_name '..'
    * in an input variable declaration.
    *
    * This allows us to parse the declaration of standard
    * extensible functions and load their interface definition
    * into the abstract syntax tree just like we do to other 
    * user defined functions.
    * This has the advantage that we can later do semantic
    * checking of calls to functions (be it a standard or user defined
    * function) in (almost) exactly the same way.
    *
    * Of course, we have a flag that disables this syntax when parsing user
    * written code, so we only allow this extra syntax while parsing the 
    * 'header' file that declares all the standard IEC 61131-3 functions.
    */
    void *visit(extensible_input_parameter_c *symbol) {
      symbol->var_name->accept(*this);
      s4o.print(" .. ");
      return NULL;
    }


    /* var1_list ':' array_spec_init */
    void *visit(array_var_init_decl_c *symbol) {
      symbol->var1_list->accept(*this);
      s4o.print(" : ");
      symbol->array_spec_init->accept(*this);
      return NULL;
    }


    /*  var1_list ':' initialized_structure */
    void *visit(structured_var_init_decl_c *symbol) {
      symbol->var1_list->accept(*this);
      s4o.print(" : ");
      symbol->initialized_structure->accept(*this);
      return NULL;
    }

    /* name_list ':' function_block_type_name ASSIGN structure_initialization */
    /* structure_initialization -> may be NULL ! */
    void *visit(fb_name_decl_c *symbol) {
      symbol->fb_name_list->accept(*this);
      s4o.print(" : ");
      symbol->fb_spec_init->accept(*this);
      return NULL;
    }

    /* name_list ',' fb_name */
    // void *visit(fb_name_list_c *symbol) {return print_list(symbol, "", ", ");}

    /* VAR_OUTPUT [RETAIN | NON_RETAIN] var_init_decl_list END_VAR */
    /* option -> may be NULL ! */
    void *visit(output_declarations_c *symbol) {
      if (typeid(*(symbol->method)) == typeid(explicit_definition_c)) {
        s4o.print(s4o.indent_spaces); s4o.print("VAR_OUTPUT ");
        if (symbol->option != NULL)
          symbol->option->accept(*this);
        s4o.print("\n");
        s4o.indent_right();
        symbol->var_init_decl_list->accept(*this);
        s4o.indent_left();
        s4o.print(s4o.indent_spaces); s4o.print("END_VAR\n");
      }
      return NULL;
    }

    /*  VAR_IN_OUT  END_VAR */
    void *visit(input_output_declarations_c *symbol) {
      s4o.print(s4o.indent_spaces); s4o.print("VAR_IN_OUT ");
      s4o.print("\n");
      s4o.indent_right();
      symbol->var_declaration_list->accept(*this);
      s4o.indent_left();
      s4o.print(s4o.indent_spaces); s4o.print("END_VAR\n");
      return NULL;
    }

    /* helper symbol for input_output_declarations */
    /* var_declaration_list var_declaration ';' */
    void *visit(var_declaration_list_c *symbol) {
      // return print_list(symbol, s4o.indent_spaces, ";\n" + s4o.indent_spaces, ";\n");
    }

    /*  var1_list ':' array_specification */
    void *visit(array_var_declaration_c *symbol) {
      symbol->var1_list->accept(*this);
      s4o.print(" : ");
      symbol->array_specification->accept(*this);
      return NULL;
    }

    /*  var1_list ':' structure_type_name */
    void *visit(structured_var_declaration_c *symbol) {
      symbol->var1_list->accept(*this);
      s4o.print(" : ");
      symbol->structure_type_name->accept(*this);
      return NULL;
    }

    /* VAR [CONSTANT] var_init_decl_list END_VAR */
    /* option -> may be NULL ! */
    void *visit(var_declarations_c *symbol) {
      s4o.print(s4o.indent_spaces); s4o.print("VAR ");
      if (symbol->option != NULL)
        symbol->option->accept(*this);
      s4o.print("\n");
      s4o.indent_right();
      symbol->var_init_decl_list->accept(*this);
      s4o.indent_left();
      s4o.print(s4o.indent_spaces); s4o.print("END_VAR\n");
      return NULL;
    }

    /*  VAR RETAIN var_init_decl_list END_VAR */
    void *visit(retentive_var_declarations_c *symbol) {
      s4o.print(s4o.indent_spaces); s4o.print("VAR RETAIN ");
      s4o.print("\n");
      s4o.indent_right();
      symbol->var_init_decl_list->accept(*this);
      s4o.indent_left();
      s4o.print(s4o.indent_spaces); s4o.print("END_VAR\n");
      return NULL;
    }

    /*  VAR [CONSTANT|RETAIN|NON_RETAIN] located_var_decl_list END_VAR */
    /* option -> may be NULL ! */
    void *visit(located_var_declarations_c *symbol) {
      s4o.print(s4o.indent_spaces); s4o.print("VAR ");
      if (symbol->option != NULL)
        symbol->option->accept(*this);
      s4o.print("\n");
      s4o.indent_right();
      symbol->located_var_decl_list->accept(*this);
      s4o.indent_left();
      s4o.print(s4o.indent_spaces); s4o.print("END_VAR\n");
      return NULL;
    }

    /* helper symbol for located_var_declarations */
    /* located_var_decl_list located_var_decl ';' */
    void *visit(located_var_decl_list_c *symbol) {
      // return print_list(symbol, s4o.indent_spaces, ";\n" + s4o.indent_spaces, ";\n");
    }

    /*  [variable_name] location ':' located_var_spec_init */
    /* variable_name -> may be NULL ! */
    void *visit(located_var_decl_c *symbol) {
      if (symbol->variable_name != NULL) {
        symbol->variable_name->accept(*this);
        s4o.print(" ");
      }
      symbol->location->accept(*this);
      s4o.print(" : ");
      symbol->located_var_spec_init->accept(*this);
      return NULL;
    }


    /*| VAR_EXTERNAL [CONSTANT] external_declaration_list END_VAR */
    /* option -> may be NULL ! */
    void *visit(external_var_declarations_c *symbol) {
      s4o.print(s4o.indent_spaces); s4o.print("VAR_EXTERNAL ");
      if (symbol->option != NULL)
        symbol->option->accept(*this);
      s4o.print("\n");
      s4o.indent_right();
      symbol->external_declaration_list->accept(*this);
      s4o.indent_left();
      s4o.print(s4o.indent_spaces); s4o.print("END_VAR\n");
      return NULL;
    }

    /* helper symbol for external_var_declarations */
    /*| external_declaration_list external_declaration';' */
    void *visit(external_declaration_list_c *symbol) {
      // return print_list(symbol, s4o.indent_spaces, ";\n" + s4o.indent_spaces, ";\n");
    }

    /*  global_var_name ':' (simple_specification|subrange_specification|enumerated_specification|array_specification|prev_declared_structure_type_name|function_block_type_name) */
    void *visit(external_declaration_c *symbol) {
      symbol->global_var_name->accept(*this);
      s4o.print(" : ");
      symbol->specification->accept(*this);
      return NULL;
    }

    /*| VAR_GLOBAL [CONSTANT|RETAIN] global_var_decl_list END_VAR */
    /* option -> may be NULL ! */
    void *visit(global_var_declarations_c *symbol) {
      s4o.print(s4o.indent_spaces); s4o.print("VAR_GLOBAL ");
      if (symbol->option != NULL)
        symbol->option->accept(*this);
      s4o.print("\n");
      s4o.indent_right();
      symbol->global_var_decl_list->accept(*this);
      s4o.indent_left();
      s4o.print(s4o.indent_spaces); s4o.print("END_VAR\n");
      return NULL;
    }

    /* helper symbol for global_var_declarations */
    /*| global_var_decl_list global_var_decl ';' */
    void *visit(global_var_decl_list_c *symbol) {
      // return print_list(symbol, s4o.indent_spaces, ";\n" + s4o.indent_spaces, ";\n");
    }

    /*| global_var_spec ':' [located_var_spec_init|function_block_type_name] */
    /* type_specification ->may be NULL ! */
    void *visit(global_var_decl_c *symbol) {
      symbol->global_var_spec->accept(*this);
      s4o.print(" : ");
      if (symbol->type_specification != NULL)
        symbol->type_specification->accept(*this);
      return NULL;
    }

    /*| global_var_name location */
    void *visit(global_var_spec_c *symbol) {
      symbol->global_var_name->accept(*this);
      s4o.print(" ");
      symbol->location->accept(*this);
      return NULL;
    }

    /*  AT direct_variable */
    void *visit(location_c *symbol) {
      s4o.print("AT ");
      symbol->direct_variable->accept(*this);
      return NULL;
    }

    /*| global_var_list ',' global_var_name */
    // void *visit(global_var_list_c *symbol) {return print_list(symbol, "", ", ");}

    /*  var1_list ':' single_byte_string_spec */
    void *visit(single_byte_string_var_declaration_c *symbol) {
      symbol->var1_list->accept(*this);
      s4o.print(" : ");
      symbol->single_byte_string_spec->accept(*this);
      return NULL;
    }

    /*  single_byte_limited_len_string_spec [ASSIGN single_byte_character_string] */
    /* single_byte_character_string ->may be NULL ! */
    void *visit(single_byte_string_spec_c *symbol) {
      symbol->string_spec->accept(*this);
      if (symbol->single_byte_character_string != NULL) {
        s4o.print(" := ");
        symbol->single_byte_character_string->accept(*this);
      }
      return NULL;
    }

    /*  STRING ['[' integer ']'] */
    /* integer ->may be NULL ! */
    void *visit(single_byte_limited_len_string_spec_c *symbol) {
      symbol->string_type_name->accept(*this);
      if (symbol->character_string_len != NULL) {
        s4o.print(" [");
        symbol->character_string_len->accept(*this);
        s4o.print("]");
      }
      return NULL;
    }

    /*  var1_list ':' double_byte_string_spec */
    void *visit(double_byte_string_var_declaration_c *symbol) {
      symbol->var1_list->accept(*this);
      s4o.print(" : ");
      symbol->double_byte_string_spec->accept(*this);
      return NULL;
    }

    /*  double_byte_limited_len_string_spec [ASSIGN double_byte_character_string] */
    /* integer ->may be NULL ! */
    /* double_byte_character_string ->may be NULL ! */
    void *visit(double_byte_string_spec_c *symbol) {
      symbol->string_spec->accept(*this);
      if (symbol->double_byte_character_string != NULL) {
        s4o.print(" := ");
        symbol->double_byte_character_string->accept(*this);
      }
      return NULL;
    }

    /* WSTRING ['[' integer ']'] */
    /* integer ->may be NULL ! */
    void *visit(double_byte_limited_len_string_spec_c *symbol) {
      symbol->string_type_name->accept(*this);
      if (symbol->character_string_len != NULL) {
        s4o.print(" [");
        symbol->character_string_len->accept(*this);
        s4o.print("]");
      }
      return NULL;
    }

    /*| VAR [RETAIN|NON_RETAIN] incompl_located_var_decl_list END_VAR */
    /* option ->may be NULL ! */
    void *visit(incompl_located_var_declarations_c *symbol) {
      s4o.print(s4o.indent_spaces); s4o.print("VAR ");
      if (symbol->option != NULL)
        symbol->option->accept(*this);
      s4o.print("\n");
      s4o.indent_right();
      symbol->incompl_located_var_decl_list->accept(*this);
      s4o.indent_left();
      s4o.print(s4o.indent_spaces); s4o.print("END_VAR\n");
      return NULL;
    }

    /* helper symbol for incompl_located_var_declarations */
    /*| incompl_located_var_decl_list incompl_located_var_decl ';' */
    void *visit(incompl_located_var_decl_list_c *symbol) {
      // return print_list(symbol, s4o.indent_spaces, ";\n" + s4o.indent_spaces, ";\n");
    }

    /*  variable_name incompl_location ':' var_spec */
    void *visit(incompl_located_var_decl_c *symbol) {
      symbol->variable_name->accept(*this);
      s4o.print(" ");
      symbol->incompl_location->accept(*this);
      s4o.print(" : ");
      symbol->var_spec->accept(*this);
      return NULL;
    }


    /*  AT incompl_location_token */
    void *visit(incompl_location_c *symbol) {
      s4o.print(" AT ");
      return print_token(symbol);
    }


    /* intermediate helper symbol for:
    *  - non_retentive_var_decls
    *  - output_declarations
    */
    /* | var_init_decl_list var_init_decl ';' */
    void *visit(var_init_decl_list_c *symbol) {
      // return print_list(symbol, s4o.indent_spaces, ";\n" + s4o.indent_spaces, ";\n");
    }
};


class generate_abs_c: public null_visitor_c {
  private:
    stage4out_c &s4o;


  public:
    generate_abs_c(stage4out_c *s4o_ptr): s4o(*s4o_ptr) {}
    ~generate_abs_c(void) {}



    /***************************/
    /* B 0 - Programming Model */
    /***************************/
    void *visit(library_c *symbol) {
    // generate_data_c      generate_data     (&s4o);
    // generate_operation_c generate_operation(&s4o);
    generate_jsontree_c  generate_jsontree  (&s4o);
    
    s4o.print(s4o.indent_spaces + "[\n");
    s4o.indent_right();
    //   symbol->accept(generate_data); 
      symbol->accept(generate_jsontree);
    s4o.indent_left();    
    s4o.print(s4o.indent_spaces + "]\n");
    
    return NULL;
    }


}; /* class generate_abs_c */


/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/ 




visitor_c *new_code_generator(stage4out_c *s4o, const char *builddir)  {
    _s4o = s4o;
    return new generate_abs_c(s4o);
}
void delete_code_generator(visitor_c *code_generator) {delete code_generator;}