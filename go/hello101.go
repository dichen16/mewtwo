package main

import (
	"fmt"
	"math"
)

// simple function
func calc(num1 int, num2 int) (int, int) {
	sum := num1 + num2
	diff := num1 - num2
	return sum, diff
}

type Vertex struct {
	X float64
	Y float64
}

// Go does not have classes. However, you can define methods on types.
// A method is a function with a special receiver argument.
// Remember: a method is just a function with a receiver argument. Methods are functions
func (v Vertex) Abs() float64 {
	return math.Sqrt(v.X*v.X + v.Y*v.Y)
}

func sum_func(s []int, c chan int) {
	localsum := 0
	for _, v := range s { //we didn’t need the index, so we ignored it with the blank identifier _.
		localsum += v
	}
	c <- localsum // send sum to c
}

func main() {
	fmt.Println("Hello World! This is my first Go program")
	// ":=" A short variable declaration uses this syntax
	a, b := 15, 10

	// function call
	sum, diff := calc(a, b)

	// pointers
	p := &sum
	fmt.Println("Sum is", *p)
	// point to another object
	p = &diff
	fmt.Println("Diff is", *p)

	// create a object
	v := Vertex{3, 4}
	fmt.Println("x is", v.X)
	fmt.Println("Abs is", v.Abs())

	s := []int{7, 2, 8, -9, 4, 0} // The type [n]T is an array of n values of type T.

	c := make(chan int) // Like maps and slices, channels must be created before use.

	go sum_func(s[:len(s)/2], c)
	go sum_func(s[len(s)/2:], c)

	x, y := <-c, <-c // receive from c

	fmt.Println(x, y, x+y)
}
