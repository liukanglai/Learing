#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# comment
'''
"""
jajgjgja
"""
'''

# variable

# int (no limit for size)
NUMBER = 1_000_000_000
HEXO = 0xabc3_abc

# float (no limit for size, but there is a sign of 'inf' to display infinite)
FLOAT = -1.23
FLOAT = 1.23e9
FLOAT = 1.23e-5

# String
STRING = 'abc'
STRING = "abc"
STRING = "a'bc"
STRING = "a\'b\"c\""
STRING = '1'

NUM = int(STRING)

# I/O

print(10 + 10)
print('10 + 10 =', 10 + 10)
print('hell', 'j', "jslg")  # , will be space

name = input('please enter your name: ')
print('hello:', name)

birthday = input('birth: ')
print(birthday)
"""
# format
print('Hello, %s' % 'world')
print('Hello, %s, %d' % ('world', 100))  # %d %f %s %x(0x int) %%(no \%)

number = 19.2
'Hello, {0}, 成绩提升了 {1:.1f}%'.format('小明', number)
print(f'The area of a circle with radius {name} is {number:.2f}')

print("\n, \t, \\")
print(r"\n, \t, \\")

print('''line0
line1
line2 ''')  # no print(r''' ''')

# Python 2：None, 0, 和空字符串都被算作 False，其他的均为 True
# Python 3：None，0，空字符串，空列表，空字典都算是False，所有其他值都是True
print(3 > 2)  # boolean
bool0 = False  # and or not

no = None
no = 1  # dynamic language
no = 'a'
print(no)

# constant
PI = 3.14159265359

# character (Unicode)
print('包含中文的str')

X = b'ABC'  # (bytes)
'ABC'.encode('ascii')
'中文'.encode('utf-8')
b'ABC'.decode('ascii') == 'ABC'
b'\xe4\xb8\xad\xe6\x96\x87'.decode('utf-8') == '中文'
b'\xe4\xb8\xad\xff'.decode('utf-8', errors='ignore')
len('ABC') == 3
len('中文') == 2
len('中文'.encode('utf-8')) == 6

# data
classmates = ['Cong', 'Yuan', 'Che']  # (list)
len(classmates)
classmates[0] = 'Hao'
classmates[-1]  # the end !!! so -2, -3 ...

classmates.append('Lang')
classmates.insert(0, 'Lai')
classmates.pop()  # delete end
classmates.pop(3)

classmates = ['people', 123, True]
classmates = ['people', [123, True]]
classmates[1][1]
L = []
len(L) == 0

# tuple, can't be changed, safe, so use it as long as possible.
classmates = ('Cong', 'Yuan', 'Lang')  # changable tuple ???

atuple = (1)  # it is int 1 !
atuple = (1, )

# dict dictionary map use key-value (structure)
dic = {'Cong': 80, 'Lang': 95, 'Yuan': 90}
print(dic['Yuan'])
if 'Cong' in dic:
    dic['Cong'] = 90

if dic.get('Yuan'):  # dic.get('Lang', -1), return what you like.
    dic['Yuan'] = 88

dic.pop('Cong')
print(dic)
dic = {'Cong', 'Lang', 'Yuan'}
print(dic)
dic = {(1, 2): 1}
# dic = {(1, 2, [1, 2]): 1}

# 查找和插入的速度极快，不会随着key的增加而变慢；
# 需要占用大量的内存，内存浪费多。
# Hash need key can't change, so the key is the fixed variable.

# set (only key, no repetition)
alist = [1, 2, 3]
classSet0 = set(alist)
print(classSet0)
alist = (1, 2, 3)
classSet0 = set(alist)
print(classSet0)
classSet1 = set([1, 1, 1, 2, 2, 3])
print(classSet1)

classSet1.add(1)
classSet1.add(0)
classSet1.remove(0)

classSet0 & classSet1
classSet0 | classSet1

# about the changable
testList = ['b', 'c', 'a']
testList.sort()
print(testList)

testStr = 'abc'
testStr.replace('a', 'A')

# list is changable, while 'abc'(String) can't change, thought testStr changed.
# list/tuple is sequential link-list, while dict(set) is Hash map which use key to map value, so the key must fixed.

# operation
print(10 / 3)
print(10 // 3)
print(10 % 3)
ord('中')
chr(66)
print('\u4e2d\u6587')

# control-flow

number = 100
if number >= 0:  # must indent
    print(number)
else:
    print(-number)

if number > 0:
    print(number)
elif number > 100:
    print(number + number)
else:
    print(number + number + number)

# Loop
for mate in classmates:
    print(mate)
sum = 0
for x in list(range(101)):
    sum = sum + x
print(sum)

number = 100
while number > 0:
    if number > 10:
        break
        continue
    sum = sum + number
    number = number - 2
print(sum)
"""
