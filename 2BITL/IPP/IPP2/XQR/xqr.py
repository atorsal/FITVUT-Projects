#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# Project: XML Query (XQR)
# Author: David Bolvansky xbolva00

import re
import os
import xml.dom.minidom as mdom
import sys
import argparse
import codecs


# Print error message if wrong arguments
class ArgsParser(argparse.ArgumentParser):
    def error(self, message):
        sys.stderr.write('Wrong script arguments\n')
        sys.exit(1)

options = ['--help', '--input', '--output', '--query', '--qf', '-n', '--root']

# Handle arguments
parser = ArgsParser(add_help=False)
parser.add_argument('--help', action='store_true', help='Print script help.')
parser.add_argument('--input', help='Input XML file')
parser.add_argument('--output', help='Output XML file')
parser.add_argument('--query', help='Query to be applied at input XML file')
parser.add_argument('--qf', help='File with query to be applied at input XML file')
parser.add_argument('-n', action='store_true', help='XML header in output will not be generated')
parser.add_argument('--root', help='Name of root element')
args = parser.parse_args()

# Handle duplicated options
opts = []
# Prune script name from argv
argv = sys.argv[1:]
for i in argv:
    if '=' in i:
        pos = i.index('=')
        i = i[:pos]

    if not i in options:
        sys.stderr.write('Wrong script arguments\n')
        sys.exit(1)

    if i in opts:
        sys.stderr.write('Duplicated options\n')
        sys.exit(1)

    opts.append(i)

# Handle help option
if args.help != False:
    if len(sys.argv) == 2:
        sys.stdout.write("""XML Query:
        --help                 - Print help
        --input=filename.ext   - Input XML file
        --output=filename.ext  - Output XML file
        --query='query'        - Query to be applied at input XML file
        --qf=filename.ext      - File with query to be applied atinput XML file
        -n                     - Do not generate XML header
        -r=element             - Name of root element\n""")
        sys.exit(0)
    else:
        sys.stderr.write('Help cannot be used with other options\n')
        sys.exit(1)

# Check for duplicated options


# Handle banned combination of options
if args.query != None and args.qf != None:
    sys.stderr.write('--query and --qf cannot be used together\n')
    sys.exit(1)

if args.query == None and args.qf == None:
    sys.stderr.write('--query or --qf not specified\n')
    sys.exit(80)

if args.qf == "":
    sys.stderr.write('--qf is empty\n')
    sys.exit(80)

if args.query == "":
    sys.stderr.write('--query is empty\n')
    sys.exit(80)

if args.root == "":
    sys.stderr.write('--root is empty\n')
    sys.exit(1)

# Open input file
if args.input != None:
    if not os.path.isfile(args.input):
        sys.stderr.write("Not a file.\n")
        sys.exit(2)
    try:
        sys.stdin = codecs.open(args.input, 'r', 'utf-8')
    except:
        sys.stderr.write("Cant open input file.\n")
        sys.exit(2)

# Open output file
if args.output != None:
    try:
        sys.stdout = codecs.open(args.output, 'w+', 'utf-8')
    except:
        sys.stderr.write("Cant open output file.\n")
        sys.exit(3)

# Read query from argument or from file
query_chars = []
if args.qf != None:
    if not os.path.isfile(args.qf):
        sys.stderr.write("Not a file.\n")
        sys.exit(80)

    try:
        f = open(args.qf, "r", encoding='utf-8')
    except:
        sys.stderr.write("Cant open file with query.\n")
        sys.exit(80)
    try:
        data = f.read()
    finally:
        f.close()

    for i in data:
        query_chars.append(i)

if args.query != None:
    for i in args.query:
        query_chars.append(i)

# Constants
EOF = ENDMARK = '$'
HANDLE = '<'
KEYWORDS = ["SELECT", "FROM", "WHERE", "CONTAINS", "NOT", "AND", "OR", "ORDER", "BY", "ROOT", "LIMIT", "ASC", "DESC"]
query_chars_index = 0;
query_chars.append(EOF)

source = mdom.parse(sys.stdin)

# XML root for query
xml_root = None
# Filtered elements
xml_selected_elements = []

