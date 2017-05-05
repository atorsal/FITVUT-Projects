<?php

/**
 * Task: C Header Analysis, CHA
 * @author	Dávid Bolvanský xbolva00
 */

mb_internal_encoding("UTF-8");

// Some constants
const EXIT_OK = 0;
const INVALID_ARGUMENTS = 1;
const INPUT_FILE_OR_DIR_ERROR = 2;
const OUTPUT_FILE_ERROR = 3;

const SPACE = ' ';
const EMPTY_STRING = '';
const NEW_LINE = "\n";
const STAR = '*';

const SWITCHES = ['--help', '--input', '--output', '--remove-whitespace', '--max-par', '--no-inline', '--no-duplicates', '--pretty-xml'];

// Print help with details about skript arguments
function help() {
    echo "Usage of C Header Analysis:\n";
    echo "--help                Prints program help.\n";
    echo "--input=fileordir     Input file or directory to analyze C headers. if not set, the current directory will be used.\n";
    echo "--output=filename     Output XML file. If not set, output will be printed to the stdout.\n";
    echo "--pretty-xml=k        Indents XML elements by k spaces. Default value is 4.\n";
    echo "--no-inline           Skips inline functions.\n";
    echo "--max-par=n           Analyzes only functions with n parameters.\n";
    echo "--no-duplicates       Ignores function duplicates, uses the first occurance.\n";
    echo "--remove-whitespace   Removes whitespaces in function return types.\n";
    exit(EXIT_OK);
}

// Normalize spaces - remove tabs and new lines.
// If remove-whitespace is set, remove whitespaces.
// param text: text to be processed
function normalize_spaces($text) {
    $name = trim($text);
    global $remove_whitespace;
    if ($remove_whitespace) {
        $name = preg_replace('/\s+/u', SPACE, $name);
        $name = preg_replace('/\s(\*+)\s*/u', '\1', $name);
		$name = preg_replace('/\s*(,|\(|\))\s*/u', '\1', $name);
    }
    $name = preg_replace('/[\t]/u', SPACE, $name);
    $name = preg_replace('/[\n]/u', SPACE, $name);
    return $name;
}

// Print msg to error stream and exit script
// param msg: text to print to stderr
// param code: script exit code
function exit_msg($text, $code) {
    file_put_contents('php://stderr', $text . NEW_LINE, FILE_APPEND);
    exit($code);
}

// Process options, support even short ones
// param option: shortened form of option to be processed to a long form
function process_option($option) {
    $option = preg_replace('/^-i$/u', '--input', $option);
    $option = preg_replace('/^-o$/u', '--output', $option);
    $option = preg_replace('/^-h$/u', '--help', $option);
    $option = preg_replace('/^-ni$/u', '--no-inline', $option);
    $option = preg_replace('/^-nd$/u', '--no-duplicates', $option);
    $option = preg_replace('/^-m$/u', '--max-par', $option);
    $option = preg_replace('/^-p$/u', '--pretty-xml ', $option);
    $option = preg_replace('/^-r$/u', '--remove-whitespace', $option);
    $option = trim($option);
    return $option;
}

// Process keys from getopt
// param key: remove dashes from getopt keys
function process_key($key) {
    $key = process_option("-" . $key);
    $key = preg_replace('/^(--|-)(.*)$/u', '\2', $key);
    $key = trim($key);
    return $key;
}

// Initialize global variables with default values
$headers_in_dir = false;
$show_help = false;
$input = "./";
$input_is_file = false;
$output = false;
$params_max = false;
$limit_params = false;
$indent = 4;
$do_indent = false;
$noinline = false;
$noduplicates = false;
$remove_whitespace = false;

// Shortened options
$shortopts  = "";
$shortopts .= "i:";
$shortopts .= "o:";
$shortopts .= "m:";
$shortopts .= "p::";
$shortopts .= "h";
$shortopts .= "r";
$shortopts .= "ni";
$shortopts .= "nd";

