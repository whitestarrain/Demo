// Server1 is a minimal "echo" server.
package main

import (
	"fmt"
	"log"
	"net/http"
)

func server() {
	http.HandleFunc("/", handler) // each request calls handler
	http.HandleFunc("/svg", svgHandler)
	log.Fatal(http.ListenAndServe("0.0.0.0:8000", nil))
}

// handler echoes the Path component of the request URL r.
func handler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "URL.Path = %q\n", r.URL.Path)
}

func svgHandler(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "image/svg+xml")
	svg(w)
}