# Write XML header to output file
def generateXMLHeader():
    ver = source.version
    if ver is not None:
        sys.stdout.write("<?xml version=\"" + ver + "\"")

    enc = source.encoding
    if enc is not None:
        sys.stdout.write(" encoding=\"" + enc + "\"")

    sta = source.standalone
    if sta is not None:
        sys.stdout.write(" standalone=\"" + ("yes" if sta else "no") + "\"")

    if ver is not None:
        sys.stdout.write("?>" + "\n")


# Precedence analysis
# Stack and methods above it
class PrecedenceStack(object):
    P_NOT = 0
    P_AND = 1
    P_OR = 2
    P_LBRACKET = 3
    P_RBRACKET = 4
    P_CONDITION = 5
    P_ENDMARK = 6
    P_EXPR = 7
    P_HANDLE = 8

    top_term_index = 0

    stack = []

    def __init__(self):
        self.stack = []

    def push(self, token):
        self.stack.append(token)

    def pop(self):
        self.stack.pop()

    def top(self):
        return self.stack[len(self.stack) - 1]

    def size(self):
        return len(self.stack)

    # Get top terminal symbol
    def get_top_term(self):
        top = len(self.stack) - 1;
        while True:
            if top < 0:
                return None
            top_token = self.stack[top]
            if PrecedenceStack.decode_token(top_token) <= PrecedenceStack.P_ENDMARK:
                self.top_term_index = top
                return top_token
            top -= 1

    # Inserts handle into precedence stack
    def insert_handle(self):
        top_term = self.get_top_term()
        i = self.stack.index(top_term)
        self.stack.insert(i + 1, Token(Token.HANDLE, HANDLE))

    # Reduces by rule
    def reduce_by_rule(self):
        top_term = self.get_top_term()

        filtered_xml_elements = []

        if top_term.type == Token.CONDITION:
            condition = top_term.value
            filtered_xml_elements = filter_elements(condition, xml_selected_elements)

        elif top_term.type == Token.AND:
            i = self.top_term_index
            a = self.stack[i - 1].value
            if i + 1 > self.size() - 1:
                sys.stderr.write('Expr: Parsing error\n')
                exit(80)
            b = self.stack[i + 1].value

            filtered_xml_elements = list(set(a) & set(b))

        elif top_term.type == Token.NOT:
            i = self.top_term_index
            if i + 1 > self.size() - 1:
                sys.stderr.write('Expr: Parsing error\n')
                exit(80)
            l = self.stack[i + 1].value
            filtered_xml_elements = list(set(xml_selected_elements) - set(l))

        elif top_term.type == Token.OR:
            i = self.top_term_index
            a = self.stack[i - 1].value
            if i + 1 > self.size() - 1:
                sys.stderr.write('Expr: Parsing error\n')
                exit(80)
            b = self.stack[i + 1].value
            filtered_xml_elements = list(set(a + b))
        elif top_term.type == Token.RIGHT_BRACKET:
            filtered_xml_elements = self.stack[self.top_term_index - 1].value
        else:
            sys.stderr.write('Expr: Parsing error\n')
            exit(80)

        while top_term.type != Token.HANDLE:
            self.stack.pop()
            top_term = self.top()

        self.stack.pop()  # pop handle

        self.push(Token(Token.EXPR, filtered_xml_elements))

    # Decode token for precedence analysis
    def decode_token(token):
        if token.type == Token.NOT:
            return PrecedenceStack.P_NOT
        elif token.type == Token.AND:
            return PrecedenceStack.P_AND
        elif token.type == Token.OR:
            return PrecedenceStack.P_OR
        elif token.type == Token.LEFT_BRACKET:
            return PrecedenceStack.P_LBRACKET
        elif token.type == Token.RIGHT_BRACKET:
            return PrecedenceStack.P_RBRACKET
        elif token.type == Token.CONTAINS:
            return PrecedenceStack.P_CONTAINS
        elif token.type == Token.LESS:
            return PrecedenceStack.P_LESS
        elif token.type == Token.GREATER:
            return PrecedenceStack.P_GREATER
        elif token.type == Token.EQUAL:
            return PrecedenceStack.P_EQUAL
        elif token.type == Token.ENDMARK:
            return PrecedenceStack.P_ENDMARK
        elif token.type == Token.CONDITION:
            return PrecedenceStack.P_CONDITION
        elif token.type == Token.HANDLE:
            return PrecedenceStack.P_HANDLE
        elif token.type == Token.EXPR:
            return PrecedenceStack.P_EXPR
        else:
            return -1


