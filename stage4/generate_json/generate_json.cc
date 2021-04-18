/*
 *  matiec - a compiler for the programming languages defined in IEC 61131-3
 *
 *  Copyright (C) 2021  Mario de Sousa (msousa@fe.up.pt)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * This code is made available on the understanding that it will not be
 * used in safety-critical situations without a full and competent review.
 */

/*
 * An IEC 61131-3 compiler.
 *
 * Based on the
 * FINAL DRAFT - IEC 61131-3, 2nd Ed. (2001-12-10)
 *
 */


/*
 * Code to be included into the code generated by the 4th stage.
 *
 * This is part of the 4th stage that generates
 * a summary of the call graph in JSON format.
 * This is file is to be used by ...
 */





// #include <stdio.h>  /* required for NULL */
#include <string>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include "generate_json.hh"

#include "../stage4.hh"
#include "../../main.hh" // required for ERROR() and ERROR_MSG() macros.

#include "../../absyntax_utils/absyntax_utils.hh"

/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/

/* Global variables containing user defined options */
static stage4out_c *_s4o = NULL;


/* Parse command line options passed from main.c !! */

int  stage4_parse_options(char *options) {return 0;}

void stage4_print_options(void) {
  printf("          (no options available when generating JSON summary of 61131-3 code)\n"); 
}




/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/


class print_datatype_c: public iterator_visitor_c {
  private:
    stage4out_c &s4o;
    static print_datatype_c *print_datatype; // singleton


  public:
    print_datatype_c(stage4out_c *s4o_ptr): s4o(*s4o_ptr) {}
    ~print_datatype_c(void) {}
    
    static print_datatype_c *get_singleton(void) {
        if (nullptr == print_datatype)
          print_datatype = new print_datatype_c(_s4o);
        return print_datatype;
    }