// Normal options
$longopts  = array(
    "input:",
    "output:",
    "max-par:",
    "pretty-xml::",
    "help",
    "remove-whitespace",
    "no-inline",
    "no-duplicates",
);

// get options from command line
$options = getopt($shortopts, $longopts);

// Shift argv to remove first arg - script name
array_shift($argv);

$script_opts = [];

// Iterate through all arguments
foreach ($argv as $arg) {
    $argv_parts = explode("=", $arg);
    $option = process_option($argv_parts[0]);

    // Check for duplicated arguments
    if (in_array($option, $script_opts)) {
        exit_msg("Duplicated arguments.", INVALID_ARGUMENTS);
    }
    array_push($script_opts, $option);

    // Check for argument validity
    if (substr($arg, 0, 1 ) == "-") {
        if (!in_array($option, SWITCHES) || preg_match("/^--(help|remove-whitespace|no-inline|no-duplicates)(.+)$/", $arg) == true
            || preg_match("/^--max-par=$/", $arg) == true || preg_match("/^--(input|output|max-par)$/", $arg) == true) {
            exit_msg("Invalid option.", INVALID_ARGUMENTS);
        }
    } else {
        if (!is_dir($arg) && !is_file($arg) && preg_match("/^[0-9]+$/", $arg) == false) {
            exit_msg("Invalid option.", INVALID_ARGUMENTS);
        }
    }

    // Check for empty values
    if (mb_substr($arg, -1) == '=') {
        if (strcmp($option, '--input') == 0) {
            exit_msg("Cannot open input file.", INPUT_FILE_OR_DIR_ERROR);
        }
        else if (strcmp($option, '--output') == 0) {
            exit_msg("Cannot open output file.", OUTPUT_FILE_ERROR);
        } else {
            exit_msg("Missing value for argument.", INVALID_ARGUMENTS);
        }
    }

    // Check for no values
    if (strcmp($arg, '--input') == 0 || strcmp($arg, '--output') == 0 || strcmp($arg, '--max-par') == 0) {
        exit_msg("Missing value for argument.", INVALID_ARGUMENTS);
    }
}

$keys = array_keys($options);

// Iterate trought keys from getopt
foreach ($keys as $key) {
    $value = $options[$key];
    // Normalize key to univerzal form
    $key = process_key($key);
    switch ($key) {
        case 'help':
            $show_help = true;
        break;
        case 'input':
            if ($value == false) {
                exit_msg("Cannot open input file.", INPUT_FILE_OR_DIR_ERROR);
            }
            $input = $value;
        break;
        case 'output':
            if ($value == false) {
                exit_msg("Cannot open output file.", OUTPUT_FILE_ERROR);
            }
            $output = $value;
        break;
        case 'pretty-xml':
            $do_indent = true;

            if (strcmp($value, "0") == 0) {
                $indent = 0;
            } else {
                if ($value != false) {
                    // Check if value is a valid number
                    if (preg_match("/^[0-9]+$/", $value) == false) {
                        exit_msg("Value for pretty-xml is not a valid number.", INVALID_ARGUMENTS);
                    }
                    $indent = $value;
                }
            }
        break;
        case 'no-inline':
            $noinline = true;
        break;
        case 'max-par':
            $limit_params = true;

            if (strcmp($value, "0") == 0) {
                $params_max = 0;
            } else {
                if ($value != false) {
                    // Check if value is a valid number
                    if (preg_match("/^[0-9]+$/", $value) == false) {
                        exit_msg("Value for max-par is not a valid number.", INVALID_ARGUMENTS);
                    }
                    $params_max = $value;
                } else {
                    exit_msg("Option max-par requires n value. ", INVALID_ARGUMENTS);
                }
            }
        break;
        case 'no-duplicates':
            $noduplicates = true;
        break;
        case 'remove-whitespace':
            $remove_whitespace = true;
        break;
        default:
            exit_msg("Invalid option. ", INVALID_ARGUMENTS);
            break;
    }
}

// Get substring of string from start index to end index
function get_substring($str, $start, $end) {
    return mb_substr($str, $start, $end - $start);
}