# Filtering logic
def filter_elements(condition, where):
    temp = []
    cond_parts = condition.split(':')
    op = cond_parts[1]
    element = cond_parts[0]
    value = cond_parts[2]
    if op == 'CONTAINS':
        for e in where:
            if '.' in element:
                if element.startswith('.'): # .attribute
                    elem_parts = element.split('.')
                    j = None

                    elements = e.getElementsByTagName('*')
                    for i in elements:
                        if i.hasAttribute(elem_parts[1]):
                            j = i
                            break

                    if e.hasAttribute(elem_parts[1]):
                        j = e

                    if j == None or j.firstChild == None:
                        continue

                    if j.firstChild.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                    attrValue = j.attributes[elem_parts[1]].value

                    if value in attrValue:
                        temp.extend([e])

                else:
                    elem_parts = element.split('.') # element.attribute
                    j = None
                    elements = e.getElementsByTagName(elem_parts[0])
                    for i in elements:
                        if i.hasAttribute(elem_parts[1]):
                            j = i
                            break

                    if e.tagName == elem_parts[0] and e.hasAttribute(elem_parts[1]):
                        j = e

                    if j == None or j.firstChild == None:
                        continue


                    if j.firstChild.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                    attrValue = j.attributes[elem_parts[1]].value

                    if value in attrValue:
                        temp.extend([e])

            else: # element
                j = None
                elems = e.getElementsByTagName(element)
                if elems != []:
                    j = elems[0]

                if e.tagName == element:
                    j = e

                if j == None or j.firstChild == None:
                    continue

                for i in j.childNodes:
                    if i.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                if value in j.firstChild.nodeValue:
                    temp.extend([e])

    elif op == '>':
        for e in where:
            if '.' in element:
                if element.startswith('.'):
                    elem_parts = element.split('.')
                    j = None

                    elements = e.getElementsByTagName('*')
                    for i in elements:
                        if i.hasAttribute(elem_parts[1]):
                            j = i
                            break

                    if e.hasAttribute(elem_parts[1]):
                        j = e

                    if j == None or j.firstChild == None:
                        continue

                    if j.firstChild.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                    try:
                        value = int(value)
                        try:
                            attrValue = float(j.attributes[elem_parts[1]].value)
                        except ValueError:
                            continue
                    except ValueError:
                        attrValue = j.attributes[elem_parts[1]].value

                    if attrValue > value:
                        temp.extend([e])

                else:
                    elem_parts = element.split('.')
                    j = None
                    elements = e.getElementsByTagName(elem_parts[0])
                    for i in elements:
                        if i.hasAttribute(elem_parts[1]):
                            j = i

                    if e.tagName == elem_parts[0] and e.hasAttribute(elem_parts[1]):
                        j = e

                    if j == None or j.firstChild == None:
                        continue

                    if j.firstChild.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                    try:
                        value = int(value)
                        try:
                            attrValue = float(j.attributes[elem_parts[1]].value)
                        except ValueError:
                            continue
                    except ValueError:
                        attrValue = j.attributes[elem_parts[1]].value

                    if attrValue > value:
                        temp.extend([e])


            else:
                j = None
                elems = e.getElementsByTagName(element)
                if elems != []:
                    j = elems[0]

                if e.tagName == element:
                    j = e

                if j == None or j.firstChild == None:
                    continue

                for i in j.childNodes:
                    if i.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                try:
                    value = int(value)
                    try:
                        attrValue = float(j.firstChild.nodeValue)
                    except ValueError:
                        continue
                except ValueError:
                    attrValue = j.firstChild.nodeValue

                if attrValue > value:
                    temp.extend([e])

    elif op == '<':
        for e in where:
            if '.' in element:
                if element.startswith('.'):
                    elem_parts = element.split('.')
                    j = None

                    elements = e.getElementsByTagName('*')
                    for i in elements:
                        if i.hasAttribute(elem_parts[1]):
                            j = i
                            break

                    if e.hasAttribute(elem_parts[1]):
                        j = e

                    if j == None or j.firstChild == None:
                        continue

                    if j.firstChild.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                    try:
                        value = int(value)
                        try:
                            attrValue = float(j.attributes[elem_parts[1]].value)
                        except ValueError:
                            continue
                    except ValueError:
                        attrValue = j.attributes[elem_parts[1]].value

                    if attrValue < value:
                        temp.extend([e])

                else:
                    elem_parts = element.split('.')
                    j = None
                    elements = e.getElementsByTagName(elem_parts[0])
                    for i in elements:
                        if i.hasAttribute(elem_parts[1]):
                            j = i
                            break

                    if e.tagName == elem_parts[0] and e.hasAttribute(elem_parts[1]):
                        j = e

                    if j == None or j.firstChild == None:
                        continue

                    if j.firstChild.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                    try:
                        value = int(value)
                        try:
                            attrValue = float(j.attributes[elem_parts[1]].value)
                        except ValueError:
                            continue
                    except ValueError:
                        attrValue = j.attributes[elem_parts[1]].value

                    if attrValue < value:
                        temp.extend([e])

            else:
                j = None
                elems = e.getElementsByTagName(element)
                if elems != []:
                    j = elems[0]

                if e.tagName == element:
                    j = e

                if j == None or j.firstChild == None:
                    continue

                for i in j.childNodes:
                    if i.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                try:
                    value = int(value)
                    try:
                        attrValue = float(j.firstChild.nodeValue)
                    except ValueError:
                        continue
                except ValueError:
                    attrValue = j.firstChild.nodeValue

                if attrValue < value:
                    temp.extend([e])

    elif op == '=':
        for e in where:
            if '.' in element:
                if element.startswith('.'):
                    elem_parts = element.split('.')
                    j = None

                    elements = e.getElementsByTagName('*')
                    for i in elements:
                        if i.hasAttribute(elem_parts[1]):
                            j = i
                            break

                    if e.hasAttribute(elem_parts[1]):
                        j = e

                    if j == None or j.firstChild == None:
                        continue

                    if j.firstChild.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                    try:
                        value = int(value)
                        try:
                            attrValue = float(j.attributes[elem_parts[1]].value)
                        except ValueError:
                            continue
                    except ValueError:
                        attrValue = j.attributes[elem_parts[1]].value

                    if attrValue == value:
                        temp.extend([e])

                else:
                    elem_parts = element.split('.')
                    j = None
                    elements = e.getElementsByTagName(elem_parts[0])
                    for i in elements:
                        if i.hasAttribute(elem_parts[1]):
                            j = i
                            break

                    if e.tagName == elem_parts[0] and e.hasAttribute(elem_parts[1]):
                        j = e

                    if j == None or j.firstChild == None:
                        continue

                    if j.firstChild.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                    try:
                        value = int(value)
                        try:
                            attrValue = float(j.attributes[elem_parts[1]].value)
                        except ValueError:
                            continue
                    except ValueError:
                        attrValue = j.attributes[elem_parts[1]].value

                    if attrValue == value:
                        temp.extend([e])

            else:
                j = None
                elems = e.getElementsByTagName(element)
                if elems != []:
                    j = elems[0]

                if e.tagName == element:
                    j = e

                if j == None or j.firstChild == None:
                    continue

                for i in j.childNodes:
                    if i.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                try:
                    value = int(value)
                    try:
                        attrValue = float(j.firstChild.nodeValue)
                    except ValueError:
                        continue
                except ValueError:
                    attrValue = j.firstChild.nodeValue

                if attrValue == value:
                    temp.extend([e])
    else:
        sys.stderr.write('Expr: Parsing error\n')
        exit(80)

    return temp


