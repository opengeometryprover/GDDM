// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.hpp"


// Unqualified %code blocks.
#line 32 "parser.yy"

#include "foftodb.hpp"
#include "strs.hpp"

extern struct strsList *points;

#line 53 "parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 145 "parser.cpp"

  /// Build a parser object.
  parser::parser (Driver& drv_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yy_lac_established_ (false),
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_NUMBER: // "number"
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_NUMBER: // "number"
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_NUMBER: // "number"
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_NUMBER: // "number"
        value.move< int > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        switch (yykind)
    {
      case symbol_kind::S_IDENTIFIER: // "identifier"
#line 73 "parser.yy"
                 { yyo << yysym.value.template as < std::string > (); }
#line 329 "parser.cpp"
        break;

      case symbol_kind::S_NUMBER: // "number"
#line 73 "parser.yy"
                 { yyo << yysym.value.template as < int > (); }
#line 335 "parser.cpp"
        break;

      default:
        break;
    }
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // Discard the LAC context in case there still is one left from a
    // previous invocation.
    yy_lac_discard_ ("init");

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (drv));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        if (!yy_lac_establish_ (yyla.kind ()))
          goto yyerrlab;
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        if (!yy_lac_establish_ (yyla.kind ()))
          goto yyerrlab;

        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    yy_lac_discard_ ("shift");
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_NUMBER: // "number"
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_IDENTIFIER: // "identifier"
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // unit: fof
#line 78 "parser.yy"
          {}
#line 606 "parser.cpp"
    break;

  case 3: // fof: "fof" "(" "identifier" "," "conjecture" "," forall ":" "(" "(" antecedents ")" "=>" consequent ")" ")" "."
#line 82 "parser.yy"
    {}
#line 612 "parser.cpp"
    break;

  case 4: // forall: "!" "[" varList "]"
#line 86 "parser.yy"
    {}
#line 618 "parser.cpp"
    break;

  case 5: // antecedents: %empty
#line 89 "parser.yy"
           {}
#line 624 "parser.cpp"
    break;

  case 6: // antecedents: antecedents antecedent
#line 90 "parser.yy"
                             {}
#line 630 "parser.cpp"
    break;

  case 7: // antecedents: antecedents antecedent "&"
#line 91 "parser.yy"
                                 {}
#line 636 "parser.cpp"
    break;

  case 8: // antecedent: geocmd
#line 95 "parser.yy"
    {
	drv.antconcedent[drv.numGeoCmd] = 0;
	drv.numGeoCmd++;
    }
#line 645 "parser.cpp"
    break;

  case 9: // consequent: geocmdConsequent
#line 102 "parser.yy"
    {
	drv.antconcedent[drv.numGeoCmd] = 1;
	drv.numGeoCmd++;
    }
#line 654 "parser.cpp"
    break;

  case 10: // geocmd: circle
#line 108 "parser.yy"
           {}
#line 660 "parser.cpp"
    break;

  case 11: // geocmd: coll
#line 109 "parser.yy"
           {}
#line 666 "parser.cpp"
    break;

  case 12: // geocmd: cong
#line 110 "parser.yy"
           {}
#line 672 "parser.cpp"
    break;

  case 13: // geocmd: contri
#line 111 "parser.yy"
             {}
#line 678 "parser.cpp"
    break;

  case 14: // geocmd: cyclic
#line 112 "parser.yy"
             {}
#line 684 "parser.cpp"
    break;

  case 15: // geocmd: eqangle
#line 113 "parser.yy"
              {}
#line 690 "parser.cpp"
    break;

  case 16: // geocmd: eqratio
#line 114 "parser.yy"
              {}
#line 696 "parser.cpp"
    break;

  case 17: // geocmd: para
#line 115 "parser.yy"
           {}
#line 702 "parser.cpp"
    break;

  case 18: // geocmd: perp
#line 116 "parser.yy"
           {}
#line 708 "parser.cpp"
    break;

  case 19: // geocmd: midp
#line 117 "parser.yy"
           {}
#line 714 "parser.cpp"
    break;

  case 20: // geocmd: simtri
#line 118 "parser.yy"
             {}
