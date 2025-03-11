import re
import sys

def remove_comments_and_whitespace(code):
    # Remove single line comments
    code = re.sub(r'//.*', '', code)
    # Remove multi-line comments
    code = re.sub(r'/\*.*?\*/', '', code, flags=re.DOTALL)
    # Remove extra whitespace
    code = re.sub(r'\s+', ' ', code)
    return code.strip()

def obfuscate_code(code):
    # Replace variable names with shorter ones
    variables = re.findall(r'\b[a-zA-Z_]\w*\b', code)
    unique_vars = sorted(set(variables), key=lambda x: -len(x))
    var_map = {var: f'v{i}' for i, var in enumerate(unique_vars)}
    for var, obf_var in var_map.items():
        code = re.sub(r'\b' + var + r'\b', obf_var, code)
    return code

def compress_cpp_code(file_path, output_path=None):
    with open(file_path, 'r') as file:
        code = file.read()
    
    code = remove_comments_and_whitespace(code)
    code = obfuscate_code(code)
    
    if output_path is None:
        output_path = file_path.replace('.cpp', '_compressed.cpp')
    with open(output_path, 'w') as file:
        file.write(code)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python compress.py <cpp_file>")
    else:
        compress_cpp_code(sys.argv[1])