# Token definition - has type and value
class Token(object):
    SELECT = 0
    FROM = 1
    WHERE = 2
    CONTAINS = 3
    NOT = 4
    AND = 5
    OR = 6
    ORDER = 7
    BY = 8
    ROOT = 9
    LIMIT = 10
    ASC = 11
    DESC = 12
    LESS = 13
    GREATER = 14
    EQUAL = 15
    ELEMENT = 16
    ELEMENT_ATTRIBUTE = 17
    ATTRIBUTE = 18
    NUMBER = 19
    STRING = 20
    LEFT_BRACKET = 21
    RIGHT_BRACKET = 22
    EOF = 23
    LEXICAL_ERROR = 24
    CONDITION = 25
    ENDMARK = 26
    HANDLE = 27
    EXPR = 28

    def __init__(self, type, value):
        self.type = type
        self.value = value


# Return loaded char
def ungetc():
    global query_chars_index
    query_chars_index -= 1


# Get next char
def fgetc():
    global query_chars_index
    if query_chars_index >= len(query_chars):
        return EOF
    c = query_chars[query_chars_index]
    query_chars_index += 1
    return c


# Check if id does not contain keywords
def check_keyword(id):
    try:
        return KEYWORDS.index(id)
    except ValueError:
        for keyword in KEYWORDS:
            if keyword in id:
                return Token.LEXICAL_ERROR

        if id.endswith("."):
            return Token.LEXICAL_ERROR

        if id.lower().startswith("xml"):
            return Token.LEXICAL_ERROR

        id_parts = id.split('.')
        if len(id_parts) > 2:
            return Token.LEXICAL_ERROR


        if id.startswith('.'):
            return Token.ATTRIBUTE
        elif '.' in id:
            return Token.ELEMENT_ATTRIBUTE
        else:
            return Token.ELEMENT


