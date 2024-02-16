package main

import (
	"database/sql"
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"

	_ "github.com/lib/pq"
)

const (
	urlFmt   = "<user>:<password>@<host>/<database>[?sslmode=disable]\n"
	testNum  = 3423
	pgPrefix = "postgresql://"
)

func must(err error, msg string) {
	if err == nil {
		return
	}
	fmt.Println(msg+":", err)
	os.Exit(1)
}

var db *sql.DB

func test() error {
	var (
		err     error
		readNum int
		row     *sql.Row
	)
	err = db.Ping()
	if err != nil {
		return fmt.Errorf("Failed to ping database: %w", err)
	}
	_, err = db.Exec(`CREATE TABLE pgtester_temp_table (num int);`)
	if err != nil {
		return fmt.Errorf("Failed to create temporary table")
	}
	_, err = db.Exec(`INSERT INTO pgtester_temp_table VALUES ($1);`, testNum)
	if err != nil {
		return fmt.Errorf("Failed to insert into temporary table")
	}
	row = db.QueryRow(`SELECT num FROM pgtester_temp_table;`)
	err = row.Scan(&readNum)
	if err != nil {
		return fmt.Errorf("Failed to read data from temporary table")
	}
	if readNum != testNum {
		err = fmt.Errorf("Inserted data does not match fetched data (%v != %v)\n", readNum, testNum)
	}
	_, nerr := db.Exec(`DROP TABLE pgtester_temp_table;`)
	if nerr != nil {
		if err == nil {
			err = fmt.Errorf("Failed to delete temporary database")
		} else {
			err = fmt.Errorf("Failed to to delete temporary database, also: %w", err)
		}
	}
	return err
}

func testPrint() {
	err := test()
	if err != nil {
		fmt.Println(err)
	} else {
		fmt.Println("All tests passed")
	}
}

func loop(periodSeconds int) {
	for {
		testPrint()
		time.Sleep(time.Duration(periodSeconds) * time.Second)
	}

}

func main() {
	if len(os.Args) < 2 {
		fmt.Printf("Use:\n\t%v [postgresql://]<database_url> [loop_seconds]\nURL format:\n\t%v", os.Args[0], urlFmt)
		return
	}
	dbURL := pgPrefix + strings.TrimPrefix(os.Args[1], pgPrefix)
	var err error
	db, err = sql.Open("postgres", dbURL)
	if err != nil {
		fmt.Println("Failed to open database:")
		return
	}
	if len(os.Args) > 2 {
		sec, err := strconv.Atoi(os.Args[2])
		if err != nil {
			fmt.Println("Invalid loop period, please give a whole number of seconds")
			return
		}
		loop(sec)
	} else {
		testPrint()
	}
}
