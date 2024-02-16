package main

import (
	"fmt"
	"net"
	"os"
	"time"
)

const readBufferSize = 256
const help = `%v starts TCP server on port defined by env var TCPLOG_PORT
(or default port 12893) and logs data written to it.
`

func main() {
	if len(os.Args) > 1 {
		fmt.Printf(help, os.Args[0])
		os.Exit(0)
	}
	port := os.Getenv("TCPLOG_PORT")
	if port != "" {
		port = ":" + port
	} else {
		port = ":12893"
	}
	ln, err := net.Listen("tcp", port)
	if err != nil {
		panic(err)
	}
	for {
		conn, err := ln.Accept()
		if err != nil {
			fmt.Println("Connection failed:", err)
		}
		buffer := make([]byte, readBufferSize)
		var data []byte
		i := readBufferSize
		for i == readBufferSize {
			i, err = conn.Read(buffer)
			if err != nil {
				fmt.Println("Data read failed:", err)
			}
			data = append(data, buffer[:i]...)
		}
		fmt.Println(time.Now())
		fmt.Println("--- BYTES ---")
		fmt.Println(data)
		fmt.Println("-------------")
		fmt.Println()
		fmt.Println("--- TEXT ----")
		fmt.Println(string(data))
		fmt.Println("-------------")
		conn.Write([]byte("OKI"))
		conn.Close()
	}
}