# Get token
def get_token():
    state = 0
    data = ''

    while True:
        c = fgetc()

        if c == EOF:
            if state == 0:
                return Token(Token.EOF, data)
            elif state == 4 or state == 6:
                return Token(Token.LEXICAL_ERROR, data);


        # Automaton states
        if state == 7:
            if c == '"' or c == 'n' or c == 't' or c == '\\':
                return Token(Token.LEXICAL_ERROR, data);
            else:
                state = 6

        if state == 6:
            if c == '\'':
                return Token(Token.STRING, data);
            elif c == '\\':
                state = 7
                data += c
            elif c == '\n':
                return Token(Token.LEXICAL_ERROR, data);
            else:
                data += c

        if state == 5:
            if c == '"' or c == 'n' or c == 't' or c == '\\':
                return Token(Token.LEXICAL_ERROR, data);
            else:
                state = 4

        if state == 4:
            if c == '"':
                return Token(Token.STRING, data);
            elif c == '\\':
                state = 5
                data += c
            elif not c.isprintable():
                return Token(Token.LEXICAL_ERROR, data);
            elif c == '\n':
                return Token(Token.LEXICAL_ERROR, data);
            else:
                data += c

        if state == 3:
            if (c.isalnum() or c == '.' or c == '_' or c == '-' or c == ':' or c == '·'):
                data += c
            else:
                ungetc();
                token = check_keyword(data)
                return Token(token, data)

        if state == 2:
            if (c.isdigit()):
                data += c
            else:
                ungetc();
                return Token(Token.NUMBER, data)
        if state == 1:
            if (c.isdigit()):
                data += c
                state = 2
            else:
                ungetc();
                return Token(Token.LEXICAL_ERROR, data)
        if state == 0:
            if (c.isspace() == False):
                if (c == '+' or c == '-'):
                    state = 1
                    data += c
                elif (c.isdigit()):
                    state = 2
                    data += c
                elif (c.isalnum() or c == '.' or c == '_'):
                    state = 3
                    data += c
                elif (c == '"'):
                    state = 4
                elif (c == '\''):
                    state = 6
                elif (c == '>'):
                    data += c
                    return Token(Token.GREATER, data)
                elif (c == '<'):
                    data += c
                    return Token(Token.LESS, data)
                elif (c == '='):
                    data += c
                    return Token(Token.EQUAL, data)
                elif (c == ')'):
                    data += c
                    return Token(Token.RIGHT_BRACKET, data)
                elif (c == '('):
                    data += c
                    return Token(Token.LEFT_BRACKET, data)
                else:
                    data += c
                    return Token(Token.LEXICAL_ERROR, data)


token = None


# Get next token
# If lexical error, exits script
def get_next_token():
    global token
    token = get_token()
    if token.type == Token.LEXICAL_ERROR:
        sys.stderr.write("Lexical error in query.\n")
        sys.exit(80)