// Check if help option was used correctly, without other options
if (count($argv) == 1) {
    if ($show_help) {
        help();
    }
} else {
    if ($show_help) {
        exit_msg("Option help cannot be launched with other options.", INVALID_ARGUMENTS);
    }
}

// Checks for input/output files
if (file_exists($input) == false) {
        exit_msg("Input file or dir does not exists.", INPUT_FILE_OR_DIR_ERROR);
    }

if ($output != false) {
    if (is_dir($output) == true || mb_substr($output, -1) == '/') {
            exit_msg("Output file cannot be a directory.", OUTPUT_FILE_ERROR);
    }
    if (file_exists($output) && is_writable($output) == false) {
        exit_msg("Cannot open output file.", OUTPUT_FILE_ERROR);
    }
}


$input_is_file = is_file($input);
if ($input_is_file) {
    if (is_readable($input) == false) {
        exit_msg("Input file is not readable.", INPUT_FILE_OR_DIR_ERROR);
    }
}


// Initialize XML Writter to write our data to xml file
$writer = new XMLWriter();
@$can_open = $writer->openURI($output ? $output : 'php://output');

if ($can_open == false) {
    exit_msg("Cannot open output file.", OUTPUT_FILE_ERROR);
}

if ($do_indent) {
    $writer->startDocument('1.0','UTF-8');
} else {
    $writer->writeRaw('<?xml version="1.0" encoding="UTF-8"?>');
}
$writer->setIndent($do_indent);
$writer->setIndentString(str_repeat(SPACE, $indent));

