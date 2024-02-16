package main

// gimme hosts a minimal page that lets you upload stuff from the web browser
// directly to the host computer

import (
	"fmt"
	"io"
	"net/http"
	"os"
	"time"
)

var index []byte = []byte(`
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Del filer</title>
</head>
<body>
    <input id="image-file" type="file" />
    <button onclick="upload()">Last opp</button>
    <script type="text/javascript">
        function upload() {
            let photo = document.getElementById("image-file").files[0];
            let formData = new FormData();
            formData.append("file", photo);
            fetch('/upload', {method: "POST", body: formData});
        }
    </script>
</body>
</html>
`)

func handleSite(w http.ResponseWriter, r *http.Request) {
	w.Write(index)
}

func handleUpload(w http.ResponseWriter, r *http.Request) {
	file, header, err := r.FormFile("file")
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		fmt.Println("Error:", err)
		return
	}
	fmt.Println(header.Filename)
	fileContents, err := io.ReadAll(file)
	if err != nil {
		w.WriteHeader(http.StatusInternalServerError)
		fmt.Println("Error:", err)
		return
	}
	fmt.Println(time.Now())
	fmt.Println(len(fileContents))
	os.WriteFile("/tmp/gimme/"+header.Filename, fileContents, 0644)
	//body.
	//	fmt.Println(len(body))
}

func main() {
	http.HandleFunc("/", handleSite)
	http.HandleFunc("/upload", handleUpload)
	fmt.Println("Starting server at port 8888")
	if err := http.ListenAndServe(":8888", nil); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}