# Recursive descent parser
SQL_LIMIT_TOKEN = None
def parse_limit():
    global token
    global SQL_LIMIT_TOKEN

    if token.type == Token.LIMIT:
        get_next_token()
        if token.type == Token.NUMBER:
            if int(token.value) < 0:
                sys.stderr.write('Negative number in limit.\n')
                exit(80)
            SQL_LIMIT_TOKEN = token
            get_next_token()
            if token.type == Token.EOF:
                return True;

    elif token.type == Token.EOF:
        return True

    return False


SQL_ORDER_BY_TOKEN = None
SQL_ORDER_BY_TYPE_TOKEN = None

# Parse oder clause
def parse_order_clause():
    global token
    global SQL_ORDER_BY_TOKEN
    global SQL_ORDER_BY_TYPE_TOKEN

    if token.type == Token.ORDER:
        get_next_token();
        if token.type == Token.BY:
            get_next_token();
            if token.type == Token.ELEMENT or token.type == Token.ELEMENT_ATTRIBUTE or token.type == Token.ATTRIBUTE:
                SQL_ORDER_BY_TOKEN = token
                get_next_token()
                if token.type == Token.ASC or token.type == Token.DESC:
                    SQL_ORDER_BY_TYPE_TOKEN = token
                    get_next_token()
                    return parse_limit()

    elif token.type == Token.EOF:
        return True

    return False


expr_tokens = []

# Parse condition
def parse_condition():
    global token

    if token.type == Token.ELEMENT or token.type == Token.ELEMENT_ATTRIBUTE or token.type == Token.ATTRIBUTE or token.type == Token.LEFT_BRACKET or token.type == Token.NOT:
        while True:
            expr_tokens.append(token)
            if token.type == Token.ORDER:
                break
            elif token.type == Token.LIMIT:
                break
            elif token.type == Token.EOF:
                break
            get_next_token()
        return True

    return False


# Parse where
def parse_where_clause():
    global token

    if token.type == Token.WHERE:
        get_next_token()
        if parse_condition():
            if token.type == Token.ORDER:
                return parse_order_clause()
            elif token.type == Token.LIMIT:
                return parse_limit()
            elif token.type == Token.EOF:
                return True
    elif token.type == Token.EOF:
        return True
    return False


# Parse from
SQL_FROM_TOKEN = None
def parse_from_elem():
    global token
    global SQL_FROM_TOKEN
    get_next_token()

    if token.type == Token.ELEMENT or token.type == Token.ELEMENT_ATTRIBUTE or token.type == Token.ATTRIBUTE or token.type == Token.ROOT:
        SQL_FROM_TOKEN = token
        get_next_token()
        if token.type == Token.WHERE:
            return parse_where_clause()
        elif token.type == Token.ORDER:
            return parse_order_clause()
        elif token.type == Token.LIMIT:
            return parse_limit()
        elif token.type == Token.EOF:
            return True

    elif token.type == Token.WHERE:
        return parse_where_clause()
    elif token.type == Token.ORDER:
        return parse_order_clause()
    elif token.type == Token.LIMIT:
        return parse_limit()
    elif token.type == Token.EOF:
        return True

    return False


# Parse select
SQL_SELECT_TOKEN = None
def parse_query():
    global token
    global SQL_SELECT_TOKEN
    get_next_token()

    if token.type == Token.SELECT:
        get_next_token()
        if token.type == Token.ELEMENT:
            SQL_SELECT_TOKEN = token
            get_next_token()
            if token.type == Token.FROM:
                return parse_from_elem()

    return False


# Parser query
parse_ok = parse_query()
if parse_ok == False:
    sys.stderr.write('Parsing error\n')
    exit(80)

# Find root
if SQL_FROM_TOKEN == None:
    if args.n == False:
        generateXMLHeader()

    if args.root != None:
        sys.stdout.write("<" + args.root + "/>")

    sys.stdout.write("\n")
    exit(0);

if SQL_FROM_TOKEN.type == Token.ROOT:
    xml_root = source.documentElement
elif SQL_FROM_TOKEN.type == Token.ELEMENT:
    data_from = source.getElementsByTagName(SQL_FROM_TOKEN.value)
    if data_from != []:
        xml_root = data_from[0]
elif SQL_FROM_TOKEN.type == Token.ELEMENT_ATTRIBUTE:
    data_from = SQL_FROM_TOKEN.value.split('.')
    elements = source.getElementsByTagName(data_from[0])
    for e in elements:
        if e.hasAttribute(data_from[1]):
            xml_root = e
            break