  private:

    
void *print_token(symbol_c *token) {
  return s4o.print(token->token->value);
}

/*******************************************/
/* B 1.1 - Letters, digits and identifiers */
/*******************************************/
void *visit(                 identifier_c *symbol) {return print_token(symbol);}
void *visit(derived_datatype_identifier_c *symbol) {return print_token(symbol);}
void *visit(         poutype_identifier_c *symbol) {return print_token(symbol);}


/**********************/
/* B.1.3 - Data types */
/**********************/

/***********************************/
/* B 1.3.1 - Elementary Data Types */
/***********************************/
void *visit(       time_type_name_c *symbol) {s4o.print("TIME");        return NULL;}
void *visit(       bool_type_name_c *symbol) {s4o.print("BOOL");        return NULL;}
void *visit(       sint_type_name_c *symbol) {s4o.print("SINT");        return NULL;}
void *visit(        int_type_name_c *symbol) {s4o.print("INT");         return NULL;}
void *visit(       dint_type_name_c *symbol) {s4o.print("DINT");        return NULL;}
void *visit(       lint_type_name_c *symbol) {s4o.print("LINT");        return NULL;}
void *visit(      usint_type_name_c *symbol) {s4o.print("USINT");       return NULL;}
void *visit(       uint_type_name_c *symbol) {s4o.print("UINT");        return NULL;}
void *visit(      udint_type_name_c *symbol) {s4o.print("UDINT");       return NULL;}
void *visit(      ulint_type_name_c *symbol) {s4o.print("ULINT");       return NULL;}
void *visit(       real_type_name_c *symbol) {s4o.print("REAL");        return NULL;}
void *visit(      lreal_type_name_c *symbol) {s4o.print("LREAL");       return NULL;}
void *visit(       date_type_name_c *symbol) {s4o.print("DATE");        return NULL;}
void *visit(        tod_type_name_c *symbol) {s4o.print("TOD");         return NULL;}
void *visit(         dt_type_name_c *symbol) {s4o.print("DT");          return NULL;}
void *visit(       byte_type_name_c *symbol) {s4o.print("BYTE");        return NULL;}
void *visit(       word_type_name_c *symbol) {s4o.print("WORD");        return NULL;}
void *visit(      lword_type_name_c *symbol) {s4o.print("LWORD");       return NULL;}
void *visit(      dword_type_name_c *symbol) {s4o.print("DWORD");       return NULL;}
void *visit(     string_type_name_c *symbol) {s4o.print("STRING");      return NULL;}
void *visit(    wstring_type_name_c *symbol) {s4o.print("WSTRING");     return NULL;}

void *visit(       void_type_name_c *symbol) {s4o.print("VOID");        return NULL;} /* a non-standard extension! */

void *visit(   safetime_type_name_c *symbol) {s4o.print("SAFETIME");    return NULL;}
void *visit(   safebool_type_name_c *symbol) {s4o.print("SAFEBOOL");    return NULL;}
void *visit(   safesint_type_name_c *symbol) {s4o.print("SAFESINT");    return NULL;}
void *visit(    safeint_type_name_c *symbol) {s4o.print("SAFEINT");     return NULL;}
void *visit(   safedint_type_name_c *symbol) {s4o.print("SAFEDINT");    return NULL;}
void *visit(   safelint_type_name_c *symbol) {s4o.print("SAFELINT");    return NULL;}
void *visit(  safeusint_type_name_c *symbol) {s4o.print("SAFEUSINT");   return NULL;}
void *visit(   safeuint_type_name_c *symbol) {s4o.print("SAFEUINT");    return NULL;}
void *visit(  safeudint_type_name_c *symbol) {s4o.print("SAFEUDINT");   return NULL;}
void *visit(  safeulint_type_name_c *symbol) {s4o.print("SAFEULINT");   return NULL;}
void *visit(   safereal_type_name_c *symbol) {s4o.print("SAFEREAL");    return NULL;}
void *visit(  safelreal_type_name_c *symbol) {s4o.print("SAFELREAL");   return NULL;}
void *visit(   safedate_type_name_c *symbol) {s4o.print("SAFEDATE");    return NULL;}
void *visit(    safetod_type_name_c *symbol) {s4o.print("SAFETOD");     return NULL;}
void *visit(     safedt_type_name_c *symbol) {s4o.print("SAFEDT");      return NULL;}
void *visit(   safebyte_type_name_c *symbol) {s4o.print("SAFEBYTE");    return NULL;}
void *visit(   safeword_type_name_c *symbol) {s4o.print("SAFEWORD");    return NULL;}
void *visit(  safelword_type_name_c *symbol) {s4o.print("SAFELWORD");   return NULL;}
void *visit(  safedword_type_name_c *symbol) {s4o.print("SAFEDWORD");   return NULL;}
void *visit( safestring_type_name_c *symbol) {s4o.print("SAFESTRING");  return NULL;}
void *visit(safewstring_type_name_c *symbol) {s4o.print("SAFEWSTRING"); return NULL;}

/********************************/
/* B.1.3.2 - Generic data types */
/********************************/
void *visit(generic_type_any_c      *symbol) {s4o.print("ANY");         return NULL;}

/********************************/
/* B 1.3.3 - Derived data types */
/********************************/

/*  simple_type_name ':' simple_spec_init */
void *visit(simple_type_declaration_c *symbol) {
  return print_token(symbol->simple_type_name);
}

/*  subrange_type_name ':' subrange_spec_init */
void *visit(subrange_type_declaration_c *symbol) {
  return print_token(symbol->subrange_type_name);
}


/*  enumerated_type_name ':' enumerated_spec_init */
void *visit(enumerated_type_declaration_c *symbol) {
  return print_token(symbol->enumerated_type_name);
}


/*  identifier ':' array_spec_init */
void *visit(array_type_declaration_c *symbol) {
  return print_token(symbol->identifier);
}


/*  structure_type_name ':' structure_specification */
void *visit(structure_type_declaration_c *symbol) {
  return print_token(symbol->structure_type_name);
}


/*  string_type_name ':' elementary_string_type_name string_type_declaration_size string_type_declaration_init */
void *visit(string_type_declaration_c *symbol) {
  return print_token(symbol->string_type_name);
}

/*  function_block_type_name ASSIGN structure_initialization */
/* structure_initialization -> may be NULL ! */
void *visit(fb_spec_init_c *symbol) {
  // NOTE : This visitor should not be necessary as this symbol does not declare a new datatype,
  //        This symbol should only reference a Function Block declared somewhere else.
  //        If we always use the symbol->datatype annotation created in stage3 to print the datatypes
  //        this symbol will should never get called.  
  return print_token(symbol->function_block_type_name);
}



/* ref_spec:  REF_TO (non_generic_type_name | function_block_type_name) */
// SYM_REF1(ref_spec_c, type_name)
void *visit(ref_spec_c *symbol) {
  // NOTE: We must include this symbol, as REF_TO XXXX may be used anonymously (i.e. outside a datatype declaration)
  //       which means this symbol may be used as a datatype (i.e. may be pointed to by a symbol->datatype annotation)   
  
  // I don't really know the format JSON expects for pointers/references so leave it out for now
  // Nevertheless, REF_TO is only defined in version 3 of IEC 61131-3, so very little code is using it.  
  
  /*  
  s4o.print("REF_TO ");
  symbol->type_name->accept(*this);
  */
  return NULL;
}



/* ref_type_decl: identifier ':' ref_spec_init */
// SYM_REF2(ref_type_decl_c, ref_type_name, ref_spec_init)
void *visit(ref_type_decl_c *symbol) {
  return print_token(symbol->ref_type_name);
}




}; // class print_datatype_c