// Parse file, remove useless code parts, find function headers, write them to XML
// param file: file to be analyzed
function extract_headers_for_file($file) {
    global $remove_whitespace;
    global $headers_in_dir;
    global $noinline;
    global $noduplicates;
    global $input_is_file;
    global $input;
    global $params_max;
    global $limit_params;
    global $writer;

    // read whole file
    $data = file_get_contents($file);
    // remove carriage returns
    $data = preg_replace('/[\r]/', EMPTY_STRING, $data);
    // replace tabs with spaces
    $data = preg_replace('/[\t]/', SPACE, $data);
    // remove block comments
    $data = preg_replace('/(?s)\/\*.*?\*\//u', EMPTY_STRING, $data);
    // remove typedefs and structs
    $data = preg_replace('/\s*(typedef|struct|enum)\s(.*)(?s)\{.*?\}(.*?);/u', EMPTY_STRING, $data);
    // remove escaped quote char
    $data = str_replace('\\\'', EMPTY_STRING, $data);
    $data = preg_replace('/\\\"/u', EMPTY_STRING, $data);
    // remove string literals
    $data = preg_replace('/\"(?s)(.*?)\"/u', EMPTY_STRING, $data);
    $data = preg_replace('/\'(?s)(.*?)\'/u', EMPTY_STRING, $data);
    // Split file content to lines
    $lines = preg_split('/\r\n|\n|\r/', $data);
    $data = EMPTY_STRING;
    $skip_line = false;

    // Scan all lines and remove them if they are not part of function headers
    foreach ($lines as $line) {
        // Detection for multi lines macros or comments
        if (preg_match('/\s*(#|\/\/)(.*)\\\s*/u', $line) == true) {
            $skip_line = true;
            continue;
        }

        if ($skip_line) {
            // whether the previous comment continues
            $skip_line = preg_match('/(.*)\\\s*/u', $line);
            continue;
        }
        // remove comments starting with hash
        $line = preg_replace('/\s*#(.*)/u', EMPTY_STRING, $line);
        // remove one line comments
        $line = preg_replace('/\s*\/\/(.*)/u', EMPTY_STRING, $line);

        // remove commands, variable declarations, brackets
        $line = preg_replace('/^\s*([[:alpha:]_])+\s*\([^\*]+(.*?)\)\s*;/u', EMPTY_STRING, $line);
        $line = preg_replace('/(.*)=(.*?)(;)*/u', EMPTY_STRING, $line);
        $line = preg_replace('/(.*)->(.*?)(;)*/u', EMPTY_STRING, $line);
        $line = preg_replace('/^((?![()]).)+;/u', EMPTY_STRING, $line);

        // normalize line endings for easier regex parsing
        $line = preg_replace('/\)\s*\{\s*$/u', ");",  $line);
        $line = preg_replace('/\)\s*$/u', ");",  $line);

        // remove lines with C keywords
        $line = preg_replace('/(.*)\s(if|else|for|while|switch|do)\s(.*)(\{)*/u', EMPTY_STRING, $line);
        $line = preg_replace('/(.*)\s(default|case)\s(.*):/u', EMPTY_STRING, $line);

        // remove lines with just brackets
        $line = preg_replace('/^\s*(\{|\}|\{\s*\}|;|\((.*?)\)(;)*|\")\s*$/u', EMPTY_STRING, $line);
        $line = preg_replace('/^\s*return/u', EMPTY_STRING, $line);


        // skip empty lines, lines which are not function headers for sure, etc
        if (preg_match('/^\s*$/u', $line) == true || preg_match('/\|\|/u', $line) == true || preg_match('/&&/u', $line) == true ||
            preg_match('/^\s*(\*)*\s*(\w)+\((.*?)\);\s*$/u', $line) == true || preg_match('/(.*?)\((.*?)\)\s*\[(.*)\]\s*;/u', $line) == true) {
            continue;
        }
        $data = $data . $line . NEW_LINE;
    }

    // main regex to parser function header
    $regex = "/(?s)(.*?)\((?s)(.*?)\)\s*;/u";

    preg_match_all($regex, $data, $matches, PREG_OFFSET_CAPTURE);

    // number of found functions
    $size = count($matches[0]);
    $headers_in_file = $size > 0;

    if ($input_is_file) {
        $writer->startElement('functions');
        $writer->writeAttribute('dir', EMPTY_STRING);
    }

    $function_names = [];
    for ($i = 0;$i < $size;$i++) {
        $function = trim($matches[0][$i][0]);
        $header_preffix = trim($matches[1][$i][0]);
        $function_params = trim($matches[2][$i][0]);
        $middle_part = $function_params;
        if (strpos($middle_part, ')') < strpos($middle_part, '(')) {
            // not a function
            continue;
        }

        // check if complex function
        if (mb_substr($middle_part, 0,1) == STAR) {
            // regex to parse complex function
            $regex_adv = "/(([\s\S]*?)\*\s*)(\w(.*?))(\((?s)(.*))\s*(;|(?s)\{.*?\})/u";

            preg_match_all($regex_adv,  $function, $matches2, PREG_OFFSET_CAPTURE);

            $name = $matches2[3][0][0];
            $function_name = trim($name);

            $suffix = trim($matches2[5][0][0]);
            $brackets = 1;

            $suffix_chars = mb_str_split($suffix);
            array_shift($suffix_chars);
            $arg = EMPTY_STRING;
                foreach ($suffix_chars as $char) {
                    if (strcmp($char, '(') == 0) {
                        $brackets++;
                    }
                    if (strcmp($char, ')') == 0) $brackets--;
                    if ($brackets == 0) {
                        break;
                    }
                    $arg = $arg . $char;
                }

            $params_list = '(' . $arg . ')';
            $function_params = normalize_spaces($arg);

            if ($remove_whitespace) {
                $return_type = trim($matches2[1][0][0]);

            } else {
                $return_type = $matches2[1][0][0];
                $return_type .= str_replace($function_name, EMPTY_STRING, $name);
            }
            $return_type .=  $matches2[5][0][0];
            $return_type = str_replace_once($params_list, EMPTY_STRING, $return_type);
            $return_type = normalize_spaces($return_type);
        } else {
            // find position where return type ends
            $pos = strrpos($header_preffix, "*");
            if ($pos === false) {
                $pos = strrpos($header_preffix, NEW_LINE);
                if ($pos === false) {
                    $pos = strrpos($header_preffix, SPACE);
                    if ($pos === false) {
                        continue;
                    }
                }
            }

            $return_type = normalize_spaces(get_substring($header_preffix, 0, $pos + 1));
            $function_name = trim(get_substring($header_preffix, $pos + 1, mb_strlen($header_preffix)));
        }

        // detect if function with vargs
        $has_vargs = mb_substr($function_params, -3) == '...' ? "yes" : "no";

        if (in_array($function_name, $function_names) && $noduplicates) {
            // detected duplicated functions, if no-duplicates is set, skip duplications
            continue;
        }
        array_push($function_names, $function_name);

        // Skip inline functions if no-inline is set
        if (mb_strstr($return_type, "inline") != False && $noinline) {
            continue;
        }

        // Skip functions which has more arguments than maximal value defined using max-par
        if ($limit_params && get_params_count($function_params) > $params_max) {
            continue;
        }

        $return_type = normalize_spaces($return_type);

        if ($input_is_file) {
            $writer->startElement('function');
            $writer->writeAttribute('file', $input);
        } else {
            $filename = basename($file);
            $subdir = str_replace("./", EMPTY_STRING, $input);
            $cwd = realpath(getcwd());
			// remove base dir from path
            $file = str_replace($cwd . "/", EMPTY_STRING, $file);
            $file = str_replace($subdir, EMPTY_STRING, $file);
            $writer->startElement('function');
            $writer->writeAttribute('file', $file);
        }


        $writer->writeAttribute('name', $function_name);
        $writer->writeAttribute('varargs', $has_vargs);
        $writer->writeAttribute('rettype', $return_type);

        parse_params_list($function_params);

        $writer->endElement();

    }

    if ($input_is_file) {
        $writer->endElement();
    }
}

