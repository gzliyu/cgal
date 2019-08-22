//
// Copyright (c) 2012  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// SPDX-License-Identifier: GPL-3.0+
//
// Author(s): Saurabh Singh <ssingh@cs.iitr.ac.in>

#include "AlgebraicCurveParser.h"

AlgebraicCurveParser::AlgebraicCurveParser(std::string &expression) : expression(expression) {};

bool AlgebraicCurveParser::validateExpression(const std::string &expression) {
  std::string expressionMutable = expression;
  for (auto iterator = expressionMutable.begin(); iterator != expressionMutable.end(); iterator++) {
    if (*iterator == 'x' || *iterator == 'y') continue;
    if (*iterator == '+' || *iterator == '-' || *iterator == '*' || *iterator == '^') continue;
    if (isdigit(*iterator) || isspace(*iterator)) continue;
    else return false;
  }
  return true;
}

Terms AlgebraicCurveParser::extractTerms() {
  Terms algebraicTerms;

  //Preprocess the expression: trim and remove spaces
  if (!this->validateExpression(expression)) return algebraicTerms;
  this->expression.erase(remove_if(expression.begin(), expression.end(), isspace), this->expression.end());

  long termIndex = 0;

  //iterate through the string
  for (auto iterator = expression.begin(); iterator != expression.end(); iterator++) {
    auto next = iterator + 1;
    if (*next == '+' || *next == '-' || next == expression.end()) {
      //find the term between the +/- and the previous +/-
      std::string subExpression = std::string(expression.begin() + termIndex, next);
      AlgebraicCurveTerm term{};
      
      bool result = parseTerm(subExpression.begin(), subExpression.end(), term);
      if (result){
        algebraicTerms.push_back(term);
      }

      termIndex = next - expression.begin();
    }
  }
  return algebraicTerms;
}

template<typename Iterator>
bool AlgebraicCurveParser::parseTerm(Iterator first, Iterator last, AlgebraicCurveTerm &term) {
  using boost::spirit::qi::long_long;
  using boost::spirit::qi::phrase_parse;
  using boost::phoenix::ref;
  long long xExponent = 0;
  long long yExponent = 0;
  long long coefficient = 1;
  bool isPositive = true;
  if (this->signPresent(std::string(first, last))){
    isPositive = this -> extractSign(std::string(first, last));
    first++;
  }
  bool r = phrase_parse(first, last,
          // Begin parser
               (
                // For terms
                long_long[ref(coefficient) = boost::spirit::qi::_1]  >>
                 -("x^" >> long_long[ref(
                 xExponent)= boost::spirit::qi::_1] )
                 >>
                 -("y^" >> long_long[ref(
                 yExponent)= boost::spirit::qi::_1])
               |
                 "x^" >> long_long[ref(
                 xExponent) = boost::spirit::qi::_1]  >>
                 -("y^"
                 >> long_long[ref(yExponent)= boost::spirit::qi::_1] )
               |
                  "y^" >> long_long[ref(
                  yExponent) = boost::spirit::qi::_1]  >>
                  -("x^"
                  >> long_long[ref(
                  xExponent) = boost::spirit::qi::_1] )

          )
          ,
               boost::spirit::ascii::space
  );
  if (!r || first != last) return false;
  term.coefficient = coefficient;
  term.xExponent = xExponent;
  term.yExponent = yExponent;
  term.isPositive = isPositive;
  return r;
}

bool AlgebraicCurveParser::extractSign(std::string subExpression) {
    bool positiveSign;
    switch (*subExpression.begin()) {
        case '+':
            positiveSign = true;
            break;
        case '-':
            positiveSign = false;
            break;
        default:
            positiveSign = true;
    }
    return positiveSign;
}

bool AlgebraicCurveParser::signPresent(std::string subExpression) {

    bool signPresent;
    switch (*subExpression.begin()) {
        case '+':
            signPresent = true;
            break;
        case '-':
            signPresent = true;
            break;
        default:
            signPresent = false;
    }
    return signPresent;
}
