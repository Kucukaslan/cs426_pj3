package main

import(
	"fmt"
	"os"
	"bufio"
	"strconv"
	"flag"

	"github.com/fatih/color"

)

// diff is a simple go file to find mathematical differences between two files
// file names are passed as command line arguments
// each line should be a number
var e = flag.Float64("e", 0.00001, "ignore differences smaller than epsilon")


func main() {
	// define epsilon flag to ignore small differences
	flag.Usage =  func() {
		fmt.Fprintf(flag.CommandLine.Output(), "Usage of %s:\n", os.Args[0])
		fmt.Print("xdiff file1 file2 [flags]\n")
		flag.PrintDefaults()
	}
		
	flag.Parse()

	

	// check for the correct number of arguments
	if len(os.Args) < 3 {
		fmt.Println("Usage: xdiff file1 file2")
		os.Exit(1)
	}

	// open the first file
	file1, err := os.Open(os.Args[1])
	if err != nil {
		fmt.Println("Error: ", err)
		os.Exit(1)
	}

	// open the second file
	file2, err := os.Open(os.Args[2])
	if err != nil {
		fmt.Println("Error: ", err)
		os.Exit(1)
	}

	// close the files when we are done
	defer file1.Close()
	defer file2.Close()

	// create a scanner for each file
	scanner1 := bufio.NewScanner(file1)
	scanner2 := bufio.NewScanner(file2)

	color.Set(color.FgRed, color.Bold)
	// loop through the files
	diffCount := 0
	for scanner1.Scan() && scanner2.Scan() {
		// convert the text to numbers
		num1, err := strconv.ParseFloat(scanner1.Text(), 64)
		if err != nil {
			fmt.Println("Error: ", err)
			os.Exit(1)
		}
		num2, err := strconv.ParseFloat(scanner2.Text(), 64)
		if err != nil {
			fmt.Println("Error: ", err)
			os.Exit(1)
		}

		// print the difference if it is larger than epsilon
		if num1 - num2 > *e || num2 - num1 > *e {
			fmt.Println(num1 - num2, "due ", num1, " - ", num2)
			diffCount++
		}
	}

	color.Set(color.FgGreen, color.Bold)
	if diffCount == 0 {
		fmt.Printf("No differences found between %s and %s that is larger than %f\n", os.Args[1], os.Args[2], *e)
	}

	// check for errors
	if scanner1.Err() != nil {
		fmt.Println("Error: ", scanner1.Err())
		os.Exit(1)
	}

	if scanner2.Err() != nil {
		fmt.Println("Error: ", scanner2.Err())
		os.Exit(1)
	}
	color.Unset() // Don't forget to unset

}