// Split string to char array
// param string: split string to individual chars
function mb_str_split($string)
{
    return preg_split('/(?<!^)(?!$)/u', $string);
}

// Find function arguments and return them
// param params: split string with args list to array of params
function get_function_params($params) {
    $params_chars = mb_str_split($params);
    array_push($params_chars, ','); // zarazka
    $params_parts = [];
    $brackets = 0;
    $param = EMPTY_STRING;
    foreach ($params_chars as $char) {
        if (strcmp($char, '(') == 0) $brackets++;
        if (strcmp($char, ')') == 0) $brackets--;
        if (strcmp($char, ',') == 0) {
            if ($brackets == 0) {
                array_push($params_parts, trim($param));
                $param = EMPTY_STRING;
                continue;
            }
        }
        $param = $param . $char;
    }

    return $params_parts;
}

// Get number of function params
function get_params_count($params) {
    if (empty($params) || strcmp($params, "void") == 0) return 0;
    $has_vargs = mb_substr($params, -3) == '...';
    $params_parts = get_function_params($params);
    $size = count($params_parts);
    if ($has_vargs) $size--;
    return $size;
}

// Replace last substring occurance
function str_last_replace($search, $replace, $subject)
{
    $pos = strrpos($subject, $search);

    if ($pos !== false) {
        $subject = substr_replace($subject, $replace, $pos, mb_strlen($search));
    }

    return $subject;
}

// Replace first substring occurance
function str_replace_once($str_pattern, $str_replacement, $string){

    if (strpos($string, $str_pattern) !== false){
        $occurrence = strpos($string, $str_pattern);
        return substr_replace($string, $str_replacement, strpos($string, $str_pattern), strlen($str_pattern));
    }

    return $string;
}