elif SQL_FROM_TOKEN.type == Token.ATTRIBUTE:
    data_from = SQL_FROM_TOKEN.value.split('.')
    elements = source.getElementsByTagName('*')
    for e in elements:
        if e.hasAttribute(data_from[1]):
            xml_root = e
            break

# Select from root
if SQL_SELECT_TOKEN.value == source.documentElement.tagName and SQL_FROM_TOKEN.type == Token.ROOT:
    xml_selected_elements = [source.documentElement]
else:
    if xml_root != None:
        selected = xml_root.getElementsByTagName(SQL_SELECT_TOKEN.value)
        xml_selected_elements.extend(selected)

# Check for semantic errors in conditions
precedence_analysis_tokens = []
for i in expr_tokens:
    if i.type == Token.ELEMENT or i.type == Token.ELEMENT_ATTRIBUTE or i.type == Token.ATTRIBUTE:
        index = expr_tokens.index(i)

        if expr_tokens[index + 1].type == Token.CONTAINS:
            if expr_tokens[index + 2].type != Token.STRING:
                sys.stderr.write('Error: Not a string in contains condition\n' + str(expr_tokens[index + 2].type))
                exit(80)
        elif expr_tokens[index + 1].type == Token.LESS or expr_tokens[index + 1].type == Token.GREATER or expr_tokens[index + 1].type == Token.EQUAL:
            if expr_tokens[index + 2].type != Token.STRING and expr_tokens[index + 2].type != Token.NUMBER:
                sys.stderr.write('Error: Bad condition\n')
                exit(80)
        else:
            sys.stderr.write('Error: Bad condition\n')
            exit(80)

        if index + 2 >= len(expr_tokens):
            sys.stderr.write('Parsing error\n')
            exit(80)

        a = expr_tokens[index].value + ":" + expr_tokens[index + 1].value + ":" + expr_tokens[index + 2].value

        precedence_analysis_tokens.append(Token(Token.CONDITION, a))

    if i.type == Token.AND or i.type == Token.OR or i.type == Token.NOT or i.type == Token.LEFT_BRACKET or i.type == Token.RIGHT_BRACKET:
        precedence_analysis_tokens.append(i)

# Preparation for precedence analysis
precedence_analysis_tokens.append(Token(Token.ENDMARK, ENDMARK))

P_STACK = PrecedenceStack()
P_STACK.push(Token(Token.ENDMARK, ENDMARK))
precedence_analysis_tokens_index = 0


# Get token from array of tokens in the expression (ie. condition in WHERE)
def get_token_expr():
    global precedence_analysis_tokens_index
    token = precedence_analysis_tokens[precedence_analysis_tokens_index]
    precedence_analysis_tokens_index += 1
    return token


# Precedence table
# Row - stack top
# Column - input token
#         NOT  AND   OR   (    )    C    $
table = [['<', '>', '>', '<', '>', '<', '>'],  # NOT
         ['<', '>', '>', '<', '>', '<', '>'],  # AND
         ['<', '>', '>', '<', '>', '<', '>'],  # OR
         ['<', '<', '<', '<', '=', '<', ''],  # (
         ['>', '>', '>', '', '>', '', '>'],  # )
         ['', '>', '>', '', '>', '', '>'],  # C
         ['<', '<', '<', '<', '', '<', '']]  # $

# Precedence analysis
expr_token = get_token_expr()
while True:
    top_term = P_STACK.get_top_term()
    if expr_token.type == Token.ENDMARK and top_term.type == Token.ENDMARK:
        break

    s = table[PrecedenceStack.decode_token(top_term)][PrecedenceStack.decode_token(expr_token)]

    if s == "=":
        P_STACK.push(expr_token)
        expr_token = get_token_expr()
    elif s == '<':
        P_STACK.insert_handle()
        P_STACK.push(expr_token)
        expr_token = get_token_expr()
    elif s == '>':
        P_STACK.reduce_by_rule()
    else:
        sys.stderr.write('Error in condition\n')
        exit(80)

# Get final filtered elements by conditions
if P_STACK.top().value != ENDMARK:
    xml_selected_elements = P_STACK.top().value

