import sys

def save_file(output_file, integers, reals):
    """ Write to output file: I/O """
    file = open(output_file, "w")
    kk = len(integers[0])
    if kk>0:
        file.write('integer:\n')
    for i in range(kk):
        file.write(str(integers[0][i])+' '+str(integers[1][i])+'\n')
    kk = len(reals[0])
    if kk > 0:
        file.write('real:\n')
    for i in range(kk):
        file.write(str(reals[0][i]) + ' ' + str(reals[1][i]) + '\n')
    file.close()

def read_file(input_file):
    """ Read input file: I/O """
    file = open(input_file, "r")
    text = file.read()
    file.close()
    return text

def find_k_most_integers( k , input_text ):
    """ Find k most integer numbers """
    temp = input_text
    if temp.find(',')!=-1:
        temp = temp.replace(',',' ')
    splitted_numbers = temp.split()
    pure_integers = list(filter(lambda item: test_integer(item), splitted_numbers))
    unique_integers = list(unique_list(pure_integers))
    freqs = list(map(lambda item: freq_count(pure_integers,item),unique_integers))
    parallel_list = list(map(lambda x,y: (x,y) ,unique_integers,freqs))
    after_sort = bubble_sort_tuple(parallel_list)
    final_freqs = list(map (lambda item:(item[1]),after_sort))
    if len(final_freqs)>k:
        freq_lst = bubble_sort_list(unique_list(final_freqs))
        min_freq = min(freq_lst[:k])
        ks = len(list(filter(lambda x: x[1]>=min_freq,after_sort)))
    else:
        ks = k
    final_freqs =list(map (lambda item:(item[1]),after_sort))[:ks]
    final_integers = list(map (lambda item:(item[0]),after_sort))[:ks]
    return (final_integers,final_freqs)

def find_k_most_reals( k , input_text ):
    """ Find k most real numbers """
    temp = input_text
    if temp.find(',') != -1:
        temp = temp.replace(',', ' ')
    splitted_reals = temp.split()
    pure_reals = list(filter(lambda item: test_real(item), splitted_reals))
    unique_reals = list(unique_list(pure_reals))
    freqs = list(map(lambda item: freq_count(pure_reals, item), unique_reals))
    parallel_list = list(map(lambda x, y: (x, y), unique_reals, freqs))
    after_sort = bubble_sort_tuple(parallel_list)
    final_freqs = list(map(lambda item: (item[1]), after_sort))
    if len(final_freqs) > k:
        freq_lst = bubble_sort_list(unique_list(final_freqs))
        min_freq = min(freq_lst[:k])
        ks = len(list(filter(lambda x: x[1] >= min_freq, after_sort)))
    else:
        ks = k
    final_freqs = list(map (lambda item:(item[1]),after_sort))[:ks]
    final_reals = list(map (lambda item:(item[0]),after_sort))[:ks]
    return (final_reals, final_freqs)

def test_integer(s):
    """ Method to verify integer number"""
    try:
        float(s)
        return float(s).is_integer()
    except ValueError:
        return False

def test_real(s):
    """ Method to verify real number"""
    try:
        if float(s) and '.' in str(s) and str(s)[-1]!='.':
            return True
    except ValueError:
        return False

def unique_list(lst):
    new_lst = []
    list(filter(lambda x: new_lst.append(x) if x not in new_lst else None, lst))
    return new_lst

def freq_count(lst,key):
    """ Counts freq. of key in the lst """
    if lst == []:
        return 0
    if lst[0] == key:
        return 1 + freq_count(lst[1:],key)
    else:
        return 0 + freq_count(lst[1:],key)

def bubble_sort_tuple(ar):
    """
    Customized Recursive Bubble Sort for tuples
    Title: Sort Algorithm using BubbleSort without using loops
    Author: devansh24
    Link:  https://www.geeksforgeeks.org/sort-an-array-using-bubble-sort-without-using-loops/
    """
    if len(ar) <= 1:
        return ar
    if len(ar) == 2:
        if ar[0][1] > ar[1][1]:
            return ar
        elif ar[0][1] == ar[1][1]:
            if float(ar[0][0]) < float(ar[1][0]):
                return ar
            else:
                return [ar[1], ar[0]]
        else:
            [ar[1], ar[0]]

    a, b = ar[0], ar[1]
    bs = ar[2:]
    res = []
    if a[1] > b[1]:
        res = [a] + bubble_sort_tuple([b] + bs)
    else:
        if a[1] == b[1]:
            res = [a] + bubble_sort_tuple([b] + bs)
        else:
            res = [b] + bubble_sort_tuple([a] + bs)
    return bubble_sort_tuple(res[:-1]) + res[-1:]

def bubble_sort_list(ar):
    """
    Customized Recursive Bubble Sort for lists
    Title: Sort Algorithm using BubbleSort without using loops
    Author: devansh24
    Link:  https://www.geeksforgeeks.org/sort-an-array-using-bubble-sort-without-using-loops/
    """
    if len(ar) <= 1:
        return ar
    if len(ar) == 2:
        return ar if ar[0] > ar[1] else [ar[1], ar[0]]
    a, b = ar[0], ar[1]
    bs = ar[2:]
    res = []
    if a > b:
        res = [a] + bubble_sort_list([b] + bs)
    else:
        res = [b] + bubble_sort_list([a] + bs)
    return bubble_sort_list(res[:-1]) + res[-1:]

############### APPLICATION ###############
### Read args
if len(sys.argv)>=2 and len(sys.argv[1].split(";"))==3:
    # validate input args
    args = sys.argv[1]
    k = args.split(";")[0].split("=")[1]
    if(not test_integer(int(k))):
        # validate input k
        print("Error! Valid k is required!")
        exit()
    k = int(k)
    input_file = args.split(";")[1].split("=")[1]
    output_file = args.split(";")[2].split("=")[1]
else:
    print("Error! Please correct your arguments as: k=3;input=input.txt;output=freqnumber.txt")
    exit()

### Read input file
try:
    f = open(input_file,'r').readlines()
    input_text = read_file(input_file)
except FileNotFoundError:
    print("Error! Input file not accessible!")
    exit()

### Count Integers
integers = find_k_most_integers(k,input_text)

### Count Reals
reals = find_k_most_reals(k,input_text)

### Save Result
save_file(output_file,integers,reals)
