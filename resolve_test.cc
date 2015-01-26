def int bar(int, char)
def int bar(int, char){}
def int bar(int){}

bar(10)

int a
float b
char c

def int foo(int);
def int foo(float);
def int foo(char);

foo(a);
foo(b);
foo(c);

def int foo(int, char);
def int foo(char, int);

foo(10, 'a');
foo('a', 10);
foo(11, 'a');


def int rand(){}
def void foo(int)
foo(rand())


struct Vector{struct Point{}};

def const ref Vector.Point(Vector.Point);
def Vector create(const ref Vector.Point p1, const ref Vector.Point p2){
}

def Vector create(Vector.Point p1, Vector.Point p2){
}

Vector.Point p1, p2;

def Vector neg(Vector v);

neg(create(p1, p2))


def float(int)
def void magic(float)
magic(rand())

def extern int printf(char ...)
printf('\n');

def extern int printf(int ...)
printf('a', 100, 200, 300, 1.212, 'a')
printf('10');


using def int fType(int);
def fType getFunction(){}
getFunction()(10);