#line 720 "parser.cpp"
    break;

  case 21: // circle: "circle" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 122 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "circle";
	drv.point1[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
	points = addStr(yystack_[7].value.as < std::string > (), points);
	points = addStr(yystack_[5].value.as < std::string > (), points);
	points = addStr(yystack_[3].value.as < std::string > (), points);
	points = addStr(yystack_[1].value.as < std::string > (), points);
    }
#line 736 "parser.cpp"
    break;

  case 22: // coll: "coll" "(" "identifier" "," "identifier" "," "identifier" ")"
#line 136 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "coll";
	drv.point1[drv.numGeoCmd] = yystack_[5].value.as < std::string > ();
	drv.point2[drv.numGeoCmd] = yystack_[3].value.as < std::string > ();
	drv.point3[drv.numGeoCmd] = yystack_[1].value.as < std::string > ();
	points = addStr(yystack_[5].value.as < std::string > (), points);
	points = addStr(yystack_[3].value.as < std::string > (), points);
	points = addStr(yystack_[1].value.as < std::string > (), points);
    }
#line 750 "parser.cpp"
    break;

  case 23: // cong: "cong" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 148 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "cong";
	drv.point1[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
	points = addStr(yystack_[7].value.as < std::string > (), points);
	points = addStr(yystack_[5].value.as < std::string > (), points);
	points = addStr(yystack_[3].value.as < std::string > (), points);
	points = addStr(yystack_[1].value.as < std::string > (), points);
    }
