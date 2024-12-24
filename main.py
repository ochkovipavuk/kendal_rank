import argparse
import os
import time as t


def compile(name):
    try:
        os.system(f'g++ -o {name}.o {name}.cpp -fopenmp -O0')
    except Exception as e:
        print('### cant compile c++ code')
        print(f'### type of error: {type(e)}')
        print(f'### error message: {str(e)}')
        exit(1)

def check_files(file_list, ext='txt'):
    for file in file_list:
        if not os.path.exists(f'{file}.{ext}'):
            return False
    return True

def read_file(file):
    with open(file, 'r') as f:
        data = f.read().strip().split()
        return [int(num) for num in data]

def main():
    parser = argparse.ArgumentParser()

    parser.add_argument('--lenght', type=int, default=100000, help='lenght of vectors')
    parser.add_argument('--max', type=int, default=9, help='max value in vector')
    parser.add_argument('--min', type=int, default=1, help='min value in vector')
    parser.add_argument('--name', type=str, default='vector', help='vector files name')
    parser.add_argument('--generate', action='store_true', help='generate vectors')
    parser.add_argument('--compare', action='store_true', help='print result of algorithm without parallel')
    parser.add_argument('--check', action='store_true', help='print result of scipy lib')
    parser.add_argument('--compile', action='store_true', help='compile the c++ code with force')

    args = parser.parse_args()

    print(f'### input:\n'
          f' lenght: {args.lenght}\n'
          f' max: {args.max}\n'
          f' min: {args.min}\n'
          f' name: {args.name}\n'
          f' generate: {args.generate}\n'
          f' compare: {args.compare}\n'
          f' check: {args.check}\n'
          f' compile: {args.compile}\n'
          )

    cpp_files = [
        'vec_gen',
        'knd_tau'
    ]

    # preparation code
    code_exist = check_files(cpp_files, 'o')
    if args.compile or not code_exist:
        if not code_exist and not args.compile:
            print('### not all file .o, we need compile')
        print('### compile')
        for file in cpp_files:
            compile(file)

    # generate vectors
    vec_list = []
    for i in range(2):
        vec_list.append(f'{args.name}_{i+1}')
    vec_exist = check_files(vec_list)
    if args.generate or not vec_exist:
        if not vec_exist and not args.generate:
            print('### not all vectors exist')
        print('### generate vectors')
        for vec in vec_list:
            os.system(f'{cpp_files[0]}.o {args.lenght} {args.min} {args.max} {vec}.txt')

    # our parallel knd tau work
    print('\n')
    start_time = t.time()
    os.system(f'{cpp_files[1]}.o {args.name}_1.txt {args.name}_2.txt parallel')
    end_time = t.time()
    print(f'### time: {end_time - start_time}')

    # compare
    if args.compare:
        print('\n')
        start_time = t.time()
        os.system(f'{cpp_files[1]}.o {args.name}_1.txt {args.name}_2.txt sequential')
        end_time = t.time()
        print(f'### time: {end_time - start_time}')

    # check
    if args.check:
        from scipy.stats import kendalltau
        
        print('\n')
        start_time = t.time()
        tau, p_value = kendalltau(read_file(f'{args.name}_1.txt'), read_file(f'{args.name}_2.txt'))
        print(f'### scipy tau: {tau}')
        end_time = t.time()
        print(f'### time: {end_time - start_time}')

    return 0

if __name__ == '__main__':
    main()
