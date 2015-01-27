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


def int operator = (int, int);
struct Point{
	int x, y;
	def int getSth();
	def int foo(int);
	def int foo(){
		x;
	}
};

Point somePoint;
somePoint.foo(10);
somePoint->foo();


struct Vector{
	Point start, end;
};



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


def int operator = (int, int);


Vector v1, v2;
def int Vector.magic(){
start.x = end.x;
}

def void operator = (Point, Point);
Point p3;
p1 = p3;
p1.x = p3.y;

foo(v1.end.x);

struct __Point{
def int foo(int);
def int foo(char);
def int foo();   
};

struct __Vector{
def int foo(int);
def int foo(char);
def int foo(); 

__Point a, b;
};

__Vector __v1, __v2;

__v1.a.foo(10);

__v1.b.foo();
__v1.foo()
__v2.b.foo('a');
__v2.foo('a');

struct UM{
int x, y;
def int foo(){x};
};