#line 766 "parser.cpp"
    break;

  case 24: // contri: "contri" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 162 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "contri";
	drv.point1[drv.numGeoCmd]=yystack_[11].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[9].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point5[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point6[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
	points = addStr(yystack_[11].value.as < std::string > (), points);
	points = addStr(yystack_[9].value.as < std::string > (), points);
	points = addStr(yystack_[7].value.as < std::string > (), points);
	points = addStr(yystack_[5].value.as < std::string > (), points);
	points = addStr(yystack_[3].value.as < std::string > (), points);
	points = addStr(yystack_[1].value.as < std::string > (), points);
    }
#line 786 "parser.cpp"
    break;

  case 25: // cyclic: "cyclic" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 180 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "cyclic";
	drv.point1[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
	points = addStr(yystack_[7].value.as < std::string > (), points);
	points = addStr(yystack_[5].value.as < std::string > (), points);
	points = addStr(yystack_[3].value.as < std::string > (), points);
	points = addStr(yystack_[1].value.as < std::string > (), points);
    }
#line 802 "parser.cpp"
    break;

  case 26: // eqangle: "eqangle" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 194 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "eqangle";
	drv.point1[drv.numGeoCmd]=yystack_[15].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[13].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[11].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[9].value.as < std::string > ();
	drv.point5[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point6[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point7[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point8[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
	points = addStr(yystack_[15].value.as < std::string > (), points);
	points = addStr(yystack_[13].value.as < std::string > (), points);
	points = addStr(yystack_[11].value.as < std::string > (), points);
	points = addStr(yystack_[9].value.as < std::string > (), points);
	points = addStr(yystack_[7].value.as < std::string > (), points);
	points = addStr(yystack_[5].value.as < std::string > (), points);
	points = addStr(yystack_[3].value.as < std::string > (), points);
	points = addStr(yystack_[1].value.as < std::string > (), points);
    }
#line 826 "parser.cpp"
    break;

  case 27: // eqratio: "eqratio" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 216 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "eqratio";
	drv.point1[drv.numGeoCmd]=yystack_[15].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[13].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[11].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[9].value.as < std::string > ();
	drv.point5[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point6[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point7[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point8[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
	points = addStr(yystack_[15].value.as < std::string > (), points);
	points = addStr(yystack_[13].value.as < std::string > (), points);
	points = addStr(yystack_[11].value.as < std::string > (), points);
	points = addStr(yystack_[9].value.as < std::string > (), points);
	points = addStr(yystack_[7].value.as < std::string > (), points);
	points = addStr(yystack_[5].value.as < std::string > (), points);
	points = addStr(yystack_[3].value.as < std::string > (), points);
	points = addStr(yystack_[1].value.as < std::string > (), points);
    }
#line 850 "parser.cpp"
    break;

  case 28: // para: "para" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 238 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "para";
	drv.point1[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
	points = addStr(yystack_[7].value.as < std::string > (), points);
	points = addStr(yystack_[5].value.as < std::string > (), points);
	points = addStr(yystack_[3].value.as < std::string > (), points);
	points = addStr(yystack_[1].value.as < std::string > (), points);
    }
#line 866 "parser.cpp"
    break;

  case 29: // perp: "perp" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 252 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "perp";
	drv.point1[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
	points = addStr(yystack_[7].value.as < std::string > (), points);
	points = addStr(yystack_[5].value.as < std::string > (), points);
	points = addStr(yystack_[3].value.as < std::string > (), points);
	points = addStr(yystack_[1].value.as < std::string > (), points);
    }
#line 882 "parser.cpp"
    break;

  case 30: // midp: "midp" "(" "identifier" "," "identifier" "," "identifier" ")"
#line 266 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "midp";
	drv.point1[drv.numGeoCmd] = yystack_[5].value.as < std::string > ();
	drv.point2[drv.numGeoCmd] = yystack_[3].value.as < std::string > ();
	drv.point3[drv.numGeoCmd] = yystack_[1].value.as < std::string > ();
	points = addStr(yystack_[5].value.as < std::string > (), points);
	points = addStr(yystack_[3].value.as < std::string > (), points);
	points = addStr(yystack_[1].value.as < std::string > (), points);
    }
#line 896 "parser.cpp"
    break;

  case 31: // simtri: "simtri" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 278 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "simtri";
	drv.point1[drv.numGeoCmd]=yystack_[11].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[9].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point5[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point6[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
	points = addStr(yystack_[11].value.as < std::string > (), points);
	points = addStr(yystack_[9].value.as < std::string > (), points);
	points = addStr(yystack_[7].value.as < std::string > (), points);
	points = addStr(yystack_[5].value.as < std::string > (), points);
	points = addStr(yystack_[3].value.as < std::string > (), points);
	points = addStr(yystack_[1].value.as < std::string > (), points);
    }
#line 916 "parser.cpp"
    break;

  case 32: // geocmdConsequent: circleConsequent
#line 295 "parser.yy"
                     {}
#line 922 "parser.cpp"
    break;

  case 33: // geocmdConsequent: collConsequent
#line 296 "parser.yy"
                     {}
#line 928 "parser.cpp"
    break;

  case 34: // geocmdConsequent: congConsequent
#line 297 "parser.yy"
                     {}
#line 934 "parser.cpp"
    break;

  case 35: // geocmdConsequent: contriConsequent
#line 298 "parser.yy"
                       {}
#line 940 "parser.cpp"
    break;

  case 36: // geocmdConsequent: cyclicConsequent
#line 299 "parser.yy"
                       {}
#line 946 "parser.cpp"
    break;

  case 37: // geocmdConsequent: eqangleConsequent
#line 300 "parser.yy"
                        {}
#line 952 "parser.cpp"
    break;

  case 38: // geocmdConsequent: eqratioConsequent
#line 301 "parser.yy"
                        {}
#line 958 "parser.cpp"
    break;

  case 39: // geocmdConsequent: paraConsequent
#line 302 "parser.yy"
                     {}
#line 964 "parser.cpp"
    break;

  case 40: // geocmdConsequent: perpConsequent
#line 303 "parser.yy"
                     {}
#line 970 "parser.cpp"
    break;

  case 41: // geocmdConsequent: midpConsequent
#line 304 "parser.yy"
                     {}
#line 976 "parser.cpp"
    break;

  case 42: // geocmdConsequent: simtriConsequent
#line 305 "parser.yy"
                       {}
#line 982 "parser.cpp"
    break;

  case 43: // circleConsequent: "circle" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 309 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "circle";
	drv.point1[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
    }
#line 994 "parser.cpp"
    break;

  case 44: // collConsequent: "coll" "(" "identifier" "," "identifier" "," "identifier" ")"
#line 319 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "coll";
	drv.point1[drv.numGeoCmd] = yystack_[5].value.as < std::string > ();
	drv.point2[drv.numGeoCmd] = yystack_[3].value.as < std::string > ();
	drv.point3[drv.numGeoCmd] = yystack_[1].value.as < std::string > ();
    }
#line 1005 "parser.cpp"
    break;

  case 45: // congConsequent: "cong" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 328 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "cong";
	drv.point1[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
    }
#line 1017 "parser.cpp"
    break;

  case 46: // contriConsequent: "contri" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 338 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "contri";
	drv.point1[drv.numGeoCmd]=yystack_[11].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[9].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point5[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point6[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
    }
#line 1031 "parser.cpp"
    break;

  case 47: // cyclicConsequent: "cyclic" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 350 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "cyclic";
	drv.point1[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
    }
#line 1043 "parser.cpp"
    break;

  case 48: // eqangleConsequent: "eqangle" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 360 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "eqangle";
	drv.point1[drv.numGeoCmd]=yystack_[15].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[13].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[11].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[9].value.as < std::string > ();
	drv.point5[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point6[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point7[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point8[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
    }
#line 1059 "parser.cpp"
    break;

  case 49: // eqratioConsequent: "eqratio" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 374 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "eqratio";
	drv.point1[drv.numGeoCmd]=yystack_[15].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[13].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[11].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[9].value.as < std::string > ();
	drv.point5[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point6[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point7[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point8[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
    }
#line 1075 "parser.cpp"
    break;

  case 50: // paraConsequent: "para" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 388 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "para";
	drv.point1[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
    }
#line 1087 "parser.cpp"
    break;

  case 51: // perpConsequent: "perp" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 398 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "perp";
	drv.point1[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
    }
#line 1099 "parser.cpp"
    break;

  case 52: // midpConsequent: "midp" "(" "identifier" "," "identifier" "," "identifier" ")"
#line 408 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "midp";
	drv.point1[drv.numGeoCmd] = yystack_[5].value.as < std::string > ();
	drv.point2[drv.numGeoCmd] = yystack_[3].value.as < std::string > ();
	drv.point3[drv.numGeoCmd] = yystack_[1].value.as < std::string > ();
    }
#line 1110 "parser.cpp"
    break;

  case 53: // simtriConsequent: "simtri" "(" "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" "," "identifier" ")"
#line 417 "parser.yy"
    {
	drv.typeGeoCmd[drv.numGeoCmd] = "simtri";
	drv.point1[drv.numGeoCmd]=yystack_[11].value.as < std::string > ();
	drv.point2[drv.numGeoCmd]=yystack_[9].value.as < std::string > ();
	drv.point3[drv.numGeoCmd]=yystack_[7].value.as < std::string > ();
	drv.point4[drv.numGeoCmd]=yystack_[5].value.as < std::string > ();
	drv.point5[drv.numGeoCmd]=yystack_[3].value.as < std::string > ();
	drv.point6[drv.numGeoCmd]=yystack_[1].value.as < std::string > ();
    }
#line 1124 "parser.cpp"
    break;

  case 54: // varList: %empty
#line 428 "parser.yy"
           {}
#line 1130 "parser.cpp"
    break;

  case 55: // varList: varList "identifier"
#line 429 "parser.yy"
                           {}
#line 1136 "parser.cpp"
    break;

  case 56: // varList: varList "identifier" ","
#line 430 "parser.yy"
                               {}
#line 1142 "parser.cpp"
    break;


#line 1146 "parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      yy_lac_discard_ ("error recovery");
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    static const char *const yy_sname[] =
    {
    "end of file", "error", "invalid token", "-", "+", "*", "/", "(", ")",
  "[", "]", ",", ".", "fof", "conjecture", "&", "!", ":", "=>", "circle",
  "coll", "cong", "contri", "cyclic", "eqangle", "eqratio", "midp", "para",
  "perp", "simtri", "identifier", "number", "$accept", "unit", "fof",
  "forall", "antecedents", "antecedent", "consequent", "geocmd", "circle",
  "coll", "cong", "contri", "cyclic", "eqangle", "eqratio", "para", "perp",
  "midp", "simtri", "geocmdConsequent", "circleConsequent",
  "collConsequent", "congConsequent", "contriConsequent",
  "cyclicConsequent", "eqangleConsequent", "eqratioConsequent",
  "paraConsequent", "perpConsequent", "midpConsequent", "simtriConsequent",
  "varList", YY_NULLPTR
    };
    return yy_sname[yysymbol];
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

#if YYDEBUG
    // Execute LAC once. We don't care if it is successful, we
    // only do it for the sake of debugging output.
    if (!yyparser_.yy_lac_established_)
      yyparser_.yy_lac_check_ (yyla_.kind ());
#endif

    for (int yyx = 0; yyx < YYNTOKENS; ++yyx)
      {
        symbol_kind_type yysym = YY_CAST (symbol_kind_type, yyx);
        if (yysym != symbol_kind::S_YYerror
            && yysym != symbol_kind::S_YYUNDEF
            && yyparser_.yy_lac_check_ (yysym))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
      }
    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }




  bool
  parser::yy_lac_check_ (symbol_kind_type yytoken) const
  {
    // Logically, the yylac_stack's lifetime is confined to this function.
    // Clear it, to get rid of potential left-overs from previous call.
    yylac_stack_.clear ();
    // Reduce until we encounter a shift and thereby accept the token.
#if YYDEBUG
    YYCDEBUG << "LAC: checking lookahead " << symbol_name (yytoken) << ':';
#endif
    std::ptrdiff_t lac_top = 0;
    while (true)
      {
        state_type top_state = (yylac_stack_.empty ()
                                ? yystack_[lac_top].state
                                : yylac_stack_.back ());
        int yyrule = yypact_[+top_state];
        if (yy_pact_value_is_default_ (yyrule)
            || (yyrule += yytoken) < 0 || yylast_ < yyrule
            || yycheck_[yyrule] != yytoken)
          {
            // Use the default action.
            yyrule = yydefact_[+top_state];
            if (yyrule == 0)
              {
                YYCDEBUG << " Err\n";
                return false;
              }
          }
        else
          {
            // Use the action from yytable.
            yyrule = yytable_[yyrule];
            if (yy_table_value_is_error_ (yyrule))
              {
                YYCDEBUG << " Err\n";
                return false;
              }
            if (0 < yyrule)
              {
                YYCDEBUG << " S" << yyrule << '\n';
                return true;
              }
            yyrule = -yyrule;
          }
        // By now we know we have to simulate a reduce.
        YYCDEBUG << " R" << yyrule - 1;
        // Pop the corresponding number of values from the stack.
        {
          std::ptrdiff_t yylen = yyr2_[yyrule];
          // First pop from the LAC stack as many tokens as possible.
          std::ptrdiff_t lac_size = std::ptrdiff_t (yylac_stack_.size ());
          if (yylen < lac_size)
            {
              yylac_stack_.resize (std::size_t (lac_size - yylen));
              yylen = 0;
            }
          else if (lac_size)
            {
              yylac_stack_.clear ();
              yylen -= lac_size;
            }
          // Only afterwards look at the main stack.
          // We simulate popping elements by incrementing lac_top.
          lac_top += yylen;
        }
        // Keep top_state in sync with the updated stack.
        top_state = (yylac_stack_.empty ()
                     ? yystack_[lac_top].state
                     : yylac_stack_.back ());
        // Push the resulting state of the reduction.
        state_type state = yy_lr_goto_state_ (top_state, yyr1_[yyrule]);
        YYCDEBUG << " G" << int (state);
        yylac_stack_.push_back (state);
      }
  }

  // Establish the initial context if no initial context currently exists.
  bool
  parser::yy_lac_establish_ (symbol_kind_type yytoken)
  {
    /* Establish the initial context for the current lookahead if no initial
       context is currently established.

       We define a context as a snapshot of the parser stacks.  We define
       the initial context for a lookahead as the context in which the
       parser initially examines that lookahead in order to select a
       syntactic action.  Thus, if the lookahead eventually proves
       syntactically unacceptable (possibly in a later context reached via a
       series of reductions), the initial context can be used to determine
       the exact set of tokens that would be syntactically acceptable in the
       lookahead's place.  Moreover, it is the context after which any
       further semantic actions would be erroneous because they would be
       determined by a syntactically unacceptable token.

       yy_lac_establish_ should be invoked when a reduction is about to be
       performed in an inconsistent state (which, for the purposes of LAC,
       includes consistent states that don't know they're consistent because
       their default reductions have been disabled).

       For parse.lac=full, the implementation of yy_lac_establish_ is as
       follows.  If no initial context is currently established for the
       current lookahead, then check if that lookahead can eventually be
       shifted if syntactic actions continue from the current context.  */
    if (yy_lac_established_)
      return true;
    else
      {
#if YYDEBUG
        YYCDEBUG << "LAC: initial context established for "
                 << symbol_name (yytoken) << '\n';
#endif
        yy_lac_established_ = true;
        return yy_lac_check_ (yytoken);
      }
  }

  // Discard any previous initial lookahead context.
  void
  parser::yy_lac_discard_ (const char* event)
  {
   /* Discard any previous initial lookahead context because of Event,
      which may be a lookahead change or an invalidation of the currently
      established initial context for the current lookahead.

      The most common example of a lookahead change is a shift.  An example
      of both cases is syntax error recovery.  That is, a syntax error
      occurs when the lookahead is syntactically erroneous for the
      currently established initial context, so error recovery manipulates
      the parser stacks to try to find a new initial context in which the
      current lookahead is syntactically acceptable.  If it fails to find
      such a context, it discards the lookahead.  */
    if (yy_lac_established_)
      {
        YYCDEBUG << "LAC: initial context discarded due to "
                 << event << '\n';
        yy_lac_established_ = false;
      }
  }


  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
         In the first two cases, it might appear that the current syntax
         error should have been detected in the previous state when
         yy_lac_check was invoked.  However, at that time, there might
         have been a different syntax error that discarded a different
         initial context during error recovery, leaving behind the
         current lookahead.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -26;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -12,    -5,     4,   -26,   -25,   -26,    -4,    -6,    -2,   -10,
       1,    18,   -26,    15,    -7,    29,   -26,    26,   -26,   -26,
      -8,    20,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,   -26,   -26,   -26,   -26,   -26,   -26,
     -26,   -26,   -26,   -26,   -26,   -26,     5,    21,    22,    23,
      24,    25,    27,    30,    31,    44,    45,    46,   -26,    49,
      52,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,
     -26,   -26,   -26,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    65,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    88,    87,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,   107,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     109,   129,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   -26,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   161,   192,   193,   194,   195,
     196,   162,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   -26,   212,   213,
     214,   215,   216,   -26,   217,   218,   219,   239,   220,   240,
     241,   242,   243,   244,   221,   245,   246,   247,   251,   252,
     250,   254,   253,   255,   257,   259,   258,   233,   -26,   238,
     248,   249,   256,   260,   -26,   261,   262,   263,   -26,   -26,
     264,   -26,   265,   266,   -26,   -26,   267,   268,   269,   270,
     272,   271,   273,   275,   277,   276,   278,   287,   288,   289,
     -26,   -26,   274,   -26,   279,   280,   -26,   -26,   281,   282,
     283,   284,   285,   290,   291,   292,   294,   298,   296,   297,
     308,   293,   295,   299,   300,   -26,   301,   302,   -26,   309,
     307,   310,   311,   313,   315,   -26,   303,   304,   -26,   305,
     306,   316,   317,   312,   314,   318,   319,   -26,   -26,   329,
     330,   -26,   -26
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     0,     0,     2,     0,     1,     0,     0,     0,     0,
       0,     0,    54,     0,     0,     0,     4,    55,     5,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     6,     8,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     9,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     3,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    22,     0,     0,
       0,     0,     0,    30,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    44,     0,
       0,     0,     0,     0,    52,     0,     0,     0,    21,    23,
       0,    25,     0,     0,    28,    29,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      43,    45,     0,    47,     0,     0,    50,    51,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    24,     0,     0,    31,     0,
       0,     0,     0,     0,     0,    46,     0,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,    26,    27,     0,
       0,    48,    49
  };

  const signed char
  parser::yypgoto_[] =
  {
     -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,
     -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,
     -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,   -26,
     -26,   -26
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,     2,     3,    11,    20,    33,    70,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    14
  };

  const short
  parser::yytable_[] =
  {
      21,     1,     4,    16,     5,     6,    10,     7,     8,     9,
      12,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    15,    17,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    13,    18,    19,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
       0,    83,    84,    85,    86,    87,    94,    88,    58,    95,
      89,    90,    96,    97,    98,    99,   100,   101,   102,   103,
     104,     0,   105,     0,    91,    92,    93,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,     0,     0,
       0,     0,     0,     0,     0,   117,   128,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   129,   140,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   163,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,     0,     0,     0,     0,     0,     0,     0,   164,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   197,
     203,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     194,   195,   196,   198,   199,   200,   201,   202,   204,   205,
     206,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   228,   234,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   229,   230,   231,   232,   233,   235,   236,   237,   238,
     239,   240,   241,   247,   242,   244,   243,   245,   248,   246,
       0,     0,     0,     0,     0,     0,   260,   261,   249,   250,
     263,   262,   264,   266,   265,   267,   251,   268,     0,   269,
     252,   253,   254,   255,   256,   257,   258,   259,   270,   271,
     272,   281,   282,   283,   273,   284,   285,   286,   287,   274,
     275,   276,   277,   278,   279,   280,   288,   295,   296,   298,
     307,   297,   308,   289,   299,   290,   300,   305,   306,   291,
     292,   293,   294,   301,   302,   303,   304,   311,   312,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   309,   310
  };

  const signed char
  parser::yycheck_[] =
  {
       8,    13,     7,    10,     0,    30,    16,    11,    14,    11,
       9,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,     7,    30,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    17,     7,    11,    18,     7,
       7,     7,     7,     7,     7,     7,     7,     7,     7,     7,
      -1,    30,    30,    30,    30,    30,     7,    30,    15,     7,
      30,    30,     7,     7,     7,     7,     7,     7,     7,     7,
       7,    -1,     8,    -1,    30,    30,    30,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    30,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    11,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    30,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    30,
      12,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,
       8,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      30,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     8,     8,
      30,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      11,    11,    11,    11,    11,    11,    11,    11,    11,     8,
       8,    11,     8,    30,    11,     8,    11,     8,    30,    11,
      -1,    -1,    -1,    -1,    -1,    -1,     8,     8,    30,    30,
       8,    11,    11,     8,    11,     8,    30,    11,    -1,    11,
      30,    30,    30,    30,    30,    30,    30,    30,    11,    11,
      11,    11,    11,    11,    30,    11,     8,    11,    11,    30,
      30,    30,    30,    30,    30,    30,     8,     8,    11,     8,
       8,    11,     8,    30,    11,    30,    11,    11,    11,    30,
      30,    30,    30,    30,    30,    30,    30,     8,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30,    30
  };

  const signed char
  parser::yystos_[] =
  {
       0,    13,    33,    34,     7,     0,    30,    11,    14,    11,
      16,    35,     9,    17,    63,     7,    10,    30,     7,    11,
      36,     8,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    37,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    18,     7,     7,     7,
       7,     7,     7,     7,     7,     7,     7,     7,    15,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      38,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    30,     7,     7,     7,     7,     7,     7,
       7,     7,     7,     7,     7,     8,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    30,     8,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      11,    11,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    12,    11,    11,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    11,    11,    11,     8,    11,    11,
      11,    11,    11,     8,    11,    11,    11,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    11,     8,    11,
      11,    11,    11,    11,     8,    11,    11,    11,     8,     8,
      11,     8,    11,    11,     8,     8,    11,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    30,    30,
       8,     8,    11,     8,    11,    11,     8,     8,    11,    11,
      11,    11,    11,    30,    30,    30,    30,    30,    30,    30,
      30,    11,    11,    11,    11,     8,    11,    11,     8,    30,
      30,    30,    30,    30,    30,     8,    11,    11,     8,    11,
      11,    30,    30,    30,    30,    11,    11,     8,     8,    30,
      30,     8,     8
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    32,    33,    34,    35,    36,    36,    36,    37,    38,
      39,    39,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    63,    63
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,    17,     4,     0,     2,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,    10,     8,    10,    14,    10,    18,    18,    10,    10,
       8,    14,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,    10,     8,    10,    14,    10,    18,    18,
      10,    10,     8,    14,     0,     2,     3
  };




#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,    78,    78,    81,    85,    89,    90,    91,    94,   101,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   121,   135,   147,   161,   179,   193,   215,   237,   251,
     265,   277,   295,   296,   297,   298,   299,   300,   301,   302,
     303,   304,   305,   308,   318,   327,   337,   349,   359,   373,
     387,   397,   407,   416,   428,   429,   430
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1888 "parser.cpp"

#line 436 "parser.yy"


void yy::parser::error(const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << '\n';
}
