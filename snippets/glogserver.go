package main

// glogserver simply takes any request and prints it.
// Often useful for debugging stuff.

import (
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
)

func main() {
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		b, err := ioutil.ReadAll(r.Body)
		if err != nil {
			log.Print("error:", err)
		}
		defer r.Body.Close()
		log.Println("Request from", r.RemoteAddr)
		log.Printf("%v %v | Headers: %v\n", r.Method, r.RequestURI, r.Header)
		if len(b) > 0 {
			log.Println("-<body>-----------------------------")
			log.Println(string(b))
			log.Println("------------------------------------")
		}
	})
	port := ":3000"
	if len(os.Args) == 2 {
		port = fmt.Sprintf(":%v", os.Args[1])
	}
	log.Println("Starting server at", port)
	log.Fatal(http.ListenAndServe(port, nil))
}