// Parse function params list
function parse_params_list($params) {
    global $writer;

    // get args for function
    $params_parts = get_function_params($params);
    // get number of params
    $size = get_params_count($params);
    for ($i = 0;$i < $size;$i++) {
        // function argument
        $arg = trim($params_parts[$i]);

        $arg_number = $i+1;

        // search for function pointers in params
        $fun_ptr_found = preg_match_all('/\(\*(.*?)\)/u', $arg, $matches, PREG_OFFSET_CAPTURE);

        // search for arrays in params
        $arr_found = preg_match_all('/(.*?)(\[(.*)\])/u', $arg, $matches2, PREG_OFFSET_CAPTURE);

        // parse if not function pointer
        if ($fun_ptr_found == 0) {
            // find position where param name starts
            $pos = strrpos($arg, "*");
            $pos2 = strrpos($arg, NEW_LINE);
            $pos3 = strrpos($arg, SPACE);
            $pos = max($pos,$pos2,$pos3);
            if ($pos === false) {
                // param has no name
                $pos = mb_strlen($arg);
            }

            if ($arr_found != 0) {
                preg_match_all('/\s*(.*)\[(.*)?]/u', $arg, $matches3, PREG_OFFSET_CAPTURE);
                $arr_arg = trim($matches3[0][0][0]);
                // param name with data type
                $param_name = trim($matches3[1][0][0]);

                preg_match_all('/(.*)\s(.*)\s*/u', $param_name, $matches4, PREG_OFFSET_CAPTURE);

                // param name detection
                if (isset($matches4[2][0][0])) {
                    $param_name = trim($matches4[2][0][0]);
                }

                $arr_no_name = str_last_replace($param_name, EMPTY_STRING, $arr_arg);
                $type = normalize_spaces(str_replace($arr_arg, $arr_no_name, $arg));
            } else {
                $param_name = trim(get_substring($arg, $pos + 1, mb_strlen($arg)));
                // special case if const and struct or enum
                preg_match_all('/const\s+(struct|enum)\s(.*?)\s(.*)/u', $arg, $matches5, PREG_OFFSET_CAPTURE);
                if (isset($matches5[3][0][0])) {
                    $param_name = trim($matches5[3][0][0]);
                    $arg = str_last_replace($param_name, EMPTY_STRING, $arg);
                }
                $type = normalize_spaces(get_substring($arg, 0, $pos + 1));
                if (preg_match('/^(const|const\s*(struct|enum))$/u', $type) == true) {
                    $type = normalize_spaces($arg);
                }
            }

            // special case if param ends with data type, then this is not a param name
            if (preg_match('/(float|char|int|long|short|double|void)/u', $param_name) == true) {
                $type = normalize_spaces($arg);
            }

			// handle struct or enums without param name
			if (preg_match('/^\s*(struct|enum)*\s*(\w|_)+(\[(.*)\])*$/u', $arg) == true) {
                $type = normalize_spaces($arg);
            }
        } else {
            $param_name = trim($matches[1][0][0]); // param name
            // parse function pointer as param
            preg_match_all('/\(\*+(.+?)\)/u', $arg, $matches3, PREG_OFFSET_CAPTURE);
            $fun_ptr = trim($matches3[0][0][0]);
            $fun_ptr_no_name = str_replace($param_name, EMPTY_STRING, $fun_ptr);
            $type = str_replace($fun_ptr, $fun_ptr_no_name, $arg);
            $type = normalize_spaces($type);
        }
        $writer->startElement('param');
        $writer->writeAttribute('number', $arg_number);
        $writer->writeAttribute('type', $type);
        $writer->endElement();
    }
}

// Scan all header files in dir, recursively
if (is_dir($input)) {
    $dirs = new RecursiveDirectoryIterator(realpath($input), RecursiveDirectoryIterator::SKIP_DOTS);
    $iter = new RecursiveIteratorIterator($dirs);

	if (mb_substr($input, -1) != '/') $input = $input . "/";
    $writer->startElement('functions');
    $writer->writeAttribute('dir', $input);

    foreach ($iter as $file) {
        if (pathinfo($file, PATHINFO_EXTENSION) == "h") {
            if (is_readable($file) == false) {
                exit_msg("Input file is not readable.", INPUT_FILE_OR_DIR_ERROR);
            }

            extract_headers_for_file($file);
        }
    }

    $writer->endElement();
}
// Scan headers in file
else {
    extract_headers_for_file($input);
}

$writer->endDocument();
$writer->flush();
unset($writer);

?>