# Sort elements
if SQL_ORDER_BY_TOKEN != None and xml_selected_elements != []:
    for t in xml_selected_elements:
        data = SQL_ORDER_BY_TOKEN.value.split('.')
        if SQL_ORDER_BY_TOKEN.type == Token.ELEMENT_ATTRIBUTE:
            elements = t.getElementsByTagName(data[0])
            j = None
            for e in elements:
                data = SQL_ORDER_BY_TOKEN.value.split('.')
                if e.hasAttribute(data[1]):
                    j = e
                    break

            if t.tagName == data[0] and t.hasAttribute(data[1]):
                if t.firstChild.nodeType != t.TEXT_NODE:
                    sys.stderr.write('Element node type is not text\n')
                    exit(4)
                elements = t.parentNode.getElementsByTagName(data[0]);
                xml_selected_elements.sort(
                    key=lambda elements: elements.attributes[data[1]].value)
                break

            if j == None:
                sys.stderr.write('Cant find element for sorting\n')
                exit(4)

            if j.firstChild.nodeType != j.TEXT_NODE:
                sys.stderr.write('Element node type is not text\n')
                exit(4)

            xml_selected_elements.sort(
                key=lambda xml_selected_elements: xml_selected_elements.getElementsByTagName(data[0])[0].attributes[data[1]].value)
            break

        elif SQL_ORDER_BY_TOKEN.type == Token.ATTRIBUTE:
            elements = t.getElementsByTagName('*')
            j = None

            for e in elements:
                if e.hasAttribute(data[1]):
                    j = e
                    break

            if t.hasAttribute(data[1]):
                if t.firstChild.nodeType != t.TEXT_NODE:
                    sys.stderr.write('Element node type is not text\n')
                    exit(4)
                elements = t.parentNode.getElementsByTagName('*');
                xml_selected_elements.sort(
                    key=lambda elements: elements.attributes[data[1]].value)
                break

            if j == None:
                sys.stderr.write('Cant find element for sorting\n')
                exit(4)

            if j.firstChild.nodeType != j.TEXT_NODE:
                sys.stderr.write('Element node type is not text\n')
                exit(4)

            xml_selected_elements.sort(
                key=lambda xml_selected_elements: xml_selected_elements.getElementsByTagName('*')[0].attributes[data[1]].value)
            break
        elif SQL_ORDER_BY_TOKEN.type == Token.ELEMENT:
            if xml_selected_elements != []:
                if t.tagName == SQL_ORDER_BY_TOKEN.value:
                    if t.firstChild.nodeType != t.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)
                    elements = t.parentNode.getElementsByTagName(data[0]);
                    xml_selected_elements.sort(
                        key=lambda elements: elements.firstChild.nodeValue)
                    break

                elements = t.getElementsByTagName(SQL_ORDER_BY_TOKEN.value)
                if elements == []:
                    sys.stderr.write('Cant find element for sorting\n')
                    exit(4)
                j = elements[0]

                for i in j.childNodes:
                    if i.nodeType != j.TEXT_NODE:
                        sys.stderr.write('Element node type is not text\n')
                        exit(4)

                xml_selected_elements.sort(
                    key=lambda xml_selected_elements: xml_selected_elements.getElementsByTagName(SQL_ORDER_BY_TOKEN.value)[0].firstChild.nodeValue)
                break

    if SQL_ORDER_BY_TYPE_TOKEN.type == Token.DESC:
        xml_selected_elements = list(reversed(xml_selected_elements))
    for e in xml_selected_elements:
        i = xml_selected_elements.index(e) + 1
        e.setAttribute("order", str(i))


# Generate XML header if option -n is not set
if args.n == False:
    generateXMLHeader()

# Limit results
if xml_selected_elements != []:
    if SQL_LIMIT_TOKEN != None:
        limit = int(SQL_LIMIT_TOKEN.value)
        xml_selected_elements = xml_selected_elements[:limit]

# Print result elements
if xml_selected_elements != []:
    if args.root != None:
        sys.stdout.write("<" + args.root + ">" + "\n")
    for f in xml_selected_elements:
        sys.stdout.write(f.toxml())
    if args.root != None:
        sys.stdout.write("\n" + "</" + args.root + ">")
else:
    if args.root != None:
        sys.stdout.write("<" + args.root + "/>")

sys.stdout.write("\n")