print_datatype_c *print_datatype_c::print_datatype = NULL; 





/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/






class generate_operation_c: public iterator_visitor_c {
//class generate_operation_c: public visitor_c {
  private:
    stage4out_c       &s4o;

    
void *print_token(symbol_c *token) {
  return s4o.print(token->token->value);
}


void *print_datatype(symbol_c *datatype) {
  return datatype->accept(*(print_datatype_c::get_singleton()));
}



void *print_inputargs(symbol_c *pou_decl) {
  function_param_iterator_c param_iter(pou_decl);
  int count = 0;
  
  while (param_iter.next() != nullptr) {
    if (param_iter.is_extensible_param     ()) break;
    if (param_iter.is_en_eno_param_implicit()) continue;
    
    function_param_iterator_c::param_direction_t parmdir;
    parmdir = param_iter.param_direction();
    if   ((parmdir != function_param_iterator_c::direction_out) 
       && (parmdir != function_param_iterator_c::direction_inout))
      continue;
    
    if (count != 0)
      s4o.print(",\n");
      
    s4o.print(s4o.indent_spaces + "\"");
    print_datatype(param_iter.param_type());
    s4o.print("\"");
    count++;
  }
  if (count != 0)
    s4o.print(",\n");
  return NULL;
}


    
void *print_operation(symbol_c *name,
                      symbol_c *returns,
                      symbol_c *args
                     ) {
  s4o.print(s4o.indent_spaces + "{\n");
  s4o.indent_right();

    s4o.print(s4o.indent_spaces + "\"operation\": {\n");
    s4o.indent_right();

      // "name": "<string>",
      s4o.print(s4o.indent_spaces + "\"name\": \"");
      print_token(name);
      s4o.print("\",\n");
    
      // "returns": "<datatype>",
      if (nullptr != returns) {
        s4o.print(s4o.indent_spaces + "\"returns\": \"");
        print_datatype(returns);
        s4o.print("\",\n");
      }
  
      // "args": [
      //    "<datatype>"
      //  ],
      s4o.print(s4o.indent_spaces + "\"args\": [\n");
      s4o.indent_right();
      //args->accept(*(print_inputargs_c::get_singleton()));
      print_inputargs(name->parent);
      s4o.indent_left();    
      s4o.print(s4o.indent_spaces + "],\n");

      // "creates": [
      //    "<datatype>"
      //  ],
      s4o.print(s4o.indent_spaces + "\"creates\": [\n");
      s4o.indent_right();
      // visit...
      s4o.indent_left();    
      s4o.print(s4o.indent_spaces + "],\n");

      // "destroys": [
      //    "<datatype>"
      //  ],
      s4o.print(s4o.indent_spaces + "\"destroys\": [\n");
      s4o.indent_right();
      // visit...
      s4o.indent_left();    
      s4o.print(s4o.indent_spaces + "],\n");
      
      // "calls": [
      //    "<operation>"
      //  ],
      s4o.print(s4o.indent_spaces + "\"calls\": [\n");
      s4o.indent_right();
      // visit...
      s4o.indent_left();    
      s4o.print(s4o.indent_spaces + "],\n");
      
      // "uses": [
      //    "<data>"
      //  ],
      s4o.print(s4o.indent_spaces + "\"uses\": [\n");
      s4o.indent_right();
      // visit...
      s4o.indent_left();    
      s4o.print(s4o.indent_spaces + "],\n");
      
      // "hints": {
      //    XXX
      //  },
      s4o.print(s4o.indent_spaces + "\"hints\": {\n");
      s4o.indent_right();
      // visit...
      s4o.indent_left();    
      s4o.print(s4o.indent_spaces + "},\n");
      
    s4o.indent_left();    
    s4o.print(s4o.indent_spaces + "}\n");  // "operation":

  s4o.indent_left();    
  s4o.print(s4o.indent_spaces + "}\n");
  
  return NULL;
}
    
    
  public:
    generate_operation_c(stage4out_c *s4o_ptr): s4o(*s4o_ptr) {}
    ~generate_operation_c(void) {}

    
/********************/
/* 2.1.6 - Pragmas  */
/********************/
void *visit( enable_code_generation_pragma_c * symbol)  {s4o. enable_output(); return NULL;}
void *visit(disable_code_generation_pragma_c * symbol)  {s4o.disable_output(); return NULL;} 
    
    

/***********************/
/* B 1.5.1 - Functions */
/***********************/
void *visit(function_declaration_c *symbol) {
  return print_operation(symbol->derived_function_name, symbol->type_name, symbol->var_declarations_list);
}


/*****************************/
/* B 1.5.2 - Function Blocks */
/*****************************/
/*  FUNCTION_BLOCK derived_function_block_name io_OR_other_var_declarations function_block_body END_FUNCTION_BLOCK */
void *visit(function_block_declaration_c *symbol) {
  return print_operation(symbol->fblock_name, NULL, symbol->var_declarations);
}


/**********************/
/* B 1.5.3 - Programs */
/**********************/
/*  PROGRAM program_type_name program_var_declarations_list function_block_body END_PROGRAM */
void *visit(program_declaration_c *symbol) {
  return print_operation(symbol->program_type_name, NULL, symbol->var_declarations);
}




}; /* class generate_operation_c */



/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/


class generate_json_c: public null_visitor_c {
  private:
    stage4out_c &s4o;


  public:
    generate_json_c(stage4out_c *s4o_ptr): s4o(*s4o_ptr) {}
    ~generate_json_c(void) {}



/***************************/
/* B 0 - Programming Model */
/***************************/
void *visit(library_c *symbol) {
  generate_operation_c generate_operation(&s4o);
  
  s4o.print(s4o.indent_spaces + "[\n");
  s4o.indent_right();
  symbol->accept(generate_operation);
  s4o.indent_left();    
  s4o.print(s4o.indent_spaces + "]\n");
  
  return NULL;
}


}; /* class generate_json_c */



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
    return new generate_json_c(s4o);
}
void delete_code_generator(visitor_c *code_generator) {delete code_generator;}















